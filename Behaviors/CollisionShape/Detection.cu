#include <iostream>
#include "../CollisionShape/CollisionShape.h"
#include "./Detection.cuh"

typedef struct Vec2 {
    double x, y;
} Vec2;

__device__ bool cmpVec(Vec2* first, Vec2* other)
{
    const float TOLERANCE = 0.0001;
    auto a = std::abs(first->x - other->x) < TOLERANCE;
    auto b = std::abs(first->y - other->y) < TOLERANCE;
    return a && b;
}

struct CollisionDetail {
    bool collision = false;
    double leastOverlap = 1000000000;
    double greatestOverlap = 0.f;
    Vec2 leastAxis{};
};

typedef struct CPoint {
    float dist{};
    Vec2 point{};
    bool contains{};
} CPoint;

__device__ void getLine(Vec2 p1, Vec2 p2, double &a, double &b, double &c)
{
    // (x- p1X) / (p2X - p1X) = (y - p1Y) / (p2Y - p1Y)
    a = p1.y - p2.y;
    b = p2.x - p1.x;
    c = p1.x * p2.y - p2.x * p1.y;
}

__device__ CPoint dist(Vec2 p, Vec2 start, Vec2 end)
{
    Vec2 edge{end.x - start.x, end.y - start.y};
    Vec2 pointDir{p.x - start.x, p.y - start.y};
    auto proj = pointDir.x * edge.x + pointDir.y * edge.y;
    auto contains = proj >= 0 && proj <= (edge.x * edge.x + edge.y * edge.y);

    double a, b, c;
    // ax + by + c = 0

    getLine(start, end, a, b, c);

    auto distance = (float)(abs(a * p.x + b * p.y + c) / sqrt(a * a + b * b));
    // Calculate the coordinates of the closest point on the line
    auto closestPointX = (float)((b * (b * p.x - a * p.y) - a * c) / (a * a + b * b));
    auto closestPointY = (float)((a * (-b * p.x + a * p.y) - b * c) / (a * a + b * b));

    return {distance, {closestPointX, closestPointY}, contains};
}

__device__ void findMinDistance(
        Vec2** globals,
        const unsigned int sizes[2],
        bool inverse,
        Vec2* contact1,
        Vec2* contact2,
        float* minDistSq,
        int* contactCount
)
{
    auto first = 0;
    auto second = 1;
    if (inverse)
    {
        first = 1;
        second = 0;
    }
    auto globalA = globals[first];
    auto globalB = globals[second];

    auto verticesA = sizes[first];
    auto verticesB = sizes[second];

    for (int i = 0; i < verticesA; i++) {
        auto vertexA = globalA[i];

        for (int j = 0; j < verticesB; j++) {
            auto bStart = globalB[j];
            auto bEnd = globalB[(j + 1) % verticesB];

            auto cp = dist(vertexA, bStart, bEnd);

            if (!cp.contains)
                continue;

            if (abs(cp.dist - *minDistSq) < 0.0001)
            {
                if (cmpVec(&cp.point, contact1)) continue;

                *contact2 = cp.point;
                *contactCount = 2;
            }
            else if (cp.dist < *minDistSq)
            {
                *minDistSq = cp.dist;
                *contactCount = 1;
                *contact1 = cp.point;
            }
        }
    }
}

__device__ CollidingPoints findContactPoints(
        Vec2** globals,
        unsigned int sizes[2]
)
{
    auto contact1 = Vec2();
    auto contact2 = Vec2();
    auto contactCount = 0;

    float minDistSq = INT8_MAX;

    findMinDistance(
            globals,
            sizes,
            false,
            &contact1,
            &contact2,
            &minDistSq,
            &contactCount
    );

    findMinDistance(
            globals,
            sizes,
            true,
            &contact1,
            &contact2,
            &minDistSq,
            &contactCount
    );

    return CollidingPoints{*(Vector2*)(&contact1), *(Vector2*)(&contact2), contactCount};
}

__device__ void getProjection(Vec2 axis, Vec2* vertices, unsigned int length, Vec2* result)
{
    auto vertex = vertices[0];
    double min = axis.x * vertex.x + axis.y * vertex.y;
    double max = min;
    for (int i = 1; i < length; i++) {
        vertex = vertices[i];
        double p = axis.x * vertex.x + axis.y * vertex.y;
        if (p < min) {
            min = p;
        } else if (p > max) {
            max = p;
        }
    }
    result->x = min;
    result->y = max;
}

__global__ void getCollisionAxis(
        Vec2* axes,
        Vec2* vertices1,
        unsigned int size1,
        Vec2* vertices2,
        unsigned int size2,
        CollisionDetail* details
        )
{
    auto axis = axes[threadIdx.x];
    auto detail = &details[threadIdx.x];

    Vec2 p1;
    getProjection(axis, vertices1, size1, &p1);
    Vec2 p2;
    getProjection(axis, vertices2, size2, &p2);

    bool overlap = p1.x < p2.y && p2.x < p1.y;
    if (!overlap) {
        detail->collision = false;
    }
    else {
        detail->collision = true;
        double minOverlap = max(p1.x, p2.x);
        double maxOverlap = min(p1.y, p2.y);
        Vec2 newOverlap = {minOverlap, maxOverlap};
        auto nO = abs(newOverlap.x - newOverlap.y);

        detail->leastOverlap = nO;
        auto a = axis;
        if (p1.y > p2.y)
            a.y *= -1;
        if (p1.x > p2.x)
            a.x *= -1;

        detail->greatestOverlap = nO;
        detail->leastAxis = a;
    }
}

typedef struct CudaShape {
    unsigned int pointCount;
    Vec2* points;
    float* transform;
    ShapeType type;
} CudaShape;

__global__ void getGlobalVertices(CudaShape* shapes, const unsigned int sizes[2], Vec2 offset, Vec2** vertices)
{
    auto i = blockIdx.x;
    auto s = shapes[i];
    if (threadIdx.x >= sizes[i])
        return;
    auto vertex = s.points[threadIdx.x];
    auto matrix = s.transform;

    Vec2 res;
    res.x = (matrix[0] * vertex.x) + (matrix[4] * vertex.y) + matrix[12];
    res.y = (matrix[1] * vertex.x) + (matrix[5] * vertex.y) + matrix[13];

    if (i == 0)
    {
        res.x += offset.x;
        res.y += offset.y;
    }

    vertices[i][threadIdx.x].x = res.x;
    vertices[i][threadIdx.x].y = res.y;
}

__global__ void getAxes(Vec2** vertices, const unsigned int sizes[2], Vec2** axes)
{
    const unsigned int i = blockIdx.x;
    auto v = vertices[i];
    unsigned int length = sizes[i];
    const unsigned int j = threadIdx.x;

    //Getting vertex
    auto v1 = v[j];
    //Getting other vertex
    auto v2 = v[j+1 == length ? 0 : j+1];

    Vec2 edge;
    edge.x = v1.x - v2.x;
    edge.y = v1.y - v2.y;

    Vec2 normal;
    normal.x = -edge.y;
    normal.y = edge.x;

    double l = sqrt(normal.x * normal.x + normal.y * normal.y);
    normal.x /= l;
    normal.y /= l;

    //Normalizing to get accurate projections
    axes[i][j] = normal;
}

__global__ void getFinalDetail(Colliding res[2], CollisionDetail* details, unsigned int length, unsigned int index)
{
    CollisionDetail final;
    final.collision = true;

    for (int i = 0; i < length; i++) {
        auto detail = details[i];

        if (!detail.collision)
        {
            final.collision = false;
            break;
        }

        if (detail.leastOverlap < final.leastOverlap)
        {
            final.leastOverlap = detail.leastOverlap;
            final.leastAxis = detail.leastAxis;
        }
        if (detail.greatestOverlap > final.greatestOverlap)
            final.greatestOverlap = detail.greatestOverlap;
    }

    res[index].collision = final.collision;
    res[index].penetration = final.greatestOverlap;
    res[index].overlap = final.leastOverlap;
    res[index].normal.x = final.leastAxis.x;
    res[index].normal.y = final.leastAxis.y;
}

__global__ void getCollision(Vec2** axes, Vec2** vertices, unsigned int sizes[2], Colliding res[2])
{
    const unsigned int index = blockIdx.x;
    auto a = axes[index];
    auto length = sizes[index];
    auto details = (CollisionDetail*)malloc(sizeof(CollisionDetail) * length);
    getCollisionAxis<<<1, length>>>(a, vertices[0], sizes[0], vertices[1], sizes[1], details);

    getFinalDetail<<<1,1>>>(res, details, length, index);
}

__global__ void done(Vec2** globals, unsigned int sizes[2], Colliding* cs, unsigned int i, Colliding* results)
{
    auto c1 = cs[0];
    auto c2 = cs[1];
    auto colliding = c1.collision && c2.collision;

    auto obj = (Colliding*)malloc(sizeof(Colliding));
    obj->collision = colliding;

    obj->overlap = c1.overlap;
    obj->normal = c1.normal;

    obj->penetration = c1.penetration;

    obj->collidingPoints = findContactPoints(globals, sizes);

    results[i] = *obj;

    free(obj);
}

__global__ void freeAll(Vec2** axes, Vec2** globals, unsigned int sizes[2], CudaShape* shapes, Colliding* cs)
{
    free(axes);
    free(globals);
    free(sizes);
    free(shapes);
    free(cs);
}

__global__ void checkContinuous(int m, Vector2 direction, CudaShape first, CudaShape second, Colliding* results)
{
    auto i = threadIdx.x;
    float j = (float)i / (float)m;

    //auto pos = startPos + j * direction;
    Vec2 pos{0,0};

    pos.x += j * direction.x;
    pos.y += j * direction.y;

    /*for (int k = 0; k < first.pointCount; ++k) {
        first.points[k].x += (float)pos.x;
        first.points[k].y += (float)pos.y;
    }*/

    //satCollision(ShapeType::RECTANGLE, &first, &second, colliding);

    if (first.type == CIRCLE && second.type == CIRCLE)
    {
        printf("Circle\n");
        return;
    }
    else if (first.type == RECTANGLE && second.type == RECTANGLE)
    {
        auto maxSize = max(first.pointCount, second.pointCount);
        auto sizes = new unsigned int[2];
        sizes[0] = first.pointCount;
        sizes[1] = second.pointCount;

        Vec2** globals = (Vec2**)malloc(sizeof(Vec2*) * 2);
        globals[0] = (Vec2*)malloc(sizeof(Vec2) * first.pointCount);
        globals[1] = (Vec2*)malloc(sizeof(Vec2) * second.pointCount);

        auto shapes = new CudaShape[2];
        shapes[0] = first;
        shapes[1] = second;
        getGlobalVertices<<<2, maxSize>>>(shapes, sizes, pos, globals);

        //Get axes
        Vec2** axes = (Vec2**)malloc(sizeof(Vec2*) * 2);
        axes[0] = (Vec2*)malloc(sizeof(Vec2)*first.pointCount);
        axes[1] = (Vec2*)malloc(sizeof(Vec2)*second.pointCount);
        getAxes<<<2, maxSize>>>(globals, sizes, axes);


        auto cs = (Colliding*)malloc(sizeof(Colliding) * 2);
        getCollision<<<2,1>>>(axes, globals, sizes, cs);

        done<<<1,1>>>(globals, sizes, cs, threadIdx.x, results);

        freeAll<<<1, 1>>>(axes, globals, sizes, shapes, cs);
    }
}

Colliding checkContinuousWrapper(Vector2 startPos, Vector2 direction, sf::Shape* s1, sf::Shape* s2) {
    int device = 0;
    cudaDeviceProp deviceProp{};
    cudaGetDeviceProperties(&deviceProp, cudaGetDevice(&device));

    if(!(deviceProp.major > 3 || (deviceProp.major == 3 && deviceProp.minor >=5)))
    {
        printf("GPU %d - %s does not support CUDA Dynamic Parallelism\n Exiting.", device, deviceProp.name);
        exit(1);
    }

    CudaShape first;
    auto s = s1->getPointCount();
    first.pointCount = s;
    first.type = RECTANGLE;

    cudaMalloc(&first.points, sizeof(Vector2) * s);
    for (int i = 0; i < s; i++) {
        auto p = (Vector2)s1->getPoint(i);
        cudaMemcpy(&first.points[i], &p, sizeof(Vector2), cudaMemcpyHostToDevice);
    }

    const float* matrix = s1->getTransform().getMatrix();
    cudaMalloc(&first.transform, sizeof(float) * 16);
    for (int i = 0; i < 16; i++) {
        cudaMemcpy(&first.transform[i], &matrix[i], sizeof(float), cudaMemcpyHostToDevice);
    }

    CudaShape second;
    s = s2->getPointCount();
    second.pointCount = s;
    second.type = RECTANGLE;

    cudaMalloc(&second.points, sizeof(Vector2) * s);
    for (int i = 0; i < s; i++) {
        auto p = (Vector2)s2->getPoint(i);
        cudaMemcpy(&second.points[i], &p, sizeof(Vector2), cudaMemcpyHostToDevice);
    }

    cudaMalloc(&second.transform, sizeof(float) * 16);
    matrix = s2->getTransform().getMatrix();
    for (int i = 0; i < 16; i++) {
        cudaMemcpy(&second.transform[i], &matrix[i], sizeof(float), cudaMemcpyHostToDevice);
    }

    const float precision = .25f;
    const int max = (int)(1 / precision);

    Colliding* results;
    cudaMalloc(&results, sizeof(Colliding) * max);
    checkContinuous<<<1, max>>>(max, direction, first, second, results);

    auto* r = (Colliding*)malloc(sizeof(Colliding) * max);
    cudaMemcpy(r, results, sizeof (Colliding) * max, cudaMemcpyDeviceToHost);

    cudaFree(&first);
    cudaFree(&second);
    cudaFree(&results);

    Vector2 old = startPos;
    for (int i = 0; i < max; ++i) {
        auto c = r[i];
        if (c.collision) {
            s1->setPosition((sf::Vector2f)old);
            return c;
        }
        old = (startPos + direction * ((float)(i-1) / (float)max));
    }

    return r[0];
}
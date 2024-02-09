#include "CollisionShape.h"
#include "Detection.cuh"

Colliding CollisionShape::getCollision(Vector2* axes, unsigned int length, const sf::Shape& first, const sf::Shape& second)
{
    double leastOverlap = INT16_MAX;
    Vector2 leastAxis;
    double greatestOverlap = 0.f;
    bool colliding = true;
    std::vector<Vector2> list;

    auto vertices1 = getGlobalVertices(first);
    auto vertices2 = getGlobalVertices(second);

    //auto detail = getCollisionAxisWrapper(axes, length, vertices1, first.getPointCount(), vertices2, second.getPointCount());
    for (int i = 0; i < length; i++)
    {
        auto axis = axes[i];
        auto contains = false;
        for (const auto &item: list) {
            if (item == axis) { contains = true; break; }
        }
        if (contains)
            continue;
        list.push_back(axis);

        Vector2 p1 = getProjection(axis, vertices1, first.getPointCount());
        Vector2 p2 = getProjection(axis, vertices2, second.getPointCount());

        bool overlap = p1.x < p2.y && p2.x < p1.y;
        if (!overlap) {
            colliding = false;
            break;
        }
        else {
            double minOverlap = std::max(p1.x, p2.x);
            double maxOverlap = std::min(p1.y, p2.y);
            auto newOverlap = Vector2(minOverlap, maxOverlap);
            auto lO = leastOverlap;
            auto nO = abs(newOverlap.x - newOverlap.y);
            /*if (abs(lO - nO) < 0.0001) {
                auto a = axis;
                if (p1.y > p2.y)
                    a.y *= -1;
                if (p1.x > p2.x)
                    a.x *= -1;

                leastAxis += a;
                leastAxis = leastAxis.normalize();
            }
            else */
            if (nO < lO) {
                //leastOverlap = newOverlap;
                leastOverlap = nO;
                auto a = axis;
                if (p1.y > p2.y)
                    a.y *= -1;
                if (p1.x > p2.x)
                    a.x *= -1;

                if (greatestOverlap < nO)
                    greatestOverlap = nO;

                leastAxis = a;
            }
        }
    }

    delete[] vertices1;
    delete[] vertices2;

    return Colliding{colliding, greatestOverlap, leastOverlap, leastAxis};
    //return Colliding{detail.collision, detail.greatestOverlap, detail.leastOverlap, detail.leastAxis};
}

Vector2* CollisionShape::getGlobalVertices(const sf::Shape& s)
{
    const unsigned int length = s.getPointCount();
    auto vertices = new Vector2[length];

    for (int i = 0; i < length; i++)
    {
        auto vertex = s.getPoint(i);
        auto p = s.getTransform().transformPoint(vertex);

        vertices[i] = (Vector2)p;
    }

    return vertices;
}

Vector2* CollisionShape::getAxes(const sf::Shape& s)
{
    const unsigned int length = s.getPointCount();
    auto axes = new Vector2[length];
    auto vertices = getGlobalVertices(s);
    for (int i = 0; i < length; i++)
    {
        //Getting vertex
        auto v1 = vertices[i];
        //Getting other vertex
        auto v2 = vertices[i+1 == length ? 0 : i+1];

        Vector2 edge = v1 - v2;

        Vector2 normal = edge.orthogonal();

        //Normalizing to get accurate projections
        axes[i] = normal.normalize();
    }
    return axes;
}

Vector2 CollisionShape::getProjection(Vector2 axis, Vector2* vertices, unsigned int length)
{
    double min = axis.dot(vertices[0]);
    double max = min;
    for (int i = 1; i < length; i++) {
        double p = axis.dot(vertices[i]);
        if (p < min) {
            min = p;
        } else if (p > max) {
            max = p;
        }
    }
    return {min, max};
}

void CollisionShape::findMinDistance(
        size_t verticesA,
        size_t verticesB,
        sf::Shape* boundsA,
        sf::Shape* boundsB,
        Vector2* contact1,
        Vector2* contact2,
        float* minDistSq,
        int* contactCount
)
{
    auto globalA = getGlobalVertices(*boundsA);
    auto globalB = getGlobalVertices(*boundsB);

    for (int i = 0; i < verticesA; i++) {
        auto vertexA = globalA[i];

        for (int j = 0; j < verticesB; j++) {
            auto bStart = globalB[j];
            auto bEnd = globalB[(j + 1) % verticesB];

            auto cp = math::dist(vertexA, bStart, bEnd);

            if (!cp.contains)
                continue;

            if (abs(cp.dist - *minDistSq) < 0.0001)
            {
                if (cp.point == *contact1) continue;

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

CollidingPoints CollisionShape::findContactPoints(
        sf::Shape* boundsA,
        sf::Shape* boundsB
)
{
    auto contact1 = Vector2();
    auto contact2 = Vector2();
    auto contactCount = 0;

    auto verticesA = boundsA->getPointCount();
    auto verticesB = boundsB->getPointCount();

    float minDistSq = INT8_MAX;

    findMinDistance(
            verticesA,
            verticesB,
            boundsA,
            boundsB,
            &contact1,
            &contact2,
            &minDistSq,
            &contactCount
    );

    findMinDistance(
            verticesB,
            verticesA,
            boundsB,
            boundsA,
            &contact1,
            &contact2,
            &minDistSq,
            &contactCount
    );

    return CollidingPoints{contact1, contact2, contactCount};
}
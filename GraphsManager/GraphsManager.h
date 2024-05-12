#ifndef PHYSICSSIMULATIONS_GRAPHSMANAGER_H
#define PHYSICSSIMULATIONS_GRAPHSMANAGER_H

#include <vector>
#include <functional>
#include <SFML/System/Vector2.hpp>
//ROOT
#include <TROOT.h>
#include "TCanvas.h"
#include "TGraph.h"
#include "TApplication.h"
#include "TAxis.h"
#include "TStyle.h"

enum GraphMode {
    WITH_LINES,
    DEFAULT
};

typedef const std::function<void(TGraph*)>& graph_modifier;

typedef struct Graph {
    std::unique_ptr<TGraph> graph;
    Option_t* mode;
} Graph;

class GraphsManager {
private:
    std::vector<Graph*> graphs;
    TApplication rootApp;
    std::unique_ptr<TCanvas> canvas;

    static Option_t* getGraphMode(GraphMode m);
public:
    inline GraphsManager() :
        rootApp("Graphs", nullptr, nullptr),
        canvas(std::make_unique<TCanvas>("canvas", "Physics simulations"))
    {}

    void setCanvasSize(sf::Vector2i size);

    void setCanvasPosition(sf::Vector2i pos);

    /**
     * Add a graph to the canvas
     * @param title graph title
     * @param func override and change some graph's properties
     * @param mode draw mode for the graph
     */
    void addGraph(
            const char* title,
            graph_modifier func = [](TGraph* _){},
            GraphMode mode = GraphMode::DEFAULT
    );

    void build();

    void addPoint(int i, double x, double y);

    void render();

    void reset();
};


#endif //PHYSICSSIMULATIONS_GRAPHSMANAGER_H

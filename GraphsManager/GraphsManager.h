#ifndef PHYSICSSIMULATIONS_GRAPHSMANAGER_H
#define PHYSICSSIMULATIONS_GRAPHSMANAGER_H

#include <iostream>
#include <vector>

//ROOT
#include <TROOT.h>
#include "TCanvas.h"
#include "TGraph.h"
#include "TApplication.h"
#include "TAxis.h"
#include "TStyle.h"

typedef struct Graph {
    std::unique_ptr<TGraph> graph;
    Option_t* mode;
} Graph;

class GraphsManager {
private:
    std::vector<Graph*> graphs;
    TApplication rootApp;
    std::unique_ptr<TCanvas> canvas;
public:
    inline GraphsManager() :
        rootApp("Graphs", nullptr, nullptr),
        canvas(std::make_unique<TCanvas>("canvas", "Physics simulations"))
    {
        canvas->SetWindowSize(1550, 700);
        canvas->SetWindowPosition(-2048, 0);
    }

    void addGraph(const std::string& title, const char* mode);

    void build();

    inline void addPoint(int i, double x, double y)
    {
        graphs[i]->graph->AddPoint(x, y);
    }

    void render();
};


#endif //PHYSICSSIMULATIONS_GRAPHSMANAGER_H

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

class GraphsManager {
private:
    std::vector<std::unique_ptr<TGraph>> graphs;
    TApplication rootApp;
    std::unique_ptr<TCanvas> canvas;
public:
    inline GraphsManager() :
        rootApp("Graphs", nullptr, nullptr),
        canvas(std::make_unique<TCanvas>("canvas", "Physics simulations"))
    {
        canvas->SetWindowSize(1550, 700);
        canvas->SetWindowPosition(/*-2048*/0, 0);
    }

    void addGraph(const std::string& title);

    void build();

    inline void addPoint(int i, double x, double y)
    {
        graphs[i]->AddPoint(x, y);
    }

    void render();
};


#endif //PHYSICSSIMULATIONS_GRAPHSMANAGER_H

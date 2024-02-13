#include "GraphsManager.h"

void GraphsManager::addGraph(const std::string &title, const char* mode) {
    auto graph = std::make_unique<TGraph>(1024);
    graph->SetTitle(title.c_str());
    //graph->SetMinimum(0);
    graph->SetMarkerSize(1.5);
    graph->SetMarkerStyle(21);
    graph->SetLineWidth(3);
    graph->SetLineColor(kBlue);
    static Graph g;
    g.graph = std::move(graph);
    g.mode = mode;
    graphs.push_back(&g);
}

void GraphsManager::build() {
    auto size = (int)graphs.size();
    auto rows = (int)sqrt(size);

    canvas->Divide(rows, size - rows + 1);

    for (int i = 0; i < size; i++) {
        canvas->cd(i+1);
        auto g = graphs[i];
        g->graph->Draw(g->mode);
    }
}

void GraphsManager::render() {
    for (int i = 0; i < (int)graphs.size(); i++) {
        canvas->cd(i+1);
        canvas->Update();
        canvas->Pad()->Draw();
    }
}

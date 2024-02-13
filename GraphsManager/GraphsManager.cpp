#include "GraphsManager.h"

void GraphsManager::addGraph(const char* title, graph_modifier func, GraphMode mode) {
    auto graph = std::make_unique<TGraph>(1024);
    graph->SetTitle(title);
    if (mode == DEFAULT) {
        graph->SetMarkerSize(1.5);
        graph->SetMarkerStyle(21);
    }
    graph->SetLineWidth(3);
    graph->SetLineColor(kBlue);
    func(graph.get());

    static Graph g;
    g.graph = std::move(graph);
    g.mode = getGraphMode(mode);
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

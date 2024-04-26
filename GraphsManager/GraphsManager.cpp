#include "GraphsManager.h"

Option_t *GraphsManager::getGraphMode(GraphMode m) {
    Option_t* t = "AP";
    switch (m) {
        case WITH_LINES:
            t = "APL";
            break;
        case DEFAULT:
            t = "AP";
            break;
    }
    return t;
}

void GraphsManager::setCanvasSize(sf::Vector2i size) {
    canvas->SetWindowSize(size.x, size.y);
}

void GraphsManager::setCanvasPosition(sf::Vector2i pos) {
    canvas->SetWindowPosition(pos.x, pos.y);
}

void GraphsManager::addPoint(int i, double x, double y) {
    graphs[i]->graph->AddPoint(x, y);
}

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

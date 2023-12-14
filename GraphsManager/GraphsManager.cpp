#include "GraphsManager.h"

void GraphsManager::addGraph(const std::string &title) {
    auto graph = std::make_unique<TGraph>(1024);
    graph->SetTitle(title.c_str());
    //graph->SetMinimum(0);
    graph->SetLineWidth(3);
    graph->SetLineColor(kBlue);
    graphs.push_back(std::move(graph));
}

void GraphsManager::build() {
    auto size = (int)graphs.size();
    auto rows = (int)sqrt(size);

    canvas->Divide(rows, size - rows + 1);

    for (int i = 0; i < size; i++) {
        canvas->cd(i+1);
        graphs[i]->Draw();
    }
}

void GraphsManager::render() {
    for (int i = 0; i < (int)graphs.size(); i++) {
        canvas->cd(i+1);
        canvas->Update();
        canvas->Pad()->Draw();
    }
}

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "../GraphsManager/GraphsManager.h"

namespace Simulations {
    class Simulation {
    public:
        double time = 0.f;
        GraphsManager graphsManager;
        bool start;
        bool paused;

        sf::Clock runTimeClock;
        sf::Clock deltaTimeClock;
        sf::Clock clock;
        sf::Time elapsedTime;

        Simulation() :
                start(false),
                paused(false)
        {
            //Graphs
            graphsManager.addGraph("Graph");
            graphsManager.addGraph("Graph");
            graphsManager.build();
        }

        virtual void onCreate() {}
        virtual void onEvent(sf::Event) {}
        virtual void onUpdate() {}
        virtual void onDrawGraphs() {}
    };
}
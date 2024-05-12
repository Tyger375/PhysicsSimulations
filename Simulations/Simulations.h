#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../GraphsManager/GraphsManager.h"

namespace Simulations {
    class Simulation {
    public:
        double time = 0.f;
        GraphsManager* graphsManager;
        bool start;
        bool paused;

        sf::Clock runTimeClock;
        sf::Clock deltaTimeClock;
        sf::Clock clock;
        sf::Time elapsedTime;

        explicit Simulation(GraphsManager* manager) :
                graphsManager(manager),
                start(false),
                paused(false)
        {}

        virtual void onCreate() {}
        virtual void onEvent(sf::Event) {}
        virtual void onUpdate() {}
        virtual void onDrawGraphs() {}
        virtual void onRender(sf::RenderWindow*) {}
    };
}
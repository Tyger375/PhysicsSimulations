#include "../Vector2/Vector2.h"
#include "../Object/Object.h"
#include "../Rope/RopeRigidBody.h"
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

    void Projectile() {
        auto size = Vector2(50, 50);
        static Object obj(size, size);
        obj.setPosition(Vector2(50, 400));

        auto ground2Size = Vector2(500 * 10, 50);
        static Object ground(ground2Size, ground2Size, sf::Color::Blue);
        ground.rb.useGravity = false;
        ground.setPosition(Vector2(250, 650));

        /*auto ground3Size = Vector2(50, 100);
        static Object ground2(ground3Size, ground3Size, sf::Color::Yellow);
        ground2.rb.useGravity = false;
        ground2.setPosition(Vector2(250, 550));*/

        GlobalVars::entities.push_back(&obj);
        GlobalVars::entities.push_back(&ground);
        //GlobalVars::entities.push_back(&ground2);
    }
}
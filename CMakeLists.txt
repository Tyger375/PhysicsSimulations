cmake_minimum_required(VERSION 3.24)
project(PhysicsSimulations)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        Test
        main.cpp
        Behaviors/RigidBody/RigidBody.h
        Behaviors/RigidBody/RigidBody.cpp main.h
        Behaviors/CollisionShape/CollisionShape.h
        Behaviors/CollisionShape/CollisionShape.cpp

        Behaviors/RectangleShape/RectangleShape.h
        Behaviors/RectangleShape/RectangleShape.cpp
        Behaviors/CircleShape/CircleShape.h
        Behaviors/CircleShape/CircleShape.cpp

        Rope/Rope.cpp
        Rope/Rope.h
        Rope/RopeRigidBody.h
        Object.h
        Object.cpp
        Vector2/Vector2.h
)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML_32/lib/cmake/SFML)
find_package(SFML 2.5 COMPONENTS system window graphics audio network REQUIRED)

# include_directories(C:/SFML/include)
target_link_libraries(Test
        PRIVATE
        sfml-graphics
        sfml-window
        sfml-main
        sfml-network
        sfml-audio
        )
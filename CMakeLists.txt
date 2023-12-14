cmake_minimum_required(VERSION 3.24)
project(PhysicsSimulations)

set(CMAKE_CXX_STANDARD 17)

set(ROOT_BIN_DIR "C:/root_v6.30.02/bin")
set(ROOT_DIR "C:/root_v6.30.02/cmake")
find_package(ROOT REQUIRED COMPONENTS CORE RIO Net Graf Gpad)
link_directories(${ROOT_LIBRARY_DIR} ${ROOT_BIN_DIR})

#set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML-2.6.1/lib/cmake/SFML)
find_package(SFML 2.6.1 REQUIRED COMPONENTS system window graphics audio network)

include_directories(C:/SFML-2.6.1/include)

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
        Object/Object.h
        Object/Object.cpp
        Vector2/Vector2.h
        Simulations/Simulations.h Entity.h Behaviors/Behavior.h Rope/RopeMember.h
        GraphsManager/GraphsManager.cpp
        GraphsManager/GraphsManager.h
        Simulations/Test.h
        Simulations/RopeTest.h
)

# target_compile_options(Test PUBLIC -Zc:__cplusplus)
target_compile_definitions(Test PUBLIC SIMULATION_NAME="test")

target_include_directories(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIRS})
target_link_libraries(Test
        PRIVATE
        sfml-graphics
        sfml-window
        sfml-main
        sfml-network
        sfml-audio
        ${ROOT_LIBRARIES}
)
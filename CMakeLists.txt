cmake_minimum_required(VERSION 3.24)
project(PhysicsSimulations CUDA CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_STANDARD 17)

## DEPENDENCY

# VARS
set(ROOT_BIN_DIR "C:/root_v6.30.02/bin")
set(ROOT_DIR "C:/root_v6.30.02/cmake")
set(SFML_DIR "C:/SFML-2.6.1/lib/cmake/SFML")
set(USE_CUDA false)

# Setting ROOT
find_package(ROOT REQUIRED COMPONENTS CORE RIO Net Graf Gpad)
link_directories(${ROOT_LIBRARY_DIR} ${ROOT_BIN_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIRS})

# Setting SFML
find_package(SFML 2.6.1 REQUIRED COMPONENTS system window graphics audio network)
include_directories(C:/SFML-2.6.1/include)

# Adding project's files
add_library(
        Main
        Behaviors/RigidBody/RigidBody.h
        Behaviors/RigidBody/RigidBody.cpp main.h
        Behaviors/CollisionShape/CollisionShape.h
        Behaviors/CollisionShape/CollisionShape.cpp
        Behaviors/CollisionShape/Detection.cu
        Behaviors/CollisionShape/Detection.cuh

        Behaviors/RectangleShape/RectangleShape.h
        Behaviors/RectangleShape/RectangleShape.cpp
        Behaviors/CircleShape/CircleShape.h
        Behaviors/CircleShape/CircleShape.cpp

        Rope/Rope.cpp
        Rope/Rope.h
        Rope/RopeRigidBody.h
        Object/Object.h
        Object/Object.cpp
        Vectors/Vector2.h
        Simulations/Simulations.h Entity/Entity.h Behaviors/Behavior.h Rope/RopeMember.h
        GraphsManager/GraphsManager.cpp
        GraphsManager/GraphsManager.h
        Simulations/Test.h
        Simulations/RopeTest.h
        Simulations/Projectile.h
        Utils.h
        Entity/Entity.cpp
        Vectors/Vector3.h
)

set_target_properties(Main PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)

target_link_libraries(Main
        PUBLIC
        sfml-graphics
        sfml-window
        sfml-main
        sfml-network
        sfml-audio
        ${ROOT_LIBRARIES}
)

## EXECUTABLES
add_executable(Test main.cpp)
target_link_libraries(Test Main)
target_compile_definitions(Test PUBLIC SIMULATION=1)

add_executable(TestCuda main.cpp)
target_link_libraries(TestCuda Main)
target_compile_definitions(TestCuda PUBLIC
        SIMULATION=1
)

if(${USE_CUDA})
    add_compile_definitions(USECUDA)
endif()

add_executable(Projectile main.cpp)
target_link_libraries(Projectile Main)
target_compile_definitions(Projectile PUBLIC SIMULATION=3)
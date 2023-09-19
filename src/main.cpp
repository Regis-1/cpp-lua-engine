#include <iostream>

#include "Engine.hpp"

int main(int argc, char** argv) {
    Engine engine({1280, 720}, "Beyond...");

    engine.run();

    return 0;
}

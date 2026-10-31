#include "spch.h"
#include "Core/EntryPoint.h"
#include "Game.h"

namespace Six {
    SApplication* CreateApplication() {
        return new Game();
    }
}

int main(int argc, char** argv) {
    return Six::RunApplication();
}

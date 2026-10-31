#include "SixEngine.h"
#include "Core/EntryPoint.h"
#include "SEditorApp.h"

namespace Six {
    SApplication* CreateApplication() {
        return new SixEditor::SEditorApp();
    }
}

int main(int argc, char** argv) {
    return Six::RunApplication();
}

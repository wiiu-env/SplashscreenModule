#include "gfx/SplashScreenDrawer.h"
#include "gfx/gfx.h"
#include "utils/logger.h"
#include "utils/utils.h"
#include "version.h"

#define MODULE_VERSION      "v0.3"
#define MODULE_VERSION_FULL MODULE_VERSION SPLASHSCREEN_MODULE_VERSION_EXTRA

int main(int argc, char **argv) {
    initLogging();
    DEBUG_FUNCTION_LINE_INFO("Running SplashScreen Module " MODULE_VERSION_FULL "");

    std::filesystem::path envDir;
    if (argc >= 1) {
        envDir = argv[0];
    }

    GfxInit();
    {
        SplashScreenDrawer splashScreenDrawer(envDir);
        splashScreenDrawer.Draw();
    }
    GfxShutdown();

    deinitLogging();

    return 0;
}

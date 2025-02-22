#include "gfx/SplashScreenDrawer.h"
#include "gfx/gfx.h"
#include "utils/logger.h"
#include "utils/utils.h"
#include "version.h"

#define MODULE_VERSION      "v0.3"
#define MODULE_VERSION_FULL MODULE_VERSION SPLASHSCREEN_MODULE_VERSION_EXTRA

int32_t main(int32_t argc, char **argv) {
    initLogging();
    DEBUG_FUNCTION_LINE_INFO("Running SplashScreen Module " MODULE_VERSION_FULL "");

    std::filesystem::path basePath = "fs:/vol/external01/wiiu";
    if (argc >= 1) {
        basePath = argv[0];
    }

    GfxInit();
    {
        SplashScreenDrawer splashScreenDrawer(basePath);
        splashScreenDrawer.Draw();
    }
    GfxShutdown();

    deinitLogging();

    return 0;
}

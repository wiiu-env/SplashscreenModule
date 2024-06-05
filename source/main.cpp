#include "gfx/SplashScreenDrawer.h"
#include "gfx/gfx.h"
#include "utils/logger.h"
#include "utils/utils.h"
#include "version.h"
#include <coreinit/memory.h>

#define MODULE_VERSION      "v0.1"
#define MODULE_VERSION_FULL MODULE_VERSION SPLASHSCREEN_MODULE_VERSION_EXTRA

int32_t main(int32_t argc, char **argv) {
    initLogging();
    DEBUG_FUNCTION_LINE_INFO("Running SplashScreen Module " MODULE_VERSION_FULL "");

    std::string basePath = "fs:/vol/external01/wiiu";
    if (argc >= 1) {
        basePath = argv[0];
    }

    if (existsAsFile(basePath + "/splash.png") || existsAsFile(basePath + "/splash.tga")) {
        GfxInit();
        {
            SplashScreenDrawer splashScreenDrawer(basePath + "/");
            splashScreenDrawer.Draw();
        }
        GfxShutdown();
    } else {
        DEBUG_FUNCTION_LINE_INFO("No splash.png or splash.tga found in \"%s\". Skipping splash screen.", basePath.c_str());
    }

    deinitLogging();

    return 0;
}

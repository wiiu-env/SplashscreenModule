[![CI-Release](https://github.com/wiiu-env/SplashscreenModule/actions/workflows/ci.yml/badge.svg)](https://github.com/wiiu-env/SplashscreenModule/actions/workflows/ci.yml)
# SplashscreenModule
This module is supposed to be loaded with the [EnvironmentLoader](https://github.com/wiiu-env/EnvironmentLoader). It will render a "splash screen" from the sd card while
other modules of the environment are loading.

## Usage
Place the `01_splashscreen.rpx` in the `[ENVIRONMENT]/modules/setup` folder and run the EnvironmentLoader. 
- Requires a `splash.png` (recommended) or `splash.tga` in the `[ENVIRONMENT]` directory.
  - When using a `tga` make sure its 24 bit and uncompressed
  - In theory any (reasonable) resolution is supported, something like 1280x720 is recommended

If no splash screen is found on the sd card, this module will effectively do nothing.

**Note:** `[ENVIRONMENT]` is the directory of the environment, for Aroma with would be `sd:/wiiu/enviroments/aroma/splash.png`

## Buildflags

### Logging
Building via `make` only logs errors (via OSReport). To enable logging via the [LoggingModule](https://github.com/wiiu-env/LoggingModule) set `DEBUG` to `1` or `VERBOSE`.

`make` Logs errors only (via OSReport).  
`make DEBUG=1` Enables information and error logging via [LoggingModule](https://github.com/wiiu-env/LoggingModule).  
`make DEBUG=VERBOSE` Enables verbose information and error logging via [LoggingModule](https://github.com/wiiu-env/LoggingModule).

If the [LoggingModule](https://github.com/wiiu-env/LoggingModule) is not present, it'll fall back to UDP (Port 4405) and [CafeOS](https://github.com/wiiu-env/USBSerialLoggingModule) logging.

## Building
For building, you just need [wut](https://github.com/devkitPro/wut/) installed, then use the `make` command.

## Building using the Dockerfile

It's possible to use a docker image for building. This way you don't need anything installed on your host system.

```
# Build docker image (only needed once)
docker build . -t splashscreenmodule-builder

# make 
docker run -it --rm -v ${PWD}:/project splashscreenmodule-builder make

# make clean
docker run -it --rm -v ${PWD}:/project splashscreenmodule-builder make clean
```

## Format the code via docker

`docker run --rm -v ${PWD}:/src ghcr.io/wiiu-env/clang-format:13.0.0-2 -r ./source -i`

## Credits
- Maschell
- Crementif
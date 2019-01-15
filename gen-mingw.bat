
@set Generator="MinGW Makefiles"
@set BuildDir=build
@set RootDir=.

@rmdir /Q /S %BuildDir%

cmake -G%Generator% -H%RootDir% -B%BuildDir% -DCMAKE_SH="CMAKE_SH-NOTFOUND"

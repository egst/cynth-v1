@echo off

set build_dir=build\

rmdir /s /q %build_dir%
mkdir %build_dir%
cd %build_dir%

cmake -G"MinGW Makefiles" ..

@set LLVMPath=%DIR_BIN%\\llvm\\bin

@set Generator=Ninja
@set BuildDir=build
@set RootDir=.

@set CC=%LLVMPath%\\clang-cl
@set CXX=%LLVMPath%\\clang-cl
@set CXXFLAGS=/MD

@rmdir /Q /S %BuildDir%

cmake -G%Generator% -H%RootDir% -B%BuildDir%

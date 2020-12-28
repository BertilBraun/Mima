@echo off
TITLE Compiling...

cd ../emsdk

@call emsdk construct_env

cmd /c em++ ../Mima/Mima/src/main.cpp -o index.html -s EXPORTED_FUNCTIONS=["_canStep","_step","_load"] -s EXPORTED_RUNTIME_METHODS=["ccall","cwrap"]

echo Successfully compiled!
TITLE Done Compiling
PAUSE

TITLE Starting Node
node Index.js

TITLE Done!
PAUSE
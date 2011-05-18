#!/bin/bash

cd ${0%/*}
g++ -o PikaCmd.temp PikaCmd.cpp BuiltIns.cpp ../src/*.cpp
pikacmd=$PROJECT_DIR/build/Mac/GCC/Release/$EXECUTABLE_PATH
./PikaCmd.temp UpdateBuiltIns.pika
if [ ! -d build/GCC/Amalgamated ]; then mkdir -p build/GCC/Amalgamated; fi
cat ../src/PikaScript.h ../src/PikaScriptImpl.h ../src/QStrings.h ../src/QuickVars.h ../src/PikaScript.cpp ../src/QStrings.cpp  BuiltIns.cpp PikaCmd.cpp >PikaCmdAmalgam.cpp
g++ -x c++ -pipe -Wno-trigraphs -fno-rtti -O3 -Wreturn-type -Wunused-variable -DNDEBUG -fmessage-length=0 -funroll-loops -ffast-math -fstrict-aliasing -msse3 -fvisibility=hidden -fvisibility-inlines-hidden -fno-threadsafe-statics -mmacosx-version-min=10.4 -o build/GCC/Amalgamated/PikaCmd PikaCmdAmalgam.cpp
./build/GCC/Amalgamated/PikaCmd ../src/unittests.pika

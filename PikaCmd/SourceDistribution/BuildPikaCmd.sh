#!/bin/bash

BuildCpp()
{
	compiler_path="g++"
	static_lib_option=

	# use gcc on mac, better backwards compatiblity
	
	if [ -e "/Developer/usr/bin/g++" ]; then
		echo USING GCC
		compiler_path="/Developer/usr/bin/g++"
		static_lib_option="-static-libgcc"
	fi

	echo Compiling $1...
	${compiler_path} -x c++ -pipe -Wno-trigraphs -fno-rtti -Os -Wreturn-type -Wunused-variable -DNDEBUG -fmessage-length=0 -funroll-loops -ffast-math -fstrict-aliasing -m32 -fvisibility=hidden -fvisibility-inlines-hidden -fno-threadsafe-statics ${static_lib_option} -o $@ >/var/tmp/buildlog.txt 2>&1
	if [ $? -ne 0 ]; then
		cat /var/tmp/buildlog.txt
		echo Compilation of $1 failed
		exit 1
	else
		echo Compiled $1
	fi
}

if [ -e ./PikaCmd ]; then
	chmod +x ./PikaCmd >/dev/null 2>&1
else
	BuildCpp ./PikaCmd -DPLATFORM_STRING=UNIX PikaCmdAmalgam.cpp
	if [ $? -ne 0 ]; then
		exit 1
	fi
	echo Testing...
	if [ -e ./unittests.pika ]; then
		./PikaCmd unittests.pika >/dev/null
		if [ $? -ne 0 ]; then
			echo Unit tests failed
			exit 1
		fi
	fi
	if [ -e ./systoolsTests.pika ]; then
		./PikaCmd systoolsTests.pika
		if [ $? -ne 0 ]; then
			echo Systools tests failed
			exit 1
		fi
	fi
	echo Success
fi

exit 0

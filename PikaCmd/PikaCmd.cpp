/**
	\file PikaCmd.cpp

	PikaCmd is a simple command-line tool for executing a PikaScript source code file.
	
	The command syntax is: PikaCmd [ -? | <filename> [<arguments> ...] | '{' <code> '}' ]
		
	Command-line arguments are available in the global scope variables $1, $2 etc. ($0 is the script filename.) The
	process exit code will be that of the global variable 'exitCode' (default is 0), or 255 if an exception occurs.
	
	Notice that you may need to enclose <code> in double quotes (") to prevent the special interpretation of some
	characters (e.g. < and >). Double quotes inside <code> may need to be escaped, for example: \".
	
	There are "built-in" versions of some standard .pika files that will be used in case an external file with the same
	name does not exist. The built-in files are: 'stdlib.pika', 'debug.pika', 'help.pika', 'interactive.pika' and
	'default.pika'. ('default.pika' runs 'interactive.pika'.)
	
	If you run PikaCmd without arguments it will execute 'default.pika'. The built-in 'default.pika' runs
	'interactive.pika'.

	\version

	Version 0.93
	
	\page Copyright

	PikaScript is released under the "New Simplified BSD License". http://www.opensource.org/licenses/bsd-license.php
	
	Copyright (c) 2010-2011, NuEdge Development / Magnus Lidstroem
	All rights reserved.

	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
	following conditions are met:

	Redistributions of source code must retain the above copyright notice, this list of conditions and the following
	disclaimer. 
	
	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
	disclaimer in the documentation and/or other materials provided with the distribution. 
	
	Neither the name of the NuEdge Development nor the names of its contributors may be used to endorse or promote
	products derived from this software without specific prior written permission.
	
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
	INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define PIKA_UNICODE 0
#define QUICKER_SCRIPT 1

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#if !defined(PikaScript_h)
#include "../src/PikaScript.h"
#endif

#if (QUICKER_SCRIPT)
	#if !defined(QStrings_h)
	#include "../src/QStrings.h"
	#endif
	#if !defined(PikaScriptImpl_h)
	#include "../src/PikaScriptImpl.h"
	#endif
	#if !defined(QuickVars_h)
	#include "../src/QuickVars.h"
	#endif

	struct QuickerScriptConfig;
	typedef Pika::Script<QuickerScriptConfig> Script;

	struct QuickerScriptConfig {
		typedef Pika::STLValue< QStrings::QString<char> > Value;
		typedef Pika::QuickVars<Script::STLVariables> Locals;
		typedef Locals Globals;
	};
#else
	typedef Pika::StdScript Script;
#endif

extern const char* BUILT_IN_DEBUG;
extern const char* BUILT_IN_HELP;
extern const char* BUILT_IN_INTERACTIVE;
extern const char* BUILT_IN_STDLIB;

const char* BUILT_IN_USAGE =
		"print('"
		"\n"
		"PikaCmd [ -? | <filename> [<arguments> ...] | ''{'' <code> ''}'' ]\n"
		"\n"
		"Command-line arguments are available in the global scope variables $1, $2 etc. ($0 is the script filename.) "
		"The process exit code will be that of the global variable ''exitCode'' (default is 0), or 255 if an exception "
		"occurs.\n"
		"\n"
		"Notice that you may need to enclose <code> in double quotes (\") to prevent the special interpretation of "
		"some characters (e.g. < and >). Double quotes inside <code> may need to be escaped, for example: \\\".\n"
		"\n"
		"There are \"built-in\" versions of some standard .pika files that will be used in case an external file "
		"with the same name does not exist. The built-in files are: ''stdlib.pika'', ''debug.pika'', ''help.pika'', "
		"''interactive.pika'' and ''default.pika''.\n"
		"\n"
		"If you run PikaCmd without arguments it will execute ''default.pika''. The built-in ''default.pika'' runs "
		"''interactive.pika''.\n"
		"');";
		
const char* BUILT_IN_DEFAULT = "run('interactive.pika', 'go')";
			
const char* BUILT_IN_DIRECT =
		"{ run('stdlib.pika'); s = ''; for (i = 0; i < $n; ++i) s #= ' ' # $[i]; "
		"print('---- (' # evaluate(s, @::) # ')') }";

std::pair<Script::String, Script::String> BUILT_IN_FILES[] = {
	std::pair<Script::String, Script::String>("debug.pika", Script::String(BUILT_IN_DEBUG))
	, std::pair<Script::String, Script::String>("default.pika", Script::String(BUILT_IN_DEFAULT))
	, std::pair<Script::String, Script::String>("help.pika", Script::String(BUILT_IN_HELP))
	, std::pair<Script::String, Script::String>("interactive.pika", Script::String(BUILT_IN_INTERACTIVE))
	, std::pair<Script::String, Script::String>("stdlib.pika", Script::String(BUILT_IN_STDLIB))
	, std::pair<Script::String, Script::String>("-?", Script::String(BUILT_IN_USAGE))
};

Script::String overloadedLoad(const Script::String& file) {
	std::basic_ifstream<Script::Char> instream(Pika::toStdString(file).c_str()); // Sorry, can't pass a wchar_t filename. MSVC supports it, but it is non-standard. So we convert to a std::string to be on the safe side.
	if (!instream.good()) {
		for (int i = 0; i < sizeof (BUILT_IN_FILES) / sizeof (*BUILT_IN_FILES); ++i)
			if (file == BUILT_IN_FILES[i].first) return BUILT_IN_FILES[i].second;
		throw Script::Xception(Script::String("Cannot open file for reading: ") += Pika::escape(file));
	}
	Script::String chars;
	while (!instream.eof()) {
		if (instream.bad())
			throw Script::Xception(Script::String("Error reading from file: ") += Pika::escape(file));
		Script::Char buffer[1024];
		instream.read(buffer, 1024);
		chars += Script::String(buffer, static_cast<Script::String::size_type>(instream.gcount()));
	}
	return chars;
}

int main(int argc, const char* argv[]) {
	int exitCode = 255;
	std::srand(static_cast<unsigned int>(std::time(0)) ^ static_cast<unsigned int>(std::clock()));
	rand();
	if (argc < 2)
		std::cout << "PikaCmd version " << PIKA_SCRIPT_VERSION << ". (C) 2010-2011 NuEdge Development. "
				"All rights reserved." << std::endl << "Run PikaCmd -? for command-line argument syntax."
				<< std::endl << std::endl;
	try {
		Script::FullRoot root;
		root.registerNative("load", overloadedLoad);
		root.assign("exitCode", Script::Value(0));
		const Script::String fn(argc < 2 ? "default.pika" : argv[1]);
		std::vector<Script::Value> args(1, fn);
		for (int i = 2; i < argc; ++i) args.push_back(Script::String(argv[i]));
		root.call("run", (fn[0] == '{' ? Script::String(BUILT_IN_DIRECT) : Script::Value()), args.size(), &args[0]);
		exitCode = static_cast<int>(root.getOptional("exitCode"));
	} catch (const Script::Xception& x) {
		std::cerr << "!!!! " << x.what() << std::endl;
		exitCode = 255;
	}
#if (QUICKER_SCRIPT)
	QStrings::QString<char>::deinit();
#endif
	return exitCode;
}

PikaScript is a small interpreting C-style scripting language implemented with modern C++. The source code for PikaScript is amazingly compact. It's core is around 1000 lines of C++ plus an additional header file of 500 lines including doxygen comments. The optional standard library (written in PikaScript), provides utilities for strings, math, algorithms and objects in less than 300 lines of source code. Despite it's small size the language is powerful enough to support sophisticated concepts such as higher order functions and closures.

### Goals of PikaScript ###

  * Easy to learn high-level scripting language with C-style syntax.
  * Compact bug-free implementation in modern C++.
  * Easy to interface with host code. E.g., all variables are represented by C++ strings.
  * 100% interpreted. No compilation pass required, just load a string and run.
  * Minimalistic syntax. E.g., all statements counts as expressions and return values, eliminating redundant operators such as ? : (instead "if" can be used inside expressions).
  * 100% dynamic with full run-time reflection of variables, code and even stack. (Making it possible to even write debuggers in PikaScript.)
  * No need for memory heaps and explicit or implicit memory management (e.g. garbage collection). The only dynamically allocated entity in PikaScript is the variable. E.g., an array is a number of variables linked by name only.

### Non-goals of PikaScript ###

  * Speed. PikaScript is slow, no doubt about that. Even with the optional quicker string class it will never be able to compete with compiled languages.
  * Small memory footprint. PikaScript can eat memory like a horse, especially if you use standard STL maps and strings for variable storage (which is the default configuration).
  * Object-orientation. You can create and use objects in PikaScript, but it is a secondary language contruct created without explicit support from the language syntax.

Documentation is available here: http://nuedge.net/pikascript/docs/PikaScript%20Documentation.html and in the repository.
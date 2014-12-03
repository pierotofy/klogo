KLogo
=====

A simple interpreted language similar to Logo (hence the name KLogo, Kind of Logo). It supports 4 variable types (bool, number, string and lists), iterators, conditional statements, user-defined functions. It also has a simple math library and a few I/O functions to interact with the console.

Docs
=====
See the Documentation.htm file.

Build Instructions
==================

You will need gcc >= 4.3.4, flex >= 2.5.35 and bison >= 2.4.2. Prebuilt binaries are available for Linux and Windows (cygwin).

To build, from the src directory type:

```
make
./KLogo <inputfile>
```

Notes
=====
The /examples directory contains some scripts that show some of the features of the laguage. This compiler is a proof of concept mainly developed for learning purposes and should be threated as such.

(C) Piero Toffanin 2011
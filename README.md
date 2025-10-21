# TextFlattener

A very simple C++ text compression program. Only supports 61 of the 256 ASCII characters: a-z, A-Z, ., ,, -, !, ', ", \n, space. All other characters are represented as #.

## THIS WAS COMPLETED FOR THE ECS 101 COMPRESSION GROUP PROJECT

## UML Class Diagram
![uml](diagrams/UML-Class-Diagram.png)

## UML Draft (Pre-code planning)
![uml-draft](diagrams/UML-Draft.jpeg)

Note: Dynamic runtime dictionary approach was eliminated by professor limitation

## Building & Running - Windows
---
1. Ensure a c++ compiling toolchain is installed (i.e. Cygwin, MSVC, etc.) with support for make
2. Ensure the make command is on your path (or simply copy the path to the make executable)
3. Open powershell and change directory to the unzipped source directory
4. Execute the following command (where \<make\> is either the path to the make executable if not added to path)
```
<make> -j8
```
5. Execute the program like so:
```
./main
```
6. Help will be printed to the screen. Use the following command to print more detailed help:
```
./main help
```
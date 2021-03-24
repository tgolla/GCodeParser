# `GCodeParser` Library for Arduino ![Version 1.0.0](https://img.shields.io/badge/Version-1.0.0-blue.svg)

**Written by:** *Terence F. Golla*.  
**Copyright:** **2021**, *Terence F. Golla, Released under [MIT](/LICENSE) licence*.

## About
The GCodeParser library is a light weight G-Code parser for the Arduino using only a single character buffer to first collect a line of code (also called a 'block') from a serial or file input and then parse that line into a code block and comments.

The parser was originally designed for use with code for the SphereBot, an EggBot clone.
https://github.com/tgolla/SphereBot

## Usage

A working example of the parser can be found the following GitHub repository.
https://github.com/tgolla/SphereBot

## Limitations
Currently the parser is not sophisticated enough to deal with parameters, Boolean operators, expressions, binary operators, functions and repeated items. However, this should not be an obstacle when building 2D/3D plotters, CNC, and projects with an Arduino controller.

## References

The following are just a few sources of information on G-Code.

https://www.autodesk.com/products/fusion-360/blog/cnc-programming-fundamentals-g-code/

http://www.machinekit.io/docs/gcode/overview/

https://www.reprap.org/wiki/G-code

https://howtomechatronics.com/tutorials/g-code-explained-list-of-most-important-g-code-commands/
 
# `GCodeParser` Library for Arduino ![Version 1.0.0](https://img.shields.io/badge/Version-1.0.0-blue.svg)

**Written by:** *Terence F. Golla*.  
**Copyright:** **2021**, *Terence F. Golla, Released under [MIT](/LICENSE) licence*.

## About
The GCodeParser library is a lightweight G-Code parser for the Arduino using only a single character buffer to first collect a line of code (also called a 'block') from a serial or file input and then parse that line into a code block and comments.

The parser was originally designed for use with code for the SphereBot, an EggBot clone.
https://github.com/tgolla/SphereBot

## Usage
The GCodeParser library is a lightweight G-Code parser in that it uses a single character buffer (string) which by default is defined as 256 characters plus 2.  This low memory usage profile is highly desirable in the limited memory world of the Arduino Uno.  To use the parser across your Arduino code you will want to define a global variable in your application.

```
GCodeParser GCode = GCodeParser();
```

The first step in parsing a line of G-Code is to load a line into the line buffer.  This is done a character at a time by calling the AddCharToLine method.  The AddCharToLine method returns a Boolean value of true when the end line is detected (\r\n or \n).  At this point the line buffer is ready to be parsed.  The following is an example of processing the serial input.

```
void loop()
{
  if (Serial.available() > 0)
  {
    if (GCode.AddCharToLine(Serial.read()))
    {
      GCode.ParseLine();
      // Code to process the line of G-Code here…
    }
  }
}
```

Once the line is parsed you can use the HasWord method to determine if a G-Code command (also referred to as a ‘word’) exist.  You can then get the value for the command with the GetWordValue method. 

A working example of the parser can be found the following GitHub repository.
https://github.com/tgolla/SphereBot

### `beginEnd`
The beginEnd attribute is a Boolean which returns true if the first character of the line it the percent character (%) which is used to denote the first and last line of the program.  

### `blockDelete`
The blockDelete attribute is a Boolean which returns true when the line begins with a slash '/'.

### `comments`
The comments attribute points to the comment(s) separated from the G-Code command line after executing the ParseLine method. Initially comment(s) contain the comment separators (parenthesis or semicolon). Executing the RemoveCommentSeparators method will remove the comment separators from all of the comments.

### `completeLineIsAvailableToParse`
The completeLineIsAvailableToParse attribute is a Boolean which returns true when there is a line available to parse. The value of the attribute is also returned by `AddCharToLine(char c)`.

### `lastComment`
The lastComment attribute points to the last comment on the command line. This is important as the last command will always be interpreted for active comment syntax.

### `line`
The line attribute points to the character buffer. After executing the ParseLine method the line attribute points to the G-Code command line (also called a 'block').

### `AddCharToLine(char c)`
The AddCharToLine method adds the provided character to the line buffer.  Each line should be terminated with either a carriage return/line feed (\r\n Windows) or line feed (\n Linux). The method returns a Boolean true when the end of line has been reached.

### `FindWord(char letter)`
The FindWord method returns a pointer to where the word (character) begins in the command line. In G-Code a word is a letter other than N followed by a real value. The method does not confirm the word is a valid G-Code and for this reason could be used to find the first occurrence of any character in the command line.

### `GetWordValue(char letter)`
The GetWordValue returns the value that follows the word character provided. If the word does not exist in the command line zero is returned.  For this reason it is best to use the HasWord method first to confirm the word exist in order to confirm the value returned is valid.

### `HasWord(char letter)`
The HasWord method returns a Boolean true if the word (letter followed by value). The method does test to confirm the character provided is a valid G-Code word.

### `Initialize()`
The Initialize method is call when the class in initialized and each time a new line is detected in `AddCharToLine(char c)`. If using the same instance of the GCodeParser with multiple files you may need to manually execute the method.

### `IsWord(char letter)`
The IsWord method returns a Boolean true if the character provided represents a valid G-Code word.

### `NoWords()`
The NoWords method returns a Boolean the if the line is blank and/or has no G-Code words.

### `ParseLine()`
The ParseLine method parses the command line removing whitespace and comments. The method should be used after the `AddCharToLine` method returns true.

### `ParseLine(char* gCode)`
The ParseLine method when passed g-code parses the command line passed removing whitespace and comments. The method is an alternative to first using the `AddCharToLine` method to build a line.

### `RemoveCommentSeparators()`
The RemoveCommentSeparators removes the comment separators (parenthesis or semicolon) from of the comments. The method should be used after the command line is parsed.

## Limitations
Currently the parser is not sophisticated enough to deal with parameters, Boolean operators, expressions, binary operators, functions and repeated items. However, this should not be an obstacle when building 2D/3D plotters, CNC, and projects with an Arduino controller.

## References

The following are just a few sources of information on G-Code.

https://www.autodesk.com/products/fusion-360/blog/cnc-programming-fundamentals-g-code/

http://www.machinekit.io/docs/gcode/overview/

https://www.reprap.org/wiki/G-code

https://howtomechatronics.com/tutorials/g-code-explained-list-of-most-important-g-code-commands/
 
/*
MIT License

Copyright (c) 2021 Terence Golla

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "GCodeParser.h"
#include <stdlib.h>
#include <string.h>

 /// <summary>
 /// Initializizes class.
 /// </summary>
void GCodeParser::Initialize()
{
	lineCharCount = 0;
	line[lineCharCount] = '\0';
	comments = line;
	lastComment = comments;
	blockDelete = false;
	beginEnd = false;
	completeLineIsAvailableToParse = false;
}

/// <summary>
/// Class constructor.
/// </summary>
/// <remark>
/// The G Code language is based on the RS274/NGC language. The G Code 
/// language is based on lines of code. Each line (also called a 'block')
/// may include commands to do several different things. Lines of code
/// may be collected in a file to make a program.
/// </remark>
GCodeParser::GCodeParser()
{
	Initialize();
}

/// <summary>
/// Adds a character to the line to be parsed.
/// </summary>
/// <param name="letter">The character to add.</param>
/// <returns>True if a complete line is available to parse.</returns>
/// <remarks>Adding a character after a CR/LF (\r\n - Windows) or LF (\n - Linux, Mac) have been added will reset the line buffer.</remarks>
bool GCodeParser::AddCharToLine(char c)
{
	// Determine is a new line is being added.
	if (completeLineIsAvailableToParse)
		Initialize();

	// Look for end of line. CRLF (\r\n) or just LF (\n).
	if (c == '\r' || c == '\n')
	{
		if (c == '\n') // Ignore CR (\r)
			completeLineIsAvailableToParse = true;
	}
	else
	{
		// Add character to line.
		line[lineCharCount] = c;
		lineCharCount++;

		// Deal with buffer overflow by initializing. TODO: Need a better solution.  i.e. Throw error?
		if (lineCharCount > MAX_LINE_SIZE)
			Initialize();

		line[lineCharCount] = '\0';
	}

	return completeLineIsAvailableToParse;
}

/// <summary>
/// Parses the line passed removing spaces, tabs and comments. Comments are shifted to the end of the line buffer.
/// </summary>
void GCodeParser::ParseLine(char* gCode)
{
	Initialize();

  int pointer = 0;
  while (gCode[pointer] != '\0')
  {
	  AddCharToLine(gCode[pointer]);
	  pointer = pointer + 1;
  }
  	
  AddCharToLine('\n');
  ParseLine();
}


/// <summary>
/// Parses the line removing spaces, tabs and comments. Comments are shifted to the end of the line buffer.
/// </summary>
void GCodeParser::ParseLine()
{
	int lineLength = strlen(line);
	line[lineLength + 1] = '\0';

	int pointer = 0;
	bool openParentheseFound = false;
	bool semicolonFound = false;
	int correctCommentsPointerBy = 0;

	while (line[pointer] != '\0')
	{
		char c = line[pointer];

		// Look for start of comment.
		if (!semicolonFound && c == '(')
			openParentheseFound = true; // Open parenthese... start of comment.

		if (!openParentheseFound && c == ';')
			semicolonFound = true; // Semicolon... start of comment to end of line.

		// If we are inside a comment, we need to move it to the end of the buffer in order to seperate it.
		if (openParentheseFound || semicolonFound)
		{
			// Shift line left.
			for (int i = pointer; i < lineLength; i++)
			{
				line[i] = line[i + 1];
			}
			line[lineLength] = c;
		}
		else
		{
			// Spaces and tabs are allowed anywhere on a line of code and do not change the meaning of 
			// the line, except inside comments. Remove spaces and tabs except in comments. 
			if (c == ' ' || c == '\t')
			{
				int removeCharacterPointer = pointer;

				while (line[removeCharacterPointer] != '\0')
				{
					line[removeCharacterPointer] = line[removeCharacterPointer + 1];

					removeCharacterPointer++;
				}

				correctCommentsPointerBy++;
			}
			else
				pointer++;
		}

		// Look for end of comment.
		if (!semicolonFound && c == ')')
		{
			openParentheseFound = false;

			// Is this the end of the comment? Scan forward for second closing parenthese, but no opening parenthese first.
			int scanAheadPointer = pointer;

			while (line[scanAheadPointer] != '\0')
			{
				if (line[scanAheadPointer] == '(')
					break;

				if (line[scanAheadPointer] == ')')
				{
					openParentheseFound = true;
					break;
				}

				scanAheadPointer++;
			}
		}
	}

	// Set pointer to comments.
	comments = line + strlen(line) + correctCommentsPointerBy + 1;
	lastComment = comments;

	// There are several 'active' comments which look like comments but cause some action, like
	// '(debug,..)' or '(print,..)'. If there are several comments on a line, only the last comment
	// will be interpreted according to these rules. For this reason there is a pointer to the last comment.
	pointer = 0;
	openParentheseFound = false;

	while (comments[pointer] != '\0')
	{
		char c = comments[pointer];

		// Open parenthese... start of comment.
		if (c == '(')
		{
			lastComment = comments + pointer;
			openParentheseFound = true; 
		}

		// Semicolon... start of comment to end of line, the last comment.
		if (!openParentheseFound && c == ';')
		{
			lastComment = comments + pointer;
			break;
		}

		// Look for end of comment.
		if (c == ')')
		{
			openParentheseFound = false;

			// Is this the end of the comment? Scan forward for second closing parenthese, but no opening parenthese first.
			int scanAheadPointer = pointer + 1;

			while (comments[scanAheadPointer] != '\0')
			{
				if (comments[scanAheadPointer] == '(')
					break;

				if (comments[scanAheadPointer] == ')')
				{
					openParentheseFound = true;
					break;
				}

				scanAheadPointer++;
			}
		}

		pointer++;
	}

	// The optional block delete character the slash '/' when placed first on a line can be used
	// by some user interfaces to skip lines of code when needed.
	blockDelete = (line[0] == '/');

	// The '%' is used to demarcate the beginning (first line) and end (last line) of the program. It is optional if the file has an 'M2' or 'M30'. 
	beginEnd = (line[0] == '%');
}

/// <summary>
/// Removes the comment seperators for comments and last comment along with any leading spaces.
/// </summary>
/// <remark>Once removed they cannot be replaced.</remark>
void GCodeParser::RemoveCommentSeparators()
{
	int commentsLength = strlen(comments);

	int pointer = 0;
	bool openParentheseFound = false;
	int correctCommentsPointerBy = 0;

	while (comments[pointer] != '\0')
	{
		char c = comments[pointer];

		// Look for start of comment.
		if (c == '(')
		{
			comments[pointer] = ' ';
			openParentheseFound = true; // Open parenthese... start of comment.
		}

		if (!openParentheseFound && c == ';')
		{
			comments[pointer] = ' ';
			break;
		}

		// Look for end of comment.
		if (c == ')')
		{
			openParentheseFound = false;

			// Is this the end of the comment? Scan forward for second closing parenthese, but no opening parenthese first.
			int scanAheadPointer = pointer + 1;

			while (comments[scanAheadPointer] != '\0')
			{
				if (comments[scanAheadPointer] == '(')
					break;

				if (comments[scanAheadPointer] == ')')
				{
					openParentheseFound = true;
					break;
				}

				scanAheadPointer++;
			}

			if (!openParentheseFound)
			{
				// Shift line left.
				for (int i = pointer; i < commentsLength; i++)
				{
					comments[i] = comments[i + 1];
				}
			}
			else
				pointer++;
		}
		else
			pointer++;
	}

	while (comments[0] == ' ')
	{
		// Shift pointer right
		comments = comments + 1;
	}

	while (lastComment[0] == ' ')
	{
		// Shift pointer right
		lastComment = lastComment + 1;
	}
}

/// <summary>
/// Looks for a word in the line.
/// </summary>
/// <param name="c">The letter of the word to look for in the line.</param>
/// <returns>A pointer to where the word starts.  Points to \0 if the word was not found.</returns>
int GCodeParser::FindWord(char letter)
{
	int pointer = 0;
	bool openParentheseFound = false;
	bool semicolonFound = false;

	while (line[pointer] != '\0')
	{
		// Look for the word.
		if (letter == line[pointer])
		{
			return pointer;
		}

		pointer++;
	}

	return pointer;
}

/// <summary>
/// Looks through the code block to determin if a word exist.
/// </summary>
/// <param name="letter">The letter of the GCode word.</param>
/// <returns>True if the word exist on the line.</returns>
bool GCodeParser::HasWord(char letter)
{
	if (IsWord(letter))
	{
		int pointer = FindWord(letter);

		if (line[pointer] == '\0')
		{
			return false;
		}
	}

	return true;
}

char wordLetter[] = { 'A', 'B', 'C', 'D', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '\0' };

/// <summary>
/// Determine if the letter provided represents a valid GCode word.
/// </summary>
/// <param name="letter">The letter to be tested.</param>
/// <returns>True if the letter represents a valid word.</returns>
/// <remark>
/// Words may begin with any of the letters shown in the following
/// Table. The table includes N, @, ^ and / for completeness, even 
/// though, line numbers, polar coordinates and the block delete  
/// character are not considered words. Several letters (I, J, K,
/// L, P, R) may have different meanings in different contexts.
/// Letters which refer to axis names are not valid on a machines
/// which do not have the corresponding axis.
/// 
/// A - A axis of machine.
/// B - B axis of machine.
/// C - C axis of machine.
/// D - Tool radius compensation number.
/// F - Feed rate.
/// G - General function(See table Modal Groups).
/// H - Tool length offset index.
/// I - X offset for arcsand G87 canned cycles.
/// J - Y offset for arcsand G87 canned cycles.
/// K - Z offset for arcsand G87 canned cycles. Spindle - Motion Ratio for G33 synchronized movements.
/// L - generic parameter word for G10, M66and others.
/// M - Miscellaneous function(See table Modal Groups).
/// N - Line number. Line numbers are not considered words.
/// P - Dwell time in canned cyclesand with G4. Key used with G10.
/// Q - Feed increment in G73, G83 canned cycles.
/// R - Arc radius or canned cycle plane.
/// S - Spindle speed.
/// T - Tool selection.
/// U - U axis of machine.
/// V - V axis of machine.
/// W - W axis of machine.
/// X - X axis of machine.
/// Y - Y axis of machine.
/// Z - Z axis of machine
/// @ - Polar coordinate for the distance. Polar coordinates are not considered words.
/// ^ - Polar coordinate for the angle. Polar coordinates are not considered words.
/// / - The block delete character causes the processor to skips the line and is not considered a word.
/// % - Indicated the beginning and end of a program and is not considered a word.
/// </remark>
bool GCodeParser::IsWord(char letter)
{	
	int pointer = 0;
	while (wordLetter[pointer] != '\0')
	{
		if (letter == wordLetter[pointer])
		{
			return true;
		}

		pointer++;
	}

	return false;
}

/// <summary>
/// Determine if the line contains any GCode words.
/// </summary>
/// <returns>True if there are no words.</returns>
/// <remarks>Words are not validated.<remark>
bool GCodeParser::NoWords()
{
	if (line[0] == '\0' || blockDelete || beginEnd)
	{
		return true;
	}

	int pointer = 0;
	while (wordLetter[pointer] != '\0')
	{
		if (HasWord(wordLetter[pointer]))
		{
			return false;
		}

		pointer++;
	}

	return true;
}

/// <summary>
/// Gets the value following the word.
/// </summary>
/// <param name="letter">The letter of the word to look for in the line.</param>
/// <returns>The value following the letter for the word.</returns>
/// <remarks>
/// Currently the parser is not sophisticated enough to deal with parameters, 
/// Boolean operators, expressions, binary operators, functions and repeated items.
/// </remarks>
double GCodeParser::GetWordValue(char letter)
{
	int pointer = FindWord(letter);

	if (line[pointer] != '\0')
		return (double)strtod(&line[pointer + 1], NULL);

	return 0.0;
}

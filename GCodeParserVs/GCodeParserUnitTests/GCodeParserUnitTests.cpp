#include "pch.h"
#include "CppUnitTest.h"
#include "../../src/GCodeParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GCodeParserUnitTests
{
	TEST_CLASS(GCodeParserUnitTests)
	{
	public:
		int AddLine(GCodeParser *gcode, int startAt)
		{
			// G01 (Comment Here) Z0.0
			// M300 S125 ;Comment to end of line.
			// G01 X3.2 Y1.5 Z5.0
			// /G21 (Block Delete) G90 ;MSG, 123
			// ...Blank Line...
			// M300 S125
			// (Comment Here)
			// ...Garbage no words... ;Comment
			// %
			char lines[] = { 'G', '0', '1' , '\t', '(', 'C', 'o', 'm', 'm', 'e', 'n', 't', ' ',	'H', 'e', 'r', 'e', ')', 'Z' , '0' , '.' , '0' , '\r', '\n',
				'M', '3', '0', '0', ' ', 'S', '1', '2', '5', ' ', ';', 'C', 'o', 'm', 'm', 'e', 't', ' ', 't', 'o', ' ', 'e', 'n', 'd', ' ', 'o', 'f', ' ', 'l', 'i', 'n', 'e', '.','\r', '\n',
				'G', '0', '1',' ','X','3','.','2',' ','Y','1','.','5',' ', '(', 'C', 'o', 'm', 'm', 'e', 'n', 't', ')', ' ', 'H', 'e', 'r', 'e', ')', ' ', 'Z', '5', '.', '0', '\r', '\n',
				'/', 'G', '2', '1', ' ', '(', 'B', 'l', 'o', 'c', 'k', ' ', ';', 'D', 'e', 'l',	'e', 't', 'e', ')', ' ', 'G', '9', '0', ' ', ';', 'M', 'S', 'G', ',', '1', '2', '3', '(', '?', ')', '\n',
				'\n',
				'M', '3', '0', '0', ' ', 'S', '1', '2', '5', '\n',
				'(', 'C', 'o', 'm', 'm', 'e', 'n', 't', ' ',	'H', 'e', 'r', 'e', ')', '\n',
				's', '3', '7', '3', '%', '^', '@', '/', ';', 'C', 'o', 'm', 'm', 'e', 'n', 't', '\n',
				'%', '\0'
			};

			bool completeLineIsAvailableToInterpret;
			while (lines[startAt] != '\0')
			{
				completeLineIsAvailableToInterpret = gcode->AddCharToLine(lines[startAt]);

				startAt++;

				if (completeLineIsAvailableToInterpret)
					return startAt;
			}

			return startAt;
		}

		TEST_METHOD(AddCharToLine_AddLines_ConfirmLines)
		{
			GCodeParser GCode = GCodeParser();

			int startAt = AddLine(&GCode, 0);

			char resultLine[] = { 'G', '0', '1', '\t', '(', 'C', 'o', 'm', 'm', 'e', 'n', 't', ' ',
				'H', 'e', 'r', 'e', ')', 'Z', '0', '.', '0', '\0' };

			int pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.line[pointer], resultLine[pointer]);
			} while (GCode.line[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			char resultLine2[] = { 'M', '3', '0', '0', ' ', 'S', '1', '2', '5', ' ', ';', 'C', 'o', 'm',
				'm', 'e', 't', ' ', 't', 'o', ' ', 'e', 'n', 'd', ' ', 'o', 'f', ' ', 'l', 'i', 'n', 'e', '.', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.line[pointer], resultLine2[pointer]);
			} while (GCode.line[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			char resultLine3[] = { 'G', '0', '1',' ','X','3','.','2',' ','Y','1','.','5',' ',
				'(', 'C', 'o', 'm', 'm', 'e', 'n', 't', ')', ' ', 'H', 'e', 'r', 'e', ')', ' ',
				'Z', '5', '.', '0', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.line[pointer], resultLine3[pointer]);
			} while (GCode.line[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			char resultLine4[] = { '/', 'G', '2', '1', ' ', '(', 'B', 'l', 'o', 'c', 'k', ' ', ';', 'D', 'e', 'l',
				'e', 't', 'e', ')', ' ', 'G', '9', '0', ' ', ';', 'M', 'S', 'G', ',', '1', '2', '3', '(', '?', ')', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.line[pointer], resultLine4[pointer]);
			} while (GCode.line[pointer] != '\0');


			startAt = AddLine(&GCode, startAt);

			char resultLine7[] = { '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.line[pointer], resultLine7[pointer]);
			} while (GCode.line[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			char resultLine6[] = { 'M', '3', '0', '0', ' ', 'S', '1', '2', '5', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.line[pointer], resultLine6[pointer]);
			} while (GCode.line[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			char resultLine5[] = { '(', 'C', 'o', 'm', 'm', 'e', 'n', 't', ' ',	'H', 'e', 'r', 'e', ')', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.line[pointer], resultLine5[pointer]);
			} while (GCode.line[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			char resultLine9[] = { 's', '3', '7', '3', '%', '^', '@', '/', ';', 'C', 'o', 'm', 'm', 'e', 'n', 't', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.line[pointer], resultLine9[pointer]);
			} while (GCode.line[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			char resultLine8[] = { '%', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.line[pointer], resultLine8[pointer]);
			} while (GCode.line[pointer] != '\0');
		}

		TEST_METHOD(ParseLine_ConfirmCodeBlock)
		{
			GCodeParser GCode = GCodeParser();

			int startAt = AddLine(&GCode, 0);

			GCode.ParseLine();

			char resultCode[] = { 'G', '0', '1', 'Z', '0', '.', '0', '\0' };

			int pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.line[pointer], resultCode[pointer]);
			} while (GCode.line[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultCode2[] = { 'M', '3', '0', '0', 	'S', '1', '2', '5', '\0' };

			pointer = -1;
			do 
			{
				pointer++;
				Assert::AreEqual(GCode.line[pointer], resultCode2[pointer]);
			} while (GCode.line[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultCode3[] = { 'G', '0','1','X','3','.','2','Y','1','.','5', 'Z','5','.','0', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.line[pointer], resultCode3[pointer]);
			} while (GCode.line[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultCode4[] = { '/', 'G', '2', '1', 'G', '9', '0', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.line[pointer], resultCode4[pointer]);
			} while (GCode.line[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultCode5[] = { '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.line[pointer], resultCode5[pointer]);
			} while (GCode.line[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultCode6[] = { 'M', '3', '0', '0', 'S', '1', '2', '5', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.line[pointer], resultCode6[pointer]);
			} while (GCode.line[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultCode7[] = { '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.line[pointer], resultCode7[pointer]);
			} while (GCode.line[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultCode9[] = { 's', '3', '7', '3', '%', '^', '@', '/', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.line[pointer], resultCode9[pointer]);
			} while (GCode.line[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultCode8[] = { '%', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.line[pointer], resultCode8[pointer]);
			} while (GCode.line[pointer] != '\0');
		}

		TEST_METHOD(ParseLine_ConfirmComments)
		{
			GCodeParser GCode = GCodeParser();

			int startAt = AddLine(&GCode, 0);

			GCode.ParseLine();

			char resultComments[] = { '(', 'C', 'o', 'm', 'm', 'e', 'n', 't', ' ', 'H', 'e', 'r', 'e', ')', '\0' };

			int pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.comments[pointer], resultComments[pointer]);
			} while (GCode.comments[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultComments2[] = { ';', 'C', 'o', 'm', 'm', 'e', 't', ' ', 't', 'o', ' ', 'e',
				'n', 'd', ' ', 'o', 'f', ' ', 'l', 'i',	'n', 'e', '.', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.comments[pointer], resultComments2[pointer]);
			} while (GCode.comments[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultComments3[] = { '(', 'C', 'o', 'm', 'm', 'e', 'n', 't', ')', ' ', 'H', 'e', 'r', 'e', ')', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.comments[pointer], resultComments3[pointer]);
			} while (GCode.comments[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultComments4[] = { '(', 'B', 'l', 'o', 'c', 'k', ' ', ';', 'D', 'e', 'l',
				'e', 't', 'e', ')', ';', 'M', 'S', 'G', ',', '1', '2', '3', '(', '?', ')', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.comments[pointer], resultComments4[pointer]);
			} while (GCode.comments[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultComments7[] = { '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.comments[pointer], resultComments7[pointer]);
			} while (GCode.comments[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultComments6[] = { '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.comments[pointer], resultComments6[pointer]);
			} while (GCode.comments[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultComments5[] = { '(', 'C', 'o', 'm', 'm', 'e', 'n', 't', ' ',	'H', 'e', 'r', 'e', ')', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.comments[pointer], resultComments5[pointer]);
			} while (GCode.comments[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultComments9[] = { ';', 'C', 'o', 'm', 'm', 'e', 'n', 't', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.comments[pointer], resultComments9[pointer]);
			} while (GCode.comments[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultComments8[] = { '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.comments[pointer], resultComments8[pointer]);
			} while (GCode.comments[pointer] != '\0');

			GCode.ParseLine("G00 X0 Y3200;Comment");

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.comments[pointer], resultComments9[pointer]);
			} while (GCode.comments[pointer] != '\0');
		}

		TEST_METHOD(ParseLine_ConfirmLastComment)
		{
			GCodeParser GCode = GCodeParser();

			int startAt = AddLine(&GCode, 0);

			GCode.ParseLine();

			char resultLastComment[] = { '(', 'C', 'o', 'm', 'm', 'e', 'n', 't', ' ', 'H', 'e', 'r', 'e', ')', '\0' };

			int pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.lastComment[pointer], resultLastComment[pointer]);
			} while (GCode.lastComment[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultLastComment2[] = { ';', 'C', 'o', 'm', 'm', 'e', 't', ' ', 't', 'o', ' ', 'e',
				'n', 'd', ' ', 'o', 'f', ' ', 'l', 'i',	'n', 'e', '.', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.lastComment[pointer], resultLastComment2[pointer]);
			} while (GCode.lastComment[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultLastComment3[] = { '(', 'C', 'o', 'm', 'm', 'e', 'n', 't', ')', ' ', 'H', 'e', 'r', 'e', ')', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.lastComment[pointer], resultLastComment3[pointer]);
			} while (GCode.lastComment[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultLastComment4[] = { ';', 'M', 'S', 'G', ',', '1', '2', '3', '(', '?', ')', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.lastComment[pointer], resultLastComment4[pointer]);
			} while (GCode.lastComment[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultLastComment7[] = { '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.lastComment[pointer], resultLastComment7[pointer]);
			} while (GCode.lastComment[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultLastComment6[] = { '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.lastComment[pointer], resultLastComment6[pointer]);
			} while (GCode.lastComment[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultLastComment5[] = { '(', 'C', 'o', 'm', 'm', 'e', 'n', 't', ' ',	'H', 'e', 'r', 'e', ')', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.lastComment[pointer], resultLastComment5[pointer]);
			} while (GCode.lastComment[pointer] != '\0');
			
			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultLastComment9[] = { ';', 'C', 'o', 'm', 'm', 'e', 'n', 't', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.lastComment[pointer], resultLastComment9[pointer]);
			} while (GCode.lastComment[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			char resultLastComment8[] = { '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.lastComment[pointer], resultLastComment8[pointer]);
			} while (GCode.lastComment[pointer] != '\0');

			GCode.ParseLine("/G21 (Block ;Delete) G90 ;MSG,123(?)");

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.lastComment[pointer], resultLastComment4[pointer]);
			} while (GCode.lastComment[pointer] != '\0');
		}

		TEST_METHOD(ParseLine_ConfirmBlockDelete)
		{
			GCodeParser GCode = GCodeParser();

			int startAt = AddLine(&GCode, 0);

			GCode.ParseLine();

			Assert::AreEqual(GCode.blockDelete, false);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.blockDelete, false);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.blockDelete, false);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.blockDelete, true);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.blockDelete, false);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.blockDelete, false);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.blockDelete, false);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.blockDelete, false);

			GCode.ParseLine("/M300 ;Block Delete");

			Assert::AreEqual(GCode.blockDelete, true);

			GCode.ParseLine("M300 ;Block Delete");

			Assert::AreEqual(GCode.blockDelete, false);
		}

		TEST_METHOD(ParseLine_ConfirmBeginEnd)
		{
			GCodeParser GCode = GCodeParser();

			int startAt = AddLine(&GCode, 0);

			GCode.ParseLine();

			Assert::AreEqual(GCode.beginEnd, false);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.beginEnd, false);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.beginEnd, false);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.beginEnd, false);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.beginEnd, false);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.beginEnd, false);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.beginEnd, false);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.beginEnd, false);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.beginEnd, true);

			GCode.ParseLine("%");

			Assert::AreEqual(GCode.beginEnd, true);

			GCode.ParseLine("G00 X20 Y20");

			Assert::AreEqual(GCode.beginEnd, false);
		}

		TEST_METHOD(ParseLine_ConfirmNoWords)
		{
			GCodeParser GCode = GCodeParser();

			int startAt = AddLine(&GCode, 0);

			GCode.ParseLine();

			Assert::AreEqual(GCode.NoWords(), false);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.NoWords(), false);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.NoWords(), false);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.NoWords(), true);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.NoWords(), true);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.NoWords(), false);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.NoWords(), true);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.NoWords(), true);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.NoWords(), true);

			GCode.ParseLine("zf00^dsa&f%df; Comment");

			Assert::AreEqual(GCode.NoWords(), true);

			GCode.ParseLine("M300 ;Comment");

			Assert::AreEqual(GCode.NoWords(), false);
		}

		TEST_METHOD(RemoveCommentSeparators_ConfirmComments)
		{
			GCodeParser GCode = GCodeParser();

			int startAt = AddLine(&GCode, 0);

			GCode.ParseLine();
			GCode.RemoveCommentSeparators();

			char resultComments[] = { 'C', 'o', 'm', 'm', 'e', 'n', 't', ' ', 'H', 'e', 'r', 'e', '\0' };

			int pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.comments[pointer], resultComments[pointer]);
			} while (GCode.comments[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();
			GCode.RemoveCommentSeparators();

			char resultComments2[] = { 'C', 'o', 'm', 'm', 'e', 't', ' ', 't', 'o', ' ', 'e',
				'n', 'd', ' ', 'o', 'f', ' ', 'l', 'i',	'n', 'e', '.', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.comments[pointer], resultComments2[pointer]);
			} while (GCode.comments[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();
			GCode.RemoveCommentSeparators();

			char resultComments3[] = { 'C', 'o', 'm', 'm', 'e', 'n', 't', ')', ' ', 'H', 'e', 'r', 'e', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.comments[pointer], resultComments3[pointer]);
			} while (GCode.comments[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();
			GCode.RemoveCommentSeparators();

			char resultComments4[] = { 'B', 'l', 'o', 'c', 'k', ' ', ';', 'D', 'e', 'l',
				'e', 't', 'e', ' ', 'M', 'S', 'G', ',', '1', '2', '3', '(', '?', ')', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.comments[pointer], resultComments4[pointer]);
			} while (GCode.comments[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();
			GCode.RemoveCommentSeparators();

			char resultComments7[] = { '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.comments[pointer], resultComments7[pointer]);
			} while (GCode.comments[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();
			GCode.RemoveCommentSeparators();

			char resultComments6[] = { '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.comments[pointer], resultComments6[pointer]);
			} while (GCode.comments[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();
			GCode.RemoveCommentSeparators();

			char resultComments5[] = { 'C', 'o', 'm', 'm', 'e', 'n', 't', ' ',	'H', 'e', 'r', 'e', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.comments[pointer], resultComments5[pointer]);
			} while (GCode.comments[pointer] != '\0');
						
			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();
			GCode.RemoveCommentSeparators();

			char resultComments9[] = { 'C', 'o', 'm', 'm', 'e', 'n', 't','\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.comments[pointer], resultComments9[pointer]);
			} while (GCode.comments[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();
			GCode.RemoveCommentSeparators();

			char resultComments8[] = { '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.comments[pointer], resultComments8[pointer]);
			} while (GCode.comments[pointer] != '\0');
		}

		TEST_METHOD(RemoveCommentSeparators_ConfirmLastComments)
		{
			GCodeParser GCode = GCodeParser();

			int startAt = AddLine(&GCode, 0);

			GCode.ParseLine();
			GCode.RemoveCommentSeparators();

			char resultLastComment[] = { 'C', 'o', 'm', 'm', 'e', 'n', 't', ' ', 'H', 'e', 'r', 'e', '\0' };

			int pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.lastComment[pointer], resultLastComment[pointer]);
			} while (GCode.lastComment[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();
			GCode.RemoveCommentSeparators();

			char resultLastComment2[] = { 'C', 'o', 'm', 'm', 'e', 't', ' ', 't', 'o', ' ', 'e',
				'n', 'd', ' ', 'o', 'f', ' ', 'l', 'i',	'n', 'e', '.', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.lastComment[pointer], resultLastComment2[pointer]);
			} while (GCode.lastComment[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();
			GCode.RemoveCommentSeparators();

			char resultLastComment3[] = { 'C', 'o', 'm', 'm', 'e', 'n', 't', ')', ' ', 'H', 'e', 'r', 'e', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.lastComment[pointer], resultLastComment3[pointer]);
			} while (GCode.lastComment[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();
			GCode.RemoveCommentSeparators();

			char resultLastComment4[] = {'M', 'S', 'G', ',', '1', '2', '3', '(', '?', ')', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.lastComment[pointer], resultLastComment4[pointer]);
			} while (GCode.lastComment[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();
			GCode.RemoveCommentSeparators();

			char resultLastComment7[] = { '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.lastComment[pointer], resultLastComment7[pointer]);
			} while (GCode.lastComment[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();
			GCode.RemoveCommentSeparators();

			char resultLastComment6[] = { '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.lastComment[pointer], resultLastComment6[pointer]);
			} while (GCode.lastComment[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();
			GCode.RemoveCommentSeparators();

			char resultLastComment5[] = { 'C', 'o', 'm', 'm', 'e', 'n', 't', ' ', 'H', 'e', 'r', 'e', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.lastComment[pointer], resultLastComment5[pointer]);
			} while (GCode.lastComment[pointer] != '\0');
						
			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();
			GCode.RemoveCommentSeparators();

			char resultLastComment9[] = { 'C', 'o', 'm', 'm', 'e', 'n', 't', '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.lastComment[pointer], resultLastComment9[pointer]);
			} while (GCode.lastComment[pointer] != '\0');

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();
			GCode.RemoveCommentSeparators();

			char resultLastComment8[] = { '\0' };

			pointer = -1;
			do {
				pointer++;
				Assert::AreEqual(GCode.lastComment[pointer], resultLastComment8[pointer]);
			} while (GCode.lastComment[pointer] != '\0');
		}

		TEST_METHOD(IsWord_ValidWord_ReturnsTrue)
		{
			GCodeParser GCode = GCodeParser();

			Assert::AreEqual(GCode.IsWord('G'), true);
		}

		TEST_METHOD(IsWord_InvalidWord_ReturnsFalse)
		{
			GCodeParser GCode = GCodeParser();

			Assert::AreEqual(GCode.IsWord('E'), false);
		}
		
		TEST_METHOD(HasWord_NoLine_ReturnsFalse)
		{
			GCodeParser GCode = GCodeParser();

			Assert::AreEqual(GCode.HasWord('G'), false);
		}

		TEST_METHOD(HasWord_WordExist_ReturnsTrue)
		{
			GCodeParser GCode = GCodeParser();

			int startAt = AddLine(&GCode, 0);

			GCode.ParseLine();

			Assert::AreEqual(GCode.HasWord('G'), true);
			Assert::AreEqual(GCode.HasWord('Z'), true);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.HasWord('M'), true);
			Assert::AreEqual(GCode.HasWord('S'), true);

			AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.HasWord('Y'), true);
			Assert::AreEqual(GCode.HasWord('Z'), true);
		}

		TEST_METHOD(HasWord_WordDoesNotExist_ReturnsFalse)
		{
			GCodeParser GCode = GCodeParser();

			int startAt = AddLine(&GCode, 0);

			GCode.ParseLine();

			Assert::AreEqual(GCode.HasWord('M'), false);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.HasWord('C'), false);

			AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.HasWord('C'), false);
			Assert::AreEqual(GCode.HasWord('H'), false);
		}

		TEST_METHOD(GetWordValue_ExistingWord_ReturnsValue)
		{
			GCodeParser GCode = GCodeParser();

			int startAt = AddLine(&GCode, 0);

			GCode.ParseLine();

			Assert::AreEqual((int)GCode.GetWordValue('G'), 1);
			Assert::AreEqual(GCode.GetWordValue('Z'), 0.0);

			startAt = AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual((int)GCode.GetWordValue('M'), 300);
			Assert::AreEqual(GCode.GetWordValue('S'), 125.0);

			AddLine(&GCode, startAt);

			GCode.ParseLine();

			Assert::AreEqual(GCode.GetWordValue('X'), 3.2);
			Assert::AreEqual(GCode.GetWordValue('Z'), 5.0);
		}
	};
}

#include <GCodeParser.h>

GCodeParser GCode = GCodeParser();

void setup() 
{
  Serial.begin(115200);

  Serial.println("Ready");
  delay(100);
}

void loop() 
{ 
  if (Serial.available() > 0)
  {
    if (GCode.AddCharToLine(Serial.read()))
    {
      GCode.ParseLine();
      // Code to process the line of G-Code here…

      Serial.print("Command Line: ");
      Serial.println(GCode.line);

      GCode.RemoveCommentSeparators();

      Serial.print("Comment(s): ");
      Serial.println(GCode.comments);
      
      if (GCode.HasWord('G'))
      {
        Serial.print("Process G code: ");
        Serial.println((int)GCode.GetWordValue('G'));
      }
    }
  }
}

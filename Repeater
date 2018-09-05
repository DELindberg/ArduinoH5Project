void Repeater()
{
  static int RecordingCountdown = 100;//Recording interval  

  static int IntervalCountdown = 0;//Duration between blinks
  IntervalFromAnalog = analogRead(potentiometerPin) /100;
  if(IntervalCountdown <= 0)
  {
    NumberOfBlinks++;
    //Serial.println(NumberOfBlinks);
    IntervalCountdown = IntervalFromAnalog; 
  }
  else
  {
    IntervalCountdown--;
  }

  if(RecordingCountdown <= 0)
  {
    RecordingCountdown = 100;
    Kwh.enqueue((NumberOfBlinks));
    NumberOfBlinks = 0; //Reset number of blinks for next recording
    Serial.println(Kwh.count());
  }
  else
  {
    RecordingCountdown--;
  }
}

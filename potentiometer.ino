#include <SimpleFIFO.h>
#include <DueTimer.h>

int potentiometerPin = A0;    // select the input pin for the potentiometer
int val = 0;       // variable to store the value coming from the sensor
int IntervalFromAnalog = 0;
SimpleFIFO<float, 60> Kwh;
static int NumberOfBlinks = 0;  //How many blinks have occurred since previous recording

void setup() {
   // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200); //Open serial communication
  Timer0.attachInterrupt(Repeater).start(10000); //10ms
}


void loop() {
    //Serial.println(val);
    //val = analogRead(potentiometerPin);    // read the value from the sensor
}

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
    //Serial.println("Recording...");
  }
  else
  {
    RecordingCountdown--;
  }

  if(Kwh.count() > 5)//If more than a specific number of counts have occurred
  {
    //Serial.println("exceeding 5, dequeuing:");

    while(Kwh.count() > 0)
    {
         Serial.println(Kwh.dequeue());
    }
    
  }
  
}

void Blink()
{
  //Serial.println("blink");
  NumberOfBlinks++;
}


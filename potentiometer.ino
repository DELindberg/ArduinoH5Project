/*
 * Included libraries
 */
#include <SimpleFIFO.h>
#include <DueTimer.h>
#include <SPI.h>
#include <Ethernet2.h>


/*
 *  Potentiometer global variables
  */
int potentiometerPin = A0;    // select the input pin for the potentiometer
int val = 0;       // variable to store the value coming from the sensor
int IntervalFromAnalog = 0;
SimpleFIFO<float, 60> Kwh;
static int NumberOfBlinks = 0;  //How many blinks have occurred since previous recording


/*
 * Webserver global variables
 * Server mac, ip, and port address'
 */
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED,
};
IPAddress ip(192,168,1,1);
EthernetServer server(80);

void setup() {
  /* 
   * initialize digital pin LED_BUILTIN as an output. 
   */
  pinMode(LED_BUILTIN, OUTPUT);
  /*
   * Open serial communication
   */
  Serial.begin(115200);
  /*
   * Timer runs function Repeater every 10000 microseconds. IE. 10 milliseconds.
   */
  Timer0.attachInterrupt(Repeater).start(10000); //10ms

  /*
   * Webserver code start
   */
   Ethernet.begin(mac,ip);
   
   /*
    * Check if Ethernet shield is equipped on the DUE board, if not present halt program
    */
//   if (Ethernet.hardwareStatus() == EthernetNoHardware) {
//    while(true){
//      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
//    }
//  }
  /*
   * Check if there is an Ethernet cable connected, if not present halt program
   */
//  if (Ethernet.linkStatus() == LinkOFF) {
//    while(true){
//      Serial.println("Ethernet cable is not connected.");
//    }
//  }

  /*
   * Start the server
   */
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP()); 
}

void loop() {
  static float deqVal;
  if(Kwh.count() > 5)//If more than a specific number of counts have occurred
  {
   /*
   * Feeds the client the incoming bytes from the webserver
   */
    EthernetClient client = server.available();
  
    /*
     * If client is connected
     */
    if(client){
      boolean currentLineIsBlank = true;
      while(client.connected()){
        if(client.available()) {
          char c = client.read();
          /*
           * Sends a http header that automatically refreshes the page every 5 seconds
           */
           if (c == '\n' && currentLineIsBlank) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println("Refresh: 5");
            client.println();
            /*
             * Send HTML start to client
             */
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            /*
             *  For every value in the FIFO queue
             */
            while(Kwh.count() > 0)
            {
              /*  
               *   Dequeue from the FIFO queue to a temp variable for printing
               */
              deqVal = Kwh.dequeue();
              Serial.println("b");
              //Serial.println(Kwh.count());
              client.println(deqVal);
              client.println("<br />");
            }
            Serial.println("y");
            /*
             * Send HTML close tag to client
             */
            client.println("</html>");
            break;
           }
           if (c == '\n') {
             // you're starting a new line
             currentLineIsBlank = true;
          }
          else if (c != '\r') {
            // you've gotten a character on the current line
            currentLineIsBlank = false;
          }
        }
      }
      // give the web browser time to receive the data
      delay(1);
      Serial.println("x");
      // close the connection:
      client.stop();
      }
  }
}

void Blink()
{
  //Serial.println("blink");
  NumberOfBlinks++;
}

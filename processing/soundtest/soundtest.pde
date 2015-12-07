import processing.serial.*;
import processing.sound.*;
import processing.net.*;

// Toggles simulation environment, 
// processing to processing instead of arduino to processing
boolean sim = false;
boolean phoneControl = false;

OscHandler oscHandler;

// Server for simulation
Server server;
Client client;
int serverPort = 5204;

// ***** Serial communication ***
String value;
Serial arduinoPort;
int portRate = 9600;

Sound[] files;

// ***** Arduiono Protocol **********
//String oneDown = "B1D";
//String oneUp = "B1U";
char terminator = '\n';

void setup() {
  files = new Sound[9];
  
  // Initiate phone control
  if (phoneControl) {
    oscHandler = new OscHandler();
  }
  
  // Arduino or computer input
  if (!sim) {
    int portNumber = 1;
    String portName = Serial.list()[portNumber];
    arduinoPort = new Serial(this, portName, portRate);
    arduinoPort.bufferUntil('\n');
  } else {
    server = new Server(this, serverPort);
  }
  
  // Screen Setup
  size(200, 200);
  background(255,60,50);
 
  // *********** SoundFiles **************************** 
  // Load soundfiles from the /data folder of the sketch
  final int fileAmount = 8;
  for (int i=0; i<fileAmount; i++) {
    //files[i] = new SoundFile(this, (i+1) + ".wav");
    SoundFile newFile = new SoundFile(this, (i+1) + ".wav"); 
    files[i] = new Sound(newFile);
  }
}    

void draw() {
  // Simulation input. Otherwise handled as a SerialEvent
  if (sim) {
    client = server.available();
    if (client != null) {
      value = client.readStringUntil(terminator);
      handleInput(value);
    }
  }
}

// Handles all events from the serial port
void serialEvent(Serial arduinoPort) {
  // Data available
    value = arduinoPort.readStringUntil('\n');
    handleInput(value);
}

void handleInput(String value) {
  if (value != null) {
    value = value.trim();
    println("From Arduino: " + value);
    char firstChar = value.charAt(0);
    if (firstChar == 'B') {
        char secondChar = value.charAt(1);
        char thirdChar = value.charAt(2);
        int buttonNumber = Character.getNumericValue(secondChar);
        if (thirdChar == 'D') {
          if (files[buttonNumber] != null) {
            //playSound(files[buttonNumber]);
            files[buttonNumber].play();
            // Tell the arduino the duration of the sample
            //println(files[buttonNumber].duration());
            String message = "B" + buttonNumber + files[buttonNumber].duration + terminator;
            //println("To arduino: " + message);
            if (!sim) {
              arduinoPort.write(message);
            } else {
              server.write(message);
            }
          } else {
            println("Sound file missing");
          }
          
         } else if (thirdChar == 'U') {
          // Button released 
         }
    }
  }
}
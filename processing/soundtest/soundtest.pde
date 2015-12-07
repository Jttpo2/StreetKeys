import processing.serial.*;
import processing.sound.*;
import processing.net.*;

// Toggles simulation environment, 
// processing to processing instead of arduino to processing
boolean sim = false; 

OscHandler oscHandler;

// Server for simulation
Server server;
Client client;
int serverPort = 5204;

// ***** Serial communication ***
String value;
Serial arduinoPort;
int portRate = 9600;
//boolean firstContact = true;
//boolean firstContact = false;

SoundFile[] files;

// ***** Arduiono Protocol **********
//String oneDown = "B1D";
//String oneUp = "B1U";
char terminator = '\n';

void setup() {
  
  files = new SoundFile[9];
  
  // Initiate phone control
  oscHandler = new OscHandler();
  
  // Arduino or computer input
  if (!sim) {
    int portNumber = 1;
    String portName = Serial.list()[portNumber];
    arduinoPort = new Serial(this, portName, portRate);
    arduinoPort.bufferUntil('\n');
  } else {
    server = new Server(this, serverPort);
  }
  
  size(200, 200);
  background(255,60,50);
 
  // *********** SoundFiles **************************** 
  // Load soundfiles from the /data folder of the sketch
  int fileAmount = 8;
  for (int i=0; i<fileAmount; i++) {
    files[i] = new SoundFile(this, (i+1) + ".wav");
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

  // Play file with correct rate, despite its sample rate 
void playSound(SoundFile file) {
  if (file == null) {
    println("Sound file missing");
    return;
  }
  float rate = getPlayRate(file);
  file.rate(rate);
  file.play();
}

float getPlayRate(SoundFile file) {
  int sampleRate = file.sampleRate();
    float playRate = 44100 / sampleRate;
    playRate = 1/playRate;
    return playRate;
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
            playSound(files[buttonNumber]);
            // Tell the arduino the duration of the sample
            //println(files[buttonNumber].duration());
            String message = "B" + buttonNumber + files[buttonNumber].duration() + terminator;
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
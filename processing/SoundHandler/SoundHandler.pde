import processing.serial.*;
import processing.sound.*;
import processing.net.*;

// Toggles simulation environment, 
// processing to processing instead of arduino to processing
boolean sim = false;
boolean phoneControl = false;

OscHandler oscHandler;
SoundObserver soundObserver;

// Server for simulation
Server server;
Client client;
int serverPort = 5204;

// ***** Serial communication ***
String value;
Serial arduinoPort;
int portRate = 9600;

Sound[] sounds;

// ***** Arduiono Protocol **********
//String oneDown = "B1D";
//String oneUp = "B1U";
char terminator = '\n';

void setup() {
  sounds = new Sound[9];
  soundObserver= new SoundObserver(this);
  
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
    SoundFile newFile = new SoundFile(this, (i+1) + ".wav"); 
    Sound newSound = new Sound(newFile, i); 
    newSound.addObserver(soundObserver);
    sounds[i] = newSound;
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
  
  // Give sounds a chance to loop or stop
  for (Sound s: sounds) {
    if (s != null) {
      s.update();
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
        if (sounds[buttonNumber] != null) {
          if (thirdChar == 'D') {
            sounds[buttonNumber].loop();
            // Tell the arduino the duration of the sample
            //String message = "B" + buttonNumber + sounds[buttonNumber].duration + terminator;
            //println("To arduino: " + message);
            
          } else if (thirdChar == 'U') {
          // Button released
            sounds[buttonNumber].stopAfterCurrent();
            
          }
        } else {
          println("Sound file missing");
        }
      }
    }
  }
  
  void communicate(Sound sound) {
    String message = "B" + sound.id + sound.duration + terminator;
    // Tell the arduino the duration of the sample
    if (!sim) {
      arduinoPort.write(message);
    } else {
      server.write(message);
    }
  }
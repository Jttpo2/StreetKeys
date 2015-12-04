import processing.serial.*;
import processing.sound.*;

// ***** Serial communication ***
String value;
Serial arduinoPort;
int portRate = 9600;
//boolean firstContact = true;
boolean firstContact = false;

SoundFile[] files;

// ***** Arduiono Protocol **********
//String oneDown = "B1D";
//String oneUp = "B1U";
char terminator = '\n';

void setup() {
  
  files = new SoundFile[9];
  
  int portNumber = 1;
  String portName = Serial.list()[portNumber];
  arduinoPort = new Serial(this, portName, portRate);
  arduinoPort.bufferUntil('\n');
  
  size(200, 200);
  background(255,60,50);
 
  // *********** SoundFiles **************************** 
  // Load soundfiles from the /data folder of the sketch
  int fileAmount = 2;
  for (int i=0; i<fileAmount; i++) {
    files[i] = new SoundFile(this, (i+1) + ".wav");
  }
}    

void draw() {
}

  // Play file with correct rate, despite its sample rate 
void playSound(SoundFile file) {
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
    if (value != null) {
      value = value.trim();
      println("From Arduino: " + value);
      
      if (!firstContact) {
        if (value.equals("A")) {
          arduinoPort.clear();
          firstContact = true;
          arduinoPort.write("A");
          println("contact made with arduino");
        }
      } else { //if we've already established contact, keep getting and parsing data
         char firstChar = value.charAt(0);
         if (firstChar == 'B') {
           char secondChar = value.charAt(1);
           char thirdChar = value.charAt(2);
           int buttonNumber = Character.getNumericValue(secondChar);
           if (thirdChar == 'D') {
            playSound(files[buttonNumber]);
             // Tell the arduino the duration of the sample
             //println(files[buttonNumber].duration());
             arduinoPort.write("B" + buttonNumber + files[buttonNumber].duration() + terminator);
           } else if (thirdChar == 'U') {
            // Button released 
           }
         }
         
          // ask for more data
          //println("sending A to arduino");
          //arduinoPort.write("A");
        
      }
    }
}
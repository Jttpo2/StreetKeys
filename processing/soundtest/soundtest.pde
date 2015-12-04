import processing.serial.*;
import processing.sound.*;

String value;
Serial arduinoPort;
int portRate = 9600;

SoundFile file;
SoundFile file2;

String fileName2 = "test.wav";
String fileName = "bugsbunny2.wav";
float playbackRate = 0.25;

String oneDown = "1D";
String oneUp = "1U";

void setup() {
  
  int portNumber = 1;
  String portName = Serial.list()[portNumber];
  arduinoPort = new Serial(this, portName, portRate);
  
  size(640, 360);
  background(255);
 
  // *********** SoundFiles ****************************
  // Load a soundfile from the /data folder of the sketch and play it back
  file = new SoundFile(this, fileName);
  file.rate(playbackRate);
 // file.play();
  
  // Load a soundfile from the /data folder of the sketch and play it back
  file2 = new SoundFile(this, fileName2);
  file2.rate(playbackRate);
  //file2.play();
  
}      

void draw() {
  if (arduinoPort.available() > 0) {
    // Data available
    value = arduinoPort.readStringUntil('\n');
    // Remove newline
    value = value.trim();
    if (value.equals(oneDown)) {
      file2.play();
    }
    println(value);
  }
  
  
}
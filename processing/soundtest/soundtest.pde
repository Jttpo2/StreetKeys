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

void setup() {
  
  int portNumber = 1;
  String portName = Serial.list()[portNumber];
  arduinoPort = new Serial(this, portName, portRate);
  
  size(640, 360);
  background(255);
    
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
    //if (value.equals("S")) {
      file2.play();
    //}
    println(value);
  }
  
  
}
import processing.serial.*;
import processing.sound.*;

// ***** Serial communication ***
String value;
Serial arduinoPort;
int portRate = 9600;

SoundFile[] files;

// ***** Arduiono Protocol **********
String oneDown = "1D";
String oneUp = "1U";
char terminator = '\n';

void setup() {
  
  files = new SoundFile[9];
  
  int portNumber = 1;
  String portName = Serial.list()[portNumber];
  arduinoPort = new Serial(this, portName, portRate);
  
  size(640, 360);
  background(255);
 
  // *********** SoundFiles **************************** 
  // Load soundfiles from the /data folder of the sketch
  int fileAmount = 2;
  for (int i=0; i<fileAmount; i++) {
    files[i] = new SoundFile(this, (i+1) + ".wav");
  }
}    

void draw() {
  if (arduinoPort.available() > 0) {
    // Data available
    value = arduinoPort.readStringUntil('\n');
    // Remove newline
    value = value.trim();
    println("From Arduino: " + value);
    if (value.equals(oneDown)) {
      //files[1].play();
      playSound(files[1]);
      // Tell the arduino the duration of the sample
      arduinoPort.write(oneDown + files[1].duration() + terminator);
    }
    //println(files[1].duration());
    //println(getDuration(files[1]));
  }
  
  //if (mousePressed) {
  //  arduinoPort.write(oneDown + terminator);
  //  println("yeah");
  //}
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
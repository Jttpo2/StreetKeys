import processing.serial.*;
import processing.sound.*;

String value;
Serial arduinoPort;
int portRate = 9600;

SoundFile[] files;

String fileName2 = "test.wav";
String fileName = "bugsbunny2.wav";
float playbackRate = 1;

String oneDown = "1D";
String oneUp = "1U";

void setup() {
  
  files = new SoundFile[9];
  
  int portNumber = 1;
  String portName = Serial.list()[portNumber];
  arduinoPort = new Serial(this, portName, portRate);
  
  size(640, 360);
  background(255);
 
  // *********** SoundFiles ****************************
  // Load a soundfile from the /data folder of the sketch and play it back
  files[0] = new SoundFile(this, fileName);
  files[0].rate(playbackRate);
 // files[0].play();
  
  // Load a soundfile from the /data folder of the sketch and play it back
  files[1] = new SoundFile(this, fileName2);
  files[1].rate(playbackRate);
  //files
  //files[1].play();
  
}      

void draw() {
  if (arduinoPort.available() > 0) {
    // Data available
    value = arduinoPort.readStringUntil('\n');
    // Remove newline
    value = value.trim();
    if (value.equals(oneDown)) {
      //files[1].play();
      playSound(files[1]);
    }
    println(files[1].duration());
    //println(getDuration(files[1]));
  }
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

// UNNECESSARY Get file duration, regardless of its sample rate
float getDuration(SoundFile file) {
  float rate = getPlayRate(file);
  
  float duration = (file.duration() * (1/rate));  
  println("rate: "+ duration);
  return duration;
}
  
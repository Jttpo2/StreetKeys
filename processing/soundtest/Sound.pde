class Sound {
  SoundFile file;
  float duration;
  boolean isPlaying;
  boolean loop;
  boolean stopAfterCurrent;
  long startTime;
  
  // For shortening the sample interval slightly, 
  // so looping sounds smoother
  final static int modifier = 50; 
  
  Sound(SoundFile file) {
    this.file = file;
    duration = file.duration();
    isPlaying = false;
    loop = false;
  }
  
  void play() {
    startSample();
    loop = false;
  }
  
  void loop() {
    startSample();
    loop = true;
  }
  
  void stopAfterCurrent() {
    loop = false;
  }
  
  void startSample() {
    // Stop old sample playing first
    file.stop();
    // Play file at right speed, despite its sample rate
    float rate = getPlayRate();
    file.rate(rate);
    file.play();
    startTime = millis();
    isPlaying = true;
  }
  
  float getPlayRate() {
    int sampleRate = file.sampleRate();
    float playRate = 44100 / sampleRate;
    playRate = 1/playRate;
    return playRate;
  } 
  
  void update() {
    if (isPlaying) {
      // Check to see whether the clip is still playing
      if (millis() - startTime < (duration * 1000 - modifier)) {
        // Still playing
      } else {
        println("Sample done");
        // Done playing one round
        isPlaying = false;
        if (loop) {
          startSample();
        } else {
          // Enjoy the silence
        }
      }
    }
  }
  
}
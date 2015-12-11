import java.util.Observer;

class SoundObserver implements Observer {
  SoundHandler soundHandler;
  
  SoundObserver(SoundHandler handler) {
    soundHandler = handler;
    println("Init observer");
  }
  
  void update(Observable o, Object arg) {
    println("SoundObserver update");
    soundHandler.communicate((Sound) o);
  }
}
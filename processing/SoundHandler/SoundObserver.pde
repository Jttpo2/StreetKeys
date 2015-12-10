import java.util.Observer;

class SoundObserver implements Observer {
  SoundHandler soundHandler;
  
  SoundObserver(SoundHandler handler) {
    soundHandler = handler;
  }
  
  void update(Observable o, Object arg) {
    soundHandler.communicate((Sound) o);
  }
}
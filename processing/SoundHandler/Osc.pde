import netP5.*;
import oscP5.*;

class OscHandler {
  OscP5 osc;
  int oscPort = 5000;
  
  String buttonLabel = "/1/button" ;
  
  OscHandler() {
    init();
  }
    
  void init() {
    osc = new OscP5(this, oscPort);
    //osc.plug(this, "processButtonInput", buttonLabel);  
  }
  
  void processButtonInput(int i) {
    println(i);
  }
  
  void oscEvent(OscMessage m) {
    println(m);
    
    String buttonPart = m.addrPattern().substring(0, 9); // nine first chars of "/1/button1" //<>//
    //println(buttonPart);
    if (buttonPart.equals(buttonLabel) && (int)m.arguments()[0] == 0) { 
      // Correct button label and the argument 1 indicates that button has pressed
      int buttonNumber = Character.getNumericValue(m.addrPattern().charAt(9)) -1;
     //println(buttonNumber);
     //String translatedMessage = "B" + buttonNumber + "D"; // Button pressed
     String translatedMessage = "B" + buttonNumber + "P"; // Play once
     // Send to main class for handling
     handleInput(translatedMessage);
     
     //delay(10);
     // Since OSC only gives button down presses we have to send the release event here as well
     //String upMessage = "B" + buttonNumber + "U"; // Button released
     //handleInput(upMessage);
    }
  }
}
  
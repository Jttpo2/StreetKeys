//import processing.serial.*;
import processing.net.*; 

Timer timer;
static int numberOfTimers; // Hack to have timer id's

Client client;
int serverPort = 5204;
String value;

// Protocol
char terminator = '\n';

int counter = 0;
color color1 = #88164A;
color color2 = #629418;
color color3 = #A1481A;
color color4 = #126B4E;

int rectSide;

Pad[] pads;

boolean mouseIsPressed = false;

void setup() {
  
  size(210, 210);
  background(color4);
  rectSide = width/3;
  
  pads = new Pad[9];
  pads[0] = new Pad(0,0,rectSide, color1);
  pads[1] = new Pad(rectSide, 0, rectSide, color2);
  pads[2] = new Pad(rectSide*2, 0, rectSide, color3);
  
  pads[3] = new Pad(0, rectSide, rectSide, color2);
  pads[4] = new Pad(rectSide, rectSide, rectSide, color3);
  pads[5] = new Pad(rectSide*2, rectSide, rectSide, color4);
  
  pads[6] = new Pad(0, rectSide*2, rectSide, color3);
  pads[7] = new Pad(rectSide, rectSide*2, rectSide, color4);
  pads[8] = new Pad(rectSide*2, rectSide*2, rectSide, color1);
 
  // Connect to server
  client = new Client(this, "127.0.0.1", serverPort);
  
  timer = new Timer(1000);
  //timer.start();
}

void draw() {

  for (int i=0; i<pads.length; i++) {
    if (pads[i] != null) {
      pads[i].display();
    }
  }
  
  if (mousePressed) {
    if (!mouseIsPressed){ // To only get one mouseevent per press
      mouseIsPressed = true;
      for (int i=0; i<pads.length; i++) {
        if (pads[i].isWithin(mouseX, mouseY)) {
          pads[i].press();
          println("pressed: " + (i));
          client.write("B" + i + "D" + terminator);
          break;
        }
      }
    }
  } else {
    mouseIsPressed = false;
  }
 
  timer.update();
}

void clientEvent(Client someClient) {
  value = someClient.readStringUntil(terminator);
  if (value != null) {
    println(value);
    
  }
  
}
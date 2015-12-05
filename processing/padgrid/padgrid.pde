int counter = 0;
color color1 = #88164A;
color color2 = #629418;
color color3 = #A1481A;
color color4 = #126B4E;

int rectSide;

Pad[] pads;

void setup() {
  size(300, 300);
  background(color4);
  rectSide = width/3;
  
  pads = new Pad[9];
  pads[0] = new Pad(0,0,rectSide, color1);
}

void draw() {
  for (int i=0; i<pads.length; i++) {
    if (pads[i] != null) {
      
    }
  }
  pads[0].display();
  
  //fill(color1);
  //rect(0, 0, rectSide, rectSide);
  //fill(color2);
  //rect(rectSide, 0, rectSide, rectSide);
  //fill(color3);
  //rect(rectSide*2, 0 , rectSide, rectSide);
  
  //fill(color3);
  //rect(0, rectSide, rectSide, rectSide);
  //fill(color4);
  //rect(rectSide, rectSide, rectSide, rectSide);
  //fill(color1);
  //rect(rectSide*2, rectSide, rectSide, rectSide);
  
  //fill(color2);
  //rect(0, rectSide*2, rectSide, rectSide);
  //fill(color3);
  //rect(rectSide, rectSide*2, rectSide, rectSide);
  //fill(color4);
  //rect(rectSide*2, rectSide*2, rectSide, rectSide);
  
  if (mousePressed) {
    println("pressed" + counter++);
    fill(color1);
  } else {
    fill(color2);
  }
 
}
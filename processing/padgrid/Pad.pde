class Pad {
   final int interval = 30;
   final int fullBrightness = 255;
  
   int x;
   int y;
   int size;
   color c;
   color borderColor = 150;
   color pressedColor;
   
   Pad(int x, int y, int size, color c) {
      this.x = x;
      this.y = y;
      this.size = size;
      this.c = c;
      pressedColor = this.c - 100;
   }
   
   void display() {
     fill(c);
     stroke(borderColor);
     rect(x, y, size, size);
   }
   
   void press() {
     fill(pressedColor);
     stroke(borderColor);
     rect(x, y, size, size);
   }
   
   void lightAndFade(float duration) {
     //fill(pressedColor);
     //stroke(borderColor);
     //rect(x, y, size, size);
     
     // find out how many times dimming should occur
     float pieces = duration / interval;
     // find out how much to dim each time
     float dimAmount = fullBrightness / pieces;
     
     //timer
     
     //start looping timer
     //timer fires
     //  dim light a bit
     //when light is gone
     //  kill timer
   }
    
   boolean isWithin(int x, int y) {
     return x >= this.x && x<=(this.x + size)
     && y >= this.y && y<=(this.y + size);
   }
}
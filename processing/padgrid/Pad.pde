class Pad {
   int x;
   int y;
   int size;
   color c;
   color borderColor = 150;
   
   Pad(int x, int y, int size, color c) {
      this.x = x;
      this.y = y;
      this.size = size;
      this.c = c;
   }
   
   void display() {
     fill(c);
     stroke(borderColor);
     rect(x, y, size, size);
     
   }
}
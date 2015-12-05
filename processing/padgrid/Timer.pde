class Timer {
  //static int numberOfTimers; // in main file instead
  final int id; 
  
  boolean running;
  boolean loop;
  long interval; 
  long startTime;
  long currentTime;
  
  Timer(long interval) {
    numberOfTimers++;
    id = numberOfTimers;
    
    //running = true;
    loop = true;
    startTime = millis();
    this.interval = interval;
  }
  
 void update() {
   if (running) {
     currentTime = millis();
     //println(currentTime - previousTime);
     if ((currentTime - startTime) >= interval) {
       println("boom!");
       if (loop) {
         startTime = currentTime;
       } else {
         running = false;
       }
     }
   }
 }
 
 void start() {
   println("starting timer");
   running = true;
 }
 
 //void addEvent() {
 //}
  
}
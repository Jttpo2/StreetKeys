int ledPin = 13;
int brightness = 0;
int fadeAmount = 5;

const int interval = 20; // adjustment interval

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    analogWrite(ledPin, brightness);
    brightness += fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
  }
}

const int switchPin = 9;
const int ledPin = 13;      // the number of the LED pin

int buttonState = 0;
int counter = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(switchPin, INPUT);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
buttonState = digitalRead(switchPin);

if (buttonState == HIGH) {
  digitalWrite(ledPin, HIGH);
  Serial.println(counter++);
  delay(500);
} else {
  digitalWrite(ledPin, LOW);
  
}

}

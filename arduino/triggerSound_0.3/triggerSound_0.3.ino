const int buttonPin = 9;
const int ledPin = 13;      // the number of the LED pin

int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// Debouncing
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

// Processing communication
String value;
String buttonDown = "1D";
String buttonUp = "1U";
char terminator = '\n';
int sampleLength = 0;

// LED setup
int brightness = 0;
int fadeAmount = 5;
const int interval = 20; // fade adjustment interval
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
//  establishContact(); // send a byte to establish contact until receiver responds 
  
  // set initial LED state
  digitalWrite(ledPin, ledState);
}

void loop() {
  // put your main code here, to run repeatedly:

// ******** Debouncing **********
  int reading = digitalRead(buttonPin);
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
        buttonState = reading;

        // only toggle the LED if the new button state is HIGH
        // Send correct button state to processing
        if (buttonState == HIGH) {
          // ****** Do something when button is pressed *****
          ledState = !ledState;
          Serial.println(buttonDown);
          brightness = 255;
        } else {
          // ****** Do something when button is released *****
          Serial.println(buttonUp);
        }
    }
  }

   // set the LED:
  //digitalWrite(ledPin, ledState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;

// ******* Receive from Processing **********
  if (Serial.available() > 0) {
    value = Serial.readStringUntil(terminator); 
    float sampleLengthInSeconds = getDuration(value);
    sampleLength = (int) (sampleLengthInSeconds * 1000);
    Serial.println(sampleLength);
//    analogWrite(ledPin, 255);
//    delay(1000);
//    analogWrite(ledPin, 0); 
  }
  

// ******* LED Fading **********
//  int sampleLength = 20000;
// sampleLength = 1828;
  int interval = sampleLength / 255;
  
  currentMillis = millis();

  if (currentMillis - previousMillis >= interval && brightness >= 0) {
    previousMillis = currentMillis;
    analogWrite(ledPin, brightness);
    brightness -= fadeAmount;
  }
  // ******** LED fading end ***********

}

float getDuration(String str) {
  String duration = str.substring(2);
  //Serial.println("Duration: " + duration);
  return duration.toFloat();
}




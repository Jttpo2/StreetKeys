const int buttonPin = 9;
const int ledPin = 13;      // the number of the LED pin

int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin


long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

String buttonDown = "1D";
String buttonUp = "1U";

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  // set initial LED state
  digitalWrite(ledPin, ledState);
}

void loop() {
  // put your main code here, to run repeatedly:

// DEBOUNCING
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
          ledState = !ledState;
          Serial.println(buttonDown);
        } else {
          Serial.println(buttonUp);
        }
    }
  }

   // set the LED:
  digitalWrite(ledPin, ledState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;

// /DEBOUNCING

/*
buttonState = digitalRead(buttonPin);

if (buttonState == HIGH) {
  digitalWrite(ledPin, HIGH);
  Serial.println(buttonDown);
  delay(500);
} else {
  digitalWrite(ledPin, LOW);
  
}
*/
}

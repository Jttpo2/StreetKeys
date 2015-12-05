const int buttonPin = 9;
const int ledPin = 13;      // the number of the LED pin

int ledState = HIGH;         // the current state of the output pin
int buttonState = LOW;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// Debouncing
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

// Processing communication
String value;
String buttonDown = "B1D";
String buttonUp = "B1U";
char terminator = '\n';

// LED setup
int brightness = 0;
int fadeAmount = 5;
const int interval = 20; // fade adjustment interval
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
float sampleLength = 0;
float multiplier = 0.75; // To shorten the led fading times slightly


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
        sampleLength = (sampleLengthInSeconds * 1000);
//        Serial.println(sampleLength);
  }
  
// ******* LED Fading **********
  int interval = 30;
  float modifiedSampleLength = sampleLength * multiplier;
  fadeAmount = 255 / (modifiedSampleLength / interval); // map fading interval times to analog output amount

  currentMillis = millis();

  if (currentMillis - previousMillis >= interval && brightness >= 0) {
    previousMillis = currentMillis;
    analogWrite(ledPin, brightness);
    brightness -= fadeAmount;
  }
  if (brightness < 0) {
    brightness = 0;
  }
  // ******** LED fading end ***********
}

float getDuration(String str) {
  String duration = str.substring(3);
//  Serial.println("Duration: " + duration);
  return duration.toFloat();
}


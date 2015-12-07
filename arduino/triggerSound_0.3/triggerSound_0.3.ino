const int buttonPin = 9;
const int ledPin = 13;      // the number of the LED pin

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
float brightness = 0;
float fadeAmount;
const int interval = 30; // fade adjustment interval
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
float sampleLength = 0;

float multiplier = 1.003; // To shorten or lengthen the led fading times slightly

// Led timer - for diagnostics
unsigned long ledTimer = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // Init serial communication
  Serial.begin(9600);
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
          Serial.println(buttonDown);
          brightness = 255;
//          ledTimer = millis(); // For measuring led on time
        } else {
          // ****** Do something when button is released *****
          Serial.println(buttonUp);
        }
    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;

  readSerial();
  
  currentMillis = millis();
  checkFadeTimer();
  checkIfLedTimerDone();
}

// Receive from Processing 
void readSerial() {
  if (Serial.available() > 0) {
    value = Serial.readStringUntil(terminator); 
    float sampleLengthInSeconds = getDuration(value);
    sampleLength = (sampleLengthInSeconds * 1000);
//  Serial.println(sampleLength);
    startLedFade(sampleLength);
  }
}

void startLedFade(float sampleLength) {
  float modifiedSampleLength = sampleLength * multiplier;
  fadeAmount = 255.0 / (modifiedSampleLength / interval); // map fading interval times to analog output amount
  Serial.println("FadeAmount: " + String(fadeAmount) + " Interval: " + interval + " Modified sample length: " +  modifiedSampleLength);
}

// Fade a led some
void fade(int ledNumber, float amount) {
  brightness -= amount;
    if (brightness < 0) {
      brightness = 0;
    }
}

void checkIfLedTimerDone() {
  // Led timer
  if (0 < brightness && brightness < 2) {
//    Serial.println("SampleLength: " + String(sampleLength));
//    Serial.println("Measured Duration: " + String(currentMillis - ledTimer));
    ledTimer = currentMillis;
  }
}

float getDuration(String str) {
  String duration = str.substring(2);
  Serial.println("Duration: " + duration);
  return duration.toFloat();
}

// For measuring how long the light is actually on for
void checkFadeTimer() {
  if (currentMillis - previousMillis >= interval && brightness >= 0) {
    previousMillis = currentMillis;
    analogWrite(ledPin, brightness);
    fade(0, fadeAmount);
    Serial.println("Time for fade, brightness: " + String(brightness) + " fade: " + fadeAmount  );
  }
}


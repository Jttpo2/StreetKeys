class Led {
  private:
    int pinNumber;
    const float fullBrightness = 255;
    
  public: 
    float brightness;
    Led(int pinNumber);
    Led(int pinNumber, float b);
    void turnOn();
    void turnOn(float b);
    void turnOff();
    boolean isOn();
    void fade(float fadeAmount);
    String toString();
};

  Led::Led(int pinNumber) {
    this->pinNumber = pinNumber;
    this->brightness = 0;
  }

  Led::Led(int pinNumber, float b) {
    this->pinNumber = pinNumber;
    this->brightness = b;
  }
  
  void Led::turnOn() {
    this->brightness = fullBrightness;
    analogWrite(this->pinNumber, this->brightness);
  }

  void Led::turnOn (float b) {
    this->brightness = b;
    analogWrite(this->pinNumber, this->brightness);
  }
    
  void Led::turnOff() {
    this->brightness = 0;
    analogWrite(this->pinNumber, this->brightness);
  }

  boolean Led::isOn() {
    return this-> brightness > 0;
  }

  // Fade a led some
  void Led::fade(float fadeAmount) {
//    Serial.println("Fade: " + String(fadeAmount));
    this->brightness -= fadeAmount;
    if (this->brightness < 0) {
      this->brightness = 0;
    }
    Serial.println("Brightness after: " + String(this->brightness));
    this->turnOn(this->brightness);
  }

  String Led::toString() {
    return "Led, pin: " + String(pinNumber) + " Brightness: " + brightness;
  }


// ************************* Main Class ****************************

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
float fadeAmount;
const int interval = 30; // fade adjustment interval
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
float sampleLength = 0;

const int ledsAmount = 9;
Led *leds[ledsAmount];

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

  // LED array
  leds[0] = new Led(ledPin);
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
  leds[0]->turnOn();
  float modifiedSampleLength = sampleLength * multiplier;
  fadeAmount = 255.0 / (modifiedSampleLength / interval); // map fading interval times to analog output amount
  Serial.println("FadeAmount: " + String(fadeAmount) + " Interval: " + interval + " Modified sample length: " +  modifiedSampleLength);
}

float getDuration(String str) {
  String duration = str.substring(2);
  Serial.println("Duration: " + duration);
  return duration.toFloat();
}

void checkFadeTimer() {
  if (currentMillis - previousMillis >= interval && leds[0]) {
    previousMillis = currentMillis;
    leds[0]->fade(fadeAmount);
    Serial.println("Time for fade, brightness: " + String(leds[0]->brightness) + " fade: " + fadeAmount  );
  }
}

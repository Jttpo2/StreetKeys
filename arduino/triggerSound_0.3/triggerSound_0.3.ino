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
//    Serial.println("Brightness after: " + String(this->brightness));
    this->turnOn(this->brightness);
  }

  String Led::toString() {
    return "Led, pin: " + String(pinNumber) + " Brightness: " + brightness;
  }

class Button {
  private:

  public:
  
};


// ************************* Main Class ****************************

const int buttonPin = 9;
const int ledPin = 13;      // the number of the LED pin

int buttonState = LOW;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// Debouncing
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

// Processing communication
String buttonDown = "B0D";
String buttonUp = "B0U";
char terminator = '\n';

// LED setup
const int LED_AMOUNT= 9;
float fadeAmount[LED_AMOUNT];
const int FADE_INTERVAL = 30; // fade adjustment interval
unsigned long currentTime;
unsigned long previousTime;

Led *leds[LED_AMOUNT];

float multiplier = 1.003; // To shorten or lengthen the led fading times slightly

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // Init serial communication
  Serial.begin(9600);

  // LED array
  leds[0] = new Led(ledPin);

  currentTime = previousTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:

  currentTime = millis();

// ******** Debouncing **********
  int reading = digitalRead(buttonPin);
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = currentTime;
  }

  if ((currentTime - lastDebounceTime) > debounceDelay) {
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

  String value = readSerial();
  actOnMessage(value);
  
  checkFadeTimer();
}

// Receive from Processing 
String readSerial() {
  if (Serial.available() > 0) {
    return Serial.readStringUntil(terminator); 
  } else {
    return "";
  }
}

void actOnMessage(String message) {
  if (!message.equals("")) {
    int ledNumber = getLedNumber(message);
    float sampleDuration = getSampleDuration(message);
    if (leds[ledNumber]) {
      leds[ledNumber]->turnOn();
      fadeAmount[ledNumber] = calcFadeAmount(sampleDuration, FADE_INTERVAL);  
    }
  }
}

void checkFadeTimer() {
  if (currentTime - previousTime >= FADE_INTERVAL) {
    previousTime = currentTime;
    fadeLeds();
  }
}

void fadeLeds() {
  for (int i=0; i<LED_AMOUNT; i++) {
      if (leds[i]) {
        leds[i]->fade(fadeAmount[i]);   
      }
    }
}

// ************ Helpers *****************

// Extract LED number duration from protocol string
int getLedNumber(String str) {
  Serial.println("Led: " + str.substring(1,2));
  return str.substring(1,2).toInt();
}

// Extract sample duration from protocol string
float getSampleDuration(String message) {
  String sampleLengthInSeconds = message.substring(2);
  float duration = (sampleLengthInSeconds.toFloat() * 1000);
  Serial.println("Duration: " + String(duration));
  return duration;
}

// Calculate amount to fade an LED for the interval given to end up at zero when the sample runs out
float calcFadeAmount(float sampleLength, int fadeInterval) {
  float modifiedSampleLength = sampleLength * multiplier;
  float fadeAmount = 255.0 / (modifiedSampleLength / fadeInterval); // map fading interval times to analog output amount
  Serial.println("FadeAmount: " + String(fadeAmount) + " Interval: " + fadeInterval + " Modified sample length: " +  modifiedSampleLength);
  return fadeAmount;
}

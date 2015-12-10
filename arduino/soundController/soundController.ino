// ******************* LED Class ****************************

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
    // initialize the LED pin as an output:
    pinMode(pinNumber, OUTPUT);
    this->pinNumber = pinNumber;
    this->brightness = 0;
  }

  Led::Led(int pinNumber, float b) {
    // initialize the LED pin as an output:
    pinMode(pinNumber, OUTPUT);
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

// ******************* Button Class ****************************

class Button {
  private:
    int buttonState = LOW;             // the current reading from the input pin
    int lastButtonState = HIGH;   // the previous reading from the input pin  
    // Debouncing
    long lastDebounceTime = 0;  // the last time the output pin was toggled
    long debounceDelay = 50;    // the debounce time; increase if the output flickers
  
  public:
    int id;
    int pinNumber;
    Button(int pinNumber, int id);
    void update();
};

  Button::Button(int pinNumber, int id) {
    pinMode(pinNumber, INPUT);
    this->pinNumber = pinNumber;
    this->id = id;
  }

  void Button::update() {
    // ******** Debouncing **********
    
    int reading = digitalRead(this->pinNumber);
    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH),  and you've waited
    // long enough since the last press to ignore any noise:
  
    // If the switch changed, due to noise or pressing:
    if (reading != this->lastButtonState) {
      // reset the debouncing timer
      this->lastDebounceTime = millis();
    }
  
    if ((millis() - this->lastDebounceTime) > this->debounceDelay) {
      // whatever the reading is at, it's been there for longer
      // than the debounce delay, so take it as the actual current state:
  
      // if the button state has changed:
      if (reading != this->buttonState) {
          this->buttonState = reading;
  
          // only toggle the LED if the new button state is HIGH
          // Send correct button state to processing
          if (this->buttonState == HIGH) {
            // ****** Do something when button is pressed *****
            String downMessage = "B" + String(id) + "D";
            // Send to processing
            Serial.println(downMessage);
          } else {
            // ****** Do something when button is released *****
            String upMessage = "B" + String(id) + "U";
            // Send to processing
            Serial.println(upMessage);
          }
      }
    }
  
    // save the reading.  Next time through the loop,
    // it'll be the lastButtonState:
    this->lastButtonState = reading;
    }


// ************************* Main Class ****************************
/*
 * Protocol for coms with Processing:
 * B4D - Button number 4 down
 * B1U - Button number 1 up
 */

const int buttonPin = 9;
const int ledPin = 13;      // the number of the LED pin

unsigned long currentTime;

char terminator = '\n'; // Processing protocol end of message

const int BUTTON_AMOUNT = 9;
Button *buttons[BUTTON_AMOUNT];

// LEDs setup
const int LED_AMOUNT= BUTTON_AMOUNT;
Led *leds[LED_AMOUNT];
float fadeAmount[LED_AMOUNT]; // Last calculated fading amount for all LEDs
const int FADE_INTERVAL = 30; // Fade adjustment interval
unsigned long previousFadeTime;
float multiplier = 1.003; // To shorten or lengthen the led fading times slightly

void setup() {
  // Init serial communication
  Serial.begin(9600);

  // Init arrays
  leds[0] = new Led(ledPin);
  buttons[0] = new Button(buttonPin, 0);

  currentTime = previousFadeTime = millis();
}

void loop() {
  currentTime = millis();

  // Update button states
  for (int i=0; i<BUTTON_AMOUNT; i++) {
    buttons[i]->update();
  }
  
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
  if (currentTime - previousFadeTime >= FADE_INTERVAL) {
    previousFadeTime = currentTime;
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

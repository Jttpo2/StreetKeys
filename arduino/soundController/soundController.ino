#include <Adafruit_NeoPixel.h>


// ****************** Setup ***************************
#define PAD_AMOUNT 1
#define LED_STRIP_AMOUNT 20 //60*3

//  const int LEDS_PER_PAD = LED_STRIP_AMOUNT/9;
const int LEDS_PER_PAD = LED_STRIP_AMOUNT;

int padHues[]         = {400, 3, 395, 483, 699, 200, 38, 88, 533};
int padSaturations[]  = {255, 255, 255, 255, 255, 255, 255, 255, 255};
int buttonPins[]      = {5, 4, 3, 10, 12, 11, 13, 10, 1};
float multiplier = 1; // To shorten or lengthen the led fading times slightly
                      // multiplier for 180 leds: 0.25
                       // 60 leds: 0.9
const int FADE_INTERVAL = 20;


// ****************************************************










#define RED 0
#define GREEN 1
#define BLUE 2
#define HUE 0
#define SATURATION 1
#define BRIGHTNESS 2
#define FULL 255

/******************************************************************************
   accepts hue, saturation and brightness values and outputs three 8-bit color
   values in an array (color[])

   saturation (sat) and brightness (bright) are 8-bit values.

   hue (index) is a value between 0 and 767. hue values out of range are
   rendered as 0.

 *****************************************************************************/
void hsb2rgb(uint16_t index, uint8_t sat, uint8_t bright, uint8_t color[3])
{
  uint16_t r_temp, g_temp, b_temp;
  uint8_t index_mod;
  uint8_t inverse_sat = (sat ^ 255);

  index = index % 768;
  index_mod = index % 256;

  if (index < 256)
  {
    r_temp = index_mod ^ 255;
    g_temp = index_mod;
    b_temp = 0;
  }

  else if (index < 512)
  {
    r_temp = 0;
    g_temp = index_mod ^ 255;
    b_temp = index_mod;
  }

  else if ( index < 768)
  {
    r_temp = index_mod;
    g_temp = 0;
    b_temp = index_mod ^ 255;
  }

  else
  {
    r_temp = 0;
    g_temp = 0;
    b_temp = 0;
  }

  r_temp = ((r_temp * sat) / 255) + inverse_sat;
  g_temp = ((g_temp * sat) / 255) + inverse_sat;
  b_temp = ((b_temp * sat) / 255) + inverse_sat;

  r_temp = (r_temp * bright) / 255;
  g_temp = (g_temp * bright) / 255;
  b_temp = (b_temp * bright) / 255;

  color[RED]  = (uint8_t)r_temp;
  color[GREEN]  = (uint8_t)g_temp;
  color[BLUE] = (uint8_t)b_temp;
}

// ******************* LED Class ****************************

class Led {
  private:
    int pinNumber;

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
  this->brightness = FULL;
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
  return this->brightness > 0;
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
//    int buttonState = LOW;             // the current reading from the input pin
    int buttonState = HIGH;             // the current reading from the input pin
//    int lastButtonState = HIGH;   // the previous reading from the input pin
    int lastButtonState = LOW;   // the previous reading from the input pin
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
//      if (this->buttonState == HIGH) {
        if (this->buttonState == LOW) {
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

// ******************* Pad Class ****************************

class Pad {
  private:

    Button *button;
    int buttonPin;
    int ledRangeStart;
    int ledRangeEnd;
    int hsb[3] = {0,0,0}; // color
    uint8_t rgb[3] = {0,0,0};
    float fadeAmount;
    unsigned long previousFadeTime;
    long startTime;
    float currentSampleDuration;
    void fadeLeds();

  public:
    Adafruit_NeoPixel *strip;
    Pad(int id, int buttonPin, Adafruit_NeoPixel *strip, int ledRangeStart, int ledRangeEnd, int hue, int saturation);
    void turnLedsOn();
    void turnLedsOn(int brightness);
    void turnOnAndFade(long duration);
    void turnLedsOff();
    void update(unsigned long currentTime);
};

Pad::Pad(int id, int buttonPin, Adafruit_NeoPixel *strip, int ledRangeStart, int ledRangeEnd, int hue, int saturation) {
  this->strip = strip;
  this->button = new Button(buttonPin, id);
  this->buttonPin = buttonPin;
  this->ledRangeStart = ledRangeStart;
  this->ledRangeEnd = ledRangeEnd;
  this->hsb[HUE] = hue;
  this->hsb[SATURATION] = saturation;

//  this->hsb = hsb;
//Serial.println("H: " + String(hsb[HUE]) + "S: " + String(hsb[SATURATION]) + "B: " + String(hsb[BRIGHTNESS]));
//  this->hsb[HUE] = hsb[HUE];
//  this->hsb[SATURATION] = hsb[SATURATION];
//  this->hsb[BRIGHTNESS] = 0;
//  this->hsb[BRIGHTNESS] = hsb[BRIGHTNESS];
  
  this->previousFadeTime = millis();
  Serial.println("Pad created");
}

void Pad::update(unsigned long currentTime) {
  this->button->update();
//  Serial.println("update");
  
  if (currentTime - this->previousFadeTime >= FADE_INTERVAL) {
//    Serial.println("fade interval reached");
    this->previousFadeTime = currentTime;
    this->fadeLeds();
  }
}

void Pad::turnLedsOn() {
//  Serial.println("Leds on " + String(this->rgb[RED]));
  this->turnLedsOn(FULL);
}

void Pad::turnLedsOn(int brightness) {
  this->hsb[BRIGHTNESS] = brightness;

  // convert hsb to rgb and put into rgb array
  hsb2rgb(this->hsb[HUE], this->hsb[SATURATION], this->hsb[BRIGHTNESS], this->rgb);

  for (int i = this->ledRangeStart; i < this->ledRangeEnd; i++) {
    this->strip->setPixelColor(i, this->rgb[RED], this->rgb[GREEN], this->rgb[BLUE]);
  }
//  Serial.print
//  Serial.println("Turned on pad, red: " + String(this->rgb[RED]));
  
}

void Pad::turnOnAndFade(long duration) {
//  this->turnLedsOn(FULL);
  this->startTime = millis();
  this->currentSampleDuration = duration;
  
  this->hsb[BRIGHTNESS]= FULL;
  this->fadeAmount = calcFadeAmount(duration, FADE_INTERVAL);  
  Serial.println("turn on and fade " + String(this->hsb[BRIGHTNESS]) +  " " + String(this->fadeAmount));
}

void Pad::fadeLeds() {
  if(this->hsb[BRIGHTNESS] > 0) {
    
    int remainingTime = calculateRemainingTime(this->startTime, this->currentSampleDuration);
    this->fadeAmount = calcFadeAmount(remainingTime, FADE_INTERVAL);
    
    float newBrightness = this->hsb[BRIGHTNESS] - this->fadeAmount;
    if (newBrightness < 0) {
      newBrightness = 0;
    }
//   Serial.println("Brightafter: " + this->hsb[BRIGHTNESS]);
    this->turnLedsOn(newBrightness);
  }
}

int calculateRemainingTime(long startTime, int totalDuration) {
  int remainingTime = (startTime + totalDuration) - millis();
//  int remainingTime = totalDuration - (millis() - startTime);
  if (remainingTime < 0) {
    remainingTime = 0;
  }
//  Serial.println("Remaining time: " + remainingTime);
  return remainingTime;
}

void Pad::turnLedsOff() {
  this->hsb[BRIGHTNESS] = 0;
}

// ************************* Main Class ****************************
/*
   Protocol for coms with Processing:
   B4D - Button number 4 down
   B1U - Button number 1 up
*/

#define LED_STRIP_PIN 6 // LED strip pin

const int overallBrightness = FULL;

#define BUTTON_PIN 9
#define LED_PIN 13      // the number of the test LED pin
#define LED_AMOUNT 9

unsigned long currentTime;

char terminator = '\n'; // Processing protocol end of message

Button *buttons[PAD_AMOUNT];

// Individual LEDs setup
Led *leds[LED_AMOUNT];
float fadeAmount[LED_AMOUNT]; // Last calculated fading amount for all LEDs
unsigned long previousFadeTime;


// Strips 
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_STRIP_AMOUNT, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

Pad *pads[PAD_AMOUNT];



void setup() {
  // Init serial communication
  Serial.begin(9600);

  strip.begin();

 // fucked: 8, 9, 2, 7, 
  // Pad colors in Hue, Sat, Bright

  // new Pad(id, pin, strip-adress, led range start, led range end, colorHSB[3])
  for (int i=0, ledStartIndex=0, ledEndIndex=LEDS_PER_PAD; i<PAD_AMOUNT; i++, ledStartIndex+=LEDS_PER_PAD, ledEndIndex+=LEDS_PER_PAD) {
//    pads[i] = new Pad(i, buttonPins[i], &strip, ledStartIndex, ledEndIndex, padColorsHSB[i]);
    pads[i] = new Pad(i, buttonPins[i], &strip, ledStartIndex, ledEndIndex, padHues[i], padSaturations[i]);
  }

  strip.setBrightness(overallBrightness); // set overall brightness
  strip.show(); // Initialize all pixels to 'off'

  currentTime = previousFadeTime = millis();
}

void loop() {
  currentTime = millis();

  String value = readSerial();
  actOnMessage(value);

  for (int i=0; i<PAD_AMOUNT; i++) {
    pads[i]->update(currentTime);
  }
  strip.show();

  // Single test LEDs
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
    Serial.println("From Processing: " + message);
    int ledNumber = getLedNumber(message);
    float sampleDuration = getSampleDuration(message);
    
    pads[ledNumber]->turnOnAndFade(sampleDuration);
    
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
  // Single test LEDs
  for (int i = 0; i < LED_AMOUNT; i++) {
    if (leds[i]) {
      leds[i]->fade(fadeAmount[i]);
    }
  }
}

// ************ Helpers *****************

// Extract LED number duration from protocol string
int getLedNumber(String str) {
  Serial.println("Led: " + str.substring(1, 2));
  return str.substring(1, 2).toInt();
}

// Extract sample duration from protocol string
float getSampleDuration(String message) {
  String sampleLengthInSeconds = message.substring(2);
  float duration = (sampleLengthInSeconds.toFloat() * 1000);
  Serial.println("Duration: " + String(duration));
  return duration;
}

// Calculate amount to fade an LED for the interval given to end up at zero when the sample runs out
float calcFadeAmount(int sampleLength, int fadeInterval) {
  int modifiedSampleLength = sampleLength * multiplier;
  float fadeAmount = 255.0 / (modifiedSampleLength / fadeInterval); // map fading interval times to analog output amount
  Serial.println("FadeAmount: " + String(fadeAmount) + " Interval: " + fadeInterval + " Remaining time: " +  modifiedSampleLength);
  return fadeAmount;
}



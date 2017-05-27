#include <Bounce2.h>
#include <FastLED.h>
#include <IRremote.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN    2
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    143

#define FRAMES_PER_SECOND  120

//#define MILLI_AMPS         1000     // IMPORTANT: set the max milli-Amps of your power supply (4A = 4000mA)
#define FRAMES_PER_SECOND  120 // here you can control the speed. With the Access Point / Web Server the animations run a bit slower.

#define IR_RECV_PIN 3

CRGB leds[NUM_LEDS];

IRrecv irReceiver(IR_RECV_PIN);

#include "irCommands.h"

InputCommand command;

const uint8_t brightnessCount = 5;
uint8_t brightnessMap[brightnessCount] = { 16, 32, 64, 128, 255 };
uint8_t brightness = brightnessMap[brightnessCount - 1];

const uint8_t buttonCount = 5;

const uint8_t buttonRedPin    =  7;
const uint8_t buttonGreenPin  =  8;
const uint8_t buttonBluePin   =  9;
const uint8_t buttonYellowPin = 10;
const uint8_t buttonWhitePin  = 11;

const uint8_t ledRedPin    = 0;
const uint8_t ledGreenPin  = 1;
const uint8_t ledBluePin   = 4;
const uint8_t ledYellowPin = 5;
const uint8_t ledWhitePin  = 6;

Bounce buttonRed = Bounce();
Bounce buttonGreen = Bounce();
Bounce buttonBlue = Bounce();
Bounce buttonYellow = Bounce();
Bounce buttonWhite = Bounce();

uint8_t currentPatternIndex = 0;

uint8_t buttonPins[] = {
  buttonRedPin,
  buttonGreenPin,
  buttonBluePin,
  buttonYellowPin,
  buttonWhitePin,
};

uint8_t ledPins[] = {
  ledRedPin,
  ledGreenPin,
  ledBluePin,
  ledYellowPin,
  ledWhitePin,
};

Bounce buttons[] = {
  buttonRed,
  buttonGreen,
  buttonBlue,
  buttonYellow,
  buttonWhite,
};

bool buttonChanged[5];

ulong buttonPressTimes[5];

String buttonNames[] = {
  "Red",
  "Green",
  "Blue",
  "Yellow",
  "White",
};

CHSV buttonColors[] = {
  CHSV(0, 255, 128),
  CHSV(96, 255, 128),
  CHSV(160, 255, 128),
  CHSV(64, 255, 128),
  CHSV(0, 0, 128),
};

#include "ball.h"
#include "juggle.h"
#include "launcher.h"
#include "colorInvaders.h"
#include "simon.h"

void pulse() {
  shiftDown();

  fadeToBlackBy(leds, 1, 10);

  for (uint8_t i = 0; i < buttonCount; i++) {
    if (buttons[i].fell()) {
      leds[0] += buttonColors[i];
    }
  }
}

void add() {
  shiftDown();

  fadeToBlackBy(leds, 1, 10);

  for (uint8_t i = 0; i < buttonCount; i++) {
    if (!buttons[i].read()) {
      leds[0] += buttonColors[i];
    }
  }
}

// List of modes.  Each is defined as a separate function below.
typedef void (*SimpleModeList[])();
SimpleModeList modes = {
  launcher,
  pulse,
  add,
  colorInvaders,
  simon,
  juggle,
};

uint8_t currentModeIndex = 0; // Index number of which mode is current

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

const int modeCount = ARRAY_SIZE(modes);

void setup() {
  Serial.begin(9600);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(brightness);
  //  FastLED.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);

  for (uint8_t i = 0; i < buttonCount; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  for (uint8_t i = 0; i < buttonCount; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  for (uint8_t i = 0; i < buttonCount; i++) {
    buttons[i].attach(buttonPins[i]);
  }

  // Initialize the IR receiver
  irReceiver.enableIRIn();
  irReceiver.blink13(true);
}

void loop() {
  for (uint8_t i = 0; i < buttonCount; i++) {
    buttonChanged[i] = buttons[i].update();
    if (buttonChanged[i]) {
      buttonPressTimes[i] = millis();

      Serial.print(buttonNames[i]);

      Serial.print(" button");

      if (buttons[i].fell()) {
        Serial.print(" pressed time: ");
        Serial.println(buttonPressTimes[i]);
        digitalWrite(ledPins[i], HIGH);
      } else {
        Serial.println(" released");
        digitalWrite(ledPins[i], LOW);
      }
    }
  }

  modes[currentModeIndex]();

  FastLED.show();

  handleInput();
}

void move(int delta) {
  moveTo(currentModeIndex + delta);
}

void moveTo(int index) {
  currentModeIndex = index;

  if (currentModeIndex >= modeCount)
    currentModeIndex = 0;
  else if (currentModeIndex < 0)
    currentModeIndex = modeCount - 1;

  fill_solid(leds, NUM_LEDS, CRGB::Black);
}

int getBrightnessLevel() {
  int level = 0;
  for (int i = 0; i < brightnessCount; i++) {
    if (brightnessMap[i] >= brightness) {
      level = i;
      break;
    }
  }
  return level;
}

void adjustBrightness(int delta) {
  int level = getBrightnessLevel();

  level += delta;
  if (level < 0)
    level = 0;
  if (level >= brightnessCount)
    level = brightnessCount - 1;

  brightness = brightnessMap[level];
  FastLED.setBrightness(brightness);
}

void handleInput() {
  command = readCommand(defaultHoldDelay);

  if (command != InputCommand::None) {
    Serial.print("command: ");
    Serial.println((int) command);
  }

  if (command == InputCommand::Up) {
    move(1);
  }
  else if (command == InputCommand::Down) {
    move(-1);
  }
  else if (command == InputCommand::Left) {
    move(-1);
  }
  else if (command == InputCommand::Right) {
    move(1);
  }
  else if (command == InputCommand::Power) {
    powerOff();
  }
  else if (command == InputCommand::BrightnessUp) {
    adjustBrightness(1);
  }
  else if (command == InputCommand::BrightnessDown) {
    adjustBrightness(-1);
  }

  // pattern buttons

  else if (command == InputCommand::Pattern1) {
    moveTo(0);
  }
  else if (command == InputCommand::Pattern2) {
    moveTo(1);
  }
  else if (command == InputCommand::Pattern3) {
    moveTo(2);
  }
  else if (command == InputCommand::Pattern4) {
    moveTo(3);
  }
  else if (command == InputCommand::Pattern5) {
    moveTo(4);
  }
  else if (command == InputCommand::Pattern6) {
    moveTo(5);
  }
  else if (command == InputCommand::Pattern7) {
    moveTo(6);
  }
  else if (command == InputCommand::Pattern8) {
    moveTo(7);
  }
  else if (command == InputCommand::Pattern9) {
    moveTo(8);
  }
  else if (command == InputCommand::Pattern10) {
    moveTo(9);
  }
  else if (command == InputCommand::Pattern11) {
    moveTo(10);
  }
  else if (command == InputCommand::Pattern12) {
    moveTo(11);
  }
}

void powerOff()
{
  // clear the display
  // fill_solid(leds, NUM_LEDS, CRGB::Black);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
    FastLED.show(); // display this frame
    delay(1);
  }

  FastLED.show(); // display this frame

  while (true) {
    InputCommand command = readCommand();
    if (command == InputCommand::Power ||
        command == InputCommand::Brightness)

      // go idle for a while, converve power
      delay(250);
  }
}

void shiftDown() {
  for (int i = NUM_LEDS - 1; i > 0; i--) {
    leds[i] = leds[i - 1];
  }
}

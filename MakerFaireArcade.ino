#include <Bounce2.h>
#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN    2
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    143
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          255
#define FRAMES_PER_SECOND  120

//#define MILLI_AMPS         1000     // IMPORTANT: set the max milli-Amps of your power supply (4A = 4000mA)
#define FRAMES_PER_SECOND  120 // here you can control the speed. With the Access Point / Web Server the animations run a bit slower.

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

#include "juggle.h"
#include "launcher.h"

void setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
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

  //  pulse();
  //  add();
  //  juggle();
  launcher();

  FastLED.show();
}

void shiftDown() {
  for (int i = NUM_LEDS - 1; i > 0; i--) {
    leds[i] = leds[i - 1];
  }
}

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

void simon()
{
  static byte sequence[255];

  static bool reset = true;

  static uint16_t resetDelay = 2000;
  static uint16_t sequenceDelay = 500;
  static uint16_t sequencePauseDelay = 250;

  static uint32_t waitTime;

  static bool playingSequence = true;
  static bool pausingBetweenSequence = true;
  static bool pausingBeforeSequence = true;

  static byte sequenceLength = 255;
  static byte sequenceIndex = 0;

  random16_add_entropy(random());

  if (waitTime > 0 && millis() < waitTime)
    return;

  if (reset || pausingBetweenSequence || pausingBeforeSequence) {
    for (byte i = 0; i < 5; i++) {
      digitalWrite(ledPins[i], LOW);
    }
  }

  if (reset) {
    for (int i = 0; i < 255; i++) {
      sequence[i] = random8(0, 4);
    }

    sequenceLength = 1;
    sequenceIndex = 0;

    playingSequence = true;
    pausingBetweenSequence = true;

    reset = false;

    fill_solid(leds, NUM_LEDS, CRGB::Black);

    waitTime = millis() + resetDelay;
    return;
  }

  if (sequenceIndex >= sequenceLength) {
    playingSequence = false;
    pausingBetweenSequence = false;
    sequenceIndex = 0;
  }

  if (pausingBeforeSequence) {
    pausingBeforeSequence = false;
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    waitTime = millis() + sequenceDelay;
    return;
  }

  if (pausingBetweenSequence) {
    pausingBetweenSequence = false;
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    waitTime = millis() + sequencePauseDelay;
    return;
  }

  byte sequenceColorIndex = sequence[sequenceIndex];
  CRGB sequenceColor = CHSV(buttonColors[sequenceColorIndex].hue, 255, 255);

  if (playingSequence) {
    sequenceIndex++;
    fill_solid(leds, NUM_LEDS, sequenceColor);

    for (byte i = 0; i < 5; i++) {
      if (CHSV(buttonColors[i].hue, 255, 255) == sequenceColor) {
        digitalWrite(ledPins[i], HIGH);
        break;
      }
    }

    pausingBetweenSequence = true;
    waitTime = millis() + sequenceDelay;
    return;
  }

  bool buttonPressed = false;
  CRGB colorPressed = CRGB::Black;

  for (uint8_t i = 0; i < 5; i++) {
    if (buttonChanged[i] && buttons[i].fell()) {
      buttonPressed = true;
      colorPressed = CHSV(buttonColors[i].hue, 255, 255);
      digitalWrite(ledPins[i], HIGH);
    }
  }

  if (!buttonPressed) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    return;
  }

  fill_solid(leds, NUM_LEDS, sequenceColor);

  if (colorPressed == sequenceColor) {
    if (sequenceIndex + 1 == sequenceLength) {
      sequenceLength++;
      sequenceIndex = 0;
      playingSequence = true;
      pausingBetweenSequence = true;
      pausingBeforeSequence = true;
      waitTime = millis() + sequencePauseDelay;
      return;
    }
    else {
      sequenceIndex++;
      waitTime = millis() + sequencePauseDelay;
      return;
    }
  }
  else {
    reset = true;
    waitTime = millis() + resetDelay;
    return;
  }
}

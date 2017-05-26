void juggle() {
  static bool ballLoaded = false;
  static bool ballVisible = false;
  static float ballPosition = 0;
  static float ballVelocity = 0;
  CRGB ballColor = CRGB::Red;

  static ulong ballDropTime = 0;

  static ulong ballLaunchTime = 0;

  const float ballVelocityDecay = 0.0036;

  const float maxCatchDiff = 100;

  fadeToBlackBy(leds, NUM_LEDS, 20);
  //  FastLED.clear();

  if (!ballLoaded) {
    ballLaunchTime = millis() + 1000;
    ballLoaded = true;
  }

  if (ballLoaded && !ballVisible && millis() >= ballLaunchTime) {
    ballVisible = true;
    ballLoaded = true;
    ballPosition = 0;
    ballVelocity = 1.0;
  }

  if (!ballVisible) {
    return;
  }

  ballPosition += ballVelocity;

  ballVelocity -= ballVelocityDecay;

  if (ballPosition < 0 && ballVisible) {
    ballDropTime = millis();
    Serial.print("ball drop time: ");
    Serial.println(ballDropTime);
    ballVisible = false;
    ballLoaded = false;
//    ballPosition = 0;
  }

  if (ballPosition < 0) {
    long diff = abs(ballDropTime - buttonPressTimes[0]);
    Serial.print("diff: ");
    Serial.println(diff);
    if (diff < maxCatchDiff) {
      // ball caught, relaunch it
      ballVisible = true;
      ballLoaded = true;
      ballPosition = 0;
      ballVelocity = 1.0;
    }
  }

  if (ballPosition >= NUM_LEDS) {
    ballVelocity *= -1.0;
    ballPosition = NUM_LEDS - 1;
  }

  if (ballPosition > 0 && ballPosition < NUM_LEDS) {
    leds[(uint8_t) ballPosition] += ballColor;
  }
}


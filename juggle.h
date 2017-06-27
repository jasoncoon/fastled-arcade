void juggle() {
  static Ball balls[buttonCount];

  const float velocityDecay = 0.0036;

  const float maxCatchDiff = 200;

  fadeToBlackBy(leds, NUM_LEDS, 20);
  //  FastLED.clear();

  if (modeInit) {
    modeInit = false;

    for (uint8_t i = 0; i < buttonCount; i++) {
      balls[i].visible = false;
      balls[i].loaded = false;
      balls[i].position = 0;
    }
  }

  static uint8_t activeBallCount = 1;

  bool allActiveBallsCaught = false;

  for (uint8_t i = 0; i < activeBallCount; i++) {
    if (!balls[i].visible && !balls[i].loaded) {
      balls[i].launchTime = millis() + 1000;
      balls[i].loaded = true;
      balls[i].position = 0;
    }

    if (balls[i].loaded && millis() >= balls[i].launchTime) {
      balls[i].visible = true;
      balls[i].loaded = false;
      balls[i].position = 0;
      balls[i].velocity = 1.0;
    }

    if (!balls[i].visible) {
      return;
    }

    balls[i].position += balls[i].velocity;

    balls[i].velocity -= velocityDecay;

    if (balls[i].position < 0) {
      balls[i].dropTime = millis();
      // Serial.print("ball drop time: ");
      // Serial.println(balls[i].dropTime);

      long diff = balls[i].dropTime - buttonPressTimes[i];
      // Serial.print("diff: ");
    // Serial.println(diff);

      if (diff < maxCatchDiff) {
        // ball caught, relaunch it
        balls[i].visible = true;
        balls[i].position = 0;
        balls[i].velocity = 1.0;

        if (i == activeBallCount - 1) {
          allActiveBallsCaught = true;
        }
      }
      else {
        for (uint8_t j = 0; j < buttonCount; j++) {
          balls[j].visible = false;
          balls[j].loaded = false;
          balls[j].position = 0;
        }
        activeBallCount = 1;
      }
    }

    if (balls[i].position >= NUM_LEDS) {
      balls[i].velocity *= -1.0;
      balls[i].position = NUM_LEDS - 1;
    }

    if (balls[i].position > 0 && balls[i].position < NUM_LEDS) {
      leds[(uint8_t) balls[i].position] += buttonColors[i];
    }
  }

  if (allActiveBallsCaught && activeBallCount < buttonCount) {
    activeBallCount++;
  }
}


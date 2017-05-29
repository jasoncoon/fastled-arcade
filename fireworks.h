void fireworks() {
  static Ball balls[buttonCount];

  const float ballVelocityDecay = 0.0036;
  const uint8_t explosionSteps = 64;

  fadeToBlackBy(leds, NUM_LEDS, 10);

  for (uint8_t i = 0; i < buttonCount; i++) {
    // check input
    if (buttonChanged[i]) {
      if (buttons[i].fell()) {
        if (balls[i].position < 0) {
          // launch firework
          leds[0] += CHSV(0, 0, 128);
          balls[i].position = 0;
          balls[i].velocity = 1.0;
          balls[i].color = buttonColors[i];
        }
        else if (balls[i].exploding < 1) {
          // detonate firework
          balls[i].exploding = 1;
        }
      }
    }

    // update
    if (balls[i].position >= 0) {
      uint8_t exploding = balls[i].exploding;

      if (exploding == 0) {
        balls[i].position += balls[i].velocity;
        balls[i].velocity -= ballVelocityDecay;
        if (balls[i].position >= NUM_LEDS) {
          balls[i].velocity *= -1.0;
          balls[i].position = NUM_LEDS - 1;
        }
      }

      // draw
      if (exploding < 1) {
        leds[(uint8_t) balls[i].position] += CHSV(0, 0, 32);
      }
      else if (exploding == 1) {
        leds[(uint8_t) balls[i].position] += CRGB::White;
      }
      else {
        int16_t particlePosition = balls[i].position + exploding;

        if (particlePosition < NUM_LEDS) {
          leds[particlePosition] += balls[i].color;
        }

        particlePosition = balls[i].position - exploding;

        if (particlePosition >= 0) {
          leds[particlePosition] += balls[i].color;
        }
      }

      if (exploding > 0) {
        if (exploding < explosionSteps) {
          balls[i].exploding++;
        }
        else {
          balls[i].exploding = 0;
          balls[i].position = -1;
        }
      }
    }
  }
}


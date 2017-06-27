void launcher() {
  const uint8_t ballCount = 100;

  static Ball balls[ballCount];

  const float ballVelocityDecay = 0.0036;

  fadeToBlackBy(leds, NUM_LEDS, 20);

  if (modeInit) {
    modeInit = false;
    for (uint8_t j = 0; j < ballCount; j++) {
      balls[j].position = -1;
    }
  }

  for (uint8_t i = 0; i < buttonCount; i++) {
    if (buttonChanged[i]) {
      if (buttons[i].fell()) {
        for (uint8_t j = 0; j < ballCount; j++) {
          if (balls[j].position < 0) {
            balls[j].position = 0;
            balls[j].velocity = 1.0;
            balls[j].color = buttonColors[i];
            break;
          }
        }
      }
    }
  }

  for (uint8_t i = 0; i < ballCount; i++) {
    if (balls[i].position >= 0) {
      balls[i].position += balls[i].velocity;

      balls[i].velocity -= ballVelocityDecay;

      if (balls[i].position >= NUM_LEDS) {
        balls[i].velocity *= -1.0;
        balls[i].position = NUM_LEDS - 1;
      }

      leds[(uint8_t) balls[i].position] += balls[i].color;
    }
  }
}


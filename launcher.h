void launcher() {
  static Ball balls[5];

  const float ballVelocityDecay = 0.0036;

  fadeToBlackBy(leds, NUM_LEDS, 20);

  for (uint8_t i = 0; i < 5; i++) {
    if (buttonChanged[i]) {
      if (buttons[i].fell() && balls[i].position < 0) {
        balls[i].position = 0;
        balls[i].velocity = 1.0;
      }
    }

    if (balls[i].position >= 0) {
      balls[i].position += balls[i].velocity;

      balls[i].velocity -= ballVelocityDecay;

      if (balls[i].position >= NUM_LEDS) {
        balls[i].velocity *= -1.0;
        balls[i].position = NUM_LEDS - 1;
      }
      
      leds[(uint8_t) balls[i].position] += buttonColors[i];
    }
  }
}


void launcher() {
  static float ballPosition[5];
  static float ballVelocity[5];

  const float ballVelocityDecay = 0.0036;

  fadeToBlackBy(leds, NUM_LEDS, 20);

  for (uint8_t i = 0; i < 5; i++) {
    if (buttonChanged[i]) {
      if (buttons[i].fell() && ballPosition[i] < 0) {
        ballPosition[i] = 0;
        ballVelocity[i] = 1.0;
      }
    }

    if (ballPosition[i] >= 0) {
      ballPosition[i] += ballVelocity[i];

      ballVelocity[i] -= ballVelocityDecay;

      if (ballPosition[i] >= NUM_LEDS) {
        ballVelocity[i] *= -1.0;
        ballPosition[i] = NUM_LEDS - 1;
      }
      
      leds[(uint8_t) ballPosition[i]] += buttonColors[i];
    }
  }
}


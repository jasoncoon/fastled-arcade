void shooter() {
  static Ball missile;

  const uint8_t maxEnemyCount = 10;
  const float enemyAcceleration = 0.0036;

  const uint8_t enemySpawnRateDefault = 3;
  const uint8_t enemySpawnChanceDefault = 127;
  const uint8_t enemyVelocityDefault = 0.25;

  uint8_t enemySpawnRate = 254;
  uint8_t enemySpawnChance = 127;
  uint8_t enemyVelocity = 0.25;

  static Ball enemies[maxEnemyCount];

  static bool initialized = false;

  if (!initialized) {
    initialized = true;

    missile.position = -1;
    for (uint8_t i = 0; i < maxEnemyCount; i++) {
      enemies[i].position = -1;
    }
  }

  fadeToBlackBy(leds, NUM_LEDS, 20);

  for (uint8_t i = 0; i < 5; i++) {
    if (buttonChanged[i]) {
      if (buttons[i].fell()) {
        if (missile.position < 0) {
          missile.position = 0;
          missile.velocity = 1.0;
          missile.color = CHSV(buttonColors[i].hue, 255, 255);
          break;
        }
      }
    }
  }

  for (uint8_t i = 0; i < maxEnemyCount; i++) {
    if (enemies[i].position >= 0 && enemies[i].position < NUM_LEDS) {
      if(enemies[i].exploding < 1) {
        enemies[i].position -= enemies[i].velocity;
      }

      if (enemies[i].position <= 0 && enemies[i].exploding < 1) {
        // player explodes
        while (leds[0] != CRGB(CRGB::White)) {
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] += CRGB(2, 2, 2);
          }
          LEDS.show();
        }

        while (leds[0] != CRGB(CRGB::Black)) {
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] -= CRGB(2, 2, 2);
          }
          LEDS.show();
        }

        // game resets
        for (int i = 0; i < maxEnemyCount; i++)
          enemies[i].position = -1;

        enemySpawnRate = enemySpawnRateDefault;
        enemySpawnChance = enemySpawnChanceDefault;
        enemyVelocity = enemyVelocityDefault;

        return;
      }
      else if (enemies[i].color.hue == missile.color.hue && abs(enemies[i].position - missile.position) < 2) {
        // enemy explodes
        enemies[i].exploding = 16;
        enemies[i].velocity = 0;
        missile.position = -1;

        if(enemySpawnChance > 0) {
          enemySpawnChance--;
        }
      }

      if (enemies[i].exploding < 1) {
        enemies[i].velocity += enemyAcceleration;
      }

      leds[(uint8_t) enemies[i].position] += enemies[i].color;

      if (enemies[i].exploding > 0) {
        enemies[i].exploding--;
        leds[(uint8_t) enemies[i].position] += CHSV(0, 0, 255);

        if(enemies[i].exploding == 0) {
          enemies[i].position = -1;
        }
      }
    }
  }

  if (missile.position >= 0) {
    missile.position += missile.velocity;

    if (missile.position >= NUM_LEDS) {
      missile.position = -1;
    }
    else {
      leds[(uint8_t) missile.position] += missile.color;
    }
  }

  EVERY_N_SECONDS(255 - enemySpawnRate) {
    if (random8() >= enemySpawnChance) {
      // spawn a new enemy
      for (uint8_t i = 0; i < maxEnemyCount; i++) {
        if (enemies[i].position < 0) {
          enemies[i].position = NUM_LEDS - 1;
          enemies[i].color = CHSV(buttonColors[random8(0, 6)].hue, 255, 255);
          enemies[i].velocity = enemyVelocity;
          break;
        }
      }
    }
  }
}


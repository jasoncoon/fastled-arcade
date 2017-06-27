void shooter() {
  static Ball missile;

  const float enemyAcceleration = 0.0036;

  const uint16_t enemySpawnChanceDefault = 16383; // out of 65535
  const uint8_t enemyVelocityDefault = 0.125;

  uint8_t enemySpawnChance = enemySpawnChanceDefault;
  uint8_t enemyVelocity = enemyVelocityDefault;

  static Ball enemy;

  static bool initialized = false;

  static uint16_t enemiesHit = 0;
  static uint8_t enemyColorCount = 3;

  if (!initialized || modeInit) {
    initialized = true;
    modeInit = false;

    missile.position = -1;
    enemy.position = NUM_LEDS;
    enemiesHit = 0;
  }

  fadeToBlackBy(leds, NUM_LEDS, 20);

  // launch new missile?
  if (missile.position < 0) {
    for (uint8_t i = 0; i < buttonCount; i++) {
      if (buttonChanged[i]) {
        if (buttons[i].fell()) {
          missile.position = 0;
          missile.velocity = 1.0;
          missile.color = buttonColors[i];
          break;
        }
      }
    }
  }

  // update enemy
  if (enemy.position > 0 && enemy.position < NUM_LEDS) {
    if (enemy.exploding < 1) {
      enemy.position -= enemy.velocity;
      enemy.velocity += enemyAcceleration;
    }

    if (enemy.exploding > 0) {
      enemy.exploding++;
      if (enemy.exploding > 16) {
        enemy.exploding = 0;
        enemy.position = NUM_LEDS;
      }
    }
  }
  else if (random16() <= enemySpawnChance) {
    // spawn a new enemy
    enemy.position = NUM_LEDS - 1;
    enemy.color = buttonColors[random8(0, enemyColorCount)];
    enemy.velocity = enemyVelocity;
  }

  // update missile
  if (missile.position >= 0 && missile.position < NUM_LEDS) {
    missile.position += missile.velocity;
  }

  if (missile.position >= NUM_LEDS) {
    missile.position = -1;
  }

  // check for collisions

  // enemy hit base?
  if (enemy.position <= 0) {
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
    enemy.position = NUM_LEDS;
    missile.position = -1;
    enemiesHit = 0;

    enemySpawnChance = enemySpawnChanceDefault;
    enemyVelocity = enemyVelocityDefault;

    return;
  }

  // missile hit enemy?
  if (enemy.color == missile.color && missile.position >= enemy.position) {
    // enemy explodes
    enemy.exploding = 1;
    enemy.velocity = 0;
    missile.position = -1;

    // increase enemy spawn chance
    if (enemySpawnChance < 65535) {
      enemySpawnChance++;
    }

    // increase enemy velocity
    enemyVelocity += 0.1;

    enemiesHit++;
    if (enemyColorCount < buttonCount && enemiesHit % 10 == 0)
      enemyColorCount++;
  }

  // draw enemy
  if (enemy.position > 0 && enemy.position < NUM_LEDS) {
    if (enemy.exploding < 1) {
      leds[(uint8_t) enemy.position] += enemy.color;
    }
    else {
      leds[(uint8_t) enemy.position] += CHSV(0, 0, 255);
    }
  }

  // draw missile
  if (missile.position >= 0 && missile.position < NUM_LEDS) {
    leds[(uint8_t) missile.position] += missile.color;
  }
}


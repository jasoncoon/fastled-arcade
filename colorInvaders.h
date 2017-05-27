// Color Invaders
// Loosely based on Colour Invaders: http://hamsterworks.co.nz/mediawiki/index.php/Colour_Invaders

/* Speed the missile moves - lower is faster */
#define MISSILE_SPEED 1

/* How long the explosion lasts */
#define EXPLOSION_DELAY 300

/* How long it takes for the weapon to recharge */
#define RECHARGE_DELAY 64

uint16_t invader_default_speed = 64;

/* Speed of the invader - gradually reduces */
uint16_t invader_speed = invader_default_speed;

byte invaders[NUM_LEDS];
byte invaders_colours[NUM_LEDS];
byte missile = 0;
word missile_delay = 1000;
byte missile_colour;
word invader_delay;
byte recharge_delay;
word explosion = 0;
word explosion_count;
byte random_colour = 0;

byte playerHue = 0;
byte playerBrightness = 0;

void test_for_missile_hit(void) {
  if (missile > 0 && invaders[missile]) {
    if (invaders_colours[missile] == missile_colour) {
      invaders[missile] = 0;
      explosion = missile;
      explosion_count = EXPLOSION_DELAY;
      if (invader_speed > 50)
        invader_speed -= 8;
    }
    missile = 0;
  }
}

uint32_t colorInvaders()
{
  if (invaders[0]) {
    // player explodes
    while (leds[0] != CRGB(CRGB::White)) {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] += CRGB(1, 1, 1);
      }
      LEDS.show();
    }

    while (leds[0] != CRGB(CRGB::Black)) {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] -= CRGB(1, 1, 1);
      }
      LEDS.show();
    }

    // game resets
    for (int i = 0; i < NUM_LEDS; i++)
      invaders[i] = 0;

    invader_speed = invader_default_speed;

    return 0;
  }

  fill_solid(leds, NUM_LEDS, CRGB::Black);

  /* Keep track of the recharge */
  if (recharge_delay > 0)
    recharge_delay--;

  playerBrightness = 255 - recharge_delay;

  leds[0] = CHSV(playerHue, 255, playerBrightness);

  bool fired = false;

  for (uint8_t i = 0; i < 5; i++) {
    if (buttonChanged[i] && buttons[i].fell()) {
      playerHue = buttonColors[i].hue;
      fired = true;
    }
  }

  /* Is a missile in flight? */
  if (missile == 0)
  {
    /* Are we recharged? Is the button pushed? */
    if (fired) { // if (recharge_delay == 0)
      /* Fire missile! */
      missile_colour = playerHue;
      missile = 1;
      missile_delay = MISSILE_SPEED;
      recharge_delay = RECHARGE_DELAY;
    }
  }
  else
  {
    /* Is it time to update the missile location? */
    if (missile_delay == 0)
    {
      /* Yep, check for the missile being off the end of the strip */
      if (missile >= NUM_LEDS - 1)
        missile = 0;
      else
      {
        missile++;
        test_for_missile_hit();
        missile_delay = MISSILE_SPEED;
      }
    }
    else
      missile_delay--;
  }

  /* Is it time to move the invaders? */
  if (invader_delay == 0) {
    /* Move them along one */
    for (int i = 0; i < NUM_LEDS - 1; i++)
    {
      invaders[i] = invaders[i + 1];
      invaders_colours[i] = invaders_colours[i + 1];
    }

    /* Has one moved into the missile */
    test_for_missile_hit();
    invaders[NUM_LEDS - 1] = 0;

    /* Is it time to send a new invader? */
    if (invaders[NUM_LEDS - 2] == 0 && invaders[NUM_LEDS - 3] == 0 && invaders[NUM_LEDS - 4] == 0) {
      invaders[NUM_LEDS - 1] = 1;
      invaders_colours[NUM_LEDS - 1] = buttonColors[random8(0, 5)].hue;
    }
    invader_delay = invader_speed;
  }
  else
    invader_delay--;

  /* Display the missile */
  if (missile > 0) leds[missile] = CHSV(missile_colour, 255, 255);
  if (missile > 1) CHSV(missile_colour, 255, 128);
  if (missile > 2) CHSV(missile_colour, 255, 255);

  /* Display the invaders */
  for (int i = 0; i < NUM_LEDS; i++)
  {
    if (invaders[i] != 0)
      leds[i] = CHSV(invaders_colours[i], 255, 255);
  }

  /*********************************************
    We love explosions! Do we have one to paint?
  *********************************************/
  if (explosion_count > EXPLOSION_DELAY * 3 / 4) {
    explosion_count--;
    leds[explosion] += CRGB(48, 48, 48);
  }
  else if (explosion_count > EXPLOSION_DELAY / 2) {
    explosion_count--;
    if (explosion > 1) leds[explosion - 1] += CRGB(48, 48, 48);
    leds[explosion] += CRGB(96, 96, 96);
    if (explosion < NUM_LEDS - 1) leds[explosion + 1] += CRGB(48, 48, 48);
  }
  else if (explosion_count > 0) {
    explosion_count--;
    if (explosion > 1) leds[explosion - 1] += CRGB(24, 24, 24);
    leds[explosion] += CRGB(48, 48, 48);
    if (explosion < NUM_LEDS - 1) leds[explosion + 1] += CRGB(24, 24, 24);
  }

  return 0;
}

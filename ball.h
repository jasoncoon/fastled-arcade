struct Ball {
  float position;
  float velocity;
  bool loaded;
  bool visible;
  unsigned long launchTime;
  unsigned long dropTime;
  CHSV color;
  uint8_t exploding;
};

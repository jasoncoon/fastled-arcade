struct Ball {
  float position;
  float velocity;
  bool loaded;
  bool visible;
  ulong launchTime;
  ulong dropTime;
  CHSV color;
  uint8_t exploding;
};

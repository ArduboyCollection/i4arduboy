#pragma once

#include "gamecore.h"
#include "constants.h"

struct GameLevel;

#define ECHO_CYCLE (60)

struct Echo {
  void reset(GameLevel& context, const byte subX);
  void add(const int left, const char top, const char bottom);
  void draw(GameLevel& context) const;

  private:
  static const byte ECHO_GRID_SIZE = 3;
  static const byte ECHO_VERT_RESO = SCREEN_HEIGHT / ECHO_GRID_SIZE;
  static const byte ECHO_HORI_RESO = 16;

  byte intensities[ECHO_VERT_RESO];
  byte submarineX;
  bool acceptFlag;
};


enum {
  PLATOON_ZIG_FILE = 0,
  PLATOON_TRI_LINE,
  PLATOON_TRI_SHOAL,
};

struct Platoons {
  void initialize();
  void set(const char y, const byte type);
  void spawn(GameLevel& context);
  bool checkBonus(const byte idx, bool killed);

  private:
  inline bool inUse(const byte idx) const {
    return (status[idx] & PLATOON_USING_MASK) != 0;
  }
  
  private:
  static const byte PLATOON_MAX        = 5;
  static const byte PLATOON_CONSISTS   = 4;
  static const byte PLATOON_USING_MASK = 0x80;
  static const byte PLATOON_COUNT_MASK = 0x7;

  char spawnYs[PLATOON_MAX];
  byte types[PLATOON_MAX];
  byte timers[PLATOON_MAX];
  byte status[PLATOON_MAX];
};


#define INST_SPAWN_MASK  (B10000000)
#define INST_TYPE_MASK   (B01111111)
#define INST_RAND_MASK   (B11000000)
#define INST_Y_MASK      (B00111111)

#define INST_RAND_WIDE   (B11000000)
#define INST_RAND_NARROW (B10000000)
#define INST_RAND_NONE   (B01000000)
#define INST_RAND_ERROR  (B00000000)

#define INST_END_WAVE    (B00000000)
#define INST_SPAWN(type, rand, y) INST_SPAWN_MASK | (type), (rand) | ((y) & INST_Y_MASK)
#define INST_DELAY(frame)         ((frame) > INST_TYPE_MASK ? INST_TYPE_MASK : (frame))

enum {
  SPAWN_NONE = 0,
  SPAWN_BIG,
  SPAWN_ZIG_SINGLE,
  SPAWN_ZIG_FILE,
  SPAWN_TRI_SINGLE,
  SPAWN_TRI_LINE,
  SPAWN_TRI_SHOAL,
  SPAWN_MAX,
};

struct Generator {
  void initialize();
  void spawn(GameLevel& context);
  void draw(GameLevel& context) const;
  inline byte getDifficulty() const { return (difficulty > DIFFICULTY_CAP) ? DIFFICULTY_CAP : difficulty; }
  
  private:
  static const byte WAVES_IN_ZONE  = 6;
  static const byte DIFFICULTY_CAP = 100;
  
  byte difficulty;
  byte zone;
  byte waveCount;
  byte waveIndex;
  byte progCount;
  byte delayTimer;
  byte dispTimer;
};

// === Enemy spawn scripts ===

PROGMEM const byte waveEmpty[] = { INST_DELAY(127), INST_DELAY(127), INST_DELAY(127), INST_END_WAVE };
PROGMEM const byte waveBeginner0[] = {
  INST_SPAWN(SPAWN_BIG, INST_RAND_NARROW, 38), 
  INST_DELAY(127), INST_DELAY(127),
  INST_END_WAVE
};
PROGMEM const byte waveBeginner1[] = {
  INST_SPAWN(SPAWN_BIG, INST_RAND_NARROW, 46), 
  INST_DELAY(40), 
  INST_SPAWN(SPAWN_BIG, INST_RAND_NARROW, 12), 
  INST_DELAY(127), INST_DELAY(127),
  INST_END_WAVE
};
PROGMEM const byte waveRandomShoal[] = {
  INST_SPAWN(SPAWN_TRI_SHOAL, INST_RAND_WIDE, 0), INST_DELAY(127), 
  INST_DELAY(127), 
  INST_END_WAVE
};
PROGMEM const byte waveRandomLines[] = {
  INST_SPAWN(SPAWN_TRI_LINE, INST_RAND_WIDE, 0), INST_DELAY(127), 
  INST_SPAWN(SPAWN_TRI_LINE, INST_RAND_WIDE, 0), INST_DELAY(127),
  INST_DELAY(127), 
  INST_END_WAVE
};
PROGMEM const byte waveZigTop[] = {
  INST_SPAWN(SPAWN_ZIG_FILE, INST_RAND_NARROW, 20), INST_DELAY(127), 
  INST_DELAY(60), 
  INST_END_WAVE
};
PROGMEM const byte waveZigBottom[] = {
  INST_SPAWN(SPAWN_ZIG_FILE, INST_RAND_NARROW, 24), INST_DELAY(127), 
  INST_DELAY(60), 
  INST_END_WAVE
};
PROGMEM const byte waveTriTri[] = {
  INST_SPAWN(SPAWN_ZIG_FILE, INST_RAND_NONE, 18),  INST_DELAY(60), 
  INST_SPAWN(SPAWN_ZIG_FILE, INST_RAND_NONE, 52),  INST_DELAY(60), 
  INST_SPAWN(SPAWN_ZIG_FILE, INST_RAND_NONE, 32),
  INST_DELAY(127), INST_DELAY(127), 
  INST_END_WAVE
};
PROGMEM const byte waveSandwich[] = {
  INST_SPAWN(SPAWN_TRI_LINE, INST_RAND_NARROW, 10),
  INST_SPAWN(SPAWN_TRI_LINE, INST_RAND_NARROW, 54),
  INST_DELAY(100), 
  INST_SPAWN(SPAWN_ZIG_FILE, INST_RAND_NONE, 30),
  INST_DELAY(127),
  INST_END_WAVE
};
PROGMEM const byte waveCatapult[] = {
  INST_SPAWN(SPAWN_TRI_LINE, INST_RAND_NONE, 21),
  INST_SPAWN(SPAWN_TRI_LINE, INST_RAND_NONE, 43),
  INST_DELAY(60), 
  INST_SPAWN(SPAWN_BIG     , INST_RAND_NONE, 29),
  INST_DELAY(127), INST_DELAY(64), 
  INST_END_WAVE
};
PROGMEM const byte waveSuperPlatoon[] = {
  INST_SPAWN(SPAWN_BIG, INST_RAND_NARROW, 26),
  INST_SPAWN(SPAWN_BIG, INST_RAND_NARROW, 39),  INST_DELAY(80),
  INST_SPAWN(SPAWN_BIG, INST_RAND_NARROW, 13),
  INST_SPAWN(SPAWN_BIG, INST_RAND_NARROW, 52), 
  INST_DELAY(127), INST_DELAY(127), 
  INST_END_WAVE
};
PROGMEM const byte waveArrows[] = {
  INST_SPAWN(SPAWN_BIG, INST_RAND_NONE, 46), INST_DELAY(80),
  INST_SPAWN(SPAWN_BIG, INST_RAND_NONE, 52), 
  INST_SPAWN(SPAWN_BIG, INST_RAND_NONE, 40), 
  INST_DELAY(127), 
  INST_DELAY(60),
  INST_SPAWN(SPAWN_BIG, INST_RAND_NONE, 18), INST_DELAY(80),
  INST_SPAWN(SPAWN_BIG, INST_RAND_NONE, 24), 
  INST_SPAWN(SPAWN_BIG, INST_RAND_NONE, 12),  
  INST_DELAY(127), 
  INST_DELAY(127), 
  INST_END_WAVE
};
PROGMEM const byte waveCamouflage0[] = {
  INST_SPAWN(SPAWN_TRI_SHOAL, INST_RAND_NONE  , 16), 
  INST_DELAY(90),
  INST_SPAWN(SPAWN_TRI_SHOAL, INST_RAND_NONE  , 44), 
  INST_DELAY(30),
  INST_SPAWN(SPAWN_BIG      , INST_RAND_NARROW, 44), 
  INST_DELAY(97), 
  INST_END_WAVE
};
PROGMEM const byte waveCamouflage1[] = {
  INST_SPAWN(SPAWN_TRI_SHOAL, INST_RAND_NONE  , 16), 
  INST_DELAY(30),
  INST_SPAWN(SPAWN_BIG      , INST_RAND_NARROW, 16), 
  INST_DELAY(60),
  INST_SPAWN(SPAWN_TRI_SHOAL, INST_RAND_NONE  , 44), 
  INST_DELAY(127), 
  INST_END_WAVE
};
PROGMEM const byte waveBigWall[] = {
  INST_SPAWN(SPAWN_BIG, INST_RAND_NONE, 56), INST_DELAY(40),
  INST_SPAWN(SPAWN_BIG, INST_RAND_NONE, 44), INST_DELAY(40),  
  INST_SPAWN(SPAWN_BIG, INST_RAND_NONE, 32), INST_DELAY(40), 
  INST_SPAWN(SPAWN_BIG, INST_RAND_NONE, 20), INST_DELAY(40), 
  INST_SPAWN(SPAWN_BIG, INST_RAND_NONE,  8), INST_DELAY(40), 
  INST_DELAY(127), INST_DELAY(127), INST_DELAY(127),
  INST_END_WAVE
};
PROGMEM const byte waveJamming[] = {
  INST_SPAWN(SPAWN_TRI_SHOAL, INST_RAND_NONE, 16), INST_DELAY(20),
  INST_SPAWN(SPAWN_TRI_SHOAL, INST_RAND_NONE, 48), INST_DELAY(20),
  INST_SPAWN(SPAWN_TRI_SHOAL, INST_RAND_NONE, 32), INST_DELAY(20),
  INST_DELAY(60), 
  INST_SPAWN(SPAWN_BIG, INST_RAND_WIDE, 0),
  INST_DELAY(60), 
  INST_END_WAVE
};

PROGMEM const byte waveTest[] = {
  INST_SPAWN(SPAWN_BIG, INST_RAND_NARROW, 26),
  INST_SPAWN(SPAWN_BIG, INST_RAND_NARROW, 39),  INST_DELAY(80),
  INST_SPAWN(SPAWN_BIG, INST_RAND_NARROW, 13),
  INST_SPAWN(SPAWN_BIG, INST_RAND_NARROW, 52), 
  INST_DELAY(127), INST_DELAY(127), 
  INST_END_WAVE
};


#pragma once

#include "gamecore.h"
#include "constants.h"

struct Context;

#define ECHO_GRID_SIZE (3)
#define ECHO_VERT_RESO (SCREEN_HEIGHT / ECHO_GRID_SIZE)
#define ECHO_HORI_RESO (16)
#define ECHO_CYCLE     (60)

struct Echo {
  void reset(Context& context, const byte subX);
  void add(const int left, const char top, const char bottom);
  void draw(Context& context);

  private:
  byte intensities[ECHO_VERT_RESO];
  byte submarineX;
  bool acceptFlag;
};


#define PLATOON_MAX        (5)
#define PLATOON_CONSISTS   (4)
#define PLATOON_USING_MASK (0x80)
#define PLATOON_COUNT_MASK (0x7)

struct Platoons {
  void initialize();
  void set(const char y, const byte type);
  void spawn(Context& context);
  bool checkBonus(const byte idx, bool killed);
  
  private:
  inline bool inUse(const byte idx) const {
    return (status[idx] & PLATOON_USING_MASK) != 0;
  }
  
  char spawnYs[PLATOON_MAX];
  byte types[PLATOON_MAX];
  byte timers[PLATOON_MAX];
  byte status[PLATOON_MAX];
};

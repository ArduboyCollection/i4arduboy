#include <SPI.h>
#include <EEPROM.h>
#include "Arduboy.h"

#include "gamecore.h"
#include "context.h"
#include "title.h"

enum {
  SCENE_TITLE = 0,
  SCENE_GAME,
};

GameCore core;
Title title;
Context context(core);

unsigned char scene = 0;

void setup() {
  Serial.begin(9600);
  core.setup();
  context.initialize();
}

void loop() {
  if(!(core.nextFrame())) {
    return;
  }
  core.clearDisplay();

  switch(scene) {
    case SCENE_TITLE:
      switch(title.loop(core)) {
        case TITLE_START_GAME:
          context.initialize();
          scene = SCENE_GAME;
          break;

        default: break;
      }
      break;
    
    default:
      if(context.loop()) {
        scene = SCENE_TITLE;
      }
      break;
  }
  
/*
  char text[16];
  sprintf(text, "%d, %d, %d", bitmapShockwave1, bitmapShockwave1[0], bitmapShockwave1[1]);
  core.setCursor(10, 10);
  core.print(text);
  */
  
  core.display();
}


#ifndef PLATFORMMANAGER_H
#define PLATFORMMANAGER_H
#include "Character.h"

const int NUM_PLATEFORMES = 3;
const int PIN_BUZZER = 7;

class PlatformManager {
  public:
    int plateformes[NUM_PLATEFORMES][2];

    PlatformManager() {
      plateformes[0][0] = 1; plateformes[0][1] = 5;
      plateformes[1][0] = 3; plateformes[1][1] = 3;
      plateformes[2][0] = 0; plateformes[2][1] = 1;
    }

    void scrollPlatforms() {
      for (int i = 0; i < NUM_PLATEFORMES; i++) {
        plateformes[i][1]++;
        
        if (plateformes[i][1] > BASE_Y) {
          generateNewPlatform(i);
        }
      }
    }

    bool checkPlatformCollision(Character &player) {
      for (int i = 0; i < NUM_PLATEFORMES; i++) {
        if (player.checkCollision(plateformes[i][0], plateformes[i][1])) {
          player.y = plateformes[i][1] - 1;
          player.direction = -1;
          return true;
        }
      }
      return false;
    }

    void jumpSound(){
      tone(PIN_BUZZER, 1000, 50);  
      delay(50); 
      tone(PIN_BUZZER, 1200, 50); 
    }

  private:
    void generateNewPlatform(int index) {
      int newX, newY;
      do {
        newX = random(0, matrix.width() - 1);
        newY = 0;
      } while (!positionValide(newX, newY));
      plateformes[index][0] = newX;
      plateformes[index][1] = newY;
    }

    bool positionValide(int newX, int newY) {
      for (int i = 0; i < NUM_PLATEFORMES; i++) {
        if (abs(newY - plateformes[i][1]) < 2) {
          return false;
        }
      }
      return true;
    }
};

#endif
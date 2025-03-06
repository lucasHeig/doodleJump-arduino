#ifndef CHARACTER_H
#define CHARACTER_H

#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>

const int PIN_MATRIX = 6;
const int BASE_Y = 7;
const int JUMP_HEIGHT = 5;

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(5, 8, PIN_MATRIX, NEO_MATRIX_TOP + 
  NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE, 
  NEO_GRB + NEO_KHZ800);

class Character {
  public:
    int x, y;
    int direction = -1;  // -1 = montée, 1 = descente

    Character() {
      x = 2;  
      y = BASE_Y;
    }

    void moveLeft() {
      if (x > 0) x--;
    }

    void moveRight() {
      if (x < matrix.width() - 1) x++;
    }

    void updatePosition(bool canFall) {
      if (direction == 1 && canFall) {
        y++;
      } else {
        y += direction;
      }
      
      if (y <= BASE_Y - JUMP_HEIGHT) {
        direction = 1;  // Redescend après le sommet du saut
      }
    }

    bool checkCollision(int px, int py) {
      return (direction == 1 && y + 1 == py && (x == px || x == px + 1));
    }
};

#endif
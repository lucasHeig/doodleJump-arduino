#ifndef GAME_H
#define GAME_H

#include "Character.h"
#include "PlatformManager.h"
#include "EEPROM.h" // mémoire 
#include <SparkFunLSM9DS1.h> // accéléromètre
#include <LiquidCrystal.h> // écran lcd

const int SEUIL = 300;  // seuil inclinaison
const int rs = 13, en = 12, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int MELODY[] = { 262, 330, 392, 523, 660, 784, 1046 };
const int NOTE_DURATIONS[] = { 200, 200, 200, 150, 150, 150, 500 };

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LSM9DS1 imu;

class Game {
  private:
    Character player;
    PlatformManager platformManager;
    int score = 0;
    int bestScore = EEPROM.read(0);

  public:
    void setup() {
      Serial.begin(9600);

      lcd.begin(16, 2);
      Wire.begin();
      if (!imu.begin()) {
        Serial.println("Failed to communicate with IMU (LSM9DS1).");
        while (1);
      }
      imu.calibrate();
      matrix.begin();
      matrix.setBrightness(20);
      randomSeed(analogRead(0));

      // Placer personnage sur plateforme la plus basse
      int lowestY = BASE_Y;
      int startX = 2;
      for (int i = 0; i < NUM_PLATEFORMES; i++) {
        if (platformManager.plateformes[i][1] > lowestY) {
          lowestY = platformManager.plateformes[i][1];
          startX = platformManager.plateformes[i][0];
        }
      }
      player.x = startX;
      player.y = lowestY - 1;
    }

    void loop() {
      // Déplacement avec accéléromètre
      imu.readAccel();
      if (imu.ax < -SEUIL) player.moveLeft();
      if (imu.ax > SEUIL) player.moveRight();

      // Vérifier collisions avec plateformes
      bool onPlatform = platformManager.checkPlatformCollision(player);
      if (onPlatform) {
        score++;
        platformManager.jumpSound();
      } 

      // Update position joueur
      player.updatePosition(!onPlatform);

      // Défilement des plateformes si le joueur atteint le haut
      if (player.y <= 3) {
        platformManager.scrollPlatforms();
      }

      // Vérifier si le personnage est tombé
      if (player.y > BASE_Y) {
        gameOver();
      }

      drawGame();
      delay(200);
    }

    void drawGame() {
      matrix.fillScreen(0);

      // Dessiner le personnage (bleu)
      matrix.drawPixel(player.x, player.y, matrix.Color(0, 0, 255));
      matrix.drawPixel(player.x, player.y - 1, matrix.Color(0, 0, 255));

      // Dessiner les plateformes (vert)
      for (int i = 0; i < NUM_PLATEFORMES; i++) {
        int px = platformManager.plateformes[i][0];
        int py = platformManager.plateformes[i][1];
        matrix.drawPixel(px, py, matrix.Color(0, 255, 0));
        matrix.drawPixel(px + 1, py, matrix.Color(0, 255, 0));
      }

      matrix.show();
    }

    bool highScore(int score) {
      bool isNewhighScore = false;
      if(score > bestScore){
        EEPROM.update(0, score);
        bestScore = EEPROM.read(0);
        isNewhighScore = true;
      }
      return isNewhighScore;
    }

    void gameOver() {
      if(!highScore(score)){
        drawUnhappyFace();
        gameOverSound();
      } else {
        drawHappyFace();
        highScoreSound(PIN_BUZZER);
      }
    
      lcd.print("Score: "); lcd.print(score);
      lcd.setCursor(0, 1);
      lcd.print("Best score: "); lcd.print(bestScore);
      delay(2000);
      matrix.fillScreen(0);
      matrix.show();
      Serial.print("Score: "); Serial.println(score);
      Serial.print("Best score: "); Serial.println(bestScore);

      while (1);
    }

    void drawUnhappyFace() {
      matrix.fillScreen(0);
      matrix.drawPixel(1, 1, matrix.Color(255, 0, 0));
      matrix.drawPixel(3, 1, matrix.Color(255, 0, 0));
      matrix.drawPixel(1, 2, matrix.Color(255, 0, 0));
      matrix.drawPixel(3, 2, matrix.Color(255, 0, 0));
      matrix.drawPixel(1, 5, matrix.Color(255, 0, 0));
      matrix.drawPixel(2, 5, matrix.Color(255, 0, 0));
      matrix.drawPixel(3, 5, matrix.Color(255, 0, 0));
      matrix.drawPixel(0, 6, matrix.Color(255, 0, 0));
      matrix.drawPixel(4, 6, matrix.Color(255, 0, 0));
      matrix.show();
    }

    void drawHappyFace() {
      matrix.fillScreen(0);
      matrix.drawPixel(1, 1, matrix.Color(255, 255, 0));
      matrix.drawPixel(3, 1, matrix.Color(255, 255, 0));
      matrix.drawPixel(1, 2, matrix.Color(255, 255, 0));
      matrix.drawPixel(3, 2, matrix.Color(255, 255, 0));
      matrix.drawPixel(0, 4, matrix.Color(255, 255, 0));
      matrix.drawPixel(4, 4, matrix.Color(255, 255, 0));
      matrix.drawPixel(1, 5, matrix.Color(255, 255, 0));
      matrix.drawPixel(2, 5, matrix.Color(255, 255, 0));
      matrix.drawPixel(3, 5, matrix.Color(255, 255, 0));
      matrix.show();
    }

    void highScoreSound(int pin) {
      for (int i = 0; i < 7; i++) {
        tone(pin, MELODY[i], NOTE_DURATIONS[i]);
        delay(NOTE_DURATIONS[i] * 1.3);
      }
      noTone(pin);
    }

    void gameOverSound() {
      tone(PIN_BUZZER, 1000, 200);
      delay(200);
      tone(PIN_BUZZER, 800, 200);
      delay(200);
      tone(PIN_BUZZER, 500, 800);
    }
};

#endif
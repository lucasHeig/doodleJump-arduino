#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>

const int PIN_MATRIX = 6;
const int PIN_LEFT = 8;
const int PIN_RIGHT = 9;

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(5, 8, PIN_MATRIX, 
    NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE, 
    NEO_GRB + NEO_KHZ800);

const int baseY = 7;
const int jumpHeight = 5;
int pos;
int direction = -1;
int x;
int score = 0;

const int numPlateformes = 3;  // Nombre de plateformes visibles à la fois
int plateformes[numPlateformes][2] = {  
  {1, 5},  // Plateforme 1 (X=1, Y=5)
  {3, 3},  // Plateforme 2 (X=3, Y=3)
  {0, 1}   // Plateforme 3 (X=0, Y=1)
};

void setup() {
  Serial.begin(9600);
  matrix.begin();
  matrix.setBrightness(20);
  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_RIGHT, INPUT_PULLUP);
  randomSeed(analogRead(0)); // Initialiser le générateur aléatoire

  // Trouver la plateforme la plus basse pour placer le joueur
  int lowestY = baseY;
  int startX = 2;  // Par défaut au centre

  for (int i = 0; i < numPlateformes; i++) {
    if (plateformes[i][1] > lowestY) {
      lowestY = plateformes[i][1];
      startX = plateformes[i][0]; // Position en X d'une plateforme
    }
  }

  pos = lowestY + 1;  // Placer le joueur juste au-dessus de la plateforme
  x = startX;         // Aligné avec la plateforme
}

void loop() {
  // Déplacement gauche/droite
  if (digitalRead(PIN_LEFT) == LOW && x > 0) {
    x--;
    delay(100);
  }
  if (digitalRead(PIN_RIGHT) == LOW && x < matrix.width() - 1) {
    x++;
    delay(100);
  }

  // Effacer l'affichage
  matrix.fillScreen(0);

  // Dessiner le personnage (rouge)
  matrix.drawPixel(x, pos, matrix.Color(10, 10, 200));
  matrix.drawPixel(x, pos - 1, matrix.Color(10, 10, 200));

  // Dessiner les plateformes (vertes)
  for (int i = 0; i < numPlateformes; i++) {
    int px = plateformes[i][0];
    int py = plateformes[i][1];
    matrix.drawPixel(px, py, matrix.Color(10, 200, 10));
    matrix.drawPixel(px + 1, py, matrix.Color(10, 200, 10));
  }

  matrix.show();
  delay(200);

  // Vérifier si le personnage touche une plateforme par en dessous (éviter de traverser)
  for (int i = 0; i < numPlateformes; i++) {
    int px = plateformes[i][0];
    int py = plateformes[i][1];

    if (direction == 1 && pos + 1 == py && (x == px || x == px + 1)) {
      pos = py - 1;  // Reste sur la plateforme
      direction = -1;  // Rebond automatique
      score++;
    }
  }

  // Mise à jour de la position verticale (descente contrôlée)
  if (direction == 1) {
    bool canFall = true;

    // Vérifier si une plateforme bloque la chute
    for (int i = 0; i < numPlateformes; i++) {
      int px = plateformes[i][0];
      int py = plateformes[i][1];

      if (pos + 1 == py && (x == px || x == px + 1)) {
        canFall = false;
        break;
      }
    }

    if (canFall) {
      pos += direction;
    }
  } else {
    pos += direction;
  }

  // Vérifier les limites du saut
  if (pos <= baseY - jumpHeight) {
    direction = 1;  // Redescendre après le sommet du saut
  }

  // Scrolling des plateformes vers le bas
  if (pos <= 3) { // Déclenche le scrolling quand le joueur atteint le haut
    for (int i = 0; i < numPlateformes; i++) {
      plateformes[i][1]++;  // Déplacer toutes les plateformes vers le bas
      if (plateformes[i][1] >= baseY) { // Si une plateforme sort de l'écran
        plateformes[i][1] = 0; // La replacer en haut
        plateformes[i][0] = random(0, matrix.width() - 1); // Nouvelle position aléatoire en X
      }
    }
  }

  // Vérifier si le personnage tombe en dehors de l'écran -> GAME OVER
  if (pos > baseY) {
    drawSkull();
    delay(5000);
    matrix.fillScreen(0);  // Éteindre la matrice
    matrix.show();
    Serial.print("Score: "); Serial.println(score);
    while (1);  // Bloquer le programme
  }
}

void drawSkull() {
  matrix.fillScreen(0);  // effacer l'écran

  matrix.drawPixel(1, 1, matrix.Color(255, 0, 0));
  matrix.drawPixel(3, 1, matrix.Color(255, 0, 0));
  matrix.drawPixel(1, 2, matrix.Color(255, 0, 0));
  matrix.drawPixel(3, 2, matrix.Color(255, 0, 0));

  matrix.drawPixel(1, 5, matrix.Color(255, 0, 0));
  matrix.drawPixel(2, 5, matrix.Color(255, 0, 0));
  matrix.drawPixel(3, 5, matrix.Color(255, 0, 0));
  matrix.drawPixel(0, 6, matrix.Color(255, 0, 0));
  matrix.drawPixel(4, 6, matrix.Color(255, 0, 0));

  matrix.show();  // update affichage
}

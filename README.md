# DoodleJump Arduino

![Description de l'image](/docs/assets/photo-de-mon-projet.png)

Réalisé par Lucas Tschaler  
Option Arduino 2025 – COMEM+

---

## Description

Mon projet est un jeu de type Doodle Jump. Le but est simple :
Le joueur doit diriger un personnage sur des plateformes sans tomber dans le vide. Le personnage saute automatiquement, le joueur peut simplement déplacer le personnage de gauche à droite. Dès que le personnage touche une plateforme, le score augmente. Le nombre de plateformes est infini.

Voici le matériel pour réaliser le jeu :

- Carte Arduino Uno R3
- Matrice de LED RGB 5x8 (Adafruit NeoPixel Shield)
- Accéléromètre (Sparkfun LSM9DS1)

L'affichage du jeu se fait sur la matrice de LEDs. Le personnage est représenté par deux pixels verticales bleues et les plateformes sont deux pixels horizontales vertes. Pour le game over, un smiley pas content apparaît en rouge.
Pour déplacer le personnage de gauche à droite, le joueur utilise l'accéléromètre.
J'ai essayé plusieurs contrôleurs : deux boutons ou avec un joystick. L'accéléromètre étant le plus fun et dynamique à utiliser, j'ai opté pour celui-ci. Par contre, il rend le jeu plus compliqué.

## Documentation

Retrouvez la documentation dans le dossier [docs](docs/).

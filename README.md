# Tetris 3D - OpenGL

Tetris 3D est une implémentation moderne et immersive du jeu classique Tetris en utilisant la bibliothèque graphique OpenGL. Profitez de la stratégie de Tetris dans un environnement tridimensionnel avec des contrôles fluides et un gameplay captivant.

---

## Aperçu
![Capture d'écran du gameplay](path/to/screenshot.jpg)  
*Une capture d’écran montrant un tetromino en mouvement dans la matrice 3D.*

---

## Fonctionnalités principales
- Jeu Tetris complet avec des pièces classiques adaptées à la 3D.
- Rotations autour des axes X, Y et Z.
- Matrice de jeu tridimensionnelle dynamique.
- Contrôles via clavier et souris.
- Affichage graphique performant avec OpenGL.

---

## Installation

### **Prérequis**
Assurez-vous d'avoir les outils suivants installés sur votre système :
- Un compilateur C++ (comme GCC ou MSVC).
- La bibliothèque OpenGL.
- La bibliothèque GLUT (ou FreeGLUT).

### **Instructions**
2. Installez les dependecies :
   ```bash
    sudo apt-get install build-essential freeglut3 
   ```

2. Clonez le dépôt :
   ```bash
   git clone https://github.com/votre-repo/tetris-3d.git
   cd tetris-3d
   ```
3. Compilez le projet (avec g++ par exemple) :
   ```bash
   g++ -o tetris3d testnew.cpp -lGL -lGLU -lglut
   ```
4. Lancez le jeu :
   ```bash
   ./tetris3d
   ```

---

## Comment jouer

### **Contrôles clavier**
- **P** : Mettre en pause / Reprendre le jeu.
- **A/D** : Déplacer la pièce à gauche / à droite.
- **W/S** : Déplacer la pièce vers l’avant / l’arrière.
- **Q** : Descendre la pièce plus rapidement.
- **E/R/F** : Tourner la pièce autour des axes X, Y et Z.

### **Souris**
- **Clic gauche + Mouvement** : Rotation de la caméra.
- **Clic droit + Mouvement** : Panoramique.
- **Molette** : Zoom avant / arrière.

---

## Problèmes connus
- Certaines rotations peuvent provoquer des collisions incorrectes.
- L’optimisation graphique peut être améliorée pour des matrices plus grandes.

---

## Améliorations futures
- Implémentation d’un système de score.
- Ajout d'un mode multijoueur en ligne.
- Création d’une IA pour simuler un adversaire.

---

## Contributeurs
- [Malek BelKahla]
- [Elyes Bouaziz]

---

## Licence
Ce projet est sous licence [MIT](LICENSE).

---

Merci d'avoir joué à Tetris 3D !


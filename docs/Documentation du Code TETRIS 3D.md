**Documentation du Code TETRIS 3D**

#### **1\. Introduction**

Ce projet implémente une version 3D du jeu Tetris en utilisant C++ et OpenGL. Le jeu est conçu pour simuler des pièces tombantes (tetrominos) dans un espace tridimensionnel, avec des fonctionnalités comme la rotation des pièces, le déplacement, et la détection des collisions.

---

#### **2\. Classes et Fonctions Principales**

##### **2.1 Classe `tetromino`**

Cette classe représente une pièce de Tetris. Elle contient les données et méthodes nécessaires pour manipuler une pièce, comme la rotation, le déplacement, et la gestion de sa position.

* **Attributs :**

  * `Shape shape` : Type de tetromino (I, J, O, S, T).  
  * `std::vector<std::tuple<float, float, float>> blockPositions` : Coordonnées 3D des blocs qui composent le tetromino.  
  * `float rotationX, rotationY, rotationZ` : Angles de rotation autour des axes X, Y et Z.  
* **Méthodes :**

  * `initializeShape()` : Initialise la forme et les positions des blocs.  
  * `rotate(int axis, float angle)` : Effectue une rotation autour d’un axe (X, Y ou Z).  
  * `move(float dx, float dy, float dz)` : Déplace le tetromino dans l’espace.  
  * `getBlockPositions()` : Renvoie les positions actuelles des blocs.

##### **2.2 Fonction `displayTetromino`**

Affiche un tetromino spécifique avec OpenGL.

* **Entrées :**

  * `const tetromino& t` : Le tetromino à afficher.  
  * `int color` : La couleur associée au tetromino.  
* **Sorties :**

  * Utilise OpenGL pour dessiner les cubes qui composent le tetromino avec leurs rotations et positions respectives.

##### **2.3 Fonction `checkCollision`**

Vérifie si un tetromino entre en collision avec une autre pièce ou le sol.

* **Entrées :**

  * `const tetromino& t` : Le tetromino à vérifier.  
* **Retour :**

  * `true` si une collision est détectée, `false` sinon.

##### **2.4 Fonction `rotateTetromino`**

Permet de tourner la pièce sélectionnée autour d’un axe donné.

* **Entrées :**  
  * `int axis` : L’axe de rotation (0 \= X, 1 \= Y, 2 \= Z).

---

#### **3\. Boucle Principale**

##### **3.1 Fonction `main`**

Initialise le programme, configure OpenGL, et lance la boucle principale du jeu.

* **Étapes :**  
  1. Initialisation des paramètres OpenGL (fenêtre, couleurs, profondeur).  
  2. Enregistrement des fonctions de rappel (`glutDisplayFunc`, `glutKeyboardFunc`, etc.).  
  3. Création initiale des tetrominos.  
  4. Lancement de la boucle de rendu (`glutMainLoop`).

---

#### **4\. Interactions Utilisateur**

##### **4.1 Commandes Clavier**

Les touches clavier contrôlent les déplacements, rotations, et autres interactions.

* **Touches principales :**  
  * `A/D` : Déplacer le tetromino à gauche/droite.  
  * `W/S` : Déplacer le tetromino vers l’avant/arrière.  
  * `E/R/F` : Rotation autour des axes X, Y, et Z.  
  * `P` : Pause ou reprise du jeu.

##### 

##### **4.2 Souris**

La souris permet de contrôler la caméra (zoom, rotation, panoramique).

---

#### **5\. Problèmes Connus**

1. **Rotations incorrectes :** Lors de certaines rotations, les pièces sortent des limites ou entrent en collision.

   * **Solution envisagée :** Affiner la logique de collision après rotation.  
2. **Synchronisation graphique :** Les mises à jour ne sont pas toujours parfaitement fluides.

   * **Solution envisagée :** Optimiser les appels `glutPostRedisplay`.


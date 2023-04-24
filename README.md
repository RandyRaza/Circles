# Travail pratique 3 : Le jeu *Circles*

## Description

* Conception d'un jeu Circles, qui reprend l'idée du jeu Super Hexagon.
* le but du jeu étant de manipuler un personnage, (représenté par un triangle) afin d'éviter des murs qui se rapprochent
* du triangle.
* Projet réalisé dans le cadre du cours INF3135 - 

## Auteur

- Randy Razafindrabe RAZR11308805

## Dépendances

* SDL2/SDL2_gfxPrimitives.h
* SDL2/SDL_mixer.h
* application.h
* constants.h
* menu.h
* play.h
* sdl2.h
* spritesheet.h
* stdbool.h
* stdio.h
* stdlib.h


## Fonctionnement

* Pour compiler le programme, il faut simplement utiliser la commande `make`, une fois compilé un exécutable sera créé 
  entrer `./circles` en ligne de commande pour lancer la partie. Une fois la partie lancée, l'utilisateur pourra choisir
  un niveau de difficulté en utilisant les flèches directionnelles du clavier. La partie continuera tant que le joueur 
  n'a pas perdu la partie.
* Le jeu reprend le principe du jeu Super hexagon, le joueur est représenté par un petit triangle 
  qui doit bouger autour d'un disque central pour éviter les murs qui se rapprochent.
* Les touches pour naviguer dans le menu sont les flèches de direction
* De la même manière que le menu, le triangle est supposé se déplacer grace au flèches du clavier


## Plateformes supportées

l'application a été testé sur le système d'exploitation MacOs, cependant il devrait fonctionner sans problème sur un 
environnement Linux.


## Division des tâches

 J'ai réalisé ce travail seul.

## État du projet

Le projet n'est pas terminé, il faut implémenter toute la partie "Jeu", seule la musique et le choix des niveaux de 
difficulté ont été réalisés.

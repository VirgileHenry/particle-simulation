
# <center>Quarks</center>

### <center>Implémentation d'une librairie de simulation de particules</center>

## Introduction

Quark est une librairie "header-only" de simulation de particule, donnant accès aux fonctionnalités de base ainsi qu'a une simplicité de rajouter ses propres systèmes. 

A son coeur, cette librairie est une collection de particule qui possèdent force, vitesse et position, et auxquelles on applique les force d'interactions données.

## Exemple d'utilisation :

```c++
// Tout d'abord, il est pratique de définir le type d'univers que l'on va utiliser, car on peut se retrouver à le ré-écrire plusieurs fois.
// les paramètres template sont la dimension, le nombre de particule, la longueur caractéristique et le rayon de coupe.
typedef Universe<2, 3000, 1.0, 0.2> MyUniverse;

// Ensuite, on instancie notre univers. Avec le constructeur par défaut, il va générer le nombre de particules demandés dans le cube [0, 1]^D .
MyUniverse universe = MyUniverse();

// On peut ensuite rajouter autant d'intéracteurs que l'on veut. Il existe des interacteurs par défaut, mais il est possible de créer ses propres interacteurs.
GravityInteractor<2> interactor = GravityInteractor<2>();
universe.registerInteractor(&interactor);

// de même pour la visualisation.
SDLVisulizer<MyUniverse> visulizer = SDLVisulizer<MyUniverse>();
universe.registerVisualizer(&visulizer);

// Enfin, on lance la simulation en appelant la méthode "step" de l'univers.
for(unsigned int i = 0; i < 1000; i++) {
    universe.step(0.0000001);
}

```

## Lab 1

On teste les performances d'ajout de particules à une liste pour différents nombres de particules:

    int nbParticles = 64 -> Mesure : 0.000371615s
    int nbParticles = 128 -> Mesure : 0.000774339s
    int nbParticles = 1024 -> Mesure : 0.00566584s
    int nbParticles = 2048 -> Mesure : 0.0103812s
    int nbParticles = 4096 -> Mesure : 0.0159918s
    int nbParticles = 32768 -> Mesure : 0.0683438s

On commence ainsi à avoir une véritable différence de performances pour 2048 particules en 
arrivant au centième de seconde de temps de calcul. 

## Lab 2
## Lab 3
## Lab 4
## Lab 5
## Lab 6

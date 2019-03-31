# Pentaminos

**Difficulté :** moyenne    
**Temps :** 3-7 heures

## Présentation

Ci-joint des fichiers contenant quelques classes permettant de construire des
images en mémoire puis de les exporter sous forme de fichiers png.

Les sources sont dans l'arborescence src et sont en c++11.

Le dossier racine contient un fichier Jamroot permettant de construire le
projet avec boost.build (bjam). Vous pouvez créer votre propre makefile
ou utiliser n'importe quel autre système de build (cmake, scons, Visual Studio,
etc.si vous préférer, mais dans ce cas vous devrez le créer vous même. Pas
de panique le fichier de build est très simple.

## Objectif

L'objectif est de créer un programme cherchant une solution au jeu des
pentaminos.

Les pentaminos sont les 12 formes possibles à obtenir avec des formes composées
de 5 carreaux (comme Tetris, mais avec un carreau de plus).

Les 12 formes possibles sont les suivantes:

```
+--------+------+------+-----+-----+------+   
| XXXXXX |  X   | X    | XXX | XXX |  X   |   
|        | XXXX | XXXX | X   | XX  |  XXX |   
|        |      |      | X   |     |  X   |   
+--------+------+------+-----+-----+------+   
| X      | X    | X    | XXX | XX  |  X   |   
| XXXX   | XXX  | XXX  | X X |  XX | XXX  |   
|        |   X  |  X   |     |   X |  X   |   
+--------+------+------+-----+-----+------+   
```

Le but du problème est d'écrire un programme permettant de disposer
ces 12 formes dans un rectangle de 6 x 10. 

On peut arrêter le programme à la première solution trouvée ou le 
laisser chercher toutes les solutions.

On utilisera ensuite le code fourni en exemple (voir src/main/draw.cpp) pour
générer une image png représentant une solution trouvée, chaque pièce étant 
dessinée en utilisant une couleur différente. Le résultat sera ensuite exporté 
par le programme dans un fichier png.

## Recommandations:

- la façon de faire compte autant que de faire un programme fonctionnel.

- on s'attachera à faire des classes modelisant les objets manipulés
plutôt qu'une unique fonction.

Have fun.


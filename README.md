# ========Format de compression d’image numérique - Théo BRYANT et Adrien DEVILLEZ GRP2========

## Compilation

Pour compliler la bibliothèque partagée il suffit d'inscrire la commande suivante:

**make -f makelib**

Le fichier libqtc.so se trouvera ensuite dans le répertoire lib.

Pour compliler le projet utilisant la bibliothèque partagée il faut utiliser la commande suivante:

**make -f makeapp**

Pour executer le programme, utilisez :

./codec [options]

-c                              : encodeur

-u                              : décodeur

-i input.{pgm|qtc}              : nom du fichier d’entrée

-o output.{qtc|pgm}             : renommer le fichier de sortie - {QTC|PGM}/out.{qtc|pgm} (par défaut)

-a nb                        	: seuil de filtrage (0.0 par défaut (= aucun filtrage))

-g                              : édition de la grille de segmentation

-h                              : aide

-v                              : mode bavard - silence (par défaut)

Pour nettoyer le projet ainsi que la bibliothèque partagée:

**make -f makelib clean**

**make -f makeapp clean**

## Repartition du travail

Nous nous sommes répartis le projet de manière équitable. Nous avons dû revoir à plusieurs reprises nos fonctions et adapter le code de chacun lorsqu'une personne avaçait sur une partie afin de fournir un code qui soit lisible et compréhensible au sein du binôme mais aussi pour les correcteurs. De plus, nous avons eu des difficultées sur la réalisation de la grille de segmentation mais aussi la manière dont il fallait calculer les variances pour le filtrage. D'autre part, nous avons essayer de gérer au maximum toutes les options possibles et de gérer par la même occasion les erreurs sur la ligne de commande (option non reconnue, tentative de décompression d’un fichier PGM...)

# ======================================================

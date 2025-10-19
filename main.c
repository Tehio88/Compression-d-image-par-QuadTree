#include "../include/qtc.h"
#include "../include/pgm.h"

void help() {
    printf("usage : ./codec [options]\n");
    printf("-c                              : encodeur\n");
    printf("-u                              : décodeur\n");
    printf("-i input.{pgm|qtc}              : nom du fichier d’entrée\n");
    printf("-o output.{qtc|pgm}             : renommer le fichier de sortie - {QTC|PGM}/out.{qtc|pgm} (par défaut)\n");
    printf("-a (nb)                         : seuil de filtrage (0.0 par défaut (= aucun filtrage))\n");
    printf("-g                              : édition de la grille de segmentation\n");
    printf("-h                              : aide\n");
    printf("-v                              : mode bavard - silence (par défaut)\n");
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Erreur : Nombre d'argument incorrect.\n");
        return EXIT_FAILURE;
    }

    int option;
    int mode = -1; // Valeur par défaut
    int verbose = 0; // Silence par défaut
    int grille_segmentation = 0;
    float alpha = 0.0; // Seuil pour le filtrage
    char * input = "<stdin>";
    char * output = "<stdout>";

    while((option = getopt(argc, argv, ":cua:i:o:ghv")) != -1) {
        switch(option) {
            case 'c':
                mode = 0;
                break;
            case 'u':
                mode = 1;
                break;
            case 'a':
                alpha = atof(optarg);
                if (alpha < 0) {
                    fprintf(stderr, "Erreur : -a nécissite une valeur positive ou nulle\n");
                    return -1;
                }
                break;
            case 'i':
                input = optarg;
                break;
            case 'o':          
                output = optarg;
                break;
            case 'g':       
                grille_segmentation = 1;
                break;
            case 'v':
                verbose = 1;
                break;
            case 'h':
                help();
                return 0;
            default: 
                fprintf(stderr, "Erreur : Option inconnue ou oubli d'un argument <-%c>\n", optopt);
                help();
                return -1;
                break;
        }
    }

    if(mode == -1) { // Pas d'option de traitement
        fprintf(stderr, "Vous n'avez pas entré le sens de traitement encodeur (-c) ou décodeur (-u)\n");
        return -1;
    }

    if(strcmp(output, "<stdout>") == 0) { // fichier de sortie par défaut
        output = (mode == 0) ? "QTC/out.qtc" : "PGM/out.pgm";
    }

    if(strcmp(input, output) == 0) { 
        fprintf(stderr, "Erreur : Le fichier d'entrée et de sortie ne peuvent pas être identiques\n");
    }

    if((mode == 0) && !input) {
        fprintf(stderr, "Erreur : Fichier d'entrée manquant pour l'encodeur\n");
        return -1;
    }

    if(verbose) {
        printf("Sens de traitement : %s\n", (mode == 0) ? "encodeur" : "décodeur");
        printf("Fichier d'entrée : %s\n", input);
        printf("Fichier de sortie : %s\n", output);
        printf("Grille de segmentation : %s\n", (grille_segmentation == 0) ? "désactivée" : "activée");
        printf("Seuil de perte : %f\n", alpha);
    }

    
    if(mode == 0) { // MODE ENCODEUR
        FILE * fichier_in = fopen(input, "rb");
        if (!fichier_in) {
            fprintf(stderr, "Erreur lors de l'ouverture du fichier d'entrée\n");
            return -1;
        }

        int sizeTab = 0;
        Cellule *tab;
        tab = readPGM(fichier_in, &sizeTab);
        if (!tab) {
            fprintf(stderr, "Erreur : Lecture du fichier PGM échouée.\n");
            fclose(fichier_in);
            return -1;
        }

        if(alpha != 0.0) {
            float medvar, maxvar = 0.0;
            calc_variances(tab, sizeTab + tailleracine(sizeTab), &medvar, &maxvar);
            float sigma = medvar / maxvar;
            filtrage(tab, 0, sizeTab + tailleracine(sizeTab), sigma, alpha);
        }
        codage(tab, 0, sizeTab + tailleracine(sizeTab));


        FILE * fichier_out = fopen(output, "wb");
        if (!fichier_out) {
            fprintf(stderr, "Erreur lors de l'ouverture du fichier de sortie\n");
            free(tab);
            fclose(fichier_in);
            return -1;
        }
        writeQTC(fichier_out, tab, sizeTab + tailleracine(sizeTab));

        if (grille_segmentation) {
            if (verbose) {
                printf("Edition de la grille de segmentation activée.\n");
            }
            codageSegm(tab,0,sizeTab +tailleracine(sizeTab), tailleracine(sizeTab));
            FILE * segm = fopen("segm.pgm", "wb");
            writePGM(segm, tab, sqrt(sizeTab), tailleracine(sizeTab));
        }

        free(tab);
    } else { // MODE DECODEUR
        FILE * fichier_in = fopen(input, "rb");
        if (!fichier_in) {
            fprintf(stderr, "Erreur lors de l'ouverture du fichier QTC pour lecture\n");
            return -1;
        }

        int sizeTab = 0;
        Cellule *tab;
        tab = readQTC(fichier_in, &sizeTab);
        if (!tab) {
            fprintf(stderr, "Erreur : Lecture du fichier QTC échouée.\n");
            fclose(fichier_in);
            return -1;
        }
        decodage(tab, 0, sizeTab + tailleracine(sizeTab));
        
        FILE *fichier_out = fopen(output, "wb");
        if (!fichier_out) {
            fprintf(stderr, "Erreur lors de l'ouverture du fichier PGM pour écriture");
            free(tab);
            return -1;
        }
        writePGM(fichier_out, tab, sqrt(sizeTab), tailleracine(sizeTab));

        if (grille_segmentation) {
            if (verbose) {
                printf("Edition de la grille de segmentation activée.\n");
            }
            codage(tab, 0, sizeTab + tailleracine(sizeTab));
            codageSegm(tab,0,sizeTab +tailleracine(sizeTab), tailleracine(sizeTab));
            FILE * segm = fopen("segm.pgm", "wb");
            writePGM(segm, tab, sqrt(sizeTab), tailleracine(sizeTab));
        }

        free(tab);
    }
    return 0;
}

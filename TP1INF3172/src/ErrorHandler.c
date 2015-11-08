#include "ErrorHandler.h"
#include "Fichier.h"
#include <stdio.h>
#include <string.h>

int verifierCheminAbsolu(char * chemin){
    if (strlen(chemin) > 40){
        printf("\nLe chemin absolu est trop long : 40 caracteres maximum.\n");
        return 0;
    }
    return 1;
}

int verifierExisteDeja(char *chemin){
    if(getAdressRepertoire(chemin) == -1){
        return 1;
    }
    printf("%s existe deja\n", chemin);
    return 0;
}

int verifierFicLongueur(char * chemin){
    char * contenu = strchr(chemin, ' ');
    if (!contenu || !contenu[1]){
        printf("Le contenu du fichier ne peut pas etre vide\n");
        return 0;
    }
    contenu++;
    int len = strlen(contenu);
    if (len > 590){
        printf("La longueur du contenu du fichier ne peut pas etre superieur a 590");
        return 0;
    }
    return 1;
}

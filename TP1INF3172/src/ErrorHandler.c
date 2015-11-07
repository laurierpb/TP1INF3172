#include "ErrorHandler.h"
#include <string.h>


int verifierCheminAbsolu(char * chemin){
    if (strlen(chemin) > 40){
        printf("\nLe chemin absolu est trop long : 40 caracteres maximum.\n");
        return 1;
    }
    return 0;
}

int verifierExist(char * chemin){
    printf("%s existe", chemin);
    printf("%s n'existe pas", chemin);
    return 0;
}

int verifierRepParent(char * chemin){
    char * nom = strrchr(chemin, '/');
    //nom est vide
    if (!nom){
        return 0;
    }
    // nom = chemin
    if (!strcmp(nom, chemin)){
        return verifierExist(nom);
    }

    char tmp[1000];
    int lenNom = strlen(nom);
    int len = strlen(chemin);
    int pos = len - lenNom;

    strncpy(tmp, chemin, pos);

    return verifierExist(tmp);
}

int verifierFicVide(char * chemin){
    return 0;
}

int verifierFicVolume(char * chemin){
    return 0;
}

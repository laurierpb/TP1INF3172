#ifndef FICHIER_H
#define FICHIER_H

char blocLibre[64000];

typedef struct iNode{
    char bloc[8];
    struct indirectionSimple* indSimp;
    struct IndirectionDouble* indDoub;
    struct indirectionTriple* indTrip;
} iNode;

typedef struct indirectionSimple{
    char bloc[8];
} indirectionSimple;

typedef struct IndirectionDouble{
    struct indirectionSimple* listIndSimp[8];
} IndirectionDouble;

typedef struct indirectionTriple{
    struct IndirectionDouble* listIndDoub[8];
} indirectionTriple;

int creerRepertoire(char* name);
int supprimerRepertoire(char* name);
int creerFichier(char* name, char* contenu);
int supprimerFichier(char* name);
int lireFichier(char* name);

#endif // FICHIER_H

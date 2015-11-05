#ifndef FICHIER_H
#define FICHIER_H

int blocLibre[64000] = {0};

typedef struct iNode{
    int bloc[8];
    struct indirectionSimple* indSimp;
    struct IndirectionDouble* indDoub;
    struct indirectionTriple* indTrip;
} iNode;

typedef struct indirectionSimple{
    int bloc[8];
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
iNode* populateInode();

#endif // FICHIER_H

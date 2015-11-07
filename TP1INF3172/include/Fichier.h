#ifndef FICHIER_H
#define FICHIER_H

int blocLibre[64000] = {0};
int blocLibreIndirection[1000] = {0};
int blocLibreInode[100] = {0};

int blocLibreFichier[1000] = {0};

typedef struct iNode{
    int bloc[8];
    int indirectionSimple;
    int indirectionDouble;
    int indirectionTriple;
} iNode;

typedef struct indirection{
    int bloc[8];
} indirection;

int creerRepertoire(char* name);
int supprimerRepertoire(char* name);
int creerFichier(char* name, char* contenu);
int supprimerFichier(int positionInode);
int lireFichier(char* name);
void printInode(int position);
void loadBlocLibre();
iNode* populateInode();

#endif // FICHIER_H

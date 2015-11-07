#ifndef FICHIER_H
#define FICHIER_H
typedef struct iNode{
    int bloc[8];
    int indirectionSimple;
    int indirectionDouble;
    int indirectionTriple;
} iNode;

typedef struct indirection{
    int bloc[8];
} indirection;

typedef struct element{
    char chemin[40];
    int parent;
    int adresseInode;
} element;

int creerRepertoire(char* name);
int supprimerRepertoire(char* name);
int creerFichier(char* name, char* contenue);
int supprimerFichier(int positionInode);
int lireFichier(char* name);
void loadBlocLibre();

#endif // FICHIER_H

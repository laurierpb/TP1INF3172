#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Fichier.h>

void printInode(struct iNode *node);
void writeCharacterInFile(char charactere, int position);
int gestionIndirectionSimple(char* contenue, int *position);
int gestionIndirectionDouble(char* contenue, int *position);
int gestionIndirectionTriple(char* contenue, int *position);
void writeNodeInInodeFile(iNode *node);

int creerRepertoire(char* name){
    return 0;
}

int supprimerRepertoire(char* name){
    return 0;
}

int creerFichier(char* name, char* contenue){
    iNode *node = malloc(sizeof(iNode));
    node->indirectionSimple = -1;
    node->indirectionDouble = -1;
    node->indirectionTriple = -1;
    //initialisation des indirection pour dire qu'ils n'ont pas ete aloué

    int positionInode = 0;
    int positionInFile = 0;

    int i = 0;
    while(contenue[i] != '\0'){
        /**Trouve l'espace ou il peut ecrire*/
        while(blocLibre[positionInFile] != 0){
            positionInFile++;
        }
        if(i<8){
            writeCharacterInFile(contenue[i], positionInFile);
            positionInFile++;
            node->bloc[positionInode] = positionInFile;
            positionInode++;
            blocLibre[i] = 1;
            i++;
        }else if(node->indirectionSimple == -1){
            node->indirectionSimple = gestionIndirectionSimple(contenue, &i);
        }else if(node->indirectionDouble == -1){
            node->indirectionDouble = gestionIndirectionDouble(contenue, &i);
        }else if(node->indirectionTriple == -1){
            node->indirectionTriple = gestionIndirectionTriple(contenue, &i);
        }
    }
    writeNodeInInodeFile(node);
    free(node);
    return 0;
}

int supprimerFichier(char* name){
    return 0;
}

int lireFichier(char* name){
    return 0;
}

void writeNodeInInodeFile(iNode *node){
    int position = 0;
    while(blocLibreInode[position] != 0){
        position++;
    }
    blocLibreInode[position] = 1;

    FILE *fp = fopen("inode.dat", "rb+");
    fwrite(blocLibreInode, sizeof(blocLibreInode), 1, fp);
    fseek(fp, position + sizeof(blocLibreInode)/sizeof(blocLibreInode[1]), SEEK_SET);
    fwrite(&node, sizeof(iNode), 1, fp);
    fclose(fp);
}

void writeCharacterInFile(char charactere, int position){
    FILE *fp = fopen("chaine.dat", "rb+");
    fseek(fp, position, SEEK_SET);
    fwrite(&charactere, sizeof(charactere), 1, fp);
    fclose(fp);
}

void writeBlockEntierInFile(int *bloc, int position){
    FILE *fp = fopen("indirection.dat", "rb+");
    fseek(fp, position*sizeof(int) +
          sizeof(blocLibreIndirection)/sizeof(blocLibreIndirection[0]), SEEK_SET);
    fwrite(bloc, sizeof(int), 8, fp);
    fclose(fp);
}

void afficheEntierInFileFromPosition(int position){
    int test[8];
    FILE *fp = fopen("indirection.dat", "rb+");
    fseek(fp, position * sizeof(int) +
          sizeof(blocLibreIndirection)/sizeof(blocLibreIndirection[0]), SEEK_SET);
    fread(&test, sizeof(int), 8, fp);
    int j=0;
    for(j=0; j<8 ; j++){
        printf("le chifre qui a ete lu est : %d\n", test[j]);
    }
}

/**
* Fait tout la gestion de l'adressage de l'indirection simple
*/
int gestionIndirectionSimple(char* contenue, int *position){
    indirection *indirectionSimple = malloc(sizeof(indirection));
    int positionInode = 0; /**position dans la structure block[]*/
    int positionBlocLibre = 0; /**Position dans le fichier fichier.dat*/

    for(; contenue[*position] != '\0' ; *position = *position + 1){
        /**Trouve l'espace ou il peut ecrire*/
        while(blocLibre[positionBlocLibre] != 0)
            positionBlocLibre++;

        writeCharacterInFile(contenue[*position], positionBlocLibre);
        indirectionSimple->bloc[positionInode] = positionBlocLibre;
        blocLibre[positionBlocLibre] = 1;
        positionInode++;
        if(positionInode == 8){
            break;
        }
    }

    while(positionInode < 8){
        indirectionSimple->bloc[positionInode] = -1;
        positionInode++;
    }

    /**set le bloque libre d'indirection*/
    int i = 0;
    while(blocLibreIndirection[i] != 0){
        i++;
    }

    blocLibreIndirection[i] = 1;
    writeBlockEntierInFile(indirectionSimple->bloc, i);
    return i;
}

/**
* Fait tout la gestion de l'adressage de l'indirection double
*/
int gestionIndirectionDouble(char* contenue, int *position){
    indirection *indirectionDouble = malloc(sizeof(indirection));

    int j = 0;
    for(j = 0 ; j < 8 ; j++){
        if(contenue[*position] == '\0'){
            break;
        }
        indirectionDouble->bloc[j] = gestionIndirectionSimple(contenue, position);
    }
    //allocation manuel d'une valeur impossible dans le but de ne pas
    //laisser le s/e mettre une valeur et corrompre ces données
    while(j < 8){
        indirectionDouble->bloc[j] = -1;
        j++;
    }

    //set le bloque libre d'indirection
    int i = 0;
    while(blocLibreIndirection[i] != 0){
        i++;
    }
    if(indirectionDouble->bloc[0] == -1){
        return -1;
    }
    writeBlockEntierInFile(indirectionDouble->bloc, i);
    blocLibreIndirection[i] = 1;
    return i;
}

/**
* Fait tout la gestion de l'adressage de l'indirection triple
*/
int gestionIndirectionTriple(char* contenue, int *position){
    indirection *indirectionTriple = malloc(sizeof(indirection));

    int j = 0;
    for(j = 0 ; j < 8 ; j++){
        if(contenue[*position] == '\0'){
            break;
        }
        indirectionTriple->bloc[j] = gestionIndirectionDouble(contenue, position);
    }
    //allocation manuel d'une valeur impossible dans le but de ne pas
    //laisser le s/e mettre une valeur et corrompre ces données
    while(j < 8){
        indirectionTriple->bloc[j] = -1;
        j++;
    }

    //set le bloque libre d'indirection
    int i = 0;
    while(blocLibreIndirection[i] != 0){
        i++;
    }
    if(indirectionTriple->bloc[0] == -1){
        return -1;
    }
    writeBlockEntierInFile(indirectionTriple->bloc, i);
    blocLibreIndirection[i] = 1;
    return i;
}

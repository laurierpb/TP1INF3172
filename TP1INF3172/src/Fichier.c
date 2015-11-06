#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Fichier.h>

void printInode(struct iNode *node);
void writeCharacterInFile(char charactere, int position);
int gestionIndirectionSimple(char* contenue, int *position, int *positionInFile);
int gestionIndirectionDouble(char* contenue, int position);
int gestionIndirectionTriple(char* contenue, int position);

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

    int i;
    for(i = 0 ; contenue[i] != '\0' ; i++){
        /**Trouve l'espace ou il peut ecrire*/
        while(blocLibre[positionInFile] != 0){
            positionInFile++;
        }
        if(i<8){
            writeCharacterInFile(contenue[i], positionInFile);
            positionInFile++;
            node->bloc[positionInode] = positionInFile;
            positionInode++;
        }else if(node->indirectionSimple == -1){
            node->indirectionSimple = gestionIndirectionSimple(contenue, &i, &positionInFile);
            printf("l'indirection simple pointe vers : %d", node->indirectionSimple);
        }
    }

    free(node);
    return 0;
}

int supprimerFichier(char* name){
    return 0;
}

int lireFichier(char* name){
    return 0;
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

int gestionIndirectionSimple(char* contenue, int *position, int *positionInFile){
    indirection *indirectionSimple = malloc(sizeof(indirection));
    int positionInode = 0; //position dans la structure

    for(; contenue[*position] != '\0' ; *position = *position + 1){
        /**Trouve l'espace ou il peut ecrire*/
        while(blocLibre[*positionInFile] != 0){
            *positionInFile = *positionInFile + 1;;
        }

        writeCharacterInFile(contenue[*position], *positionInFile);
        indirectionSimple->bloc[positionInode] = *positionInFile;
        *positionInFile = *positionInFile + 1;
        positionInode++;
        if(positionInode == 8){
            break;
        }
    }
    //set le bloque libre d'indirection
    int i = 0;
    while(blocLibreIndirection[i] != 0){
        i++;
    }
    blocLibreIndirection[i] = 1;

    writeBlockEntierInFile(indirectionSimple->bloc, i);
    afficheEntierInFileFromPosition(i);
    return i;
}

int gestionIndirectionDouble(char* contenue, int position){
    int retour;
    return retour;
}

int gestionIndirectionTriple(char* contenue, int position){
    int retour;
    return retour;
}


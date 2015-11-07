#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Fichier.h>

void writeCharacterInFile(char charactere, int position);
int gestionIndirectionSimple(char* contenue, int *position);
int gestionIndirectionDouble(char* contenue, int *position);
int gestionIndirectionTriple(char* contenue, int *position);
int writeNodeInInodeFile(iNode *node);
int creerInode(char* contenue);
void printIndirectionSimple(int indirectionSimple);
void printIndirectionDouble(int indirectionSimple);
void printIndirectionTriple(int indirectionSimple);
void writeCharacterInFile(char charactere, int position);
void writeBlockEntierInFile(int *bloc, int position);
void afficheEntierInFileFromPosition(int position);
void supprimerIndirectionSimple(int positionIndirection);
void supprimerIndirectionDouble(int positionIndirection);
void supprimerIndirectionTriple(int positionIndirection);
void writeInodeLibreInFile();
void writeIndirectionLibreInFile();
void writeBlocLibreInFile();


/***********************************
*   Section à faire
*
*/
int creerRepertoire(char* name){
    return 0;
}
int supprimerRepertoire(char* name){
    return 0;
}
int creerFichier(char* name, char* contenue){
    return creerInode(contenue);
}
/**
*   Section à faire fin
*********************************/


int creerInode(char* contenue){
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
            node->bloc[positionInode] = positionInFile;
            blocLibre[positionInFile] = 1;
            positionInFile++;
            positionInode++;
            i++;
        }else if(node->indirectionSimple == -1){
            node->indirectionSimple = gestionIndirectionSimple(contenue, &i);
        }else if(node->indirectionDouble == -1){
            node->indirectionDouble = gestionIndirectionDouble(contenue, &i);
        }else if(node->indirectionTriple == -1){
            node->indirectionTriple = gestionIndirectionTriple(contenue, &i);
        }
    }
    /**set les bloc à -1 pour dire qu'ils sont vide*/
    while(i<8){
        node->bloc[i] = -1;
        i++;
    }
    int retour = writeNodeInInodeFile(node);
    writeInodeLibreInFile();
    writeIndirectionLibreInFile();
    writeBlocLibreInFile();
    free(node);
    return retour;
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
            *position = *position + 1;
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
    free(indirectionSimple);
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
    /**allocation manuel d'une valeur impossible dans le but de ne pas
       laisser le s/e mettre une valeur et corrompre ces données*/
    while(j < 8){
        indirectionDouble->bloc[j] = -1;
        j++;
    }

    /**set le bloque libre d'indirection*/
    int i = 0;
    while(blocLibreIndirection[i] != 0){
        i++;
    }
    if(indirectionDouble->bloc[0] == -1){
        free(indirectionDouble);
        return -1;
    }
    writeBlockEntierInFile(indirectionDouble->bloc, i);
    blocLibreIndirection[i] = 1;
    free(indirectionDouble);
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
        free(indirectionTriple);
        return -1;
    }
    writeBlockEntierInFile(indirectionTriple->bloc, i);
    blocLibreIndirection[i] = 1;
    free(indirectionTriple);
    return i;
}

int supprimerFichier(int positionInode){
    struct iNode node;
    FILE *fp = fopen("inode.dat", "rb+");
    fseek(fp, positionInode*sizeof(struct iNode), SEEK_SET);
    fread(&node, sizeof(struct iNode), 1, fp);
    fclose(fp);
    /**Liberation de l'espace memoire de l'inode*/
    blocLibreInode[positionInode] = 0;
    /**Liberation des bloc dans l'inode*/
    int i = 0;
    while(i<8 && node.bloc[i] != -1){
        blocLibre[node.bloc[i]] = 0;
        i++;
    }
    /**liberation de l'espace memoire des indirection*/
    if(node.indirectionSimple != -1){
        supprimerIndirectionSimple(node.indirectionSimple);
    }
    if(node.indirectionDouble != -1){
        supprimerIndirectionDouble(node.indirectionTriple);
    }
    if(node.indirectionTriple != -1){
        supprimerIndirectionTriple(node.indirectionTriple);
    }
    writeBlocLibreInFile();
    writeIndirectionLibreInFile();
    writeInodeLibreInFile();
    return 0;
}
void supprimerIndirectionSimple(int positionIndirection){
    struct indirection indirectionSimple;
    FILE *fp = fopen("indirection.dat", "rb+");
    fseek(fp, positionIndirection*sizeof(struct indirection), SEEK_SET);
    fread(&indirectionSimple, sizeof(struct indirection), 1, fp);
    fclose(fp);
    int i = 0;
    while(i<8 && indirectionSimple.bloc[i] != -1){
        blocLibre[indirectionSimple.bloc[i]] = 0;
        i++;
    }
}
void supprimerIndirectionDouble(int positionIndirection){
    struct indirection indirectionDouble;
    FILE *fp = fopen("indirection.dat", "rb+");
    fseek(fp, positionIndirection*sizeof(struct indirection), SEEK_SET);
    fread(&indirectionDouble, sizeof(struct indirection), 1, fp);
    fclose(fp);
    int i = 0;
    while(i<8 && indirectionDouble.bloc[i] != -1){
            supprimerIndirectionSimple(indirectionDouble.bloc[i]);
            i++;
    }
}
void supprimerIndirectionTriple(int positionIndirection){
    struct indirection indirectionTriple;
    FILE *fp = fopen("indirection.dat", "rb+");
    fseek(fp, positionIndirection*sizeof(struct indirection), SEEK_SET);
    fread(&indirectionTriple, sizeof(struct indirection), 1, fp);
    fclose(fp);
    int i = 0;
    while(i<8 && indirectionTriple.bloc[i] != -1){
            supprimerIndirectionDouble(indirectionTriple.bloc[i]);
            i++;
    }
}

int lireFichier(char* name){
    return 0;
}

void printInode(int positionInode){
    if(blocLibreInode[positionInode] == 0){
        return;
    }
    struct iNode node;
    int i;
    for(i = 0 ; i < 8 ; i++){
        node.bloc[i] = -1;
    }
    node.indirectionSimple=-1;
    node.indirectionDouble=-1;
    node.indirectionTriple=-1;

    FILE *fp = fopen("inode.dat", "rb+");
    fseek(fp, positionInode*sizeof(struct iNode), SEEK_SET);
    fread(&node, sizeof(struct iNode), 1, fp);
    fclose(fp);

    char affichage;
    FILE *fpChaine = fopen("chaine.dat", "rb+");
    for(i = 0 ; i < 8 ; i++){
        if(node.bloc[i] != -1){
            fseek(fpChaine, node.bloc[i]*sizeof(char), SEEK_SET);
            fread(&affichage, sizeof(char), 1, fpChaine);
            printf("%c", affichage);
        }
    }
    fclose(fpChaine);

    if(node.indirectionSimple != -1){
        printIndirectionSimple(node.indirectionSimple);
    }
    if(node.indirectionDouble != -1){
        printIndirectionDouble(node.indirectionDouble);
    }
    if(node.indirectionTriple!= -1){
        printIndirectionTriple(node.indirectionTriple);
    }
}
void printIndirectionSimple(int indirectionSimplePointeur){
    struct indirection indirectionSimple;
    FILE *fp = fopen("indirection.dat", "rb+");
    fseek(fp, indirectionSimplePointeur*sizeof(struct indirection), SEEK_SET);
    fread(&indirectionSimple, sizeof(struct indirection), 1, fp);
    fclose(fp);
    int i;
    char affichage;
    FILE *fpChaine = fopen("chaine.dat", "rb+");
    for(i = 0 ; i < 8 && indirectionSimple.bloc[i] !=  -1; i++){
        if(indirectionSimple.bloc[i] != -1){
            fseek(fpChaine, indirectionSimple.bloc[i]*sizeof(char), SEEK_SET);
            fread(&affichage, sizeof(char), 1, fpChaine);
            printf("%c", affichage);
        }
    }
    fclose(fpChaine);
}
void printIndirectionDouble(int indirectionDoublePointeur){
    struct indirection indirectionDouble;
    FILE *fp = fopen("indirection.dat", "rb+");
    fseek(fp, indirectionDoublePointeur*sizeof(struct indirection), SEEK_SET);
    fread(&indirectionDouble, sizeof(struct indirection), 1, fp);
    fclose(fp);
    int j;
    for(j = 0 ; j < 8 ; j++){
        if(indirectionDouble.bloc[j] != -1){
            printIndirectionSimple(indirectionDouble.bloc[j]);
        }
    }
}
void printIndirectionTriple(int indirectionTriplePointeur){
    struct indirection indirectionTriple;
    FILE *fp = fopen("indirection.dat", "rb+");
    fseek(fp, indirectionTriplePointeur*sizeof(struct indirection), SEEK_SET);
    fread(&indirectionTriple, sizeof(struct indirection), 1, fp);
    fclose(fp);
    int j;
    for(j = 0 ; j < 8 ; j++){
        if(indirectionTriple.bloc[j] != -1){
            printIndirectionDouble(indirectionTriple.bloc[j]);
        }
    }
}
int writeNodeInInodeFile(iNode *node){
    int position = 0;
    while(blocLibreInode[position] != 0){
        position++;
    }
    blocLibreInode[position] = 1;

    FILE *fp = fopen("inode.dat", "rb+");
    fseek(fp, position*sizeof(struct iNode), SEEK_SET);
    fwrite(node, sizeof(struct iNode), 1, fp);

    fclose(fp);
    return position;
}

void writeCharacterInFile(char charactere, int position){
    FILE *fp = fopen("chaine.dat", "rb+");
    fseek(fp, position, SEEK_SET);
    fwrite(&charactere, sizeof(char), 1, fp);
    fclose(fp);
}

void writeBlockEntierInFile(int *bloc, int position){
    FILE *fp = fopen("indirection.dat", "rb+");
    fseek(fp, position*sizeof(indirection), SEEK_SET);
    fwrite(bloc, sizeof(indirection), 1, fp);
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
        printf("%d", test[j]);
    }
}
void writeInodeLibreInFile(){
    FILE *fp = fopen("inodeLibre.dat", "rb+");
    fseek(fp, 0, SEEK_SET);
    fwrite(blocLibreInode, sizeof(blocLibreInode), 1, fp);
    fclose(fp);
}
void writeIndirectionLibreInFile(){
    FILE *fp = fopen("indirectionLibre.dat", "rb+");
    fseek(fp, 0, SEEK_SET);
    fwrite(blocLibreIndirection, sizeof(blocLibreIndirection), 1, fp);
    fclose(fp);
}

void writeBlocLibreInFile(){
    FILE *fp = fopen("blocLibre.dat", "rb+");
    fseek(fp, 0, SEEK_SET);
    fwrite(blocLibre, sizeof(blocLibre), 1, fp);
    fclose(fp);
}

void loadBlocLibre(){
    FILE *fp = fopen("indirectionLibre.dat", "rb+");
    fseek(fp, 0, SEEK_SET);
    fread(&blocLibreIndirection, sizeof(int) * (sizeof(blocLibreIndirection)/sizeof(blocLibreIndirection[0])), 1, fp);
    fclose(fp);

    FILE *fp1 = fopen("inodeLibre.dat", "rb+");
    fseek(fp1, 0, SEEK_SET);
    fread(&blocLibreInode, sizeof(int) * (sizeof(blocLibreInode)/sizeof(blocLibreInode[0])), 1, fp1);
    fclose(fp1);

    FILE *fp2 = fopen("blocLibre.dat", "rb+");
    fseek(fp2, 0, SEEK_SET);
    fread(&blocLibre, sizeof(int) * (sizeof(blocLibre)/sizeof(blocLibre[0])), 1, fp2);
    fclose(fp2);
}



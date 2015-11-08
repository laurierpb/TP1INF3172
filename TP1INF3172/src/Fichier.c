#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Fichier.h"

void supprimerIndirectionSimple(int positionIndirection);
void supprimerIndirectionDouble(int positionIndirection);
void supprimerIndirectionTriple(int positionIndirection);
int creerInode(char* contenue);
void printInode(int positionInode);
void printIndirectionSimple(int indirectionSimplePointeur);
void printIndirectionDouble(int indirectionDoublePointeur);
void printIndirectionTriple(int indirectionTriplePointeur);
int writeNodeInInodeFile(iNode *node);
void writeCharacterInFile(char charactere, int position);
void writeBlockEntierInFile(int *bloc, int position);
void afficheEntierInFileFromPosition(int position);
void writeInodeLibreInFile();
void writeIndirectionLibreInFile();
void writeBlocLibreInFile();
void writeFichierLibreInFile();
int gestionIndirectionSimple(char* contenue, int *position);
int gestionIndirectionDouble(char* contenue, int *position);
int gestionIndirectionTriple(char* contenue, int *position);
void initialiseElement(element *repertoire);
void writeRepertoireLibreInFile();
int writeRepertoireInFile(element *element);
void supprimerEnfant(int adresseParent);

int blocLibre[64000] = {0};
int blocLibreIndirection[1000] = {0};
int blocLibreInode[1000] = {0};
int blocLibreRepertoire[1000] = {0};

/**return -1 si le fichier existe deja*/
int creerFichier(char* name, char* contenue){
    /**Si le fichier existe deja on retourne -1*/
    if(getAdressRepertoire(name) != -1){
        return -1;
    }

    /**Creation du nom du fichier parent*/
    char nomFichierParent[40] = {'\0'};
    /**calcul de la longeur du char name*/
    int i = 0;
    while(name[i] != '\0'){
        i++;
    }
    int longeurName = i;
    while(name[longeurName] != '/' && i > 0){
        longeurName--;
    }
    for(i = 0 ; i < longeurName ; i++){
        nomFichierParent[i] = name[i];
    }


    struct element *elementInsertion = malloc(sizeof(element));
    initialiseElement(elementInsertion);

    if(nomFichierParent[0] == '\0'){
        /**Cele veut dire que le fichier est un root*/
        elementInsertion->parent = -1;
    }else{
        elementInsertion->parent = getAdressRepertoire(nomFichierParent);
        if(elementInsertion->parent == -1){
            printf("Le repertoire parent n'existe pas : %s\n", nomFichierParent);
            free(elementInsertion);
            return -1;
        }
    }
    /**creation du nom du fichier*/
    for(i = 0 ; name[i] != '\0' ; i++){
        elementInsertion->chemin[i] = name[i];
    }

    elementInsertion->adresseInode = creerInode(contenue);
    int retour = writeRepertoireInFile(elementInsertion);
    writeRepertoireLibreInFile();
    return retour;
}

int lireFichier(char* name){
    int positionDansFichier = getAdressRepertoire(name);
    if(positionDansFichier == -1){
        printf("Le fichier n'existe pas\n");
        return 0;
    }
    struct element elementAfficher;
    FILE *fp = fopen("repertoire.dat","rb+");
    fseek(fp, positionDansFichier*sizeof(struct element), SEEK_SET);
    fread(&elementAfficher, sizeof(struct element), 1, fp);
    fclose(fp);
    if(elementAfficher.adresseInode == -1){
        return -1;
    }
    printInode(elementAfficher.adresseInode);
    return 0;
}

int creerRepertoire(char* name){
    /**Si le fichier existe deja on retourne -1*/
    if(getAdressRepertoire(name) != -1){
        return -1;
    }

    /**Creation du nom du fichier parent*/
    char nomFichierParent[40] = {'\0'};
    /**Calcul de la longeur du char name*/
    int i = 0;
    while(name[i] != '\0'){
        i++;
    }

    /**Trouve la valeur du parent*/
    int longeurName = i;
    while(name[longeurName] != '/' && i > 0){
        longeurName--;
    }
    for(i = 0 ; i < longeurName ; i++){
        nomFichierParent[i] = name[i];
    }

    struct element *elementInsertion = malloc(sizeof(element));
    initialiseElement(elementInsertion);

    if(nomFichierParent[0] == '\0'){
        /**Cele veut dire que le fichier est un root*/
        elementInsertion->parent = -1;
    }else{
        elementInsertion->parent = getAdressRepertoire(nomFichierParent);
        if(elementInsertion->parent == -1){
            /**Cele veut dire que le parent n'existe pas et donc c'est une erreur*/
            printf("Le repertoire parent n'existe pas : %s\n", nomFichierParent);
            free(elementInsertion);
            return -1;
        }
    }
    /**creation du nom du fichier*/
    for(i = 0 ; name[i] != '\0' ; i++){
        elementInsertion->chemin[i] = name[i];
    }

    elementInsertion->adresseInode = -1;
    int retour = writeRepertoireInFile(elementInsertion);
    writeRepertoireLibreInFile();
    return retour;
}
int supprimerRepertoire(char* name){
    int positionDansFichier = getAdressRepertoire(name);
    if(positionDansFichier == -1){
        printf("Le repertoire/fichier n'existe pas");
        return 0;
    }
    struct element elementSupprimer;

    FILE *fp = fopen("repertoire.dat","rb+");
    fseek(fp, positionDansFichier*sizeof(struct element), SEEK_SET);
    fread(&elementSupprimer, sizeof(struct element), 1, fp);
    fclose(fp);

    blocLibreRepertoire[positionDansFichier] = 0;
    if(elementSupprimer.adresseInode != -1){
        supprimerFichier(elementSupprimer.adresseInode);
    }
    supprimerEnfant(positionDansFichier);
    return 0;
}

void supprimerEnfant(int adresseParent){
    int i;
    struct element elementSupprimer;
    FILE *fp = fopen("repertoire.dat","rb+");
    for(i = 0 ; i < sizeof(blocLibreRepertoire)/sizeof(int) ; i++){
        while(blocLibreRepertoire[i] != 1 && i < sizeof(blocLibreRepertoire)/sizeof(int)){
            i++;
        }
        fseek(fp, i*sizeof(struct element), SEEK_SET);
        fread(&elementSupprimer, sizeof(struct element), 1, fp);
        if(elementSupprimer.parent == adresseParent){
            blocLibreRepertoire[i] = 0;
            supprimerFichier(elementSupprimer.adresseInode);
            supprimerEnfant(i);
        }
    }
    fclose(fp);
}

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

void printInode(int positionInode){
    if(blocLibreInode[positionInode] == 0){
        return;
    }
    struct iNode node;
    int i;
    for(i = 0 ; i < 8 ; i++){
        node.bloc[i] = -1;
    }
    node.indirectionSimple = -1;
    node.indirectionDouble = -1;
    node.indirectionTriple = -1;

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
    printf("\n");
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
int writeRepertoireInFile(element *element){
    int position = 0;
    while(blocLibreRepertoire[position] != 0){
        position++;
    }
    blocLibreRepertoire[position] = 1;

    FILE *fp = fopen("repertoire.dat", "rb+");
    fseek(fp, position*sizeof(struct element), SEEK_SET);
    fwrite(element, sizeof(struct element), 1, fp);

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
void writeRepertoireLibreInFile(){
    FILE *fp = fopen("repertoireLibre.dat", "rb+");
    fseek(fp, 0, SEEK_SET);
    fwrite(blocLibreRepertoire, sizeof(blocLibreRepertoire), 1, fp);
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

    FILE *fp3 = fopen("repertoireLibre.dat", "rb+");
    fseek(fp3, 0, SEEK_SET);
    fread(&blocLibreRepertoire, sizeof(int) * (sizeof(blocLibreRepertoire)/sizeof(blocLibreRepertoire[0])), 1, fp3);
    fclose(fp3);
}


/**
* return -1 si le fichier n'existe pas
*/
int getAdressRepertoire(char* nomFichier){
    struct element elementLue;

    FILE *fp = fopen("repertoire.dat", "rb+");

    int i = 0;
    while(1){
        while(blocLibreRepertoire[i] != 1){
            i++;
            /**Regarde si le fichier à dépacer la longueur de l'allocation de fichier*/
            if(i >= sizeof(blocLibreRepertoire)/sizeof(blocLibreRepertoire[0])){
                return -1;
            }
        }
        fseek(fp, i*sizeof(struct element), SEEK_SET);
        fread(&elementLue, sizeof(struct element), 1, fp);
        int j = 0;
        while(nomFichier[j] == elementLue.chemin[j]
              && nomFichier[j] != '\0'
              && elementLue.chemin[j] != '\0'){
            j++;
        }
        if(nomFichier[j] == '\0' && elementLue.chemin[j] == '\0'){
            fclose(fp);
            return i;
        }

        i++;
    }
    fclose(fp);
    return -1;
}

void initialiseElement(element *Repertoire){
    int i;
    for(i = 0 ; i < 40 ; i++){
        Repertoire->chemin[i] = '\0';
    }
    Repertoire -> parent = -1;
    Repertoire -> adresseInode = -1;
}

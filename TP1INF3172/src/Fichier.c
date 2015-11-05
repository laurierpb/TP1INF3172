#include "Fichier.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void printInode(iNode* node);

int creerRepertoire(char* name){
    return 0;
}

int supprimerRepertoire(char* name){
    return 0;
}

int creerFichier(char* name, char* contenu){

    return 0;
}

int supprimerFichier(char* name){
    return 0;
}

int lireFichier(char* name){
    return 0;
}


/**SUPER TEMP*//**SUPER TEMP*//**SUPER TEMP*//**SUPER TEMP*/
iNode* populateInode(){
    iNode* node = (iNode*)malloc(sizeof(struct iNode));

    int i;
    for(i = 0 ; i < 8 ; i++){
        node->bloc[i] = i;
    }

    node->indSimp = (indirectionSimple*)malloc(sizeof(struct indirectionSimple));
    for(i = 0 ; i < 8 ; i++){
        node->indSimp->bloc[i] = i;
    }

    node->indDoub = (IndirectionDouble*)malloc(sizeof(struct IndirectionDouble));
    for(i = 0 ; i < 8 ; i ++){
        int j;
        node->indDoub->listIndSimp[i] = (indirectionSimple*)malloc(sizeof(struct indirectionSimple));
        for(j = 0 ; j < 8 ; j++){
            node->indDoub->listIndSimp[i]->bloc[j] = j;
        }
    }

    printInode(node);
    return node;
}
void printInode(iNode* node){
    FILE *fp = fopen("file.bin","r+");
    if(fp == NULL){
        printf("ah shit");
    }



    int i;
    for(i = 0 ; i < 8 ; i++){
        printf("%d", node->bloc[i]);
    }

    printf("\nIndirectionSimple : ");
    for(i = 0 ; i < 8 ; i++){
        printf("%d", node->indSimp->bloc[i]);
    }

    printf("\n\nIndirectionDouble : ");
    for(i = 0 ; i < 8 ; i ++){
        printf("\nIndirectionSimple : ");
        int j;
        for(j = 0 ; j < 8 ; j++){
            printf("%d", node->indDoub->listIndSimp[i]->bloc[j]);
        }
    }
}

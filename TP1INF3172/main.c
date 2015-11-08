#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ErrorHandler.h"
#include "Fichier.h"

int verifierDemarage();
int creerFichiersFichierDemarage();
int demarerInviteCommande();
void analyserLecture(char* lecture);

char* pathChaine = "chaine.dat";
char* pathInode = "inode.dat";
char* pathIndirection = "indirection.dat";
char* pathRepertoire= "repertoire.dat";

char* pathBlocLibre = "blocLibre.dat";
char* pathInodeLibre = "inodeLibre.dat";
char* pathIndirectionLibre = "indirectionLibre.dat";
char* pathRepertoireLibre = "repertoireLibre.dat";

int main(){
    char * d = "/sfdsf/dsfsdfds/wwwee/wwwqqqff ff";
    verifierFicLongueur(d);
    return;
    verifierDemarage();
    loadBlocLibre();
    demarerInviteCommande();

    char lecture[1000];
    int continuer = 1;
    while(continuer){
        fgets(lecture, sizeof(lecture), stdin);
        char* ptr = strchr(lecture, '\n');
        if (ptr != NULL) *ptr = '\0';

        if(!strcmp(lecture, "fin")){
            continuer = 0;
        }else{
            analyserLecture(lecture);
        }
    }
    return 0;
}

/**
* Verifie que tous les elements pour le demarage sont present
*/
int verifierDemarage(){
    /**à optimiser vraiment pas beau code*/
    if(fopen(pathChaine, "rb+") == NULL){
        creerFichiersFichierDemarage();
    }else if(fopen(pathIndirection, "rb+") == NULL){
        creerFichiersFichierDemarage();
    }else if(fopen(pathInode, "rb+") == NULL){
        creerFichiersFichierDemarage();
    }else if(fopen(pathInodeLibre, "rb+") == NULL){
        creerFichiersFichierDemarage();
    }else if(fopen(pathIndirectionLibre, "rb+") == NULL){
        creerFichiersFichierDemarage();
    }
    return 0;
}

int creerFichiersFichierDemarage(){
    FILE *fp1 = fopen(pathChaine, "wb+");
    fseek(fp1, 512*1024, SEEK_SET);
    fputc('\n', fp1);
    fclose(fp1);

    /**Ces fichiers n'ont pas besoin de setter la grosseur*/
    FILE *fp2 = fopen(pathIndirection, "wb+");
    fclose(fp2);
    FILE *fp3 = fopen(pathInode, "wb+");
    fclose(fp3);
    FILE *fp4 = fopen(pathBlocLibre, "wb+");
    fclose(fp4);
    FILE *fp5 = fopen(pathInodeLibre, "wb+");
    fclose(fp5);
    FILE *fp6 = fopen(pathIndirectionLibre, "wb+");
    fclose(fp6);
    FILE *fp7 = fopen(pathRepertoireLibre, "wb+");
    fclose(fp7);
    FILE *fp8 = fopen(pathRepertoire, "wb+");
    fclose(fp8);
    return 0;
}

//appelle les différentes fonctions du programme
void analyserLecture(char* lecture){
    if(!strncmp("creation_repertoire ", lecture, 20)){

        printf("\nDebut, creation_repertoire\n");
        lecture = lecture + 20;
        printf("\n%s\n", lecture);
        if (verifierCheminAbsolu(lecture)
            && verifierExist(lecture)
            && verifierRepParent(lecture)){
            creerRepertoire(lecture);
        }
        printf("\nFin, creation_repertoire\n");

    }else if(!strncmp("suppression_repertoire ", lecture, 23)){

        printf("\nDebut, suppression_repertoire\n");
        lecture = lecture + 23;
        printf("\n%s\n", lecture);//
        if (verifierExist(lecture)){
            supprimerRepertoire(lecture);
        }
        printf("\nFin, suppression_repertoire\n");

    }else if(!strncmp("creation_fichier ", lecture, 17)){

        printf("\nDebut, creation_fichier\n");
        lecture = lecture + 17;
        printf("\n%s\n", lecture);//
        if (verifierFicLongueur(lecture)
            && verifierExist(lecture)
            && verifierRepParent(lecture)
            && verifierCheminAbsolu(lecture)){
            creerFichier(lecture, lecture);
        }
        printf("\nFin, creation_fichier\n");

    }else if(!strncmp("suppression_fichier ", lecture, 20)){

        printf("\nDebut, suppression_fichier\n");
        lecture = lecture + 20;
        printf("\n%s\n", lecture);//
        if (verifierExist(lecture)){
            supprimerRepertoire(lecture);
        }
        printf("\nFin, suppression_fichier\n");

    }else if(!strncmp("lire_fichier ", lecture, 13)){

        printf("\nDebut, lire_fichier\n");
        lecture = lecture + 13;
        printf("\n%s\n", lecture);//
        if (verifierExist(lecture)){
            lireFichier(lecture);
        }
        printf("\nFin, lire_fichier\n");

    }else{
        printf("Commande invalide\n");
    }

    return;
}

int demarerInviteCommande(){
    printf("Veuillez entrer vos commandes\n");
    printf("Ecrire 'fin' pour quitter\n");
    return 0;
}


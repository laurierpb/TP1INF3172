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
    }else if(fopen(pathRepertoire, "rb+") == NULL){
        creerFichiersFichierDemarage();
    }else if(fopen(pathRepertoireLibre, "rb+") == NULL){
        creerFichiersFichierDemarage();
    }else if(fopen(pathBlocLibre, "rb+") == NULL){
        creerFichiersFichierDemarage();
    }
    return 0;
}

int creerFichiersFichierDemarage(){
    FILE *fp1 = fopen(pathChaine, "wb+");
    fseek(fp1, 500*1024, SEEK_SET);
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
        lecture = lecture + 20;
        if (verifierCheminAbsolu(lecture)
            && verifierExisteDeja(lecture)){
            creerRepertoire(lecture);
        }

    }else if(!strncmp("suppression_repertoire ", lecture, 23)){
        lecture = lecture + 23;
        supprimerRepertoire(lecture);

    }else if(!strncmp("creation_fichier ", lecture, 17)){
        lecture = lecture + 17;
        if (verifierFicLongueur(lecture)){
            char * contenu = strchr(lecture, ' ');
            char nom[1000] = {'\0'};
            int lenContenu = strlen(contenu);
            int len = strlen(lecture);
            int pos = len - lenContenu;
            strncpy(nom, lecture, pos);
            contenu++;
            if (verifierExisteDeja(nom) && verifierCheminAbsolu(nom)){
                creerFichier(nom, contenu);
            }
        }

    }else if(!strncmp("suppression_fichier ", lecture, 20)){
        lecture = lecture + 20;
        supprimerRepertoire(lecture);

    }else if(!strncmp("lire_fichier ", lecture, 13)){
        lecture = lecture + 13;
        lireFichier(lecture);

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


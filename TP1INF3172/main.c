#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int verifierDemarage();
int creerFichiersFichierDemarage();
int demarerInviteCommande();
void analyserLecture(char* lecture);

char* pathChaine = "chaine.dat";
char* pathInode = "inode.dat";
char* pathIndirection = "indirection.dat";
char* pathBlocLibre = "blocLibre.dat";

int main(){
    verifierDemarage();
    demarerInviteCommande();
    creerFichier("ThisIsATest", "67mbcbvndfn ,sdfjgj lksjfglk jdkfgjxdfj gkdfjgljsdlgjslkdjf ");

    char lecture[64000];
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
    return 0;
}

//appelle les différentes fonctions du programme
void analyserLecture(char* lecture){
    if(!strcmp(lecture, "1")){
        printf("work");
    }else if(strstr(lecture, " ") == NULL){
        printf("Commande invalide\n");
    }

    return;
}

int demarerInviteCommande(){
    printf("Veuillez entrer vos commandes\n");
    printf("Ecrire 'fin' pour quitter\n");
    return 0;
}

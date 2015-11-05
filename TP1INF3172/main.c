#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int verifierDemarage();
int creerFichiersDemarage();
int demarerInviteCommande();
void analyserLecture(char* lecture);


char* path = "file.bin";

int main(){
    if(!verifierDemarage()){
        creerFichiersDemarage();
    }
    demarerInviteCommande();

    populateInode();

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

    return 0;
}

int creerFichiersDemarage(){
    FILE *fp = fopen(path, "w+");
    fseek(fp, 512*1024, SEEK_SET);
    fputc('a', fp);
    fclose(fp);
    return 0;
}

//appelle les différentes fonctions du programme
void analyserLecture(char* lecture){
    if(!strcmp(lecture, "1")){
        printf("work");
    }

    return;
}

int demarerInviteCommande(){
    printf("Veuillez entrer vos commandes\n");
    printf("Ecrire 'fin' pour quitter\n");
    return 0;
}

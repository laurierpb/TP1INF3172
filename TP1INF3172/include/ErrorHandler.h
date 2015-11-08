#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

/**
* le répertoire existe déjà;
* un des répertoires parents n'existe pas;
* le chemin absolu du répertoire est trop long.
* un fichier ne peut pas être vide;
* le fichier existe déjà;
* le répertoire n'existe pas;
* le fichier est trop volumineux;
* le chemin absolu du fichier est trop long.
* le fichier n'existe pas
*/


int verifierCheminAbsolu(char *);
int verifierExist(char *);
int verifierRepParent(char *);
int verifierFicLongueur(char *);
int verifierExisteDeja(char *chemin);


#endif // ERRORHANDLER_H

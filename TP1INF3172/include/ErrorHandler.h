#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

/**
* le r�pertoire existe d�j�;
* un des r�pertoires parents n'existe pas;
* le chemin absolu du r�pertoire est trop long.
* un fichier ne peut pas �tre vide;
* le fichier existe d�j�;
* le r�pertoire n'existe pas;
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

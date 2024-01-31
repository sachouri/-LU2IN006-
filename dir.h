#ifndef dir_H
#define dir_H
#include "liste.h"

/* retourne une liste contenant le noms des fichiers et repertoires qui se trouvent dans une adresse */
List* listdir(char* root_dir);

/* indique si le fichier existe dans le repertoire courant */
int file_exists(char *file);

/* copie le contenu d'un fichier vers un autre */
void cp(char *to, char *from);

/* retourne le chemin d'un fichier a partir de son hash */
char* hashToPath(char* hash);

/* enregistre un instantane du fichier donne en entree */
void blobFile(char* file);

#endif
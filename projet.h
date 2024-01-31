#ifndef projet_H
#define projet_H

/* calcule le hash du contenu du premier fichier et l’ecrit dans le deuxieme fichier */
int hashFile(char* source, char* dest);

/* retourne chaine de caracteres contenant le hash du fichier donne en parametre */
char* sha256file(char* file);

#endif
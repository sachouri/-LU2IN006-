#ifndef branch_H
#define branch_H
#include "liste.h"


/* cree le fichier cache .current_branch contenant la chaine de caracteres "master" */
void initBranch();

/* verifie l'existance d'une branche */
int branchExists(char* branch);

/* cree une reference branch, qui pointe vers le meme commit que la reference HEAD */
void createBranch(char* branch);

/* lit le fichier cache .current_branch pour retourner le nom de la branche courante */
char* getCurrentBranch();

/* parcourt la branche branch, et affiche le hash et le message descriptif de chacun de ses commits */
void printBranch(char* branch);

/* contruit et retourne une liste chainee contenant le hash de tous les commits de la branche */
List* branchList(char* branch);

/* renvoie la liste des hash des commits de toutes les branches sans repetition */
List* getAllCommits();


#endif
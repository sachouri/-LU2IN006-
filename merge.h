#ifndef merge_H
#define merge_H
#include "liste.h"
#include "work.h"

/* retourne une liste composee des noms de fichiers/repertoires qui sont en conflit 
(utilisée pour les test dans main.c) */
List* conflictList(WorkTree* wt1, WorkTree* wt2);

/* cree une liste composee des noms de fichiers/repertoires qui sont en conflit
et retourne un nouveau WorkTree compose des fichiers et/ou repertoires qui ne sont pas en conflit */
WorkTree *mergeWorkTrees(WorkTree *wt1, WorkTree *wt2, List **conflicts);

/* retourne le worktree associé a la reference d'une branche 
(utilisée pour les test dans main.c) */
WorkTree* btwt(char* branch);

/* fusionne la branche courante avec la branche passee en parametre si aucun conflit n'existe */
List *merge(char *remote_branch, char *message);

/* cree et ajoute un commit de suppression sur la branche branch, 
correspondant à la suppression des elements de la liste conflicts */
void createDeletionCommit(char *branch, List *conflicts, char *message);

/* donne à l'utilisateur les differents choix poissibles de fusion en cas de conflits 
(utilisée dans myGit.c) */
void conflitsChoice(char *branch1, char *branch2, List *conflicts, List **conflicts1, List **conflicts2);

#endif
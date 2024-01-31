#ifndef checkout_H
#define checkout_H
#include "liste.h"

/* restaure le worktree associe à un commit dont le hash est donne en parametre */
void restoreCommit(char* hash_commit);

/* permet de changer de branche */
void myGitCheckoutBranch(char* branch);

/* retourne une nouvelle liste contenant uniquement les elements de L qui commencent par pattern */
List* filterList(List* L, char* pattern);

/* permet à l'utilisateur de retourner sur n'importe quelle version de son projet */
void myGitCheckoutCommit(char* pattern);

#endif
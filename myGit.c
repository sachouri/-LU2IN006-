#include "branch.h"
#include "checkout.h"
#include "commit.h"
#include "dir.h"
#include "liste.h"
#include "merge.h"
#include "projet.h"
#include "ref.h"
#include "work.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  /*
      ./myGit init
      Initialise le repertoire de references et la branche courante
  */
  if (strcmp(argv[1], "init") == 0) {
    initRefs();
    initBranch();
  }

  /*
      ./myGit list-refs
      Affiche toutes les references existantes
  */
  if (strcmp(argv[1], "list-refs") == 0) {
    printf("REFS : \n");
    if (file_exists(".refs") != 0) {
      List *L = listdir(".refs"); //à Libérer
      List l = *L;
      while (l != NULL) {
        if (l->data[0] == '.') {
          l = l->next;
          continue;
        }
        char *ref = getRef(l->data);
        printf("-%s\t%s\n", l->data, ref);
        free(ref);
        l = l->next;
      }
      freeList(L);
    }
  }

  /*
      ./myGit create-ref <name> <hash>
      Cree la reference <name> qui pointe vers le commit correspondant au hash
     donne
  */
  if (strcmp(argv[1], "create-ref") == 0) {
    createUpdateRef(argv[2], argv[3]);
  }

  /*
      ./myGit delete-ref <name>
      Supprime la reference name
  */
  if (strcmp(argv[1], "delete-ref") == 0) {
    deleteRef(argv[2]);
  }

  /*
      ./myGit add <elem> [<elem2> <elem3> ...]
      Ajoute un ou plusieurs fichiers/repertoires a la zone de preparation (pour
     faire partie du prochain commit)
  */
  if (strcmp(argv[1], "add") == 0) {
    for (int i = 2; i < argc; i++) {
      myGitAdd(argv[i]);
    }
  }

  /*
      ./myGit list-add
      Affiche le contenu de la zone de preparation.
  */
  if (strcmp(argv[1], "list-add") == 0) {
    printf("Zone de preparation : \n");
    if (file_exists(".add")) {
      WorkTree *wt = ftwt(".add");
      char *s = wtts(wt);
      printf("%s\n", s);
      free(s);
      freeWorkTree(wt);
    } else {
      printf("Zone de preparation vide\n");
    }
  }

  /*
      ./myGit clear-add
      Vide la zone de preparation
  */
  if (strcmp(argv[1], "clear-add") == 0) {
    system("rm .add");
  }

  /*
      ./myGit commit <branch_name> [-m <message>]
      Effectue un commit sur une branche, avec ou sans message descriptif
  */
  if (strcmp(argv[1], "commit") == 0) {
    if (argc > 3) {
      if (strcmp(argv[3], "-m") == 0) {
        myGitCommit(argv[2], argv[4]);
      }
    } else {
      myGitCommit(argv[2], NULL);
    }
  }

  /*
      ./myGit get-current-branch
      Affiche le nom de la branche courante
  */
  if (strcmp(argv[1], "get-current-branch") == 0) {
    char* branch = getCurrentBranch();
    printf("%s\n", branch);
    free(branch);
  }

  /*
      ./myGit branch <branch-name>
      Cree une branche <branch-name>.
      Si la branche existe deja, la commande affiche un message d’erreur
  */
  if (strcmp(argv[1], "branch") == 0) {
    if (!branchExists(argv[2])) {
      createBranch(argv[2]);
    } else {
      printf("Erreur : la branche existe déjà\n");
    }
  }

  /*
      ./myGit branch-print <branch-name>
      Affiche le hash de tous les commits de la branche, accompagnes de leur
     message descriptif eventuel. Si la branche n’existe pas, la commande
     affiche un message d’erreur
  */
  if (strcmp(argv[1], "branch-print") == 0) {
    if (!branchExists(argv[2])) {
      printf("Erreur : la branche n'existe pas\n");
    } else {
      printBranch(argv[2]);
    }
  }

  /*
      ./myGit checkout-branch <branch-name>
      Realise un deplacement sur la branche <branch-name>.
      Si cette branche n’existe pas, la commande affiche un message d’erreur
  */
  if (strcmp(argv[1], "checkout-branch") == 0) {
    if (!branchExists(argv[2])) {
      printf("Erreur : la branche n'existe pas\n");
    } else {
      myGitCheckoutBranch(argv[2]);
    }
  }

  /*
      ./myGit checkout-commit <pattern>
      Realise un deplacement sur le commit qui commence par <pattern>.
      La commande affiche des messages d’erreur quand <pattern> ne correspond
      pas à exactement un commit
  */
  if (strcmp(argv[1], "checkout-commit") == 0) {
    myGitCheckoutCommit(argv[2]);
  }


  /*
      ./myGit merge <branch>
      S’il n’y a pas de collision entre la branche courante et la branche <branch>,
      la commande realise le merge.
      S’il y a des collisions, plusieurs options sont possibles : 
        1 : Garder les fichiers de la branche courante
        2 : Garder les fichiers de la branche <branch>
        3 : Choisir manuellement, conflit par conflit, la branche sur laquelle 
        on souhaite garder le fichier/repertoire qui est en conflit
  */
  if (strcmp(argv[1], "merge") == 0) {

    printf("on commence la fusion\n");
    if (argc < 4){
      printf("veuillez preciser un message\n");
      return 0;
    }
    char* current_branch = getCurrentBranch();
    printf("passed 1\n");
    char* remote_branch = strdup(argv[2]);
    printf("passed 2\n");
    char* message = strdup(argv[3]);
    printf("passed 3\n");
    List *conflicts = merge(remote_branch, message);

    printf("on evalue la liste des conflits\n");

    if (conflicts==NULL) {
      /* Cas sans conflits */
      printf("merge : La fusion s'est bien passée (Pas de conflits)\n");
    } 
    
    else {
      /* Cas avec conflits */
      printf("1 : créer un commit de suppression pour la branche %s\n",remote_branch);
      printf("2 : créer un commit de suppression pour la branche courant %s\n",current_branch);
      printf("3 : choisir manuellement dans quelle branche garder les commits\n");
      
      List *conflicts_branch = initList();
      List *conflicts_current = initList();

      int choix;
      scanf("%d", &choix);

      switch(choix) {

        case 1:
          printf("choix 1\n");
          createDeletionCommit(remote_branch, conflicts, message);
          merge(remote_branch, message);
          break;

        case 2:
          printf("choix 2\n");
          createDeletionCommit(current_branch, conflicts,message);
          merge(remote_branch, message);
          break;

        case 3:
          printf("choix 3\n");
          conflitsChoice(current_branch,remote_branch, conflicts, &conflicts_current,&conflicts_branch);
          createDeletionCommit(remote_branch, conflicts_branch, message);
          createDeletionCommit(current_branch, conflicts_current, message);
          merge(remote_branch, message);
          break;

        default:
          printf("myGit : Erreur formatage valeur\n");
      }
      freeList(conflicts_branch);
      freeList(conflicts_current);
    }
    free(remote_branch);
    free(current_branch);
    free(message);
    freeList(conflicts);

  }
  return 0;
}

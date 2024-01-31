#include "branch.h"
#include "checkout.h"
#include "commit.h"
#include "dir.h"
#include "liste.h"
#include "projet.h"
#include "ref.h"
#include "work.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void restoreCommit(char *hash_commit) {

  if (hash_commit==NULL || strcmp(hash_commit,"")==0){
    printf("La reference est vide\n");
    return ;
  }

  char *commit_path = hashToPathCommit(hash_commit);
  Commit *c = ftc(commit_path);

  char *hash = commitGet(c, "tree");
  char *worktree_path = hashToPathWorkTree(hash);
  WorkTree *wt = ftwt(worktree_path);

  restoreWorkTree(wt, ".");

  free(commit_path);
  free(worktree_path);
  freeWorkTree(wt);
  freeCommit(c);
}

void myGitCheckoutBranch(char *branch) {

  FILE *f = fopen(".current_branch", "w");
  if (f == NULL) {
    printf("myGitCheckoutBranch : Erreur ouverture fichier\n");
    return ;
  }
  fprintf(f, "%s", branch);
  fclose(f);

  char *hash_commit = getRef(branch);
  createUpdateRef("HEAD", hash_commit);
  printf("myGitCheckoutBranch : we updated HEAD lets restore %s\n",hash_commit);
  restoreCommit(hash_commit);
  printf("myGitCheckoutBranch : we restored the commit\n");
  free(hash_commit);
}

List *filterList(List *L, char *pattern) {

  int n = strlen(pattern);
  if (L==NULL){
    return NULL;
  }
  List *filtered_list = initList();
  List tmp_cell = *L;

  while (tmp_cell != NULL) {

    if (strncmp(tmp_cell->data, pattern, n) == 0) {

      Cell *new_cell = buildCell(tmp_cell->data);
      insertLast(filtered_list, new_cell);
    }
    tmp_cell = tmp_cell->next;
  }

  return filtered_list;
}

void myGitCheckoutCommit(char *pattern) {

  List *L = getAllCommits();
  List *filtered_list = filterList(L, pattern);


  if (listSize(filtered_list) == 1) {

    char *commit_hash = (listGet(filtered_list, 0))->data;
    createUpdateRef("HEAD", commit_hash);
    restoreCommit(commit_hash);

  } 
  
  else {
    if (listSize(filtered_list) == 0) {
      printf("myGitCheckoutCommit : pas de commit correspodant\n");

    } 
    else {
      List l = *filtered_list;
      while (l != NULL) {
        printf("->%s\n", l->data);
        l = l->next;
      }

    }
  }
  freeList(L);
  freeList(filtered_list);
}
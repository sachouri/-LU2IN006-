#include "branch.h"
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

void initBranch() {
  FILE *f = fopen(".current_branch", "w");
  if (f == NULL) {
    printf("initBranch : Erreur d'ouverture de fichier\n");
    return;
  }
  fputs("master", f);
  fclose(f);
}

int branchExists(char *branch) {
  List *list_refs = listdir(".refs");
  Cell *result = searchList(list_refs, branch);
  if (result == NULL) {
    freeList(list_refs);
    return 0;
  } else {
    freeList(list_refs);
    return 1;
  }
}

void createBranch(char *branch) {
  char *hash_HEAD = getRef("HEAD");
  createUpdateRef(branch, hash_HEAD);
  free(hash_HEAD);
}

char *getCurrentBranch() {
  FILE *f = fopen(".current_branch", "r");
  if (f == NULL) {
    printf("getCurrentBranch : Erreur d'ouverture de fichier\n");
    return NULL;
  }
  char *branch = (char *)malloc(sizeof(char) * 100);
  fscanf(f, "%s", branch);
  fclose(f);
  return branch;
}

void printBranch(char *branch) {
  
  char *commit_hash = getRef(branch);
  printf("branchList : on a eu la ref\n");
  if (commit_hash==NULL || commit_hash[0]=='\0'){
    printf("la reference de la branche est vide\n");
    free(commit_hash);
    return;
  }

  char *commit_path = hashToPathCommit(commit_hash);
  printf("branchList : on a eu le path\n");
  Commit *c = ftc(commit_path);

  while (c != NULL) {

    /* affichage */
    char *message = commitGet(c, "message");

    if (message != NULL) {
      printf("%s -> %s \n", commit_hash, message);
    } else {
      printf("%s \n", commit_hash);
    }

    /* parcours */
    char *predecessor = commitGet(c, "predecessor");

    if (predecessor != NULL) {

      strcpy(commit_hash, predecessor);
      char* path_commit = hashToPathCommit(predecessor);
      strcpy(commit_path,path_commit);
      free(path_commit);
      freeCommit(c);
      c = ftc(commit_path);
    } 
    else {

      freeCommit(c);
      c = NULL;
    }
  }

  free(commit_hash);
  free(commit_path);
}


List* branchList(char* branch){

    List* L = initList();
    char* commit_hash = getRef(branch);
    char* commit_path = hashToPathCommit(commit_hash);
    Commit* c = ftc(commit_path);

    while (c != NULL){

        /* creation d'une cell de list */
        Cell* commit_cell = buildCell(commit_hash);
        insertFirst(L, commit_cell);
        
        /* parcours */
        char* predecessor = commitGet(c, "predecessor");

        if (predecessor!=NULL){
            strcpy(commit_hash,predecessor);
            strcpy(commit_path,hashToPathCommit(predecessor));
            freeCommit(c);
            c = ftc(commit_path); 
        }

        else{
            freeCommit(c);
            c = NULL; 
        }
    }
    free(commit_hash);
    free(commit_path);
    return L;
}


List* getAllCommits() {

  List* content = listdir(".refs"); 

  if (content == NULL || *content==NULL){
    return NULL ;
  }

  List* L = initList();
  List tmp = *content;

  while (tmp != NULL) {

    if (tmp->data[0] == '.') {
      tmp = tmp->next;
      continue;
    }

    List *list = branchList(tmp->data);

    if (list==NULL){
      tmp = tmp->next;
      continue ;
    }

    List l = *list;

    while (l != NULL) {

      Cell* search_cell = searchList(L, l->data);
      if (search_cell == NULL) {
        insertFirst(L, buildCell(l->data));
      }
      l = l->next;
    }

    tmp = tmp->next;
    freeList(list);

  }

  freeList(content);

  return L;
}
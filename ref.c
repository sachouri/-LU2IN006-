#include "commit.h"
#include "work.h"
#include "projet.h"
#include "liste.h"
#include "dir.h"
#include "ref.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void initRefs(){
    if (!file_exists(".refs")){
        system("mkdir .refs");
    }
    system("touch .refs/master");
    system("touch .refs/HEAD");
}

void createUpdateRef(char *ref_name, char *hash) {
  char ref[200];
  sprintf(ref, ".refs/%s", ref_name);
  if (file_exists(ref) == 0) { /* Si la référence n'existe pas */
    /* Création de la référence */
    char command[500];
    sprintf(command, "touch %s", ref);
    system(command);
  }
  FILE* f = fopen(ref,"w");
  if(f == NULL){
    printf("createUpdateRef : Erreur Ouverture\n");
    exit(EXIT_FAILURE);
  }
  /* Mise à jour du hash */
  fprintf(f,"%s",hash);
  fclose(f);
}

void deleteRef(char* ref_name){
    char path[100];
    sprintf(path,".refs/%s",ref_name);
    if (!file_exists(path)){
        printf("deleteref : la reference n'existe pas\n");
    } else {
        char command[200];
        sprintf(command,"rm %s ",path);
        system(command);
    }
}


char* getRef(char* ref_name){
    char path[300];
    sprintf(path,".refs/%s",ref_name);
    if (!file_exists(path)){
        printf("getRef : La reference n'existe pas\n");
        return NULL;
    } 
    else {
        FILE *f = fopen(path,"r");
        if (f == NULL){
            printf("getRef : Erreur d'ouverture du fichier");
            return NULL;
        }
        char * hash = (char*)malloc(300*sizeof(char));
        fgets(hash,300,f);
        fclose(f);
        return hash ;
    }
}

void createFile(char* file_name){
    char command[100];
    sprintf(command,"touch %s",file_name);
    system(command);
}

void myGitAdd(char *file_or_folder) {
  WorkTree *wt;
  if (file_exists(".add") == 0) { /* Si le fichier .add n'existe pas */

    /* Création du fichier .add */
    wt = initWorkTree();
    system("touch .add");

  } else { /* Si le fichier .add existe */

    /* Récupération du fichier .add */
    wt = ftwt(".add");

  }

  if (file_exists(file_or_folder) != 0) { /* Si le fichier/repetoire existe */

    /* Ajouter au Worktree + Mise à jour .add */
    appendWorkTree(wt, file_or_folder, NULL, 0);
    wttf(wt, ".add");

  } else {

    printf("myGitAdd : le fichier/repertoire %s n'existe pas\n", file_or_folder);

  }

  freeWorkTree(wt);
}

void myGitCommit(char* branch_name, char* message){
    if (!file_exists(".refs")){
        printf("Initialiser d'abord les references du projet\n");
        return ;
    }

    char path[100];
    sprintf(path,".refs/%s",branch_name);
    if (!file_exists(path)){
        printf("La branche n'existe pas\n");
        return ;
    }

    char* branch_hash = getRef(branch_name);
    char* head_hash = getRef("HEAD");
    if (strcmp(branch_hash,head_hash)!=0){
        printf("HEAD doit pointer sur le dernier commit de la branche\n");
        return ;
    }

    WorkTree * wt;
    if (!file_exists(".add")){
        wt = initWorkTree();
    } else {
        wt = ftwt(".add");
        system("rm .add");
    }
    
    char* add_hash = saveWorkTree(wt,".");
    Commit* c = createCommit(add_hash); 
    if (strlen(branch_hash)!=0){
        commitSet(c,"predecessor",branch_hash);
    }
    if (message!=NULL){
        commitSet(c,"message",message);
    }
    char* commit_hash = blobCommit(c);
    createUpdateRef(branch_name, commit_hash);
    createUpdateRef("HEAD", commit_hash);
    free(branch_hash);
    free(head_hash);
    free(add_hash);
    free(commit_hash);
    freeWorkTree(wt);
    freeCommit(c);

}

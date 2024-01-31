#include "liste.h"
#include "dir.h"
#include "work.h"
#include "ref.h"
#include "commit.h"
#include "branch.h"
#include "checkout.h"
#include "merge.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*  un conflit survient lorsque les worktrees à fusionner contiennent 
    un fichier/repertoire de meme nom, mais avec des hash (et donc des contenus) differents
*/

List* conflictList(WorkTree* wt1, WorkTree* wt2){
  List* conflicts = initList();
    
  for (int i=0; i<wt1->n;i++){

    char* name1 = (wt1->tab[i]).name ;
    char* hash1 = (wt1->tab[i]).hash ;
    
    int k = inWorkTree(wt2, name1);

    if (k>=0){
      char* hash2 = (wt2->tab[k]).hash ;
      if (strcmp(hash1,hash2)!=0) {
        insertFirst(conflicts, buildCell(name1));
      }   
    }
  }
  return conflicts;
}

WorkTree* btwt(char* branch){
    // transforme ref en commit
    char* commit_hash = getRef(branch);
    char* commit_path = strcat(hashToPath(commit_hash), ".c");
    Commit* c = ftc(commit_path);

    // recupere le hash du worktree associé à ce commit
    char* wt_hash = commitGet(c, "tree");

    // path du wt associé
    char *wt_path = strcat(hashToPath(wt_hash), ".t");

    // creation du worktree associé
    WorkTree* wt = ftwt(wt_path);

    free(commit_hash);
    free(commit_path);
    free(wt_hash);
    free(wt_path);

    return wt ;
}


WorkTree* mergeWorkTrees(WorkTree* wt1, WorkTree* wt2, List** conflicts) {
  WorkTree* merge_wt = initWorkTree();
  _Bool conflictExists = false;
    
  for (int i = 0; i < wt1->n; i++) {

    for (int j = 0; j < wt2->n; j++) {
      if (strcmp(wt1->tab[i].name, wt2->tab[j].name) == 0 && strcmp(wt1->tab[i].hash, wt2->tab[j].hash) != 0) { // conflit
        /* s'il y a un conflit */
        insertFirst(*conflicts, buildCell(wt1->tab[i].name));
        conflictExists = true;

      }
    }

    if (conflictExists == false) {
      appendWorkTree(merge_wt, wt1->tab[i].name, wt1->tab[i].hash, wt1->tab[i].mode);
    } 
    else {
      conflictExists = false;
    }
  }
    
  for (int j = 0; j < wt2->n; j++) {
    if (searchList(*conflicts, wt2->tab[j].name) == NULL) {
      /* si ce n'est pas un conflit */
      appendWorkTree(merge_wt, wt2->tab[j].name, wt2->tab[j].hash, wt2->tab[j].mode);
    }
  }
    
  return merge_wt;
}

List* merge(char* remote_branch, char* message) {
	  printf("merge : debut \n");
    List* L = initList();
    List** conflicts = &L;
    char* current_branch = getCurrentBranch();
    
    printf("reconsitution du commit de remote_branch\n");
    char path_remote[100] = "";
    char* hash_remote = getRef(remote_branch);
    strcpy(path_remote, hash_remote);
    strcat(path_remote, ".c");
    char* path_remote_commit = hashToPath(path_remote);
    Commit* commit_remote = ftc(path_remote_commit);
    
    printf("reconsitution du WorkTree de remote_branch\n");
    WorkTree* wt_remote = ctwt(commit_remote);
    
    printf("reconsitution du commit de current_branch\n");
    char path_current[100] = "";
    char* hash_current = getRef(current_branch);
    strcpy(path_current, hash_current);
    strcat(path_current, ".c");
    char* path_current_commit = hashToPath(path_current);
    Commit* commit_current = ftc(path_current_commit);

    printf("reconsitution du WorkTree de current_branch\n");
    WorkTree* wt_current = ctwt(commit_current);

    printf("fusion des deux wt\n");
    WorkTree* wt_fusion = mergeWorkTrees(wt_remote, wt_current, conflicts);
    
    
    if (**conflicts == NULL) {
      printf("liste de conflits nulle (debut)\n");
    	char* hash_wt_fusion = blobWorkTree(wt_fusion);
      Commit* c = createCommit(hash_wt_fusion);
      commitSet(c, "predecessor", hash_current);
      commitSet(c, "merged_predecessor", hash_remote);
      commitSet(c, "message", message);
      printf("debut de creation instantenne commit merge\n");
      char* hash_commit_fusion = blobCommit(c);
      printf("fin de creation instantenne commit merge\n");
      printf("mise à jour de la ref de la branche courante et HEAD\n");
      createUpdateRef(current_branch, hash_commit_fusion);
      createUpdateRef("HEAD", hash_commit_fusion);

      deleteRef(remote_branch);
      printf("restauration work tree merge\n");
      restoreWorkTree(wt_fusion, ".");

      printf("liste de conflits nulle (fin)\n");
      free(path_remote_commit);
      freeCommit(commit_remote);
      freeWorkTree(wt_remote);
      free(path_current_commit);
      freeCommit(commit_current);
      freeWorkTree(wt_current);
      freeWorkTree(wt_fusion);

      free(hash_wt_fusion);
      freeCommit(c);
      free(hash_commit_fusion);
      freeList(L);
      return NULL;
        
    } else {
    	printf("Au moins une collision détectée entre la branche %s et la branche courante\nIl n'y a rien à faire", remote_branch);
      free(path_remote_commit);
      freeCommit(commit_remote);
      freeWorkTree(wt_remote);
      free(path_current_commit);
      freeCommit(commit_current);
      freeWorkTree(wt_current);
      freeWorkTree(wt_fusion);
      return *conflicts;
    }
}


void createDeletionCommit(char* branch, List* conflicts, char* message) {

	printf("createDeletionCommit sur la branche %s (debut)\n", branch);
	
	char* current_branch = getCurrentBranch();
	
  /* deplacement sur la branche branch */
  myGitCheckoutBranch(branch);

  char hash_path_c[100] = "";
  char* hash_c = getRef(branch);
  strcpy(hash_path_c, hash_c);
  strcat(hash_path_c, ".c");
  char* path_c = hashToPath(hash_path_c);
  Commit* c = ftc(path_c);
    
  WorkTree* wt = ctwt(c); //
  
  if (file_exists(".add")){
    system("./myGit clear-add");
  }
    
  for (int i = 0; i < wt->n; i++) {
    if (searchList(conflicts, wt->tab[i].name) == NULL) { /* Si ce n'est pas un conflit */
      myGitAdd(wt->tab[i].name);
    }
  }
  
  /* commit obtenu apres suppression des conflits */
  myGitCommit(branch, message); 
  
  /* retour sur la branche courante */
  myGitCheckoutBranch(current_branch);
  
  free(current_branch);
  free(path_c);
  freeCommit(c);
  freeWorkTree(wt);
  printf("createDeletionCommit (fin)\n");   
}

void conflitsChoice(char *branch1, char *branch2, List *conflicts,List **conflicts1, List **conflicts2) {
  
  List cour = *conflicts;

  while (cour != NULL) {
    
    printf("fichier/repertoire : %s\n", cour->data);
    printf("1 : Ajouter à la branche %s\n", branch1);
    printf("2 : Ajouter à la branche %s\n", branch2);

    int choix;
    scanf("%d", &choix);

    switch(choix) {

    case 1:
      if (cour->data!=NULL){
        insertFirst(*conflicts1, buildCell(cour->data));
      }
      cour = cour->next;
      break;

    case 2:
      if (cour->data!=NULL) {
        insertFirst(*conflicts2, buildCell(cour->data));
      }
      cour = cour->next;
      break;

    default:
      printf("conflitsChoice : Erreur de formatage\n");
    }
    
  }
}

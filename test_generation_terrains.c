#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "generation_terrains.h"

int nb_cases_occupees(Terrain T){
	int occupee = 0;
	int x, y;
	for(x=0; x<largeur(T); x++){
		for(y=0; y<hauteur(T); y++){
			if(T.tab[x][y] != LIBRE)
			occupee++;
		}
	}
	return occupee;
}

// Test de generation aléatoire de terrains
// Le programme génère n terrains de largeur et hauteur fixes
// avec largeur et hauteur impaires et inférieures a dimension_max de terrain.h
// avec densité d'obstacle dObst
// autre que la case centrale soit occupee
// l'appel du programme se fait avec 5 arguments :
// generation_terrains N largeur hauteur dObstacle fichier_res
// la sortie se fait dans le fichier resultat

int main(int argc, char ** argv){
  int N, i, l, h,yes_no;
  float dObst,concret_dObst,moy_concret_dObst=0;
  float per_terrains_correct=0;
  FILE* resFile;
  Terrain T;
    
  if (argc < 6) {
    printf("Usage: %s <N> <largeur> <hauteur> <densite_obstacle> <fichier_res> \n", argv[0]);
    return 1;
  }
    
  N = strtol(argv[1], NULL, 10);
  l = strtol(argv[2], NULL, 10);
  h = strtol(argv[3], NULL, 10);
  dObst = strtof(argv[4], NULL);
  
  // test de l et h
  if (N<=0) {
	printf("Entrer un nombre N > 0 : \n");
	}

  if (l>DIM_MAX || l%2 == 0) {
    printf("Largeur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if (h>DIM_MAX || h%2 == 0) {
    printf("Hauteur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if ((dObst > 1) || (dObst < 0)) {
    printf("Densité incorrecte : doit être comprise entre 0 et 1\n");
    return 1;
  }
	
  // Ouverture du fichier résultat
  resFile = fopen(argv[5], "w");
	if ( resFile == NULL ) {
		printf("Erreur écriture terrains : erreur d'ouverture du fichier\n");
    		exit(1);
	}
  // Écriture du nombre de terrains
  i=fprintf(resFile, "%d\n", N);
  if (i<0) return 1;// [BUG] : ecriture dans le fichier

  //Initialisation de la fonction random
  // A compléter
   srand(time(NULL)); 
  // A compléter

  // Génération aléatoire des terrains
	for (i=0; i<N; i++) {
  		generation_aleatoire(&T, l, h, dObst);
		afficher_terrain(&T);
		while (!existe_chemin_vers_sortie(T)) {
			generation_aleatoire(&T, l, h, dObst);afficher_terrain(&T);
		}
			
  // Écriture des terrains générés dans le fichier resFile
		ecrire_terrain( resFile, T, T.largeur/2, T.hauteur/2);
  // Écriture/Affichage des statistiques
		concret_dObst = cmp_per_Obst(T,dObst,&yes_no);
		fprintf (resFile, "%f\n", concret_dObst);

		per_terrains_correct+=yes_no;

		moy_concret_dObst+=concret_dObst;

	}

	//la densité d'obstacle moyenne sur les terrains
	moy_concret_dObst=moy_concret_dObst*1.0/N;
	fprintf (resFile, "%f\n", moy_concret_dObst);
	
	//le pourcentage de terrain valides par rapport au nombre total généré
	per_terrains_correct=per_terrains_correct*100.0/N;
	fprintf (resFile, "%f\n",per_terrains_correct);
  // fermeture des fichiers	
  fclose(resFile);
  return 0;
}

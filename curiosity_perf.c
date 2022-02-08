#include "environnement.h"
#include "programme.h"
#include "interprete.h"
#include "generation_terrains.h"

#include <stdio.h>
#include <stdlib.h>


void gestion_erreur_terrain(erreur_terrain e) {
  switch(e) {
  case OK: break;
  case ERREUR_FICHIER_VIDE:
    printf("Erreur lecture du terrain : fichier vide\n");
    exit(1);
  case ERREUR_FICHIER:
    printf("Erreur lecture du terrain : erreur d'ouverture du fichier\n");
    exit(1);
  case ERREUR_LECTURE_LARGEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la largeur\n");
    exit(1);
  case ERREUR_LECTURE_HAUTEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la hauteur\n");
    exit(1);
  case ERREUR_LARGEUR_INCORRECTE:
    printf("Erreur lecture du terrain : largeur incorrecte\n");
    exit(1);
  case ERREUR_HAUTEUR_INCORRECTE:
    printf("Erreur lecture du terrain : hauteur incorrecte\n");
    exit(1);
  case ERREUR_CARACTERE_INCORRECT:
    printf("Erreur lecture du terrain : caractere incorrect\n");
    exit(1);
  case ERREUR_LIGNE_TROP_LONGUE:
    printf("Erreur lecture du terrain : ligne trop longue\n");
    exit(1);
  case ERREUR_LIGNE_TROP_COURTE:
    printf("Erreur lecture du terrain : ligne trop courte\n");
    exit(1);
  case ERREUR_LIGNES_MANQUANTES:
    printf("Erreur lecture du terrain : lignes manquantes\n");
    exit(1);
  case ERREUR_POSITION_ROBOT_MANQUANTE:
    printf("Erreur lecture du terrain : position initiale du robot manquante\n");
  case ERREUR_POSITION_ROBOT_EN_TROP:
    printf("Erreur lecture du terrain : position initiale du robot manquante\n");
    exit(1);
  }
}

void affichage_position_programme(erreur_programme e) {
  int i;
  printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
  printf("%s\n", e.ligne);
  /* Impression de e.num_colonne-1 espaces */
  for(i = 1; i < e.num_colonne; i++) {
    printf(" ");
  }
  /* Impression d'un curseur de position */
  printf("^\n");
}

void gestion_erreur_programme(erreur_programme e) {
  switch(e.type_err) {
  case OK_PROGRAMME: break;
  case ERREUR_FICHIER_PROGRAMME:
    printf("Erreur lecture du programme : erreur d'ouverture du fichier\n");
    exit(2);
  case ERREUR_BLOC_NON_FERME:
    printf("Erreur lecture du programme : bloc non ferme\n");
    exit(2);
  case ERREUR_FERMETURE_BLOC_EXCEDENTAIRE:
    printf("Erreur lecture du programme : fermeture de bloc excedentaire\n");
    affichage_position_programme(e);
    exit(2);
  case ERREUR_COMMANDE_INCORRECTE:
    printf("Erreur lecture du programme : commande incorrecte\n");
    affichage_position_programme(e);
    exit(2);
  }
}

int main(int argc, char ** argv) {

  //Curiosity---------------------
  Environnement envt;
  Programme prog;
  //erreur_terrain errt;   // avoir ...
  erreur_programme errp;
  etat_inter etat;
  resultat_inter res;
  
  int steps_sortie=0 ;
  int blocke=0, roche=0, eau=0, sortie=0 ; // Les statistics pertinenet
//  int x, y; // Position initiale du robot


  //generation tterrain-----------
  int N, i, l, h/*,yes_no*/; int graine,nb_step_max;
  float dObst,/*concret_dObst,*/moy_concret_dObst=0;
  float per_terrains_correct=0;
  FILE* resFile;

    
  if (argc < 9) {
    printf("Usage: %s <fichier_programme> <N> <largeur> <hauteur> <densite_obstacle> <graine> <nb_step_max> <fichier_res>\n", argv[0]);
    return 1;
  }

  //generation tterrain-----------
  N = strtol(argv[2], NULL, 10);
  l = strtol(argv[3], NULL, 10);
  h = strtol(argv[4], NULL, 10);
  dObst = strtof(argv[5], NULL);
  graine = strtol(argv[6], NULL, 10);
  nb_step_max = strtol(argv[7], NULL, 10);

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




  //Curiosity---------------------

  /* Lecture du programme */
  errp = lire_programme(&prog, argv[1]);
  gestion_erreur_programme(errp);

  /* Initialisation de l'environnement : generation/lecture des terrains,
     initialisation de la position du robot */

  //Initialisation de la fonction random
   srand(graine); 

  // Ouverture du fichier résultat
  resFile = fopen(argv[8], "w");
	if ( resFile == NULL ) {
		printf("Erreur écriture résultats : erreur d'ouverture du fichier\n");
    		exit(1);
	}
  // Écriture du nombre de terrains
  i=fprintf(resFile, "%d\n", N);
  if (i<0) return 1;// [BUG] : ecriture dans le fichier

 // Génération aléatoire des terrains
	for (i=0; i<N; i++) {
  		generation_aleatoire(&(envt.t), l, h, dObst);
		
		while (!existe_chemin_vers_sortie(envt.t)) {
			generation_aleatoire(&(envt.t), l, h, dObst);
		}
		//afficher_terrain(&(envt.t));	
  // Écriture des terrains générés dans le fichier resFile
		init_robot(&(envt.r), envt.t.largeur/2, envt.t.hauteur/2, Est); 

//---------------------------------------------------A completer----------------------------------------------------------
 /* Initialisation de l'état */
  init_etat(&etat);
  do {
    res = exec_pas(&prog, &envt, &etat);
    /* Affichage du terrain et du robot */
    //afficher_envt(&envt);
  } while( (res == OK_ROBOT) && (etat.pc < nb_step_max) );

  /* Affichage du résultat */
  switch(res) {
  case OK_ROBOT: /*printf("Robot sur une case libre, programme non termine (ne devrait pas arriver)\n");*/ fprintf(resFile, "%d\n", -1); blocke++;  break;
  case SORTIE_ROBOT:/* printf("Le robot est sorti :-)\n");*/ fprintf(resFile, "%d\n", etat.pc); sortie++ ; steps_sortie+= etat.pc; break;
  case ARRET_ROBOT: /*printf("Robot sur une case libre, programme termine :-/\n"); */fprintf(resFile, "%d\n", -1); blocke++; break;
  case PLOUF_ROBOT: /*printf("Le robot est tombe dans l'eau :-(\n");*/ fprintf(resFile, "%d\n", -2); eau ++ ; break;
  case CRASH_ROBOT:/* printf("Le robot s'est ecrase sur un rocher X-(\n");*/ fprintf(resFile, "%d\n", -3); roche++ ; break;
  case ERREUR_PILE_VIDE: /*printf("ERREUR : pile vide\n");*/  fprintf(resFile, "%d\n", -200); break;
  case ERREUR_ADRESSAGE: /*printf("ERREUR : erreur d'adressage\n"); */ fprintf(resFile, "%d\n", -300); break;
  case ERREUR_DIVISION_PAR_ZERO: /*printf("ERREUR : division par 0\n");*/  fprintf(resFile, "%d\n", -400); break;
  }


//-------------------------------------------------------------------------------------------------------------
 // Écriture/Affichage des statistiques

	/*
		concret_dObst = cmp_per_Obst((envt.t),dObst,&yes_no);

		per_terrains_correct+=yes_no;

		moy_concret_dObst+=concret_dObst;*/

	}

	printf ("Les statistiques pertinentes de «performance» du programme : \n ");
	printf("\t>nombre et pourcentage de terrains d'où le robot est sorti : ( %d , %f ) \n", sortie, sortie*100.0/N); 
	printf("\t>où il est resté bloqué : ( %d , %f ) \n", blocke, blocke*100.0/N);
	printf("\t>où il est rentré dans un obstacle : ( %d , %f ) \n", roche, roche*100.0/N);
	if ( sortie != 0 ) {
		printf("Le nombre moyen de pas effectués pour les sorties : %f \n", steps_sortie*1.0/ sortie); 
		}else {
		printf("Domage!, il n'existe pas de nombre moyen de pas effectués pour \"les sorties\" \n"); 
		}	

	//la densité d'obstacle moyenne sur les terrains
	moy_concret_dObst=moy_concret_dObst*1.0/N;

	
	//le pourcentage de terrain valides par rapport au nombre total généré
	per_terrains_correct=per_terrains_correct*100.0/N;

  // fermeture des fichiers	
  fclose(resFile);
  return 0;
}


 

#include "terrain.h"
#include <stdio.h>

int main(int argc, char ** argv) {
  Terrain t;
  int x, y;
  
  if(argc < 2) {
    printf("Usage : %s <fichier>\n", argv[0]);
    return 1;
  }

  char nom_fich[256];
  erreur_terrain results_lec, results_aff;
  int nbr_lectures=0;
  int break_flag=0;



  results_lec=lire_terrain(argv[1], &t, &x, &y);
  while (break_flag==0) {
  
  switch (results_lec)
  {
	case ERREUR_FICHIER :
	printf("La lecture du terrain est erronée : ERREUR_FICHIER \n"); 
 	printf("Usage : %s <fichier>\n", argv[0]);
	printf("nom du fichier : \t");
	scanf("%s",nom_fich); printf("\n");
	results_lec=lire_terrain(nom_fich, &t, &x, &y); nbr_lectures+=1;break;
	case ERREUR_FICHIER_VIDE :
	printf("Erreur lecture du terrain : erreur notre fichier est vide \n");break_flag=1;break;
	 case OK:   printf("La lecture du terrain est réussit\n");  break_flag=1;break;
       case ERREUR_LECTURE_LARGEUR :
	printf("La lecture du terrain est erronee : ERREUR_LECTURE_LARGEUR \n"); break_flag=1; 
	break; 
 	case  ERREUR_LARGEUR_INCORRECTE:
	printf("La lecture du terrain est erronee : ERREUR_LARGEUR_INCORRECTE \n"); break_flag=1; 
	break;
 	case  ERREUR_LECTURE_HAUTEUR:
	printf("La lecture du terrain est erronee : ERREUR_LECTURE_HAUTEUR \n"); break_flag=1; 
	break;
 	case  ERREUR_HAUTEUR_INCORRECTE:
	printf("La lecture du terrain est erronee : ERREUR_HAUTEUR_INCORRECTE \n"); break_flag=1; 
	break;
 	case  ERREUR_LIGNES_MANQUANTES:
	printf("La lecture du terrain est erronee : ERREUR_LIGNES_MANQUANTES \n"); break_flag=1; 
	break;

 	case  ERREUR_LIGNE_TROP_COURTE:
	printf("La lecture du terrain est erronee : ERREUR_LIGNE_TROP_COURTE \n"); break_flag=1; 
	break;
 	case  ERREUR_LIGNE_TROP_LONGUE:
	printf("La lecture du terrain est erronee : ERREUR_LIGNE_TROP_LONGUE \n"); break_flag=1; 
	break;
 
 	case  ERREUR_CARACTERE_INCORRECT:
	printf("La lecture du terrain est erronee : ERREUR_CARACTERE_INCORRECT \n"); break_flag=1; 
	break;
 	case  ERREUR_POSITION_ROBOT_MANQUANTE:
	printf("La lecture du terrain est erronee : ERREUR_POSITION_ROBOT_MANQUANTE \n"); break_flag=1; 
	break;
 	case ERREUR_POSITION_ROBOT_EN_TROP:
	printf("La lecture du terrain est erronee : ERREUR_POSITION_ROBOT_EN_TROP \n"); break_flag=1; 
	break;
}
	if (nbr_lectures >= 3) { break_flag=1;} //après 3 mauvaises essaies au max, la lecture s'arrete ..
}
  


  printf("On a eu %d essaies d'ouverture de fichiers qui ont echouees\n", nbr_lectures);


	
results_aff = afficher_terrain(&t);
  if (results_aff ==OK) {
	printf("Affichage du terrain est reussit\n");
	if (x>-50) printf("Position initiale du robot : (%d, %d)\n", x, y);

  }else {
  	   printf("Affichage du terrain a mal tourne\n");
  }

}

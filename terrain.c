
#include "terrain.h"
#include "robot.h"
#include <stdio.h>
#include <string.h>

erreur_terrain lire_terrain(char * nom_fichier, Terrain * t, int * x, int * y) {
  FILE * f;
  int l, h; // Dimensions du terrain
  int rx, ry; // Coordonnées initiales du robot
  *x=-50;
  *y=-50;
  int n=0;
  char *res = NULL;
  int i,j,len_ligne;
  char ligne [DIM_MAX];
  Case c;
  int robot_trouve=0;
  // Ouverture du fichier en lecture
  f = fopen(nom_fichier, "r");
  if (f == NULL) {
    return ERREUR_FICHIER;
  }
	printf("-------------------------------\n");	printf("Lecture du terrain en cours ..\n");

  if (feof(f)) {
	return ERREUR_FICHIER_VIDE;
	}
  // Lecture de la largeur
  n = fscanf(f,"%d", &l);
  if (n == 0) {
    return ERREUR_LECTURE_LARGEUR;
  } else if ((l < 0) || (l > DIM_MAX)) {
    return ERREUR_LARGEUR_INCORRECTE;
  }
  t->largeur = l;


  // Lecture de la hauteur
  n = fscanf(f,"%d", &h);
  if (n == 0) {
    return ERREUR_LECTURE_HAUTEUR;
  } else if ((h < 0) || (h > DIM_MAX)) {
    return ERREUR_HAUTEUR_INCORRECTE;
  }
  t->hauteur = h;



// initialition du terrain
for(j = 0; j < h; j++) {
	for(i = 0; i < l; i++) {
            t->tab[i][j] = INCONNUE; //utile dans la gestion d'erreurs pendant l'affichage 
    	}
}


  // Lecture du terrain
  // Lecture du caractère de retour à la ligne précédant la première ligne
  fscanf(f,"\n");
  for(j = 0; j < h; j++) {
    // Lecture d'une ligne dans le fichier
    res = fgets(ligne, DIM_MAX, f);
    if (res == NULL) {
      return ERREUR_LIGNES_MANQUANTES;
    }
    len_ligne = strlen(ligne) - 1; // Tenir compte du caractère de retour à la ligne
    if(len_ligne < l) {
      return ERREUR_LIGNE_TROP_COURTE;
    } else if (len_ligne > l) {
      return ERREUR_LIGNE_TROP_LONGUE;
    }
    // Parcours de la ligne
    for(i = 0; i < l; i++) {
      // Initialisation d'une case
      switch(ligne[i]) {
      case '.': c = LIBRE; break; 
      case '#': c = ROCHER; break; 
      case '~': c = EAU; break;
      case 'C':
        c = LIBRE;
        rx = i;
        ry = j;
        robot_trouve += 1;
        break;
      default:
        return ERREUR_CARACTERE_INCORRECT;
      }
      t->tab[i][j] = c;
    }
  }
  if (robot_trouve<1) {
    return ERREUR_POSITION_ROBOT_MANQUANTE;
  }else if (robot_trouve>1) {
    return ERREUR_POSITION_ROBOT_EN_TROP;
  }else {



  // Initialisation de la position du robot
  *x = rx;
  *y = ry;
}
  

  fclose(f);
  return OK;
}






int largeur(Terrain t) {
  return t.largeur;
}

int hauteur(Terrain t) {
  return t.hauteur;
}

int est_case_libre(Terrain t, int x, int y) {
  if ((x >= 0) && (x < t.largeur)
      && (y >= 0) && (y < t.hauteur)) {
    return t.tab[x][y] == LIBRE;
  } else {
    return 0; // false
  }
}

erreur_terrain afficher_terrain(Terrain * t) {
  int i, j;
  char c;
  erreur_terrain resultat= OK;
printf("-------------------------------");
printf("\nAffichage du terrain :\n");
  printf("\nla taille du terrain est : %d x %d \n",t->largeur, t->hauteur);
  for(j = 0; j < t->hauteur; j++) {
    for(i = 0; i < t->largeur; i++) {
      switch(t->tab[i][j]) {
      case LIBRE : c = '.'; break;
      case ROCHER: c = '#'; break;
      case EAU   : c = '~'; break;
	case INCONNUE : c= 'x';resultat= ERREUR_CARACTERE_INCORRECT;break;

}
      printf("%c", c);
    }
    printf("\n");
  }

        
    return resultat;

}

void ecrire_terrain(FILE * f, Terrain T, int x, int y){
  int i, j;
  char c;
printf("-------------------------------");
printf("\nEcriture du terrain ...\n");
fprintf(f,"%d\n",T.largeur);
//fputc('\n', f);
fprintf(f,"%d\n",T.hauteur);
//fputc('\n', f);


  for(j = 0; j < T.hauteur; j++) {
    for(i = 0; i < T.largeur; i++) {
      switch(T.tab[i][j]) {
      case LIBRE : if ((i==x)&&(j==y)) {c = 'C';}else{c = '.';}; break;
      case ROCHER: c = '#'; break;
      case EAU   : c = '~'; break;
	case INCONNUE : c= 'x'; break;
	
}
	
      fputc(c, f);
    }
    fputc('\n',f);
  }

        

}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "generation_terrains.h"


/* Génère aléatoirement un terrain T, de largeur l, de hauteur h,
   avec une densité d'obstacle dObst : 0 <= dObst <= 1.
   Précondition : T est un pointeur vers un terrain existant.
   Résultat : T est modifié et contient le terrain généré.
              La case centrale de T ne contient pas d'obstacle.
 */
void generation_aleatoire(Terrain* T, int l, int h, float dObst){
  /* À compléter */ 
	int i,j;
 if ( (h*l) > 1000 ) {
	Case c;
	float per=0.0;
	T->largeur=l;
	T->hauteur=h;
  for(j = 0; j < T->hauteur; j++) {
    for(i = 0; i < T->largeur; i++) {
	

	//tirer un nombres entre 0 et 100
	per = (float)rand() / (float)RAND_MAX ;
	//per= v/100; printf ("|%f|\n",per);
	if (per >= dObst || (i== T->largeur/2 && j==T->hauteur/2)) {
		c=LIBRE;
	}else {
		c=ROCHER;
	}
		
      	T->tab[i][j] = c;}
    }
}else{
	T->largeur=l;
	T->hauteur=h;
	if (dObst>0.5)
	{	
	int taille_lxh=l*h;
	float nbr_O2=(int) (dObst*10000);
	int nbr_O =  (int) (nbr_O2*taille_lxh/10000);//printf("%d\n",nbr_O); 
	
  for(j = 0; j < T->hauteur; j++) {
    for(i = 0; i < T->largeur; i++) {
	if ( !(i== T->largeur/2 && j==T->hauteur/2) ) {
		T->tab[i][j] = ROCHER;
	}else { T->tab[i][j] = LIBRE ; }
  } }
	
	nbr_O+=1;
	while ( nbr_O < taille_lxh ) {
	i = rand() % l;
	j = rand() % h;
	if (!( T->tab[i][j] == LIBRE)) {
		T->tab[i][j] = LIBRE;
		
		nbr_O+=1;
		}
	}




	}else {
	float nbr_O2=(int) (dObst*10000);
	int nbr_O =  (int) (nbr_O2*l*h/10000);//printf("%d\n",nbr_O); 
  for(j = 0; j < T->hauteur; j++) {
    for(i = 0; i < T->largeur; i++) {
	T->tab[i][j] = LIBRE;
	}
  }	
	while ( nbr_O >=0 ) {
	i = rand() % l;
	j = rand() % h;
	if ( !(i== T->largeur/2 && j==T->hauteur/2) && !( T->tab[i][j] == ROCHER)) {
		T->tab[i][j] = ROCHER;
		
		nbr_O-=1;
		}
	}
}

}

}


// determine s'il existe un chemin du centre au bord du terrain T
// version avec tableau annexe
int existe_chemin_vers_sortie(Terrain T){
	
  int l = largeur(T);
  int h = hauteur(T);
  int chemin_trouve = 0;
  int i;
  // tableau de marque
  // initialisation
  //   Marque(x,y) = 0 <=> la case est libre et non marquee
  //   Marque(x,y) = 3 <=> la case est occupee (eau/roche)
  // boucle
  //   Marque(x,y) = 0 <=> la case est libre et non marquee
  //   Marque(x,y) = 1 <=> la case est libre, marquee et non traitee
  //   Marque(x,y) = 2 <=> la case est libre, marquee et traitee
  //   Marque(x,y) = 3 <=> la case est occupee
	
  //
  int Marque[DIM_MAX][DIM_MAX];
  int x,y,x1,y1;
  int existe_case_1;
	
  // initialiser le tableau Marque
  for (y=0; y<h;y++){
    for (x=0; x<l;x++){
      if (est_case_libre(T,x,y)){
        Marque[x][y] = 0;
      }
      else{
        Marque[x][y] = 3;
      }
    }
  }
  // marquer la seule case centrale
  x = l/2;
  y = h/2;
  Marque[x][y] = 1;

  // boucle de recherche du chemin : trouver un ensemble connexe
  // de cases (marquées 1 ou 2) contenant la case centrale et
  // une case de bord
  existe_case_1 = 1;
  while(existe_case_1 && !chemin_trouve){
    // rechercher une case libre, marquee et non traitee
    existe_case_1 = 0;
    x = 0;
    y = 0;
    while(y<h && !existe_case_1){
      if(Marque[x][y] == 1){
        // la case (x,y) est marquée 1
        existe_case_1 = 1;
      }
      else{
        // passer à la case suivante
        x++;
        if(x>=l){
          x = 1;
          y++;
        }
      }
    }
		
    if(existe_case_1){
      // marquer la case (x,y) comme marquee et traitee
      Marque[x][y] = 2; 
			
      // rechercher les cases voisines de (x,y) non marquees (0)
      // et pour chaque case voisine (x1,y1) vide et non marquee, 
      // si c'est une case de bord, 
      //   mettre le booleen chemin_trouve a VRAI
      // sinon
      //  la marquer (1,x,y) et l'empiler
      for (i=0; i<4;i++){
        switch(i){
        case 0:
          x1 = x;
          y1 = y+1;
          break;
        case 1:
          x1 = x;
          y1 = y-1;
          break;							
        case 2:
          x1 = x+1;
          y1 = y;
          break;
        case 3:
          x1 = x-1;
          y1 = y;
          break;
        default:
          break;
        }
        if(x1 < l && y1 < h && Marque[x1][y1] == 0){
          Marque[x1][y1] = 1;
          if(x1==0 || x1==l-1 || y1==0 || y1==h-1){
            chemin_trouve = 1;
          }
        }
      }
    }
  }
  return chemin_trouve;
}

/* 5% de marge est permise  */
float cmp_per_Obst (Terrain T, float dObst, int* flip_flop){
  /* À compléter */
	int i,j,v=0;
	float per=0,min,max;
	char percent='%';
	
  for(j = 0; j < T.hauteur; j++) {
    for(i = 0; i < T.largeur; i++) {
	
	
	if ( !est_case_libre(T,i,j)) {
		v +=1 ;
	}
		
      	
    }
  }
	per=v*1.0/(T.largeur*T.hauteur);
	min = dObst-0.05;
	max = dObst+0.05;
	if ((per > min) && (per < max )) {
		printf ("Nbr d'Obstacle réservé à 5 %c marche d'erreur\n",percent);*flip_flop=1;
	}else{
		printf ("Nbr d'Obstacle non-réservé\n");*flip_flop=0;
	}
	return per;
}



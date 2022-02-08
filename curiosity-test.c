#include "environnement.h"
#include "programme.h"
#include "interprete.h"
#include<string.h>

#include <stdio.h>
#include <stdlib.h>



void gestion_erreur_terrain(erreur_terrain e) {
  switch(e) {
  case OK: break;
  case ERREUR_FICHIER:
    printf("Erreur lecture du terrain : erreur d'ouverture du fichier\n");
    exit(1);
  case ERREUR_FICHIER_VIDE :
  printf("Erreur lecture du terrain : erreur notre fichier est vide \n");
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
    exit(1);
  case ERREUR_POSITION_ROBOT_EN_TROP:
    printf("Erreur lecture du terrain : position initiale du robot est en trop\n");
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
//---------------------------------**-*---*----*------*****************------------********************************

/* Affiche une orientation sur la sortie standard */
void afficher_orientation(Orientation o) {
  switch(o) {
  case Nord : printf("Nord\n"); break;
  case Est  : printf("Est\n"); break;
  case Sud  : printf("Sud\n"); break;
  case Ouest: printf("Ouest\n"); break;
  }
}


void ecris(resultat_inter res)
{  
  switch(res) {
  case OK_ROBOT: printf("Robot sur une case libre, programme non termine (ne devrait pas arriver)\n"); break;
  case SORTIE_ROBOT:  printf("Le robot est sorti :-)\n");break;
  case ARRET_ROBOT:  printf("Robot sur une case libre, programme termine :-/\n"); break;
  case PLOUF_ROBOT:  printf("Le robot est tombe dans l'eau :-(\n"); break;
  case CRASH_ROBOT:  printf("Le robot s'est ecrase sur un rocher X-(\n"); break;
  default: break;
  }
}




void ecris2(char c)
{
	char screen [75];	
	switch ( c)
       	{
           case 'P' : strcpy(screen,"le robot est tombe dans l'eau"); break;
           case 'F' : strcpy(screen,"le programme est termine"); break;
           case 'S' : strcpy(screen,"le robot est sorti du terrain"); break;
           case 'N' : strcpy(screen,"le robot est sur une position normale a l'interieur du terrain"); break;
	   case 'O' : strcpy(screen,"le robot a rencontre un obstacle");break;
	}
	printf("%s",screen);
	

}




int main(int argc, char ** argv) {
  Environnement envt;
  Programme prog;
  erreur_terrain errt;
  erreur_programme errp;
  etat_inter etat;
  resultat_inter res;
  char *res_fgets=NULL;
  if (argc < 2) {
    printf("Usage: %s <fichier>\n", argv[0]);
      return 1;
  }
  char nom_terrain[25];
  FILE *f_principal;
for (int index=1;index<argc;index++)
{
  f_principal=fopen(argv[index],"r");



  printf ("******  %d : %s  *************************************\n",index, argv[index]);
  if (f_principal == NULL) {
	printf("[Bug] : erreur Fichier, ouverture impossible du \" %s \" \n",argv[index]);
	return 1;
  }



  res_fgets=fgets(nom_terrain,256,f_principal);
  if (res_fgets == NULL) {
	printf("[Bug] : erreur lecture du nom du fichier terrain\n");
	return 1;
  }

  //remplace '\n' par '\0' : marque du fin chaine de caracteres
  int length=0;
  length=strlen(nom_terrain);  
  nom_terrain[length-1]=nom_terrain[length];

  /* Initialisation de l'environnement : lecture du terrain,
     initialisation de la position du robot */
  errt = initialise_environnement(&envt,nom_terrain);

  gestion_erreur_terrain(errt);



  /* Lecture du programme */
  char nom_programme[250];
  res_fgets=fgets(nom_programme,256,f_principal);
  if (res_fgets == NULL) {
	printf("[Bug] : erreur lecture du nom du fichier programme\n");
	return 1;
  }

  //remplace '\n' par '\0' : marque du fin chaine de caracteres
  length=strlen(nom_programme);  
  nom_programme[length-1]=nom_programme[length];


  errp = lire_programme(&prog, nom_programme);
  gestion_erreur_programme(errp);


  /* Lecture du nombre de pas a executer */
  int nbr_max;
  fscanf(f_principal,"%d",&nbr_max);


  /* Initialisation de l'état */
  init_etat(&etat);
  int compteur=0;


  /* Execution du programme */
  do {
    res = exec_pas(&prog, &envt, &etat);compteur++;
    
    /* Affichage du terrain et du robot */
    //afficher_envt(&envt);
  } while(compteur < nbr_max && res == OK_ROBOT );

  /* Lecture du resultat attendu */
    char f_result; int n_err=0;
    n_err=fscanf(f_principal," ");
    n_err=fscanf(f_principal,"%c",&f_result);
    if (n_err == 0) { 
	printf("[Bug] : erreur lecture du resultat attendu du programme\n");
	return 2;
  }




  /* Conversion du résultat */
    char result_prog='\0';
  switch(res) {
  case OK_ROBOT: result_prog = 'N'; break;
  case SORTIE_ROBOT: result_prog = 'S'; break;
  case ARRET_ROBOT: result_prog = 'F'; break;
  case PLOUF_ROBOT: result_prog = 'P'; break;
  case CRASH_ROBOT: result_prog = 'O'; break;
  case ERREUR_PILE_VIDE: printf("ERREUR : pile vide\n"); return 3; break; 
  case ERREUR_ADRESSAGE: printf("ERREUR : erreur d'adressage\n"); return 3; break;
  case ERREUR_DIVISION_PAR_ZERO: printf("ERREUR : division par 0\n"); return 3; break;
  }


  /* Affichage du résultat */
  if ( result_prog == f_result ) { 
   printf("[OK] (etat): L'etat finale du robot est le meme que la description\n");
   if ( result_prog == 'F' || result_prog == 'N')
   {
       int fich_x,fich_y;
       char fich_ori;
       Orientation orient_f;


       n_err=fscanf(f_principal,"%d",&fich_x);
	    if (n_err == 0) { 
		printf("[Bug] : erreur lecture du resultat attendu du programme : le parametre 'x'\n");
		return 4;
	  }	


       n_err=fscanf(f_principal,"%d",&fich_y);
	    if (n_err == 0) { 
		printf("[Bug] : erreur lecture du resultat attendu du programme : le parametre 'y'\n");
		return 4;
	  }	


	fscanf(f_principal," "); //lire le retour à la ligne'\n'
	n_err = fscanf(f_principal,"%c",&fich_ori);
	    if (n_err == 0) { 
		printf("[Bug] : erreur lecture du resultat attendu du programme : le parametre 'Orientation'\n");
		return 4;
	  }	

	   /* Conversion du résultat : Orientation */  
	switch (fich_ori)
       {
           case 'O' : orient_f=Ouest; break;
           case 'E' : orient_f=Est; break;
           case 'S' : orient_f=Sud; break;
           case 'N' : orient_f=Nord; break;
           default : printf("[err]: lecture orientaion -> valeur non-valide \t.....\tuse: {N,S,E,O}\n"); return 1;
       }    



    
         /* Affichage du résultat */
       if (fich_x == envt.r.x && fich_y == envt.r.y && orient_f == envt.r.o )
       {
		printf("[OK] (config): La config finale du robot est le meme que la description\n");
		printf("-----------------------------------------------------------------------\n");
		printf("TOUT EST BON !!\n");
		printf("-----------------------------------------------------------------------\n");
       }
	else {
		printf("-----------------------------------------------------------------------\n");
		printf("[ERR] (config): resultat atendu (%d,%d) orientation : ",fich_x,fich_y);
		afficher_orientation(orient_f);
		printf("[ERR] (config): resultat obtenu (%d,%d) orientation : ",envt.r.x,envt.r.y);
		afficher_orientation(envt.r.o);
		printf("-----------------------------------------------------------------------\n");
	}

   }else { 
		printf("[OK] (config): pas besoin du config finale du robot dans la description\n");		
		printf("-----------------------------------------------------------------------\n");
		printf("TOUT EST BON !!\n");
		printf("-----------------------------------------------------------------------\n");
	}
}
else{
	printf("-----------------------------------------------------------------------\n");         
	printf("[ERR] (etat): L'etat finale du robot est :\n");
	ecris(res);
	printf("On attend l'etat suivant : ");
	ecris2(f_result);
	printf("\n");
	printf("-----------------------------------------------------------------------\n");
	printf("\n");printf("\n");	
     }
}
 return 0;
}

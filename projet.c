#include "projet.h"


int main(int argc, char** argv){	

	particule tab[NP]; /* Tableau de particules principal */
	int REAL_NP; /* Nombre réel de particules */
	int i, j;

	float sum = 0.0;

	double NEFF = 0.0;
	double NSEUIL = 0.0;
	
	double angle[NB_ITERATIONS]; /* Tableau des angles de déplacement de l'utilisateur, à lire dans le fichier csv */
	double distance_parcourue[NB_ITERATIONS]; /* Tableau des distances de déplacement de l'utilisateur, à lire dans le fichier csv */
	mesure M[NB_MESURES]; /* Mesures de référence, à lire dans le fichier csv */
	double observation[NB_ITERATIONS][NB_BALISES]; /* Tableau des signaux recus par le smartphone de la part de chaque balise, à chaque itération, à lire dans le fichier csv */

	double x_estim; /* Coordonnées estimées de l'utilisateur */
	double y_estim;
	double true_x[NB_ITERATIONS]; /* Coordonnées réelles de l'utilisateur */
	double true_y[NB_ITERATIONS];
	double erreur_x = 0.0; /* Erreurs entre les coordonnées réelles et estimées de l'utilisateur */
	double erreur_y = 0.0;

	double t = clock(); /* Lancement du chronomètre */
	
	srand((unsigned)time(0));
 
	
	
	/* Initialisation du tableau de particules */
	REAL_NP = init(tab); 

	/* Lecture des fichiers csv pour remplir les variables angle, distance_parcourue, M et observation */
	read_csv(distance_parcourue, angle, M, observation, true_x, true_y);
	

	for(i = 0; i < NB_ITERATIONS; i++){ /* Pour chaque déplacement de l'utilisateur */
	
		if (angle[i] < 0) /* Si l'angle correspondant à l'itération i est négatif, on le passe en positif */
			angle[i] += TWOPI;
			
		new_coord(distance_parcourue[i], angle[i], tab, REAL_NP); /* Calcul des nouvelles coordonnees de chaque particule */
		
		angle[i] = angle[i] * 360 / (2.0 * 3.141592653589793238462643383279502884197169399375); /* Conversion en degrés de l'angle en radians */

		calcul_poids(M, tab, observation[i], angle[i], REAL_NP); /* Mise à jour du poids des particules */

	/* Détermination s'il y a besoin ou non de faire un rééchantillonnage */
		sum = 0.0;	
		for (j = 0; j < REAL_NP; j++)
			sum += tab[j].poids*tab[j].poids;

		NEFF = 1.0/sum;	
		NSEUIL = REAL_NP/2.0;
		
		if (NEFF < NSEUIL){
			printf("***********************************************\n");
			reechantillonnage(tab, REAL_NP);
		}
		
	/* Estimation des coordonnées de l'utilisateur */
		x_estim = 0.0;
		y_estim = 0.0;	
		for(j = 0; j < REAL_NP; j++){
			x_estim += tab[j].x * tab[j].poids;
			y_estim += tab[j].y * tab[j].poids;
		}
	
	/* Si les vraies coordonnées de l'utilisateur sont présentes dans le fichier csv (différentes de 0,0), on affiche la position estimée et l'erreur */
		if ((true_x[i] > 0) && (true_y[i] > 0)){
			erreur_x = fabs(true_x[i] - x_estim);
			erreur_y = fabs(true_y[i] - y_estim);
			printf("Position estimee apres %d iterations : %f, %f  Erreur sur x : %f  Erreur sur y : %f\n",i+1,x_estim,y_estim,erreur_x,erreur_y);
		} else /* Sinon on affiche uniquement la position estimée */
			printf("Position estimee apres %d iterations : %f, %f\n",i+1,x_estim,y_estim);
	 }
	 
	t = (clock()-t) / CLOCKS_PER_SEC; /* Calcul du temps d'exécution */
	
	printf("Derniere position estimee : %f, %f\n",x_estim,y_estim);
	printf("Temps d'execution : %f\n",t);

	return 0;
}

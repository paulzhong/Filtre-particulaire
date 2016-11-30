#ifndef projet
#define projet
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include <time.h>

#define NB_BALISES 6 /* Nombre de balises qui émettent un signal */
#define NP 110 /* Nombre de particules souhaité - supérieur ou égal au nombre réel de particules utiles*/
#define Pi 3.141592653589793238462643383279502884197169399375
#define WIDTH 23 /* Largeur de la zone d'étude */
#define HEIGHT 17 /* Longueur de la zone d'étude */
#define SIGMA 0.05 /* Ecart type du bruit qui va perturber le calcul des nouvelles coordonnées des particules */
#define NB_MESURES 321 /* Nombre de mesures de référence */
#define NB_ITERATIONS 113
#define CSV_REFERENCE "DatabaseSimple.csv"
#define CSV_MESURE "ReleveSimple.csv"
#define TWOPI 2.0 * 3.141592653589793238462643383279502884197169399375
 
/* 
   RANDN is a macro which returns a pseudo-random numbers from a normal
   distribution with mean zero and standard deviation one. This macro uses Box
   Muller's algorithm
*/

 #define RANDN sqrt(-2.0 * log(ran_numd())) * cos(TWOPI * ran_numd())


/* Structure définissant une particule, déterminée par ses coordonnées et son poids */
struct particule {
	double x;
	double y;
	double poids;
};
typedef struct particule particule;

/* Structure définissant une mesure de référence, dont on connait les coordonnées, le signal recu par chaque balise et l'orientation pour laquelle on reçoit ces signaux */
struct mesure {
	double x;
	double y;
	double orientation;
	double RSSI[NB_BALISES];
};
typedef struct mesure mesure;

double NormalDistribution(double mu,double sigma);
double dist_eucli_radio(double RSSI[NB_BALISES], double observation[NB_BALISES]);
double dist_eucli_particule(particule P, mesure M);
double distance_radio_empreinte_la_plus_proche(particule P, mesure M[NB_MESURES],double observation[NB_BALISES], double angle);
void read_csv(double distance_parcourue[NB_ITERATIONS], double angle[NB_ITERATIONS], mesure M[NB_MESURES], double observation[NB_ITERATIONS][NB_BALISES], double true_x[NB_ITERATIONS], double true_y[NB_ITERATIONS]);
int init(particule tab[NP]);
void new_coord(double d, double theta, particule ttttt[NP], int REAL_NP);
void calcul_poids(mesure M[NB_MESURES], particule ttttt[NP], double observation[NB_BALISES], double angle,int REAL_NP);
void reechantillonnage(particule ttttt[NP], int REAL_NP);
double ran_numd();

#endif

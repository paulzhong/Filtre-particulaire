#include "projet.h"
 
double NormalDistribution(double mu,double sigma){
/*
   This function returns a pseudo-random numbers from a normal distribution with
   mean equal at mu and standard deviation equal at sigma > 0
*/
   if(sigma <= 0.0){
   	fprintf(stderr,"Error in file %s line %u function %s : sigma must be > 0\nExit program\n",__FILE__,__LINE__,__FUNCTION__);
   	exit(EXIT_FAILURE);
   }

   return (mu+(sigma*RANDN));
}

/* Fonction qui calcule la distance euclidienne entre le vecteur des signaux reçu par chaque balise (en un point de référence) et le vecteur des signaux reçu par chaque balise observé lors d'une itération */
double dist_eucli_radio(double RSSI[NB_BALISES], double observation[NB_BALISES]){
	double total = 0;
	int nb_zeros = 0;
	int i = 0;
	
	for (i = 0; i < NB_BALISES; i++) {
		if (RSSI[i] == 0){  /* Dans le cas ou l'on n'a pas de i-ème mesure, on compte le nombre de mesures manquantes */
			nb_zeros++;
		} else {
			total += (RSSI[i] - observation[i]) * (RSSI[i] - observation[i]);
		}
	}
	
	if (nb_zeros == NB_BALISES)
		total = 100; /* Dans le cas où une mesure ne comporte pas de valeur (que des 0), on fixe le total à une valeur fixe pour éviter de renvoyer une distance nulle */
	else
		total = total * (NB_BALISES  / (NB_BALISES - nb_zeros)); /* Compensation des mesures manquantes */

	return(sqrt(total));
}	

/* Fonction qui calcule la distance euclidienne spatiale entre une particule P et un point de mesure de référence M */
double dist_eucli_particule(particule P, mesure M){
	double total = 0;
	
	total = sqrt((P.x-M.x)*(P.x-M.x) + (P.y-M.y)*(P.y-M.y));
		
	return(total);
}

/* Fonction qui calcule la distance radio entre le point de mesure de référence le plus proche spatialement d'une particule P et le signal reçu lors d'une itération */
double distance_radio_empreinte_la_plus_proche(particule P, mesure M[NB_MESURES],double observation[NB_BALISES], double angle){
	int i;
	double distance; /* Distance spatiale entre P et un point de mesure M */
	double distance_radio; /* Différence entre un vecteur de RSSI correspondant à une mesure M et le vecteur d'observation */
	double meilleure_distance; /* Plus petite distance spatiale entre P et un point de mesure M */
	double meilleur_angle; /* Plus petit angle entre l'orientation d'une mesure M et l'angle de l'utilisateur pour l'itération en question */
	double difference_angles;
	
	meilleure_distance = dist_eucli_particule(P, M[0]);
	meilleur_angle = 360;
	
	for (i = 0; i < NB_MESURES; i++){
		distance = dist_eucli_particule(P, M[i]);  /* Pour chaque mesure de référence M, on calcule la distance entre M et P */
		
		if (distance < meilleure_distance){ /* Dans le cas où le point de mesure i est plus proche que le précédent */
			meilleure_distance = distance;
			meilleur_angle = fabs(M[i].orientation - angle);
			distance_radio = dist_eucli_radio(M[i].RSSI,observation);
		} else if (distance == meilleure_distance) { /* Dans le cas où l'on se situe sur un point de mesure aux mêmes coordonnées, mais à l'orientation nécessairement différente */
			difference_angles = abs(M[i].orientation - angle);
			if (difference_angles < meilleur_angle){ /* La distance radio à prendre en compte sera celle du point de mesure le plus proche spatialement et à l'orientaion la plus proche de l'angle de l'utilisateur */
				meilleur_angle = difference_angles;
				distance_radio = dist_eucli_radio(M[i].RSSI,observation);
			}
		}
	}

	return distance_radio;
}


/* Fonctions permettant de lire les fichiers csv */
char* getfield(char* line, int num)
{
    char* tok;
    for (tok = strtok(line, ";"); tok && *tok; tok = strtok(NULL, ";\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

void read_csv(double distance_parcourue[NB_ITERATIONS], double angle[NB_ITERATIONS], mesure M[NB_MESURES], double observation[NB_ITERATIONS][NB_BALISES], double true_x[NB_ITERATIONS], double true_y[NB_ITERATIONS])
{   
    int i = 0;
    int j; 
    char* contenu_case;
  
    FILE* stream = fopen("ReleveSimple.csv", "r");
    
    char line[1024];
    char* tmp;
    
    while (fgets(line, 1024, stream))
    {
		/* Lecture de l'angle de déplacement de l'utilisateur à l'itération i */
        tmp = strdup(line);
        contenu_case = getfield(tmp, 5);
		angle[i] = atof(contenu_case);
		free(tmp);
		
		/* Lecture de la distance de déplacement de l'utilisateur à l'itération i */
		tmp = strdup(line);
        contenu_case = getfield(tmp, 6);
		distance_parcourue[i] = atof(contenu_case);
        free(tmp);
        
        /* Lecture de la vraie coordonnée x de l'utilisateur à l'itération i - 0 si il n'y a pas de valeur */
        tmp = strdup(line);
        contenu_case = getfield(tmp, 14);
		true_x[i] = atof(contenu_case);
        free(tmp);
        
        /* Lecture de la vraie coordonnée y de l'utilisateur à l'itération i - 0 si il n'y a pas de valeur */
        tmp = strdup(line);
        contenu_case = getfield(tmp, 15);
		true_y[i] = atof(contenu_case);
        free(tmp);
        
        /* Lecture du signal reçu de la part de chaque balise à l'itération i */
		for(j = 0; j < NB_BALISES; j++){
			tmp = strdup(line);
			contenu_case = getfield(tmp, 7+j);
			observation[i][j] = atof(contenu_case);
		}
        i++;
    }
    fclose(stream);
    
    i = 0;
    stream = fopen("DatabaseSimple.csv", "r");
    while (fgets(line, 1024, stream))
    {
		/* Lecture de la coordonnée x de la mesure numéro i */
        tmp = strdup(line);
        contenu_case = getfield(tmp, 1);
		M[i].x = atof(contenu_case);
		
		/* Lecture de la coordonnée y de la mesure numéro i */
		tmp = strdup(line);
        contenu_case = getfield(tmp, 2);
		M[i].y = atof(contenu_case);
		
		/* Lecture de l'orientation pour laquelle la mesure numéro i est valable */
		tmp = strdup(line);
        contenu_case = getfield(tmp, 3);
		M[i].orientation = atof(contenu_case);

		/* Lecture des observations faites pour la mesure i */
        for(j = 0; j < NB_BALISES; j++){
			tmp = strdup(line);
			contenu_case = getfield(tmp, 4+j);
			M[i].RSSI[j] = atof(contenu_case);
		}
        i++;      
    }
    fclose(stream);
    
}

double ran_numd(){
	double ran_num = 0.0;
	ran_num = rand()/(double)(RAND_MAX);

	return ran_num;
}


/* Fonction qui prend en paramètre le nombre de particules souhaité et qui initialise le nuage de particules, réparti de maniere uniforme sur la zone d'étude*/
int init(particule tab[NP]){
	
	int Nw = sqrt(NP*WIDTH/HEIGHT);
	int Nh = sqrt(NP*HEIGHT/WIDTH);
	double dd = HEIGHT / Nh;
	int REAL_NP;
	int w, h, i = 0;
	double poids;

	printf("%d\n", NP);
	
	printf("Nw = %d, Nh = %d\n",Nh, Nw);
	
	REAL_NP = Nw * Nh; /* Avec les formules données, on ne peut pas répartir uniformément NP particules quelque soit NP. On doit donc calculer le vrai nombre de particules utiles*/
	
	/* Répartition uniforme des REAL_NP particules utiles */
	poids = 1.0 / REAL_NP;

	for(w = 0; w < Nw; w++){
		for(h = 0; h < Nh; h++){
			tab[i].x = w * dd;
			tab[i].y = h * dd;
			tab[i].poids = poids;
			i++;
		}
	}
	
	/* On retourne REAL_NP afin que cette valeur puisse etre utilisée dans les autres fonctions */
	return REAL_NP;
}

/* Fonction qui calcule les nouvelles coordonnées des REAL_NP particules pour une itération */ 
void new_coord(double d, double theta, particule tab[NP], int REAL_NP){
	int i;
	double bruit = NormalDistribution(0,SIGMA);
	
	for(i = 0; i < REAL_NP; i++){
		tab[i].x += d * cos(theta) + bruit;
		tab[i].y += d * sin(theta) + bruit ;
	}
}

/* Fonction qui recalcule le poids des REAL_NP particules pour une itération */
void calcul_poids(mesure M[NB_MESURES], particule tab[NP], double observation[NB_BALISES], double angle, int REAL_NP){
	int i;
	double somme = 0;
	double distance;	
	
	for(i = 0; i < REAL_NP; i++){
		distance = distance_radio_empreinte_la_plus_proche(tab[i], M, observation, angle); /* Détermination de la distance radio entre la mesure de référence la plus proche (spatialement parlant) de la particule i et le vecteur de RSSI observé */                                                           
		tab[i].poids = tab[i].poids * (1.0/distance); /* Calcul du nouveau poids de la particule i */
		somme += tab[i].poids;
	}
	for(i = 0; i < REAL_NP; i++){
		tab[i].poids = tab[i].poids / somme; /* Normalisation des nouveaux poids*/
	}
}

/* Fonction de réechantillonnage des particules */
void reechantillonnage(particule tab[NP], int REAL_NP){
	double proba_cumul[REAL_NP];
	double tableau_alea[REAL_NP];
	particule tab_tmp[NP];
	int i;
	int j;
	int flag;


	proba_cumul[0] = tab[0].poids;
	
	for(i = 1; i < REAL_NP; i++)
		proba_cumul[i] = proba_cumul[i-1] + tab[i].poids; /* Formation du tableau des probabilités cumulées */
	
		
	
	for(i = 0; i < REAL_NP; i++)
		tableau_alea[i] = ran_numd(); /* Création d'un tableau de REAL_NP nombres aléatoires compris entre 0 et 1 */

	
	for(i = 0; i < REAL_NP; i++){ /* Pour chaque nombre du tableau aléatoire */
		flag = 0;
		j = 0;
		while(flag==0){			
			if(tableau_alea[i]<proba_cumul[j]){ /* On regarde si la j-ème valeur du tableau des probabilites cumulees est superieure au nombre aleatoire */
				tab_tmp[i].x = tab[j].x; /* On duplique la j-ème particule du tableau*/
				tab_tmp[i].y = tab[j].y;
				flag = 1; /* Fin du while */
			} else {
				j++;  /*Sinon on passe a l'index suivant dans le tableau des probabilités cumulées */
			}
		}
	}
	
	/* Copie du contenu du tableau temporaire dans le tableau principal des particules et réinitialisation des poids des particules */
	for(i = 0; i < REAL_NP; i++){
		tab[i].x = tab_tmp[i].x;
		tab[i].y = tab_tmp[i].y;
		tab[i].poids = 1.0 / REAL_NP;
	}
}

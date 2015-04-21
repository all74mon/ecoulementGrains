# include "Sable.h"

/**  Gestion de la simulation du sable */


////////////// SIMULATION ET FONCTIONS AUXILLIAIRES //////////////////////

/* Etape elementaire de la simulation à un pas de temps */
int Sable::etape_simulation(int compteur, double pavage[3], StockContacts* stock, double dt, int frequence_affichage, string* path, string* path2) {

    //cout<<"DEBUG : Simulation -- iteration numero " << compteur <<endl;

    /* Update des contacts*/
    suppression_contacts_ouverts(stock); // parcours de la liste + calcul de distance
    cherche_nouveaux_contacts(stock, dt); // sys casiers + examiner couples de grains
    update_liste_contacts(dt); // sys casiers + examiner couples de grains


    /* Affichage periodique de l'image + refresh stock*/
    if(compteur%frequence_affichage ==0){
        afficher_image(compteur, stock, path, path2);
    }

    deplacement(dt, pavage);
    compteur ++;

    return compteur;
}

/* efface gnutest + vide stock contacts, pteurs des casiers et contacts du sable */
 void Sable::effacements(string* path, StockContacts* stock) {

    // WINDOWS : system( ("del " + *path + "gnutest").c_str() );
    system( ("rm " + *path + "gnutest").c_str() ); // LINUX
    stock->vide_stock();
    delete stock;
    casiers->vide_grains();
    delete casiers;      // vidage des casiers
    vide_contacts_sable();

}

/* Creation animation + suppression des png */
void Sable::genere_animation(string* path) {
    string cvertPng =  "convert -delay 5 -loop 0 ";
    cvertPng += *path;
    cvertPng += "*.png";
    cvertPng += " animation.gif ";
    // WINDOWS : string delPng = "del ";
    string delPng = "rm ";
    delPng += *path;
    delPng += "*.png";
    system(cvertPng.c_str() );
    system(delPng.c_str() );
    system("animate animation.gif");
}

/* Lancement d'une simulation */
void Sable::simulation(double duree , double dt , int frequence_affichage, double pavage[3],
                       string *path, string *path2){

    cout << "\n---------- Fonction Simulation ---------- " << endl;

    if (g[0] == NULL ) { // tas de sable vide

        cout << "Tas de sable vide: fin de la simulation" << endl;

    } else {

        int compteur = 0;
        double temps = 0.0;
        StockContacts* stock = new StockContacts();
        init_pavage_grains(pavage);

        for (temps = 0; temps<= duree; temps+=dt) {
            compteur = etape_simulation(compteur, pavage, stock, dt, frequence_affichage, path, path2);
        }

        effacements(path, stock);
        genere_animation(path);
        cout << "FIN SIMULATION " << endl;
    }
}

////////////////////////////////////////////////////////////////


/* Deplacement de tous les grains lors d'un pas de temps
    + update de tous les casiers */
void Sable::deplacement(double dt, double pavage[3]) {
    int i = 0;
    while (g[i] != NULL) {
        if(g[i]->paroie == false){
            g[i]->deplacement(dt, pavage);
        }
        i++;
    }

    /* Update casiers puis suppression casiers vides */
    casiers->update_casiers();
    casiers->supprime_casiers_vides();
}


/* Distance au carre entre 2 grains */
double Sable::distance_carre(Grain* g1, Grain* g2){
    double dist = pow((g1->X[0]-g2->X[0]),2) + pow((g1->X[1]-g2->X[1]),2) + pow((g1->X[2]-g2->X[2]),2);
    return dist;
}

/* Initialise les coordonnees des grains selon le pavage  + ajout aux casiers*/
void Sable::init_pavage_grains(double pavage[3]) {
    int j = 0;
    while (g[j] != NULL) {
        for (int ic=0; ic<3; ic++) {
            g[j]->x_case[ic] = floor( g[j]->X[ic]/pavage[ic] );
        }
        casiers->insertion_casier(g[j]);
        j++;
    }
}

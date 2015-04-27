# include "Sable.h"

/**  Gestion de la simulation du sable */


////////////// SIMULATION ET FONCTIONS AUXILLIAIRES //////////////////////

/* remise a zero de f[] et m[]   pour tous les grains en contacts*/
void Sable::reinitialise_efforts_grains() {
    Contact* curs = c;
    while (curs != NULL) {
        curs->reinitialise_efforts_grains(); // remise a zero des efforts des grains
        curs = curs->next;
    }
}


/* Etape elementaire de la simulation à un pas de temps */
int Sable::etape_simulation(int compteur, StockContacts* stock, double dt, int frequence_affichage, string* path, string* path2) {

///// AFFICHAGE POSITION CORRESPONDANT A INSTANT PRECEDENT

    /* Affichage periodique de l'image + refresh stock*/
    if(compteur%frequence_affichage ==0){
        afficher_image(compteur, stock, path, path2);
    }


//// REINITIALISATION

    /* remise a zero de f[] et m[]   pour tous les grains */
    reinitialise_efforts_grains();

    /* Update des contacts*/
    suppression_contacts_ouverts(stock); // retrait des CONTACTS OUVERTS (F_n, F_s)
    cherche_nouveaux_contacts_pavage(stock, dt); // nouveaux contacts pavage
    update_efforts_contacts(dt); // Mise a jour effort de TOUS CONTACTS (F_n et F_s)

    /* Update des efforts des grains*/
    update_efforts_grains();

    /* Deplacement => update position des grains*/
    deplacement(dt); // Deplacement de TOUS LES GRAINS en fonjction de f[] et m[]

    /* Mise a jour pavage (c'est a dire, la grille) => grains affectes aux bons paves */
    update_grille();

    compteur ++;

    return compteur;
}

/* efface gnutest + vide stock contacts, contacts du sable, et pavage */
 void Sable::effacements(string* path, StockContacts* stock) {

    system( ("del " + *path + "gnutest").c_str() ); //WINDOWS :
    //LINUX: system( ("rm " + *path + "gnutest").c_str() );

    stock->vide_stock(); // vidage du stock de contacts
    delete stock;

    vide_contacts_sable(); // vidage des contacts du sable

    delete grille;   // vidage du pavage
}

/* Creation animation + suppression des png */
void Sable::genere_animation(string* path) {
    string cvertPng =  "convert -delay 5 -loop 0 ";
    cvertPng += *path;
    cvertPng += "*.png";
    cvertPng += " animation.gif ";
    string delPng = "del ";  // WINDOWS
    //LINUX: string delPng = "rm ";
    delPng += *path;
    delPng += "*.png";
    system(cvertPng.c_str() );
    system(delPng.c_str() );
    // LINUX : system("animate animation.gif");
}

/* Lancement d'une simulation */
void Sable::simulation(double duree , double dt , double coord_min[3], double coord_max[3],
                       int frequence_affichage, string *path, string *path2){

    cout << "\n---------- Fonction Simulation ---------- " << endl;

    if (g[0] == NULL ) { // tas de sable vide

        cout << "Tas de sable vide: fin de la simulation" << endl;

    } else {

        // Suppression des png et fichiers segment et contour precedents
        // => Precaution pour eviter les bugs
        /* WINDOWS */
        system( ("del " + *path + "*.png").c_str() );
        system( ("del " + *path + "segment").c_str() );
        system( ("del " + *path + "contour").c_str() );
        system( ("del " + *path + "gnutest").c_str() );

        /*LINUX
        system( ("rm " + *path + "*.png").c_str() );
        system( ("rm " + *path + "segment").c_str() );
        system( ("rm " + *path + "contour").c_str() );
        system( ("rm " + *path + "gnutest").c_str() ); */

        int compteur = 0;
        double temps = 0.0;
        StockContacts* stock = new StockContacts();
        // Pavage
        double r_max = rayon_grain_max();
        grille = new Pavage(coord_min, coord_max, r_max);
        initialise_grille(); // affectation INITIALE des grains au pavage (numero de pave affecte)

        for (temps = 0; temps<= duree; temps+=dt) {
            compteur = etape_simulation(compteur, stock, dt, frequence_affichage, path, path2);
        }

        effacements(path, stock);
        genere_animation(path);
        cout << "FIN SIMULATION " << endl;
    }
}

////////////////////////////////////////////////////////////////


/* Deplacement de tous les grains lors d'un pas de temps
    + update de tous les casiers */
void Sable::deplacement(double dt) {
    int i = 0;
    while (g[i] != NULL) {
        if(g[i]->paroie == false){
            g[i]->deplacement(dt); // update positions grains
        }
        i++;
    }
}


/* Distance au carre entre 2 grains */
double Sable::distance_carre(Grain* g1, Grain* g2){
    double dist = pow((g1->X[0]-g2->X[0]),2) + pow((g1->X[1]-g2->X[1]),2) + pow((g1->X[2]-g2->X[2]),2);
    return dist;
}



//////////////// CALCULER L'ENERGIE DU SABLE ////////////////


/* Calcule l'energie du sable */
double Sable::eg_sable() {
    double eg = 0.0;

    int i = 0;
    while (g[i] != NULL) {
        eg += g[i]->eg_grain();
        i++;
    }

    return eg;
}

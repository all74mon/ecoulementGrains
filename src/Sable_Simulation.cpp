# include "Sable.h"

/**  Gestion de la simulation du sable */


/* Lancement d'une simulation */
void Sable::simulation(double duree , double dt , int frequence_affichage,
                       string *path, string *path2){

    if (g[0] == NULL ) { // tas de sable vide

        cout << "Tas de sable vide: fin de la simulation" << endl;

    } else {
        cout << "\n\n\n\nDEBUG  ---------- Fonction Simulation ---------- " << endl;

        int compteur = 0;
        double temps = 0.0;

        /* Stock de contacts */
        StockContacts* stock = new StockContacts();

        for ( temps = 0; temps<= duree; temps+=dt){

            cout<<"DEBUG : Simulation -- iteration numero " << compteur <<endl;

            suppression_contacts_ouverts(stock); // parcours de la liste + calcul de distance
            cherche_nouveaux_contacts(stock, dt); // sys casiers + examiner couples de grains
            update_liste_contacts(dt); // sys casiers + examiner couples de grains

            /* Affichage periodique de l'image + refresh stock*/
            if(compteur%frequence_affichage ==0){
                afficher_image(compteur, stock, path, path2);
            }
            deplacement(dt);  // deplacement d'un pas de temps
            compteur ++;
        }

        /* Effacement de gnutest*/
        system( ("rm " + *path + "gnutest").c_str() );

        /* Vidage du stock et de la liste de contacts du sable */
        stock->vide_stock();
        delete stock;
        vide_contacts_sable();


        /* Creation du gif*/
        string cvertPng =  "convert -delay 5 -loop 0 ";
        cvertPng += *path;
        cvertPng += "*.png";
        cvertPng += " animation.gif ";
        string delPng = "rm ";
        delPng += *path;
        delPng += "*.png";
        system(cvertPng.c_str() );
        system(delPng.c_str() );

        system("animate animation.gif");
        cout << "DEBUG : FIN SIMULATION " << endl;

    }
}

/* Deplacement de tous les grains lors d'un pas de temps */
void Sable::deplacement(double t){
    int i = 0;
    while (g[i] != NULL) {
        if(g[i]->paroie == false){
            g[i]->deplacement(t);
        }
        i++;
    }
}


/* Distance au carre entre 2 grains */
double Sable::distance_carre(Grain* g1, Grain* g2){
    double dist = pow((g1->X[0]-g2->X[0]),2) + pow((g1->X[1]-g2->X[1]),2) + pow((g1->X[2]-g2->X[2]),2);
    return dist;
}

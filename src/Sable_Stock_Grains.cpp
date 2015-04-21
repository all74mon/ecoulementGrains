#include "Sable.h"

/** Fonction de Sable liées au stock de grains
et à la gestion de ce stock*/

/* Affichage des grains contenus dans le casier */
void Sable::affiche_casiers() {
    casiers->affiche_casiers();
}

/* Affichage de la liste des grains present dans le tas + positions eventuellement  */
void Sable::affiche_grains_sable(bool pos) {
    cout << "\n\n\n ----- Affichage liste de grains ----- " << endl;
    int z = 0;
    while (g[z] != NULL) {
        g[z]->afficher_grain(pos);
        z++;
    }
}

/* Ajout d'un grain au tas de sable */
void Sable::ajout_grain(bool PAROIE, const double coord[3], const double v[3] , const double TETA[3] , const double OMEGA[3] , double rayon, double masse, double MOMENT_INERTIE, double kn, double ks, double cn, double cs ){
    static int ifin=0;
    if (ifin>taille-1) {
        //cout << "nombre max de grain atteint !!!" <<endl;
    }
    Grain* nouveauGrain = new Grain(PAROIE, coord, v , TETA , OMEGA , rayon , masse , MOMENT_INERTIE , kn , ks , cn , cs );

    // Ajout nouveau grain dans le tas
    g[ifin] = nouveauGrain;
    ifin++;
    cout << "DEBUG: ajoute Grain: grain ajoute " << nouveauGrain << "---- valeur de ifin: " << ifin << endl;

}


/* Ajout d'un bloc de grains */
void Sable::ajout_bloc(int nombre, const double X[3], const double v[3], const double teta[3], const double omega[3], double kn, double ks, double rayon, double epsi){ //coordonnes du grain en haut a gauche
    cout<<"debut fonction ajout_bloc"<<endl;

    /* Copie de X dans coord locale => eviter les problemes de pointeurs */
    double coord[3]; // coordonnees
    for(int i = 0; i <= 2; i++) {
        coord[i] = X[i];
    }

    int nbg_cote;
    int i = 2;
    bool verif = false;
    if(nombre > 100 || nombre < 2){
        cout<< "Nombre de grain par bloc : entre 2 et 100" <<endl;
    }
    else {
        while( (i < 11) && (verif == false) ){
            if(nombre < (i*i + 1)){
                nbg_cote = i;
                verif = true;
            }
            i++;
        }
        cout<<"DEBUG_SABLE : fin boucle : longueur du coté = " << nbg_cote <<endl;
        bool paroie = false;
        double m = 1;
        double J = 1;
        double cn = 0;
        double cs = 0;
        bool quitter = false;
        int compteur = 0;
        while (compteur<nombre-1){
            for (int j =0; j<nbg_cote; j++){
                for (int i = 0; i<nbg_cote;i++){
                    this->ajout_grain(paroie,coord,v,teta,omega,rayon,m,J,kn,ks,cn,cs);
                    coord[0] += 2*rayon + epsi ;
                    compteur++;
                    if(compteur==nombre){
                        quitter = true;
                    }
                    if(quitter){
                        break;
                    }
                }
                if(quitter){
                    break;
                }
                coord[0]-= (nbg_cote)*(2*rayon + epsi);
                coord[1] -= 2*rayon + epsi ;
            }
            if(quitter){
                break;
            }
        }
    }
}


// vider le stock de grains
void Sable::vide_grains() {
    if (g != NULL) {
        for (int i = 0; i <= taille - 1; i++) {
            delete g[i];
        }
        delete g;
        g = NULL;
    }
}

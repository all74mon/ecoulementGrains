#include "Sable.h"

//////////// GESTION DE LA GRILLE (PAVAGE) ///////////////

/* Initialise grille : met tous les grains dedans */
void Sable::initialise_grille() {

    int i = 0;

    while(g[i] != NULL && i <= taille - 1 ) {
        grille->initialise_pavage(g[i]);
        i++;
    }

}

/* Update de la grille */
void Sable::update_grille() {

    int i = 0;
    while(g[i] != NULL && i <= taille - 1 ) {
        grille->update_pave_grain(g[i]);
        i++;
    }
}

/* Affichage de la grille */
void Sable::affiche_grille() {
    grille->affichage_pavage();
}

/* Rayon maximal des grains */
double Sable::rayon_grain_max() {
    int i = 0;
    double rmax = 0.0;
    while(g[i] != NULL) {
        if (g[i]->rayon > rmax)
            rmax = g[i]->rayon;
        i++;
    }
    return rmax;
}

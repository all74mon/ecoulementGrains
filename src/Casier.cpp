#include "Casier.h"

Casier::Casier(int XCASE, int YCASE, int ZCASE)
{
    xcase = XCASE;
    ycase = YCASE;
    zcase = ZCASE;

    suivant = NULL;
    g = NULL;
}


/* Detruire le casier => vidage de la liste des grains automatiquement */
Casier::~Casier()
{
    vide_casier();
    suivant = NULL;
}


/* Renvoie 0 => casier numero (x, y, z) trouve */
/* Renvoie 1 => casier numero aller au suivant */
/* Renvoie -1 => inserer a cet endroit là */
int Casier::exam_casier(int XCASE, int YCASE, int ZCASE) {
    if (xcase == XCASE && ycase == YCASE && zcase == ZCASE) {
        return 0;
    } else if ( xcase < XCASE || (xcase == XCASE && ycase < YCASE)
                || (xcase == XCASE && ycase == YCASE && zcase < ZCASE) ) { // avancer
        return 1;
    } else { // insertion
        return -1;
    }
}

/* vidage du casier => Ne plus POINTER sur les grains, NE LES DELETE PAS
 + remet à NULL les pointeurs &s des Grains */
void Casier::vide_casier() {
    Grain* temp = NULL;
    while (g != NULL) {
        temp = g;
        g = g->s; // avancer
        temp->s = NULL;
    }
}

/* ajout d'un grain au casier courant */
void Casier::ajout_grain(Grain* G) { // insertion en-tête
    Grain* temp = g;
    g = G;
    g->s = temp;
}


/* affichage du casier */
void Casier::affiche_casier() {
    cout << "---> Affichage casier numero : " << xcase
        << " " << ycase << " " << zcase << endl;
    Grain* curs = g;

    while (curs != NULL) {
        curs->afficher_grain(false);
        curs = curs->s;
    }
}

#include "Sable.h"

/** Constructeur et destructeur du sable */

Sable::Sable(int CAPA){
    taille = CAPA;
    g = new Grain*[taille];
    for (int i=0; i < taille; i++) {
        g[i] = NULL;
    }

    c = NULL;
    eg = 0.0;
    grille = NULL;
}

Sable::~Sable(){
    vide_grains(); // vidage du stock de grains
}

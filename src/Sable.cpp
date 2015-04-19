#include "Sable.h"

/* Constructeur et destructeur du sable */
Sable::Sable(int CAPA){
    taille = CAPA;
    g = new Grain* [taille];
    for (int i = 0; i <= taille - 1; i++) {
        g[i] = NULL;
    }
    c = NULL;
    casiers = new ListeCasiers();
}

Sable::~Sable(){
    vide_grains(); // vidage du stock de grains
}

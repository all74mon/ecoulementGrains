#ifndef CASIER_H
#define CASIER_H

#include "Grain.h"
/** Casiers = listes de grains anotées du
numero de case dans le pavage */

class Casier
{
    public:
        Casier(int XCASE, int YCASE, int ZCASE); // coordonnees du casier);
        virtual ~Casier();
        void vide_casier(); // vider le casier de tous ses grains (suppression)
        int exam_casier(int XCASE, int YCASE, int ZCASE); // examen du casier pour ajout grain
        void ajout_grain(Grain* G); // insertion grain
        void affiche_casier();

    /* Attributs */
        int xcase; // coordonnees case
        int ycase;
        int zcase;
        Casier* suivant; // pointeur sur element suivant
        Grain* g; // pointeur sur la liste de grains contenus
};

#endif // CASIER_H

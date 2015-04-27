#ifndef PAVAGE_H
#define PAVAGE_H

#include <math.h>
#include "Grain.h"
#include "stdlib.h"
#include <fstream>
#include <sstream>

/** realise le pavage (stockage des grains dans des casiers numerotes) pour la recherche
    optimale des voisins */

class Pavage
{
    public:
        // Creation et destruction
        Pavage(double COORD_MIN[3], double COORD_MAX[3] , double RMAX);
        virtual ~Pavage();
        void vidage_pavage();
        void vidage_casier(Grain** CASIER);
        // Initialisation + Mise a jour du pavage
        void initialise_pavage(Grain* G);
        void update_pave_grain(Grain* G);
        void inserer_grain_pave(Grain *G, int PAVE[3]);
        void calcul_nv_pave(Grain* G, int NV_PAVE[3]);
        bool change_pave_grain(Grain* G, int NV_PAVE[3]);
        // Affichage du pavage
        void affichage_pavage();
        void affichage_casier(Grain* CASIER);
        // Dresse la liste des voisins
        void dresse_liste_voisins( Grain* G, Grain* LISTE_VOISINS[100] );
        // ajout voisins de G sauf G lui-meme
        int ajout_grains_liste_voisins(Grain* G, Grain* LISTE_VOISINS[100],
                                        int X, int Y, int Z, int nb_voisins);


    /* Attributs */
        double coord_min[3];   // coord min du point delimitant la paroi
        double coord_max[3];   // coord max du point delimitant la paroi
        double cote_pave;      // dimensions des paves elementaires
        int nb_paves[3];       // nb paves elementaires par cotes
        Grain**** casier;      // caisers (listes de grains)
};

#endif // PAVAGE_H

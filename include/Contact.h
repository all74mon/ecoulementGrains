#ifndef CONTACT_H
#define CONTACT_H

#include "Grain.h"

class Contact
{
    public:
        Contact();
        virtual ~Contact(){};
        void initialise_contact(Grain* G1, Grain* G2, double dt);
        void met_a_jour_efforts(double distance, double dt);
        void reinitialise_efforts_grains();
        void MAJ_efforts_grains();
        void reinitialise_efforts_contact();
        void produit_vectoriel(double V1[3], double V2[3], double tab[3]);


    // attributs
        double n[3];        //Vecteurs unitaire des positions relatives entre les deux grains
        double s[3];
        double v[3];        //Vecteur vitesse relative des deux grains
        double vs[3];       //Vecteur vitesse relative dans la direction tangentielle
        double deltan;   //Vecteur deplacement relatif dans la direction normale
        double deltas[3];   //Vecteur deplacement relatif dans la direction tangentielle
        double delta_fn[3];      //vecteur variation force normale du contact
        double delta_fs[3];      //vecteur variation force de cisaillement du contact

        double fn[3]; // force resultante
        double fs[3];
        int nb_dt;      //Durée du contact en nombre de pas de temps

        Grain* g1;
        Grain* g2;
        // PAROI CYLINDIRQUE
        // Paroi * paroi; // A AJOUTER
        Contact* next;         // contact suivant dans la liste (cf classe StocksContacts)

};

#endif // CONTACT_H

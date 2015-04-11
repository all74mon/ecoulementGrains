#ifndef CONTACT_H
#define CONTACT_H

#include "Grain.h"

class Contact
{
    public:
        Contact();
        virtual ~Contact(){};
        void initialiseContact(Grain* G1, Grain* G2, double distance, double dt);
        void metAjourEfforts(double distance, double dt);

    //private:
        double n[3];        //Vecteurs unitaire des positions relatives entre les deux grains
        double s[3];
        double v[3];        //Vecteur vitesse relative des deux grains
        double vs[3];       //Vecteur vitesse relative dans la direction tangentielle
        double deltan[3];   //Vecteur deplacement relatif dans la direction normale
        double deltas[3];   //Vecteur deplacement relatif dans la direction tangentielle
        double F_n[3];      //vecteur force normale
        double F_s[3];      //vecteur force de cisaillement

        Grain* g1;
        Grain* g2;
        Contact* next;         // contact suivant dans la liste (cf classe StocksContacts)

};

#endif // CONTACT_H

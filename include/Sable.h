#ifndef SABLE_H
#define SABLE_H
#include "Grain.h"
#include <fstream>
#include<cmath>
#include <math.h>
#include <stdlib.h>
#include "Contact.h"
#include "StockContacts.h"
#include <sstream>

#define MAXCONTACTS 20


using namespace std;

class Sable
{
    public:
        Sable(int nb);
        virtual ~Sable();
        void ajouteGrain(bool PAROIE, double coord[3], double v[3] ,double TETA[3] , double OMEGA[3] , double rayon, double masse, double MOMENT_INERTIE, double kn, double ks, double cn, double cs);
        void simulation(double duree, double dt);
        void deplacement(double dt);
        void ecriture_fichier();
        double distance(Grain* g1 , Grain* g2);
        //void creation_contact();
        void simulation(double duree , double dt , int frequence_affichage);
        void ajout_Bloc(int nombre, double coord[3], double v[3], double rayon, double epsi);
        void afficher_image(int t, StockContacts* stock);
            // Gestion de la liste de contacts
        void suppressionContactsOuverts(StockContacts* stock); // parcours de la liste + calcul de distance
        void chercheNouveauxcontacts(StockContacts* stock, double dt); // sys casiers + examiner couples de grains
        void updateListeContacts(double dt); // mettre a jour tous les contacts
        bool contactPresent(Grain* g1, Grain* g2);
        void insertionContact(Grain* g1, Grain* g2, StockContacts* stock, double distance, double dt);
        void afficherListeSable(); // affichage
            // fonction de vidage
        void videContactsSable();
        void videGrainsSable();
    //private:
        int nbg;
        Grain** g;
        Contact* c;  // liste de contacts
        int nbcMax; // nombre de contacts ayant ete inseres : debug

};

#endif // SABLE_H

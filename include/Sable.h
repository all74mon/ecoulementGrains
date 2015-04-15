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



using namespace std;

class Sable
{
    public:
        // Constructeur et destructeur de Sable
        Sable(int CAPA);
        virtual ~Sable();

        // Gestion du stock de grains
        void ajout_bloc(int nombre, double coord[3], double v[3], double rayon, double epsi);
        void ajout_grain(bool PAROIE, double X[3], double V[3] ,double TETA[3] , double OMEGA[3] , double rayon, double masse, double MOMENT_INERTIE, double kn, double ks, double cn, double cs);
        void vide_grains();

        // simulation + mise à jour des grains
        void simulation(double duree , double dt , int frequence_affichage, string *path, string *path2);
        void deplacement(double dt);
        double distance_carre(Grain* g1 , Grain* g2); // distance au carre

        // Gestion des fichiers
        void ecriture_fichier(string *path);
        void ecriture_grain(Grain* g, string fichContour, string fichSegment);
        void ecrire_gnuplot(string gnupl, char* image, int largeur, string *path);
        void afficher_image(int t, StockContacts* stock, string *path, string *path2);

        // Gestion de la liste de contacts
        void suppression_contacts_ouverts(StockContacts* stock); // parcours de la liste + calcul de distance
        void cherche_nouveaux_contacts(StockContacts* stock, double dt); // sys casiers + examiner couples de grains
        void update_liste_contacts(double dt); // mettre a jour tous les contacts
        bool contact_present(Grain* g1, Grain* g2);
        void insertion_contact(Grain* g1, Grain* g2, StockContacts* stock, double distance, double dt);
        void affiche_contacts_sable(); // affichage Contacts Sable
        void vide_contacts_sable(); // fonction de vidage


            // attributs
        int taille;    // capacité du tas de sable
        Grain** g;     // grains contenus dans tas de sable
        Contact* c;    // liste de contacts

};

#endif // SABLE_H

#ifndef SABLE_H
#define SABLE_H
#include "Grain.h"
#include <fstream>
#include<cmath>
#include <math.h>
#include <stdlib.h>
#include "Contact.h"
#include "StockContacts.h"
#include "Pavage.h"
#include <sstream>



using namespace std;

class Sable
{
    public:

        // Constructeur et destructeur de Sable
        Sable(int CAPA);
        virtual ~Sable();

        // Gestion du stock de grains
        void affiche_casiers();
        void affiche_grains_sable(bool pos);

        void ajout_bloc(int nombre, double epsi, const double X[3], const double v[3], const double teta[3],
                        const double omega[3], double rayon,double masse,double J,double kn,
                        double ks,double cn,double cs);

        void ajout_grain(bool PAROIE, const double X[3], const double V[3] , const double TETA[3] ,
                         const double OMEGA[3] , double rayon, double masse, double MOMENT_INERTIE,
                         double kn, double ks, double cn, double cs);

        void vide_grains();

        // Gestion des efforts des grains
        void update_efforts_grains();
        void reinitialise_efforts_grains();

        // Calcul distance au carre entre deux grains
        double distance_carre(Grain* g1 , Grain* g2); // distance au carre

        // Deplacement des grains à chaque DT
        void deplacement(double dt);

        // Calcul de l'energie du sable
        double eg_sable();


        // simulation
        void simulation(double duree, double dt, double coord_min[3], double coord_max[3],
                        int frequence_affichage, string *path, string *path2);
        int etape_simulation(int compteur, StockContacts* stock, double dt,  int frequence_affichage, string* path, string* path2);

        // Gestion des fichiers
        void ecriture_fichier(string *path);
        void ecriture_grain(Grain* g, string fichContour, string fichSegment);
        void ecrire_gnuplot(string gnupl, char* image, double largeur, string *path);
        void effacements(string* path, StockContacts* stock);
        void afficher_image(int t, StockContacts* stock, string *path, string *path2);
        void genere_animation(string* path);

        // Gestion de la liste de contacts
        void suppression_contacts_ouverts(StockContacts* stock); // parcours de la liste + calcul de distance
        void cherche_nouveaux_contacts(StockContacts* stock, double dt); // sys casiers + examiner couples de grains
        void update_efforts_contacts(double dt); // mettre a jour efforts de tous les contacts
        bool contact_present(Grain* G1, Grain* G2);
        void insertion_contact(Grain* g1, Grain* g2, StockContacts* stocks, double dt);
        void affiche_contacts_sable(); // affichage Contacts Sable
        void vide_contacts_sable(); // fonction de vidage

        // Gestion de la liste contacts avec PAVAGE
        void cherche_nouveaux_contacts_pavage(StockContacts* stock, double dt);
        void creer_nouveaux_contacts(StockContacts* STOCK, double DT,
                                     Grain* G, Grain* LISTE_VOISINS[100]);

        // Fonction de gestion de la grille (pavage)
        void initialise_grille();
        void update_grille();
        void affiche_grille();
        double rayon_grain_max();

        /* attributs */
        int taille;         // Capacite du tas de sable
        Grain** g;          // Stock de grains
        Contact* c;        // liste de contacts
        // LISTE DE PAROIS A AJKOUTER ?
        double eg;         // energie totale du tas
        Pavage* grille;    // grille du pavage

};

#endif // SABLE_H

#ifndef GRAIN_H
#define GRAIN_H

#include <iostream>
using namespace std;

class Grain
{
    public:
        Grain(bool PAROIE, const double coord[3], const double V[3],const double TETA[3] , const double OMEGA[3] , double r, double MASSE, double MOMENT_INERTIE, double kn, double ks, double cn, double cs);
        ~Grain();
        void deplacement(double dt);
        void afficher_positions();
        void afficher_grain(bool pos);
        double eg_grain(); // Calcul de l'energie du grain
            // getters
        Grain* get_suivant();
        Grain* get_precedent();
        int* get_x_case();
            // setters
        void set_suivant(Grain* S);
        void set_precedent(Grain* P);
        void set_x_case(int X_CASE[3]);


    /* attributs */
        int id; // numero du grain (dans l'ordre de l'insertion dans le tas
        bool paroie;
        double rayon;
        double masse;
        double X[3];        //Coordonnées
        double v[3];        //Vitesees
        double teta[3];     //Angles
        double omega[3];    //Vitesses angulaires
        double J;           //Moment d'Inertie
        double f[3];        //Forces
        double m[3];        //Moments
        double C;           //Coefficient d'amortissement de trasistion globale
        double C2;          //Coefficient d'amortissement de rotation globale
        double kn;          //Constante d'elasticite dans la direction normale
        double ks;          //Constante d'elasticite dans la direction tangente
        double cn;          //Coefficient normal d'amortissement
        double cs;          //Coefficient tangentiel d'amortissement

    private:
        // Liste du pavage
        Grain* p;       // Pointeur sur le grain precedent
        Grain* s;       // Pointeur sur le grain suivant
        int x_case[3]; // coordonnees de la case dans pavage
};

#endif // GRAIN_H

#ifndef GRAIN_H
#define GRAIN_H

#include <iostream>
using namespace std;

class Grain
{
    public:
        Grain(bool PAROIE, const double coord[3], const double V[3],const double TETA[3] , const double OMEGA[3] , double r, double MASSE, double MOMENT_INERTIE, double kn, double ks, double cn, double cs);
        ~Grain();
        void deplacement(double dt, double pavage[3]);
        void afficher_positions();
        void afficher_grain(bool pos);

    // attributs:
        int id; // numero du grain (dans l'ordre de l'insertion dans le tas
        int x_case[3]; // coordonnees de la case dans pavage

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

        Grain* s;          //Pointeur sur le grain suivant
};

#endif // GRAIN_H

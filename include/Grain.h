#ifndef GRAIN_H
#define GRAIN_H

#include <iostream>
using namespace std;

class Grain
{
    public:
        Grain(bool PAROIE, double coord[3], double V[3],double TETA[3] , double OMEGA[3] , double r, double m, double MOMENT_INERTIE, double kn, double ks, double cn, double cs);
        ~Grain();
        void deplacement(double t);
        void afficher_positions();

    //private:
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
        Grain * p;          //Pointeur sur le grain prcedent
        Grain * s;          //Pointeur sur le grain suivant
        double C;           //Coefficient d'amortissement de trasistion globale
        double C2;          //Coefficient d'amortissement de rotation globale
        double kn;          //Constante d'elasticite dans la direction normale
        double ks;          //Constante d'elasticite dans la direction tangente
        double cn;          //Coefficient normal d'amortissement
        double cs;          //Coefficient tangentiel d'amortissement
};

#endif // GRAIN_H

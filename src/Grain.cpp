#include "Grain.h"
#include <cmath>
#include <math.h>
#include <iostream>
#include <fstream>
#define PI 3.14159265358979323846

#include <stdlib.h> // debug

    Grain::Grain(bool PAROIE ,const double coord[3] , const double V[3] , const double TETA[3] ,const double OMEGA[3] , double r ,double MASSE ,double MOMENT_INERTIE, double Kn, double Ks, double Cn, double Cs){
        // Affectation id
        static int num = 1;
        id = num;
        num++;

        // Paroi affectée
        paroie = PAROIE;

        // affectation coordonnees et caracteristiques
        for(int i =0; i<3;i++){
            X[i] = coord[i];
            x_case[i] = 0; // coordonnee de la case
            v[i] = V[i];
            teta[i] = TETA[i];
            omega[i] = OMEGA[i];
            f[i] = 0.0;
            m[i] = 0.0;
        }

        rayon = r;
        masse = MASSE;
        J = MOMENT_INERTIE;
        kn = Kn;
        ks = Ks;
        cn = Cn;
        cs = Cs;
        C = 0.0;
        C2 = 0.0;

        // Initialisation pointeurs
        s=NULL;
    }


    Grain::~Grain()
    {
    }


    void Grain::deplacement(double dt, double pavage[3]){
        int ic;
        double g[3] = {0.0, -9.81, 0.0};

        /* Update coordonnees grain + pavage */
        for (ic=0; ic<3; ic++) {
            X[ic] += dt * v[ic];
            x_case[ic] = floor( X[ic]/pavage[ic] );
        }

        for (ic=0; ic<3; ic++) {
            v[ic] += dt * ( f[ic]/masse + g[ic] );
            //cout<< " Valeur de la nouvelle vitesse du grain numero   "<< id  << " : "<< v[ic] <<endl;
        }

        // dTheta/dt = Omega
        for (ic=0; ic<3; ic++)
            teta[ic] += dt * omega[ic];

        // dOmega/dt = m/J
        for (ic=0; ic<3; ic++)
            omega[ic] += dt * m[ic]/J;

    }

    // afficher positions
   void Grain::afficher_positions(){
        cout << "position absolue:   ";
        cout<<"x = "<<X[0]<<" y = "<<X[1]<<" z = "<<X[2]<< endl;

        cout << "position dans pavage:  ";
        cout <<"x_case = "<<x_case[0]<<" y_case = "<<x_case[1]<<" z_case = "<<x_case[2]<< endl;
    }

    // affichage du grain + de ses positions si pos est true
    void Grain::afficher_grain(bool pos) {
        cout << "Grain numero:  " << id << endl;
        if (pos) {
            cout << "--->Affichage du pointeur sur grain suivant: ";
            cout << s << endl;
            cout << "---> affichage des positions: " ;
            afficher_positions();
        }
    }



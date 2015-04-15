#include "Grain.h"
#include <cmath>
#include <math.h>
#include <iostream>
#include <fstream>
#define PI 3.14159265358979323846

#include <stdlib.h> // debug

    Grain::Grain(bool PAROIE ,double coord[3] , double V[3] ,double TETA[3] , double OMEGA[3] , double r ,double m ,double MOMENT_INERTIE, double Kn, double Ks, double Cn, double Cs){
        paroie = PAROIE;
        for(int i =0; i<2;i++){
            X[i] = coord[i];
            v[i] = V[i];
            teta[i] = TETA[i];
            omega[i] = OMEGA[i];
        }
        X[0] = coord[0];
        X[1] = coord[1];
        X[2] = coord[2];
        v[0] = V[0];
        v[1] = V[1];
        v[2] = V[2];
        rayon = r;
        masse = m;
        J = MOMENT_INERTIE;
        kn = Kn;
        ks = Ks;
        cn = Cn;
        cs = Cs;
   // Initialisation pointeurs
        p=NULL;
        s=NULL;

    }


    Grain::~Grain()
    {
    }

    void Grain::deplacement(double dt){
        int ic;
        double g[3] = {0.0, -9.81, 0.0};

        for (ic=0; ic<3; ic++)
            X[ic] += dt * v[ic];


        for (ic=0; ic<3; ic++)
            v[ic] += dt * ( f[ic]/masse + g[ic] );


        // dTheta/dt = Omega
        for (ic=0; ic<3; ic++)
            teta[ic] += dt * omega[ic];



        // dOmega/dt = m/J
        for (ic=0; ic<3; ic++)
            omega[ic] += dt * m[ic]/J;
    }

   void Grain::afficher_positions(){
        cout<<"x = "<<X[0]<<" y = "<<X[1]<<" z = "<<X[2]<< endl;
    }


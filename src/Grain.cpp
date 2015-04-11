#include "Grain.h"
#include <cmath>
#include <math.h>
#include <iostream>
#include <fstream>
#define PI 3.14159265358979323846

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
        // dX/dt = v
        // X - Xp = v * dt
       // cout<<"avant X[1]= "<<X[1]<<endl;
        for (ic=0; ic<3; ic++) X[ic] += dt * v[ic];
        //cout<<"apres X[1]= "<<X[1]<<endl;
        /*X[0] += t*v[0] + pow(t,2)*f[0]/masse;
        X[1] += t*v[1] + pow(t,2)*f[1]/masse - 9.81*pow(t,2)/2;
        X[2] += t*v[2] + pow(t,2)*f[2]/masse;*/
        //cout<< "DEBUG_GRAIN : affichage de X[" <<endl;

        // dv/dt = F/masse
        // V - Vp = F/masse * dt
        for (ic=0; ic<3; ic++) v[ic] += dt * ( f[ic]/masse + g[ic] );
        /*v[0] = v[0] + t*f[0]/masse;
        v[1] = v[1] + t*f[1]/masse - 9.81*t;
        v[2] = v[2] + t*f[2]/masse;*/

        // dTheta/dt = Omega
        for (ic=0; ic<3; ic++) teta[ic] += dt * omega[ic];
        /*teta[0] +=  t*omega[0] + pow(t,2)*m[0]/J;
        teta[1] +=  t*omega[1] + pow(t,2)*m[1]/J;
        teta[2] +=  t*omega[2] + pow(t,2)*m[2]/J;*/


        // dOmega/dt = m/J
        for (ic=0; ic<3; ic++) omega[ic] += dt * m[ic]/J;
    }

   void Grain::afficher_positions(){
        cout<<"x = "<<X[0]<<" y = "<<X[1]<<" z = "<<X[2]<< endl;
    }

    void Grain::ecriture_fichier(string fichContour, string fichSegment){

        ofstream contour(fichContour.c_str(), ios::out | ios::app);  // ouverture en écriture avec effacement du fichier ouvert
        if(contour){  // si l'ouverture a réussi
                contour << X[0] << " " << X[1] << " " << rayon << endl;
                contour.close();  // on ferme le fichier
        }
        else{
                cerr << "Impossible d'ouvrir le fichier !" << endl;
            }
        ofstream segment(fichSegment.c_str(), ios::out | ios::app);  // ouverture en écriture avec effacement du fichier ouvert
        if(segment) {  // si l'ouverture a réussi
            segment << X[0] << " " << X[1] << endl;
            segment << X[0] + (rayon*cos(teta[0])) << " " << X[1] + (rayon*sin(teta[1])) << endl << endl;
            segment.close();  // on ferme le fichier
        }
    else{
        cerr << "Impossible d'ouvrir le fichier !" << endl;
        }

}

void Grain::ecrireGnuplot(string gnupl, char* image, int largeur)
{
    ofstream gnu(gnupl.c_str(), ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
    if(gnu)  // si l'ouverture a réussi
    {
        gnu << "set term pngcairo" << endl;
        gnu << "set output " << image << endl; // écriture
        gnu << "set xrange [" << -largeur << ":" << largeur << "]" << endl;
        gnu << "set yrange [" << -3*largeur/4 << ":" << 3*largeur/4 << "]" << endl;
        gnu << "plot 'contour' using 1:2:3 with circles, 'segment' using 1:2 with lines" << endl;
        gnu.close();  // on ferme le fichier
    }
    else
        cerr << "Impossible d'ouvrir le fichier !" << endl;
}



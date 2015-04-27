#include <iostream>
#include<vector>
#include<cmath>
#include "Sable.h"
#define PI 3.14159265358979323846
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <sstream>
#include <string>


using namespace std;

int main()
{
    int freq_affichage = 25;
    bool paroie = false;
    double r,m,J,kn,ks,cn,cs;
    double X[3],v[3],teta[3],omega[3];

    /* Declaration du tas de sable*/
    Sable sable(71); // taille max du sable 71


    /* Premier Grain : descend*/
    X[0] = -0.1;
    X[1] = 0.1;
    X[2] = 0;
    v[0] = 1;
    v[1] = 0;
    v[2] = 0;
    teta[0] = 0;
    teta[1] = 0;
    teta[2] = 0;
    omega[0] = 0;
    omega[1] = 0;
    omega[2] = PI/8.;
    r = 0.01;
    m = 1000 *  4./3.*3.1415*pow(r,3.);
    J = 2./5. * m * pow(r,2.);
    kn = 1e5;
    ks = kn;
    cn = 0;
    cs = 0;
    sable.ajout_grain(paroie, X,v,teta,omega,r,m,J,kn,ks,cn,cs);
    //sable.ajout_bloc(5, r/5.,X, v,teta,omega, r,m,J,kn,ks,cn,cs);

    /* Premier Grain : descend*/
    X[0] = 0.1;
    X[1] = 0.1;
    X[2] = 0;
    v[0] = -1;
    v[1] = 0;
    v[2] = 0;
    teta[0] = 0;
    teta[1] = 0;
    teta[2] = 0;
    omega[0] = 0;
    omega[1] = 0;
    omega[2] = PI/8.;
    r = 0.01;
    m = 1000 *  4./3.*3.1415*pow(r,3.);
    J = 2./5. * m * pow(r,2.);
    kn = 1e5;
    ks = kn;
    cn = 0;
    cs = 0;
    sable.ajout_grain(paroie, X,v,teta,omega,r,m,J,kn,ks,cn,cs);
    //sable.ajout_bloc(5, r/5.,X, v,teta,omega, r,m,J,kn,ks,cn,cs);

    /* Ajout du second bloc = monte */
    X[0] = 0;
    X[1] = -0.1;
    X[2] = 0;
    v[0] = 0;
    v[1] = 1;
    v[2] = 0;
    omega[0] = 0;
    omega[1] = 0;
    omega[2] = -PI/8;
    r = 0.01;
    m = 1000 *  4./3.*3.1415*pow(r,3.);
    J = 2./5. * m * pow(r,2.);
    sable.ajout_grain(paroie, X,v,teta,omega,r,m,J,kn,ks,cn,cs);
    //sable.ajout_bloc(5, r/5.,X, v,teta,omega, r,m,J,kn,ks,cn,cs);


    /* Ajout grain paroie */
    X[0] = 0 ;
    X[1] = 0 ;
    X[2] = 0 ;
    v[0] = 0 ;
    v[1] = 0 ;
    v[2] = 0 ;
    r = 0.2;
    m = 10000;
    teta[0] = PI/4;
    teta[1] = PI/4;
    omega[0] = -PI;
    omega[1] = -PI;
    paroie = true;
    sable.ajout_grain(paroie, X,v,teta,omega,r,m,J,kn,ks,cn,cs);



    /*Definition du path + Lancement simulation */
    // path: là ou il y a les fichiers segment, contour et gnutest
    //WINDOWS: string path ="D:\\UNIV_2014_2015_L3\\S2\\stage_applicatif\\ecoulementGrains\\";
    string path =""; //LINUX

    // path 2 : path avec " et "\\"
    /* WINDOWS:
    string path2 = "\"";  // guillemets => Ne pas toucher
	path2 += "D:\\";
	path2 += "\\";
	path2 += "UNIV_2014_2015_L3\\";
	path2 += "\\";
	path2 += "S2\\";
	path2 += "\\";
	path2 += "stage_applicatif\\";
	path2 += "\\";
	path2 += "ecoulementGrains\\";
	path2 += "\\"; */
    string path2 = "\""; //LINUX


    // Defintion paramètres de temps et de frequence pour simulation
    double duree = 1;
    double dt = 1e-6;
    double nbPdt = 50;
    freq_affichage = (int) ( duree/dt / (double)nbPdt );
    // Definition Parametres pavage
    double coord_min[3] = {-10, -10, 0};  // coord min et max du pavage
    double coord_max[3] = {10, 10, 0};
    sable.simulation(duree, dt , coord_min, coord_max, freq_affichage, &path, &path2);

    return 0;
}

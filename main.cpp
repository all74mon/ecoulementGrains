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
    int freq_affichage = 200;
    bool paroie = false;

    /* Declaration du tas de sable*/
    Sable sable(71); // taille max du sable 71

    /* Premier bloc : descend*/
    double X[3];
    X[0] = 1;
    X[1] = 30;
    X[2] = 0;
    double v[3];
    v[0] = 0;
    v[1] = 0;
    v[2] = 0;
    double teta[3];
    teta[0] = 0;
    teta[1] = 0;
    teta[2] = 0;
    double omega[3];
    omega[0] = PI/4;
    omega[1] = PI/4;
    omega[2] = 0;
    double r = 1.0;
    double m = 1;
    double J = 1;
    double kn = 20;
    double ks = 0.1;
    double cn = 0;
    double cs = 0;
    sable.ajout_bloc(10 ,X, v, teta, omega,1,0.5);

    /* Ajout du second bloc = monte */
    X[0] = 1;
    X[1] = -30;
    v[0] = 0;
    v[1] = 15;
    v[2] = 0;
    teta[0] = PI;
    teta[1] = PI;
    omega[0] = -PI/4;
    omega[1] = -PI/4;
    sable.ajout_bloc(3 ,X, v, teta, omega, 1.01, 0.5);


    /*Ajout grain paroie*/
    X[0] = 0 ;
    X[1] = 0 ;
    X[2] = 0 ;
    v[0] = 0 ;
    v[1] = 0 ;
    v[2] = 0 ;
    r = 30;
    m = 10000;
    kn = 500;
    ks = 0.1;
    teta[0] = PI;
    teta[1] = PI;
    omega[0] = 0;
    omega[1] = 0;
    paroie = true;
    sable.ajout_grain(paroie, X,v,teta,omega,r,m,J,kn,ks,cn,cs);

    /* Definition manuelle du pavage */
    double pavage[3];
    for (int i = 0; i < 3; i++) {
        pavage[i] = 10.0;
    }

    /*Definition du path + Lancement simulation */
        // path: là ou il y a les fichiers segment, contour et gnutest
    string path ="D:\\UNIV_2014_2015_L3\\S2\\stage_applicatif\\ecoulementGrains\\";

        // path 2 : path avec " et "\\"
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
	path2 += "\\";


    // PAS DE TEMPS 10^-3
    sable.simulation(5, 0.001 , freq_affichage, pavage, &path, &path2);


    return 0;
}

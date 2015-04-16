#include <iostream>
#include<vector>
#include<cmath>
#include "Sable.h"
#define PI 3.14159265358979323846
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <sstream>

#include <string> // debug


using namespace std;



int main()
{
    int freq_affichage = 1;
    bool paroie = false;

    /* Declaration du tas de sable*/
    Sable sable(71); // taille max du sable 71

    /* Premier bloc : descend */
    double X[3];
    X[0] = 0;
    X[1] = 20;
    X[2] = 0;
    double v[3];
    v[0] = 0;
    v[1] = -5;
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
   sable.ajout_bloc(10 ,X, v,1,0.5);   // Vitesse en parametre => faire tests


    /* Ajout du second bloc = monte */
    X[1] = -25;
    v[1] = 20;
    //cout << "DEBUG_MAIN: ajout du second bloc" << endl;
    sable.ajout_bloc(10 ,X, v, 1.01, 0.5);


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
    paroie = true;
    sable.ajout_grain(paroie, X,v,teta,omega,r,m,J,kn,ks,cn,cs);

    /*Definition du path + Lancement simulation*/

        // path: là ou il y a les fichiers segment, contour et gnutest
    string path ="";

        // path 2 : path avec " et "\\"
    string path2 = "\"";  // guillemets => Ne pas toucher



    sable.simulation(2, 0.25 , freq_affichage, &path, &path2);


    return 0;
}



//////   ANCIENS TESTS

    /* TESTS STOCK CONTACTS => MARCHE NICKEL
    cout << "\n---------- TEST DE STOCKS CONTACT ------------\n" << endl;
    int dt = 1; // pas de temps

    StockContacts* stock = new StockContacts();
    stock->afficherStock();
    Contact* c1 = stock->getContact();
    c1->initialiseContact(sable.g[0], sable.g[1], distance(sable.g[0], sable.g[1]), dt);
    stock->setContact(c1);

    stock->afficherStock();
    stock->videStock(2);
    Contact *c2 = stock->getContact();
    Contact *c3 = stock->getContact();
    Contact *c4 = stock->getContact();


    stock->afficherStock();
    stock->setContact(c2);
    stock->setContact(c3);
    stock->setContact(c4);
    stock->afficherStock();
    //stock->videStock(2);
    stock->afficherStock();
    cout << "----------Nombre de contacts stockes: " << stock->getNbContacts() << endl;

    //stock->videStock();
    stock->afficherStock();

    delete stock;
    stock = NULL; */

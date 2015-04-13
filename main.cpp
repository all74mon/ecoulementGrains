#include <iostream>
#include<vector>
#include<cmath>
#include "Sable.h"
#define PI 3.14159265358979323846
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <sstream>



using namespace std;



int main()
{
//    int frequence_affichage = 20;
    bool paroie = false;

/* Premier bloc : descend */
    double X[3];
    X[0] = 0;
    X[1] = 30;
    X[2] = 0;
    double v[3];
    v[0] = -1;
    v[1] = -5;
    v[2] = 0;
    double teta[3];
    double omega[3];
    double r = 1;
    double m = 1;
    double J = 1;
    double kn = 20;
    double ks = 0.1;
    double cn = 0;
    double cs = 0;
    Sable sable(21);
    cout<< "DEBUG_MAIN : sable declare"<<endl;
    sable.ajout_Bloc(20,X, v,1,0.5);   // Vitesse en parametre => faire tests
    cout << "DEBUG_MAIN :ajout_Bloc utilisé" <<endl;

    /*Ajout grain paroie*/
    X[0] = 0 ;
    X[1] = 20 ;
    X[2] = 0 ;
    v[0] = 0 ;
    v[1] = 0 ;
    v[2] = 0 ;
    r = 30;
    m = 10000;
    kn = 500;
    ks = 0.1;
    paroie = true;
    sable.ajouteGrain(paroie, X,v,teta,omega,r,m,J,kn,ks,cn,cs);

    cout << "DEBUG_MAIN :nombre de grains dans le sable : "<< sable.nbg <<endl;

    /*Lancement simulation*/
    sable.simulation(1 , 0.05 , 1);


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

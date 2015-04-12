#include "Sable.h"
#include <math.h>
#include<cmath>



Sable::Sable(int nb){
    nbg = nb;
    g = new Grain* [nb];
    c = NULL;
    nbcMax = 0;
}


Sable::~Sable(){}

void Sable::ajouteGrain(bool PAROIE,double coord[3], double v[3] ,double TETA[3] , double OMEGA[3] , double rayon, double masse, double MOMENT_INERTIE, double kn, double ks, double cn, double cs ){
    static int ifin=0;
    if (ifin>nbg-1) {
        //cout << "nombre max de grain atteint !!!" <<endl;
    }
    Grain* nouveauGrain = new Grain(PAROIE, coord, v , TETA , OMEGA , rayon , masse , MOMENT_INERTIE , kn , ks , cn , cs );
    g[ifin] = nouveauGrain;
    ifin++;

}

void Sable::deplacement(double t){
    for(int i = 0; i<nbg ; i++){
        if(g[i]->paroie == false){
            g[i]->deplacement(t);
        }
    }
}

void Sable::ecriture_fichier(){
    for(int i = 0 ; i < nbg ; ++i){
        g[i]->ecriture_fichier("contour","segment");
    }
}

double Sable::distance(Grain* g1, Grain* g2){
    double dist = pow((g1->X[0]-g2->X[0]),2) + pow((g1->X[1]-g2->X[1]),2) + pow((g1->X[2]-g2->X[2]),2);
    return dist;
}


void Sable::afficher_image(int t, StockContacts* stock){
            //cout << t << endl;
            int nbChiffres = 0;
            int t_temp = t;
            while (t_temp > 0){
                t_temp /= 10;
                nbChiffres++;
            }
            stringstream sstm;
            sstm << "\"output";
            for(int i = 1 ; i<= 5-nbChiffres ; i++){
                sstm << 0;
            }
            sstm << t << ".png\"";
            string ans = sstm.str();
            char * ans2 = (char *) ans.c_str();
            g[0]->ecrireGnuplot("gnutest",ans2,50);
            system("gnuplot gnutest");
            system("rm contour");
            system("rm segment");
            ecriture_fichier();
            /* Vider le stock de grains si nb de contacts depasse MAXCONTACTS */
            stock->videStock(MAXCONTACTS);
}

void Sable::simulation(double duree , double dt , int frequence_affichage){
    int compteur = 0;
    double temps = 0.;

    /* Stock de contacts */
    StockContacts* stock = new StockContacts();
    for ( temps = 0; temps<= duree; temps+=dt){
        //cout<<"temps, dt= "<<temps<<"   "<<dt<<endl;


        suppressionContactsOuverts(stock); // parcours de la liste + calcul de distance
        chercheNouveauxcontacts(stock, dt); // sys casiers + examiner couples de grains
        updateListeContacts(dt); // sys casiers + examiner couples de grains

        /* Affichage periodique de l'image + refresh stock*/
        if(compteur%frequence_affichage ==0){

            afficher_image(compteur, stock);
        }
        deplacement(dt);  // deplacement d'un pas de temps

        compteur ++;
    }

    /* Vidage du stock et de la liste de contacts du sable */
    stock->videStock();
    delete stock;
    stock = NULL;
    videContactsSable();
    videGrainsSable();


    system("convert -delay 5 -loop 0 *.png animation.gif ");
    system("rm *.png");
    system("animate animation.gif");
}


void Sable::suppressionContactsOuverts(StockContacts* stock) // parcours de la liste + calcul de distance
{
    if (c != NULL) { // liste a au moins 1 elt
        cout<<"JE PASSE dANS suppressionContactsOuverts"<<endl;
       static int nbSuppressions = 1;
        // traitement des n premiers contacts ouverts successifs
        while ( c != NULL && distance(c->g1, c->g2) > pow(c->g1->rayon + c->g2->rayon , 2) && (c->g1->paroie == false) && (c->g2->paroie == false) ) {
            Contact *curs = c;
            c = c->next;
            stock->setContact(curs); // rendre le contact au stock
            curs = NULL;
            nbSuppressions++;
                // DEBUG
            cout << "\n\n---- DEBUG: Suppression contact ouvert ----" << endl;
            stock->afficherListe();
            afficherListeSable();
        }

        // Traitement des contacts suivants
        if (c != NULL) { // c pointe sur un contact ferme
            Contact *curs = c->next;
            Contact *prec = c;

            while (curs != NULL) {

                if ( distance(curs->g1, curs->g2) > pow(curs->g1->rayon + curs->g2->rayon , 2) && (c->g1->paroie == false) && (c->g2->paroie == false) ) { //ouvert
                    Contact* old = curs;
                    curs = curs->next;
                    prec->next = curs;
                    stock->setContact(old); // rendre le contact au stock
                    old = NULL;
                    nbSuppressions++;
                        // DEBUG
                    cout << "\n\n---- DEBUG: Suppression contact ouvert ----" << endl;
                    stock->afficherListe();
                    afficherListeSable();
                } else { // ferme
                    prec = curs;
                    curs = curs->next;
                }

            }
        }

    }

}
// tester la presence d'un contact dans le sable
bool Sable::contactPresent(Grain* g1, Grain* g2) {

    if (c == NULL) { // liste vide
        return false;
    } else { // parcours de liste
        Contact* curs = c;
        while (curs != NULL) {
            if (curs->g1 == g1 && curs->g2 == g2) { // contact trouve
                return true;
            }
            curs = curs->next;
        }
        return false;
    }

}

// insertion contact dans le sable
void Sable::insertionContact(Grain* g1, Grain* g2, StockContacts* stock, double distance, double dt) {

    Contact* nveau = stock->getContact(); // demande d'un contact au stock
    nveau->initialiseContact(g1, g2, distance, dt);
    nveau->next = c;
    c = nveau;

    nbcMax += 1; // DEBUG

}

  //  on cherche les nouveaux contacts et on met à jour la liste chainée des contats du sable
void Sable::chercheNouveauxcontacts(StockContacts* stock, double dt) { // sys casiers + examiner couples de grains
    Grain* g1 =NULL;
    Grain* g2 = NULL;
    for (int i = 0; i <= nbg - 2; i++) {
        g1 = g[i];
        for (int j = i+1; j <= nbg - 1; j++) {
            g2 = g[j];
            double d2 = distance(g1, g2); // distance au carre
            if ( (d2 < pow(g1->rayon + g2->rayon, 2)) && (g1->paroie == false) && (g2->paroie == false) ) {
                cout<<"JE VOIS UN CONTACT"<<endl;
                if ( !contactPresent(g1, g2) ) {
                    insertionContact(g1, g2, stock, sqrt(d2), dt);
                        // DEBUG
                    cout << "\n\n---- DEBUG: contact insere dans sable ----" << endl;
                    stock->afficherListe();
                    afficherListeSable();
                }
            }
        }
    }
}


//  Mise a jour de chaque contact de la liste chainee
void Sable::updateListeContacts(double dt) { // sys casiers + examiner couples de grains

    if (c != NULL) { // liste non vide
    cout<<"j'UPDAATE  UN CONTACT"<<endl;
        Contact * curs = c;
        while (curs != NULL) {
            double d = sqrt( distance(curs->g1, curs->g2) );
            curs->metAjourEfforts(d, dt);
            curs = curs->next;
        }

    }

}

// vider la liste de contacts du sable
void Sable::videContactsSable() {
    if (c != NULL) {
        Contact * curs = NULL;
        while (c != NULL) {
            curs = c;
            c = c->next;
            delete curs;
            curs = NULL;
        }
    }
}


// vider le tableau de grains
void Sable::videGrainsSable() {
    if (g != NULL) {
        for (int i = 0; i <= nbg - 1; i++) {
            delete g[i];
        }
        delete g;
        g = NULL;
    }
}

// affichage de la liste et de ses elements
void Sable::afficherListeSable(){
    Contact* temp = c;
    int indice = 1;

    cout << "----- Afficher Liste Sable -----" << endl;

    if (temp == NULL) {
        cout << "liste vide" << endl;
    } else {

        while(temp != NULL) {
            cout << "elt " << indice << " : " << endl;
            cout << "   grain1: " << temp->g1 << endl;
            cout << "   grain2: " << temp->g2 << endl;
            temp = temp->next;
            indice++;
        }
    }
}

/*void Sable::simulation(double duree , double dt , int frequence_affichage){
    cout << "DEBUG_SABLE :  Simulation lancee" << endl;
    cout << "DEBUG_SABLE :  nombre de grains : "<< this->nbg << endl;

    int compteur = 0;
    int t = 0;
    //int temps = 10;
    //char* nomfile = "sortie";
    int nbChiffres;
    int t_temp;
    double d1;
    /*double d2;
    double d3;
   /* int d1_p;
    int d2_p;
    int d3_p;*/
    //Contact* c1 = new Contact();
    //Contact* c2 = new Contact();
   // Contact* c3 = new Contact();
    /*Contact* c1_p = new Contact();
    Contact* c2_p = new Contact();
    Contact* c3_p = new Contact();*/

    //for (double i = 0; i <= duree; i+=dt){
     //   d1 = distance(g[0],g[1]);
        //d2 = distance(g[1],g[2]);
        //d3 = distance(g[0],g[1]);
        //d1_p = distance(g[0],g[3]);
        //d2_p = distance(g[1],g[3]);
        //d3_p = distance(g[2],g[3]);
       // if(d1 > g[1]->rayon - g[0]->rayon){
            //cout<<" Contact cree distance entre le grain 1 et la paroie = " << g[1]->rayon - d1 <<endl;
          //  c3->initialiseContact(g[0],g[1],d1 , dt);
       // }
        /*if(d2 > g[2]->rayon - g[1]->rayon){
            c1->initialiseContact(g[1],g[2], d2 , dt);
        }
        if(d3 < g[0]->rayon + g[1]->rayon){
            c2->initialiseContact(g[0],g[1],d3 , dt);
        }

       /* if(d1_p>g[3]->rayon-g[0]->rayon){
            c1_p->initialiseContact(g[3],g[0],g[3]->rayon-d1_p , dt);
        }
        if(d2_p>g[3]->rayon-g[1]->rayon){
            c2_p->initialiseContact(g[3],g[1],g[3]->rayon-d2_p , dt);
        }
        if(d3_p>g[3]->rayon-g[2]->rayon){
            c3_p->initialiseContact(g[3],g[2],g[3]->rayon-d3_p , dt);
            c3_p-> metAjourEfforts(g[3]->rayon-d3_p,dt);
        }*/

       /* if(compteur%frequence_affichage ==0){
            //cout << t << endl;
            nbChiffres = 0;
            t_temp = t;
            while (t_temp > 0){
                t_temp /= 10;
                nbChiffres++;
            }
            stringstream sstm;
            sstm << "\"output";
            for(int i = 1 ; i<= 5-nbChiffres ; i++){
                sstm << 0;
            }
            sstm << t << ".png\"";
            string ans = sstm.str();
            char * ans2 = (char *) ans.c_str();
            g[0]->ecrireGnuplot("gnutest",ans2,50);
            system("gnuplot gnutest");
            system("rm contour");
            system("rm segment");
            ecriture_fichier();
        }
    t++;
   // cout << "DEBUG_SABLE :nombre de grains dans le sable : "<< this->nbg <<endl;
    deplacement(dt);
    compteur ++;
    }
    system("convert -delay 5 -loop 0 *.png animation.gif ");
    system("rm *.png");
    system("animate animation.gif");
}*/

void Sable::ajout_Bloc(int nombre, double coord[3], double rayon, double epsi){ //coordonnes du grain en haut a gauche
    cout<<"debut fonction ajout_Bloc"<<endl;
    int nbg_cote;
    int i = 2;
    bool verif = false;
    if(nombre > 100 || nombre < 2){
        cout<< "Nombre de grain par bloc : entre 2 et 100" <<endl;
    }
    else {
        while( (i < 11) && (verif == false) ){
            if(nombre < (i*i + 1)){
                nbg_cote = i;
                verif = true;
            }
            i++;
        }
        cout<<"DEBUG_SABLE : fin boucle : longueur du coté = " << nbg_cote <<endl;
        bool paroie = false;
        double v[3];
        double teta[3];
        double omega[3];
        for(int i = 0; i<3 ; i++){
            v[i] = 0;
            teta[i] = 0;
            omega[i] = 0;
        }
        double m = 1;
        double J = 1;
        double kn = 20;
        double ks = 0.1;
        double cn = 0;
        double cs = 0;
        bool quitter = false;
        int compteur = 0;
        while (compteur<nombre-1){
            for (int j =0; j<nbg_cote; j++){
                for (int i = 0; i<nbg_cote;i++){
                    //cout<< "DEBUG_Sable : avant ajout grain" <<endl;
                    this->ajouteGrain(paroie,coord,v,teta,omega,rayon,m,J,kn,ks,cn,cs);
                    //cout<< "DEBUG_Sable : apres ajout grain" <<endl;
                    //cout<< "DEBUG_Sable : coordonnées du grain numero " << compteur << " : " <<  coord[0] << "  " << coord[1] <<endl;
                    coord[0] += 2*rayon + epsi ;
                    compteur++;
                    if(compteur==nombre){
                        quitter = true;
                    }
                    if(quitter){
                        break;
                    }
                    cout << "compteur : " << compteur << endl;
                }
                if(quitter){
                    break;
                }
                coord[0]-= (nbg_cote)*(2*rayon + epsi);
                coord[1] -= 2*rayon + epsi ;

            }
            if(quitter){
                break;
            }
        }
    }
}

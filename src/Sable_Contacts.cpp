#include "Sable.h"

/** Gestion de la liste de contacts du sable */

/////////// RECHERCHE NOUVEAUX CONTACTS AVEC PAVAGE /////////////////

/*  on cherche les nouveaux contacts grace au pavage
    et on met à jour la liste chainée des contats du sable */
void Sable::cherche_nouveaux_contacts_pavage(StockContacts* stock, double dt) {

    int i = 0;
    Grain* liste_voisins[100];

    while (i <= taille -2 && g[i] != NULL) { // parcourt tous les grains du sable

        for (int j = 0; j < 100; j++) { // reinitialisation liste voisins à NULL
            liste_voisins[j] = NULL;
        }

        grille->dresse_liste_voisins(g[i], liste_voisins); // dresser liste voisins
        creer_nouveaux_contacts(stock, dt, g[i], liste_voisins); // creer nouveaux contacts
        i++;
    }
}

/* Creer nouveaux contacts detectes dans la liste des voisins */
void Sable::creer_nouveaux_contacts(StockContacts* STOCK, double DT,
                                    Grain* G, Grain* LISTE_VOISINS[100]) {
    double eps = 0; // à 10-3 pres
    int j = 0;
    while (LISTE_VOISINS[j] != NULL) {
        Grain* g1 = G;
        Grain* g2 = LISTE_VOISINS[j];
        StockContacts* stock = STOCK;
        double dt = DT;

        double d2 = distance_carre(g1, g2); // distance au carre

        // contact grain-grain
        if ( ( d2 - pow(g1->rayon + g2->rayon, 2)  <= eps ) && (g1->paroie == false) && (g2->paroie == false) ) {
            if ( !contact_present(g1, g2) ) {
                insertion_contact(g1, g2, stock, dt);
            }
        }

        // contact grain-paroi
        else if ( ( d2 - pow(g2->rayon - g1->rayon, 2)  >= eps ) && (g1->paroie == false) && (g2->paroie == true) ) {
            if ( !contact_present(g1, g2) ) {
                insertion_contact(g1, g2, stock, dt);
            }
        }

        j++; // aller au voisin suivant
    }
}




///////////////// GESTION CONTACTS ////////////////////////

void Sable::suppression_contacts_ouverts(StockContacts* stock) // parcours de la liste + calcul de distance
{
    if (c != NULL) { // liste a au moins 1 elt
       static int nbSuppressions = 1;

        // traitement des premiers contacts ouverts successifs
        if(c->g2->paroie==false){

            while ( c != NULL && ( distance_carre(c->g1, c->g2) > pow(c->g1->rayon + c->g2->rayon , 2) )  ) {
                Contact *curs = c;
                c = c->next;
                stock->set_contact(curs); // rendre le contact au stock
                curs = NULL;
                nbSuppressions++;
            }

            // Traitement des contacts suivants
            if (c != NULL) { // c pointe sur un contact ferme
                Contact *curs = c->next;
                Contact *prec = c;

                while (curs != NULL) {

                    if ( ( distance_carre(curs->g1, curs->g2) > pow(curs->g1->rayon + curs->g2->rayon , 2) )  ) { //ouvert
                        Contact* old = curs;
                        curs = curs->next;
                        prec->next = curs;
                        stock->set_contact(old); // rendre le contact au stock
                        old = NULL;
                        nbSuppressions++;

                    } else { // ferme
                        prec = curs;
                        curs = curs->next;
                    }

                }
            }
        }

        else{

            while ( c != NULL && ( distance_carre(c->g1, c->g2)  < pow(c->g2->rayon - c->g1->rayon , 2) )  ) {
                Contact *curs = c;
                c = c->next;
                stock->set_contact(curs); // rendre le contact au stock
                curs = NULL;
                nbSuppressions++;
            }

            // Traitement des contacts suivants
            if (c != NULL) { // c pointe sur un contact ferme
                Contact *curs = c->next;
                Contact *prec = c;

                while (curs != NULL) {

                    if ( ( distance_carre(curs->g1, curs->g2)  < pow(curs->g2->rayon - curs->g1->rayon , 2) )  ) { //ouvert
                        Contact* old = curs;
                        curs = curs->next;
                        prec->next = curs;
                        stock->set_contact(old); // rendre le contact au stock
                        nbSuppressions++;

                    } else { // ferme
                        prec = curs;
                        curs = curs->next;
                    }

                }
            }

        }


    }
}


//  on cherche les nouveaux contacts et on met à jour la liste chainée des contats du sable
void Sable::cherche_nouveaux_contacts(StockContacts* stock, double dt) { // sys casiers + examiner couples de grains
    Grain* g1 =NULL;
    Grain* g2 = NULL;
    double eps = 0; // à 10-3 pres
    int i = 0;
    int j = 0;


    while (i <= taille -2 && g[i] != NULL) {
        g1 = g[i];
        j = i + 1;

        while (j <= taille - 1 && g[j] != NULL) {
            g2 = g[j];
            double d2 = distance_carre(g1, g2); // distance au carre
            if ( ( d2 - pow(g1->rayon + g2->rayon, 2)  <= eps ) && (g1->paroie == false) && (g2->paroie == false) ) {
                if ( !contact_present(g1, g2) ) {
                    insertion_contact(g1, g2, stock, dt);
                }
            }
            else if ( ( d2 - pow(g2->rayon - g1->rayon, 2)  >= eps ) && (g1->paroie == false) && (g2->paroie == true) ) {
                if ( !contact_present(g1, g2) ) {
                    if (eg != eg_sable()) {
                        eg = eg_sable();
                    }
                    insertion_contact(g1, g2, stock, dt);
                }
            }
            j++;
        }
        i++;
    }
}


/* update efforts grains qui sont en contact */
void Sable::update_efforts_grains() {

    if (c != NULL) {
        eg = eg_sable();
        // mise a jour efforts de grains
        Contact* curs = c;
        while (curs != NULL) {
           curs->MAJ_efforts_grains();
           curs = curs->next;
        }
    }
}

//  Mise a jour de chaque contact de la liste chainee
void Sable::update_efforts_contacts(double dt) {

    if (c != NULL) { // liste non vide
        Contact * curs = c;
        while (curs != NULL) {
            double d = sqrt( distance_carre(curs->g1, curs->g2) );
            curs->met_a_jour_efforts(d, dt);    // muise a jour efforts de contact
            curs = curs->next;
        }
    }

}

// affichage de la liste de contacts du sable et de ses elements
void Sable::affiche_contacts_sable(){
    Contact* temp = c;
    int indice = 1;

    cout << "----- Afficher Contacts du Sable -----" << endl;

    if (temp == NULL) {
        cout << "liste vide" << endl;
    } else {

        while(temp != NULL) {
            cout << "elt " << indice << " : " << endl;
            cout << "   grain1: " << temp->g1->id << endl;
            cout << "   grain2: " << temp->g2->id << endl;
            temp = temp->next;
            indice++;
        }
    }
}

// insertion contact dans le sable
void Sable::insertion_contact(Grain* g1, Grain* g2, StockContacts* stock, double dt) {
    Contact* nveau = stock->get_contact(); // demande d'un contact au stock
    nveau->initialise_contact(g1, g2, dt);
    nveau->next = c;
    c = nveau;
}

// vider la liste de contacts du sable + remise a ZERO TOUS LES EFFORTS DES GRAINS
void Sable::vide_contacts_sable() {
    if (c != NULL) {
        Contact * curs = NULL;
        while (c != NULL) {
            curs = c;
            c = c->next;
            curs->reinitialise_efforts_grains(); // remise a zero efforts grains
            delete curs;
            curs = NULL;
        }
    }
}


// tester la presence d'un contact dans le sable
bool Sable::contact_present(Grain* G1, Grain* G2) {

    if (c == NULL) { // liste vide
        return false;
    } else { // parcours de liste
        Contact* curs = c;
        while (curs != NULL) {
            if ( (curs->g1 == G1 && curs->g2 == G2)
                || (curs->g1 == G2 && curs->g2 == G1) ) { // contact trouve
                return true;
            }
            curs = curs->next;
        }
        return false;
    }

}

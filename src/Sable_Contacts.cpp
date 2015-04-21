#include "Sable.h"

/** Gestion de la liste de contacts du sable */

void Sable::suppression_contacts_ouverts(StockContacts* stock) // parcours de la liste + calcul de distance
{
    if (c != NULL) { // liste a au moins 1 elt
       static int nbSuppressions = 1;
        // traitement des premiers contacts ouverts successifs
        if(c->g2->paroie==false){

            while ( c != NULL && ( distance_carre(c->g1, c->g2) > pow(c->g1->rayon + c->g2->rayon , 2) )  ) {
                Contact *curs = c;
                c = c->next;
                curs->reinitialise_efforts(); // remise a zero des efforts
                delete curs;
                //stock->set_contact(curs); // rendre le contact au stock
                curs = NULL;
                nbSuppressions++;
                stock->affiche_contacts_stock();  // DEBUG
                 affiche_contacts_sable(); // DEBUG
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
                        old->reinitialise_efforts(); // remise a zero des efforts
                        delete old;
                        //stock->set_contact(old); // rendre le contact au stock
                        old = NULL;
                        nbSuppressions++;
                        stock->affiche_contacts_stock();   // DEBUG
                        affiche_contacts_sable(); // DEBUG
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
                curs->reinitialise_efforts(); // remise a zero des efforts
                delete curs;
                //stock->set_contact(curs); // rendre le contact au stock
                curs = NULL;
                nbSuppressions++;
                stock->affiche_contacts_stock();  // DEBUG
                 affiche_contacts_sable(); // DEBUG
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
                        old->reinitialise_efforts(); // remise a zero des efforts
                        delete old;
                        //stock->set_contact(old); // rendre le contact au stock
                        old = NULL;
                        nbSuppressions++;
                        stock->affiche_contacts_stock();   // DEBUG
                        affiche_contacts_sable(); // DEBUG
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
                    insertion_contact(g1, g2, stock, sqrt(d2), dt);
                    // stock->affiche_contacts_stock();  // DEBUG
                    affiche_contacts_sable(); // DEBUG
                }
            }
            else if ( ( d2 - pow(g2->rayon - g1->rayon, 2)  >= eps ) && (g1->paroie == false) && (g2->paroie == true) ) {
                //cout<< "Valeur de la distance des grains avec la paroie : " << d2 <<endl;
                //cout<< "Valeur de la difference des rayons des grains : " << pow(g2->rayon - g1->rayon, 2) <<endl;
                if ( !contact_present(g1, g2) ) {
                    insertion_contact(g1, g2, stock, sqrt(d2), dt);
                    // stock->affiche_contacts_stock();  // DEBUG
                    affiche_contacts_sable(); // DEBUG
                }
            }
            j++;
        }

        i++;

    }

}


//  Mise a jour de chaque contact de la liste chainee
void Sable::update_liste_contacts(double dt) { // sys casiers + examiner couples de grains

    if (c != NULL) { // liste non vide
        Contact * curs = c;
        while (curs != NULL) {
            double d = sqrt( distance_carre(curs->g1, curs->g2) );
            curs->met_a_jour_efforts(d, dt);
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
void Sable::insertion_contact(Grain* g1, Grain* g2, StockContacts* stock, double distance, double dt) {
    Contact* nveau = stock->get_contact(); // demande d'un contact au stock
    nveau->initialise_contact(g1, g2, distance, dt);
    nveau->next = c;
    c = nveau;
}

// vider la liste de contacts du sable
void Sable::vide_contacts_sable() {
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


// tester la presence d'un contact dans le sable
bool Sable::contact_present(Grain* g1, Grain* g2) {

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

#include "ListeCasiers.h"

/* Constructeur de liste vide */
ListeCasiers::ListeCasiers()
{
    c = NULL;
}

ListeCasiers::~ListeCasiers()
{
    vide_grains();
}


// vider le stock de grains de tous les casiers
void ListeCasiers::vide_grains() {
    Casier* temp = NULL;

    while (c != NULL) { // parcours de la liste des casiers
        c->vide_casier();
        temp = c;
        c = c->suivant;
        delete temp;
    }

    c = NULL;
}


//////////////// INSERTION CASIER ET FONCTIONS AUXILLIAIRES //////////////////

/* Examiner tete de la liste */
/* Renvoie true si insertion a eu lieu, false sinon */
bool ListeCasiers::insertion_casier_tete(Grain* G) {

    if (c == NULL) { // casiers
        c = new Casier(G->x_case[0], G->x_case[1], G->x_case[2]);
        c->ajout_grain(G);
        return true;

    } else if (exam_casier(c, G) == 0) {
        c->ajout_grain(G); // ajout grain
        return true;

    } else if (exam_casier(c, G) == -1) { // insertion en tête nveau casier + grain dedans
        Casier* temp = c;
        c = new Casier(G->x_case[0], G->x_case[1], G->x_case[2]);
        c->suivant = temp;
        c->ajout_grain(G); // ajout grain
        return true;

    }

    return false; // if (exam_casier(c, G) == 1)  (warning supprime)
}

/* Examiner suite de la liste pour insertion grain */
void ListeCasiers::insertion_casier_suite(Grain* G) {
    Casier* prec = c;
    Casier* curs = c->suivant;
    bool insertion_ok = false;

    // PARCOURS SUITE LISTE + INSERTION EVENTUELLE
    while (curs != NULL  && insertion_ok == false) {
        if (exam_casier(curs, G) == 0) { // ajouter le grain
           curs->ajout_grain(G);
           insertion_ok = true;

        } else if (exam_casier(curs, G) == 1) { // avancer sans insertion
            prec = prec->suivant;
            curs = curs->suivant;

        } else if (exam_casier(curs, G) == -1) { // allocation puis ajout avant casier courant
            prec->suivant = new Casier(G->x_case[0], G->x_case[1], G->x_case[2]);
            prec->suivant->suivant = curs;
            prec->suivant->ajout_grain(G);
            insertion_ok = true;
        }
    }

    // INSERTION FAITE OU ON EST ARRIVE AU BOUT
    if (curs == NULL && insertion_ok == false) { // insertion en queue
        prec->suivant = new Casier(G->x_case[0], G->x_case[1], G->x_case[2]);
        prec->suivant->ajout_grain(G);
        insertion_ok = true;
    }
}


/* Parcours les listes casier + en crée 1 au bon endroit + ajoute le grain dedans */
void ListeCasiers::insertion_casier(Grain* G) {

    // tenter l' insertion en tête
    if (insertion_casier_tete(G) == false) {
        // insertion dans la suite de la liste
        insertion_casier_suite(G);
    }
}

/* Examen du casier courant => bon casier pour le grain ?*/
/* Renvoie 0 => casier numero (x, y, z) trouve */
/* Renvoie 1 => casier numero aller au suivant */
/* Renvoie -1 => inserer a cet endroit là */
int ListeCasiers::exam_casier(Casier *C, Grain *G) {
    return C->exam_casier(G->x_case[0], G->x_case[1], G->x_case[2]);
}


////////////// UPDATE CASIERS /////////////////////////


/* Parcours les casiers + remet les grains a leur bonne place */
void ListeCasiers::update_casiers() {

    if (c != NULL) { // si la liste est non vide

        Casier *curs = c; // curseur pour casiers

        /* Parcours de la liste des casiers */
        while (curs != NULL) {
            /* Parcours de la liste des grains du casier curs */
            // Deplacement des premiers grains
            while (curs->g != NULL) {
                if (exam_casier(curs, curs->g) != 0) {// grain a changer
                    Grain *temp = curs->g;
                    curs->g = curs->g->s;
                    insertion_casier(temp);
                } else {
                    break;
                }
            }

            // Examen des grains suivants (si liste grains non vide)
            if (curs->g != NULL) {
                Grain* prec = curs->g;
                Grain *g = curs->g->s;

                while (g != NULL) { // parours liste grains
                    if (exam_casier(curs, g) != 0) { // grain a changer
                        Grain* temp = g;
                        g = g->s;
                        prec->s = g;
                        insertion_casier(temp);
                    } else { // grain ne change pas => avancer
                        prec = prec->s;
                        g = g->s;
                    }
                }

            }

            curs = curs->suivant; // avancer dans liste des casiers
        }
    }
}


////////////// SUPRRESSION CASIERS  VIDES /////////////////////////

/* Parcours de la liste des casiers + suppression des casiers vides */
/* appel sur une liste non vide */
void ListeCasiers::supprime_casiers_vides() {

    /* Elimination des premiers casiers vides */
    while (c != NULL) {
        if (c->g == NULL) { // casier vide
            Casier* temp = c;
            c = c->suivant;
            delete temp;
        } else {
            break; // sortir du while
        }
    }

    /*Parcours des casiers restants (liste non vide) */
    if (c != NULL) {
        Casier* prec = c;
        Casier* curs = c->suivant;

        while (curs != NULL) {

            if (curs->g == NULL) {
                Casier* temp = curs;
                curs = curs->suivant;
                prec->suivant = curs;
                delete temp;
            } else {
                prec = prec->suivant;
                curs = curs->suivant;
            }

        }

    }

}


//////////////// AFFICHE CASIERS ////////////////

 void ListeCasiers::affiche_casiers() {
    cout << "\n------ Affichage des casiers ------\n" << endl;
    Casier* curs = c;
    cout << "DEBUG: afficheCasiers   " << c << endl;
    while (curs != NULL) {
        curs->affiche_casier();
        curs = curs->suivant;
    }
 }

#include "Pavage.h"


///////// CONSTRUCTION - DESTRUCTION PAVAGE

Pavage::Pavage(double COORD_MIN[3], double COORD_MAX[3] , double RMAX) {

    // Affectation bornes du pavage
    for (int i=0; i <3; i++) {
        coord_min[i] = COORD_MIN[i];
        coord_max[i] = COORD_MAX[i];
    }

    // Calcul dimensions paves elementaires
    double eps = 0.1 * RMAX; // ecart eltaire
    cote_pave = 2*RMAX + eps;


    // Calcul du nombre de paves elementaires par cote
    double cote_paven[3];
    for (int i = 0; i < 2; i++) {
        nb_paves[i] = (int) ceil(  ( coord_max[i] - coord_min[i] ) / cote_pave );
        cote_paven[i] = ( coord_max[i] - coord_min[i] ) / (double) nb_paves[i] ;
    }
    // 3D
    nb_paves[2] = 0;
    cote_paven[2] = 0;


    // Maximum de cote_paven => decoupage optimal
    cote_pave = cote_paven[0];
    for (int i = 1; i < 3; i++) {
        if (cote_paven[i] > cote_pave)
            cote_pave = cote_paven[i];
    }


    for (int i = 0; i < 3; i++) {
        int ancien_max = coord_max[i];
        coord_max[i] = coord_min[i] + cote_pave *nb_paves[i];
        if (coord_max[i] < ancien_max) {
            cerr << "Pavage::Pavage : ERREUR - Pb dans le calcul du pavage avec paves cubiques"<< endl;
            exit(-1);
        }
    }


    // Allocation des casiers de grains
    casier = new Grain***[ nb_paves[0] ];
    for (int i =0; i < nb_paves[0]; i++) {
        casier[i] = new Grain**[ nb_paves[1] ];
        for (int j=0; j < nb_paves[1]; j++) {
            casier[i][j] = new Grain*[ nb_paves[2] ];
            //for (int k=0; k < nb_paves[2]; k++) { // 3D
                int k = 0;
                // Initialisation des casiers à NULL
                casier[i][j][k] = NULL;
            //}
        }
    }

}

/* Vidage du pavage avant destruction */
Pavage::~Pavage()
{
    vidage_pavage();
}

/* Vidage entier du pavage */
void Pavage::vidage_pavage() {

    for (int i=0; i < nb_paves[0]; i++) {
        for (int j=0; j < nb_paves[1]; j++) {
            for (int k=0; k < nb_paves[2]; k++) {
                vidage_casier(&casier[i][j][k]);
            }
        }
    }

}

/* Vidage du casier correspondant à casier[i][j][k] */
void Pavage::vidage_casier(Grain** CASIER) {

    while (*CASIER != NULL) { // si casier est non vide
        Grain* premier = *CASIER;                // pointeur sur premier element
        Grain* s = (*CASIER)->get_suivant(); // pointeur sur element suivant du premier

        // premier element retire de la liste
        *CASIER = s;
        (*CASIER)->set_precedent(NULL);

        // premier element ne pointe plus sur personne
        premier->set_precedent(NULL);
        premier->set_suivant(NULL);
    }

}


/////// INSERTION GRAIN

/* Insertion de grain dans le pave approprie*/
/* Erreur + Interruption programme si  Grain hors du maillage
   => Code d'erreur : -1*/
void Pavage::inserer_grain_pave(Grain *G, int PAVE[3]) {

    /* Verifier si G est hors du maillage */
    bool trop_haut = PAVE[0] >= nb_paves[0] || PAVE[1] >= nb_paves[1];  // 3D: ajout PAVE[2] >= nb_paves[2]
    bool trop_bas = PAVE[0] < 0 || PAVE[1] < 0;                         // 3D: ajout PAVE[2] < 0

    if (trop_haut || trop_bas) {
            cerr << "\nERREUR : Grain "<< G->id << " hors du pavage." <<endl;
            cerr << "--> Coordonnees du grain  "<< G->X[0] << " "
                << G->X[1] << " " << G->X[2] << endl;
            cerr << "--> Coordonnees du nouveau pave " << PAVE[0] << " " << PAVE[1]
                << " " << PAVE[2] << endl;
            cerr << " --- PROGRAMME INTERROMPU ---- " << endl;
            exit(-1);
    }

    // Insertion en tete de liste du nouveau grain
    Grain** tete = &casier[PAVE[0]][PAVE[1]][PAVE[2]];

    if (*tete == NULL) {
        *tete = G;
    } else {
        Grain* ancienne_tete = *tete;
        *tete = G;
        (*tete)->set_suivant(ancienne_tete);
        ancienne_tete->set_precedent(*tete);
    }

    // Mise a jour du numero de pave stocke dans le grain
    G->set_x_case(PAVE);

}



/////// INITIALISATION  +  MISE A JOUR PAVES GRAINS

/* Initialisation du pavage avec tous les grains du sable */
void Pavage::initialise_pavage(Grain* G) {

    int nv_pave[3];
    calcul_nv_pave(G, nv_pave);

    // insertion dans le pave approprie
    inserer_grain_pave(G, nv_pave);

}

/* Update numero de pave d'un grain donne au sein du pavage */
void Pavage::update_pave_grain(Grain* G) {

    int nv_pave[3];
    calcul_nv_pave(G, nv_pave);

    if ( change_pave_grain(G, nv_pave) ) {
        // Retrait de G de l'ancien pave
        Grain* p = G->get_precedent();
        Grain* s = G->get_suivant();
        if (p == NULL) { // on est en tete de liste
            int * ancien_pave = G->get_x_case();
            casier[ancien_pave[0]][ancien_pave[1]][ancien_pave[2]] = s;
        } else {
            p->set_suivant(s);
        }
        if (s != NULL) { // Grain n'est pas le dernier element de la liste
            s->set_precedent(p);
        }

        // Grain retire: n'a plus de precedent ni de suivant
        G->set_precedent(NULL);
        G->set_suivant(NULL);

        // insertion dans le pave approprie
        inserer_grain_pave(G, nv_pave);

    }
}

/* calcul nv pave pour un grain donne */
void Pavage::calcul_nv_pave(Grain* G, int NV_PAVE[3]) {

    NV_PAVE[0] = (int) floor( ( (G->X[0] - coord_min[0]) /  (coord_max[0] - coord_min[0]) ) * nb_paves[0] );
    NV_PAVE[1] = (int) floor( ( (G->X[1] - coord_min[1]) /  (coord_max[1] - coord_min[1]) ) * nb_paves[1] );
    NV_PAVE[2] = 0;
    // 3D : NV_PAVE[2] = (int) floor( ( (G->X[2] - coord_min[2]) /  (coord_max[2] - coord_min[2]) ) * nb_paves[2] );

}

/* Comparaison de l'ancien numero de casier (du grain) avec celui calcule (NV_PAVE)
    True => deplacer le grain / False => laisse le grain dans le pave */
bool Pavage::change_pave_grain(Grain* G, int NV_PAVE[3]) {

    int* coord = G->get_x_case();

    return     coord[0] != NV_PAVE[0]
            || coord[1] != NV_PAVE[1]
            || coord[2] != NV_PAVE[2];
}


//////////// AFFICHAGE A L'ECRAN DU PAVAGE

/* Affichage du pavage */
void Pavage::affichage_pavage() {

    static int num = 1;
    cout << "\n\n------------ Affichage PAVAGE " << num
        << " ---------------" << endl;
    num++;

    for (int i=0; i < nb_paves[0]; i++) {
        for (int j=0; j < nb_paves[1]; j++) {
            //for (int k=0; k < nb_paves[2]; k++) {  //3D
                int k = 0;
                if (casier[i][j][k] != NULL) {
                    affichage_casier(casier[i][j][k]);
                }
            //}
        }
    }
}


/* Affichage du casier*/
void Pavage::affichage_casier(Grain* CASIER) {

    int* coord = CASIER->get_x_case();

    cout << "\n---- Affichage du casier : " << coord[0]
        << " " << coord[1] << " " << coord[2]
        << " ----" << endl;

    Grain* curs = CASIER;
    while (curs != NULL) {
        curs->afficher_grain(true);
        curs = curs->get_suivant();
    }
}



//////////////////  RECHERCHE DES VOISINS ///////////////////////////////

/* Dresser la liste des voisins */
void Pavage::dresse_liste_voisins( Grain* G, Grain* LISTE_VOISINS[100] ) {

    int* coord = G->get_x_case();

    /* Selon Ox */
    int i0 = coord[0] - 1;   // pave de gauche
    if (i0 < 0) {  // 0 numero premier pave
        i0 = 0;
    }
    int i1 = coord[0] + 1;  // pave de droite
    if(i1 > nb_paves[0] - 1) { // nb_paves[0]-1 numero dernier pave
        i1 = nb_paves[0] - 1;
    }

    /* Selon Oy */
    int j0 = coord[1] - 1;   // pave du bas
    if (j0 < 0) {  // 0 numero premier pave
        j0 = 0;
    }
    int j1 = coord[1] + 1;  // pave du haut
    if(j1 > nb_paves[1] - 1) { // nb_paves[1]-1 numero dernier pave
        j1 = nb_paves[1] - 1;
    }

    /* Selon Oz */
    // 3D

    /* Remplissage liste voisins */
    int nb_voisins = 0; // nb de voisins presents dans la liste des voisins
    for (int i = i0; i <= i1; i++) {
        for (int j = j0; j <= j1; j++) {
            int k = 0; // 3D
            nb_voisins = ajout_grains_liste_voisins(G, LISTE_VOISINS, i, j, k, nb_voisins);
        }
    }

}



/* Mettre les voisins du grain G, presents dans la case X, Y, Z dans liste des voisins
 * Renvoyer une erreur si la taille est inferieure à 100 */
int Pavage::ajout_grains_liste_voisins(Grain* G, Grain* LISTE_VOISINS[100], int X, int Y, int Z, int nb_voisins) {

    if (casier[X][Y][Z] != NULL) { // pave non vide

        Grain* curs = casier[X][Y][Z];

        while(curs != NULL) {

            if (nb_voisins >= 100) { // liste voisins deja complete avant insertion
                cerr << "Pavage::ajout_grains_liste_voisins : trop de voisins : nb_voisins > 100" << endl;
                exit(-1);

            } else {

                if (curs != G ) { // ajout voisins de G sauf G lui-meme
                    LISTE_VOISINS[nb_voisins] = curs;
                    nb_voisins++;
                }
                curs = curs->get_suivant();
            }
        }
    }

    return nb_voisins;
}



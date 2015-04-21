#ifndef LISTECASIERS_H
#define LISTECASIERS_H
#include "Casier.h"
#include "stdlib.h"

class ListeCasiers
{
    public:
            /* Gestion de la liste des grains */
        ListeCasiers();
        virtual ~ListeCasiers();

        bool insertion_casier_tete(Grain* G);
        void insertion_casier_suite(Grain* G);
        void insertion_casier(Grain* G); // insertion du grain dans son casier approprié
        void vide_grains();
        int exam_casier(Casier *C, Grain *G); // Parcours de la liste des casiers + suppression des casiers vides
        void supprime_casiers_vides();
        void update_casiers();
        void affiche_casiers();

        /* Gestion de la liste des contacts */
            // TODO

        /* Attributs */
        Casier* c;

};

#endif // LISTECASIERS_H

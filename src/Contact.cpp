#include "Contact.h"

Contact::Contact(){
    for(int i = 0 ; i<3 ; i++){
         n[i] = 0;
         s[i] = 0;
         v[i] = 0;
         vs[i] = 0;
         deltan[i] = 0;
         deltas[i] = 0;
         F_n[i] = 0;
         F_s[i] = 0;
    }

    next = NULL;
    g1 = NULL;
    g2 = NULL;
}


void Contact::initialise_contact(Grain* G1, Grain* G2, double distance, double dt) {
    g1 = G1;
    g2 = G2;
    met_a_jour_efforts(distance, dt);
}

/* met a jour les efforts de contact en 3D */
//On distingue les cas où il y a une paroie parmis les deux grains en entrée pour l'instant on suppose que le deuxième grain en entrée
//sera celui succeptible d'etre une paroie
void Contact::met_a_jour_efforts(double distance, double dt)
{

    for(int i = 0 ; i<3 ; i++){ // pour les 3 dimensions
        if(g2->paroie == true){
            n[i] = -(g2->X[i]-g1->X[i])/distance;       //vecteur direction UNITAIRE
            v[i] = -(g1->v[i] - g2->v[i]) - (g2->omega[i]*g2->rayon + g1->omega[i]*g1->rayon)*n[i];
            vs[i] = -v[i] + (v[i]*n[i])*n[i] + (g2->omega[i]*g2->rayon + g1->omega[i]*g1->rayon)*n[i];
            deltan[i] = v[i]*n[i]*dt ;
            deltas[i] = vs[i]*dt ;
            F_n[i] = ((-g2->kn * deltan[i])-(g2->cn * v[i]))*n[i];
            F_s[i] = (-g2->ks * deltas[i])-(g2->cs * vs[i]);
            // Loi de Coulomb a ajouter si pb
            g1->f[i] += -(F_n[i] + F_s[i]);
            g2->f[i] += -F_n[i] - F_s[i];
            g1->m[i] += -g1->rayon*n[i]*F_s[i];
            g2->m[i] += -g2->rayon*n[i]*F_s[i];

        }
        else{   // grains normaux

            if (i == 0) { // debug
                cout<<  "Valeur de la distance : " << distance  <<endl;
            }

            n[i] = (g2->X[i]-g1->X[i])/distance;
            v[i] = (g1->v[i] - g2->v[i]) - (g2->omega[i]*g2->rayon + g1->omega[i]*g1->rayon)*n[i];
            vs[i] = v[i] - (v[i]*n[i])*n[i] + (g2->omega[i]*g2->rayon + g1->omega[i]*g1->rayon)*n[i];
            deltan[i] = v[i]*n[i]*dt ;
            deltas[i] = vs[i]*dt ;
            F_n[i] = ((-g1->kn * deltan[i])-(g1->cn * v[i]))*n[i];
            F_s[i] = (-g1->ks * deltas[i])-(g1->cs * vs[i]);
            // Loi de Coulomb a ajouter si pb
            g1->f[i] += (F_n[i] + F_s[i]);
            g2->f[i] += -F_n[i] - F_s[i];
            g1->m[i] += g1->rayon*n[i]*F_s[i];
            g2->m[i] += -g2->rayon*n[i]*F_s[i];

        }
        //cout<<"Valeur de la force dans X["<<  i <<"] sur g1 :" << g1->f[i] <<endl;

    }
}

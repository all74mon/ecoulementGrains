#include "Contact.h"

Contact::Contact(){
    for(int i = 0 ; i<3 ; i++){
         n[i] = 0;
         s[i] = 0;
         v[i] = 0;
         vs[i] = 0;
         deltas[i] = 0;
         delta_fn[i] = 0;
         delta_fs[i] = 0;
         fn[i] = 0;
         fs[i] = 0;
    }

    nb_dt = 0;
    deltan = 0;

    next = NULL;
    g1 = NULL;
    g2 = NULL;
}


void Contact::initialise_contact(Grain* G1, Grain* G2, double dt) {
    if(G1->paroie == false && G2->paroie == false){
        g1 = G1;
        g2 = G2;
    }
    else if(G1->paroie == true || G2->paroie == true){
        if(G1->paroie == true){
            g1 = G2;
            g2 = G1;
        }
        else{
            g1 = G1;
            g2 = G2;
        }
    }

    for (int i=0; i<3; i++) {
        fn[i] = 0.0;
        fs[i] = 0.0;
    }
}

/* FONCTION AUXILLIAIRE: Produit vectoriel de deux vecteurs de doubles */
void Contact::produit_vectoriel(double V1[3], double V2[3],  double tab[3]) {
    tab[0] = V1[1]*V2[2] - V1[2]*V2[1] ;
    tab[1] = V1[2]*V2[0] - V1[0]*V2[2] ;
    tab[2] = V1[0]*V2[1] - V1[1]*V2[0] ;
}



/* met a jour les efforts de contact en 3D */
//On distingue les cas où il y a une paroie parmis les deux grains en entrée pour l'instant on suppose que le deuxième grain en entrée
//sera celui succeptible d'etre une paroie
void Contact::met_a_jour_efforts(double distance, double dt)
{
    double vitesse_rotation_relative[3];
    double tab[3];
    int i = 0;
    if(g2->paroie == true){

        for( i = 0 ; i<3 ; i++){ // pour les 3 dimensions
            n[i] = - (g2->X[i]-g1->X[i])/distance;       //vecteur direction UNITAIRE
            v[i] = - (g1->v[i] - g2->v[i]) ;             //vecteur vitesse normale ralative
            vitesse_rotation_relative[i] = (g2->omega[i]*g2->rayon + g1->omega[i]*g1->rayon);
        }

        produit_vectoriel(vitesse_rotation_relative,n,tab);

        for( i = 0 ; i<3 ; i++) v[i] += -tab[i];

        double vscaln = 0.0;
        for( i = 0 ; i<3 ; i++) vscaln += v[i]*n[i];


        for( i = 0 ; i<3 ; i++){
            vs[i] = -v[i] + vscaln*n[i] ;
            deltas[i] = vs[i]*dt ;
        }

        deltan = vscaln*dt ;

        for( i = 0; i<3 ; i++){
            delta_fn[i] = (-g2->kn * deltan - g2->cn * vscaln) * n[i];
            delta_fs[i] = - g2->ks * deltas[i] - g2->cs * vs[i];
        }
        for( i = 0; i<3 ; i++){
             fn[i] += -delta_fn[i];
             fs[i] += -delta_fs[i];
             //moment_contact_g1[i] += g1->rayon*tab[i];
        }

    }

     else {   // grains normaux

        for(i = 0; i<3 ;i++){
            n[i] = (g1->X[i] - g2->X[i]) / distance;
            v[i] = (g1->v[i] - g2->v[i]) ;
            vitesse_rotation_relative[i] = (g2->omega[i]*g2->rayon + g1->omega[i]*g1->rayon);
        }

         produit_vectoriel(vitesse_rotation_relative,n,tab);
         for( i = 0 ; i<3 ; i++) v[i] += -tab[i];

         double vscaln = 0.0;
         for( i = 0 ; i<3 ; i++) vscaln += v[i]*n[i];

         for( i = 0 ; i<3 ; i++){
            //vs[i] = v[i] - vscaln * n[i] + tab[i];
            vs[i] = v[i] - vscaln * n[i] ;
            deltas[i] = vs[i] * dt ;
        }

        deltan = vscaln * dt ;

        for( i = 0; i<3 ; i++){
             delta_fn[i] = - (g2->kn * deltan + g2->cn * vscaln) * n[i];
             delta_fs[i] = - (g2->ks * deltas[i] + g2->cs * vs[i] );
        }

        for( i = 0; i<3 ; i++){
            fn[i] +=  delta_fn[i];
            fs[i] +=  delta_fs[i];
        }


        nb_dt++;

    }
}

void Contact::reinitialise_efforts_grains(){

    for(int i = 0 ; i<3 ;i++){
         g1->f[i] = 0;
         g2->f[i] = 0;
         g1->m[i] = 0;
         g2->m[i] = 0;
    }

}



void Contact::MAJ_efforts_grains(){
    double tab[3];
    produit_vectoriel(n,fs, tab);
    for(int i = 0 ; i<3 ;i++){
        g1->f[i] +=  fn[i] + fs[i];
        g2->f[i] += -fn[i] - fs[i];
        // rayon n ^ fs
        g1->m[i] +=  -g1->rayon * tab[i];
        g2->m[i] +=  -g2->rayon * tab[i];
    }

}

/* NICO: reinitialise efforts Contacts : F_n, F_s, et moment_grain1, moment g(rain_2 */
void Contact::reinitialise_efforts_contact() {

    deltan = 0.0;

    for(int i = 0 ; i<3 ;i++){
        deltas[i] = 0.0;
        delta_fn[i] = 0.0;
        delta_fs[i] = 0.0;
        fn[i] = 0.0;
        fs[i] = 0.0;
    }

}


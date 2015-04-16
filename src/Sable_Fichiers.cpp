#include "Sable.h"

/* Ecriture coordonnées  des grains dans les fichiers contour et segment */
void Sable::ecriture_fichier(string *path){
    int i =0;
    while (g[i] != NULL) {
        ecriture_grain(g[i], *path + "contour", *path + "segment");
        i++;
    }
}

/* Le grain ecrit lui meme ses coordonnes dans les fichiers contour et segment */
void Sable::ecriture_grain(Grain* g, string fichContour, string fichSegment){

        ofstream contour(fichContour.c_str(), ios::out | ios::app);  // ouverture en écriture avec effacement du fichier ouvert
        if(contour){  // si l'ouverture a réussi
                contour << g->X[0] << " " << g->X[1] << " " << g->rayon << endl;
                contour.close();  // on ferme le fichier
        }
        else{
                cerr << "Impossible d'ouvrir le fichier !" << endl;
            }
        ofstream segment(fichSegment.c_str(), ios::out | ios::app);  // ouverture en écriture avec effacement du fichier ouvert
        if(segment) {  // si l'ouverture a réussi
            segment << g->X[0] << " " << g->X[1] << endl;
            segment << g->X[0] + (g->rayon*cos(g->teta[0])) << " " << g->X[1] + (g->rayon*sin(g->teta[1])) << endl << endl;
            segment.close();  // on ferme le fichier
        }
    else{
        cerr << "Impossible d'ouvrir le fichier !" << endl;
        }
}

/* Ecriture des instructions gnuplot dans le fichier gnutest */
void Sable::ecrire_gnuplot(string gnupl, char* image, int largeur, string *path)
{
    ofstream gnu(gnupl.c_str(), ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
    if(gnu)  // si l'ouverture a réussi
    {
        gnu << "set term pngcairo" << endl;
        gnu << "set output " << image << endl; // écriture
        gnu << "set xrange [" << -largeur << ":" << largeur << "]" << endl;
        gnu << "set yrange [" << -3*largeur/4 << ":" << 3*largeur/4 << "]" << endl;
        gnu << "plot '" << *path <<"contour'"<<" using 1:2:3 with circles, '"
            << *path << "segment'" << " using 1:2 with lines" << endl;
        gnu.close();  // on ferme le fichier
    }
    else
        cerr << "Impossible d'ouvrir le fichier !" << endl;
}


/* Affichage image + renouvellement periodique du sotck */
void Sable::afficher_image(int t, StockContacts* stock, string *path, string *path2){
            //cout << t << endl;
            int nbChiffres = 0;
            int t_temp = t;
            while (t_temp > 0){
                t_temp /= 10;
                nbChiffres++;
            }
            stringstream sstm;
            sstm << "output";
            for(int i = 1 ; i<= 5-nbChiffres ; i++){
                sstm << 0;
            }
            sstm << t << ".png";

            /* Conservation ancienne valeur path2*/
            string temp = *path2;
            temp += sstm.str().c_str();
            temp += "\"";


            /* Creation image png avec gnuplot */
            char * ans2 = (char *) temp.c_str();
            ecrire_gnuplot("gnutest",ans2,50, path);
            ecriture_fichier(path);
            system( ("gnuplot " + *path + "gnutest").c_str() ) ;

            /*Suppression segment et contour */
            system( ("rm " + *path + "segment").c_str() );
            system( ("rm " + *path + "contour").c_str() );

            /* Vider le stock de grains si nb de contacts depasse MAXCONTACTS */
            stock->vide_stock(MAXCONTACTS);
}

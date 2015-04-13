#ifndef STOCKCONTACTS_H
#define STOCKCONTACTS_H
#include "Contact.h"

#define MAXCONTACTS 20 // nb contats max dans la liste de contacts


class StockContacts
{
    public:
        StockContacts();
        virtual ~StockContacts();
        Contact* getContact();
        int setContact(Contact* C);
        void videStock(int NBCMAX);
        void videStock();
        void afficherStock();
        int getNbContacts() {return nbc;}
        void afficherListe();

        Contact* c;
        int nbc;
    //private:
};

#endif // STOCKCONTACTS_H

#ifndef STOCKCONTACTS_H
#define STOCKCONTACTS_H
#include "Contact.h"

#define MAXCONTACTS 20 // nb contats max dans la liste de contacts


class StockContacts
{
    public:
        StockContacts();
        virtual ~StockContacts();
        Contact* get_contact();
        int set_contact(Contact* C);
        void vide_stock(int NBCMAX);
        void vide_stock();
        void affiche_stock();
        void affiche_contacts_stock();

    // attributs
        Contact* c;
        int nbc;
};

#endif // STOCKCONTACTS_H

#include "StockContacts.h"
#include "Contact.h"

StockContacts::StockContacts()
{
    c = NULL;
    nbc = 0;
}

StockContacts::~StockContacts()
{
    if (c != NULL) { // vider le stock
        vide_stock();
    }
}

Contact* StockContacts::get_contact()
{
    if (c == NULL) {
        return new Contact();
    } else {
        Contact* temp = c;    // adresse premier contact
        c = c->next;            // passage au suivant
        nbc--;
        return temp;
    }

}

// Renvoie 1 => erreur, 0 si OK
int StockContacts::set_contact(Contact* C)
{
    if (C == NULL) {
        cerr << "StockContacts::setContact : Contacts NULL non ranges" << endl;
        return 1;
    } else {
        C->next = c;
        c = C;
        nbc++;
        return 0;
    }

}

// Vider le stock quoi qu'il en soit
void StockContacts::vide_stock()
{
    Contact* temp = NULL;

    while (c != NULL) {
        temp = c;
        c = c->next;
        delete temp;
    }

    nbc = 0;

}

// Vider le stock si trop de grains (cf NBCMAX)
void StockContacts::vide_stock(int NBCMAX)
{
    if (nbc >= NBCMAX) {
        Contact* temp = NULL;
        while (c != NULL) {
            temp = c;
            c = c->next;
            delete temp;
        }
        nbc = 0;
    }
}

void StockContacts::affiche_stock()
{
    cout << "Nombre de contacts stockes: " << nbc << endl;
}


void StockContacts::affiche_contacts_stock(){
    Contact* temp = c;
    int indice = 1;
    cout << "----- Afficher Stock Contacts -----" << endl;
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

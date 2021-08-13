//
// Created by simone on 12/08/21.
//

#ifndef PROGETTO_ALGORITMI_20_21_DATABASE_HPP
#define PROGETTO_ALGORITMI_20_21_DATABASE_HPP

#include "Database.h"

template<typename T>
void Database::tfstampa(vector<T> _classedati_db, const string &file_db, bool append) {
    ofstream fout;

    if (append)
        fout.open(file_db, ios::app);
    else
        fout.open(file_db, ios::out);
    controlli_file(fout, file_db);
    for (auto i : _classedati_db) {
        i->fstampa(fout);
    }
}

template<typename T>
void Database::t_aggiungi(vector<T *> &_classedati_db, const string &file_db) {
    //TODO: questa funzione puo essere un template WIP
    ifstream fin;
    fin.open(_file_argomento);
    controlli_file(fin, _file_argomento);
    string row;
    string ultimo_id = leggi_id_maggiore(file_db);
    short unsigned int n = 1;

    while (!fin.eof()) {
        getline(fin, row, '\n');
        incremento_id(ultimo_id);
        //      [Aggiunta] serve la _matricola da assegnare
        if (!row.empty()) {

            T *t = new T(row, ultimo_id);

            _classedati_db.push_back(t);

            //TODO: allo stesso modo per funzioni aggiorna?
        } else {
            cout << "[Warning] Riga " << n << " vuota nel file " << _file_argomento << '\n';
        }
        n++;
    }
    fin.close();
}

#endif //PROGETTO_ALGORITMI_20_21_DATABASE_HPP

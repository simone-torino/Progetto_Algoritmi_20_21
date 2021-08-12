//
// Created by simone on 12/08/21.
//

#ifndef PROGETTO_ALGORITMI_20_21_DATABASE_HPP
#define PROGETTO_ALGORITMI_20_21_DATABASE_HPP

#include "Database.h"

template <typename T>
void Database::tfstampa(vector<T> t, const string &file_db, bool append){
    ofstream fout;

    if (append)
        fout.open(file_db, ios::app);
    else
        fout.open(file_db, ios::out);
    controlli_file(fout, file_db);
    for (auto i : t) {
        i->fstampa(fout);
    }
}

#endif //PROGETTO_ALGORITMI_20_21_DATABASE_HPP

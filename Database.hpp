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

//questa è identica a leggi<> tranne per l'incremento_id
template<typename T>
void Database::t_aggiungi(vector<T *> &_classedati_db, const string &file_db) {
    ifstream fin;
    fin.open(_file_argomento);
    try {
        controlli_file(fin, _file_argomento);
    } catch (file_non_chiuso &e) {
        cout << "errore trovato:" << e.what() << endl;
        exit(-1);
    } catch (file_non_aperto &e) {
        cout << "errore trovato:" << e.what() << endl;
        exit(-1);
    } catch (file_failed &e) {
        cout << "errore trovato:" << e.what() << endl;
        exit(-1);
    }

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

        } else {
            cout << "[Warning] Riga " << n << " vuota nel file " << _file_argomento << '\n';
        }
        n++;
    }
    fin.close();
}

//Per aggiornamento chiama una volta con file db e una volta con file argomento
template<typename T>
void Database::leggi(const string &nome_file, vector<T *> &_classedati_xx) {
    ifstream fin;
    fin.open(nome_file);
    //TODO: try catch per gestire file db non trovato
    try {
        controlli_file(fin, nome_file);
    } catch (std::runtime_error &e) {
        cout << e.what() << endl;
        exit(3);
    }

    string row;
    unsigned short n = 1;

    while (!fin.eof()) {
        getline(fin, row, '\n');

        if (!row.empty()) {
            T *t = new T(row);

            //db o agg
            _classedati_xx.push_back(t);

        } else {
            //log(n, nome_file, 1); //warning log riga vuota nel file
            cout << "[Warning] Riga " << n << " vuota nel file " << nome_file << endl;
        }
        n++;
    }
}

//Ci ho provato
template<typename T>
void Database::aggiorna_campo(const T (*get)(), void (*set)(const T&)) {
    if (!get().empty()) {
        set(get());
        cout << "Aggiornato campo: " << get() << endl;
    }
}

//Template per controllo matricola duplicata

//Per studenti e professori
template<typename T>
void Database::t_aggiorna(vector<T *> &_classedati_db, vector<T *> &_classedati_agg, const string &file_db) {

    leggi<T>(file_db, _classedati_db);
    leggi<T>(_file_argomento, _classedati_agg);

    bool matricola_trovata = false;

    for (auto i: _classedati_agg) {
        for (auto j: _classedati_agg) {
            //salto controlli su se stesso
            if (i != j) {
                if (i->getMatricola() == j->getMatricola()) {
                    cout << "[Warning] _matricola " << i->getMatricola() << " duplicata nel file " << _file_argomento
                         << endl;
                    cout << "L' aggiornamento verrà eseguito utilizzando l'ultima occorrenza" << endl;
                }
            }
        }
        for (auto k: _classedati_db) {
            //            se trovo la _matricola da aggiornare nel vettore
            if (i->getMatricola() == k->getMatricola()) {
                //                se il campo dal vettore aggiornamento non è vuoto, aggiorno
                cout << "Matricola " << i->getMatricola() << " -> " << endl;
                if (!i->getNome().empty()) {
                    k->setNome(i->getNome());
                    cout << "Aggiornato nome: " << k->getNome() << endl;
                }
                if (!i->getCognome().empty()) {
                    k->setCognome(i->getCognome());
                    cout << "Aggiornato cognome: " << k->getCognome() << endl;
                }
                if (!i->getEmail().empty()) {
                    k->setEmail(i->getEmail());
                    cout << "Aggiornato email: " << k->getEmail() << endl;
                }
                matricola_trovata = true;
            }
        }
    }
    if (!matricola_trovata) {
        cout << "[Warning] Nessuna matricola da aggiornare è stata trovata e nessun campo e' stato aggiornato\n";
    }
}


#endif //PROGETTO_ALGORITMI_20_21_DATABASE_HPP

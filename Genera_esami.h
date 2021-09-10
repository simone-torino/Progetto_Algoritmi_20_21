#ifndef MAIN_CPP_GENERA_ESAMI_H
#define MAIN_CPP_GENERA_ESAMI_H

#define n_aule 4
#define n_slot 3
#define n_giorni_appello_1 12
#define n_giorni_appello_2 24

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include "Calendario.h"

using namespace std;

class Genera_esami {
    Calendario _cal;
    int _n_esami_raggruppati{};
    vector<string> _id_esame;
    vector<vector<string>> _id_cds;
    vector<string> _anno;
    vector<int> _n_slot_necessari;
    vector<vector<string>> _id_professori;
    vector<int> _n_vers_paral;
    int _semestre{};
    vector<vector<int>> _n_studenti_iscritti;


public:

    explicit Genera_esami(const vector<string> &argomenti); //In teoria serve per chiamare funzione calendario per il db
    Genera_esami() = default; //anche se in realtà dovrebbero fare riferimento tutti allo stesso


    bool set_id_esame_nel_calendario() const;

    void print_calendar();

private:
    calendar _cal1{};
};
class slot : public Genera_esami {

private:

    struct _info_esami_da_inserire {
        string _id_esami_da_inserire;
        int _n_studenti_iscritti;
        string _id_cds_da_inserire;
        vector<string> _id_aula_da_inserire;
        int _n_versioni_da_inserire;
        int _versione_da_inserire;
        int _vincolo_da_inserire;

    };
    vector<_info_esami_da_inserire> _info_da_inserire;

    struct _info_esami_inseriti {
        string _id_esame_inserito;
        string _id_cds_inserito;
        vector<string> _id_aula_inserita;
        int _n_versioni_inserito;
        int _versione_inserita;
        int _vincolo_inserito;
    };
    vector<_info_esami_inseriti> _info_da_stampare;

    vector<string> _id_professori_inseriti;

public:

    bool set_id_esame_nello_slot(int n_esami_raggruppati, const vector<string> &id_esame,
                                 const vector<vector<string>> &id_cds, const vector<vector<string>> &id_professori,
                                 vector<int> &n_vers_paral,
                                 const vector<vector<int>> &n_studenti_iscritti, int vincolo);

    bool maggior_n_studenti(int n_studenti_iscritti_1, int n_studenti_iscritti_2);

    void print_professori();

    void print_info();

    void print_info_warnings();

};

class giorno : public Genera_esami {

private:

    vector<slot> _fasce_orarie;
    vector<string> _id_cds_inseriti;
    vector<string> _anni_inseriti;

public:

    giorno();

    bool set_id_esame_nel_giorno(int n_esami_raggruppati, const vector<string> &id_esame,
                                 const vector<vector<string>> &id_cds, const vector<string> &anno,
                                 const vector<int> &n_slot_necessari, const vector<vector<string>> &id_professori,
                                 const vector<int> &n_vers_paral, const vector<vector<int>> &n_studenti_iscritti, int vincolo);

    void print_giorno();

    vector<string> &get_id_cds_inseriti();

    vector<string> &get_anni_inseriti();

};

class appello : public Genera_esami {

private:

    vector<giorno> _giorni;
    int _quale_appello;
    vector<string> _id_esami_inseriti;

public:

    explicit appello(int quale_appello);

    bool set_id_esame_nell_appello(int n_esami_raggruppati, const vector<string> &id_esame,
                                   const vector<vector<string>> &id_cds, const vector<string> &anno,
                                   const vector<int> &n_slot_necessari, const vector<vector<string>> &id_professori,
                                   const vector<int> &n_vers_paral, const vector<vector<int>> &n_studenti_iscritti, int vincolo);

    bool trovato_cds_anno(const vector<string> &id_cds, const string &anno, int inserisco_nel_giorno, int vincolo);

    bool prof_disponibili(const vector<string> &id_professori, int inserisco_nel_giorno, int vincolo);

    void print_appello();

    int get_quale_appello() const;

    vector<string> &get_id_esami_inseriti();

    template<class InputIterator>
    InputIterator
    find_cds_anno(InputIterator first_cds, InputIterator last_cds, InputIterator first_anno, const string &cds,
                  const string &anno);

};

class sessione : public Genera_esami {

private:

    vector<appello> _appelli;
    string _quale_sessione;

public:

    explicit sessione(const string &quale_sessione);

    bool set_id_esame_nella_sessione(int n_esami_raggruppati, const vector<string> &id_esame,
                                     const vector<vector<string>> &id_cds, const vector<string> &anno,
                                     const vector<int> &n_slot_necessari,
                                     const vector<vector<string>> &id_professori,
                                     const vector<int> &n_vers_paral,
                                     const string &semestre_dell_esame,
                                     const vector<vector<int>> &n_studenti_iscritti);

    void print_sessione();

};

class calendar : public Genera_esami {

private:

    vector<sessione> _sessioni;

public:

    calendar();

    bool set_id_esame_nel_calendario(int n_esami_raggruppati, const vector<string> &id_esame,
                                     const vector<vector<string>> &id_cds, const vector<string> &anno,
                                     const vector<int> &n_slot_necessari,
                                     const vector<vector<string>> &id_professori, const vector<int> &n_vers_paral,
                                     const string &semestre_dell_esame,
                                     const vector<vector<int>> &n_studenti_iscritti);

    void print_calendario();

};


#endif //MAIN_CPP_GENERA_ESAMI_H
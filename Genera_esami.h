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
#include "Database.h"

using namespace std;

class Genera_esami {

public:

    Genera_esami() = default;

    class slot {

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

        bool set_id_esame_nello_slot(const int n_esami_raggruppati, const vector<string> &id_esame,
                                     const vector<vector<string>> &id_cds, const vector<vector<string>> &id_professori,
                                     vector<int> &n_vers_paral,
                                     const vector<vector<int>> &n_studenti_iscritti, int vincolo);

        static bool maggior_n_studenti(int n_studenti_iscritti_1, int n_studenti_iscritti_2);

        void print_professori();

        void print_info();

        void print_info_warnings();

    };

    class giorno {

    private:

        vector<slot> _fasce_orarie;
        vector<string> _id_cds_inseriti;
        vector<string> _anni_inseriti;

    public:

        giorno();

        bool set_id_esame_nel_giorno(const int n_esami_raggruppati, const vector<string> &id_esame,
                                     const vector<vector<string>> &id_cds, const vector<string> &anno,
                                     const vector<int> &n_slot_necessari, const vector<vector<string>> &id_professori,
                                     const vector<int> &n_vers_paral, const vector<vector<int>> &n_studenti_iscritti, int vincolo);

        void print_giorno();

        vector<string> &get_id_cds_inseriti();

        vector<string> &get_anni_inseriti();

    };

    class appello {

    private:

        vector<giorno> _giorni;
        int _quale_appello;
        vector<string> _id_esami_inseriti;

    public:

        appello(const int quale_appello);

        bool set_id_esame_nell_appello(const int n_esami_raggruppati, const vector<string> &id_esame,
                                       const vector<vector<string>> &id_cds, const vector<string> &anno,
                                       const vector<int> &n_slot_necessari, const vector<vector<string>> &id_professori,
                                       const vector<int> &n_vers_paral, const vector<vector<int>> &n_studenti_iscritti, int vincolo);

        bool trovato_cds_anno(const vector<string> &id_cds, const string &anno, const int inserisco_nel_giorno, int vincolo);

        bool prof_disponibili(const vector<string> &id_professori, const int inserisco_nel_giorno, const int vincolo);

        void print_appello();

        const int get_quale_appello() const;

        vector<string> &get_id_esami_inseriti();

        template<class InputIterator>
        InputIterator
        find_cds_anno(InputIterator first_cds, InputIterator last_cds, InputIterator first_anno, const string &cds,
                      const string &anno);

    };

    class sessione {

    private:

        vector<appello> _appelli;
        string _quale_sessione;

    public:

        sessione(const string &quale_sessione);

        bool set_id_esame_nella_sessione(const int n_esami_raggruppati, const vector<string> &id_esame,
                                         const vector<vector<string>> &id_cds, const vector<string> &anno,
                                         const vector<int> &n_slot_necessari,
                                         const vector<vector<string>> &id_professori,
                                         const vector<int> &n_vers_paral,
                                         const string &semestre_dell_esame,
                                         const vector<vector<int>> &n_studenti_iscritti);

        void print_sessione();

    };

    class calendar {

    private:

        vector<sessione> _sessioni;

    public:

        calendar();

        bool set_id_esame_nel_calendario(const int n_esami_raggruppati, const vector<string> &id_esame,
                                         const vector<vector<string>> &id_cds, const vector<string> &anno,
                                         const vector<int> &n_slot_necessari,
                                         const vector<vector<string>> &id_professori, const vector<int> &n_vers_paral,
                                         const string &semestre_dell_esame,
                                         const vector<vector<int>> &n_studenti_iscritti);

        void print_calendario();

    };

    bool set_id_esame_nel_calendario(const int n_esami_raggruppati, const vector<string> &id_esame,
                                     const vector<vector<string>> &id_cds, const vector<string> &anno,
                                     const vector<int> &n_slot_necessari, const vector<vector<string>> &id_professori,
                                     const vector<int> &n_vers_paral,
                                     const string &semestre_dell_esame, const vector<vector<int>> &n_studenti_iscritti);

    void print_calendar();

private:

    calendar _cal1;
};


#endif //MAIN_CPP_GENERA_ESAMI_H
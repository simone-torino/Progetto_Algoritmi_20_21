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

using namespace std;

class Genera_esami {

public:

    Genera_esami() = default;
    class slot {

    private:

        vector<string> _id_esami;
        vector<string> _id_professori;

    public:

        bool set_id_esame_nello_slot(const string &id_esame, const vector<string> &id_professori, int n_vers_paral);

        void print_professori();

        void print_id_esami();

    };
    class giorno {

    private:

        vector<slot> _fasce_orarie;
        vector<string> _id_cds_inseriti;
        vector<string> _anni_inseriti;

    public:

        giorno();

        bool set_id_esame_nel_giorno(const string &id_esame, const vector<string> &id_cds, const vector<string> &anno,
                                     const int n_slot_necessari, const vector<string> &id_professori, int n_vers_paral);

        void print_giorno();

        vector<string> &get_id_cds_inseriti();

        vector<string> &get_anni_inseriti();

    };
    class appello {

    private:

        vector<giorno> _giorni;
        int _quale_appello;

    public:

        appello(const int quale_appello);

        bool set_id_esame_nell_appello(const string &id_esame, const vector<string> &id_cds, const vector<string> &anno,
                                       const int n_slot_necessari, const vector<string> &id_professori, int n_vers_paral);

        bool trovato_cds_anno(const vector<string> &id_cds, const vector<string> &anno, const int inserisco_nel_giorno);

        bool prof_disponibili(const vector<string> &id_professori, const int inserisco_nel_giorno);

        void print_appello();

        const int get_quale_appello() const;

        template<class InputIterator, class T>
        InputIterator find_cds_anno(InputIterator first_cds, InputIterator last_cds, InputIterator first_anno, const T &cds,
                                    const T &anno);

    };
    class sessione {

    private:

        vector<appello> _appelli;
        string _quale_sessione;

    public:

        sessione(const string &quale_sessione);

        bool set_id_esame_nella_sessione(const string &id_esame, const vector<string> &id_cds, const vector<string> &anno,
                                         const int n_slot_necessari, const vector<string> &id_professori, int n_vers_paral,
                                         const vector<string> &semestre_dell_esame);

        void print_sessione();

    };
    class calendar {

    private:

        vector<sessione> _sessioni;

    public:

        calendar();

        bool set_id_esame_nel_calendario(const string &id_esame, const vector<string> &id_cds, const vector<string> &anno,
                                         const int n_slot_necessari, const vector<string> &id_professori, int n_vers_paral,
                                         const vector<string> &semestre_dell_esame);

        void print_calendario();

    };
    bool set_id_esame_nel_calendario(const string &id_esame, const vector<string> &id_cds, const vector<string> &anno,
                                     const int n_slot_necessari, const vector<string> &id_professori, int n_vers_paral,
                                     const vector<string> &semestre_dell_esame);
    void print_calendar();

private:

    calendar _cal1;
};


#endif //MAIN_CPP_GENERA_ESAMI_H
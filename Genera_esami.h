#ifndef MAIN_CPP_GENERA_ESAMI_H
#define MAIN_CPP_GENERA_ESAMI_H

#define n_aule 100
#define n_slot 6
#define n_giorni_appello_1 12
#define n_giorni_appello_2 24

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

class Genera_esami {

public:

    Genera_esami() = default;

    class slot {

    private:

        vector<string> _id_esami_inseriti;
        vector<string> _id_professori_inseriti;

    public:

        bool set_id_esame_nello_slot(const int n_esami_raggruppati, const vector<string> &id_esame,
                                     const vector<vector<string>> &id_professori, vector<int> &n_vers_paral);

        void print_professori();

        void print_id_esami();

        void fprint_professori(ofstream &fout) const;

        void fprint_id_esami(ofstream &fout) const;
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
                                     const vector<int> &n_vers_paral);

        void print_giorno();

        vector<string> &get_id_cds_inseriti();

        vector<string> &get_anni_inseriti();

        void fprint_giorno(ofstream &fout) const;
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
                                       const vector<int> &n_vers_paral);

        bool trovato_cds_anno(const vector<string> &id_cds, const string &anno, const int inserisco_nel_giorno);

        bool prof_disponibili(const vector<string> &id_professori, const int inserisco_nel_giorno);

        void print_appello();

        const int get_quale_appello() const;

        vector<string> &get_id_esami_inseriti();

        template<class InputIterator>
        InputIterator
        find_cds_anno(InputIterator first_cds, InputIterator last_cds, InputIterator first_anno, const string &cds,
                      const string &anno);

        void fprint_appello(ofstream &fout) const;
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
                                         const string &semestre_dell_esame);

        void print_sessione();

        void fprint_sessione(ofstream &fout) const;
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
                                         const string &semestre_dell_esame);

        void print_calendario();

        void fprint_calendario() const;
    };

    bool set_id_esame_nel_calendario(const int n_esami_raggruppati, const vector<string> &id_esame,
                                     const vector<vector<string>> &id_cds, const vector<string> &anno,
                                     const vector<int> &n_slot_necessari, const vector<vector<string>> &id_professori,
                                     const vector<int> &n_vers_paral,
                                     const int &semestre);

    void print_calendar();

private:

    calendar _cal1;
};


#endif //MAIN_CPP_GENERA_ESAMI_H
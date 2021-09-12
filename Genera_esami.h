#ifndef MAIN_CPP_GENERA_ESAMI_H
#define MAIN_CPP_GENERA_ESAMI_H

#define n_aule 4
#define n_slot 6
#define n_giorni_appello_1 14
#define n_giorni_appello_2 28

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
//#include "Calendario.h"

using namespace std;

class Genera_esami {
//    Calendario _cal;
protected:
    vector<string> id_aule;
    vector<int> capienza_esame;

public:

    void setIdAule(const vector<string> &idAule);

    void setCapienzaEsame(const vector<int> &capienzaEsame);

    void set_puntatore_inizio_sessioni(int punct1, int punct2, int punct3);


//    explicit Genera_esami(const vector<string> &argomenti);
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

//        vector<string> _id_aule;
//        vector<int> _capienze;

    public:

        bool set_id_esame_nello_slot(int n_esami_raggruppati, const vector<string> &id_esame,
                                     const vector<vector<string>> &id_cds, const vector<vector<string>> &id_professori,
                                     vector<int> &n_vers_paral,
                                     const vector<vector<int>> &n_studenti_iscritti, int vincolo,
                                     const vector<string> &id_aule, const vector<int> &capienze);

        bool maggior_n_studenti(int n_studenti_iscritti_1, int n_studenti_iscritti_2);

        void print_professori();

//        void set_id_aule(const vector<string> &id_aule);

//        void set_capienza(const vector<int> &capienze);

        int get_info_esami_size();

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

        bool set_id_esame_nel_giorno(int n_esami_raggruppati, const vector<string> &id_esame,
                                     const vector<vector<string>> &id_cds, const vector<string> &anno,
                                     const vector<int> &n_slot_necessari, const vector<vector<string>> &id_professori,
                                     const vector<int> &n_vers_paral, const vector<vector<int>> &n_studenti_iscritti,
                                     int vincolo, const vector<string> &id_aule, const vector<int> &capienze);

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

        explicit appello(int quale_appello);

        bool set_id_esame_nell_appello(int n_esami_raggruppati, const vector<string> &id_esame,
                                       const vector<vector<string>> &id_cds, const vector<string> &anno,
                                       const vector<int> &n_slot_necessari, const vector<vector<string>> &id_professori,
                                       const vector<int> &n_vers_paral, const vector<vector<int>> &n_studenti_iscritti,
                                       int vincolo, const vector<string> &id_aule, const vector<int> &capienze,
                                       int puntatore_sessione, int puntatore_appello);

        bool trovato_cds_anno(const vector<string> &id_cds, const string &anno, int inserisco_nel_giorno, int vincolo);

        bool prof_disponibili(const vector<string> &id_professori, int inserisco_nel_giorno, int vincolo);

        void print_appello(int puntatore_sessione, int puntatore_appello);

        int get_quale_appello() const;

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

        explicit sessione(const string &quale_sessione);

        bool set_id_esame_nella_sessione(int n_esami_raggruppati, const vector<string> &id_esame,
                                         const vector<vector<string>> &id_cds, const vector<string> &anno,
                                         const vector<int> &n_slot_necessari,
                                         const vector<vector<string>> &id_professori,
                                         const vector<int> &n_vers_paral,
                                         const string &semestre_dell_esame,
                                         const vector<vector<int>> &n_studenti_iscritti, const vector<string> &id_aule,
                                         const vector<int> &capienze, int puntatore_sessione);

        void print_sessione(int puntatore_sessione);

        void set_quale_sessione(const string &quale_sessione);

    };

    class calendar {

    private:

        vector<sessione> _sessioni;
        int _puntatore_inizio_s1 = 22; //Contiene la data già convertita ad intero dell'inizio della prima sessione
        int _puntatore_inizio_s2 = 180; //Contiene la data già convertita ad intero dell'inizio della seconda sessione
        int _puntatore_inizio_s3 = 270; //Contiene la data già convertita ad intero dell'inizio della terza sessione

    public:

        calendar();

        bool set_id_esame_nel_calendario(int n_esami_raggruppati, const vector<string> &id_esame,
                                         const vector<vector<string>> &id_cds, const vector<string> &anno,
                                         const vector<int> &n_slot_necessari,
                                         const vector<vector<string>> &id_professori, const vector<int> &n_vers_paral,
                                         const string &semestre_dell_esame,
                                         const vector<vector<int>> &n_studenti_iscritti, const vector<string> &id_aule,
                                         const vector<int> &capienze);

        void print_calendario();

//        int get_puntatore_inizio_s1();

        void set_puntatore_inizio_s1(int punct);

//        int get_puntatore_inizio_s2();

        void set_puntatore_inizio_s2(int punct);

//        int get_puntatore_inizio_s3();

        void set_puntatore_inizio_s3(int punct);

    };

    bool set_id_esame_nel_calendario(int n_esami_raggruppati, const vector<string> &id_esame,
                                     const vector<vector<string>> &id_cds, const vector<string> &anno,
                                     const vector<int> &n_slot_necessari, const vector<vector<string>> &id_professori,
                                     const vector<int> &n_vers_paral,
                                     int &semestre, const vector<vector<int>> &n_studenti_iscritti);

    void print_calendar();

private:

    calendar _cal1;
};

#endif //MAIN_CPP_GENERA_ESAMI_H
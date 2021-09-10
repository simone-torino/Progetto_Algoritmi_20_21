
#ifndef PROGETTO_ALGORITMI_20_21_CALENDARIO_H
#define PROGETTO_ALGORITMI_20_21_CALENDARIO_H

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include "Database.h"
#include "Genera_esami.h"

//Dò per scontato che le _date siano da lunedì a domenica, quindi si conta un giorno in meno
#define SEI_SETTIMANE 6*7-1
#define QUATTRO_SETTIMANE 4*7-1

using namespace std;

//Differenza tra _date, creazione eventi (esami)

class Calendario {
    string _file_argomento;
    Database _dbcal;
    Genera_esami _gen;

public:
    explicit Calendario(const string &file_argomento);

    class myDate {

        int _day;
        int _month;
        int _year;

        friend std::ostream &operator<<(std::ostream &stream, const myDate &d);

        friend bool operator!=(const myDate &d1, const myDate &d2);

        friend bool operator==(const myDate &d1, const myDate &d2);

//        int days_in_year(int year) const;
//
//        int days_in_month(int month) const;

    public:
        myDate();

        myDate(int d, int m, int y);

        int getYear() const;

        void setData(int d, int m, int y);

        void checkDate() const;

        myDate days_to_date(int g);

        int date_to_days() const;


    };

    friend class Database;

    friend class Database::Regex;

private:

    class Periodo {
        myDate _inizio;
        myDate _fine;


        friend std::ostream &operator<<(std::ostream &stream, const Periodo &p) {
            stream << p._inizio << '|' << p._fine;
            return stream;
        }

    public:
        myDate getInizio() const;

        myDate getFine() const;

        void setPeriodo(const myDate &inizio, const myDate &fine);

        void debug() const;
    };

    class Indisponibilita {
        vector<Periodo> _date;
        string _matricola = "d";

        friend std::ostream &operator<<(std::ostream &stream, const Indisponibilita &ind) {
            stream << ind._matricola;
            for (auto i: ind._date) {
                stream << ' ' << i;
            }
            return stream;
        }

    public:
        vector<Periodo> getDate() const;
        void setMatricolaProf(const string &matricola);
        string getMatricolaProf() const;
    };

    class Anno_Accademico {
        string _primo, _secondo;

        friend std::ostream &operator<<(std::ostream &stream, const Anno_Accademico &a) {
            stream << a._primo << '-' << a._secondo;
            return stream;
        }

    public:
        string getPrimo() const;

        string getSecondo() const;

        void setAnnoAccademico(const string &inizio, const string &fine);
    };

    const string file_db_date_sessioni = "db_date_sessioni.txt";

    Anno_Accademico _anno_accademico;
    Periodo _inverno1;
    Periodo _estate2;
    Periodo _autunno3;

    vector<Indisponibilita> _ind_db;
    vector<Indisponibilita> _ind_agg;

    Database::Regex _regcal;

    //Restituisce vero se la data rientra nel periodo
    bool falls_within(Periodo p, myDate d);

    //controlli sulle _date delle sessioni
    void check_sessioni() const;

    void display_date_sessioni() const;

    void display_indisponibilita(vector<Indisponibilita> &v) const;

    void update_indisponibilita(ifstream &fin_in);

    void read_indisponibilita(ifstream &fin, vector<Indisponibilita> &v_ind);

    void check_anno_accademico(int year) const;

public:

    myDate offset(myDate &d, int n);

    vector<string> leggi_db_date_sessioni(const vector<string> &argomenti_es);

    void set_date_sessioni(const vector<string> &argomenti_sessioni, bool source_db);

    void fstampa_date_sessioni();

    void fstampa_indisponibilita();


    void set_indisponibilita(const vector<string> &argomenti_ind);

    void ordina_giorni();

    void genera_date_esami(const vector<string> &argomenti_es);

    //vector<Indisponibilita> get_indisponibilita();

private:
    //TODO: CONVIENE RACCOGLIERE QUA TUTTI I DATI NEL MODO IN CUI VERRANNO STAMPATI
//    <giorno_della_sessione>
//    <fascia oraria1>;<id_esame1>[-<numero_versione>](<id_corso_studi>);...
//    <fascia oraria2>;<id_esame1>(<id_corso_studi>);...
public:
    class GiornoSessione {
        myDate _data;  //giorno della sessione
        const string _fasce[6] = {"08:00-10:00", "10:00-12:00", "12:00-14:00", "14:00-16:00", "16:00-18:00",
                                  "18:00-20:00"};

        struct Esame {
            int _numero_versioni;
            Database::Corso_id _id_esame; //TODO: potrebbe anche essere solo string
            string _id_cds;
            bool _fascia_oraria[6]{}; //Se l'indice corrispondente è vero, l'Esame occupa quella fascia
            void fstampa_esame(ofstream &fout) const;
        };

        vector<Esame> _esami_del_giorno;

    public:
        Calendario::myDate getData();

        void fstampa_giornosessione(ofstream &fout);
    };

    class Genera_esami {
        Database _dbgen;
    public:

        Genera_esami();

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

private:
    vector<GiornoSessione> _esami_giorno;

};

//Restituisce la differenza tra le _date in giorni
int subtract(Calendario::myDate d1, Calendario::myDate d2);


#endif //PROGETTO_ALGORITMI_20_21_CALENDARIO_H

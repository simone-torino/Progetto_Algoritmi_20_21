
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
    Database getDbcal() const;
//    explicit Calendario(const string &file_argomento);
    Calendario() = default;

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
    public:


    private:
        string _matricola = "d";

        friend std::ostream &operator<<(std::ostream &stream, const Indisponibilita &ind) {
            stream << ind._matricola;
            for (auto i: ind._date) {
                stream << ' ' << i;
            }
            return stream;
        }

    public:
        void setDate(const vector<Periodo> &date);
        void setPeriodo(const Periodo &periodo);
        vector<Periodo> getDate() const;
        void setMatricolaProf(const string &matricola);
        string getMatricolaProf() const;
        void debug() const;
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

    class Dati_esame{ //Vettori da usare per ogni corso
    public:
        string anno_appartenenza;

        friend ostream &operator<<(ostream &os, const Dati_esame &esame);

        //anni_accademici del corso
        vector<string> id_cds; //id_cds che contengono il corso
        vector<string> id_professori;
        int n_versioni{};
        vector<int> n_iscritti;
        int semestre = 0; //1 primo semestre, 2 secondo semestre
        vector<string> id_corsi_raggruppati;
        int n_slot_necessari{}; //slot necessari a

        Dati_esame() = default;

         };

    void setDatiEsame(Database::Corso::Anno_Accademico *dati_anno, const string &id_esame , vector<Calendario::Dati_esame> &dati_esami);

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

private:
    vector<GiornoSessione> _esami_giorno;

};

//Restituisce la differenza tra le _date in giorni
int subtract(Calendario::myDate d1, Calendario::myDate d2);


#endif //PROGETTO_ALGORITMI_20_21_CALENDARIO_H


#ifndef PROG1_DATABASE_H
#define PROG1_DATABASE_H


#define LOG(x) std::cout << "\nLOG: " << x << std::endl;
#define LOGV(x) for(auto i: x){std::cout << "Vettore:" << ' ' << i << endl;}

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <stack>
#include <map>
#include <functional>
#include "Eccezioni.h"


using namespace std;

//Aggiungi = leggi_db da file e salva in memoria
//Scrivi = scrivi su file output i dati salvati in memoria

namespace options {
    enum procedura {
        aggiunta = 'a',
        aggiornamento = 'u',
        inserimento = 'i',
        set_esami = 's',
        gen_esami = 'g'
    };

    enum opzione {
        studenti = 's',
        professori = 'd',
        aule = 'a',
        corsi = 'c',
        cds = 'f',
        current_a = 'c', //conflitto ?
        set_av = 's'
    };
}

//DB: con _matricola/id
//IN: senza _matricola/id
namespace lettura {
    enum reg_expressions {
        studenti_db = 1,
        studenti_in = 2,
        professori_db = 3,
        professori_in = 4,
        aule_db = 5,
        aule_in = 6,
        corsi_db = 7,
        corsi_in = 8,
        cds_db = 9,
        cds_in = 10,
        sessioni = 11,
        anno_acc = 12,
        indisp = 13,
        id_corsi_in = 14,
        id_cds_in = 17,
        prof_singolo = 18,
        profn_campi = 19,
        esame_campi = 20,
        esame_graffe = 21,
        id_corsi


    };

}


class Database {
    //elenco separatori const? separatore spazi, elenco, elenco prof, elenco corsi ecc..
    const string _file_db_studenti = "db_studenti.txt";
    const string _file_db_professori = "db_professori.txt";
    const string _file_db_aule = "db_aule.txt";
    const string _file_db_corsi = "db_corsi.txt";
    const string _file_db_cds = "db_corsi_studio.txt";

    string _file_argomento;

public:
    explicit Database(const string &file_argomento);

    Database() = default;

public:
    class Regex {

        //le stringhe non const vengono modificate a runtime perchè ci sono dei campi variabili

        const string _text = "([A-Z a-z0-9]*)";
        const string _persona_in =
                _text + ';' + _text + ';' + "([a-zA-Z\\@\\._]*)"; //Alessio Maria;Rossi Aliberti;am.ra@email.it
        const string _matricola_s = "s([0-9]{6});"; //s123456;
        const string _matricola_d = "d([0-9]{6});"; //d123456;
        const string _num = "([0-9]*)";
        const string _aula_in = "([AL]);([a-z A-Z0-9]*);" + _num + ';' + _num; //A;Aula 5;120;60
        const string _id_aula = "([0-9][A-Z][A-Z][0-9]);"; //4AD4;

        string corso_db_base;
        string _id_corso;
        string _anno_acc;
        string _esame_campi;
        string _esame_graffe;

        string _id_corso_n;

        //corso di studi : C120;BS;[{AXC345,BVX123},{CBV123,ASD564}]
        string _cds;
        string _laurea;
        string _id_cds;
        string _id_corso_del_semestre_n;
        string _id_corso_del_semestre;

        //LETTURA CORSI IN
        string _corso_in_base;
        string _profn_graffe;
        string _profn_campi;

        //ESPRESSIONI PER DATE ESAMI
        string _data;
        string _periodo;
        string _indisponibilita;

        std::smatch _match;

    public:

        Regex();

        std::regex target_expression(lettura::reg_expressions exp);

        void search_and_read(const std::regex &expression, const string &row, vector<string> &out);

    };

    struct BracketSearch {
        bool IsOpenBracket(char InCharacter);

        bool IsClosedBracket(char InCharacter);

        bool balancedBrackets(const string &str);

        vector<int> posBrackets(const string &str);
    };

    class Persona {
    public:
        string getMatricola() const;

        string getNome() const;

        string getCognome() const;

        string getEmail() const;

        void setMatricola(const string &matricola);

        void setNome(const string &nome);

        void setCognome(const string &cognome);

        void setEmail(const string &email);

        char _sep;

        Persona();

    protected:
        string _matricola;
        string _nome;
        string _cognome;
        string _email;

    };

    class Studente : public Persona {
        Regex _regstud;
    public:
        Studente();

        Studente(const string &row, const string &ultima_matricola);

        Studente(const string &row);

        void fstampa(ofstream &fout) const;

        void debug() const;

        ~Studente() = default;
    };

    class Professore : public Persona {
        Regex _regprof;
    public:
        Professore();

        Professore(const string &row, const string &ultima_matricola);

        explicit Professore(const string &row);

        ~Professore();

        void fstampa(ofstream &fout) const;
    };

    class Aula {
        Regex _regaula;
    public:
        Aula();

        Aula(const string &row, const string &ultimo_id);

        explicit Aula(const string &row);

        void setId(const string &id_aula);

        void setTipo(char tipo);

        void setDenominazione(const string &denominazione);

        void setCapienza(short unsigned int capienza);

        void setCapEsame(short unsigned int capienza_esame);

        string getId() const;

        char getTipo() const;

        string getDenominazione();

        short unsigned int getCapienza() const;

        short unsigned int getCapEsame() const;

        void fstampa(ofstream &fout) const;

        ~Aula();

    protected:
        string _id_aula;
        char _tipo;
        string _denominazione;//Aula o lab
        short unsigned int _capienza;
        short unsigned int _capienza_esame;

    };

    class Corso_id {

    protected:
        string _id_corso;
    public:
        Corso_id() = default;

        explicit Corso_id(const string &row);

        void fstampa_id(ofstream &fout) const;

        void setIdCorso(const string &id_corso);

    };

    class Corso : public Corso_id {
        string _titolo; //nome del corso, può essere duplicato
        short unsigned int _cfu{};
        short unsigned int _ore_lezione{};
        short unsigned int _ore_esercitazione{};
        short unsigned int _ore_laboratorio{};
//        vector<Corso_id *> _lista_corsi_aggiuntivi;

        struct Anno_Accademico {
            string _anno_accademico;
            bool _attivo; //attivo 1, non attivo 0
            short unsigned int _n_versioni_in_parallelo;

            void fstampa_anno_accademico(ofstream &fout) const;

            struct Esame {
                short unsigned int _durata_esame;
                short unsigned int _t_ingresso;
                short unsigned int _t_uscita;
                string _modalita;
                string _luogo;

                void fstampa_esame(ofstream &fout) const;

                void setDurataEsame(unsigned short int durata_esame);

                void setTimeIn(unsigned short int t_ingresso);

                void setTimeOut(unsigned short int t_uscita);

                void setMod(string &mod);

                void setLuogo(string &luogo);
            };

            struct Prof_per_versione {
                string _matricola_titolare;

                struct Profn {
                    string _matricola;
                    short unsigned int _ore_lez;
                    short unsigned int _ore_es;
                    short unsigned int _ore_lab;

                    void setMatricolaProf(string &matricola_prof);

                    void setOreLezProf(unsigned short int ore_lez_prof);

                    void setOreEsercProf(unsigned short int ore_eser_prof);

                    void setOreLabProf(unsigned short int ore_lab_prof);

                    void fstampa_profn(ofstream &fout) const;

                };

                Database::Regex _reg_profv;
                vector<Profn *> _altri_prof_n;


                void setMatricolaTitolare(const string &matricola_titolare);

                void setProf_n(Profn *pn);

                Profn *nuovo_profn(string &prof_n);

                void fstampa_versione(ofstream &fout) const;

            };

            vector<Prof_per_versione *> _versioni;
            Esame *_esame;
            vector<Corso_id *> _id_corsi_raggruppati;

            Database::Regex _reg_anno;

            void setNumVersioni(unsigned short int num_versioni);

            void setAttivo(bool attivo);

            void setAnnoAccademico(const string &anno_accademico);

            void setEsame(Esame *e);

            Esame *nuovo_esame(const string &s_esame);

            void setProfversione(Prof_per_versione *pv);

            Prof_per_versione *nuovo_Profversione(string &versione);

            vector<string> estraimultipli(const regex &reg, string &daleggere, const string &delim);

        };

        vector<Anno_Accademico *> _anni_accademici;

        Regex _regcorso; //TODO: non c'è bisogno di specificare

        ~Corso();

    public:
        Corso();

        Corso(const string &row, const string &ultimo_id);

        Corso(const string &row);

        void setOreLez(int ore_lez);

        void setOreEser(int ore_eser);

        void setOreLab(int ore_lab);

        string getTitolo();

        int getCFU() const;

        int getOreLez() const;

        int getOreEser() const;

        int getOreLab() const;

        void setCFU(int cfu);

        void setTitolo(const string &titolo);

        void setAnnoAccademico(Corso::Anno_Accademico *anno);

        Anno_Accademico *nuovo_anno_accademico(const string &anno, int n_versioni, const string &row);

        vector<string> cut_versioni(const string &row, const vector<int> &indicigraffe, int n_versioni);

        void fstampa(ofstream &fout) const;

        Database::BracketSearch _bs;

    };

    class Corso_di_studio {
        string _id_cds;
        bool _laurea; //BS 1, MS 0
        vector<vector<Corso_id>> _corsi_per_semestre;
        vector<Corso_id> _corsi_spenti;
        vector<Corso_id> _corsi_in_un_semestre;

        Regex _regcds;

        void fstampa_semestri(ofstream &fout) const;

    public:
        Corso_di_studio();

        Corso_di_studio(const string &row, const string &ultimo_id);

        Corso_di_studio(const string &row);

        void fstampa(ofstream &fout) const;

        void setIdCds(const string &id_cds);

        void setLaurea(bool laurea);

        void setCorsiDiUnSemestre(vector<Corso_id> corsi_in_un_semestre);

        void setCorsiPerSemestre(const vector<vector<Corso_id>> &corsi_per_semestre);
    };

private:
    Regex _regdb;

    //DATI DA SCRIVERE SU FILE DATABASE
    vector<Aula *> _aule_db;
    vector<Studente *> _studenti_db;
    vector<Professore *> _professori_db;
    vector<Corso *> _corsi_db;
    vector<Corso_di_studio *> _cds_db;

    //DATI DA CONFRONTARE CON DATABASE PER AGGIORNAMENTO
    vector<Studente *> _studenti_agg;
    vector<Professore *> _professori_agg;
    vector<Aula *> _aule_agg;
    vector<Corso *> _corsi_agg;
    vector<Corso_di_studio *> _cds_agg;


    //qui il file db serve per la lettura dell'ultimo id
    template<typename T>
    void leggi_in(const string &file_db, vector<T *> &_classedati_db);

    //qui il file db serve per la rilettura di tutti i dati
    template<typename T>
    void t_aggiorna(vector<T *> &_classedati_db, vector<T *> &_classedati_agg, const string &file_db);

    template<typename T>
    void aggiorna_campo(const T (*get)(), void (*set)(const T &));

    template<typename T>
    void tfstampa(vector<T> _classedati_db, const string &file_db, bool append);

    string leggi_id_maggiore(const string &file_db);


public:
    template<typename T>
    void leggi_db(const string &nome_file, vector<T *> &_classedati_xx);


    //Per i dati da file con matricole da generare
    void target_aggiungi(options::opzione o);

    void target_aggiorna(options::opzione o);

    //Per i dati ri-letti dal _dbcal o da file di aggiornamento
    void nuovo_studente(const string &row, bool source_db);

    void nuovo_professore(const string &row, bool source_db);

    void nuova_aula(const string &row, bool source_db);

    void nuovo_corso(const string &row, bool source_db);

    void leggi_prof_db();

    void leggi_corso_db();

    //Metodi getter per passare dati alla classe calendario
    vector<Aula *> &getAuleDb();

    vector<Studente *> &getStudentiDb();

    vector<Professore *> &getProfessoriDb();

    vector<Corso *> &getCorsiDb();

    vector<Corso_di_studio *> &getCdsDb();

    const string &getFileDbStudenti() const;

    const string &getFileDbProfessori() const;

    const string &getFileDbAule() const;

    const string &getFileDbCorsi() const;

    const string &getFileDbCds() const;

//funzioni per aggiornare dati
    void aggiorna_aule();

    void inserimento_corsi();

//    con append fstampa_giornosessione può stampare sia in append che in out, sovrascrivendo tutto, utile per aggiornamento
    void target_fstampa(options::opzione o, bool append);

};

void fstampa_bool(bool b, const string &vero, const string &falso, ofstream &fout);

void controlli_file(ifstream &fin, const string &nome_file);

void controlli_file(ofstream &fout, const string &nome_file);

int strToInt(std::string const &s);

void incremento_id(string &id);


#endif //PROG1_DATABASE_H

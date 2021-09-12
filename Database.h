
#ifndef PROGETTO_ALGORITMI_20_21_DATABASE_H
#define PROGETTO_ALGORITMI_20_21_DATABASE_H


#define LOG(x) std::cout << "\nLOG: " << x << std::endl;
#define LOGV(x) for(auto i: x){std::cout << "Vettore: " << i << endl;}
#define READ_ERR(x) std::cout << "Errore lettura " << x << std::endl;

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <stack>
#include <map>
#include <functional>


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
        current_a = 'z',
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
        anno_versioni = 9,
        prof_titolare = 10,
        cds_db = 11,
        n_versioni = 12,
        cds_in = 13,
        cds_semestri = 14,
        cds_id_corso = 15,
        corsi_inserimento = 16,
        sessioni = 17,
        anno_acc = 18,
        id_prof = 19,
        prof_singolo = 20,
        profn_campi = 21,
        esame_campi = 22,
        esame_graffe = 23,
        periodo = 24,
        data = 25

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

        const string _rg_text = "([A-Z a-z0-9]*)";
        const string _rg_persona_in =
                _rg_text + ';' + _rg_text + ';' + "([a-zA-Z\\@\\._]*)"; //Alessio Maria;Rossi Aliberti;am.ra@email.it
        const string _rg_matricola_s = "s([0-9]{6});"; //s123456;
        const string _rg_matricola_d = "(d[0-9]{6})"; //d123456;
        const string _rg_num = "([0-9]*)";
        const string _rg_aula_in = "([AL]);([a-z A-Z0-9]*);" + _rg_num + ';' + _rg_num; //A;Aula 5;120;60
        const string _rg_id_aula = "([0-9][A-Z][A-Z][0-9]);"; //4AD4;

        const string _rg_versioni = R"(\{[d0-9,]+,\[[{d0-9,}]+]\})";

        const string _rg_id_corso = "([A-Z]{3}[0-9]{3})";
        const string _rg_corso_db_base =
                "c;" + _rg_id_corso + ';' + _rg_text + ';' + _rg_num + ';' + _rg_num + ';' + _rg_num + ';' + _rg_num +
                ';';
        const string _rg_n_versioni = "([0-9]*);\\[";
        const string _rg_anno_acc = "([0-9]{4})-([0-9]{4})";
        const string _rg_esame_campi = "\\{" + _rg_num + "," + _rg_num + "," + _rg_num + "," + "([SOP]*),([AL])\\}";
        const string _rg_esame_graffe = "\\{([0-9,SOPAL]+)\\}";

        //corso di studi : C120;BS;[{AXC345,BVX123},{CBV123,ASD564}]
        const string _rg_cds_row = "(BS|MS);\\[([{}A-Z0-9,]*)\\]";
        const string _rg_cds_semestri = "\\{([A-Z0-9,]+)\\}";
        const string _rg_cds_db = "(C[0-9]{3});" + _rg_cds_row;


        //LETTURA CORSI IN
        const string
                _rg_corso_in_base =
                _rg_anno_acc + ';' + _rg_text + ';' + _rg_num + ';' + _rg_num + ';' + _rg_num + ';' + _rg_num + ';' +
                _rg_num;
        const string _rg_profn_graffe = "\\{([0-9,d]+)\\}";
        const string _rg_profn_campi = "([0-9d]+),([0-9]+),([0-9]+),([0-9]+)";

        //LETTURA INSERIMENTO CORSI ABC124;2019-2020;non_attivo;3;
        const string _rg_attivo = "((?:non_)?attivo)?"; //Prende entrambe le stringhe, ma non acquisisce "non_"
        const string _rg_corso_ins = _rg_id_corso + ";" + _rg_anno_acc + ";" + _rg_attivo + ";" + _rg_num;

        //ESPRESSIONI PER DATE ESAMI
        const string _rg_data = "([0-9]{1,2})\\-([0-9]{1,2})\\-([0-9]{4})";
        const string _rg_periodo = _rg_data + "\\|" + _rg_data;

        std::smatch _match;

    public:

        std::regex target_expression(lettura::reg_expressions exp);

        void search_and_read(const std::regex &expression, const string &row, vector<string> &out);

        void multiple_fields(const std::regex &expression, const string &row, vector<string> &out);

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

        explicit Studente(const string &row);

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

        ~Corso_id();

        explicit Corso_id(const string &id_corso);

        void fstampa_id(ofstream &fout) const;

        void setIdCorso(const string &id_corso);

        const string &getIdCorso() const;

        void debug();

    };

    class Corso : public Corso_id {
    public:
        class Anno_Accademico {
        public:
            explicit Anno_Accademico(const string &row);

            void fstampa_anno_accademico(ofstream &fout) const;

            class Esame {
                short unsigned int _durata_esame;
                short unsigned int _t_ingresso;
                short unsigned int _t_uscita;
                string _modalita;
                string _luogo;

                Database::Regex _reg_esame;
            public:
                explicit Esame(const string &str_esame);

                void fstampa_esame(ofstream &fout) const;

                unsigned short getDurataEsame() const;

                unsigned short getTIngresso() const;

                unsigned short getTUscita() const;

                void debug();

                friend ostream &operator<<(ostream &os, const Esame &esame);

            };

            class Prof_per_versione {
            public:
                class Profn {
                    string _matricola;
                    short unsigned int _ore_lez;
                    short unsigned int _ore_es;
                    short unsigned int _ore_lab;

                    Database::Regex _reg_profn;
                public:
                    explicit Profn(const string &profn);

                    void fstampa_profn(ofstream &fout) const;

                    const string &getMatricola() const;

                };

                explicit Prof_per_versione(const string &versione);

                void fstampa_versione(ofstream &fout) const;

                const string &getMatricolaTitolare() const;

                const vector<Profn *> &getAltriProfN() const;

                friend ostream &operator<<(ostream &os, const Prof_per_versione &versione);

            private:

                string _matricola_titolare;
                Database::Regex _reg_profv;
                vector<Profn *> _altri_prof_n;

            };

            void setAttivo(bool attivo);

            void setEsame(Esame *e);

            Esame *getEsame() const;

            const string &getAnnoAccademico() const;

            unsigned short getNVersioniInParallelo() const;

            const vector<Prof_per_versione *> &getVersioni() const;

            const vector<Corso_id *> &getIdCorsiRaggruppati() const;

            friend ostream &operator<<(ostream &os, const Anno_Accademico &accademico);

        private:
            string _anno_accademico;
            bool _attivo; //attivo 1, non attivo 0
            short unsigned int _n_versioni_in_parallelo;
            vector<Prof_per_versione *> _versioni;
            Esame *_esame;
            vector<Corso_id *> _id_corsi_raggruppati;

            Database::Regex _reg_anno;
        };

        Corso(const string &row, const string &ultimo_id);

        explicit Corso(const string &row);

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

        void fstampa(ofstream &fout) const;

        const vector<Anno_Accademico *> &getAnniAccademici() const;

        void debug() const;

    private:
        string _titolo; //nome del corso, può essere duplicato
        short unsigned int _cfu{};
        short unsigned int _ore_lezione{};
        short unsigned int _ore_esercitazione{};
        short unsigned int _ore_laboratorio{};

        vector<Anno_Accademico *> _anni_accademici;

        Regex _regcorso; //TODO: non c'è bisogno di specificare
    };

    class Corso_di_studio {
        string _id_cds;
        bool _laurea{}; //BS 1, MS 0

        //contiene i corsi divisi per semestre, vector<corso> è un semestre, ci sono due semestri per ogni anno
        vector<vector<Corso_id *>> _corsi_semestre;

        vector<Corso_id *> _corsi_spenti;

        Regex _regcds;

        void fstampa_semestri(ofstream &fout) const;

    public:
        Corso_di_studio(const string &row, const string &ultimo_id);

        explicit Corso_di_studio(const string &row);

        const string &getIdCds() const;

        void leggi_semestri(const string &semestri);

        void fstampa(ofstream &fout) const;

        const vector<vector<Corso_id *>> &getCorsiSemestre() const;

        const vector<Corso_id *> &getCorsiSpenti() const;

        void setIdCds(const string &id_cds);

        void setLaurea(const string &laurea);

        void debug();
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

    void checkIdCorso_in_Cds();

    void isempty(ifstream &fptr);

public:
    template<typename T>
    void leggi_db(const string &nome_file, vector<T *> &_classedati_xx);


    //Per i dati da file con matricole da generare
    void target_aggiungi(options::opzione o);

    void target_aggiorna(options::opzione o);

    //Funzione per leggere il file corso_db.txt non è un template perchè deve leggere più righe per uno stesso oggetto
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


#endif //PROGETTO_ALGORITMI_20_21_DATABASE_H


#include "Database.h"
#include "Database.hpp"

//METODI CORSI E CDS

//c;AXC345;Corso del professore Odiatissimo;6;50;10;20;
//a;2019-
//2020;attivo;1;[{d123456,[{d123456,50,0,0},{d543216,10,20}]}];{90,30,30
//,SO,A};{AXC502,ASD345}
//a;2020-
//2021;attivo;2;[{d123456,[{d123456,50,0,0},{d789012,10,20}]},{d439102,
//[{d439102,50,10,0},{d789412,0,0,20}]}];{90,30,30,S,A};{AXC502,ASD345}

void Database::Corso::Anno_Accademico::Prof_per_versione::Profn::fstampa_profn(ofstream &fout) const {
    fout << '{' << _matricola << ',' << _ore_lez << ',' << _ore_es << ',' << _ore_lab << '}';
}

void Database::Corso::Anno_Accademico::Prof_per_versione::fstampa_versione(ofstream &fout) const {
    fout << '{' << _matricola_titolare << ',' << '[';
    for (int i = 0; i < _altri_prof_n.size(); i++) {
        _altri_prof_n[i]->fstampa_profn(fout);
        if (i < _altri_prof_n.size() - 1) {
            fout << ',';
        }
    }
    fout << ']' << '}';
}

void Database::Corso::Anno_Accademico::Esame::fstampa_esame(ofstream &fout) const {
    fout << '{' << _durata_esame << ',' << _t_ingresso << ',' << _t_uscita << ',' << _modalita << ',' << _luogo << '}';
}

void Database::Corso::Anno_Accademico::fstampa_anno_accademico(ofstream &fout) const {
    fout << 'a' << ';' << _anno_accademico << ';';
//    fstampa_attivo;
    fstampa_bool(_attivo, "attivo", "non_attivo", fout);
    //parentesi strane: apro campo professori, apro _matricola titolare
    fout << ';' << _n_versioni_in_parallelo << ';' << '[';
    for (int i = 0; i < _versioni.size(); i++) {
        _versioni[i]->fstampa_versione(fout);
        if (i < _versioni.size() - 1) {
            fout << ',';
        }
    }
    //parentesi strane: chiudo Prof_n, chiudo _matricola titolare, chiudo campo professori
    fout << ']' << ';';
    _esame->fstampa_esame(fout);
    for (int i = 0; i < _id_corsi_raggruppati.size(); i++) {
        _id_corsi_raggruppati[i]->fstampa_id(fout);
        if (i < _id_corsi_raggruppati.size() - 1) {
            fout << ',';
        }
    }
}

void Database::Corso::fstampa(ofstream &fout) const {
    LOG(_id_corso);
    fout << 'c' << ';' << _id_corso << ';' << _titolo << ';' << _cfu << ';' << _ore_lezione << ';' << _ore_esercitazione
         << ';' << _ore_laboratorio << ';' << '\n';
    for (const auto &i : _anni_accademici) {
        i->fstampa_anno_accademico(fout);
        fout << '\n'; //vado a capo anche se ho un solo anno
    }
}

//[{d123456,[{d123456,50,0,0},{d543216,10,20}]},...];{90,30,30,SO,A};{AXC502,ASD345}
void Database::Corso_di_studio::fstampa(ofstream &fout) const {
    fout << _id_cds << ';';
//    fstampa_laurea(fout);
    fstampa_bool(_laurea, "BS", "MS", fout);
    fout << ';' << '[';
    fstampa_semestri(fout);
    fout << ']' << ';' << '[';
    for (unsigned int i = 0; i < _corsi_spenti.size(); i++) {
        _corsi_spenti[i].fstampa_id(fout);
        //se non sto stampando l'ultimo campo, metto una virgola
        if (i != _corsi_spenti.size() - 1) {
            fout << ',';
        }
    }
    fout << ']';
}

void Database::Corso_di_studio::fstampa_semestri(ofstream &fout) const {
    fout << '{';
    for (unsigned int i = 0; i < _corsi_per_semestre.size(); i++) {
        for (int j = 0; j < _corsi_per_semestre[i].size(); j++) {
            _corsi_per_semestre[i][j].fstampa_id(fout);
            //se non sto stampando l'ultimo campo, metto una virgola
            if (i < _corsi_per_semestre[i].size() - 1) {
                fout << ',';
            }
        }
    }
    fout << '}';
}

void Database::Corso_id::fstampa_id(ofstream &fout) const {
    fout << _id_corso;
}

void fstampa_bool(bool b, const string &vero, const string &falso, ofstream &fout) {
    if (b) {
        fout << vero;
    } else {
        fout << falso;
    }
}

void Database::aggiungi(void (Database::*nuova_classe_db)(const string &, const string &)) {
//TODO: questa funzione puo essere un template
    ifstream fin;
    fin.open(_file_argomento);
    controlli_file(fin, _file_argomento);
    string row;
    int ultima_matricola_id = leggi_matricola_maggiore(_file_db_studenti);
    short unsigned int n = 1;

    while (!fin.eof()) {
        getline(fin, row, '\n');
        ultima_matricola_id++;
        //      [Aggiunta] serve la _matricola da assegnare
        if (!row.empty()) {
            (this->*nuova_classe_db)(row, to_string(ultima_matricola_id));
            //TODO: allo stesso modo per funzioni aggiorna?
        } else {
            cout << "[Warning] Riga " << n << " vuota nel file " << _file_argomento << '\n';
        }
        n++;
    }
    fin.close();
}

void Database::aggiungi_studenti() {
    ifstream fin;
    fin.open(_file_argomento);
    controlli_file(fin, _file_argomento);
    string row;
    int ultima_matricola_id = leggi_matricola_maggiore(_file_db_studenti);
    short unsigned int n = 1;

    while (!fin.eof()) {
        getline(fin, row, '\n');
        ultima_matricola_id++;
//      [Aggiunta] serve la _matricola da assegnare
        if (!row.empty()) {
            nuovo_studente(row, to_string(ultima_matricola_id));
        } else {
            cout << "[Warning] Riga " << n << " vuota nel file " << _file_argomento << '\n';
        }
        n++;
    }
    fin.close();
}

//Questa funzione legge da file in, quindi ha bisogno della _matricola generata
void Database::nuovo_studente(const string &row, const string &ultima_matricola_id) {
    auto *s = new Studente(row, ultima_matricola_id);
//    Studente *s{new(nothrow) Studente};

//TODO: questo nel costruttore?
    vector<string> studente_temp;

//   [Aggiunta] Legge da file in (lista) quindi la _matricola va assegnata
    if (!_regdb.search_and_read(_regdb.target_expression(lettura::studenti_in), row, studente_temp)) {
        cerr << "Errore formattazione file input\n";
        exit(3);
    }

    s->setMatricola(ultima_matricola_id);
    s->setNome(studente_temp[1]);
    s->setCognome(studente_temp[2]);
    s->setEmail(studente_temp[3]);

    //TODO: questo nel template??
    _studenti_db.push_back(s);
}

//Questa funzione è pensata per leggere e salvare studenti da file database
//Ma è anche usata per aggiungere al vettore agg da file aggiornamento
void Database::nuovo_studente(const string &row, bool source_db) {
    Studente *s{new(nothrow) Studente};
    vector<string> studente_temp;

//   [Aggiornamento] Legge dal file db, quindi c'è anche la _matricola, da confrontare più avanti
    if (!_regdb.search_and_read(_regdb.target_expression(lettura::studenti_db), row, studente_temp)) {
        cerr << "Errore formattazione file input _dbcal\n";
        exit(3);
    }

    //le stringhe vuote sono gestite dai metodi setter
    s->setMatricola(studente_temp[1]);
    s->setNome(studente_temp[2]);
    s->setCognome(studente_temp[3]);
    s->setEmail(studente_temp[4]);

    if (source_db) {
        _studenti_db.push_back(s);
    } else
        _studenti_agg.push_back(s);
}

void Database::aggiungi_professori() {
    ifstream fin;
    fin.open(_file_argomento);
    controlli_file(fin, _file_argomento);
    string row;
    int ultima_matricola_id = leggi_matricola_maggiore(_file_db_professori);
    unsigned short int n = 1;

    while (!fin.eof()) {
        getline(fin, row, '\n');
        ultima_matricola_id++;

        if (!row.empty()) {
            //      [Aggiunta] serve la _matricola da assegnare
            nuovo_professore(row, to_string(ultima_matricola_id));
        } else {
            cout << "[Warning] Riga " << n << " vuota nel file" << _file_argomento << '\n';
        }
        n++;
    }
}

void Database::nuovo_corso_di_studio(const string &row, const string &ultima_matricola) {
    Corso_di_studio *cds = {new Corso_di_studio};
    vector<string> cds_temp;
    vector<vector<Corso_id>> lista_corsi_temp;

    int i = 0;
    try {
        _regdb.search_and_read(_regdb.target_expression(lettura::cds_in), row, cds_temp);
    } catch (err_formattazione_errara &e) {
        cout << "errore trovato" << e.what() << endl;
        exit(3);
    }
    lista_corsi_temp = {cds_temp[1].begin(), cds_temp[cds_temp.size()].end()};

    try {
        cds->setIdCds(ultima_matricola);
        cds->setLaurea(stoi(cds_temp[1]));
        cds->setCorsiPerSemestre(lista_corsi_temp);
    } catch (errore_matricola &e) {
        cout << "errore trovato" << e.what() << endl;
        exit(4);
    } catch (invalid_argument &e) {
        cout << "errore trovato" << e.what() << endl;
        exit(10);
    }

    _cds_db.push_back(cds);
}

void Database::aggiungi_corsi_di_studio() {
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
    string ultima_matricola_id;
    try {
        ultima_matricola_id = leggi_id_maggiore(_file_db_cds);
    } catch (errore_matricola &e) {
        cout << "errore trovato" << e.what() << endl;
        exit(4);
    } catch (errore_incongruenza_file &e) {
        cout << "errore trovato" << e.what() << endl;
        exit(9);
    }
    short unsigned int n = 1;
    try {
        while (!fin.eof()) {
            getline(fin, row, '\n');
//      [Aggiunta] serve la _matricola da assegnare
            if (!row.empty()) {
                nuovo_corso_di_studio(row, matricola_cds_incremento(ultima_matricola_id));
            } else {
                throw errore_riga_vuota();
            }
            n++;
        }
    } catch (errore_matricola &e) {
        cout << "errore trovato" << e.what() << endl;
        exit(4);
    }
    fin.close();
}


int Database::leggi_matricola_maggiore(const string &file_db) {
    //Legge file _dbcal e ritorna la _matricola pù grande, da cui partire
    ifstream fin1;
    fin1.open(file_db);
    int first = 100000;

    try {
        controlli_file(fin1, file_db);
    } catch (runtime_error &e) {
        cout << e.what() << endl;
        return first;
    }
    string rowdb, temp;

    while (!fin1.eof()) {
        getline(fin1, rowdb, '\n');
        istringstream row_stream;
        row_stream.str(rowdb);
        getline(row_stream, temp, ';'); //legge la riga fino al ;

        try {
            if (!temp.empty()) {
                temp = temp.substr(1, temp.length());
                if (stoi(temp) > first) {//aggiorna _matricola maggiore
                    first = stoi(temp);
                }
            } else {
                return first;
            }
        } catch (std::invalid_argument &e) {
            cout << "Errore stoi\n";
            exit(2);
        }
    }
    return first;
}

void Database::target_aggiungi(options::opzione o) {
    switch (o) {
        case (options::studenti): {
            cout << "Aggiunta studenti in corso...\n";
            t_aggiungi<Database::Studente>(_studenti_db, _file_db_studenti);
            _studenti_db.back()->debug();
            break;
        }
        case options::professori: {
            cout << "Aggiunta professori in corso...\n";
//            t_aggiungi<Database::Professore>(_professori_db, _file_db_professori);
            break;
        }
        case options::aule: {
            cout << "Aggiunta aule in corso...\n";

            break;
        }
        case options::corsi: {
            cout << "Aggiunta corsi in corso...\n";

            break;
        }
        case options::cds: {
            cout << "Aggiunta corsi di studio in corso...\n";

            break;
        }
        default:
            cout << "Errore inserimento parametri per aggiunta\n";
            exit(12);
    }

}

void Database::target_fstampa(options::opzione o, bool append) {
    ofstream fout;

    //questa funzione serve a mantenere i dati membri privati, per non accederci dal main
    switch (o) {
        case (options::studenti): {
            cout << "Stampa studenti in corso...\n";
            tfstampa(_studenti_db, _file_db_studenti, append);
            break;
        }
        case options::professori: {
            cout << "Stampa professori in corso...\n";
            tfstampa(_professori_db, _file_db_professori, append);
            break;
        }
        case options::aule: {
            cout << "Stampa aule in corso...\n";
            tfstampa(_aule_db, _file_db_aule, append);
            break;
        }
        case options::corsi: {
            cout << "Stampa corsi in corso...\n";
            tfstampa(_corsi_db, _file_db_corsi, append);
            break;
        }
        case options::cds: {
            cout << "Stampa corsi di studio in corso...\n";
            tfstampa(_cds_db, _file_db_cds, append);
            break;
        }
        default:
            cout << "Errore inserimento parametri per stampa su file\n";
            exit(12);
    }

}

void controlli_file(ifstream &fin, const string &nome_file) {
    if (!fin.is_open()) {
        cout << "Errore file input " << nome_file << " non aperto\n";
        throw std::runtime_error("Errore apertura file input, forse non e' stato trovato");
    }
    if (!fin.good()) {
        throw std::runtime_error("Errore file input");
    }
}

void controlli_file(ofstream &fout, const string &nome_file) {
    if (!fout.is_open()) {
        cout << "Errore file output " << nome_file << " non aperto\n";
        throw std::runtime_error("Errore apertura file output");
    }
    if (!fout.good()) {
        throw std::runtime_error("Errore file output");
    }
}

void Database::aggiungi_aule() {
    ifstream fin;
    fin.open(_file_argomento);
    controlli_file(fin, _file_argomento);
    string id_aula = "1AA1";
    try {
        id_aula = leggi_id_maggiore(_file_db_aule);
    } catch (std::runtime_error &e) {
        cout << "Prima esecuzione: creazione file " << _file_db_aule << endl;
    }
    string row;

    unsigned short int n = 1;

    while (!fin.eof()) {
        getline(fin, row, '\n');

        if (!row.empty()) {
            //      [Aggiunta] serve la _matricola da assegnare
            matricola_aula_incremento(id_aula);
            nuova_aula(row, id_aula);
        } else {
            cout << "[Warning] Riga " << n << " vuota nel file" << _file_argomento << '\n';
        }
        n++;
    }
}


void Database::Aula::setId(const string &id_aula) {
    if (!id_aula.empty()) {
        _id_aula = id_aula;
    }
}

void Database::Aula::setTipo(char tipo) {
    if (tipo != '\0') {
        _tipo = tipo;
    }
}

void Database::Aula::setDenominazione(const string &denominazione) {
    if (!denominazione.empty()) {
        _denominazione = denominazione;
    }
}

void Database::Aula::setCapienza(unsigned short int capienza) {
    _capienza = capienza;
}

void Database::Aula::setCapEsame(unsigned short int capienza_esame) {
    _capienza_esame = capienza_esame;
}

void Database::Aula::fstampa(ofstream &fout) const {
    fout << _id_aula << ';';
    fout << _tipo << ';';
    fout << _denominazione << ';';
    fout << _capienza << ';';
    fout << _capienza_esame << '\n';
}

string Database::Aula::getId() const {
    return _id_aula;
}

string Database::Aula::getDenominazione() {
    return _denominazione;
}

short unsigned int Database::Aula::getCapienza() const {
    return _capienza;
}

short unsigned int Database::Aula::getCapEsame() const {
    return _capienza_esame;
}

char Database::Aula::getTipo() const {
    return _tipo;
}

Database::Aula::~Aula() {
    LOG("Aula eliminata");
    delete this;
}

void Database::matricola_aula_incremento(string &matricola) {
// ASCII:
// 57 == '9'
// 48 == '0'
// 65 == 'A'
// 90 == 'Z'

    if (matricola[3] < 57) {
        matricola[3]++;
    } else if (matricola[2] < 90) {
        matricola[2]++;
        matricola[3] = 48;
    } else if (matricola[1] < 90) {
        matricola[1]++;
        matricola[2] = 65;
        matricola[3] = 48;
    } else if (matricola[0] < 57) {
        matricola[0]++;
        matricola[1] = 65;
        matricola[2] = 65;
        matricola[3] = 48;
    } else {
        cout << "\nlimite dei corsi immagazzinabili raggiunto";
        throw runtime_error("Errore codice matricola aula\n");
    }
}

void Database::aggiorna_studenti() {
    fstream fin_db, fin_update;
    fin_db.open(_file_db_studenti);
    fin_update.open(_file_argomento);
    string row_db, row_update;
    unsigned short int n = 1;
    while (!fin_db.eof()) {
        getline(fin_db, row_db);

//      [Aggiornamento] La _matricola viene letta dal file
        if (!row_db.empty()) {
            nuovo_studente(row_db, true);
        } else {
            cout << "[Warning] Riga " << n << " vuota in _dbcal\n";
        }
        n++;
    }
    n = 1;
    while (!fin_update.eof()) {
        getline(fin_update, row_update);
//        Qui n dati vengono messi in un vettore diverso da quello di prima, così possono essere confrontati
        if (!row_update.empty()) {
            nuovo_studente(row_update, false);
        } else {
            cout << "[Warning] Riga " << n << " vuota in  update\n";
        }
        n++;
    }

    bool matricola_trovata = false;

    for (auto i: _studenti_agg) {
        for (auto j: _studenti_agg) {
            //salto controlli su se stesso
            if (i != j) {
                if (i->getMatricola() == j->getMatricola()) {
                    cout << "[Warning] _matricola " << i->getMatricola() << " duplicata nel file " << _file_argomento
                         << endl;
                }
            }
        }
        for (auto k: _studenti_db) {
//            se trovo la _matricola da aggiornare nel vettore
            if (i->getMatricola() == k->getMatricola()) {
//                se il campo dal vettore aggiornamento non è vuoto, aggiorno
                cout << "Matricola " << i->getMatricola() << " -> ";
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

void Database::nuovo_professore(const string &row, const string &ultima_matricola_id) {
    Professore *p{new Professore};
    vector<string> prof_temp;

//   [Aggiunta] Legge da file in (lista) quindi la _matricola va assegnata
    if (!_regdb.search_and_read(_regdb.target_expression(lettura::professori_in), row, prof_temp)) {
        cerr << "Errore formattazione file input prof\n";
        exit(3);
    }

    p->setMatricola(ultima_matricola_id);
    p->setNome(prof_temp[1]);
    p->setCognome(prof_temp[2]);
    p->setEmail(prof_temp[3]);

    _professori_db.push_back(p);
}

void Database::nuovo_professore(const string &row, bool source_db) {
    Professore *p{new(nothrow) Professore};
    vector<string> prof_temp;

//   [Aggiornamento] Legge dal file _dbcal, quindi c'è anche la _matricola, da confrontare più avanti
    if (!_regdb.search_and_read(_regdb.target_expression(lettura::professori_db), row, prof_temp)) {
        cerr << "Errore formattazione file input _dbcal\n";
        exit(3);
    }

    //le stringhe vuote sono gestite dai metodi setter
    p->setMatricola(prof_temp[1]);
    p->setNome(prof_temp[2]);
    p->setCognome(prof_temp[3]);
    p->setEmail(prof_temp[4]);

    if (source_db) {
        _professori_db.push_back(p);
    } else
        _professori_agg.push_back(p);
}

void Database::aggiorna_professori() {
    fstream fin_db, fin_update;
    fin_update.open(_file_argomento);
    string row_db, row_update;
    unsigned short int n = 1;

    //Per aggiornamento, usata anche nel calendario per le indisponibilità
    leggi_prof_db();

    n = 1;
    while (!fin_update.eof()) {
        getline(fin_update, row_update);
//        Qui n dati vengono messi in un vettore diverso da quello di prima, così possono essere confrontati
        if (!row_update.empty()) {
            nuovo_professore(row_update, false);
        } else {
            cout << "[Warning] Riga " << n << " vuota in  update\n";
        }
        n++;
    }

    bool matricola_trovata = false;

    for (auto i: _professori_agg) {
        for (auto j: _professori_agg) {
            //salto controlli su se stesso
            if (i != j) {
                if (i->getMatricola() == j->getMatricola()) {
                    cout << "[Warning] matricola " << i->getMatricola() << " duplicata nel file " << _file_argomento
                         << endl;
                }
            }
        }
        for (auto k: _professori_db) {
//            se trovo la _matricola da aggiornare nel vettore
            if (i->getMatricola() == k->getMatricola()) {
//                se il campo dal vettore aggiornamento non è vuoto, aggiorno
                cout << "Matricola " << i->getMatricola() << " -> ";
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

void Database::nuova_aula(const string &row, const string &ultima_matricola_id) {
    Aula *a{new(nothrow) Aula};
    vector<string> aula_temp;

//   [Aggiunta] Legge da file in (lista) quindi la _matricola va assegnata
    if (!_regdb.search_and_read(_regdb.target_expression(lettura::aule_in), row, aula_temp)) {
        cerr << "Errore formattazione aula\n";
        exit(15);
    }
    a->setId(ultima_matricola_id);
    a->setTipo(aula_temp[1][0]);
    a->setDenominazione(aula_temp[2]);
    a->setCapienza(stoi(aula_temp[3]));
    a->setCapEsame(stoi(aula_temp[3]));

    _aule_db.push_back(a);
}

void Database::nuova_aula(const string &row, bool source_db) {
    Aula *a{new(nothrow) Aula};
    vector<string> aula_temp;

//   [Aggiornamento] Legge dal file _dbcal, quindi c'è anche la _matricola, da confrontare più avanti
    if (!_regdb.search_and_read(_regdb.target_expression(lettura::aule_db), row, aula_temp)) {
        cerr << "Errore formattazione auledb o aule update\n";
        exit(15);
    }

    LOGV(aula_temp);
    vector<int> aula_temp_int;
    aula_temp_int.resize(2);
    transform(aula_temp.begin() + 4, aula_temp.begin() + 6, aula_temp_int.begin(), strToInt);

    //le stringhe vuote sono gestite dai metodi setter
    a->setId(aula_temp[1]);
    a->setTipo(aula_temp[2][0]);
    a->setDenominazione(aula_temp[3]);
    a->setCapienza(aula_temp_int[0]);
    a->setCapEsame(aula_temp_int[1]);


    if (source_db) {
        _aule_db.push_back(a);
    } else
        _aule_agg.push_back(a);
}

void Database::aggiorna_aule() {
    ifstream fin_db, fin_update;
    fin_db.open(_file_db_aule);
    fin_update.open(_file_argomento);
    try {
        controlli_file(fin_db, _file_db_aule);
        controlli_file(fin_update, _file_argomento);
    } catch (std::runtime_error &e) {
        cout << e.what() << endl;
        exit(3);
    }

    string row_db, row_update;
    unsigned short int n = 1;
    while (!fin_db.eof()) {
        getline(fin_db, row_db);

//      [Aggiornamento] La _matricola viene letta dal file
        if (!row_db.empty()) {
            nuova_aula(row_db, true);
        } else {
            cout << "[Warning] Riga " << n << " vuota in auladb\n";
        }
        n++;
    }
    n = 1;
    while (!fin_update.eof()) {
        getline(fin_update, row_update);
//        Qui n dati vengono messi in un vettore diverso da quello di prima, così possono essere confrontati
        if (!row_update.empty()) {
            nuova_aula(row_update, false);
        } else {
            cout << "[Warning] Riga " << n << " vuota in aula update\n";
        }
        n++;
    }

    bool matricola_trovata = false;

    for (auto i: _aule_agg) {
        for (auto j: _aule_agg) {
//            cout << "Matricola: " << i->getMatricola() << endl;
            //salto controlli su se stesso
            if (i != j) {
                if (i->getId() == j->getId()) {
                    cout << "[Warning]: id aula " << i->getId() << " duplicato nel file " << _file_argomento << endl;
                }
            }
        }
        for (auto k: _aule_db) {
//            se trovo la _matricola da aggiornare nel vettore
            if (i->getId() == k->getId()) {
//                se il campo dal vettore aggiornamento non è vuoto, aggiorno
                cout << "Id Aula " << i->getId() << " -> ";
                if (i->getTipo()) {
                    k->setTipo(i->getTipo());
                    cout << "Aggiornato tipo: " << k->getTipo() << endl;
                }
                if (!i->getDenominazione().empty()) {
                    k->setDenominazione(i->getDenominazione());
                    cout << "Aggiornato denominazione: " << k->getDenominazione() << endl;
                }
                if (i->getCapienza()) {
                    k->setCapienza(i->getCapienza());
                    cout << "Aggiornato capienza: " << k->getCapienza() << endl;
                }
                if (i->getCapEsame()) {
                    k->setCapEsame(i->getCapEsame());
                    cout << "Aggiornato capienza esame: " << k->getCapEsame() << endl;
                }
                matricola_trovata = true;
            }
        }
    }

    if (!matricola_trovata) {
        cout << "[Warning] Nessuna matricola da aggiornare è stata trovata e nessun campo e' stato aggiornato\n";
    }
}

void incremento_id(string &id) {
    for (unsigned int i = id.size() - 1; i > 0; i--) {
        //controllo se il carattere è un numero o una lettera
        if (isdigit(id[i])) {
            if (id[i] < '9') {
                id[i]++;
                break;
            }
        }
        if (isalpha(id[i])) {
            if (id[i] < 'Z') {
                id[i]++;
                break;
            }
        }
        //se arriva al valore massimo resetto i caratteri precedenti
        if (id[i] == 'Z' || id[i] == '9') {
            for (unsigned int k = id.size() - 1; k >= i; k--) {
                if (isdigit(id[k])) {
                    id[k] = '0';
                }
                if (isalpha(id[k])) {
                    id[k] = 'A';
                }
            }
        }
    }
}


string Database::matricola_corso_incremento(string &matricola) {
    // ASCII:
// 57 == '9'
// 48 == '0'
// 65 == 'A'
// 90 == 'Z'


    if (matricola[5] < 57) {
        matricola[5]++;
    } else if (matricola[4] < 57) {
        matricola[4]++;
        matricola[5] = 48;
    } else if (matricola[3] < 57) {
        matricola[3]++;
        matricola[4] = 48;
        matricola[5] = 48;
    } else if (matricola[2] < 57) {
        matricola[2]++;
        matricola[3] = 48;
        matricola[4] = 48;
        matricola[5] = 48;
    } else if (matricola[1] < 90) {
        matricola[1]++;
        matricola[2] = 57;
        matricola[3] = 48;
        matricola[4] = 48;
        matricola[5] = 48;
    } else if (matricola[0] < 90) {
        matricola[0]++;
        matricola[1] = 57;
        matricola[2] = 57;
        matricola[3] = 48;
        matricola[4] = 48;
        matricola[5] = 48;
    } else {
        cout << "\nlimite dei corsi immagazzinabili raggiunto";
    }
    return matricola;
}

void Database::aggiungi_corsi() {
    ifstream fin;
    fin.open(_file_argomento);
    controlli_file(fin, _file_argomento);
    string row;
    string id_corso = "ABC123";
    try {
        id_corso = leggi_id_maggiore(_file_db_corsi);
    } catch (std::runtime_error &e) {
        cout << "Prima esecuzione: creazione file " << _file_db_corsi << endl;
    }

    short unsigned int n = 1;

    while (!fin.eof()) {
        getline(fin, row, '\n');
//      [Aggiunta] serve la _matricola da assegnare
        if (!row.empty()) {
            matricola_corso_incremento(id_corso);
            nuovo_corso(row, id_corso);
        } else {
            cout << "[Warning] Riga " << n << " vuota nel file" << _file_argomento << '\n';
        }
        n++;
    }
    fin.close();

}

string Database::leggi_id_maggiore(const string &file_db) {
    //gestisce id aule e id corso e id cds
    ifstream fin;
    fin.open(file_db);
    string first;

    if ((file_db == _file_db_studenti) || (file_db == _file_db_professori)) {
        first = "100000";
    } else if (file_db == _file_db_aule) {
        first = "1AA1";
    } else if (file_db == _file_db_corsi) {
        first = "ABC123";
    } else if (file_db == _file_db_cds) {
        first = "A123";
    }

    try {
        controlli_file(fin, file_db);
    } catch (runtime_error &e) {
        cout << e.what() << endl;
        return first;
    }

    string rowdb, temp;

    while (!fin.eof()) {
        getline(fin, rowdb, '\n');
        istringstream row_stream;
        row_stream.str(rowdb);
        if ((file_db == _file_db_aule) || (file_db == _file_db_cds) || (file_db == _file_db_studenti) ||
            (file_db == _file_db_professori)) {
            getline(row_stream, temp, ';'); //legge la riga fino al ;
        } else if (file_db == _file_db_corsi) {
            getline(row_stream, temp, ';'); //Legge c;
            getline(row_stream, temp, ';'); //Legge id_corso;
        } else {
            //errore di qualche tipo
        }

        if (!temp.empty()) {
            if (temp > first) {//aggiorna _matricola maggiore
                first = temp;
            }
        } else {
            return first;
        }
    }
    fin.close();
    return first;
}

Database::Database(const string &file_argomento) {

    _file_argomento = file_argomento;

}

//DA FILE CORSO INPUT
void Database::nuovo_corso(const string &row, const string &ultima_matricola_id) {

    Corso *c{new(nothrow) Corso};
    vector<string> corso_temp;

    //LEGGE aaaa-aaaa;titolocorso;6;50;10;20;n_versioni;
    if (!_regdb.search_and_read(_regdb.target_expression(lettura::corsi_in), row, corso_temp)) {
        cout << "Errore formattazione corso\n";
        exit(3); //TODO: codice exit
    }

    if (corso_temp.size() != 9) {
        cout << "Errore lettura corso in base\n";
    }
    vector<int> corso_temp_int;
    corso_temp_int.reserve(5);
    transform(corso_temp.begin() + 4, corso_temp.end(), corso_temp_int.begin(), strToInt);

    c->setIdCorso(ultima_matricola_id);
    c->setTitolo(corso_temp[3]);
    c->setCFU(corso_temp_int[0]);
    c->setOreLez(corso_temp_int[1]);
    c->setOreEser(corso_temp_int[2]);
    c->setOreLab(corso_temp_int[3]);

    c->setAnnoAccademico(c->nuovo_anno_accademico(corso_temp[1] + '-' + corso_temp[2], corso_temp_int[4], row));

    _corsi_db.push_back(c);

}

//DA FILE CORSO DB
//c;ABC124;titoloUNO;6;50;10;20
//a;2019-2020;attivo;1;[{d123456,[{d246813,50,0,0},{d543216,20,10,20}]}];{90,30,0,30,S}
void Database::nuovo_corso(const string &row, bool source_db) {
    Corso *c{new Corso};
    vector<string> corso_temp;

//   [Aggiornamento] Legge dal file _dbcal, quindi c'è anche la _matricola, da confrontare più avanti
    if (!_regdb.search_and_read(_regdb.target_expression(lettura::corsi_db), row, corso_temp)) {
        cerr << "Errore formattazione corso db\n";
        exit(3); //TODO: codici exit
    }

    vector<int> corso_temp_int;
    corso_temp_int.reserve(4);
    transform(corso_temp.begin() + 3, corso_temp.end(), corso_temp_int.begin(), strToInt);

    c->setIdCorso(corso_temp[1]);
    c->setTitolo(corso_temp[2]);
    c->setCFU(corso_temp_int[0]);
    c->setOreLez(corso_temp_int[1]);
    c->setOreEser(corso_temp_int[2]);
    c->setOreLab(corso_temp_int[3]);

    if (source_db) {
        _corsi_db.push_back(c);
    } else
        _corsi_agg.push_back(c);

}

string Database::matricola_cds_incremento(string &matricola) {
    // ASCII:
// 57 == '9'
// 48 == '0'
// 65 == 'A'
// 90 == 'Z'

    if (matricola[3] < 57) {
        matricola[3]++;
    } else if (matricola[2] < 57) {
        matricola[2]++;
        matricola[3] = 48;
    } else if (matricola[1] < 57) {
        matricola[1]++;
        matricola[2] = 48;
        matricola[3] = 48;
    } else if (matricola[0] < 90) {
        matricola[0]++;
        matricola[1] = 48;
        matricola[2] = 48;
        matricola[3] = 48;
    } else {
        cout << "\nlimite dei corsi immagazzinabili raggiunto";
    }
    return matricola;
}

//funzione inutile
//string Database::leggi_id_cds_maggiore(const string &file_db) {
//
//    ifstream fin;
//    fin.open(file_db);
//    controlli_file(fin, file_db);
//    string first;
//
//    if (file_db == _file_db_cds) {
//        first = "ABC123";
//    } else {
//        cerr << "file aperto non idoneo alle richieste" << endl;
//    }
//    string rowdb, temp;
//
//    while (!fin.eof()) {
//        getline(fin, rowdb, '\n');
//        istringstream row_stream;
//        row_stream.str(rowdb);
//        getline(row_stream, temp, ';'); //legge la riga fino al ;
//
//        try {
//            if (!temp.empty()) {
//
//                if (temp > first) {
//                    first = temp;
//                }
//            } else {
//                return first;
//            }
//        } catch (invalid_argument &e) {
//            cout << "Errore stoi\n" << e.what() << endl;
//        }
//    }
//    return first;
//}

void Database::leggi_prof_db() {
    ifstream fin_db;
    fin_db.open(_file_db_professori);
    controlli_file(fin_db, _file_db_professori);
    string row_db;
    int n = 1;
    while (!fin_db.eof()) {
        getline(fin_db, row_db);

//      [Aggiornamento] La _matricola viene letta dal file
        if (!row_db.empty()) {
            nuovo_professore(row_db, true);
        } else {
            cout << "[Warning] Riga " << n << " vuota in " << _file_db_professori << endl;
        }
        n++;
    }
}

void Database::leggi_corso_db() {
    ifstream fin_db;
    fin_db.open(_file_db_corsi);
    controlli_file(fin_db, _file_db_corsi);
    string row_db;
    int n = 1;
    while (!fin_db.eof()) {
        getline(fin_db, row_db);

//      [Aggiornamento] L'id_corso viene letto dal file
        if (!row_db.empty()) {
            nuovo_corso(row_db, true);

        } else {
            cout << "[Warning] Riga " << n << " vuota in " << _file_db_corsi << endl;
        }
        n++;
    }
}

vector<Database::Professore *> Database::getVProfessori() {
    return _professori_db;
}


void Database::Studente::fstampa(ofstream &fout) const {
    fout << _matricola << _sep << _nome << _sep << _cognome << _sep << _email << '\n';
}

Database::Studente::Studente() {
    _matricola = "s";
    _nome = "";
    _cognome = "";
    _email = "";
    _sep = ';';
}

void Database::Studente::debug() const {
    cout << "Dati in memoria: " << _matricola << ' ' << _nome << ' ' << _cognome << ' ' << _email << endl << endl;
}

Database::Studente::Studente(const string &row, const string &ultima_matricola_id) {
//    Studente *s{new(nothrow) Studente};
    _sep = ';';

    vector<string> studente_temp;

    //   [Aggiunta] Legge da file in (lista) quindi la _matricola va assegnata
    if (!_regstud.search_and_read(_regstud.target_expression(lettura::studenti_in), row, studente_temp)) {
        cerr << "Errore formattazione file input\n";
        exit(3);
    }

    _matricola = "s" + ultima_matricola_id;
    _nome = studente_temp[1];
    _cognome = studente_temp[2];
    _email = studente_temp[3];

//    setMatricola(ultima_matricola_id);
//    setNome(studente_temp[1]);
//    setCognome(studente_temp[2]);
//    setEmail(studente_temp[3]);

//    _studenti_db.push_back(this);
}

Database::Professore::Professore() {
    _matricola = "d";
    _nome = "";
    _cognome = "";
    _email = "";
    _sep = ';';
}

void Database::Professore::fstampa(ofstream &fout) const {
    fout << _matricola << _sep << _nome << _sep << _cognome << _sep << _email << '\n';
}

Database::Professore::~Professore() {
    LOG("Professore eliminato");
    delete this;
}

void Database::Persona::setMatricola(const string &matricola) {
    if (!matricola.empty())
        _matricola += matricola;
}

string Database::Persona::getMatricola() const {
    return _matricola;
}

string Database::Persona::getNome() const {
    return _nome;
}

string Database::Persona::getCognome() const {
    return _cognome;
}

string Database::Persona::getEmail() const {
    return _email;
}

// da file aggiornamento i campi possono essere vuoti
void Database::Persona::setNome(const string &nome) {
    if (!nome.empty())
        _nome = nome;
}

void Database::Persona::setCognome(const string &cognome) {
    if (!cognome.empty())
        _cognome = cognome;
}

// da file aggiornamento i campi possono essere vuoti
void Database::Persona::setEmail(const string &email) {
    if (!email.empty())
        _email = email;
}

string Database::Corso::getTitolo() {
    return _titolo;
}

void Database::Corso::setTitolo(const string &titolo) {
    if (!titolo.empty())
        _titolo = titolo;
}

int Database::Corso::getCFU() const {
    return _cfu;
}

void Database::Corso::setCFU(int cfu) {
    _cfu = cfu;
}

int Database::Corso::getOreLez() const {
    return _ore_lezione;
}

void Database::Corso::setOreLez(int ore_lez) {
    _ore_lezione = ore_lez;
}

int Database::Corso::getOreEser() const {
    return _ore_esercitazione;
}

void Database::Corso::setOreEser(int ore_eser) {
    _ore_esercitazione = ore_eser;
}

int Database::Corso::getOreLab() const {
    return _ore_laboratorio;
}

void Database::Corso::setOreLab(int ore_lab) {
    _ore_laboratorio = ore_lab;
}

void Database::Corso::Anno_Accademico::setNumVersioni(unsigned short num_versioni) {
    _n_versioni_in_parallelo = num_versioni;
}

void Database::Corso::Anno_Accademico::Esame::setDurataEsame(unsigned short durata_esame) {
    _durata_esame = durata_esame;
}

void Database::Corso::Anno_Accademico::Esame::setTimeIn(unsigned short t_ingresso) {
    _t_ingresso = t_ingresso;
}

void Database::Corso::Anno_Accademico::Esame::setTimeOut(unsigned short t_uscita) {
    _t_uscita = t_uscita;
}

void Database::Corso::Anno_Accademico::Esame::setMod(string &mod) {
    _modalita = mod;
}

void Database::Corso::Anno_Accademico::Esame::setLuogo(string &luogo) {
    _luogo = luogo;
}

void Database::Corso::Anno_Accademico::setAnnoAccademico(const string &anno_accademico) {
    _anno_accademico = anno_accademico;
}

void Database::Corso::Anno_Accademico::Prof_per_versione::setMatricolaTitolare(const string &matricola_titolare) {
    _matricola_titolare = matricola_titolare;
}

void Database::Corso::Anno_Accademico::Prof_per_versione::Profn::setMatricolaProf(string &matricola_prof) {
    _matricola = matricola_prof;
}

void Database::Corso::Anno_Accademico::Prof_per_versione::Profn::setOreLezProf(unsigned short ore_lez_prof) {
    _ore_lez = ore_lez_prof;
}

void Database::Corso::Anno_Accademico::Prof_per_versione::Profn::setOreEsercProf(unsigned short ore_eser_prof) {
    _ore_es = ore_eser_prof;
}

void Database::Corso::Anno_Accademico::Prof_per_versione::Profn::setOreLabProf(unsigned short ore_lab_prof) {
    _ore_lab = ore_lab_prof;
}

void Database::Corso::Anno_Accademico::setAttivo(bool attivo) {
    _attivo = attivo;
}

Database::Corso::Anno_Accademico::Prof_per_versione::Profn *
Database::Corso::Anno_Accademico::Prof_per_versione::nuovo_profn(string &prof_n) {
    Profn *pn{new Profn};
    vector<string> out_profn;
    if (!_reg_profv.search_and_read(_reg_profv.target_expression(lettura::profn_campi), prof_n, out_profn)) {
        cout << "Errore lettura profn nella versione del prof " << _matricola_titolare << endl;
        exit(2);
    }

    pn->_matricola = out_profn[1];
    pn->_ore_lez = stoi(out_profn[2]);
    pn->_ore_es = stoi(out_profn[3]);
    pn->_ore_lab = stoi(out_profn[4]);

    return pn;
}

void Database::Corso::Anno_Accademico::Prof_per_versione::setProf_n(Profn *pn) {
    _altri_prof_n.push_back(pn);
}

Database::Corso::Anno_Accademico::Prof_per_versione *
Database::Corso::Anno_Accademico::nuovo_Profversione(string &versione) {
    Prof_per_versione *pv_temp{new(nothrow) Prof_per_versione};

    pv_temp->setMatricolaTitolare(versione.substr(0, versione.find(',')));

    vector<string> professori_n = estraimultipli(_reg_anno.target_expression(lettura::prof_singolo), versione, "}");

    for (string professore_n: professori_n) {
        pv_temp->setProf_n(pv_temp->nuovo_profn(professore_n));
    }
    return pv_temp;
}

void Database::Corso::Anno_Accademico::setEsame(Database::Corso::Anno_Accademico::Esame *e) {
    _esame = e;
}

Database::Corso::Anno_Accademico *
Database::Corso::nuovo_anno_accademico(const string &anno, int n_versioni, const string &row) {
    Anno_Accademico *a_temp{new(nothrow) Anno_Accademico};
    a_temp->setAnnoAccademico(anno);
    a_temp->setAttivo(true);
    a_temp->setNumVersioni(n_versioni);


    //LETTURA [{<matricola_titolare>,[{<matricolare_prof1>,n1,n2,n3},..,{<matricolare_profn>,<ore_lez>,<ore_es>,<ore_lab>}]},...]
    vector<string> versioni = cut_versioni(row, _bs.posBrackets(row),
                                           n_versioni); //Vettore che salva {..{.},{.}..},...,{..}

    for (string stringa_versione: versioni) {
        a_temp->setProfversione(a_temp->nuovo_Profversione(stringa_versione));
    }

    vector<string> out_esame;
    if (!_regcorso.search_and_read(_regcorso.target_expression(lettura::esame_graffe), row, out_esame)) {
        cout << "Errore lettura esame graffe nel corso " << _titolo << endl;
        exit(2);
    }
    string s_esame = out_esame[0];
    a_temp->setEsame(a_temp->nuovo_esame(s_esame));

    return a_temp;
}

//{90,30,30,S,A};  {<durata_esame>,<t_ingresso>,<t_uscita>,<modalità>,<luogo(A/L)>}
Database::Corso::Anno_Accademico::Esame *Database::Corso::Anno_Accademico::nuovo_esame(const string &s_esame) {
    Esame *e{new Esame};
    vector<string> out_esame;
    if (!_reg_anno.search_and_read(_reg_anno.target_expression(lettura::esame_campi), s_esame, out_esame)) {
        cout << "Errore lettura esame_campi corso\n";
    }

    vector<int> esame_int;
    esame_int.reserve(3);
    transform(out_esame.begin() + 1, out_esame.begin() + 3, esame_int.begin(), strToInt);

    e->setDurataEsame(esame_int[0]);
    e->setTimeIn(esame_int[1]);
    e->setTimeOut(esame_int[2]);
    e->setMod(out_esame[3]);
    e->setLuogo(out_esame[4]);

    return e;
}

void Database::Corso::Anno_Accademico::setProfversione(Prof_per_versione *pv) {
    _versioni.push_back(pv);
}

void Database::Corso::setAnnoAccademico(Corso::Anno_Accademico *anno) {
    _anni_accademici.push_back(anno);
}

void Database::Corso_id::setIdCorso(const string &id_corso) {
    _id_corso = id_corso;
}

void Database::Corso_di_studio::setLaurea(bool laurea) {
    _laurea = laurea;
}

int strToInt(std::string const &s) {
    std::istringstream iss(s);
    int value;

    if (!(iss >> value)) throw std::runtime_error("stringa non valida non puo essere convertita in int");

    return value;
}

void Database::Corso_di_studio::setIdCds(const string &id_cds) {
    if (!id_cds.empty())
        _id_cds = id_cds;
    if (_id_cds.size() != 4)
        throw errore_matricola();
    for (int i = 1; i < 3 + 1; i++) {
        if (!isdigit(_id_cds[i]))
            throw errore_matricola();
    }
}

void Database::Corso_di_studio::setCorsiDiUnSemestre(vector<Corso_id> corsi_in_un_semestre) {
    for (int i = 0; i < corsi_in_un_semestre.size(); i++) {
        _corsi_in_un_semestre[i] = corsi_in_un_semestre[i];
    }
}

void Database::Corso_di_studio::setCorsiPerSemestre(const vector<vector<Corso_id>> &corsi_per_semestre) {
    for (auto &i : corsi_per_semestre) {
        setCorsiDiUnSemestre(i);
    }
}
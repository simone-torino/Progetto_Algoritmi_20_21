
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
    fout << ';';
    fout << '{';
    for (int i = 0; i < _id_corsi_raggruppati.size(); i++) {
        _id_corsi_raggruppati[i]->fstampa_id(fout);
        if (i < _id_corsi_raggruppati.size() - 1) {
            fout << ',';
        }
    }
    fout << '}';
}

void Database::Corso::fstampa(ofstream &fout) const {
//    LOG(_id_corso);
//    this->debug();
    fout << 'c' << ';' << _id_corso << ';' << _titolo << ';' << _cfu << ';' << _ore_lezione << ';' << _ore_esercitazione
         << ';' << _ore_laboratorio << ';' << '\n';
    for (const auto &i: _anni_accademici) {
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
    fout << ']' << ';';
    if (!_corsi_spenti.empty()) {
        cout << '[';
        for (unsigned int i = 0; i < _corsi_spenti.size(); i++) {
            _corsi_spenti[i]->fstampa_id(fout);
            //se non sto stampando l'ultimo campo, metto una virgola
            if (i != _corsi_spenti.size() - 1) {
                fout << ',';
            }
        }
        fout << ']';
    }
    fout << endl;
}

//C120;BS;[{AXC345, BVX123},{CBV123,ASD564}];[AXC321]
void Database::Corso_di_studio::fstampa_semestri(ofstream &fout) const {
    for (unsigned int i = 0; i < _corsi_semestre.size(); i++) {
        //stampo semestre
        fout << '{';
        for (int j = 0; j < _corsi_semestre[i].size(); j++) {
            _corsi_semestre[i][j]->fstampa_id(fout);
            //se non sto stampando l'ultimo campo, metto una virgola
            if (j < _corsi_semestre[i].size() - 1) {
                fout << ',';
            }
        }
        fout << '}';
        if (i < _corsi_semestre.size() - 1) {
            fout << ',';
        }
    }

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


//Questa funzione è pensata per leggere e salvare studenti da file database
//Ma è anche usata per aggiungere al vettore agg da file aggiornamento
void Database::nuovo_studente(const string &row, bool source_db) {
    Studente *s{new(nothrow) Studente};
    vector<string> studente_temp;

//   [Aggiornamento] Legge dal file db, quindi c'è anche la _matricola, da confrontare più avanti
    try {
        _regdb.search_and_read(_regdb.target_expression(lettura::studenti_db), row, studente_temp);

    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
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

void Database::target_aggiungi(options::opzione o) {
    switch (o) {
        case (options::studenti): {
            cout << "Aggiunta studenti in corso...\n";
            //funziona anche senza specificare il tipo
            leggi_in<Database::Studente>(_file_db_studenti, _studenti_db);
            break;
        }
        case options::professori: {
            cout << "Aggiunta professori in corso...\n";
            leggi_in<Database::Professore>(_file_db_professori, _professori_db);
            break;
        }
        case options::aule: {
            cout << "Aggiunta aule in corso...\n";
            leggi_in<Database::Aula>(_file_db_aule, _aule_db);
            break;
        }
        case options::corsi: {
            cout << "Aggiunta corsi in corso...\n";
            leggi_in<Database::Corso>(_file_db_corsi, _corsi_db);
            break;
        }
        case options::cds: {
            cout << "Aggiunta corsi di studio in corso...\n";
            //leggo anche i corsi per confrontare gli id con quelli inseriti da cds
            leggi_corso_db();

            leggi_in(_file_db_cds, _cds_db);

            this->checkIdCorso_in_Cds();
            break;
        }
        default:
            cout << "Errore inserimento parametri per aggiunta\n";
            exit(12);
    }

}

void Database::target_aggiorna(options::opzione o) {
    switch (o) {
        case (options::studenti): {
            cout << "Aggiunta studenti in corso...\n";
            t_aggiorna(_studenti_db, _studenti_agg, _file_db_studenti);
            break;
        }
        case options::professori: {
            cout << "Aggiunta professori in corso...\n";
            t_aggiorna(_professori_db, _professori_agg, _file_db_professori);
            break;
        }
        case options::aule: {
            cout << "Aggiunta aule in corso...\n";
//            t_aggiorna(_aule_db, _aule_agg, _file_db_aule);
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
        cout << "Il file in input " << nome_file << " non e' stato aperto\n";
        throw file_non_aperto(); //genera l'eccezione file_non_aperto
//        throw std::runtime_error("Errore apertura file input, forse non e' stato trovato");
    }
    if (!fin.good()) {
        throw file_failed();
//        throw std::runtime_error("Errore file input");
    }
}

void controlli_file(ofstream &fout, const string &nome_file) {
    if (!fout.is_open()) {
        cout << "Il file in output " << nome_file << " non e' stato aperto\n";
        throw file_non_aperto(); //stessa eccezione vista sopra tanto scrive già il cout di che tipo è il file
    }
    if (!fout.good()) {
        throw file_failed(); // anche qui come nella funzione sopra
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

Database::Aula::Aula(const string &row, const string &ultimo_id) {
    vector<string> aula_temp;

    //   [Aggiunta] Legge da file in (lista) quindi la _matricola va assegnata
    try {
        _regaula.search_and_read(_regaula.target_expression(lettura::aule_in), row, aula_temp);
    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        exit(15);
    }

    _id_aula = ultimo_id;
    _tipo = aula_temp[1][0];
    _denominazione = aula_temp[2];
    _capienza = stoi(aula_temp[3]);
    _capienza_esame = stoi(aula_temp[4]); //TODO: è giusto 4?


}

Database::Aula::Aula(const string &row) {
    vector<string> aula_temp;
    _tipo = 0;
    _capienza_esame = 0;
    _capienza = 0;

    //   [Aggiornamento] Legge dal file _dbcal, quindi c'è anche la _matricola, da confrontare più avanti
    try {
        _regaula.search_and_read(_regaula.target_expression(lettura::aule_db), row, aula_temp);

    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        exit(15);
    }

//    LOGV(aula_temp);
    vector<int> aula_temp_int;
    aula_temp_int.resize(2);
    transform(aula_temp.begin() + 4, aula_temp.begin() + 6, aula_temp_int.begin(), strToInt);

    //le stringhe vuote sono gestite dai metodi setter
    setId(aula_temp[1]);
    setTipo(aula_temp[2][0]);
    setDenominazione(aula_temp[3]);
    setCapienza(aula_temp_int[0]);
    setCapEsame(aula_temp_int[1]);
}

//void Database::nuovo_professore(const string &row, bool source_db) {
//    Professore *p{new(nothrow) Professore};
//    vector<string> prof_temp;
//
////   [Aggiornamento] Legge dal file _dbcal, quindi c'è anche la _matricola, da confrontare più avanti
//    try {
//        _regdb.search_and_read(_regdb.target_expression(lettura::professori_db), row, prof_temp);
//
//    } catch (errore_formattazione &e) {
//        cout << e.what() << endl;
//        exit(15);
//    }
//
//    //le stringhe vuote sono gestite dai metodi setter
//    p->setMatricola(prof_temp[1]);
//    p->setNome(prof_temp[2]);
//    p->setCognome(prof_temp[3]);
//    p->setEmail(prof_temp[4]);
//
//    if (source_db) {
//        _professori_db.push_back(p);
//    } else
//        _professori_agg.push_back(p);
//}

//void Database::nuova_aula(const string &row, bool source_db) {
//    Aula *a{new(nothrow) Aula};
//    vector<string> aula_temp;
//
////   [Aggiornamento] Legge dal file _dbcal, quindi c'è anche la _matricola, da confrontare più avanti
//    try {
//        _regdb.search_and_read(_regdb.target_expression(lettura::aule_db), row, aula_temp);
//
//    } catch (errore_formattazione &e) {
//        cout << e.what() << endl;
//        exit(15);
//    }
//
//    LOGV(aula_temp);
//    vector<int> aula_temp_int;
//    aula_temp_int.resize(2);
//    transform(aula_temp.begin() + 4, aula_temp.begin() + 6, aula_temp_int.begin(), strToInt);
//
//    //le stringhe vuote sono gestite dai metodi setter
//    a->setId(aula_temp[1]);
//    a->setTipo(aula_temp[2][0]);
//    a->setDenominazione(aula_temp[3]);
//    a->setCapienza(aula_temp_int[0]);
//    a->setCapEsame(aula_temp_int[1]);
//
//
//    if (source_db) {
//        _aule_db.push_back(a);
//    } else
//        _aule_agg.push_back(a);
//}

void Database::aggiorna_aule() {

    leggi_db<Aula>(_file_db_aule, _aule_db);
    leggi_db<Aula>(_file_argomento, _aule_agg);

    bool matricola_trovata = false;

    for (auto i: _aule_agg) {
        for (auto j: _aule_agg) {
//            cout << "Matricola: " << i->getMatricola() << endl;
            //salto controlli su se stesso
            if (i != j) {
                if (i->getId() == j->getId()) {
                    cout << "[Warning]: id aula " << i->getId() << " duplicato nel file " << _file_argomento << endl;
                    cout << "L' aggiornamento verrà eseguito utilizzando l'ultima occorrenza" << endl;
                }
            }
        }
        for (auto k: _aule_db) {
//            se trovo la _matricola da aggiornare nel vettore
            if (i->getId() == k->getId()) {
//                se il campo dal vettore aggiornamento non è vuoto, aggiorno
                cout << "Id Aula " << i->getId() << " -> " << endl;
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
        cout << "[Warning] Nessun id da aggiornare è stato trovato e nessun campo e' stato aggiornato\n";
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
        first = "ABC122";
    } else if (file_db == _file_db_cds) {
        first = "C100";
    }

    cout << "Tentativo di lettura del file " << file_db << " per assegnare codice identificativo\n";

    try {
        controlli_file(fin, file_db);
    } catch (file_non_aperto &e) {
        cout << e.what() << endl;
        return first;
    } catch (file_failed &e) {
        cout << e.what() << endl;
        return first;
    }

    try {
        isempty(fin);
    } catch (runtime_error &e) {
        cout << e.what() << endl;
        exit(5);
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
        }

        if (!temp.empty()) {
            //gli id corso e cds non hanno lettere distintive davanti
            if (file_db == _file_db_cds || file_db == _file_db_corsi) {
                temp = temp.substr(0, temp.length());
            } else
                temp = temp.substr(1, temp.length());
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

//DA FILE CORSO DB
//c;ABC124;titoloUNO;6;50;10;20
//a;2019-2020;attivo;1;[{d123456,[{d246813,50,0,0},{d543216,20,10,20}]}];{90,30,0,30,S}
void Database::nuovo_corso(const string &row, bool source_db) {
    Corso *c{new Corso(row)};
    vector<string> corso_temp;

//   [Aggiornamento] Legge dal file _dbcal, quindi c'è anche la _matricola, da confrontare più avanti
    try {
        _regdb.search_and_read(_regdb.target_expression(lettura::corsi_db), row, corso_temp);

    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        exit(15);
    }

    vector<int> corso_temp_int;
    corso_temp_int.reserve(4);
    transform(corso_temp.begin() + 3, corso_temp.end(), corso_temp_int.begin(), strToInt);

//    c->setIdCorso(corso_temp[1]);
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


//void Database::leggi_prof_db() {
//    ifstream fin_db;
//    fin_db.open(_file_db_professori);
//
//    try {
//        controlli_file(fin_db, _file_db_professori);
//    } catch (file_non_aperto &e) {
//        cout << e.what() << endl;
//    } catch (file_failed &e) {
//        cout << e.what() << endl;
//    }
//
//    string row_db;
//    int n = 1;
//    while (!fin_db.eof()) {
//        getline(fin_db, row_db);
//
////      [Aggiornamento] La _matricola viene letta dal file
//        if (!row_db.empty()) {
//            nuovo_professore(row_db, true);
//        } else {
//            cout << "[Warning] Riga " << n << " vuota in " << _file_db_professori << endl;
//        }
//        n++;
//    }
//}

void Database::leggi_corso_db() {
    ifstream fin;
    fin.open(_file_db_corsi);

    try {
        controlli_file(fin, _file_db_corsi);
    } catch (file_non_aperto &e) {
        cout << e.what() << endl;
        exit(3);
    } catch (file_failed &e) {
        cout << e.what() << endl;
        exit(3);
    }

    try {
        isempty(fin);
    } catch (runtime_error &e) {
        cout << e.what() << endl;
        exit(5);
    }

    bool letto_corso = false;
    bool letto_anno = false;
    string row_db;
    int n = 1;
    while (!fin.eof()) {
        getline(fin, row_db);

//      [Aggiornamento] L'id_corso viene letto dal file
        if (!row_db.empty()) {
            if (row_db.front() == 'c') {
                if (letto_corso && !letto_anno) {
                    cout << "Errore formattazione: nel file " << _file_db_corsi
                         << " trovato corso senza informazioni sugli anni accademici\n";
                    //TODO: throw exception
                }
                //Crea corso con le informazioni base, gli anni accademici verranno aggiunti dopo
                auto *c = new Corso(row_db);
                _corsi_db.push_back(c);
                letto_corso = true;
            } else if (row_db.front() == 'a') {
                if (!letto_corso) {
                    cout << "Erroe formattazione nel file " << _file_db_corsi
                         << " trovato anno accademico senza corso di riferimento";
                    //TODO: throw exception
                }
                auto *a = new Corso::Anno_Accademico(row_db);
                _corsi_db.back()->setAnnoAccademico(a);
                letto_anno = true;
            } else {
                cout << "Errore formattazione nel file " << _file_db_corsi << " le righe devono iniziare per c; o a;\n";
                //TODO: throw exception
            }

        } else {
            cout << "[Warning] Riga " << n << " vuota in " << _file_db_corsi << endl;
        }
        n++;
    }
}

vector<Database::Aula *> &Database::getAuleDb() {
    return _aule_db;
}

vector<Database::Studente *> &Database::getStudentiDb() {
    return _studenti_db;
}

vector<Database::Professore *> &Database::getProfessoriDb() {
    return _professori_db;
}

vector<Database::Corso *> &Database::getCorsiDb() {
    return _corsi_db;
}

vector<Database::Corso_di_studio *> &Database::getCdsDb() {
    return _cds_db;
}

const string &Database::getFileDbStudenti() const {
    return _file_db_studenti;
}

const string &Database::getFileDbProfessori() const {
    return _file_db_professori;
}

const string &Database::getFileDbAule() const {
    return _file_db_aule;
}

const string &Database::getFileDbCorsi() const {
    return _file_db_corsi;
}

const string &Database::getFileDbCds() const {
    return _file_db_cds;
}

void Database::Studente::fstampa(ofstream &fout) const {
    fout << _matricola << _sep << _nome << _sep << _cognome << _sep << _email << '\n';
}

Database::Studente::Studente() {
//    _matricola = "s";
    _nome = "";
    _cognome = "";
    _email = "";
    _sep = ';';
}

void Database::Studente::debug() const {
    cout << "Dati in memoria: " << _matricola << ' ' << _nome << ' ' << _cognome << ' ' << _email << endl << endl;
}

Database::Studente::Studente(const string &row, const string &ultima_matricola) {

    vector<string> studente_temp;

    //   [Aggiunta] Legge da file in (lista) quindi la _matricola va assegnata
    try {
        _regstud.search_and_read(_regstud.target_expression(lettura::studenti_in), row, studente_temp);

    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        exit(15);
    }

    _matricola = "s" + ultima_matricola;
    _nome = studente_temp[1];
    _cognome = studente_temp[2];
    _email = studente_temp[3];
}

Database::Studente::Studente(const string &row) {
    vector<string> studente_temp;
    _matricola = "s";

    //   [Aggiornamento] Legge dal file db, quindi c'è anche la _matricola, da confrontare più avanti
    try {
        _regstud.search_and_read(_regstud.target_expression(lettura::studenti_db), row, studente_temp);

    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        exit(15);
    }

    //le stringhe vuote sono gestite dai metodi setter
    setMatricola(studente_temp[1]);
    setNome(studente_temp[2]);
    setCognome(studente_temp[3]);
    setEmail(studente_temp[4]);
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

Database::Professore::Professore(const string &row, const string &ultima_matricola) {
    vector<string> prof_temp;

    //   [Aggiunta] Legge da file in (lista) quindi la _matricola va assegnata
    try {
        _regprof.search_and_read(_regprof.target_expression(lettura::professori_in), row, prof_temp);

    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        exit(15);
    }

    _matricola = "d" + ultima_matricola;
    _nome = prof_temp[1];
    _cognome = prof_temp[2];
    _email = prof_temp[3];

}

Database::Professore::Professore(const string &row) {
    vector<string> prof_temp;
    _matricola = "d";

    //   [Aggiornamento] Legge dal file _dbcal, quindi c'è anche la _matricola, da confrontare più avanti
    try {
        _regprof.search_and_read(_regprof.target_expression(lettura::professori_db), row, prof_temp);

    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        exit(15);
    }

    //le stringhe vuote sono gestite dai metodi setter
    setMatricola(prof_temp[1]);
    setNome(prof_temp[2]);
    setCognome(prof_temp[3]);
    setEmail(prof_temp[4]);
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

Database::Persona::Persona() {
    _sep = ';';
    _matricola.reserve(6); //TODO: serve? reserve vs resize?
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


void Database::Corso::Anno_Accademico::setAttivo(bool attivo) {
    _attivo = attivo;
}

//Database::Corso::Anno_Accademico::Prof_per_versione::Profn *
//Database::Corso::Anno_Accademico::Prof_per_versione::nuovo_profn(string &prof_n) {
//    Profn *pn{new Profn(prof_n)};
//    vector<string> out_profn;
//
//    try {
//        _reg_profv.search_and_read(_reg_profv.target_expression(lettura::profn_campi), prof_n, out_profn);
//
//    } catch (errore_formattazione &e) {
//        cout << e.what() << endl;
//        exit(15);
//    }
//
//    pn->setMatricolaProf(out_profn[1]);
//    pn->setOreLezProf(stoi(out_profn[2]));
//    pn->setOreEsercProf(stoi(out_profn[3]));
//    pn->setOreLabProf(stoi(out_profn[4]));
//
//    return pn;
//}

//void Database::Corso::Anno_Accademico::Prof_per_versione::setProf_n(Profn *pn) {
//    _altri_prof_n.push_back(pn);
//}

Database::Corso::Anno_Accademico::Prof_per_versione::Prof_per_versione(const string &versione) {

    //LETTURA matricola titolare {d000001,[{d000002,50,0,0},{d000003,20,10,20}]}
    vector<string> matricola_titolare;
    try {
        _reg_profv.search_and_read(_reg_profv.target_expression(lettura::prof_titolare), versione, matricola_titolare);
    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        READ_ERR("matricola professore titolare");
        exit(15);
    }
    _matricola_titolare = matricola_titolare[1];

    //LETTURA campi professori associati {d000001,[{d000002,50,0,0},{d000003,20,10,20}]}
    vector<string> profn_temp;
    try {
        _reg_profv.multiple_fields(_reg_profv.target_expression(lettura::prof_singolo), versione, profn_temp);
    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        READ_ERR("campi professori associati");
        exit(15);
    }

    for (const string &singolo_profn: profn_temp) {
        auto *pn = new Profn(singolo_profn);
        _altri_prof_n.push_back(pn);
    }
}

Database::Corso::Anno_Accademico::Prof_per_versione::Profn::Profn(const string &profn) {

    //LETTURA campi professore associato {d000002,50,0,0}
    vector<string> out_profn;
    try {
        _reg_profn.search_and_read(_reg_profn.target_expression(lettura::profn_campi), profn, out_profn);

    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        READ_ERR("campi professore associato");
        exit(15);
    }
    vector<int> profn_int;
    profn_int.reserve(3);
    transform(out_profn.begin() + 2, out_profn.begin() + 4, profn_int.begin(), strToInt);

    _matricola = out_profn[1];
    _ore_lez = profn_int[0];
    _ore_es = profn_int[1];
    _ore_lab = profn_int[2];
}

//Database::Corso::Anno_Accademico::Prof_per_versione *
//Database::Corso::Anno_Accademico::nuovo_Profversione(string &versione) {
//    Prof_per_versione *pv_temp{new(nothrow) Prof_per_versione(versione)};
//    cout << versione << endl;
//
//    pv_temp->setMatricolaTitolare(versione.substr(0, versione.find(',')));
//
//    vector<string> professori_n = estraimultipli(_reg_anno.target_expression(lettura::prof_singolo), versione, "}");
//
//    for (string professore_n: professori_n) {
//        pv_temp->setProf_n(pv_temp->nuovo_profn(professore_n));
//    }
//    return pv_temp;
//}

void Database::Corso::Anno_Accademico::setEsame(Database::Corso::Anno_Accademico::Esame *e) {
    _esame = e;
}

//2019-2020;Informatica;3;8;35;15;3;[{d000010,[{d000011,15,28,7},{d000012,15,28,7}]},{d000013,[{d000014,15,28,7},{d000015,15,28,7}]},{d000016,[{d000017,15,28,7},{d000018,15,28,7}]}];{90,30,30,S,A};{ABC129,ABC138,ABC143,ABC148}
//a;2021-2022;attivo;1;[{d000110,[{d000111,25,8,0},{d000112,10,8,0}]}];{90,15,0,25,S};{ABC126,ABC131,ABC133,ABC135,ABC145,ABC150}
Database::Corso::Anno_Accademico::Anno_Accademico(const string &row) {
//TODO: i processi di lettura potrebbero essere dei template

    //LETTURA aaaa-aaaa 2020-2021
    vector<string> out_anno_acc;
    try {
        _reg_anno.search_and_read(_reg_anno.target_expression(lettura::anno_acc), row, out_anno_acc);
    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        READ_ERR("campo anno accademico in anno");
        exit(15);
    }
    if (strToInt(out_anno_acc[2]) - strToInt(out_anno_acc[1]) != 1) {
        cout << "Errore anno accademico non valido, inserire due anni contigui\n";
        //TODO: throw exception
        exit(29);
    }
    _anno_accademico = out_anno_acc[1] + '-' + out_anno_acc[2];


    if (regex_match(row, std::regex("a;"))) {
        if (regex_match(row, std::regex("attivo"))) {
            _attivo = true;
        } else if (regex_match(row, std::regex("non_attivo"))) {
            _attivo = false;
        } else {
            //TODO: eccezione attivo/non_attivo non trovato
        }
    } else {
        _attivo = true; //in questo caso sto leggendo file di input
    }

    vector<string> out_n_versioni;
    try {
        _reg_anno.search_and_read(_reg_anno.target_expression(lettura::n_versioni), row, out_n_versioni);
    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        READ_ERR("numero versioni in parallelo");
        exit(15);
    }
    _n_versioni_in_parallelo = strToInt(out_n_versioni[1]);



    //LETTURA [{<matricola_titolare>,[{<matricolare_prof1>,n1,n2,n3},..,{<matricolare_profn>,<ore_lez>,<ore_es>,<ore_lab>}]},...]
//    rg_versioni {[d0-9,]+,\[[{d0-9,}]+]}
    vector<string> versioni;
    try {
        _reg_anno.multiple_fields(_reg_anno.target_expression(lettura::anno_versioni), row, versioni);
    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        READ_ERR("versioni corso nell'anno accademico");
        exit(15);
    }

//    cout << "Stampo versioni del corso:\n";
//    LOGV(versioni);
    if (versioni.size() != _n_versioni_in_parallelo) {
        //TODO: throw exception
        cout << "Errore numero versioni per il corso " << row << endl;
        cout << "Versioni acquisite: " << versioni.size() << " Versioni attese: " << _n_versioni_in_parallelo << endl;
    }

    for (const string &stringa_versione: versioni) {
        auto *pv = new Prof_per_versione(stringa_versione);
        _versioni.push_back(pv);
    }

    //LETTURA {90,30,30,S,A};  {<durata_esame>,<t_ingresso>,<t_uscita>,<modalità>,<luogo(A/L)>}
    vector<string> out_esame;

    try {
        _reg_anno.search_and_read(_reg_anno.target_expression(lettura::esame_graffe), row, out_esame);

    } catch (errore_formattazione &e) {
        READ_ERR("esame");
        cout << e.what() << endl;
        exit(15);
    }

    auto *e = new Esame(out_esame[0]);
    _esame = e;

    //LETTURA {ABC126,ABC131,ABC133,ABC135,ABC145,ABC150} {<id_corso1>,..,<id_corsoN>}
    vector<string> out_raggruppati;
    try {
        _reg_anno.multiple_fields(_reg_anno.target_expression(lettura::cds_id_corso), row, out_raggruppati);
    } catch (errore_formattazione &e) {
        READ_ERR("id corsi raggruppati");
        cout << e.what() << endl;
        exit(15);
    }

    //TODO conversione da string a corso_id* potrebbe essere un template (presente anche in costruttore corso di studio)
    for (const auto &id_corso: out_raggruppati) {
        auto *c = new Corso_id(id_corso);
        _id_corsi_raggruppati.push_back(c);
    }

}


//Database::Corso::Anno_Accademico *
//Database::Corso::nuovo_anno_accademico(const string &str_anno, int n_versioni, const string &row) {
//    Anno_Accademico *a_temp{new(nothrow) Anno_Accademico(str_anno, n_versioni, row)};
////    a_temp->setAnnoAccademico(anno);
////    a_temp->setAttivo(true);
////    a_temp->setNumVersioni(n_versioni);
//
//
//    //LETTURA [{<matricola_titolare>,[{<matricolare_prof1>,n1,n2,n3},..,{<matricolare_profn>,<ore_lez>,<ore_es>,<ore_lab>}]},...]
////    vector<string> versioni = cut_versioni(row, _bs.posBrackets(row),
////                                           n_versioni); //Vettore che salva {..{.},{.}..},...,{..}
//
////    for (string stringa_versione: versioni) {
////        a_temp->setProfversione(a_temp->nuovo_Profversione(stringa_versione));
////    }
//
//    vector<string> out_esame;
//
//    try {
//        _regcorso.search_and_read(_regcorso.target_expression(lettura::esame_graffe), row, out_esame);
//
//    } catch (errore_formattazione &e) {
//        cout << e.what() << endl;
//        exit(15);
//    }
//
//    string s_esame = out_esame[0];
//    a_temp->setEsame(a_temp->nuovo_esame(s_esame));
//
//    vector<string> out_idcorso;
//    try {
//        _regcorso.search_and_read(_regcorso.target_expression(lettura::id_corsi), row, out_idcorso);
//
//    } catch (errore_formattazione &e) {
//        cout << e.what() << endl;
//        exit(15);
//    }
//
//
//    return a_temp;
//}

Database::Corso::Anno_Accademico::Esame::Esame(const string &str_esame) {
    //{90,30,30,S,A};
    vector<string> out_esame_campi;
    try {
        _reg_esame.search_and_read(_reg_esame.target_expression(lettura::esame_campi), str_esame, out_esame_campi);

    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        READ_ERR("campi esame");
        exit(15);
    }

    vector<int> esame_int;
    esame_int.reserve(4);
    transform(out_esame_campi.begin() + 1, out_esame_campi.end() - 2, esame_int.begin(), strToInt);

    _durata_esame = esame_int[0];
    _t_ingresso = esame_int[1];
    _t_uscita = esame_int[2];
    _modalita = out_esame_campi[4];
    _luogo = out_esame_campi[5];

}

void Database::Corso::Anno_Accademico::Esame::debug() {
    cout << '{' << _durata_esame << ',' << _t_ingresso << ',' << _t_uscita << ',' << _modalita << ',' << _luogo << '}'
         << endl;
}

//{90,30,30,S,A};  {<durata_esame>,<t_ingresso>,<t_uscita>,<modalità>,<luogo(A/L)>}
//Database::Corso::Anno_Accademico::Esame *Database::Corso::Anno_Accademico::nuovo_esame(const string &s_esame) {
//    Esame *e{new Esame(s_esame)};
//    vector<string> out_esame;
//    try {
//        _reg_anno.search_and_read(_reg_anno.target_expression(lettura::esame_campi), s_esame, out_esame);
//
//    } catch (errore_formattazione &e) {
//        cout << e.what() << endl;        // qui non c'era l'exit l'ho messo comunque da verificare che sia corretto
//        exit(15);
//    }
//
//    vector<int> esame_int;
//    esame_int.reserve(3);
//    transform(out_esame.begin() + 1, out_esame.begin() + 3, esame_int.begin(), strToInt);
//
//    e->setDurataEsame(esame_int[0]);
//    e->setTimeIn(esame_int[1]);
//    e->setTimeOut(esame_int[2]);
//    e->setMod(out_esame[3]);
//    e->setLuogo(out_esame[4]);
//
//    return e;
//}

//void Database::Corso::Anno_Accademico::setProfversione(Prof_per_versione *pv) {
//    _versioni.push_back(pv);
//}


void Database::Corso::setAnnoAccademico(Corso::Anno_Accademico *anno) {
    _anni_accademici.push_back(anno);
}

Database::Corso::Corso(const string &row, const string &ultimo_id) {

    vector<string> out_corso_base;
//    cout << "STO LEGGENDO " << row << endl;
    //LEGGE aaaa-aaaa;titolocorso;6;50;10;20;n_versioni
    try {
        _regcorso.search_and_read(_regcorso.target_expression(lettura::corsi_in), row, out_corso_base);

    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        READ_ERR("informazioni corso in");
        exit(15);
    }

    if (out_corso_base.size() != 9) {
        READ_ERR("informazioni corso, numero campi non corretto");
    }
    vector<int> corso_temp_int;
    corso_temp_int.reserve(5);
    transform(out_corso_base.begin() + 4, out_corso_base.end(), corso_temp_int.begin(), strToInt);

    _id_corso = ultimo_id;
    _titolo = out_corso_base[3];
    _cfu = corso_temp_int[0];
    _ore_lezione = corso_temp_int[1];
    _ore_esercitazione = corso_temp_int[2];
    _ore_laboratorio = corso_temp_int[3];

    auto *a = new Anno_Accademico(row);
    _anni_accademici.push_back(a);

}

//LEGGI DB
//c;ABC124;Analisi1;10;50;10;20;
//a;2019-2020;attivo;3;[{d000001,[{d000002,50,0,0},{d000003,20,10,0}]},{d000004,[{d000005,50,0,0},{d000006,20,10,0}]},{d000007,[{d000008,50,0,0},{d000009,20,10,0}]}];{90,30,0,30,S};{ABC128,ABC137,ABC142,ABC147}
Database::Corso::Corso(const string &row) {

    //LETTURA c;ABC124;Analisi1;10;50;10;20;
    vector<string> out_corso_base;
    try {
        _regcorso.search_and_read(_regcorso.target_expression(lettura::corsi_db), row, out_corso_base);
    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        READ_ERR("informazioni corso db");
        exit(15);
    }

    vector<int> out_corso_int;
    out_corso_int.reserve(5);
    std::transform(out_corso_base.begin() + 4, out_corso_base.end(), out_corso_int.begin(), strToInt);


    _id_corso = out_corso_base[1];
    _titolo = out_corso_base[2];
    _cfu = out_corso_int[0];
    _ore_lezione = out_corso_int[1];
    _ore_esercitazione = out_corso_int[2];
    _ore_laboratorio = out_corso_int[3];

}


void Database::Corso::debug() const {
    cout << 'c' << ';' << _id_corso << ';' << _titolo << ';' << _cfu << ';' << _ore_lezione << ';' << _ore_esercitazione
         << ';' << _ore_laboratorio << '\n';

}

void Database::Corso_id::setIdCorso(const string &id_corso) {
    _id_corso = id_corso;
}

Database::Corso_id::Corso_id(const string &id_corso) {
    if (!id_corso.empty()) {
        if (regex_match(id_corso, std::regex("([A-Z]{3}[0-9]{3})"))) {
            _id_corso = id_corso;
        } else {
            throw errore_formattazione();
        }
    } else {
        throw errore_riga_vuota();
    }


}

void Database::Corso_id::debug() {
    cout << _id_corso << endl;
}

const string &Database::Corso_id::getIdCorso() const {
    return _id_corso;
}

Database::Corso_id::~Corso_id() {
    delete this;
}

void Database::Corso_di_studio::setLaurea(const string &laurea) {
    if (laurea == "BS") {
        _laurea = true;
    } else if (laurea == "MS") {
        _laurea = false;
    } else {
        cout << "Laurea non riconosciuta, inserire BS o MS\n";
        throw errore_formattazione();
    }

}

int strToInt(std::string const &s) {
    std::istringstream iss(s);
    int value;
    //TODO: eccezione da mettere in eccezioni.h
    if (!(iss >> value)) throw std::runtime_error("stringa non valida non puo essere convertita in int");

    return value;
}

void Database::Corso_di_studio::setIdCds(const string &id_cds) {
    //TODO: fai con regex match
    if (!id_cds.empty())
        _id_cds = id_cds;
    if (_id_cds.size() != 4)
        throw errore_matricola();
    for (int i = 1; i < 3 + 1; i++) {
        if (!isdigit(_id_cds[i]))
            throw errore_matricola();
    }
}

//void Database::Corso_di_studio::setCorsiDiUnSemestre(vector<Corso_id> corsi_in_un_semestre) {
//    for (int i = 0; i < corsi_in_un_semestre.size(); i++) {
//        _corsi_in_un_semestre[i] = corsi_in_un_semestre[i];
//    }
//}
//
//void Database::Corso_di_studio::setCorsiPerSemestre(const vector<vector<Corso_id>> &corsi_per_semestre) {
//    for (auto &i: corsi_per_semestre) {
//        setCorsiDiUnSemestre(i);
//    }
//}

void Database::checkIdCorso_in_Cds() {
//TODO probabilmente esiste un algoritmo per fare questo in due righe
    bool trovato = false;
    for (auto cds: _cds_db) {
        for (const auto &semestre: cds->getCorsiSemestre()) {
            for (auto idcorso_cds: semestre) {
                for (auto idcorso: _corsi_db) {
                    if (idcorso_cds->getIdCorso() == idcorso->getIdCorso()) {
                        trovato = true;
                    }
                }
                if (!trovato) {
                    cout << "Errore corso " << idcorso_cds->getIdCorso() << " non trovato\n";
                    //TODO: throw exception
                    exit(43);
                }
                trovato = false;
            }
        }
        for (auto id_spento: cds->getCorsiSpenti()) {
            for (auto idcorso: _corsi_db) {
                if (id_spento->getIdCorso() == idcorso->getIdCorso()) {
                    trovato = true;
                }
            }
            if (!trovato) {
                cout << "Errore corso spento " << id_spento->getIdCorso() << " non trovato\n";
                //TODO: throw exception
                exit(43);
            }
            trovato = false;
        }

    }

}

// legge BS;[{ABC123,ABC124},{ABC125,ABC126,ABC127},{ABC128,ABC129},{ABC130,ABC135,ABC136},{ABC147,ABC148},{ABC149,ABC150,ABC151}]
Database::Corso_di_studio::Corso_di_studio(const string &row, const string &ultimo_id) {
    //TODO: nei corsi di studio ci devono essere solo id_corso di corsi presenti in memoria (rileggi corsi_db ogni volta) ma leggi_db solo corso_id
    //TODO: leggi_db bs/l e trasforma in booleano

    vector<string> cds_temp;

    try {
        _regcds.search_and_read(_regcds.target_expression(lettura::cds_in), row, cds_temp);
//cds_temp[1]: BS
//cds_temp[2]: {ABC123,ABC124},{ABC125,ABC126,ABC127},{ABC128,ABC129},{ABC130,ABC135,ABC136},{ABC147,ABC148},{ABC149,ABC150,ABC151}

    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        exit(15);
    }
    //    for (int i = 0; i < cds_temp.size(); i++) {
//        cout << "Vettore " << i << " : " << cds_temp[i] << endl;
//    }

    try {
        setIdCds(ultimo_id);
        setLaurea(cds_temp[1]); // BS o MS}
    } catch (invalid_argument &e) {
        cout << "errore trovato " << e.what() << endl;
        exit(10);
    } catch (errore_matricola &e) { //lanciato dal costruttore
        cout << "errore trovato " << e.what() << endl;
        exit(4);
    }

    leggi_semestri(cds_temp[2]);

}

Database::Corso_di_studio::Corso_di_studio(const string &row) {
    //LETTURA C101;BS;[{ABC123,ABC124},{ABC125,ABC126,ABC127},{ABC128,ABC129},{ABC130,ABC131,ABC132},{ABC137,ABC138},{ABC139,ABC140,ABC141}];
    vector<string> cds_temp;

    try {
        _regcds.search_and_read(_regcds.target_expression(lettura::cds_db), row, cds_temp);
        //cds_temp[1]: C000
        //cds_temp[2]: BS
        //cds_temp[3]: {ABC123,ABC124},{ABC125,ABC126,ABC127},{ABC128,ABC129},{ABC130,ABC135,ABC136},{ABC147,ABC148},{ABC149,ABC150,ABC151}

    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        exit(15);
    }

    setIdCds(cds_temp[1]);
    setLaurea(cds_temp[2]);
    leggi_semestri(cds_temp[3]);

}

void Database::Corso_di_studio::leggi_semestri(const string &semestri) {

    vector<string> cds_temp;
    try {
        //TODO: catch delle eccezioni di multiple fields
        //ora leggo gli id dei corsi divisi per semestre
        _regcds.multiple_fields(_regcds.target_expression(lettura::cds_semestri), semestri, cds_temp);
//        {ABC123,ABC124} ecc
    } catch (errore_formattazione &e) {
        cout << "errore trovato " << e.what() << endl;
    }

//    for (int i = 0; i < cds_temp.size(); i++) {
//        cout << "Vettore " << i << " : " << cds_temp[i] << endl;
//    }

    vector<string> str_semestre_temp;
    //ora leggo i singoli id_corso per ogni semestre
    for (const auto &k: cds_temp) {
        _regcds.multiple_fields(_regcds.target_expression(lettura::cds_id_corso), k, str_semestre_temp);
//        ABC123 ecc
//        LOGV(str_semestre_temp);

        //Faccio un vettore di corso_id temporaneo per poi salvarlo nel vettore _corsi_semestre
        vector<Corso_id *> semestre_temp;
        for (const auto &id_corso_letto: str_semestre_temp) {
            //oggetto corso id da salvare nel vettore di corsi_id
            auto *c = new Corso_id(id_corso_letto);
//            cout << "Debug corso_id: ";
//            c->debug();
            semestre_temp.push_back(c);
        }
        _corsi_semestre.push_back(semestre_temp);

        semestre_temp.clear();
        str_semestre_temp.clear();

    }
}

const vector<vector<Database::Corso_id *>> &Database::Corso_di_studio::getCorsiSemestre() const {
    return _corsi_semestre;
}

const vector<Database::Corso_id *> &Database::Corso_di_studio::getCorsiSpenti() const {
    return _corsi_spenti;
}





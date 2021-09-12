
#include "Calendario.h"
#include "Eccezioni.h"
#include "Database.hpp"


void Calendario::myDate::checkDate() const {
    {
        if (_day < 1 || _day > 31) {
            cout << "Giorno " << _day << " non valido\n";
            throw range_error("Errore giorno");
        }
        if (_month < 1 || _month > 12) {
            cout << "Mese " << _month << " non valido\n";
            throw range_error("Errore mese");
        }
        if (_year < 1 || _year > 5000) {
            cout << "Anno " << _year << " non valido\n";
            throw range_error("Errore anno");
        }

    }
}

int Calendario::myDate::date_to_days() const {

    //altrimenti i membri vengono modificati
    int d = _day, m = _month, y = _year;

    m = (m + 9) % 12;
    y = y - m / 10;
    return 365 * y + y / 4 - y / 100 + y / 400 + (m * 306 + 5) / 10 + (d - 1);
}

Calendario::myDate Calendario::myDate::days_to_date(int g) {
    int y = (10000 * g + 14780) / 3652425;
    int ddd = g - (365 * y + y / 4 - y / 100 + y / 400);
    if (ddd < 0) {
        y = y - 1;
        ddd = g - (365 * y + y / 4 - y / 100 + y / 400);
    }
    int mi = (100 * ddd + 52) / 3060;
    int mm = (mi + 2) % 12 + 1;
    y = y + (mi + 2) / 12;
    int dd = ddd - (mi * 306 + 5) / 10 + 1;

    _year = y;
    _month = mm;
    _day = dd;

    return *this;
}

Calendario::myDate::myDate() {
    _day = 0;
    _month = 0;
    _year = 0;
}

Calendario::myDate::myDate(int d, int m, int y) {
    _day = d;
    _month = m;
    _year = y;
    try {
        this->checkDate();
    } catch (std::range_error &e) {
        cout << e.what() << endl;
        exit(4);
    }
}

bool operator==(const Calendario::myDate &d1, const Calendario::myDate &d2) {
    return d1._year == d2._year && d1._month == d2._month && d1._day == d2._day;
}

std::ostream &operator<<(std::ostream &stream, const Calendario::myDate &d) {
    stream << d._day << "-" << d._month << "-" << d._year;
    return stream;
}

bool operator!=(const Calendario::myDate &d1, const Calendario::myDate &d2) {
    return ((d1._day != d2._day) && (d1._month != d2._month) && (d1._year != d2._year));
}

void Calendario::myDate::setData(int d, int m, int y) {
//    cout << "Sto impostando " << d << ' ' << m << ' ' << y << endl;
    _day = d;
    _month = m;
    _year = y;
    try {
        this->checkDate();
    } catch (std::range_error &e) {
        cout << "Errore setdata: " << e.what() << endl;
        exit(4);
    }
}

int Calendario::myDate::getYear() const {
    return _year;
}


int subtract(Calendario::myDate d1, Calendario::myDate d2) {
//    cout << d1.date_to_days() << '-' << d2.date_to_days() << std::endl;
    return d1.date_to_days() - d2.date_to_days();
}

bool compare_date(Calendario::myDate d1, Calendario::myDate d2) {
//    cout << d1.date_to_days() << '-' << d2.date_to_days() << std::endl;
    return d1.date_to_days() > d2.date_to_days();
}

Calendario::myDate Calendario::offset(Calendario::myDate &d, int n) {
    return d.days_to_date(d.date_to_days() + n);
}


void Calendario::set_date_sessioni(const vector<string> &argomenti_sessioni, bool source_db) {
    vector<string> outstring;
    vector<string> periodi_temp;
    string anno_accademico_temp;
    if (source_db) {
        periodi_temp.assign(argomenti_sessioni.begin() + 1, argomenti_sessioni.end());
        anno_accademico_temp = argomenti_sessioni[0];

    } else {
        anno_accademico_temp = argomenti_sessioni[3];
        periodi_temp.assign(argomenti_sessioni.begin() + 4, argomenti_sessioni.end());

    }
//Lettura anno accademico di riferimento
    try {
        _regcal.search_and_read(_regcal.target_expression(lettura::anno_acc), anno_accademico_temp, outstring);

    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        exit(15);
    }

    _anno_accademico.setAnnoAccademico(outstring[1], outstring[2]);
    outstring.clear();


    cout << "Anno accademico: " << _anno_accademico.getPrimo() << '-' << _anno_accademico.getSecondo() << endl;

    //Lettura _date di _primo e _secondo sessioni
    for (auto &i: periodi_temp) {
        cout << i << endl;

        try {
            _regcal.search_and_read(_regcal.target_expression(lettura::sessioni), i, outstring);

        } catch (errore_formattazione &e) {
            cout << e.what() << endl;
            exit(3);
        }
    }

    //Conversione da stringa a intero per poter memorizzare i dati con il formato della classe myDate
    vector<int> date;
    transform(outstring.begin(), outstring.end(), back_inserter(date), strToInt);

    //TODO: setperiodo
    int i = 0;
    const int offset = 3;
    myDate data_inizio_temp, data_fine_temp;
    try {
        data_inizio_temp.setData(date[i + 1], date[i + 2], date[i + 3]);
        i += offset; //porto avanti di giorno, mese, anno + _match[0]
        data_fine_temp.setData(date[i + 1], date[i + 2], date[i + 3]);
        _inverno1.setPeriodo(data_inizio_temp, data_fine_temp);
        i += offset + 1;
        data_inizio_temp.setData(date[i + 1], date[i + 2], date[i + 3]);
        i += offset;
        data_fine_temp.setData(date[i + 1], date[i + 2], date[i + 3]);
        _estate2.setPeriodo(data_inizio_temp, data_fine_temp);
        i += offset + 1;
        data_inizio_temp.setData(date[i + 1], date[i + 2], date[i + 3]);
        i += offset;
        data_fine_temp.setData(date[i + 1], date[i + 2], date[i + 3]);
        _autunno3.setPeriodo(data_inizio_temp, data_fine_temp);
    } catch (runtime_error &e) {
        cout << e.what() << endl;
        exit(5);
    }
    this->display_date_sessioni();

    //try {
    check_sessioni(); // non è più necessario fare il try qui perchè lo fa già all'interno
    // } catch (std::runtime_error &e) {
    //   cout << e.what() << endl;
    //}

}

void Calendario::display_date_sessioni() const {
    cout << "Sessione invernale: ";
    cout << "dal " << _inverno1.getInizio() << " al " << _inverno1.getFine() << endl;
    cout << "Sessione estiva: ";
    cout << "dal " << _estate2.getInizio() << " al " << _estate2.getFine() << endl;
    cout << "Sessione autunnale: ";
    cout << "dal " << _autunno3.getInizio() << " al " << _autunno3.getFine() << endl;
}

void Calendario::check_sessioni() const {
    try {
        check_anno_accademico(_inverno1.getInizio().getYear());  //try su tutti i check
        check_anno_accademico(_inverno1.getFine().getYear());
        check_anno_accademico(_estate2.getInizio().getYear());
        check_anno_accademico(_estate2.getFine().getYear());
        check_anno_accademico(_autunno3.getInizio().getYear());
        check_anno_accademico(_autunno3.getFine().getYear());
    } catch (err_check_anno_accademico &e) {   //se uno dei check genera un'eccezione generiamo messagio di errore
        cout << e.what() << endl;
    }

    if (subtract(_inverno1.getFine(), _inverno1.getInizio()) != SEI_SETTIMANE) {
        throw std::runtime_error("Errore: la sessione invernale inserita non e' di sei settimane");
    }
    if (subtract(_estate2.getFine(), _estate2.getInizio()) != SEI_SETTIMANE) {
        throw std::runtime_error("Errore: la sessione estiva inserita non e' di sei settimane");
    }
    if (subtract(_autunno3.getFine(), _autunno3.getInizio()) != QUATTRO_SETTIMANE) {
        throw std::runtime_error("Errore: la sessione autunnale inserita non e' di quattro settimane");
    }


//    if (to_string(_inverno1._primo.getYear()) != _anno_accademico.second ||
//        to_string(_inverno1._secondo.getYear()) != _anno_accademico.second ||
//        to_string(_estate2._primo.getYear()) != _anno_accademico.second ||
//        to_string(_estate2._secondo.getYear()) != _anno_accademico.second ||
//        to_string(_autunno3._primo.getYear()) != _anno_accademico.second ||
//        to_string(_autunno3._secondo.getYear()) != _anno_accademico.second) {
//        throw std::runtime_error("Almeno una data non corrisponde all'anno accademico di riferimento");
//    }

}

void Calendario::fstampa_date_sessioni() {
    ofstream fout;

    //Un file diverso per ogni anno accademico
    fout.open(_anno_accademico.getPrimo() + '-' + _anno_accademico.getSecondo() + file_db_date_sessioni, ios::out);

    try {
        controlli_file(fout, file_db_date_sessioni);
    } catch (file_non_aperto &e) {
        cout << e.what() << endl;
    } catch (file_failed &e) {
        cout << e.what() << endl;
    }

    fout << _inverno1 << endl << _estate2 << endl << _autunno3;
}

bool Calendario::falls_within(Calendario::Periodo p, Calendario::myDate d) {
    if ((subtract(d, p.getInizio()) < 0) || (subtract(p.getFine(), d) > 0)) {
        cout << "La data " << d << " non e' compresa nel periodo " << p << endl;
        return false;
    } else
        return true;
}

void Calendario::set_indisponibilita(const vector<string> &argomenti_ind) {
    //anno accademico = argomenti_ind[3]
    //file indisponibilità = argomenti_ind[4]

    vector<string> outstring;

    //Lettura dell'anno accademico di riferimento
    try {
        _regcal.search_and_read(_regcal.target_expression(lettura::anno_acc), argomenti_ind[3], outstring);

    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        exit(3);
    }

    _anno_accademico.setAnnoAccademico(outstring[1], outstring[2]);
    outstring.clear();

    ifstream fin_db, fin_in;
    string db_indisponibilita =
            _anno_accademico.getPrimo() + '-' + _anno_accademico.getSecondo() + "db_indisponibilita.txt";
    fin_db.open(db_indisponibilita, ios::in);
//    cout << "DEBUG: " << argomenti_ind[4] << endl;
    fin_in.open(argomenti_ind[4], ios::in);

    try {
        controlli_file(fin_in, argomenti_ind[4]);
    } catch (file_non_aperto &e) {
        cout << e.what() << endl;
        exit(4);
    } catch (file_failed &e) {
        cout << e.what() << endl;
    }
    bool update = true;

    try {
        controlli_file(fin_db, db_indisponibilita);
    } catch (file_non_aperto &e) { //ora manda il messaggio "file non aperto o inesistente" poi lo crea
        cout << e.what() << endl;
        cout << "Creazione file indisponibilita per l'anno accademico " << _anno_accademico.getPrimo() << '-'
             << _anno_accademico.getSecondo() << endl;
        update = false;
    }

    //Se il file indisponibilita c'è già lo aggiorno, metto in ind_db i dati db e in ind_agg i dati input
    if (update) {
        LOG("Aggiornamento file indisponibilita in corso");
        read_indisponibilita(fin_db, _ind_db);
        update_indisponibilita(fin_in);
    } else {
        //Se il file indisponibilitadb è stato appena creato ci metto tutto ciò che leggo in input
        read_indisponibilita(fin_in, _ind_db);
    }


    fin_db.close();
    fin_in.close();
}

void Calendario::display_indisponibilita(vector<Indisponibilita> &v) const {
    cout << "Indisponibilita salvate in memoria:\n";
    for (const auto &i: v) {
        cout << i.getMatricolaProf();
        for (auto k: i.getDate()) {
            cout << ' ' << k;
        }
        cout << '\n';
    }
}

void Calendario::fstampa_indisponibilita() {
    //C'è un file diverso per ogni anno accademico
    ofstream fout;
    string db_indisponibilita =
            _anno_accademico.getPrimo() + '-' + _anno_accademico.getSecondo() + "db_indisponibilita.txt";
    //Stampa sempre da capo, quindi non c'è bisogno di append
    fout.open(db_indisponibilita, ios::out);

    try {
        controlli_file(fout, db_indisponibilita);
    } catch (file_non_aperto &e) {
        cout << e.what() << endl;
        exit(3); //ora come nel controllo dei file di input controlla sia l'apertura che flag di errore
    } catch (file_failed &e) {
        cout << e.what() << endl;
        exit(3);
    }

    cout << "Stampando indisponibilita su file...\n";
    for (const auto &i: _ind_db) {
        fout << i.getMatricolaProf();
        for (auto k: i.getDate()) {
            fout << ';' << k;
        }
        fout << '\n';
    }
}

void Calendario::update_indisponibilita(ifstream &fin_in) {
//    read_indisponibilita(fin, _ind_db);

    read_indisponibilita(fin_in, _ind_agg);

    bool trovato = false;

    //TODO: da finire manca aggiunta matricole nuove
    for (const auto &i: _ind_agg) {
        trovato = false;
        for (auto &k: _ind_db) {
            //Se trovo una _matricola uguale nel database
            if (i.getMatricolaProf() == k.getMatricolaProf()) {
                //Aggiorno le _date
                k.setDate(i.getDate());
                trovato = true;
            }
        }
        if (!trovato) {
            _ind_db.push_back(i);
        }
    }
    cout << "Stampo indisponibilita:\n";
    display_indisponibilita(_ind_db);

}

//qua non sono riuscito a ricondurlo ad un template
//perche dovrei passargli il vettore di matricole, ma il costruttore template prende un solo parametro
//e check_annoaccademico dovrebbe essere static e non so come passarglielo
//file pointer to file_indisponibilita.txt
//il vettore indisponibilità e db o agg a seconda da chi viene chiamato
void Calendario::read_indisponibilita(ifstream &fin, vector<Indisponibilita> &v_ind) {
    string s_temp;
    int n = 1;
    vector<string> out_riga_indisponibilita;
    vector<string> int_date;

    //Leggo i prof presenti nel database per confrontare le matricole
    _dbcal.leggi_db<Database::Professore>(_dbcal.getFileDbProfessori(), _dbcal.getProfessoriDb());

    vector<Database::Professore *> professori_temp;
    professori_temp = _dbcal.getProfessoriDb();
    //Non copio le matricole in un altro vettore perchè dovrei comunque chiamare getMatricola n volte

    while (!fin.eof()) {
        getline(fin, s_temp);

        if (!s_temp.empty()) {
            //leggo il file indisponibilita
            try {
                //prima leggo la matricola del prof
                _regcal.search_and_read(_regcal.target_expression(lettura::id_prof), s_temp, out_riga_indisponibilita);

                //poi leggo i periodi
//                _regcal.multiple_fields(_regcal.target_expression(lettura::periodo), s_temp, out_riga_indisponibilita);

                //poi leggo le date
                _regcal.multiple_fields(_regcal.target_expression(lettura::data), s_temp, out_riga_indisponibilita);
                for (int i = 2; i < out_riga_indisponibilita.size(); i++) {
                    try {
                        _regcal.search_and_read(_regcal.target_expression(lettura::data), out_riga_indisponibilita[i],
                                                int_date);
                    } catch (errore_formattazione &e) {
                        cout << e.what() << endl;
                        READ_ERR("date indisponibilita");
                        exit(3);
                    }

                }

            } catch (errore_formattazione &e) {
                cout << e.what() << endl;
                READ_ERR("delle indisponibilita");
                exit(3);
            }
//            LOG("Riga indispo: ")
//            LOGV(out_riga_indisponibilita)
//            LOG("Date int")
//            for(int i = 0; i<int_date.size(); i++){
//                cout << i << ": " << int_date[i] << endl;
//            }
            //trasformo i numeri letti in interi //TODO: da risolvere conversione cifre della data in intero, leggi cifre singole
            vector<int> gmy; //Data come interi
            try {
                transform(int_date.begin() + 1, int_date.end(), back_inserter(gmy), strToInt);
            } catch (std::runtime_error &e) {
                cout << "Errore string to int: " << e.what() << endl;
            }

            //compongo un oggetto indisponibilita per poi salvarlo
            auto *ind_temp = new Indisponibilita;
            ind_temp->setMatricolaProf(out_riga_indisponibilita[1]); //Prendo la stringa dal vettore non convertito
            bool matricola_presente = false;
            try {
                for (auto &i: professori_temp) {
//                cout << i->getMatricola() << endl;
                    if ('d' + ind_temp->getMatricolaProf() == i->getMatricola()) {
                        matricola_presente = true;
                    }

                }
                if (!matricola_presente) {

                    throw prof_non_presente();
                }
            } catch (prof_non_presente &e) {
                cout << ind_temp->getMatricolaProf() << " " << e.what() << endl;
                exit(7);
            }

            int offset = 4;
            for (int j = 0; j < gmy.size();) { //ciclo i periodi di una riga indisponibilita (comprende la _matricola)
                auto *p_temp = new Periodo;
                myDate data_inizio_temp, data_fine_temp;
                data_inizio_temp.setData(gmy[j], gmy[j + 1], gmy[j + 2]);

                j += offset;
                data_fine_temp.setData(gmy[j], gmy[j + 1], gmy[j + 2]);

                j += offset;

                try {
                    p_temp->setPeriodo(data_inizio_temp, data_fine_temp);
                    check_anno_accademico(data_fine_temp.getYear());
                    check_anno_accademico(data_inizio_temp.getYear());
                } catch (err_periodo &e) {
                    cout << e.what() << endl;
                    exit(5);
                } catch (err_check_anno_accademico &e) {
                    cout << e.what() << endl;
                    exit(5);
                }

//                ind_temp.getDate().push_back(p_temp);
                ind_temp->setPeriodo(*p_temp);
                delete p_temp;
//            cout << "Debug: " << p_temp << endl;
//                p_temp.debug();
            }

            v_ind.push_back(*ind_temp);
//            ind_temp->debug();
            delete ind_temp;
        } else {
            cout << "[Warning] Riga " << n << " vuota nel file inserito" << endl;
        }

//        int k = 0;
//        for(const auto& i: out_riga_indisponibilita){
//            cout  << k++ << ": " << i << endl;
//        }


        out_riga_indisponibilita.clear();
        int_date.clear();
        n++;
    }
    this->display_indisponibilita(v_ind);
}

void Calendario::check_anno_accademico(int year) const {
    if (to_string(year) != _anno_accademico.getSecondo()) {
        cout << "Errore anno letto: " << year << " Anno inserito come argomento: " << _anno_accademico.getSecondo()
             << endl;
        throw err_check_anno_accademico();
    }
}

void Calendario::ordina_giorni() {

//    sort(_esami_giorno.begin(), _esami_giorno.end(), compare_date);
    for (auto &i: _esami_giorno) {
        for (auto &k: _esami_giorno) {
            if (i.getData() != k.getData()) {
                if (compare_date(i.getData(), k.getData())) {
                    GiornoSessione *temp;
                    temp = &i;
                    //TODO: da finire
                }
            }

        }
    }
}


void Calendario::Periodo::setPeriodo(const myDate &inizio, const myDate &fine) {
    if (subtract(fine, inizio) < 0) {
        cout << "Errore periodo: " << inizio << '|' << fine << " non valido\n";
        throw err_periodo();
    }
    _inizio = inizio;
    _fine = fine;

}

Calendario::myDate Calendario::Periodo::getInizio() const {
    return _inizio;
}

Calendario::myDate Calendario::Periodo::getFine() const {
    return _fine;
}

void Calendario::Periodo::debug() const {
    cout << _inizio << ' ' << _fine << endl;
}

void Calendario::Anno_Accademico::setAnnoAccademico(const string &inizio, const string &fine) {
    if (stoi(fine) - stoi(inizio) != 1) {
        cout << "Anno accademico " << inizio << '-' << fine << " non valido" << endl;
        throw runtime_error("Errore anno accademico non valido");
    } else {
        _primo = inizio;
        _secondo = fine;
    }
}

string Calendario::Anno_Accademico::getPrimo() const {
    return _primo;
}

string Calendario::Anno_Accademico::getSecondo() const {
    return _secondo;
}

void Calendario::GiornoSessione::fstampa_giornosessione(ofstream &fout) {

    fout << _data << endl << endl;
    for (int k = 0; k < 6; k++) {
        fout << _fasce[k] << ';';
        //Attenzione: così stampi tutti gli esami per tutte le fasce orarie, ci vuole una condizione per
        //stampare un Esame solo se è presente nella fascia oraria**
        for (auto &i: _esami_del_giorno) {
            //Così dovrei aver risolto
            if (i._fascia_oraria[k]) {
                i.fstampa_esame(fout);
            }
        }
        fout << endl;
    }
}

Calendario::myDate Calendario::GiornoSessione::getData() {
    return _data;
}

void Calendario::GiornoSessione::Esame::fstampa_esame(ofstream &fout) const {

    if (_numero_versioni > 1) {
        for (int i = 1; i <= _numero_versioni; i++) {
            this->_id_esame.fstampa_id(fout);
            fout << '[' << i << ']'; //numero versione
            fout << '(' << _id_cds << ')'; //id corso di studi
            fout << ';';
        }
    } else {
        _id_esame.fstampa_id(fout);
        fout << '(' << _id_cds << ')';
    }
}

//argomenti -g 2020-2021 sessione_esami_2021.txt
void Calendario::genera_date_esami(const vector<string> &argomenti_es) {
    //Gli argomenti servono a settare anno accademico e file di output
    vector<string> out_anno_acc;
    try {
        _regcal.search_and_read(_regcal.target_expression(lettura::anno_acc), argomenti_es[2], out_anno_acc);
    } catch (errore_formattazione &e) {
        cout << e.what() << endl;
        READ_ERR("anno accademico argomento");
        exit(15);
    }
    string anno_acc = out_anno_acc[1] + "-" + out_anno_acc[2]; //salvo anno accademico di riferimento per cercarlo dopo

    //Salvo in memoria le date delle sessioni dal file aaaa-aaaadb_date_sessioni.txt
    set_date_sessioni(leggi_db_date_sessioni(argomenti_es), true);
    display_date_sessioni();

    //Accedo al database

    //Leggo file db_corsi.txt
   try{
       _dbcal.leggi_corso_db();
   } catch (err_anno_senza_corso &e){
       cout << e.what() << endl;
       exit(26);
   } catch (errore_formattazione_id_corsi &e){
       cout << e.what() << endl;
   }
//    _dbcal.target_fstampa(options::corsi, true); //debug

    //Leggo file db_corsi_studio.txt
    _dbcal.leggi_db(_dbcal.getFileDbCds(), _dbcal.getCdsDb());
//    _dbcal.target_fstampa(options::cds, true); //debug

    //Leggo file db_aule.txt
    _dbcal.leggi_db(_dbcal.getFileDbAule(), _dbcal.getAuleDb());
    vector<string> id_aule;
    vector<int> capienza_esame;
    for (auto aula: _dbcal.getAuleDb()) {
        id_aule.push_back(aula->getId());
        capienza_esame.push_back(aula->getCapEsame());
    }

    _gen.setIdAule(id_aule);
//    _gen.setCapienzaEsame(capienza_esame);

    //Leggo file indisponibilità.txt

    bool trovato = false;
    bool trovato_ragrupp = false;
    vector<string> corsi_acquisiti;
    //Per ogni esame
    //Per ogni corso
    for (auto corso: _dbcal.getCorsiDb()) {
        vector<Dati_esame> datiEsame;

        //Per ogni anno accademico relativo al corso
        for (auto anno_accademico: corso->getAnniAccademici()) {
            trovato = false;
            //controllo di essere nell'anno accademico giusto
            if (anno_accademico->getAnnoAccademico() == anno_acc) {

                //find ritorna end quando non trova elementi
                if (find(corsi_acquisiti.begin(), corsi_acquisiti.end(), corso->getIdCorso()) ==
                    corsi_acquisiti.end()) {
                    setDatiEsame(anno_accademico, corso->getIdCorso(), datiEsame);
                    trovato = true;
                } else {
//                    cout << "Corso " << corso->getIdCorso() << " già acquisito\n";
                }

                //Per ogni id_esame RAGGRUPPATO

                for (auto id_esame_raggruppato: anno_accademico->getIdCorsiRaggruppati()) {
                    for (auto corso_raggruppato: _dbcal.getCorsiDb()) {
                        //Per ogni anno accademico relativo al corso
                        for (auto anno_accademico_raggruppato: corso->getAnniAccademici()) {
                            trovato_ragrupp = false;
                            //controllo di essere nell'anno accademico giusto
                            if (anno_accademico_raggruppato->getAnnoAccademico() == anno_acc) {

                                //Controllo di non salvare due volte gli stessi esami

                                //Se non trovo il corso nel vettore di corsi acquisiti
                                if (find(corsi_acquisiti.begin(), corsi_acquisiti.end(),
                                         id_esame_raggruppato->getIdCorso()) == corsi_acquisiti.end()) {
                                    trovato_ragrupp = true;
                                    setDatiEsame(anno_accademico_raggruppato, id_esame_raggruppato->getIdCorso(),
                                                 datiEsame);
//                                cout << "Debug dati esame: " << datiEsame.back() << endl;
                                    corsi_acquisiti.push_back(id_esame_raggruppato->getIdCorso());
                                } else {
//                                    cout << "Corso raggruppato " << id_esame_raggruppato->getIdCorso() << " già acquisito\n";
                                }

                            }
                        }
                        if (!trovato_ragrupp) {
//                            cout << "L'esame raggruppato " << id_esame_raggruppato->getIdCorso()
//                                 << " non ha l'anno accademico " << anno_acc << " salvato nel database\n";
                            //TODO: throw exception?
//                            exit(-2);
                        }
                    }
                }
            }
        }
        if (!trovato) {
//            cout << "L'esame " << corso->getIdCorso() << " non ha l'anno accademico " << anno_acc
//                 << " salvato nel database\n";
            //TODO: throw exception?
//            exit(-2);
        }

//TODO: funzione che calcola numero di slot necessari per l'esame (ogni versione ha lo stesso numero di slot) (float)floor((120+15+25) / 120)

        cout << "Dimensione datiesame: " << datiEsame.size() << endl;
        if(!datiEsame.empty()) {
            for (int i = 0; i < datiEsame.size(); i++) {
                for (int j = 0; j < datiEsame[i].n_versioni; j++) {
                    datiEsame[i].n_iscritti.push_back(rand() % 25 + 50);
                }
            }

            vector<string> esami;
            vector<vector<string>> cds;
            vector<vector<string>> prof;
            vector<string> anni;
            vector<int> slot;
            vector<int> parall;
            vector<vector<int>> iscritti;
            int sem;
//        esami.push_back(corso->getIdCorso());
            iscritti.resize(datiEsame.size());
            prof.resize(datiEsame.size());
            cds.resize(datiEsame.size());
            for (int i = 0; i < datiEsame.size(); i++) {
                cout << "Trasferendo datiesame " << i << endl;
                anni.push_back(datiEsame[i].anno_appartenenza);
                slot.push_back(datiEsame[i].n_slot_necessari);
                parall.push_back(datiEsame[i].n_versioni);
                sem = datiEsame[i].semestre;
                iscritti[i].insert(iscritti[i].end(), datiEsame[i].n_iscritti.begin(), datiEsame[i].n_iscritti.end());
//            for (int j = 0; j < datiEsame[i].n_iscritti.size(); j++) {
//
//            }
                esami.push_back(datiEsame[i].id_corso);
//            for (int j = 0; j < datiEsame[i].id_corsi_raggruppati.size(); j++) {
//
//            }
                cds[i].insert(cds[i].end(), datiEsame[i].id_cds.begin(), datiEsame[i].id_cds.end());
//            for (int j = 0; j < datiEsame[i].id_cds.size(); j++) {
//                cout << "size cds: " << cds.size() << " indice " << j << " size id_cds " << datiEsame[i].id_cds.size() << endl;
//                cout << datiEsame[i].id_cds[1] << endl;
//
//
//            }
                prof[i].insert(prof[i].end(), datiEsame[i].id_professori.begin(), datiEsame[i].id_professori.end());
//            for (int j = 0; j < datiEsame[i].id_professori.size(); j++) {
//
//            }
            }

            _gen.set_id_esame_nel_calendario((int) datiEsame.size(), esami, cds, anni,
                                             slot, prof, parall, sem, iscritti);


            //TODO: la funzione genera esami penso che dovrebbe stare all'interno di questo ciclo
        } else{
            cout << "Errore: il vettore per il trasferimento dei dati esame e' vuoto\n";

        }
        datiEsame.clear();
    }


    _gen.print_calendar();



/* Dati da salvare dal database:
 * Corso: durata esame, bool semestre di appartenenza (elenco semestri con i corso_id)
 * Id corso di studi
 * Versioni esami, raggruppamenti*/
}

//Vale sia per esame raggruppato che per esame corrente
void Calendario::setDatiEsame(Database::Corso::Anno_Accademico *dati_anno, const string &id_esame,
                              vector<Calendario::Dati_esame> &dati_esami) {
    Dati_esame esame_temp;
    esame_temp.n_versioni = dati_anno->getNVersioniInParallelo();


    //Per ogni versione del corso in un anno accademico
    for (auto versione_raggruppato: dati_anno->getVersioni()) {
        //Salvo la matricola del titolare nel vettore di professori
        esame_temp.id_professori.push_back(versione_raggruppato->getMatricolaTitolare());
        //Per ogni professore associato
        for (auto profn: versione_raggruppato->getAltriProfN()) {
            //Salvo la matricola del professore associato
            esame_temp.id_professori.push_back(profn->getMatricola());
        }
    }

    //funzione che associa ogni id esame agli slot necessari
    bool sufficiente;
    int n_slot_necessari;
    sufficiente = false;
    n_slot_necessari = 0;
    cout << "Debug getter: " << id_esame << ':' << dati_anno->getEsame()->getDurataEsame() << ' '
         << dati_anno->getEsame()->getTIngresso() << ' ' << dati_anno->getEsame()->getTUscita() << endl;
    int minuti_necessari = dati_anno->getEsame()->getDurataEsame() + dati_anno->getEsame()->getTIngresso() +
                           dati_anno->getEsame()->getTUscita();
    cout << "Minuti necessari esame " << id_esame << ": " << minuti_necessari << endl;
    while (!sufficiente && n_slot_necessari < 7) {
        n_slot_necessari++;
        if (minuti_necessari < (120 * n_slot_necessari)) {
            sufficiente = true;
        }
    }
    if (!sufficiente) {
        cout << endl << "Esame troppo lungo!" << endl;
        n_slot_necessari = 6;
        //return qualcosa
    }
    esame_temp.n_slot_necessari = n_slot_necessari;
    cout << "Debug slot esame " << id_esame << " : " << esame_temp.n_slot_necessari << endl;

    //Per ogni corso di studio
    for (auto corsodistudio: _dbcal.getCdsDb()) {
        //Per ogni semestre nel corso di studio
        int contasemestri = 0;
        for (const auto &id_per_semestre: corsodistudio->getCorsiSemestre()) {
            contasemestri++;
            //Per ogni id nel semestre
            for (auto id_di_un_semestre: id_per_semestre) {
                if (id_di_un_semestre->getIdCorso() == id_esame) {
                    esame_temp.semestre = contasemestri %
                                          2;  //semestre di appartenenza Se dispari ho il primo semestre, per i pari il secondo
                    //aggiungo uno così semestre=1 primo semestre e semestre=2 secondo semestre
                    esame_temp.semestre++;
                    //indice da convertire per ricavare l'anno di appartenenza
                    esame_temp.anno_appartenenza = to_string(contasemestri);
                    //Salva id cds, da ricontrollare se funziona
                    esame_temp.id_cds.push_back(corsodistudio->getIdCds());
                }
            }
        }
    }
    dati_esami.push_back(esame_temp);
}

vector<string> Calendario::leggi_db_date_sessioni(const vector<string> &argomenti_es) {
    //TODO puoi metterla nel template?
    ifstream fin;
    const string f_db_sessioni = argomenti_es[2] + file_db_date_sessioni;
    fin.open(f_db_sessioni, ios::in);
    try {
        controlli_file(fin, f_db_sessioni);
    } catch (file_non_aperto &e) {
        cout << e.what() << endl;
        //cout << "Errore: file " << f_db_sessioni << " db date sessioni Esame non trovato\n";
        exit(9);
    } catch (file_failed &e) {
        cout << e.what() << endl;
        exit(9);
    }

    string temp;
    vector<string> outstring;
    //salvo anche l'anno accademico
    outstring.push_back(argomenti_es[2]);
    int n = 1;

    while (!fin.eof()) {
        getline(fin, temp);

        if (!temp.empty()) {
            //Legge un periodo
//            if (!_regcal.search_and_read(_regcal.target_expression(lettura::sessioni), temp, outstring)) {
//                cout << "Errore lettura periodo sessione da file " << f_db_sessioni << endl;
//                exit(8);}
            outstring.push_back(temp);

        } else {
            cout << "[Warning] Riga " << n << " vuota nel file " << f_db_sessioni << endl;
        }
        n++;
    }

//    for (const auto &i: outstring) {
//        cout << "outstring leggi_db db date sessioni: " << i << endl;
//    }

    return outstring;
}

//Calendario::Calendario(const string &file_argomento) {
//    _file_argomento = file_argomento;
//}

Database Calendario::getDbcal() const {
    return _dbcal;
}

/*vector<Calendario::Indisponibilita> Calendario::get_indisponibilita() {
    return _ind_agg;
}*/

vector<Calendario::Periodo> Calendario::Indisponibilita::getDate() const {
    return _date;
}

string Calendario::Indisponibilita::getMatricolaProf() const {
    return _matricola;
}

void Calendario::Indisponibilita::setMatricolaProf(const string &matricola) {
    _matricola = matricola;
}

void Calendario::Indisponibilita::debug() const {
    cout << _matricola << endl;
    for (auto data: _date) {
        cout << data << '#';
    }
    cout << endl;
}

void Calendario::Indisponibilita::setPeriodo(const Periodo &periodo) {
    _date.push_back(periodo);
}

void Calendario::Indisponibilita::setDate(const vector<Periodo> &date) {
    _date = date;
}

//ci ho provato
//Calendario::Indisponibilita::Indisponibilita(const string &row) {
//    vector<string> outstring;
//    //leggo il file indisponibilita
//    if (!_regind.search_and_read(_regind.target_expression(lettura::id_prof), row, outstring)) {
//        cerr << "Errore formattazione file indisponibilita\n";
//        exit(3);
//    }
//    //trasformo i numeri letti in interi
//    vector<int> gmy; //Data come interi
//    try {
//        transform(outstring.begin() + 1, outstring.end(), back_inserter(gmy), strToInt);
//    } catch (std::runtime_error &e) {
//        cout << "Errore string to int: " << e.what() << endl;
//    }
//
//    //compongo un oggetto indisponibilita per poi salvarlo
////    Indisponibilita ind_temp;
//    int j = 1, offset = 3;
//    _matricola = outstring[j]; //Prendo la stringa dal vettore non convertito
//    bool matricola_presente = false;
//    for (auto &i: ) {
//        //                cout << i->getMatricola() << endl;
//        if ('d' + _matricola == i->getMatricola()) {
//            matricola_presente = true;
//        }
//    }
//    if (!matricola_presente) {
//
//        cout << "Errore matricola " << _matricola << " professore non presente nel database\n";
//        exit(7
//        );
//    }
//
//    for (; j < gmy.size();) { //ciclo i periodi di una riga indisponibilita (comprende la _matricola)
//        Periodo p_temp;
//        myDate data_inizio_temp, data_fine_temp;
//        data_inizio_temp.setData(gmy[j], gmy[j + 1], gmy[j + 2]);
//        //                p_temp._primo.setData(gmy[j], gmy[j + 1], gmy[j + 2]);
//        j += offset;
//        data_fine_temp.setData(gmy[j], gmy[j + 1], gmy[j + 2]);
//        //                p_temp._secondo.setData(gmy[j], gmy[j + 1], gmy[j + 2]);
//        j += offset;
//
//
//        try {
//            p_temp.setPeriodo(data_inizio_temp, data_fine_temp);
//            check_anno_accademico(data_fine_temp.getYear());
//            check_anno_accademico(data_inizio_temp.getYear());
//        } catch (runtime_error &e) {
//            cout << e.what() << endl;
//            exit(5);
//        }
//
//        _date.push_back(p_temp);
//        //            cout << "Debug: " << p_temp << endl;
//    }
//}


ostream &operator<<(ostream &os, const Calendario::Dati_esame &esame) {
    os << "anno_appartenenza: " << esame.anno_appartenenza << " id_cds: " << esame.id_cds.size() << " id_professori: "
       << esame.id_professori.size() << " n_versioni: " << esame.n_versioni << " n_iscritti: "
       << esame.n_iscritti.size()
       << " semestre: " << esame.semestre << " id_corsi_raggruppati: " << esame.id_corsi_raggruppati.size()
       << " n_slot_necessari: " << esame.n_slot_necessari;
    return os;
}

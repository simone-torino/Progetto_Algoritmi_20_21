
#include "Calendario.h"

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

int Calendario::myDate::days_in_year(int year) const {
    if (year % 400 == 0) {
        return 366;
    }
        // not a leap year if divisible by 100
        // but not divisible by 400
    else if (year % 100 == 0) {
        return 365;
    }
        // leap year if not divisible by 100
        // but divisible by 4
    else if (year % 4 == 0) {
        return 366;
    }
        // all other years are not leap years
    else {
        return 365;
    }
}

int Calendario::myDate::days_in_month(int month) const {
    if (month == 2) {
        return 28;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else {
        return 31;
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
    if(source_db){
        periodi_temp.assign(argomenti_sessioni.begin()+1, argomenti_sessioni.end());
        anno_accademico_temp = argomenti_sessioni[0];

    }else{
        anno_accademico_temp = argomenti_sessioni[3];
        periodi_temp.assign(argomenti_sessioni.begin()+4, argomenti_sessioni.end());

    }
//Lettura anno accademico di riferimento
    if (!_regcal.search_and_read(_regcal.target_expression(lettura::anno_acc), anno_accademico_temp, outstring)) {
        cerr << "Errore formattazione anno accademico\n";
        exit(3); //TODO eccezioni
    }
    _anno_accademico.first = outstring[1];
    _anno_accademico.second = outstring[2];
    outstring.clear();


    cout << "Anno accademico: " << _anno_accademico.first << '-' << _anno_accademico.second << endl;

    //Lettura _date di _primo e _secondo sessioni
    for (auto & i : periodi_temp) {
        cout << i << endl;
        if (!_regcal.search_and_read(_regcal.target_expression(lettura::sessioni), i, outstring)) {
            cerr << "Errore formattazione date esami\n";
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

    try {
        check_sessioni();
    } catch (std::runtime_error &e) {
        cout << e.what() << endl; //TODO: da definire
    }

}

void Calendario::display_date_sessioni() const {
    cout << "Sessione invernale: ";
    cout << "dal " << _inverno1._inizio << " al " << _inverno1._fine << endl;
    cout << "Sessione estiva: ";
    cout << "dal " << _estate2._inizio << " al " << _estate2._fine << endl;
    cout << "Sessione autunnale: ";
    cout << "dal " << _autunno3._inizio << " al " << _autunno3._fine << endl;
}

void Calendario::check_sessioni() const {
    check_anno_accademico(_inverno1._inizio.getYear());
    check_anno_accademico(_inverno1._fine.getYear());
    check_anno_accademico(_estate2._inizio.getYear());
    check_anno_accademico(_estate2._fine.getYear());
    check_anno_accademico(_autunno3._inizio.getYear());
    check_anno_accademico(_autunno3._fine.getYear());

    if (subtract(_inverno1._fine, _inverno1._inizio) != SEI_SETTIMANE) {
        throw std::runtime_error("Errore: la sessione invernale inserita non e' di sei settimane");
    }
    if (subtract(_estate2._fine, _estate2._inizio) != SEI_SETTIMANE) {
        throw std::runtime_error("Errore: la sessione estiva inserita non e' di sei settimane");
    }
    if (subtract(_autunno3._fine, _autunno3._inizio) != QUATTRO_SETTIMANE) {
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
    cout << "ttappost" << endl;
}

void Calendario::fstampa_date_sessioni() {
    ofstream fout;

    //Un file diverso per ogni anno accademico
    fout.open(_anno_accademico.first + '-' + _anno_accademico.second + file_db_date_sessioni, ios::out);
    controlli_file(fout, file_db_date_sessioni);

    fout << _inverno1 << endl << _estate2 << endl << _autunno3;
}

bool Calendario::falls_within(Calendario::Periodo p, Calendario::myDate d) {
    if ((subtract(d, p._inizio) < 0) || (subtract(p._fine, d) > 0)) {
        cout << "La data " << d << " non e' compresa nel periodo " << p << endl;
        return false;
    } else
        return true;
}

void Calendario::set_indisponibilita(const vector<string> &argomenti_ind) {
    //anno accademico = argomenti_ind[3]
    //file indisponibilità = argomenti_ind[4]

    vector<string> outstring;

    //Lettura l'anno accademico di riferimento
    if (!_regcal.search_and_read(_regcal.target_expression(lettura::anno_acc), argomenti_ind[3], outstring)) {
        cerr << "Errore formattazione anno accademico\n";
        exit(3);
    }
    _anno_accademico.first = outstring[1];
    _anno_accademico.second = outstring[2];
    outstring.clear();

    ifstream fin_db, fin_in;
    string db_indisponibilita = _anno_accademico.first + '-' + _anno_accademico.second + "db_indisponibilita.txt";
    fin_db.open(db_indisponibilita, ios::in);
//    cout << "DEBUG: " << argomenti_ind[4] << endl;
    fin_in.open(argomenti_ind[4], ios::in);

    try { //TODO: eccezioni
        controlli_file(fin_in, argomenti_ind[4]);
    } catch (std::runtime_error &e) {
        cout << e.what() << endl;
        exit(4);
    }
    bool update = true;

    try { //TODO: eccezioni
        controlli_file(fin_db, db_indisponibilita);
    } catch (std::runtime_error &e) {
        cout << e.what() << endl;
        cout << "Creazione file _dbcal indisponibilita per l'anno accademico " << _anno_accademico.first << '-'
             << _anno_accademico.second << endl;
        update = false;
    }

    //Se il file _dbcal c'è già lo aggiorno, metto in ind_db i dati _dbcal e in ind_agg i dati input
    if (update) {
        LOG("Sto aggiornando");
        read_indisponibilita(fin_db, _ind_db);
        update_indisponibilita(fin_in);
    } else {
        //Se il file _dbcal è stato appena creato ci metto tutto ciò che leggo in input
        read_indisponibilita(fin_in, _ind_db);
    }


    fin_db.close();
    fin_in.close();
}

void Calendario::display_indisponibilita(vector<Indisponibilita> &v) const {
    cout << "Indisponibilita salvate in memoria:\n";
    for (const auto &i: v) {
        cout << i._matricola;
        for (auto k: i._date) {
            cout << ' ' << k;
        }
        cout << '\n';
    }
}

void Calendario::fstampa_indisponibilita() {
    //C'è un file diverso per ogni anno accademico
    ofstream fout;
    string db_indisponibilita = _anno_accademico.first + '-' + _anno_accademico.second + "db_indisponibilita.txt";
    //Stampa sempre da capo, quindi non c'è bisogno di append
    fout.open(db_indisponibilita, ios::out);

    try {
        controlli_file(fout, db_indisponibilita);
    } catch (std::runtime_error &e) {
        cout << e.what() << endl;
        exit(3); //TODO: eccezioni
    }

    cout << "Stampando indisponibilita su file...\n";
    for (const auto &i: _ind_db) {
        fout << 'd' << i._matricola;
        for (auto k: i._date) {
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
        for (auto &k: _ind_db) {
            //Se trovo una _matricola uguale nel database
            if (i._matricola == k._matricola) {
                //Aggiorno le _date
                k._date = i._date;
                trovato = true;
            }
        }
        if (!trovato) {
            _ind_db.push_back(i);
        }
    }
    cout << "Stampo _dbcal:\n";
    display_indisponibilita(_ind_db);

}

void Calendario::read_indisponibilita(ifstream &fin, vector<Indisponibilita> &v_ind) {
    string s_temp;
    int n = 1;
    vector<string> outstring;

    //Leggo i prof presenti nel database per confrontare le matricole
    _dbcal.leggi_prof_db();
    vector<Database::Professore *> professori_temp;
    professori_temp = _dbcal.getVProfessori();

    while (!fin.eof()) {
        getline(fin, s_temp);

        if (!s_temp.empty()) {
            if (!_regcal.search_and_read(_regcal.target_expression(lettura::indisp), s_temp, outstring)) {
                cerr << "Errore formattazione file indisponibilita\n";
                exit(3);
            }
            vector<int> gmy; //Data come interi
            try {
                transform(outstring.begin() + 1, outstring.end(), back_inserter(gmy), strToInt);
            } catch (std::runtime_error &e) {
                cout << "Errore string to int: " << e.what() << endl;
            }

            Indisponibilita ind_temp;
            int j = 1, offset = 3;
            ind_temp._matricola = outstring[j]; //Prendo la stringa dal vettore non convertito
            bool matricola_presente = false;
            for (auto &i: professori_temp) {
//                cout << i->getMatricola() << endl;
                if ('d' + ind_temp._matricola == i->getMatricola()) {
                    matricola_presente = true;
                }
            }
            if (!matricola_presente) {
                //TODO: eccezioni
                cout << "Errore matricola " << ind_temp._matricola << " professore non presente nel database\n";
                exit(7);
            }

            for (; j < gmy.size();) { //ciclo i periodi di una riga indisponibilita (comprende la _matricola)
                Periodo p_temp;
                myDate data_inizio_temp, data_fine_temp;
                data_inizio_temp.setData(gmy[j], gmy[j + 1], gmy[j + 2]);
//                p_temp._primo.setData(gmy[j], gmy[j + 1], gmy[j + 2]);
                j += offset;
                data_fine_temp.setData(gmy[j], gmy[j + 1], gmy[j + 2]);
//                p_temp._secondo.setData(gmy[j], gmy[j + 1], gmy[j + 2]);
                j += offset;


                try { //TODO: eccezioni
                    p_temp.setPeriodo(data_inizio_temp, data_fine_temp);
                    check_anno_accademico(data_fine_temp.getYear());
                    check_anno_accademico(data_inizio_temp.getYear());
                } catch (runtime_error &e) {
                    cout << e.what() << endl;
                    exit(5);
                }

                ind_temp._date.push_back(p_temp);
//            cout << "Debug: " << p_temp << endl;
            }

            v_ind.push_back(ind_temp);
        } else {
            cout << "[Warning] Riga " << n << " vuota nel file inserito" << endl;
        }

//        int k = 0;
//        for(const auto& i: outstring){
//            cout  << k++ << ": " << i << endl;
//        }


        outstring.clear();
        n++;
    }
    this->display_indisponibilita(v_ind);
}

void Calendario::check_anno_accademico(int year) const {
    if (to_string(year) != _anno_accademico.second) {
        cout << "Errore anno: " << year << endl;
        throw std::runtime_error("Almeno una data non corrisponde all'anno accademico di riferimento");
    }
}

//bool compare_dates(Calendario::GiornoSessione &i, Calendario::GiornoSessione &j){
//    if(subtract(i.getData(),j.getData()) > 0)
//        return true;
//    else
//        return false;
//}

void Calendario::ordina_giorni() {

//    sort(_esami.begin(), _esami.end(), compare_date);
    for (auto &i:_esami) {
        for (auto &k:_esami) {
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
    if (subtract(fine, inizio) >= 0) {
        if (subtract(fine, inizio) == 0) {
            cout << "[Warning] Il periodo: " << inizio << '|' << fine << " indica un giorno solo\n";
        }
        _inizio = inizio;
        _fine = fine;
    } else {
        cout << "Periodo: " << inizio << '|' << fine << " non valido\n";
        throw runtime_error("Errore periodo non valido");
    }

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

void Calendario::GiornoSessione::fstampa_giornosessione(ofstream &fout) {

    fout << _data << endl << endl;
    for (int k = 0; k < 6; k++) {
        fout << _fasce[k] << ';';
        //Attenzione: così stampi tutti gli esami per tutte le fasce orarie, ci vuole una condizione per
        //stampare un Esame solo se è presente nella fascia oraria**
        for (auto &i : _esami_del_giorno) {
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
            fout << '[' << i << ']';
            fout << '(' << _id_cds << ')';
            fout << ';';
        }
    } else {
        _id_esame.fstampa_id(fout);
        fout << '(' << _id_cds << ')';
    }

}

void Calendario::genera_date_esami(const vector<string> &argomenti_es) {

    //Ho bisogno di accedere al database
    //Salvo in memoria le date delle sessioni dal file aaaa-aaaadb_date_sessioni.txt
    set_date_sessioni(leggi_db_date_sessioni(argomenti_es), true);
    display_date_sessioni();
    _dbcal.leggi_corso_db();
    _dbcal.fstampa(options::corsi, true);



}

vector<string> Calendario::leggi_db_date_sessioni(const vector<string> &argomenti_es) {
    ifstream fin;
    const string f_db_sessioni = argomenti_es[2] + file_db_date_sessioni;
    fin.open(f_db_sessioni, ios::in);
    try{ //TODO: eccezioni
        controlli_file(fin, f_db_sessioni);
    }catch(runtime_error &e){
        cout << e.what() << endl;
        cout << "Errore: file " << f_db_sessioni << " db date sessioni Esame non trovato\n";
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

    for(auto i: outstring){
        cout << "outstring leggi db date sessioni: " << i << endl;
    }

    return outstring;
}

Calendario::Calendario(const string &file_argomento) {
    _file_argomento = file_argomento;
}
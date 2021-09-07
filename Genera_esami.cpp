#include "Genera_esami.h"

Genera_esami::calendar::calendar() {

    _sessioni.emplace_back("s1");
    _sessioni.emplace_back("s2");
    _sessioni.emplace_back("s3");

}

bool Genera_esami::calendar::set_id_esame_nel_calendario(const string &id_esame, const vector<string> &id_cds,
                                                         const vector<string> &anno, const int n_slot_necessari,
                                                         const vector<string> &id_professori, int n_vers_paral,
                                                         const vector<string> &semestre_dell_esame) {

    for (int i = 0; i < _sessioni.size(); i++) {
        if (!_sessioni[i].set_id_esame_nella_sessione(id_esame, id_cds, anno, n_slot_necessari, id_professori,
                                                      n_vers_paral,
                                                      semestre_dell_esame)) {
//            cout<<endl<<"Esame "<<id_esame<<" non inserito nella sessione "<<i+1<<"!"<<endl;
            return false;
        }
    }
    return true;
}

void Genera_esami::calendar::print_calendario() {

    cout << endl << "Calendario: " << endl << endl;
    for (int i = 0; i < _sessioni.size(); i++) {
        cout << endl << "\tSessione " << i + 1 << ": " << endl << endl;
        _sessioni[i].print_sessione();
    }

}

Genera_esami::appello::appello(const int quale_appello) {

    _quale_appello = quale_appello;
    if (_quale_appello == 1) {
        _giorni.resize(n_giorni_appello_1);
    } else {
        _giorni.resize(n_giorni_appello_2);
    }

}

bool
Genera_esami::appello::set_id_esame_nell_appello(const string &id_esame, const vector<string> &id_cds,
                                                 const vector<string> &anno,
                                                 const int n_slot_necessari, const vector<string> &id_professori,
                                                 int n_vers_paral) {

    int inserisco_nel_giorno = 0;

    while (inserisco_nel_giorno < _giorni.size()) {
        if ((!trovato_cds_anno(id_cds, anno, inserisco_nel_giorno)) &&
            (prof_disponibili(id_professori, inserisco_nel_giorno))) {
            if (!_giorni[inserisco_nel_giorno].set_id_esame_nel_giorno(id_esame, id_cds, anno, n_slot_necessari,
                                                                       id_professori, n_vers_paral)) {
                inserisco_nel_giorno++;
            } else {
                return true;
            }
        } else {
            inserisco_nel_giorno++;
//            cout<<endl<<"Esame "<<id_esame<<" spostato di un giorno!"<<endl;
        }
    }

    return false;

}

bool
Genera_esami::appello::trovato_cds_anno(const vector<string> &id_cds, const vector<string> &anno,
                                        const int inserisco_nel_giorno) {

    bool trovato = false;
    vector<string>::iterator it_oggi;
    vector<string>::iterator it_ieri;
    vector<string>::iterator it_domani;

    for (int i = 0; i < id_cds.size(); i++) {
        it_oggi = find_cds_anno(_giorni[inserisco_nel_giorno].get_id_cds_inseriti().begin(),
                                _giorni[inserisco_nel_giorno].get_id_cds_inseriti().end(),
                                _giorni[inserisco_nel_giorno].get_anni_inseriti().begin(),
                                id_cds[i], anno[i]);

        if (inserisco_nel_giorno != 0) {
            it_ieri = find_cds_anno(_giorni[inserisco_nel_giorno - 1].get_id_cds_inseriti().begin(),
                                    _giorni[inserisco_nel_giorno - 1].get_id_cds_inseriti().end(),
                                    _giorni[inserisco_nel_giorno - 1].get_anni_inseriti().begin(),
                                    id_cds[i], anno[i]);
        } else {
            it_ieri = _giorni[inserisco_nel_giorno - 1].get_id_cds_inseriti().end();
        }

        if (inserisco_nel_giorno != (_giorni.size() - 2)) {
            it_domani = find_cds_anno(_giorni[inserisco_nel_giorno + 1].get_id_cds_inseriti().begin(),
                                      _giorni[inserisco_nel_giorno + 1].get_id_cds_inseriti().end(),
                                      _giorni[inserisco_nel_giorno + 1].get_anni_inseriti().begin(),
                                      id_cds[i], anno[i]);
        } else {
            it_domani = _giorni[inserisco_nel_giorno + 1].get_id_cds_inseriti().end();
        }

        if ((it_oggi != _giorni[inserisco_nel_giorno].get_id_cds_inseriti().end()) ||
            (it_ieri != _giorni[inserisco_nel_giorno - 1].get_id_cds_inseriti().end()) ||
            (it_domani != _giorni[inserisco_nel_giorno + 1].get_id_cds_inseriti().end())) {
            trovato = true;
        }
    }
    return trovato;
}

// TODO: togliere T e metterci string

template<class InputIterator, class T>
InputIterator
Genera_esami::appello::find_cds_anno(InputIterator first_cds, InputIterator last_cds, InputIterator first_anno,
                                     const T &cds,
                                     const T &anno) {
    while (first_cds != last_cds) {
        if ((*first_cds == cds) && (*first_anno == anno)) return first_cds;
        ++first_cds;
        ++first_anno;
    }
    return last_cds;
}

void Genera_esami::appello::print_appello() {

    for (int i = 0; i < _giorni.size(); i++) {
        cout << endl << "Giorno " << i + 1 << ": " << endl;
        cout << "\t";
        _giorni[i].print_giorno();
        cout << endl;
    }

}

const int Genera_esami::appello::get_quale_appello() const {
    return _quale_appello;
}

bool Genera_esami::appello::prof_disponibili(const vector<string> &id_professori, const int inserisco_nel_giorno) {

    /*for(int i=0; i < id_professori.size(); i++)
    {
        for(int j=0; j < id_professori[i].get_indisponibilita.size(); j++)
        {
            // se inserisco_nel_giorno è dentro indisponibilità allora return false
        }
    }*/
    return true;
}

Genera_esami::giorno::giorno() {

    _fasce_orarie.resize(n_slot);

}

bool Genera_esami::giorno::set_id_esame_nel_giorno(const string &id_esame, const vector<string> &id_cds,
                                                   const vector<string> &anno,
                                                   const int n_slot_necessari, const vector<string> &id_professori,
                                                   int n_vers_paral) {

    bool esame_inserito = true;
    int inserisco_nello_slot = 0;

    while ((inserisco_nello_slot + n_slot_necessari - 1) < n_slot) {
        for (int i = 0; (i < n_slot_necessari) && esame_inserito; i++) {
            if (!_fasce_orarie[i + inserisco_nello_slot].set_id_esame_nello_slot(id_esame, id_professori,
                                                                                 n_vers_paral)) {
                esame_inserito = false;
            }
        }

        if (esame_inserito) {
            for (int j = 0; j < n_vers_paral; j++) {
                _id_cds_inseriti.push_back(id_cds[j]);
                _anni_inseriti.push_back(anno[j]);
            }

            return true;
        } else {
            inserisco_nello_slot++;
            esame_inserito = true;
        }
    }

//    cout<<endl<<"Esame "<<id_esame<<" non inserito nel giorno!"<<endl;
    return false;

}

void Genera_esami::giorno::print_giorno() {

    cout << endl << "------------------------------------------------------------------" << endl;

    for (int i = 0; i < n_slot; i++) {
        cout << endl << "Slot " << i + 1 << ": " << endl;
        cout << "\t";
        _fasce_orarie[i].print_id_esami();
        cout << "\t";
        _fasce_orarie[i].print_professori();
        cout << endl;
    }

    cout << endl << "------------------------------------------------------------------" << endl;

}

vector<string> &Genera_esami::giorno::get_id_cds_inseriti() {
    return _id_cds_inseriti;
}

vector<string> &Genera_esami::giorno::get_anni_inseriti() {
    return _anni_inseriti;
}

Genera_esami::sessione::sessione(const string &quale_sessione) {

    _quale_sessione = quale_sessione;
    if (_quale_sessione != "s3") {
        _appelli.emplace_back(1);
        _appelli.emplace_back(2);
    } else {
        _appelli.emplace_back(2);
    }

}

bool Genera_esami::sessione::set_id_esame_nella_sessione(const string &id_esame, const vector<string> &id_cds,
                                                         const vector<string> &anno, const int n_slot_necessari,
                                                         const vector<string> &id_professori, int n_vers_paral,
                                                         const vector<string> &semestre_dell_esame) {
    vector<bool> inserito_nell_appello(_appelli.size());
    for (auto &&j: inserito_nell_appello) {
        j = true;
    }

    for (int i = 0; i < _appelli.size(); i++) {
        if (_appelli[i].get_quale_appello() == 2) {
            if (!_appelli[i].set_id_esame_nell_appello(id_esame, id_cds, anno, n_slot_necessari, id_professori,
                                                       n_vers_paral)) {
//                cout<<endl<<"Esame "<<id_esame<<" non inserito nell'appello "<<i+1<<" della sessione "<<_quale_sessione<<"!"<<endl;
                inserito_nell_appello[i] = false;
            }
        } else {
            if ((_quale_sessione == semestre_dell_esame[0]) && (_quale_sessione != "s3")) {
                if (!_appelli[i].set_id_esame_nell_appello(id_esame, id_cds, anno, n_slot_necessari, id_professori,
                                                           n_vers_paral)) {
//                    cout<<endl<<"Esame "<<id_esame<<" non inserito nell'appello "<<i+1<<" della sessione "<<_quale_sessione<<"!"<<endl;
                    inserito_nell_appello[i] = false;
                }
            }
        }
    }

    bool all_false = true;
    for (auto &&k: inserito_nell_appello) {
        if (k)
            all_false = false;
    }

    if (all_false)
        return false;
    else
        return true;
}

void Genera_esami::sessione::print_sessione() {

    for (int i = 0; i < _appelli.size(); i++) {
        cout << endl << "Appello " << i + 1 << ": " << endl;
        cout << "\t";
        _appelli[i].print_appello();
        cout << endl;
    }

}

bool Genera_esami::slot::set_id_esame_nello_slot(const string &id_esame, const vector<string> &id_professori,
                                                 int n_vers_parall) {

    vector<string>::iterator it;

    if ((_id_esami.size() + n_vers_parall) > n_aule) {
//        cout<<endl<<"Massima capienza dello slot raggiunta! (esame "<<id_esame<<")"<<endl;
        return false;

    } else {

        for (int j = 0; j < id_professori.size(); j++) {
            it = find(_id_professori.begin(), _id_professori.end(), id_professori[j]);
            if (it != _id_professori.end()) {
//                cout<<endl<< "Professore gia' impegnato con un altro esame!" <<endl;
                return false;
            }
        }
        for (int j = 0; j < id_professori.size(); j++) {
            _id_professori.push_back(id_professori[j]);
        }
        for (int k = 0; k < n_vers_parall; k++) {
            _id_esami.push_back(id_esame);
        }
    }
    return true;
}

void Genera_esami::slot::print_professori() {
    cout << endl << "Professori: ";
    for (auto &i: _id_professori) {
        cout << i << " ";
    }
}

void Genera_esami::slot::print_id_esami() {
    cout << endl << "ID esami: ";
    for (auto &i: _id_esami) {
        cout << i << " ";
    }

}

bool Genera_esami::set_id_esame_nel_calendario(const string &id_esame, const vector<string> &id_cds,
                                               const vector<string> &anno, const int n_slot_necessari,
                                               const vector<string> &id_professori, int n_vers_paral,
                                               const vector<string> &semestre_dell_esame) {
    if (!_cal1.set_id_esame_nel_calendario(id_esame, id_cds, anno, n_slot_necessari, id_professori,
                                           n_vers_paral, semestre_dell_esame)) {
        //            cout<<endl<<"Qualcosa e' andato storto!"<<endl;
        return false;
    }
    return true;
}

void Genera_esami::print_calendar() {

    _cal1.print_calendario();

}


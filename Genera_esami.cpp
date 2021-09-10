#include "Genera_esami.h"

#include <utility>

bool Genera_esami::set_id_esame_nel_calendario(const int n_esami_raggruppati, const vector<string> &id_esame,
                                               const vector<vector<string>> &id_cds, const vector<string> &anno,
                                               const vector<int> &n_slot_necessari,
                                               const vector<vector<string>> &id_professori,
                                               const vector<int> &n_vers_paral,
                                               const int &semestre,
                                               const vector<vector<int>> &n_studenti_iscritti) {
    string semestre_dell_esame;
    if(semestre==1)
        semestre_dell_esame = "s1";
    else
        semestre_dell_esame = "s2";
    if (!_cal1.set_id_esame_nel_calendario(n_esami_raggruppati, id_esame, id_cds, anno, n_slot_necessari, id_professori,
                                           n_vers_paral, semestre_dell_esame, n_studenti_iscritti)) {
        //            cout<<endl<<"Qualcosa e' andato storto!"<<endl;

        return false;
    }
    return true;
}

void Genera_esami::print_calendar() {
    //esempio di come accedere ai dati, da levare poi
    _cal.getDbcal().leggi_db(_cal.getDbcal().getFileDbAule(), _cal.getDbcal().getAuleDb());
    for(auto aula: _cal.getDbcal().getAuleDb()){
        //ciclo tutte le aule nel database
        int capienzaesame = aula->getCapEsame();
    }

    _cal1.print_calendario();

}

Genera_esami::Genera_esami(const vector<string> &argomenti) {
    _cal.getDatiEsami(argomenti);
}

Genera_esami::calendar::calendar() {

    _sessioni.emplace_back("s1");
    _sessioni.emplace_back("s2");
    _sessioni.emplace_back("s3");

}

bool Genera_esami::calendar::set_id_esame_nel_calendario(const int n_esami_raggruppati, const vector<string> &id_esame,
                                                         const vector<vector<string>> &id_cds,
                                                         const vector<string> &anno,
                                                         const vector<int> &n_slot_necessari,
                                                         const vector<vector<string>> &id_professori,
                                                         const vector<int> &n_vers_paral,
                                                         const string &semestre_dell_esame,
                                                         const vector<vector<int>> &n_studenti_iscritti) {

    for (int i = 0; i < _sessioni.size(); i++) {
        if (!_sessioni[i].set_id_esame_nella_sessione(n_esami_raggruppati, id_esame, id_cds, anno, n_slot_necessari,
                                                      id_professori,
                                                      n_vers_paral,
                                                      semestre_dell_esame, n_studenti_iscritti)) {
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

Genera_esami::sessione::sessione(const string &quale_sessione) {

    _quale_sessione = quale_sessione;
    if (_quale_sessione != "s3") {
        _appelli.emplace_back(1);
        _appelli.emplace_back(2);
    } else {
        _appelli.emplace_back(2);
    }

}

bool Genera_esami::sessione::set_id_esame_nella_sessione(const int n_esami_raggruppati, const vector<string> &id_esame,
                                                         const vector<vector<string>> &id_cds,
                                                         const vector<string> &anno,
                                                         const vector<int> &n_slot_necessari,
                                                         const vector<vector<string>> &id_professori,
                                                         const vector<int> &n_vers_paral,
                                                         const string &semestre_dell_esame,
                                                         const vector<vector<int>> &n_studenti_iscritti) {
    //Imposto vettore di appelli a vero
    vector<bool> inserito_nell_appello(_appelli.size());
    for (auto &&j: inserito_nell_appello) {
        j = true;
    }

    //Per ogni appello
    for (int i = 0; i < _appelli.size(); i++) {

        bool esami_gia_messi = false;
        vector<string>::iterator it;
        //Per ogni id esame raggruppato
        for (int j = 0; j < n_esami_raggruppati; j++) {
            //Test se l'esame è già presente nell'appello
            it = find(_appelli[i].get_id_esami_inseriti().begin(), _appelli[i].get_id_esami_inseriti().end(),
                      id_esame[j]);
            if (it != _appelli[i].get_id_esami_inseriti().end()) {
                esami_gia_messi = true;
            }
        }

        //Se non ho già messo l'esame
        if (!esami_gia_messi) {
            int vincolo = 0;
            if (_appelli[i].get_quale_appello() == 2) {
                do {
                    if (!_appelli[i].set_id_esame_nell_appello(n_esami_raggruppati, id_esame, id_cds, anno,
                                                               n_slot_necessari,
                                                               id_professori,
                                                               n_vers_paral, n_studenti_iscritti, vincolo)) {
//                cout<<endl<<"Esame "<<id_esame<<" non inserito nell'appello "<<i+1<<" della sessione "<<_quale_sessione<<"!"<<endl;
                        inserito_nell_appello[i] = false;
                        vincolo++;
                    } else {
                        inserito_nell_appello[i] = true;
                    }
                } while (!inserito_nell_appello[i] && vincolo < 3);

            } else {
                if ((_quale_sessione == semestre_dell_esame) && (_quale_sessione != "s3")) {
                    do {
                        if (!_appelli[i].set_id_esame_nell_appello(n_esami_raggruppati, id_esame, id_cds, anno,
                                                                   n_slot_necessari, id_professori,
                                                                   n_vers_paral, n_studenti_iscritti, vincolo)) {
//                    cout<<endl<<"Esame "<<id_esame<<" non inserito nell'appello "<<i+1<<" della sessione "<<_quale_sessione<<"!"<<endl;
                            inserito_nell_appello[i] = false;
                            vincolo++;
                        } else {
                            inserito_nell_appello[i] = true;
                        }
                    } while (!inserito_nell_appello[i] && vincolo < 3);
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
    else {

        for (int i = 0; i < _appelli.size(); i++) {
            if (inserito_nell_appello[i]) {
                for (int j = 0; j < n_esami_raggruppati; j++) {
                    _appelli[i].get_id_esami_inseriti().push_back(id_esame[j]);
                }
            }
        }
        return true;

    }
}

void Genera_esami::sessione::print_sessione() {

    for (int i = 0; i < _appelli.size(); i++) {
        cout << endl << "Appello " << i + 1 << ": " << endl;
        cout << "\t";
        _appelli[i].print_appello();
        cout << endl;
    }

}

Genera_esami::appello::appello(const int quale_appello) {

    _quale_appello = quale_appello;
//    _id_esami_da_inserire.resize(0);
    if (_quale_appello == 1) {
        _giorni.resize(n_giorni_appello_1);
    } else {
        _giorni.resize(n_giorni_appello_2);
    }

}

bool
Genera_esami::appello::set_id_esame_nell_appello(const int n_esami_raggruppati, const vector<string> &id_esame,
                                                 const vector<vector<string>> &id_cds, const vector<string> &anno,
                                                 const vector<int> &n_slot_necessari,
                                                 const vector<vector<string>> &id_professori,
                                                 const vector<int> &n_vers_paral,
                                                 const vector<vector<int>> &n_studenti_iscritti, int vincolo) {

    int inserisco_nel_giorno = 0;

    while (inserisco_nel_giorno < _giorni.size()) {

//         Per ogni esame del gruppo faccio i controlli, se li passo...
//         ... per ogni esame del gruppo provo ad inserirli nel giorno[inserisco_nel_giorno]...
//         ... se li ho inseriti tutti ritorno true, altrimenti false

        bool mettibile = true;

        for (int i = 0; i < n_esami_raggruppati; i++) {
            if ((trovato_cds_anno(id_cds[i], anno[i], inserisco_nel_giorno, vincolo)) ||
                (!prof_disponibili(id_professori[i], inserisco_nel_giorno, vincolo))) {
                mettibile = false;
            }
        }

        if (mettibile) {
            if (!_giorni[inserisco_nel_giorno].set_id_esame_nel_giorno(n_esami_raggruppati, id_esame, id_cds, anno,
                                                                       n_slot_necessari,
                                                                       id_professori, n_vers_paral,
                                                                       n_studenti_iscritti, vincolo)) {
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
Genera_esami::appello::trovato_cds_anno(const vector<string> &id_cds, const string &anno,
                                        const int inserisco_nel_giorno, int vincolo) {

    bool trovato = false;

    if (vincolo == 0) {
        vector<string>::iterator it_oggi;
        vector<string>::iterator it_ieri;
        vector<string>::iterator it_domani;

        for (int i = 0; i < id_cds.size(); i++) {
            it_oggi = find_cds_anno(_giorni[inserisco_nel_giorno].get_id_cds_inseriti().begin(),
                                    _giorni[inserisco_nel_giorno].get_id_cds_inseriti().end(),
                                    _giorni[inserisco_nel_giorno].get_anni_inseriti().begin(),
                                    id_cds[i], anno);

            if (inserisco_nel_giorno != 0) {
                it_ieri = find_cds_anno(_giorni[inserisco_nel_giorno - 1].get_id_cds_inseriti().begin(),
                                        _giorni[inserisco_nel_giorno - 1].get_id_cds_inseriti().end(),
                                        _giorni[inserisco_nel_giorno - 1].get_anni_inseriti().begin(),
                                        id_cds[i], anno);
            } else {
                it_ieri = _giorni[inserisco_nel_giorno - 1].get_id_cds_inseriti().end();
            }

            if (inserisco_nel_giorno != (_giorni.size() - 2)) {
                it_domani = find_cds_anno(_giorni[inserisco_nel_giorno + 1].get_id_cds_inseriti().begin(),
                                          _giorni[inserisco_nel_giorno + 1].get_id_cds_inseriti().end(),
                                          _giorni[inserisco_nel_giorno + 1].get_anni_inseriti().begin(),
                                          id_cds[i], anno);
            } else {
                it_domani = _giorni[inserisco_nel_giorno + 1].get_id_cds_inseriti().end();
            }

            if ((it_oggi != _giorni[inserisco_nel_giorno].get_id_cds_inseriti().end()) ||
                (it_ieri != _giorni[inserisco_nel_giorno - 1].get_id_cds_inseriti().end()) ||
                (it_domani != _giorni[inserisco_nel_giorno + 1].get_id_cds_inseriti().end())) {
                trovato = true;
            }
        }
    } else {
        vector<string>::iterator it_oggi;

        for (int i = 0; i < id_cds.size(); i++) {
            it_oggi = find_cds_anno(_giorni[inserisco_nel_giorno].get_id_cds_inseriti().begin(),
                                    _giorni[inserisco_nel_giorno].get_id_cds_inseriti().end(),
                                    _giorni[inserisco_nel_giorno].get_anni_inseriti().begin(),
                                    id_cds[i], anno);

        }

        if (it_oggi != _giorni[inserisco_nel_giorno].get_id_cds_inseriti().end()) {
            trovato = true;
        }
    }
    return trovato;
}


template<class InputIterator>
InputIterator
Genera_esami::appello::find_cds_anno(InputIterator first_cds, InputIterator last_cds, InputIterator first_anno,
                                     const string &cds,
                                     const string &anno) {
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

bool Genera_esami::appello::prof_disponibili(const vector<string> &id_professori, const int inserisco_nel_giorno, const int vincolo) {

    //Per indisponibilità serve tutta la struttura
    if(vincolo == 2)
    {
        return true;
    }

//    for (int i = 0; i < id_professori.size(); i++) {
//        for (int j = 0; j < _ind_agg.size(); j++) {
//            if (_ind_agg.get_id_prof == id_professori[i]) {
//                for (int k = 0; k < _ind_agg[j].get_date().size(); k++) {
//                    if ((inserisco_nel_giorno >= _ind_agg[j].get_date()[k].get_inizio()) &&
//                        (inserisco_nel_giorno <= _ind_agg[j].get_date()[k].get_fine())) {
//                        return false;
//                    }
//                }
//            }
//        }
//    }
    return true;
}

vector<string> &Genera_esami::appello::get_id_esami_inseriti() {
    return _id_esami_inseriti;
}

Genera_esami::giorno::giorno() {

    _fasce_orarie.resize(n_slot);

}

bool Genera_esami::giorno::set_id_esame_nel_giorno(const int n_esami_raggruppati, const vector<string> &id_esame,
                                                   const vector<vector<string>> &id_cds, const vector<string> &anno,
                                                   const vector<int> &n_slot_necessari,
                                                   const vector<vector<string>> &id_professori,
                                                   const vector<int> &n_vers_paral,
                                                   const vector<vector<int>> &n_studenti_iscritti, int vincolo) {

//  ragionare a gruppo_di_esami_inserito invece che gruppo_di_esami_inserito
//  e aggiornare _id_cds_inseriti e _anni_inseriti anche per ogni esame del gruppo

    bool gruppo_di_esami_inserito = true;
    int inserisco_nello_slot = 0;
    int tot_n_slot_necessari = 0;

    for (int i = 0; i < n_esami_raggruppati; i++) {
        if (n_slot_necessari[i] >= tot_n_slot_necessari)
            tot_n_slot_necessari = n_slot_necessari[i];

    }

    while ((inserisco_nello_slot + tot_n_slot_necessari - 1) < n_slot) {
        for (int i = 0; (i < tot_n_slot_necessari) && gruppo_di_esami_inserito; i++) {
            if (!_fasce_orarie[i + inserisco_nello_slot].set_id_esame_nello_slot(n_esami_raggruppati, id_esame, id_cds,
                                                                                 id_professori,
                                                                                 const_cast<vector<int> &>(n_vers_paral),
                                                                                 n_studenti_iscritti, vincolo)) {
                gruppo_di_esami_inserito = false;
            }
        }

        if (gruppo_di_esami_inserito) {
            for (int k = 0; k < n_esami_raggruppati; k++) {
                for (int j = 0; j < n_vers_paral[k]; j++) {
                    _id_cds_inseriti.push_back(id_cds[k][j]);
                    _anni_inseriti.push_back(anno[k]);
                }
            }
            return true;
        } else {
            inserisco_nello_slot++;
            gruppo_di_esami_inserito = true;
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
        _fasce_orarie[i].print_info();
        cout << "\t";
//        _fasce_orarie[i].print_professori();
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


bool Genera_esami::slot::set_id_esame_nello_slot(const int n_esami_raggruppati, const vector<string> &id_esame,
                                                 const vector<vector<string>> &id_cds,
                                                 const vector<vector<string>> &id_professori,
                                                 vector<int> &n_vers_paral,
                                                 const vector<vector<int>> &n_studenti_iscritti, int vincolo) {

//          (n_vers_paral * n_raggruppamenti) nel primo if e
//          fare l' _id_esami_da_inserire.push_back per ogni esame raggruppato

    vector<string>::iterator it;
    int n_aule_necessarie = 0;
    for (int i = 0; i < n_esami_raggruppati; i++) {
        n_aule_necessarie = n_aule_necessarie + n_vers_paral[i];
    }

    if ((n_aule_necessarie + _info_da_stampare.size()) > n_aule) {
//        cout<<endl<<"Massima capienza dello slot raggiunta! (esame "<<id_esame<<")"<<endl;
        return false;

    } else {

        for (int j = 0; j < id_professori.size(); j++) {
            for (int k = 0; k < id_professori[j].size(); k++) {
                it = find(_id_professori_inseriti.begin(), _id_professori_inseriti.end(), id_professori[j][k]);
                if (it != _id_professori_inseriti.end()) {
//                cout<<endl<< "Professore gia' impegnato con un altro esame!" <<endl;
                    return false;
                }
            }
        }

        _info_da_inserire.clear();
        int i = 0;
        for (int j = 0; j < n_esami_raggruppati; j++) {
            for (int k = 0; k < n_vers_paral[j]; k++) {
                _info_da_inserire[i]._id_esami_da_inserire = id_esame[j];
                _info_da_inserire[i]._id_cds_da_inserire = id_cds[j][k];
                _info_da_inserire[i]._n_studenti_iscritti = n_studenti_iscritti[j][k];
                _info_da_inserire[i]._id_aula_da_inserire.clear();
                _info_da_inserire[i]._n_versioni_da_inserire = n_vers_paral[j];
                _info_da_inserire[i]._versione_da_inserire = k + 1;
                _info_da_inserire[i]._vincolo_da_inserire = vincolo;
                i++;
            }
        }

//      procedura di ordinamento degli esami da inserire per numero di studenti iscritti decrescente

        for (i = 0; i < _info_da_inserire.size() - 1; i++) {
            for (int j = i + 1; j < _info_da_inserire.size(); j++) {
                if (_info_da_inserire[i]._n_studenti_iscritti < _info_da_inserire[j]._n_studenti_iscritti) {
                    _info_esami_da_inserire temp = _info_da_inserire[i];
                    _info_da_inserire[i] = _info_da_inserire[j];
                    _info_da_inserire[j] = temp;
                }
            }
        }

//      provo ad inserire ogni esame in un'aula. Se uno non ci sta lo divido e provo a vedere se gli altri
//      continuano a starci, altrimenti ritorno false. Se ci sono stati tutti aggiorno il vettore di _info_da_stampare

// Per le aule serve solo id e capienza esame
        int cont;
        int indice_aula = 0;
//        for (i = 0; i < _info_da_inserire.size(); i++) {
//            _info_da_inserire[i]._id_aula_da_inserire.push_back(_aula[indice_aula].getId);
//            cont = 1;
//            if (_info_da_inserire[i]._n_studenti_iscritti > _aula[indice_aula].getCapeinza) {
//                do {
//                    if ((n_aule_necessarie + _info_da_stampare.size()) + cont > n_aule) {
////                    Il gruppo di esami non ci sta in questo slot
//                        return false;
//                    }
////                    _n_studenti_iscritti[i] = (_n_studenti_iscritti[i] - (_n_studenti_iscritti[i] % 2)) / 2;
//                    _info_da_inserire[i]._n_studenti_iscritti =
//                            _info_da_inserire[i]._n_studenti_iscritti - _aula[indice_aula + cont - 1].getCapienza;
//                    _info_da_inserire[i]._id_aula_da_inserire.push_back(_aula[indice_aula + cont].getId);
//                    cont++;
//                } while (_info_da_inserire[i]._n_studenti_iscritti > _aula[indice_aula + cont].getCapeinza);
//            }
//            indice_aula = indice_aula + cont;
//        }

        for (int j = 0; j < id_professori.size(); j++) {
            for (int k = 0; k < id_professori[j].size(); k++) {
                _id_professori_inseriti.push_back(id_professori[j][k]);
            }
        }


        for (i = 0; i < _info_da_inserire.size(); i++) {
            _info_da_stampare[i]._id_esame_inserito = _info_da_inserire[i]._id_esami_da_inserire;
            _info_da_stampare[i]._id_cds_inserito = _info_da_inserire[i]._id_cds_da_inserire;
            _info_da_stampare[i]._id_aula_inserita = _info_da_inserire[i]._id_aula_da_inserire;
            _info_da_stampare[i]._n_versioni_inserito = _info_da_inserire[i]._n_versioni_da_inserire;
            _info_da_stampare[i]._versione_inserita = _info_da_inserire[i]._versione_da_inserire;
            _info_da_stampare[i]._vincolo_inserito = _info_da_inserire[i]._vincolo_da_inserire;
        }
    }
    return true;
}

bool Genera_esami::slot::maggior_n_studenti(int n_studenti_iscritti_1, int n_studenti_iscritti_2) {
    return n_studenti_iscritti_1 > n_studenti_iscritti_2;
}

/*void Genera_esami::slot::print_professori() {
    cout << endl << "Professori: ";
    for (auto &i: _id_professori_inseriti) {
        cout << i << " ";
    }
}*/

void Genera_esami::slot::print_info() {
//    cout << endl << "Esami: " << endl;
    for (int i = 0; i < _info_da_stampare.size(); i++) {
        cout << ";" << _info_da_stampare[i]._id_esame_inserito;
        if (_info_da_stampare[i]._n_versioni_inserito > 1) {
            cout << "[-" << _info_da_stampare[i]._versione_inserita << "]";
        }
        cout << "(" << _info_da_stampare[i]._id_cds_inserito << ")";
        for (int j = 0; j < _info_da_stampare[i]._id_aula_inserita.size(); j++) {
            cout << "|" << _info_da_stampare[i]._id_aula_inserita[j];
        }
    }
}

void Genera_esami::slot::print_info_warnings() {
    for (int i = 0; i < _info_da_stampare.size(); i++)
    {
        if(_info_da_stampare[i]._vincolo_inserito > 0)
        {
            // Stampo primo vincolo violato
            cout << _info_da_stampare[i]._id_cds_inserito << ";" << _info_da_stampare[i]._id_esame_inserito << "; "
                   "Distanza minima tra esami dello stesso anno accademico e dello stesso corso di studi a meno di due giorni l'uno dall'altro" << endl;
        }
        if(_info_da_stampare[i]._vincolo_inserito > 1)
        {
            // Stampo terzo vincolo violato
            cout << _info_da_stampare[i]._id_cds_inserito << ";" << _info_da_stampare[i]._id_esame_inserito << "; "
                   "Indisponibilità dei docenti non rispettata" << endl;
        }
    }
}

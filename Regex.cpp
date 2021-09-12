
#include "Database.h"
#include "Eccezioni.h"

//TODO rivedi metodi statici

//prende una espressione regolare, una stringa da cui leggere e un vettore in cui salvare le stringhe lette separate
void Database::Regex::search_and_read(const std::regex &expression, const string &row, vector<string> &out) {

    if (regex_search(row, _match, expression)) {
        for (int i = 0; i < _match.length(); i++) {
            if (_match[i].matched) {
//                cout << "Match " << i << ": " << _match[i] << endl;
                out.push_back(_match[i]);
            }
        }
        return;
    } else {
        cout << "Nella riga: " << row << endl;
        throw errore_formattazione();
        return;
    }
}

//TODO: attenzione: non rileva se manca un campo
//prende una espressione regolare, una stringa da cui leggere e un vettore in cui salvare le stringhe lette separate
void Database::Regex::multiple_fields(const std::regex &expression, const string &row, vector<string> &out) {
    auto it_begin = sregex_iterator(row.begin(), row.end(), expression);
    auto it_end = sregex_iterator();

//    cout << "Found " << distance(it_begin, it_end) << " different fields\n";

    for (sregex_iterator i = it_begin; i != it_end; ++i) {
        _match = *i;
        string match_str = _match.str();
        out.push_back(match_str);
//        cout << match_str << endl;
    }

}

std::regex Database::Regex::target_expression(lettura::reg_expressions exp) {
    switch (exp) {
        //studenti su file database
        case lettura::studenti_db:
            return std::regex(_rg_matricola_s + _rg_persona_in);

            //studenti su file di aggiunta
        case lettura::studenti_in:
            return std::regex(_rg_persona_in);

            //professori su file database
        case lettura::professori_db:
            return std::regex(_rg_matricola_d + ';' + _rg_persona_in);

            //professori su file di aggiunta
        case lettura::professori_in:
            return std::regex(_rg_persona_in);

            //aule su file database
        case lettura::aule_db:
            return std::regex(_rg_id_aula + _rg_aula_in);

            //aule su file di aggiunta
        case lettura::aule_in:
            return std::regex(_rg_aula_in);

            //corsi su file database
        case lettura::corsi_db:
            return std::regex(_rg_corso_db_base);

            //corsi su file di aggiunta
        case lettura::corsi_in:
            return std::regex(_rg_corso_in_base);

        case lettura:: anno_versioni:
            return std::regex(_rg_versioni);

        case lettura::n_versioni:
            return std::regex (_rg_n_versioni);

        case lettura::prof_titolare:
            return std::regex(_rg_matricola_d);

            //corsi di studio su file di aggiunta
        case lettura::cds_in:
            return std::regex(_rg_cds_row);

            //corsi di studio su file db
        case::lettura::cds_db:
            return std::regex(_rg_cds_db);

            //id corso raggruppati per semestre
        case lettura::cds_semestri:
            return std::regex(_rg_cds_semestri);

            //corsi da inserire
        case lettura::corsi_inserimento:
            cout << "rg:" << _rg_corso_ins << endl;
            return std::regex(_rg_corso_ins);

            //id corso singoli
        case lettura::cds_id_corso:
            return std::regex(_rg_id_corso);

            //TODO: da qui in poi quanti servono?
        case lettura::prof_singolo:
            return std::regex(_rg_profn_graffe);

        case lettura::profn_campi:
            return std::regex(_rg_profn_campi);

        case lettura::sessioni:
            return std::regex(_rg_periodo);

        case lettura::anno_acc:
            return std::regex(_rg_anno_acc);

        case lettura::id_prof:
            return std::regex(_rg_matricola_d);

        case lettura::data:
            return std::regex(_rg_data);

        case lettura::periodo:
            return std::regex(_rg_periodo);

        case lettura::esame_campi:
            return std::regex(_rg_esame_campi);

        case lettura::esame_graffe:
            return std::regex(_rg_esame_graffe);

        default:
            cout << "Errore target regexp\n";
    }
    return {};
}

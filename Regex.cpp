
#include "Database.h"

//TODO rivedi metodi statici

bool Database::Regex::search_and_read(const std::regex &expression, const string &row, vector<string> &out) {

    if (regex_search(row, _match, expression)) {
        for (int i = 0; i < _match.length(); i++) {
            if (_match[i].matched) {
//                cout << "Match " << i << ": " << _match[i] << endl;
                out.push_back(_match[i]);
            }
        }
        return true;
    } else {
        cout << "No _match found for: " << row << endl;
        return false;
    }
}

//Può estrarre espressioni ripetute un numero non noto di volte, usa per profn, idcorsi raggruppati, indisponibilita ecc
void Database::Regex::multiple_fields(const std::regex &expression, const string &row, vector<string> &out){
    auto it_begin = sregex_iterator (row.begin(), row.end(), expression);
    auto it_end = sregex_iterator();

    cout << "Found " << distance(it_begin, it_end) << " different fields\n";

    for(sregex_iterator i = it_begin; i != it_end; ++i){
        smatch match = *i;
        string match_str = match.str();
        out.push_back(match_str);
//        cout << match_str << endl;
    }
}

//estrae regex multiple da una sola stringa
vector<string>
Database::Corso::Anno_Accademico::estraimultipli(const regex &reg, string &daleggere, const string &delim) {
    vector<string> v_temp;
    smatch match;
    while (regex_search(daleggere, match, reg)) {
        for (int i = 0; i < match.size(); i++) {
            if (match[i].matched) {
//                cout << "MATCH " << i << " : " << _match[i] << endl;
                if (i) {
                    v_temp.push_back(match[i]);
                }
            }
        }
        daleggere = daleggere.substr(daleggere.find(delim) + 1, daleggere.length());
    }

    return v_temp;
}


vector<string> Database::Corso::cut_versioni(const string &row, const vector<int> &indicigraffe, int n_versioni) {
    vector<string> out;
    string temp;
    size_t start;
    size_t end;

    //Salva gli indici delle graffe e poi estrae il contenuto
    for (int i = 0; i < n_versioni * 2; i += 2) {
        start = indicigraffe[i] + 1;
        end = indicigraffe[i + 1] - indicigraffe[i];
        try { //TODO: eccezioni
            temp = row.substr(start, end);
        } catch (std::out_of_range &e) {
            cout << e.what() << "\nROW: " << row << endl;

        }
//        cout << "DEBUG TEMP cut_versioni: " << temp << endl;
        out.push_back(temp);
    }
    return out;
}


std::regex Database::Regex::target_expression(lettura::reg_expressions exp) {
    switch (exp) {
        case lettura::studenti_db:
            return std::regex(_matricola_s + _persona_in);

        case lettura::studenti_in:
            return std::regex(_persona_in);

        case lettura::professori_db:
            return std::regex(_matricola_d + _persona_in);

        case lettura::professori_in:
            return std::regex(_persona_in);

        case lettura::aule_db:
            return std::regex(_id_aula + _aula_in);

        case lettura::aule_in:
            return std::regex(_aula_in);

        case lettura::corsi_in:
            return std::regex(_corso_in_base);

        case lettura::id_corsi_in:
            return std::regex(_id_corso);

        case lettura::cds_in:
            return std::regex(_id_cds);

        case lettura::corsi_db:
            LOG(corso_db_base);
            return std::regex(corso_db_base);

        case lettura::prof_singolo:
            return std::regex(_profn_graffe);

        case lettura::profn_campi:
            return std::regex(_profn_campi);

        case lettura::sessioni:
            return std::regex(_periodo);

        case lettura::anno_acc:
            return std::regex(_anno_acc);

        case lettura::indisp:
            return std::regex(_indisponibilita);

        case lettura::esame_campi:
            return std::regex(_esame_campi);

        case lettura::esame_graffe:
            return std::regex(_esame_graffe);

        default:
            cout << "Errore target regexp\n";
    }
    return std::regex();
}

Database::Regex::Regex() {
    _id_corso = "([A-Z0-9]+)";
    corso_db_base = "c;" + _id_corso + ';' + _text + ';' + _num + ';' + _num + ';' + _num + ';' + _num + ';';


    _anno_acc = "([0-9]{4})-([0-9]{4})";
    _esame_campi = "\\{" + _num + "," + _num + "," + _num + "," + "([SOP]*),([AL])\\}";
    _esame_graffe = "\\{([0-9,SOPAL]+)\\}";


    //corso di studi : C120;BS;[{AXC345,BVX123},{CBV123,ASD564}]
    _cds = _id_cds + ";" + _laurea + ";";
    _laurea = "([BS|MS])";
    _id_cds = "([A-Z0-9]+)";
    _id_corso_del_semestre_n = "\\{" + _id_corso_n;
    _id_corso_del_semestre = "\\[" + _id_corso_del_semestre_n;

    //LETTURA CORSI IN
    _corso_in_base = _anno_acc + ';' + _text + ';' + _num + ';' + _num + ';' + _num + ';' + _num + ';' + _num;
    _profn_graffe = "\\{([0-9,d]+)\\}";
    _profn_campi = "([0-9d]+),([0-9]+),([0-9]+),([0-9]+)";


    //ESPRESSIONI PER DATE ESAMI
    _data = "([0-9]{1,2})\\-([0-9]{1,2})\\-([0-9]{4})";
    _periodo = _data + "\\|" + _data;
    _indisponibilita = _matricola_d;
}

bool Database::BracketSearch::IsOpenBracket(char InCharacter) {
    vector<char> openCharacters = {'{', '(', '['};
    if (count(openCharacters.begin(), openCharacters.end(), InCharacter)) {
        return true;
    } else {
        return false;
    }
}

bool Database::BracketSearch::IsClosedBracket(char InCharacter) {
    vector<char> closedCharacters = {'}', ')', ']'};
    if (count(closedCharacters.begin(), closedCharacters.end(), InCharacter)) {
        return true;
    } else {
        return false;
    }
}

bool Database::BracketSearch::balancedBrackets(const string &str) {
    stack<char> s;
    map<char, char> matching_brackets =
            {
                    {'}', '{'},
                    {')', '('},
                    {']', '['}
            };

    for (char currentCharacter : str) {
        if (IsOpenBracket(currentCharacter)) {
            s.push(currentCharacter);
        } else if (IsClosedBracket(currentCharacter)) {
            if (s.empty()) {
                return false;
            }

            if (s.top() == matching_brackets[currentCharacter]) {
                s.pop();
            } else {
                return false;
            }
        }
    }

    if (s.empty()) {
        return true;
    }

    return false;
}

vector<int> Database::BracketSearch::posBrackets(const string &str) {
    stack<char> s;
    vector<int> pos;
//    vector<Versione> v;
//    vector<pair<int, int>> v;
    map<char, char> matching_brackets =
            {
                    {'}', '{'},
                    {')', '('},
                    {']', '['}
            };

    for (int i = 0; i < str.size(); i++) {
        if (str[i] == '{') {
            s.push(str[i]);
            if (s.size() == 1) {
                pos.push_back(i);
            }
        } else if (str[i] == '}') {
            if (s.size() == 1) {
                pos.push_back(i);
            }
            if (!s.empty()) {
                if (s.top() == matching_brackets[str[i]]) {
                    s.pop();
                }

            }
        }
    }

    return pos;
}


#include "Database.h"
#include "Calendario.h"

/*  AGGIUNTA
 * -a:s studenti
 * -a:d prof
 * -a:a aule
 * -a:c corsi
 * -a:f cds
 *
 * AGGIORNAMENTO
 * -u:s studenti
 * -u:d professori
 * -u:a aule
 *
 *  INSERIMENTO
 *  -i:c
 *
 *  ESAMI
 *  -s current_a <anno_accademico> <data_inizio>|<data_fine> <data_inizio>|<data_fine> <data_inizio>|<data_fine>
 *  -s set_availability <anno_accademico> <file_indisponibilità>
 *  -g <anno_accademico> <output_file>
 *
 */

using std::cout;

vector<string> converti_argv(int f_argc, char *f_argv[]);

int main(int argc, char *argv[]) {

    //controlli argc su ogni switch?
    if (argc < 3) { //TODO controlla questo numero
        cout << "Errore numero argomenti\n";
        //TODO: eccezione
        return -1;
    }

    //Da levare
    cout << "Argomenti inseriti: ";
    for (int i = 1; i < argc; i++) {
        const string argvs(argv[i]);
        cout << argvs << ' ';
    }
    cout << '\n';

    try {

        options::procedura p_var;
        options::opzione o_var;
        p_var = static_cast<options::procedura>(argv[1][1]); //variabile procedura
        o_var = static_cast<options::opzione>(argv[1][3]); //variabile opzione

        string file_argomento(argv[2]);

        //Dichiaro il database con il file da aprire come argomento, così tutti i metodi principali possono accederci direttamente
        Database db(file_argomento);
        Calendario cal(file_argomento);

//    std::cout << "argomento1: "<< argv[1] << "\ndebug: " << (char) p_var << ' ' << (char) o_var << '\n';
//    std::cout << "argomento2: " << argv[2] << '\n';

        switch (p_var) {
            case (options::aggiunta):
                if (argc != 3) {
                    throw err_parametri_linea_di_comando();
                    assert(argc != 3);
                }
                switch (o_var) {
                    case (options::studenti): {
                        cout << "Hai scelto: " << "aggiunta studenti\n";
                        db.target_aggiungi(o_var);
                        db.target_fstampa(o_var, true);

                        break;
                    }
                    case (options::professori): {
                        cout << "Hai scelto: " << "aggiunta professori\n";
                        db.target_aggiungi(o_var);
                        db.target_fstampa(o_var, true);

                        break;
                    }
                    case (options::aule): {
                        cout << "Hai scelto: " << "aggiunta aule\n";
                        db.target_aggiungi(o_var);
                        db.target_fstampa(o_var, true);

                        break;
                    }
                    case (options::corsi):
                        cout << "Hai scelto: " << "aggiunta corsi\n";
                        db.target_aggiungi(o_var);
                        db.target_fstampa(o_var, true);

                        break;
                    case (options::cds):
                        cout << "Hai scelto: " << "aggiunta cds\n";
                        db.target_aggiungi(o_var);
                        db.target_fstampa(o_var, true);

                        break;
                    default:
                        cout << "Errore inserimento parametri procedura aggiunta\n";
                        throw err_parametri_linea_di_comando();
                }
                break;
            case (options::aggiornamento):
                switch (o_var) {
                    case (options::studenti):
                        cout << "Hai scelto: " << "aggiornamento studenti\n";
                        //TODO: template aggiorna
//                        db.aggiorna_studenti();
                        db.target_aggiorna(o_var);
                        db.target_fstampa(o_var, false);

                        break;
                    case (options::professori):
                        cout << "Hai scelto: " << "aggiornamento professori\n";

//                        db.aggiorna_professori();
                        db.target_aggiorna(o_var);
                        db.target_fstampa(o_var, false);

                        break;
                    case (options::aule):
                        cout << "Hai scelto: " << "aggiornamento aule\n";

                        db.aggiorna_aule();
                        db.target_fstampa(o_var, false);

                        break;
                    default:
                        //TODO: eccezione
                        cout << "Errore inserimento parametri procedura aggiornamento\n";
                        throw err_parametri_linea_di_comando();
                }
                break;
            case (options::inserimento):
                cout << "Hai scelto: " << "inserimento corsi\n";


                break;
            case (options::set_esami):
                switch (o_var) {
                    case (options::current_a):
                        cout << "Hai scelto: " << "impostazione sessioni d'Esame anno accademico\n";

                        cal.set_date_sessioni(converti_argv(argc, argv), false);
                        cal.fstampa_date_sessioni();

                        break;
                    case (options::set_av):
                        cout << "Hai scelto: " << "impostazione periodi di indisponibilità\n";

                        cal.set_indisponibilita(converti_argv(argc, argv));
                        cal.fstampa_indisponibilita();

                        break;
                    default:
                        LOG("Errore argomento impostazione esami\n");
                        throw err_parametri_linea_di_comando();
                }
                break;
            case (options::gen_esami):
                cout << "Hai scelto: " << "generazione _date esami\n";

                cal.genera_date_esami(converti_argv(argc, argv));

                break;
            default:
                cout << "Errore argomento procedura\n";
                throw err_parametri_linea_di_comando();
        }
    } catch (err_parametri_linea_di_comando &e) {
        cout << "errore trovato" << e.what() << endl;
        exit(6);
    } catch (file_non_aperto &e) {
        cout << "errore trovato" << e.what() << endl;
        exit(-1);
    } catch (file_non_chiuso &e) {
        cout << "errore trovato" << e.what() << endl;
        exit(-1);
    } catch (file_failed &e) {
        cout << "errore trovato" << e.what() << endl;
        exit(-1);
    } catch (errore_matricola &e) {
        cout << "errore trovato" << e.what() << endl;
        exit(4);
    } catch (errore_riga_vuota &e) {
        cout << "errore trovato" << e.what() << endl;
        exit(7);
    } catch (errore_non_univoco &e) {
        cout << "errore trovato" << e.what() << endl;
        exit(8);
    } catch (errore_incongruenza_file &e) {
        cout << "errore trovato" << e.what() << endl;
        exit(9);
    }
    return 0;
}


vector<string> converti_argv(int f_argc, char *f_argv[]) {
    vector<string> v_temp;
    for (int i = 0; i < f_argc; i++) {
        const string s_temp(f_argv[i]);
        v_temp.push_back(s_temp);
    }
    return v_temp;
}


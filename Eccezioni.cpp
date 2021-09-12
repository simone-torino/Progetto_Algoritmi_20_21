
#include "Eccezioni.h"




file_non_aperto::file_non_aperto():messaggio("Apertura file fallita, forse non e' stato trovato") {}
const char * file_non_aperto::what() const noexcept {return messaggio;}


file_non_chiuso::file_non_chiuso():messaggio("file non chiuso correttamente") {}
const char * file_non_chiuso::what() const noexcept {return messaggio;}


file_failed::file_failed():messaggio("nel file sono presenti degli error state flag ") {}
const char * file_failed::what() const noexcept {return messaggio;}


errore_stringa_non_convert_in_int::errore_stringa_non_convert_in_int(): messaggio("Stringa non valida non puo essere convertita in int.") {}
const char * errore_stringa_non_convert_in_int::what() const noexcept {return messaggio;}


errore_anno_accademico::errore_anno_accademico():messaggio("Errore anno accademico non valido, inserire due anni contigui") {}
const char * errore_anno_accademico::what() const noexcept {return messaggio;}


errore_matricola::errore_matricola():messaggio("formattazione matricola errata") {}
const char * errore_matricola::what() const noexcept {return messaggio;}


errore_non_univoco::errore_non_univoco():messaggio("persona gia trovata, errore di non univocità, deve essere univoco") {}
const char * errore_non_univoco::what() const noexcept {return messaggio;}


errore_formattazione::errore_formattazione(): messaggio("è presente un errore di formattazione") {}
const char * errore_formattazione::what() const noexcept {return messaggio;}


err_periodo::err_periodo():messaggio("La data di inizio e' successiva alla data di fine") {}
const char * err_periodo::what() const noexcept {return messaggio;}


prof_non_presente::prof_non_presente():messaggio("matricola del professore non presente nell'elenco del file database") {}
const char * prof_non_presente::what() const noexcept {return messaggio;}


err_parametri_linea_di_comando::err_parametri_linea_di_comando():messaggio("Parametri passati da linea di comando non conformi alle richieste") {}
const char * err_parametri_linea_di_comando::what() const noexcept {return messaggio;}


errore_incongruenza_file::errore_incongruenza_file():messaggio("i file non sono gli stessi non è possibile svolgere le operazioni") {}
const char * errore_incongruenza_file::what() const noexcept {return messaggio;}


err_check_anno_accademico::err_check_anno_accademico():messaggio("Almeno una data non corrisponde all'anno accademico di riferimento"){}
const char* err_check_anno_accademico::what() const noexcept {return messaggio;}


errore_riga_vuota::errore_riga_vuota():messaggio("riga nel file vuota") {}
const char * errore_riga_vuota::what() const noexcept {return messaggio;}


errore_formattazione_id_corsi::errore_formattazione_id_corsi():messaggio("le righe devono iniziare per 'c' o 'a';") {}
const char * errore_formattazione_id_corsi::what()const noexcept {return messaggio;}


err_anno_senza_corso::err_anno_senza_corso():messaggio(" trovato anno accademico senza corso di riferimento") {}
const char * err_anno_senza_corso::what() const noexcept {return messaggio;}


file_vuoto::file_vuoto():messaggio("Il file è vuoto") {}
const char* file_vuoto::what() const noexcept {return messaggio;}


err_corso_senza_anno::err_corso_senza_anno():messaggio(" trovato corso senza informazioni sugli anni accademici") {}
const char * err_corso_senza_anno::what() const noexcept {return messaggio;}


err_formattazione_attivo_non_attivo::err_formattazione_attivo_non_attivo():messaggio("Errore formattazione del parametro 'Attivo/Non attivo'.") {}
const char * err_formattazione_attivo_non_attivo::what() const noexcept {return messaggio;}


err_numero_versioni_parallele::err_numero_versioni_parallele():messaggio("Il numero di ID degli esami paralleli differisce dalla quantità atteso") {}
const char * err_numero_versioni_parallele::what() const noexcept {return messaggio;}


err_corso_non_trovato_nel_cds::err_corso_non_trovato_nel_cds():messaggio("Id del corso non trovato all'interno del corso di studio."){}
const char* err_corso_non_trovato_nel_cds::what() const noexcept {return messaggio;}


err_corso_spento_non_trovato_nel_cds::err_corso_spento_non_trovato_nel_cds():messaggio("Id del corso spento non trovato all'interno del corso di studio."){}
const char* err_corso_spento_non_trovato_nel_cds::what() const noexcept {return messaggio;}
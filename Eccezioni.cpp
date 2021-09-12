
#include "Eccezioni.h"




file_non_aperto::file_non_aperto():messaggio("Apertura file fallita, forse non e' stato trovato") {}

const char * file_non_aperto::what() const noexcept {
    return messaggio;
}


file_non_chiuso::file_non_chiuso():messaggio("file non chiuso correttamente") {}

const char * file_non_chiuso::what() const noexcept {
    return messaggio;
}

file_failed::file_failed():messaggio("nel file sono presenti degli error state flag ") {}
const char * file_failed::what() const noexcept {return messaggio;}

errore_stringa_non_convert_in_int::errore_stringa_non_convert_in_int(): messaggio("Stringa non valida non puo essere convertita in int.") {}
const char * errore_stringa_non_convert_in_int::what() const noexcept {return messaggio;}

/*errore_intero::errore_intero():messaggio("non è un numero intero") {}

const char * errore_intero::what() const noexcept {
    return messaggio;
}


errore_data::errore_data():messaggio("formattazione data errata") {}

const char * errore_data::what() const noexcept {
    return messaggio;
}*/

errore_anno_accademico::errore_anno_accademico():messaggio("Errore anno accademico non valido, inserire due anni contigui") {}

const char * errore_anno_accademico::what() const noexcept {
    return messaggio;
}


errore_matricola::errore_matricola():messaggio("formattazione matricola errata") {}

const char * errore_matricola::what() const noexcept {
    return messaggio;
}


/* errore_durata::errore_durata():messaggio("formattazione durata errata") {}

const char * errore_durata::what() const noexcept {
    return messaggio;
}


errore_lunghezza_periodo::errore_lunghezza_periodo():messaggio("lunghezza periodo esame non conforme alle date") {}

const char * errore_lunghezza_periodo::what() const noexcept {
    return messaggio;
} */

errore_non_univoco::errore_non_univoco():messaggio("persona gia trovata, errore di non univocità, deve essere univoco") {}

const char * errore_non_univoco::what() const noexcept {
    return messaggio;
}

errore_formattazione::errore_formattazione(): messaggio("è presente un errore di formattazione") {}

const char * errore_formattazione::what() const noexcept {
    return messaggio;
}

/* err_periodi_esami::err_periodi_esami():messaggio("periodo esami non conforme alle norme, 6 detitmane per le prime due sessioni e 4 per quella di recupero") {}

const char * err_periodi_esami::what() const noexcept {
    return messaggio;
}

err_fascia_oraria::err_fascia_oraria():messaggio("la durata dell'esame + i tempi di ingresso e uscita non rispettano la fascia orario") {}

const char * err_fascia_oraria::what() noexcept {
    return messaggio;
}

err_appelli_per_semestre::err_appelli_per_semestre():messaggio("numero di appelli nel semestre non corretto") {}

const char * err_appelli_per_semestre::what() noexcept {
    return messaggio;
}

err_settimana_appello::err_settimana_appello():messaggio("settimana appello non congruente") {}
const char * err_settimana_appello::what() noexcept {return messaggio;}

err_giorno::err_giorno():messaggio("giorno non valido") {}
const char * err_giorno::what() const noexcept {
    return messaggio;
}

err_mese::err_mese():messaggio("mese non valido") {}
const char * err_mese::what() const noexcept {
    return messaggio;
}



err_date_nei_periodi::err_date_nei_periodi():messaggio("la data non è conforme al periodo") {}
const char* err_date_nei_periodi::what() const noexcept {
    return messaggio;
} */

err_periodo::err_periodo():messaggio("La data di inizio e' precedente alla data di fine") {}
const char * err_periodo::what() const noexcept {
    return messaggio;
}

prof_non_presente::prof_non_presente():messaggio("matricola del professore non presente nell'elenco del file database") {}
const char * prof_non_presente::what() const noexcept {
    return messaggio;
}


/* err_data_per_anno::err_data_per_anno():messaggio("almeno una data non è presente nell'anno") {}
const char * err_data_per_anno::what() noexcept {
    return messaggio;
}

err_prof_indisponibili::err_prof_indisponibili():messaggio("le date degli esami non rispettano le date delle disponibilità dei professori") {}
const char * err_prof_indisponibili::what() noexcept {
    return messaggio;
} */

err_parametri_linea_di_comando::err_parametri_linea_di_comando():messaggio("Parametri passati da linea di comando non conformi alle richieste") {}
const char * err_parametri_linea_di_comando::what() const noexcept {
    return messaggio;
}

/* err_procedura::err_procedura():messaggio("non è stato possibile svolgere la procedura richiesta") {}
const char * err_procedura::what() noexcept {
    return messaggio;
}

err_inserimento_parametri::err_inserimento_parametri():messaggio("Errore inserimento parametri per stampa su file") {}
const char * err_inserimento_parametri::what() noexcept {return messaggio;
} */



/* err_versioni_corso::err_versioni_corso():messaggio("numero di versioni non congruente con la ruga del file") {}
const char * err_versioni_corso::what() noexcept {return messaggio;
}

errore_cfu::errore_cfu():messaggio("cfu non è un intero") {}
const char * errore_cfu::what() const noexcept {return messaggio;
} */

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

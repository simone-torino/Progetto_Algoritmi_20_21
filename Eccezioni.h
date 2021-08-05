#ifndef PROGETTO_3_ECCEZIONI_H
#define PROGETTO_3_ECCEZIONI_H

#include <exception>
#include <cassert>



class file_non_aperto: public std::exception {
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    file_non_aperto();
};


class file_non_chiuso: public std:: exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    file_non_chiuso();
};

class file_failed: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    file_failed();
};


class errore_stringa: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    errore_stringa();
};


class errore_intero: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    errore_intero();
};

class err_date_nei_periodi: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    err_date_nei_periodi();
};

class errore_data: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    errore_data();
};


class errore_anno_accademico: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    errore_anno_accademico();
};


class errore_matricola: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    errore_matricola();
};

class errore_cfu:public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    errore_cfu();
};

class errore_durata: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    errore_durata();
};


class errore_lunghezza_periodo: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    errore_lunghezza_periodo();
};


class errore_non_univoco: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    errore_non_univoco();
};

class err_formattazione_errara: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    err_formattazione_errara();

};

class err_giorno: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    err_giorno();
};

class err_mese: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    err_mese();
};

class err_anno: public std::exception{
private:
    const char* messaggio;
public:
    const char* what()const noexcept;
    err_anno();
};

class err_periodi_esami: public std::exception{
private:
    const char* messaggio;
public:
    const char* what()const noexcept;
    err_periodi_esami();
};

class err_fascia_oraria: public std::exception{
private:
    const char* messaggio;
public:const char* what() noexcept;
    err_fascia_oraria();
};

class err_appelli_per_semestre: public std::exception{
private:
    const char* messaggio;
public:const char* what() noexcept;
    err_appelli_per_semestre();
};


class err_settimana_appello: public std::exception{
private:
    const char* messaggio;
public:const char* what() noexcept;
    err_settimana_appello();
};


class err_dist_minima_stesso_vorso: public std::exception{
private:
    const char* messaggio;
public:const char* what() noexcept;
    err_dist_minima_stesso_vorso();
};

class err_periodo: public std::exception{
private:
    const char* messaggio;
public:const char* what() noexcept;
    err_periodo();
};

class prof_non_presente: public std::exception{
private:
    const char* messaggio;
public:const char* what() noexcept;
    prof_non_presente();
};

class err_data_per_anno:public std::exception{
private:
    const char* messaggio;
public:const char* what() noexcept;
    err_data_per_anno();
};

class err_prof_indisponibili:public std::exception{
private:
    const char* messaggio;
public:
    const char* what() noexcept;
    err_prof_indisponibili();

};

class err_parametri_linea_di_comando: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() noexcept;
    err_parametri_linea_di_comando();

};

class err_procedura: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() noexcept;
    err_procedura();

};

class err_inserimento_parametri:public std::exception{
private:
    const char* messaggio;
public:
    const char* what() noexcept;
    err_inserimento_parametri();
};

class errore_riga_vuota:public std::exception{
private:
    const char* messaggio;
public:
    const char* what() noexcept;
    errore_riga_vuota();
};

class err_versioni_corso:public std::exception{
private:
    const char* messaggio;
public:
    const char* what() noexcept;
    err_versioni_corso();
};

class errore_incongruenza_file:public std::exception{
private:
    const char* messaggio;
public:
    const char* what() noexcept;
    errore_incongruenza_file();
};



#endif //PROGETTO_3_ECCEZIONI_H

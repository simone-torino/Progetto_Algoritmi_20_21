#ifndef PROGETTO_ALGORITMI_20_21_ECCEZIONI_H
#define PROGETTO_ALGORITMI_20_21_ECCEZIONI_H

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


class file_vuoto: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    file_vuoto();
};


class errore_stringa_non_convert_in_int: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    errore_stringa_non_convert_in_int();
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


class errore_non_univoco: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    errore_non_univoco();
};


class errore_formattazione: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    errore_formattazione();

};


 class err_anno_senza_corso: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    err_anno_senza_corso();
};


class err_corso_senza_anno: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    err_corso_senza_anno();
};


class err_formattazione_attivo_non_attivo: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    err_formattazione_attivo_non_attivo();
};


class errore_formattazione_id_corsi: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    errore_formattazione_id_corsi();
};


class err_periodo: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    err_periodo();
};


class prof_non_presente: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    prof_non_presente();
};


class err_parametri_linea_di_comando: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    err_parametri_linea_di_comando();

};


class errore_riga_vuota:public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    errore_riga_vuota();
};


class errore_incongruenza_file:public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    errore_incongruenza_file();
};


class err_check_anno_accademico:public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    err_check_anno_accademico();
};


class err_numero_versioni_parallele:public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    err_numero_versioni_parallele();
};


class err_corso_non_trovato_nel_cds: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    err_corso_non_trovato_nel_cds();
};


class err_corso_spento_non_trovato_nel_cds: public std::exception{
private:
    const char* messaggio;
public:
    const char* what() const noexcept;
    err_corso_spento_non_trovato_nel_cds();
};




#endif //PROGETTO_ALGORITMI_20_21_ECCEZIONI_H

#!/bin/bash

i=1
execute(){

  echo "$(tput setaf 1)$(tput bold)Esecuzione ${i} :$(tput sgr 0)" "$(tput setaf 3)$(tput bold)$1$(tput sgr 0)"

  i=$((i+1))

  ./Progetto_Algoritmi_20_21 $2

  sleep 1s
}

execute "aggiunta studenti ok" "-a:s lista_studenti.txt"
execute "aggiunta prof ok" "-a:d lista_professori.txt"
execute "aggiunta aule ok" "-a:a lista_aule.txt"
execute "aggiunta corsi ok" "-a:c lista_corsi.txt"
#execute "aggiunta cds ok" "-a:f lista_cds.txt"
execute "update studenti ok" "-u:s lista_studenti_update.txt"
execute "update prof ok" "-u:d lista_prof_update.txt"
execute "update aule ok" "-u:a lista_aule_update.txt"
#execute "inserimento ok" "-i:c lista_corsi_inserimento.txt"
execute "date sessioni ok" "-s current_a 2018-2019 14-01-2019|24-2-2019 10-06-2019|21-07-2019 2-09-2019|29-09-2019"
execute "date sessioni ok" "-s current_a 2019-2020 13-01-2020|23-2-2020 8-06-2020|19-07-2020 31-08-2020|27-09-2020"
execute "date sessioni sbagliate periodi lunghezza" "-s current_a 2019-2020 6-01-2020|16-2-2020 13-06-2022|31-07-2022 7-09-2020|4-10-2020"
execute "date sessioni sbagliate anno accademico" "-s current_a 2018-2019 10-01-2020|16-2-2020 15-06-2020|30-07-2020 4-09-2020|4-10-2020"
execute "date sessioni sbagliate periodi inversi" "-s current_a 2020-2021 10-01-2021|23-2-2020 15-06-2020|30-07-2020 6-09-2020|4-9-2020"
execute "indisponibilita ok" "-s set_availability 2020-2021 lista_indisponibilita.txt"


# altre date sessioni ok
# -s current_a 2019-2020 13-01-2020|23-2-2020 8-06-2020|19-07-2020 31-08-2020|27-09-2020
# -s current_a 2019-2020 6-01-2020|16-2-2020 15-06-2020|26-07-2020 7-09-2020|4-10-2020













#!/bin/bash

echo Esecuzione 1 : aggiunta studenti ok

./cmake-build-debug/Progetto_Algoritmi_20_21 -a:s lista_studenti.txt

sleep 3s

echo Esecuzione 2 : aggiunta prof ok

./cmake-build-debug/Progetto_Algoritmi_20_21 -a:d lista_prof.txt

sleep 3s

echo Esecuzione 3 : aggiunta aule ok

./cmake-build-debug/Progetto_Algoritmi_20_21 -a:a lista_aule.txt

sleep 3s

echo Esecuzione 4 : aggiunta corsi ok

./cmake-build-debug/Progetto_Algoritmi_20_21 -a:c lista_corsi.txt

sleep 3s

echo Esecuzione 5 : aggiunta cds ok

./cmake-build-debug/Progetto_Algoritmi_20_21 -a:f lista_cds.txt

sleep 3s

echo Esecuzione 6 : update studenti ok

./cmake-build-debug/Progetto_Algoritmi_20_21 -u:s lista_studenti_update.txt

sleep 3s

echo Esecuzione 7 : update prof ok

./cmake-build-debug/Progetto_Algoritmi_20_21 -u:d lista_prof_update.txt

sleep 3s

echo Esecuzione 8 : update aule ok

./cmake-build-debug/Progetto_Algoritmi_20_21 -u:a lista_aule_update.txt

sleep 3s

echo Esecuzione 9 : inserimento ok

./cmake-build-debug/Progetto_Algoritmi_20_21 -i:c lista_corsi_inserimento.txt

sleep 3s

echo Esecuzione 10 : date sessioni ok

./cmake-build-debug/Progetto_Algoritmi_20_21  -s current_a 2018-2019 14-01-2019|24-2-2019 10-06-2019|21-07-2019 2-09-2019|29-09-2019

sleep 3s

echo Esecuzione 11 : date sessioni ok

./cmake-build-debug/Progetto_Algoritmi_20_21  -s current_a 2019-2020 13-01-2020|23-2-2020 8-06-2020|19-07-2020 31-08-2020|27-09-2020

sleep 3s

echo Esecuzione 12 : date sessioni ok

./cmake-build-debug/Progetto_Algoritmi_20_21  -s current_a 2019-2020 6-01-2020|16-2-2020 15-06-2020|26-07-2020 7-09-2020|4-10-2020

sleep 3s

echo Esecuzione 13 : date sessioni sbagliate anno accademico

./cmake-build-debug/Progetto_Algoritmi_20_21  -s current_a 2019-2020 6-01-2020|16-2-2020 13-06-2022|31-07-2022 7-09-2020|4-10-2020

sleep 3s

echo Esecuzione 14 : date sessioni sbagliate

./cmake-build-debug/Progetto_Algoritmi_20_21  -s current_a 2018-2019 10-01-2020|16-2-2020 15-06-2020|30-07-2020 4-09-2020|4-10-2020

sleep 3s

echo Esecuzione 15 : date sessioni sbagliate

./cmake-build-debug/Progetto_Algoritmi_20_21  -s current_a 2020-2021 10-01-2021|23-2-2020 15-06-2020|30-07-2020 6-09-2020|4-9-2020

sleep 3s

echo Esecuzione 16 : indisponibilita ok

./cmake-build-debug/Progetto_Algoritmi_20_21  -s set_availability 2020-2021 indisponibilita1.txt













#!/bin/bash

# Assurer que le script d1 a les permissions d'exécution
chmod +x traitement/d1

# Définition de la fonction d1
d1() {
    
    # Compter le nombre de trajets pour chaque conducteur
     awk -F";" '/;1;/ {compteur[$6] += 1} END {for (nom in compteur) print compteur[nom], nom}' data/test1.csv | sort -nrk1,1 | head -10 > temp/temp_d1.csv

    cat temp/temp_d1.csv

    # Créer un fichier de données pour l'histogramme
     awk -v OFS=';' -F';' '{print $2, $1}' temp/temp_d1.csv > temp/histogram_data.csv

#Partie graphique (gnuplot).
        gnuplot << EOF

        # Paramètres de sortie
        set terminal pngcairo enhanced font 'Arial,10'
        set output 'images/graphique_d1.png'

        # Paramètres du graphique
        set bmargin 13    # Ajuster la marge inférieure (en unités par défaut)
        set rmargin 10
        set lmargin 2
        set title 'Top 10 des conducteurs avec le plus de trajets'
        set xlabel 'Noms des conducteurs'
        set ylabel 'Nombre de trajets'
        set xtic rotate by 90 offset 0,-9
        set xlabel rotate by 180 offset 0,-9
        set ylabel offset 92,0
        set ytic offset 83,0
        set yrange [0:250]
        set ytic rotate by 90
        set style histogram rowstacked
        set style fill solid border -1
        set boxwidth 0.5
        unset key

        # Tracé du graphique
        set datafile separator ';'
        plot 'temp/histogram_data.csv' using 2:xtic(1) with boxes lc rgb 'blue'

EOF

}

# Appel de la fonction d1Flag
d1

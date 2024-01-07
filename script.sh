#!/bin/bash

# Assurer que le script d1 a les permissions d'exécution
chmod +x traitement/d1

# Définition de la fonction d1
d1() {
    
    # Compter le nombre de trajets pour chaque conducteur
    awk -F';' '{ conducteurs[$6]++ } END { for (diver in conducteurs) print conducteurs[diver], diver }' data/test1.csv | sort -nr | head -n10 > temp/temp_d1.csv

    cat temp/temp_d1.csv

    # Créer un fichier de données pour l'histogramme
    # awk -v OFS=';' -F';' '{print $2, $1}' temp/temp_d1.csv > temp/histogram_data.csv

    # Générer l'histogramme horizontal avec gnuplot
    # gnuplot <<-EOF
    #     set terminal png size 1500,600
    #     set output 'images/histogram_d1.png'
    #     set title "Top 10 des conducteurs avec le plus de trajets"
    #     set xlabel "Nombre de trajets"
    #     set ylabel "Noms des conducteurs"
    #     set style data histograms
    #     set style fill solid border -1
    #     set yrange [0:300]
    #     set xrange [0:10] reverse
    #     set boxwidth 0.8
    #     plot 'temp/histogram_data.csv' using 1:xtic(2) notitle
    # EOF
}

# Appel de la fonction d1Flag
d1

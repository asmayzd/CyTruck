#!/bin/bash

# Assurer que le script d1 a les permissions d'exécution
chmod +x traitement/d1

# Définition de la fonction d1
d1() {
    # Compter le nombre de trajets pour chaque conducteur
    awk -F";" '/;1;/ {compteur[$6] += 1} END {for (nom in compteur) print compteur[nom], nom}' data/test1.csv | sort -nrk1,1 | head -10 > temp/temp_d1.csv

    cat temp/temp_d1.csv

    # Créer un fichier de données pour l'histogramme
    awk '{print $2 " " $3 "," $1}' temp/temp_d1.csv > temp/histogram_data.csv

    # Appel de la fonction pour tracer le graphique
    generate_plot
}

generate_plot() {
    # Vérification si le fichier existe
if [ ! -f "temp/histogram_data.csv" ]; then
    echo "Le fichier 'temp/histogram_data.csv' n'existe pas."
    exit 1
fi

	# Création du fichier de commandes pour Gnuplot
	cat << EOF > gnuplot_commands.txt
	set datafile separator ","
	set terminal pngcairo
	set output 'images/graphique_d1.png'
	#set style data histograms
	#set style fill solid
	#set boxwidth 0.5  # Ajuster la largeur des barres selon vos besoins
	#set xrange [ 0 : * ] reverse  # Inversion de l'axe y (anciennement x)
	#set offsets 0,0,0.5-0.5/2.,0.5
	#set yrange [ 0 : * ]  # Définir la plage pour l'axe x (anciennement y)
	#set xtics rotate by -90  # Fait pivoter les libellés de l'axe x de -90 degrés
	#set format y ""  # Pour enlever les libellés par défaut
	#unset key
	#set yrange [ * : * ] reverse  # Inversion de l'axe y (anciennement x)
	#plot 'temp/histogram_data.csv' using 2:ytic(1) with boxes notitle
#set yrange [*:*]      # start at zero, find max from the data
set style fill solid  # solid color boxes
unset key             # turn off all titles

set boxwidth 0.5
set offsets 0,0,0.5-0.5/2,0.5
set yrange [0:*]  # Utilisez la plage par défaut pour l'axe y
# Code couleur pour le violet en RGB (en pourcentage)
#violet = 0x993399  # Violet foncé

plot 'temp/histogram_data.csv' using (0.5*column(2)):0:(0.5*column(2)):(0.5/2.):(column(0)+1):ytic(1) with boxxy lc var
EOF

	# Exécution de Gnuplot avec les commandes du fichier
	gnuplot gnuplot_commands.txt

#mogrify -rotate "-90" images/graphique_d1.png


# Rotation de l'image pour avoir un histogramme horizontal
  #convert images/graphique_d1.png -rotate 90 images/graphique_d1_rotated.png
}
# Appel de la fonction d1Flag
d1

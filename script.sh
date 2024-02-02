#!/bin/bash

# Assurer que le script.sh a les permissions d'exécution
chmod 777 script.sh

check_dossiertemp() {
    if [ ! -d "temp" ]; then
        mkdir temp
    fi
}

check_dossierimages() {
    if [ ! -d "images" ]; then
        mkdir images
    fi
}

clean_temp() {
    rm -rf temp/*
}

clean_images() {
    rm -rf images/*
}

if [ ! -f "data/data.csv" ]; then
    echo "Erreur : Le fichier CSV 'data.csv' n'existe pas."
    exit 1
fi

if ! command -v gnuplot >/dev/null 2>&1; then
    echo "Erreur : Gnuplot n'est pas installé"
      # Mise à jour des dépôts
        sudo apt-get update
	# Installation de Gnuplot
        sudo apt-get install gnuplot
fi

if ! command -v convert >/dev/null 2>&1; then
    echo "Erreur : ImageMagick n'est pas installé"
    # Mise à jour des dépôts
    sudo apt-get update
    # Installation de ImageMagick
    sudo apt-get install imagemagick
fi

# Fonction pour vérifier l'installation du gnuplot
check_gnuplot() {
    if ! command -v gnuplot >/dev/null 2>&1; then
        echo "Erreur : l'installation de Gnuplot a échoué"
    fi
}


# Fonction pour vérifier l installation d'ImageMagick
check_imagemagick() {
    if ! command -v convert >/dev/null 2>&1; then
        echo "Erreur : l'installation de ImageMagick a échoué"
    fi
}


check_dossiertemp
check_dossierimages
clean_temp
clean_images

check_gnuplot
check_imagemagick

choix=""
while true; do
    echo "====== Bienvenue ! Choisissez une fonction : ======"
    echo "1. Fonction -d1"
    echo "2. Fonction -d2"
    echo "3. Fonction -l"
    echo "4. Fonction -t"
    echo "5. Fonction -s"
    echo "6. Aide"
    read -p "Entrez le numéro de la fonction que vous voulez exécuter (séparez les choix par des espaces) : " choix

    # Diviser les choix en un tableau
    choix_array=($choix)

    # Vérifier chaque choix
    choix_valid=true
    for c in "${choix_array[@]}"; do
        if ! [[ "$c" =~ ^[1-6]$ ]]; then
            choix_valid=false
            echo "Choix '$c' invalide. Veuillez entrer des numéros de fonction entre 1 et 6."
            break
        fi
    done

    if [ "$choix_valid" = true ]; then
        break
    fi
done


# Traiter chaque choix
for c in "${choix_array[@]}"; do
    case $c in
        1) chemin_script="traitement/d1" ;;
        2) chemin_script="traitement/d2" ;;
        3) chemin_script="traitement/l" ;;
        4)
            if [ ! -f "progc/t" ]; then
                gcc -o progc/t progc/t.c
                # Vérifier si la compilation s'est bien déroulée
                if [ $? -ne 0 ]; then
                    echo "Erreur de compilation"
                    exit 1
                fi
            fi
            chemin_script="traitement/t"
            ;;
        5)
            if [ ! -f "progc/s" ]; then
                gcc -o progc/s progc/s.c
                # Vérifier si la compilation s'est bien déroulée
                if [ $? -ne 0 ]; then
                    echo "Erreur de compilation"
                    exit 2
                fi
            fi
            chemin_script="traitement/s" ;;
        6) cat "help.txt" ;;
    esac


    # Exécuter le script correspondant au choix
    ./$chemin_script
done

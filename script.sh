#!/bin/bash

# Assurer que le script.sh a les permissions d'exécution
chmod 777 script.sh

choix=""
while ! [[ "$choix" =~ ^[1-5]$ ]]; do
    echo "====== Bienvenue ! Choisissez une fonction : ====="
    echo "1. Fonction d1"
    echo "2. Fonction d2"
    echo "3. Fonction l"
    echo "4. Fonction t"
    echo "5. Fonction s"
    read -p "Entrez le numéro de la fonction que vous voulez exécuter : " choix

if ! [[ "$choix" =~ ^[1-5]$ ]]; then
        echo "Choix invalide. Veuillez entrer un numéro de fonction entre 1 et 5."
    fi
done

case $choix in
    1) chemin_script="traitement/d1" ;;
    2) chemin_script="traitement/d2" ;;
    3) chemin_script="traitement/l" ;;
    4) chemin_script="traitement/t" ;;
    5) chemin_script="traitement/s" ;;
esac

if [ -f "$chemin_script" ]; then
    echo "Exécution du script $chemin_script..."
    # Message d'exécution du script correspondant
    sh "$chemin_script"  # Exécute le script 
else
    echo "Le script correspondant au choix n'existe pas."
    exit 1
fi

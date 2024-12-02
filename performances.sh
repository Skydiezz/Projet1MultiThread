#!/bin/bash

# Nom des programmes à tester (remplacez par les chemins réels)
PROGRAMS=("production")

# Nombre total de threads à tester
THREAD_COUNTS=(2 4 8 16 32)

# Nombre de répétitions pour chaque test
REPEATS=5

# Répertoire pour stocker les résultats
OUTPUT_DIR=""

# Désactiver les sorties STDOUT et STDERR
DEVNULL="/dev/null"

# Fonction pour exécuter un programme avec des paramètres
measure_time() {
    local program=$1
    local producers=$2
    local consumers=$3

    start=$(date +%s.%N) # Temps de début
    ./"$program" "$producers" "$consumers" > $DEVNULL 2>&1
    end=$(date +%s.%N)   # Temps de fin

    echo "$(echo "$end - $start" | bc)" # Calcul de la durée
}

# Parcourir chaque programme
for program in "${PROGRAMS[@]}"; do
    echo "Testing $program..."
    output_file="performance_results.csv"

    # En-tête du fichier CSV
    echo "Threads,Run1,Run2,Run3,Run4,Run5,Average" > "$output_file"

    # Tester chaque configuration de threads
    for total_threads in "${THREAD_COUNTS[@]}"; do
        producers=$((total_threads / 2))
        consumers=$((total_threads - producers))
        echo "  Config: $producers producers, $consumers consumers"

        # Mesurer les temps d'exécution
        times=()
        for ((i=1; i<=REPEATS; i++)); do
            time=$(measure_time "$program" "$producers" "$consumers")
            times+=("$time")
        done

        # Calculer la moyenne
        sum=0
        for time in "${times[@]}"; do
            sum=$(echo "$sum + $time" | bc)
        done
        average=$(echo "$sum / ${#times[@]}" | bc -l)

        # Ajouter les résultats au fichier CSV
        echo "$total_threads,${times[*]},$average" | sed 's/ /,/g' >> "$output_file"
    done
done

echo "Performance evaluation complete. Results saved in $output_file/"

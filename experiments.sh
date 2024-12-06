#!/bin/bash


PROGRAMS=("production" "philo" "lecture" "testi")


THREAD_COUNTS=(2 4 8 16 32)

REPEATS=5

OUTPUT_DIR="results"
mkdir -p "$OUTPUT_DIR"
DEVNULL="/dev/null"

measure_time() {
    local program=$1
    shift
    local args=("$@")


    local DEVNULL=${DEVNULL:-/dev/null}


    /usr/bin/time -f "%e" ./"$program" "${args[@]}" > "$DEVNULL" 2> /tmp/time_output.txt

    if [ -s /tmp/time_output.txt ]; then
        real_time=$(cat /tmp/time_output.txt)
    else
        echo "Problem with the execution of $program" >&2
        return 1
    fi
 
    echo "$real_time"
}



for program in "${PROGRAMS[@]}"; do
    echo "Testing $program..."
    output_file="$OUTPUT_DIR/${program}_results.csv"

    echo "Threads,Run1,Run2,Run3,Run4,Run5,Average" > "$output_file"

    # Tester chaque configuration de threads
    for total_threads in "${THREAD_COUNTS[@]}"; do

        if [[ "$program" == "production" || "$program" == "lecture" ]]; then
            producers=$((total_threads / 2))
            consumers=$((total_threads - producers))
            args=("$producers" "$consumers")
            echo "  Config: $producers type1, $consumers type2"
        else

            args=("$total_threads")
            echo "  Config: $total_threads threads (direct usage)"
        fi

        # Temps d'exécution
        times=()
        for ((i=1; i<=REPEATS; i++)); do
            time=$(measure_time "$program" "${args[@]}")
            times+=("$time")
        done

        # Moyenne
        sum=0
        for time in "${times[@]}"; do
            sum=$(echo "$sum + $time" | bc)
        done
        average=$(echo "$sum / ${#times[@]}" | bc -l)

        # Ajoute les résultats au fichier CSV
        echo "$total_threads,${times[*]},$average" | sed 's/ /,/g' >> "$output_file"
    done
done

echo "Performance evaluation complete. Results saved in $OUTPUT_DIR/"

import subprocess
import csv
import os
import time


program1 = "./production" 
program2 = "./philo"
thread_counts = [2, 4, 8, 16]


num_repeats = 5
output_csv = "performance_results.csv"



# Fonction pour exécuter une commande et mesurer son temps
def measure_time(producer_threads, consumer_threads):
    cmd = [program1, str(producer_threads), str(consumer_threads)]
    start = time.time()
    with open(os.devnull, 'w') as devnull:  # Rediriger la sortie vers /dev/null
        subprocess.run(cmd, stdout=devnull, stderr=devnull)
    return time.time() - start


results = []

for total_threads in thread_counts:
    producer_threads = total_threads // 2
    consumer_threads = total_threads - producer_threads

    times = []
    for _ in range(num_repeats):
        elapsed_time = measure_time(producer_threads, consumer_threads)
        times.append(elapsed_time)
    

    avg_time = sum(times) / len(times)
    results.append([total_threads] + times + [avg_time])

# Sauvegarder dans un fichier CSV
with open(output_csv, 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(["Threads", "Run1", "Run2", "Run3", "Run4", "Run5", "Average"])
    writer.writerows(results)

print(f"Performance results saved to {output_csv}")

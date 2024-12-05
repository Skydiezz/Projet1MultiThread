import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def show_data(data):
    thread_counts = data["Threads"]
    times = data.iloc[:, 1:-1].values  
    averages = data["Average"]

    # Calcul des écarts-types pour chaque configuration de threads
    std_devs = [np.std(times_row) for times_row in times]


    plt.figure(figsize=(10, 6))
    plt.errorbar(thread_counts, averages, yerr=std_devs, fmt='o-', capsize=5, label="Temps d'exécution moyen")
    plt.title("Temps d'exécution en fonction du nombre de threads", fontsize=14)
    plt.xlabel("Nombre de threads", fontsize=12)
    plt.ylabel("Temps d'exécution en secondes", fontsize=12)
    plt.ylim(0, None)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend(fontsize=12)


    plt.tight_layout()
    plt.show()

datas = ["lecture", "philo", "production", "testi"]

for d in datas:
    csv_file = "results/{d}_results.csv"  
    data = pd.read_csv(csv_file)
    show_data(data)

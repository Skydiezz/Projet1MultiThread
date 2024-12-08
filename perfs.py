import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def show_data(data, name):
    thread_counts = data["Threads"]
    times = data.iloc[:, 1:-1].values  
    averages = data["Average"]

    # Calcul des écarts-types pour chaque configuration de threads
    std_devs = [np.std(times_row) for times_row in times]


    plt.figure(figsize=(10, 6))
    plt.errorbar(thread_counts, averages, yerr=std_devs, fmt='o-', capsize=5, label="Temps d'exécution moyen")
    plt.title(f"Temps d'exécution en fonction du nombre de threads pour {name}", fontsize=14)
    plt.xlabel("Nombre de threads", fontsize=12)
    plt.ylabel("Temps d'exécution en secondes", fontsize=12)
    plt.ylim(0, None)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend(fontsize=12)


    plt.tight_layout()
    plt.show()

def show_test(data1, data2):
    thread_counts1 = data1["Threads"]
    thread_counts2 = data2["Threads"]
    times1 = data1.iloc[:, 1:-1].values
    times2 = data2.iloc[:, 1:-1].values  
    averages1 = data1["Average"]
    averages2 = data2["Average"]

    std_devs1 = [np.std(times_row) for times_row in times1]
    std_devs2 = [np.std(times_row) for times_row in times2]


    plt.figure(figsize=(10, 6))
    plt.errorbar(thread_counts1, averages1, yerr=std_devs1, fmt='o-', capsize=5, label="Temps d'exécution test-and-set")
    plt.errorbar(thread_counts2, averages2, yerr=std_devs1, fmt='o-', capsize=5, label="Temps d'exécution test-and-test-and-set")

    plt.title(f"Temps d'exécution en fonction du nombre de threads pour test-and-set vs test-and-test-and-set", fontsize=14)
    plt.xlabel("Nombre de threads", fontsize=12)
    plt.ylabel("Temps d'exécution en secondes", fontsize=12)
    plt.ylim(0, None)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend(fontsize=12)

    plt.tight_layout()
    plt.show()
    

datas = ["lecture", "lectureV2", "philo", "philoV2", "production", "productionV2", "test-and-set"]

for d in datas:
    csv_file = f"results\{d}_results.csv"
    if d == "test-and-set":
        name = "test-and-test-and-set"
        data1 = pd.read_csv(csv_file)
        data2 = pd.read_csv(f"results\{name}_results.csv")
        show_test(data1, data2)
    else:
        data = pd.read_csv(csv_file)
        show_data(data, d)



#!/bin/bash
chmod +x performance.sh
./performance.sh
python3 plots.py
echo "Philo"
cat results/philo_results.csv
echo "PhiloV2"
cat results/philoV2_results.csv
echo "Prod"
cat results/production_results.csv
echo "productionV2"
cat results/productionV2_results.csv

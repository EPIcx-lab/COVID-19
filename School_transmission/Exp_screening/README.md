This repository contains the data and code corresponding to the Omicron wave simulations from:

Colosi, E., Lina, B., Elias, C., Vanhems, P.,  Colizza, V. 
Proactive vs. reactive COVID-19 screening in schools: Lessons from experimental protocols in France during the Delta and Omicron waves.
MedRxiv, 2025 
doi: https://doi.org/10.1101/2025.01.17.25320676

Please cite the above reference if you use any part of this material.

Overview

This repository includes:

1. C code modeling SARS-CoV-2 school transmission during the experimental screening under the Omicron wave (Jan–Feb 2022).
2. Python notebooks for post-processing and plotting the simulation outputs.





1. C Code: Transmission Model (Omicron)

Description
The C code models school transmission under the screening protocol implemented in selected French primary schools of the Auvergne-Rhône-Alpes region during the Omicron wave.

Use the following command to compile the model from your own terminal (ensure GSL library is installed):
gcc primary_epidemics.c fun.c -I/opt/homebrew/opt/gsl/include -L/opt/homebrew/opt/gsl/lib -lgsl -lgslcblas -lm -o primary_epidemics

Use the following command to run the model from your own terminal:
./primary_epidemics 1 0.0153 15 3 1 3 8 1 1 0 1 0 0.35 0.55 0.3 0.5 1440 7 1 1 0.6

Or use the helper script: python launcher.py

2. Python Notebooks: Analysis and Plots

Description
Outputs from simulations are stored in the data_out/ directory. Use the provided Jupyter notebooks for post-processing and visualization. Each notebook includes a list of required packages at the top.

Note
Similar code and scripts were used to simulate the Delta wave (Nov–Dec 2021), with variant-specific epi parameters and nationwide protocols adjusted accordingly. This version is provided as an example.
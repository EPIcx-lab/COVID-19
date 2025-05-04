This repository contains the data and code corresponding to the Omicron wave simulations from:

Colosi, E., Lina, B., Elias, C., Vanhems, P.,  Colizza, V. 
Proactive vs. reactive COVID-19 screening in schools: Lessons from experimental protocols in France during the Delta and Omicron waves.
MedRxiv, 2025 
doi: https://doi.org/10.1101/2025.01.17.25320676

Please cite the above reference if you use any part of this material.

Overview

This repository includes:
1. C code for simulating SARS-CoV-2 transmission under different school strategies.
2. Python Jupyter notebooks for post-processing and figures.






C Code: Transmission Model (Omicron)

Description
The C code simulates SARS-CoV-2 transmission if reactive screening protocols were applied in the schools following the experimental screening during the Omicron wave, assuming the same underlying transmissibility conditions.


Or use the helper script to compile and run the code: python launcher.py


Python Notebooks: Analysis and Plots

Description
Outputs from simulations are stored in the data_out/ directory. Use the provided Jupyter notebooks for analyses on avereted school transmission and visualization. 

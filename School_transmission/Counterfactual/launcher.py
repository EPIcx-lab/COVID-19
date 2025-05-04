import subprocess
import sys
import numpy as np
from multiprocessing import Pool, cpu_count

terminal_input="gcc -g primary_epidemics.c fun.c -o primary_epidemics -lgsl -lgslcblas -lm";
proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
(out, err) = proc.communicate()

num_run = 2000

perc_immunity_stud = 0.35
perc_immunity_teac = 0.55

#tURNAROUNDTIME&iSOLATION
turn_time_min = 1440
days_isolation = 7


p_detec_stud = 0.3
p_detec_teac = 0.5

sigma_omicron=0.6

betas_values=[0.0153]
protocols = [[1, 0, 0, 0, 1]]
alpha  = [1]
deltar = 0
delay=2


test_repetition = [[1, 1]]

initial_seeds= 15 #12
initial_prodromic= 3 #3
initial_clinical= 1 #3
initial_subclinical= 3 #6
initial_rp= 8


str_param = "%d_b%.4f_%d_%d_%d_%d_%d%d%d%d%d%d_immu%.2f_%.2f_Pd%.2f_%.2f_%d_%d_f%d_%d_%f_%f_%f_s%f"
str_launch = "./primary_epidemics %d %f %d %d %d %d %d %d %d %d %d %d %f %f %f %f %d %d %d %d %f %f %f %f"




for pr in protocols:
	symptomatic = pr[0]
	preventive = pr[1]
	preventivegrade = pr[2]
	regular = pr[3]
	regularclasses = 0
	reactive = pr[4]

	for beta in betas_values:
		if (regular == 1) :
			
			for t in test_repetition:
				ntimes = t[0]
				nweek = t[1]

				name = str_param%(num_run, beta, initial_seeds, initial_prodromic, initial_clinical, initial_subclinical, initial_rp, symptomatic, preventive, preventivegrade, regular, reactive, perc_immunity_stud, perc_immunity_teac, p_detec_stud, p_detec_teac, turn_time_min, days_isolation, ntimes, nweek, sigma_omicron)

				f = open("cluster/call_%s.sh"%name,"w")
				f.write("#!/bin/bash -l\n")
				f.write("#SBATCH -J %s\n"%name)
				f.write("#SBATCH -t 48:0:0\n\n")
				# f.write("cd symp-R%d\n"%R)
				f.write(str_launch%(num_run, beta, initial_seeds, initial_prodromic, initial_clinical, initial_subclinical, initial_rp, symptomatic, preventive, preventivegrade, regular, reactive, perc_immunity_stud, perc_immunity_teac, p_detec_stud, p_detec_teac, turn_time_min, days_isolation, ntimes, nweek, sigma_omicron))
				f.close()


				terminal_input="sbatch cluster/call_%s.sh"%name;
				proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
				(out, err) = proc.communicate()
	        elif ((regular == 1) & (regularclasses == 1)) :
	        
			perc_adhesion = 0
			
			for t in test_repetition:
				ntimes = t[0]
				nweek = t[1]

				name = str_param%(num_run, beta, initial_seeds, initial_prodromic, initial_clinical, initial_subclinical, initial_rp, symptomatic, preventive, preventivegrade, regular, perc_immunity_stud, perc_immunity_teac, perc_vaccinated_stud, perc_vaccinated_teac, eff_transm_stud, eff_transm_teac, eff_inf_stud, eff_inf_teac, eff_symp_stud, eff_symp_teac, r_beta_stud_p, r_beta_teac_p, r_beta_stud_c, r_beta_teac_c, sigma_stud, sigma_teac, p_subcl_stud, p_subcl_teac, p_detec_stud, p_detec_teac, turn_time_min, days_isolation, perc_adhesion, ntimes, nweek)

				
				f = open("cluster/call_%s.sh"%name,"w")
				f.write("#!/bin/bash -l\n")
				f.write("#SBATCH -J %s\n"%name)
				f.write("#SBATCH -t 48:0:0\n\n")
				# f.write("cd symp-R%d\n"%R)
				f.write(str_launch%(num_run, beta, initial_seeds, initial_prodromic, initial_clinical, initial_subclinical, initial_rp, symptomatic, preventive, preventivegrade, regular, perc_immunity_stud, perc_immunity_teac, perc_vaccinated_stud, perc_vaccinated_teac, eff_transm_stud, eff_transm_teac, eff_inf_stud, eff_inf_teac, eff_symp_stud, eff_symp_teac, r_beta_stud_p, r_beta_teac_p, r_beta_stud_c, r_beta_teac_c, sigma_stud, sigma_teac, p_subcl_stud, p_subcl_teac, p_detec_stud, p_detec_teac, turn_time_min, days_isolation, perc_adhesion, ntimes, nweek))
				f.close()


				terminal_input="sbatch cluster/call_%s.sh"%name;
				proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
				(out, err) = proc.communicate()

		
		elif (( symptomatic == 1) & (regular == 1) & (preventive == 1)) :
		
			
			perc_regularclasses=0
			perc_adhesionclasses=0
			
			
			for a in alpha:
				for t in test_repetition:
					ntimes = t[0]
					nweek = t[1]
			
					name = str_param%(num_run, beta, initial_seeds, initial_prodromic, initial_clinical, initial_subclinical, initial_rp, symptomatic, preventive, preventivegrade, regular, reactive, perc_immunity_stud, perc_immunity_teac, perc_vaccinated_stud_2doses, perc_vaccinated_teac_2doses, 
perc_vaccinated_stud_booster, perc_vaccinated_teac_booster, 
eff_transm_stud_2doses, eff_transm_teac_2doses, eff_inf_stud_2doses, eff_inf_teac_2doses, eff_symp_stud_2doses, eff_symp_teac_2doses, 
eff_transm_stud_booster, eff_transm_teac_booster, eff_inf_stud_booster, eff_inf_teac_booster, eff_symp_stud_booster, eff_symp_teac_booster, 
r_beta_stud_p, r_beta_teac_p, r_beta_stud_c, r_beta_teac_c, sigma_stud, sigma_teac, p_subcl_stud, p_subcl_teac, p_detec_stud, p_detec_teac, turn_time_min, days_isolation, ntimes, nweek,  a, deltar, delay, sigma_omicron)
					f = open("cluster/call_%s.sh"%name,"w")
					f.write("#!/bin/bash -l\n")
					f.write("#SBATCH -J %s\n"%name)
					f.write("#SBATCH -t 48:0:0\n\n")
					# f.write("cd symp-R%d\n"%R)
					f.write(str_launch%(num_run, beta, initial_seeds, initial_prodromic, initial_clinical, initial_subclinical, initial_rp, symptomatic, preventive, preventivegrade, regular, reactive, perc_immunity_stud, perc_immunity_teac, perc_vaccinated_stud_2doses, perc_vaccinated_teac_2doses, 
perc_vaccinated_stud_booster, perc_vaccinated_teac_booster, 
eff_transm_stud_2doses, eff_transm_teac_2doses, eff_inf_stud_2doses, eff_inf_teac_2doses, eff_symp_stud_2doses, eff_symp_teac_2doses, 
eff_transm_stud_booster, eff_transm_teac_booster, eff_inf_stud_booster, eff_inf_teac_booster, eff_symp_stud_booster, eff_symp_teac_booster, 
r_beta_stud_p, r_beta_teac_p, r_beta_stud_c, r_beta_teac_c, sigma_stud, sigma_teac, p_subcl_stud, p_subcl_teac, p_detec_stud, p_detec_teac, turn_time_min, days_isolation, ntimes, nweek,  a, deltar, delay, sigma_omicron))
					f.close()


					terminal_input="sbatch cluster/call_%s.sh"%name;
					proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
					(out, err) = proc.communicate()
		
		else:
			perc_adhesion = 0
			perc_regularclasses=0
			perc_adhesionclasses=0
			ntimes = 0
			nweek = 0

			name = str_param%(num_run, beta, initial_seeds, initial_prodromic, initial_clinical, initial_subclinical, initial_rp, symptomatic, preventive, preventivegrade, regular, reactive, perc_immunity_stud, perc_immunity_teac, p_detec_stud, p_detec_teac, turn_time_min, days_isolation, ntimes, nweek, alpha[0], deltar, delay, sigma_omicron)

			f = open("cluster/call_%s.sh"%name,"w")
			f.write("#!/bin/bash -l\n")
			f.write("#SBATCH -J %s\n"%name)
			f.write("#SBATCH -t 48:0:0\n\n")
			# f.write("cd symp-R%d\n"%R)
			f.write(str_launch%(num_run, beta, initial_seeds, initial_prodromic, initial_clinical, initial_subclinical, initial_rp, symptomatic, preventive, preventivegrade, regular, reactive, perc_immunity_stud, perc_immunity_teac, p_detec_stud, p_detec_teac, turn_time_min, days_isolation, ntimes, nweek, alpha[0], deltar, delay, sigma_omicron))
						
			f.close()


			terminal_input="sbatch cluster/call_%s.sh"%name;
			proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
			(out, err) = proc.communicate()


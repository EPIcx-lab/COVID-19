#### list of scenarios

#### before running, adapt the path for the input files in the pyhton scripts for the scenarios, following the path used in the pyhton script for the fit (see 1_fit_transmission_model.ipynb)

#### output of the scenarios simulated are stored in the folder "scenarios" and can be analysed in with the script 3_analysis_scenarios.ipynb to reproduce the figures of the manuscript

import subprocess
import sys
import numpy as np
from multiprocessing import Pool, cpu_count

str_param = "%s_%d_%d_%d_%d_%f_%s"

######
#100 runs

######### MAIN

str_launch = "srun python R-delta-waned_delta-vs-BA1BA2_reinfection-Omi-Omi_addSD_wX_duratY.py %s %d %d %d %d %f %s"

n_runs = [10,100]
ritmi = ['adu_ritmo3']
ritardi = [7]
starting_weeks = {4:[1],5:[1],6:[1],7:[1],8:[1],9:[1]}## 1 or 10
durate = [4]  ## number of weeks of duration
riduzioni = [0.]
coverage = ['full'] ## full, limited

for ritmo in ritmi:
    for r in n_runs:
        for durata in durate:
            for cov in coverage:
                    for ritardo in ritardi:
                        name = str_param%(ritmo, r, ritardo, 0, durata, 0, cov)

                        f = open("bashes/call_%s.sh"%name,"w")
                        f.write("#!/bin/bash -l\n")
                        f.write("#SBATCH -J %s\n"%name)
                        f.write("#SBATCH -t 70:0:0\n\n")
                
                        for SD_reduction in riduzioni:
                            for start in starting_weeks[ritardo]:

                                f.write(str_launch%(ritmo, r, ritardo, start, durata, SD_reduction, cov))
                                f.write('\n')
                        f.close()

                        terminal_input="sbatch bashes/call_%s.sh"%name;
                        proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
                        (out, err) = proc.communicate()
                        
                        
str_launch = "srun python R-delta-waned_delta-vs-BA1BA2_reinfection-Omi-Omi_addSD_wX_duratY.py %s %d %d %d %d %f %s"
                        
n_runs = [100]
ritmi = ['adu_ritmo2_start20210901_buffer30days']
ritardi = [4,5,6,7,8,9]
starting_weeks = {4:[8,5,6,7,9,10],5:[8,5,6,7,9,10],6:[1,-3,-2,-1,0,2,3],7:[1,-3,-2,-1,0,2],8:[1,-3,-2,-1,0,2],9:[1,-3,-2,-1,0,2]}## 1 or 10
durate = [4]  ## number of weeks of duration
riduzioni = [0.1, 0.2, 0.3, 0.4, 0.5,
            0.05, 0.15, 0.25, 0.35, 0.45, 0.55]
coverage = ['limited'] ## full, limited

for ritmo in ritmi:
    for r in n_runs:
        for durata in durate:
            for cov in coverage:
                    for ritardo in ritardi:
                        name = str_param%(ritmo, r, ritardo, 0, durata, 0, cov)

                        f = open("bashes/call_%s.sh"%name,"w")
                        f.write("#!/bin/bash -l\n")
                        f.write("#SBATCH -J %s\n"%name)
                        f.write("#SBATCH -t 70:0:0\n\n")
                
                        for SD_reduction in riduzioni:
                            for start in starting_weeks[ritardo]:

                                f.write(str_launch%(ritmo, r, ritardo, start, durata, SD_reduction, cov))
                                f.write('\n')
                        f.close()

                        terminal_input="sbatch bashes/call_%s.sh"%name;
                        proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
                        (out, err) = proc.communicate()
                    
str_launch = "srun python R-delta-waned_delta-vs-BA1BA2_reinfection-Omi-Omi_addSD_wX_duratY.py %s %d %d %d %d %f %s"

n_runs = [100]
ritmi = ['adu_ritmo2_start20210801_buffer30days',
            'sen_ritmo2_start20210801_buffer30days',
            'adu_sen_ritmo2_start20210801_buffer30days',
            'adu_ritmo2_start20210901_buffer30days',
            'sen_ritmo2_start20210901_buffer30days',
            'adu_sen_ritmo2_start20210901_buffer30days']
ritardi = [4,5,6,7,8,9]
starting_weeks = {4:[1],5:[1],6:[1],7:[1],8:[1],9:[1]}## 1 or 10

durate = [4]  ## number of weeks of duration
riduzioni = [0.0]
coverage = ['limited', 'full'] ## full, limited

for ritmo in ritmi:
    for r in n_runs:
        for durata in durate:
            for cov in coverage:
                name = str_param%(ritmo, r, ritardo, 0, durata, 0, cov)

                f = open("bashes/call_%s.sh"%name,"w")
                f.write("#!/bin/bash -l\n")
                f.write("#SBATCH -J %s\n"%name)
                f.write("#SBATCH -t 70:0:0\n\n")
        
                for SD_reduction in riduzioni:
                    for ritardo in ritardi:
                        for start in starting_weeks[ritardo]:
                            f.write(str_launch%(ritmo, r, ritardo, start, durata, SD_reduction, cov))
                            f.write('\n')
                
                f.close()


                terminal_input="sbatch bashes/call_%s.sh"%name;
                proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
                (out, err) = proc.communicate()


str_launch = "srun python R-delta-waned_delta-vs-BA1BA2_reinfection-Omi-Omi_accelerate_campaign_1m_buf60d.py %s %d %d %d %d %f %s %d"

n_runs = [100]
ritmi = ['adu_ritmo2_start20210901_buffer30days']
ritardi = [8]
#ritardi = [6,7,8]
starting_weeks = [1] ## 1 or 10
durate = [4]  ## number of weeks of duration
riduzioni = [0.0]
coverage = ['limited'] ## full, limited
week_accel = [45, 46, 47, 48, 49, 50, 51, 52, 53]

for ritmo in ritmi:
    for r in n_runs:
        for ritardo in ritardi:
            for start in starting_weeks:
                for durata in durate:
                    for SD_reduction in riduzioni:
                        for cov in coverage:
                            
                            name = '1m_{}delay'.format(ritardo)

                            f = open("bashes/call_%s.sh"%name,"w")
                            f.write("#!/bin/bash -l\n")
                            f.write("#SBATCH -J %s\n"%name)
                            f.write("#SBATCH -t 48:0:0\n\n")
                            
                            for ww in week_accel:
                            # f.write("cd symp-R%d\n"%R)
                                f.write(str_launch%(ritmo, r, ritardo, start, durata, SD_reduction, cov, ww))
                                f.write('\n')
                                
                            f.close()

                            terminal_input="sbatch bashes/call_%s.sh"%name;
                            proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
                            (out, err) = proc.communicate()


str_launch = "srun python R-delta-waned_delta-vs-BA1BA2_reinfection-Omi-Omi_accelerate_campaign_2m_buf60d.py %s %d %d %d %d %f %s %d"

n_runs = [100]
ritmi = ['adu_ritmo2_start20210901_buffer30days']
ritardi = [9]
#ritardi = [7,8]
starting_weeks = [1] ## 1 or 10
durate = [4]  ## number of weeks of duration
riduzioni = [0.0]
coverage = ['limited'] ## full, limited
week_accel = [45, 46, 47, 48, 49, 50, 51, 52, 53]

for ritmo in ritmi:
    for r in n_runs:
        for ritardo in ritardi:
            for start in starting_weeks:
                for durata in durate:
                    for SD_reduction in riduzioni:
                        for cov in coverage:
                            
                            name = '2m_{}delay'.format(ritardo)

                            f = open("bashes/call_%s.sh"%name,"w")
                            f.write("#!/bin/bash -l\n")
                            f.write("#SBATCH -J %s\n"%name)
                            f.write("#SBATCH -t 48:0:0\n\n")
                            
                            for ww in week_accel:
                            # f.write("cd symp-R%d\n"%R)
                                f.write(str_launch%(ritmo, r, ritardo, start, durata, SD_reduction, cov, ww))
                                f.write('\n')
                                
                            f.close()

                            terminal_input="sbatch bashes/call_%s.sh"%name;
                            proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
                            (out, err) = proc.communicate()

str_launch = "srun python R-delta-waned_delta-vs-BA1BA2_reinfection-Omi-Omi_accelerate_campaign_3m_buf60d.py %s %d %d %d %d %f %s %d"

n_runs = [100]
ritmi = ['adu_ritmo2_start20210901_buffer30days']
ritardi = [9]
#ritardi = [8]
starting_weeks = [1] ## 1 or 10
durate = [4]  ## number of weeks of duration
riduzioni = [0.0]
coverage = ['limited'] ## full, limited
week_accel = [45, 46, 47, 48, 49, 50, 51, 52, 53]

for ritmo in ritmi:
    for r in n_runs:
        for ritardo in ritardi:
            for start in starting_weeks:
                for durata in durate:
                    for SD_reduction in riduzioni:
                        for cov in coverage:
                            
                            name = '3m_{}delay'.format(ritardo)

                            f = open("bashes/call_%s.sh"%name,"w")
                            f.write("#!/bin/bash -l\n")
                            f.write("#SBATCH -J %s\n"%name)
                            f.write("#SBATCH -t 48:0:0\n\n")
                            
                            for ww in week_accel:
                            # f.write("cd symp-R%d\n"%R)
                                f.write(str_launch%(ritmo, r, ritardo, start, durata, SD_reduction, cov, ww))
                                f.write('\n')
                                
                            f.close()

                            terminal_input="sbatch bashes/call_%s.sh"%name;
                            proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
                            (out, err) = proc.communicate()
                        

### 2 nodi per 27 ore
str_launch = "srun python R-delta-waned_delta-vs-BA1BA2_reinfection-Omi-Omi_addSD_wX_duratY.py %s %d %d %d %d %f %s"

n_runs = [100]
ritmi = ['adu_ritmo2_start20210901_buffer30days']
ritardi = [7,8]
starting_weeks = {7:[-4, -5, -6, -7],
                  8:[-4, -5, -6, -7]}## 1 or 10
durate = [4]  ## number of weeks of duration
riduzioni = [0.1, 0.2, 0.3, 0.4,
             0.05, 0.15, 0.25, 0.35, 0.45]
coverage = ['limited'] ## full, limited

for ritmo in ritmi:
    for r in n_runs:
        for durata in durate:
            for cov in coverage:
                    for ritardo in ritardi:
                        name = str_param%(ritmo, r, ritardo, 0, durata, 0, cov)

                        f = open("bashes/call_%s.sh"%name,"w")
                        f.write("#!/bin/bash -l\n")
                        f.write("#SBATCH -J %s\n"%name)
                        f.write("#SBATCH -t 70:0:0\n\n")
                
                        for SD_reduction in riduzioni:
                            for start in starting_weeks[ritardo]:

                                f.write(str_launch%(ritmo, r, ritardo, start, durata, SD_reduction, cov))
                                f.write('\n')
                        f.close()

                        terminal_input="sbatch bashes/call_%s.sh"%name;
                        proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
                        (out, err) = proc.communicate()

str_launch = "srun python R-delta-waned_delta-vs-BA1BA2_reinfection-Omi-Omi_addSD_wX_duratY.py %s %d %d %d %d %f %s"

## 8 nodi per 11x45= 8h30 ore
n_runs = [100]
ritmi = ['adu_ritmo2_start20210901_buffer30days']
ritardi = [7,8]
starting_weeks = {7:[3,4,5,6], 8:[3,4,5,6]} ## 1 or 10
durate = [4]  ## number of weeks of duration
riduzioni = [0.1, 0.2, 0.3, 0.4,
             0.05, 0.15, 0.25, 0.35, 0.45]
coverage = ['limited'] ## full, limited

for ritmo in ritmi:
    for r in n_runs:
        for durata in durate:
            for cov in coverage:
                    for ritardo in ritardi:
                        for start in starting_weeks[ritardo]:
                            name = str_param%(ritmo, r, ritardo, 0, durata, 0, cov)

                            f = open("bashes/call_%s.sh"%name,"w")
                            f.write("#!/bin/bash -l\n")
                            f.write("#SBATCH -J %s\n"%name)
                            f.write("#SBATCH -t 70:0:0\n\n")
                    
                            for SD_reduction in riduzioni:
                                f.write(str_launch%(ritmo, r, ritardo, start, durata, SD_reduction, cov))
                                f.write('\n')
                            f.close()

                            terminal_input="sbatch bashes/call_%s.sh"%name;
                            proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
                            (out, err) = proc.communicate()
                            
### figura 4 f (poco interessante perch si stratta di quella con additional SD con 6 mesi
### 1 nodo per 90 ore

str_launch = "srun python R-delta-waned_delta-vs-BA1BA2_reinfection-Omi-Omi_addSD_wX_duratY.py %s %d %d %d %d %f %s"

n_runs = [100]
ritmi = ['adu_ritmo2_start20210901_buffer30days']
ritardi = [6]
starting_weeks = {6:[3,2,1,0,-1,-2,-3,-4,-5,-6,-7]} ## 1 or 10
durate = [4]  ## number of weeks of duration
riduzioni = [0.1, 0.2, 0.3, 0.4,
             0.05, 0.15, 0.25, 0.35, 0.45]
coverage = ['limited'] ## full, limited

for ritmo in ritmi:
    for r in n_runs:
        for durata in durate:
            for cov in coverage:
                    for ritardo in ritardi:
                        name = str_param%(ritmo, r, ritardo, 0, durata, 0, cov)

                        f = open("bashes/call_%s.sh"%name,"w")
                        f.write("#!/bin/bash -l\n")
                        f.write("#SBATCH -J %s\n"%name)
                        f.write("#SBATCH -t 70:0:0\n\n")
                
                        for SD_reduction in riduzioni:
                            for start in starting_weeks[ritardo]:

                                f.write(str_launch%(ritmo, r, ritardo, start, durata, SD_reduction, cov))
                                f.write('\n')
                        f.close()

                        terminal_input="sbatch bashes/call_%s.sh"%name;
                        proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
                        (out, err) = proc.communicate()
                        

## 2 nodi per 7 ore
str_launch = "srun python R-delta-waned_delta-vs-BA1BA2_reinfection-Omi-Omi_addSD_wX_duratY.py %s %d %d %d %d %f %s"
                        
n_runs = [100]
ritmi = ['adu_ritmo2_start20210901_buffer30days']
ritardi = [5,6]
starting_weeks = {4:[],5:[4],6:[4],7:[],8:[],9:[]}## 1 or 10
durate = [4]  ## number of weeks of duration
riduzioni = [0.1, 0.2, 0.3, 0.4, 0.5,
             0.05, 0.15, 0.25, 0.35, 0.45]
coverage = ['limited'] ## full, limited

for ritmo in ritmi:
    for r in n_runs:
        for durata in durate:
            for cov in coverage:
                    for ritardo in ritardi:
                        name = str_param%(ritmo, r, ritardo, 0, durata, 0, cov)

                        f = open("bashes/call_%s.sh"%name,"w")
                        f.write("#!/bin/bash -l\n")
                        f.write("#SBATCH -J %s\n"%name)
                        f.write("#SBATCH -t 70:0:0\n\n")
                
                        for start in starting_weeks[ritardo]:
                            for SD_reduction in riduzioni:
                                f.write(str_launch%(ritmo, r, ritardo, start, durata, SD_reduction, cov))
                                f.write('\n')
                        f.close()

                        terminal_input="sbatch bashes/call_%s.sh"%name;
                        proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
                        (out, err) = proc.communicate()
  
### 1 nodo per 4 ore
str_launch = "srun python R-delta-waned_delta-vs-BA1BA2_reinfection-Omi-Omi_addSD_wX_duratY.py %s %d %d %d %d %f %s"
                        
n_runs = [100]
ritmi = ['adu_ritmo2_start20210901_buffer30days']
ritardi = [6]
starting_weeks = {4:[],5:[3],6:[4],7:[],8:[],9:[]}## 1 or 10
durate = [4]  ## number of weeks of duration
riduzioni = [0.05, 0.15, 0.25, 0.35, 0.45]
coverage = ['limited'] ## full, limited

for ritmo in ritmi:
    for r in n_runs:
        for durata in durate:
            for cov in coverage:
                    for ritardo in ritardi:
                        name = str_param%(ritmo, r, ritardo, 0, durata, 0, cov)

                        f = open("bashes/call_%s.sh"%name,"w")
                        f.write("#!/bin/bash -l\n")
                        f.write("#SBATCH -J %s\n"%name)
                        f.write("#SBATCH -t 70:0:0\n\n")
                
                        for start in starting_weeks[ritardo]:
                            for SD_reduction in riduzioni:
                                f.write(str_launch%(ritmo, r, ritardo, start, durata, SD_reduction, cov))
                                f.write('\n')
                        f.close()

                        terminal_input="sbatch bashes/call_%s.sh"%name;
                        proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
                        (out, err) = proc.communicate()


#### second main: fig 2 pannello e (sufficient e most effective SD for each delay); to test for
### adu_sen limited
### adu full
### adu_sen full
str_launch = "srun python R-delta-waned_delta-vs-BA1BA2_reinfection-Omi-Omi_addSD_wX_duratY.py %s %d %d %d %d %f %s"

n_runs = [100]
ritmi = ['adu_sen_ritmo2_start20210901_buffer30days']
ritardi = [4,5,6,7,8,9]
starting_weeks = {4:[8],5:[8],6:[1],7:[1],8:[1],9:[1]}## 1 or 10
durate = [4]  ## number of weeks of duration
riduzioni = [0.1, 0.2, 0.3, 0.4, 0.5,
             0.05, 0.15, 0.25, 0.35, 0.45]
coverage = ['limited'] ## full, limited

for ritmo in ritmi:
    for r in n_runs:
        for durata in durate:
            for cov in coverage:
                    for ritardo in ritardi:
                        name = str_param%(ritmo, r, ritardo, 0, durata, 0, cov)

                        f = open("bashes/call_%s.sh"%name,"w")
                        f.write("#!/bin/bash -l\n")
                        f.write("#SBATCH -J %s\n"%name)
                        f.write("#SBATCH -t 70:0:0\n\n")
                
                        for SD_reduction in riduzioni:
                            for start in starting_weeks[ritardo]:

                                f.write(str_launch%(ritmo, r, ritardo, start, durata, SD_reduction, cov))
                                f.write('\n')
                        f.close()

                        terminal_input="sbatch bashes/call_%s.sh"%name;
                        proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
                        (out, err) = proc.communicate()
           
str_launch = "srun python R-delta-waned_delta-vs-BA1BA2_reinfection-Omi-Omi_addSD_wX_duratY.py %s %d %d %d %d %f %s"

n_runs = [100]
ritmi = ['adu_ritmo2_start20210901_buffer30days',
        'adu_sen_ritmo2_start20210901_buffer30days']
ritardi = [4,5,6,7,8,9]
starting_weeks = {4:[8],5:[8],6:[1],7:[1],8:[1],9:[1]}## 1 or 10
durate = [4]  ## number of weeks of duration
riduzioni = [0.1, 0.2, 0.3, 0.4, 0.5,
             0.05, 0.15, 0.25, 0.35, 0.45]
coverage = ['full'] ## full, limited

for ritmo in ritmi:
    for r in n_runs:
        for durata in durate:
            for cov in coverage:
                    for ritardo in ritardi:
                        name = str_param%(ritmo, r, ritardo, 0, durata, 0, cov)

                        f = open("bashes/call_%s.sh"%name,"w")
                        f.write("#!/bin/bash -l\n")
                        f.write("#SBATCH -J %s\n"%name)
                        f.write("#SBATCH -t 70:0:0\n\n")
                
                        for SD_reduction in riduzioni:
                            for start in starting_weeks[ritardo]:

                                f.write(str_launch%(ritmo, r, ritardo, start, durata, SD_reduction, cov))
                                f.write('\n')
                        f.close()

                        terminal_input="sbatch bashes/call_%s.sh"%name;
                        proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
                        (out, err) = proc.communicate()

######### SENSITIVITY
                                
### sensitivity senza BA2: with additional social distancing
str_launch = "srun python R-delta-waned_delta-vs-BA1_woBA2.py %s %d %d %d %d %f %s"

n_runs = [100]
ritmi = ['adu_ritmo2_start20210901_buffer30days']
ritardi = [4,5,6,7,8,9]
starting_weeks = {4:[8],5:[8],6:[1],7:[1],8:[1],9:[1]}## 1 or 10
durate = [4]  ## number of weeks of duration
riduzioni = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5,
             0.05, 0.15, 0.25, 0.35, 0.45]
            
coverage = ['limited'] ## full, limited

for ritmo in ritmi:
    for r in n_runs:
        for ritardo in ritardi:
            for start in starting_weeks[ritardo]:
                for durata in durate:
                    for cov in coverage:
                            
                        name = 'woBA2_red'

                        f = open("bashes_woBA2/call_%s.sh"%name,"w")
                        f.write("#!/bin/bash -l\n")
                        f.write("#SBATCH -J %s\n"%name)
                        f.write("#SBATCH -t 48:0:0\n\n")
                    
                        for SD_reduction in riduzioni:
                            f.write(str_launch%(ritmo, r, ritardo, start, durata, SD_reduction, cov))
                            f.write('\n')
                        
                        f.close()

                        terminal_input="sbatch bashes_woBA2/call_%s.sh"%name;
                        proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
                        (out, err) = proc.communicate()

### sensitivity senza BA2
str_launch = "srun python R-delta-waned_delta-vs-BA1_woBA2.py %s %d %d %d %d %f %s"

n_runs = [100]
ritmi = ['adu_ritmo3']
ritardi = [7]
starting_weeks = {7:[1]}## 1 or 10
durate = [4]  ## number of weeks of duration
riduzioni = [0.0]
            
coverage = ['full'] ## full, limited

for ritmo in ritmi:
    for r in n_runs:
        for ritardo in ritardi:
            for start in starting_weeks[ritardo]:
                for durata in durate:
                    for cov in coverage:
                            
                        name = 'woBA2_red'

                        f = open("bashes_woBA2/call_%s.sh"%name,"w")
                        f.write("#!/bin/bash -l\n")
                        f.write("#SBATCH -J %s\n"%name)
                        f.write("#SBATCH -t 48:0:0\n\n")
                    
                        for SD_reduction in riduzioni:
                            f.write(str_launch%(ritmo, r, ritardo, start, durata, SD_reduction, cov))
                            f.write('\n')
                        
                        f.close()

                        terminal_input="sbatch bashes_woBA2/call_%s.sh"%name;
                        proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
                        (out, err) = proc.communicate()
                        
                        
## sensitivity senza BA2

str_launch = "srun python R-delta-waned_delta-vs-BA1BA2_reinfection-Omi-Omi_accelerate_campaign_3m_buf60d_noBA2.py %s %d %d %d %d %f %s %d"

n_runs = [100]
ritmi = ['adu_ritmo2_start20210901_buffer30days']
ritardi = [8]
#ritardi = [6,7,8]
starting_weeks = [1] ## 1 or 10
durate = [4]  ## number of weeks of duration
riduzioni = [0.0]
coverage = ['limited'] ## full, limited
week_accel = [45, 46, 47, 48, 49, 50, 51, 52, 53]

for ritmo in ritmi:
    for r in n_runs:
        for ritardo in ritardi:
            for start in starting_weeks:
                for durata in durate:
                    for SD_reduction in riduzioni:
                        for cov in coverage:
                            for ww in week_accel:
                            
                                name = '3m_{}w{}'.format(ritardo, ww)

                                f = open("bashes/call_%s.sh"%name,"w")
                                f.write("#!/bin/bash -l\n")
                                f.write("#SBATCH -J %s\n"%name)
                                f.write("#SBATCH -t 48:0:0\n\n")
                                
                                # f.write("cd symp-R%d\n"%R)
                                f.write(str_launch%(ritmo, r, ritardo, start, durata, SD_reduction, cov, ww))
                                f.write('\n')
                                    
                                f.close()

                                terminal_input="sbatch bashes/call_%s.sh"%name;
                                proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
                                (out, err) = proc.communicate()
                            
str_launch = "srun python R-delta-waned_delta-vs-BA1BA2_reinfection-Omi-Omi_accelerate_campaign_2m_buf60d_noBA2.py %s %d %d %d %d %f %s %d"

n_runs = [100]
ritmi = ['adu_ritmo2_start20210901_buffer30days']
ritardi = [7]
#ritardi = [6,7,8]
starting_weeks = [1] ## 1 or 10
durate = [4]  ## number of weeks of duration
riduzioni = [0.0]
coverage = ['limited'] ## full, limited
week_accel = [45, 46, 47, 48, 49, 50, 51, 52, 53]

for ritmo in ritmi:
    for r in n_runs:
        for ritardo in ritardi:
            for start in starting_weeks:
                for durata in durate:
                    for SD_reduction in riduzioni:
                        for cov in coverage:
                            for ww in week_accel:
                            
                                name = '2m_{}w{}'.format(ritardo, ww)

                                f = open("bashes/call_%s.sh"%name,"w")
                                f.write("#!/bin/bash -l\n")
                                f.write("#SBATCH -J %s\n"%name)
                                f.write("#SBATCH -t 48:0:0\n\n")
                                
                                # f.write("cd symp-R%d\n"%R)
                                f.write(str_launch%(ritmo, r, ritardo, start, durata, SD_reduction, cov, ww))
                                f.write('\n')
                                    
                                f.close()

                                terminal_input="sbatch bashes/call_%s.sh"%name;
                                proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
                                (out, err) = proc.communicate()


### 9 nodi per 9x45--> circa 7 ore
### sensitivity durate:
str_launch = "srun python R-delta-waned_delta-vs-BA1BA2_reinfection-Omi-Omi_addSD_wX_duratY.py %s %d %d %d %d %f %s"

n_runs = [100]
ritmi = ['adu_ritmo2_start20210901_buffer30days']
ritardi = [4,5,6,7,8,9]
starting_weeks = {4:[8],5:[8,7],6:[3,1],7:[2,1],8:[1],9:[1]}## 1 or 10
durate = [3,5]  ## number of weeks of duration
riduzioni = [0.1, 0.2, 0.3, 0.4, 0.5,
             0.05, 0.15, 0.25, 0.35, 0.45]
coverage = ['limited'] ## full, limited

for ritmo in ritmi:
    for r in n_runs:
        for durata in durate:
            for cov in coverage:
                    for ritardo in ritardi:
                        for start in starting_weeks[ritardo]:
                            name = str_param%(ritmo, r, ritardo, 0, durata, 0, cov)

                            f = open("bashes/call_%s.sh"%name,"w")
                            f.write("#!/bin/bash -l\n")
                            f.write("#SBATCH -J %s\n"%name)
                            f.write("#SBATCH -t 70:0:0\n\n")
                    
                            for SD_reduction in riduzioni:
                                f.write(str_launch%(ritmo, r, ritardo, start, durata, SD_reduction, cov))
                                f.write('\n')
                                
                            f.close()

                            terminal_input="sbatch bashes/call_%s.sh"%name;
                            proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
                            (out, err) = proc.communicate()


################### sensitivity immunity

for str_launch in ["srun python R-delta-waned_delta-vs-BA1BA2_reinfection-Omi-Omi_addSD_wX_duratY_no-loss-immunity_vaccination.py %s %d %d %d %d %f %s"]:

    n_runs = [100]
    ritmi = ['adu_ritmo2_start20210901_buffer30days']
    ritardi = [4,5,6,7,8,9]
    starting_weeks = {4:[1],5:[1],6:[1],7:[1],8:[1],9:[1]}## 1 or 10
    durate = [4]  ## number of weeks of duration
    riduzioni = [0.0]
    coverage = ['limited'] ## full, limited

    for ritmo in ritmi:
        for r in n_runs:
            for durata in durate:
                for cov in coverage:
                        for ritardo in ritardi:
                            for start in starting_weeks[ritardo]:
                                name = str_param%(ritmo, r, ritardo, 0, durata, 0, cov)

                                f = open("bashes/call_%s.sh"%name,"w")
                                f.write("#!/bin/bash -l\n")
                                f.write("#SBATCH -J %s\n"%name)
                                f.write("#SBATCH -t 70:0:0\n\n")
                        
                                for SD_reduction in riduzioni:
                                    f.write(str_launch%(ritmo, r, ritardo, start, durata, SD_reduction, cov))
                                    f.write('\n')
                                f.close()

                                terminal_input="sbatch bashes/call_%s.sh"%name;
                                proc = subprocess.Popen([terminal_input],stdout=subprocess.PIPE,shell=True);
                                (out, err) = proc.communicate()

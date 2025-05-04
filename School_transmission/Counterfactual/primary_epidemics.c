#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <time.h>

int main(int argc, char *argv[])

{
   // NUMBER OF ITERATIONS
   int nr;
   int num_run; 			// 5000 nr contatore sul numero di simulazioni lanciate
   contact ct;
   metadata data;
   Teach tc;
   Param param;
   Weeklyintro* introst=(Weeklyintro *) calloc(1, sizeof(Weeklyintro));
   Weeklyintro1* introtc=(Weeklyintro1 *) calloc(1, sizeof(Weeklyintro1));

   Trast rbeta_st;
   Tratc rbeta_tc;
   Subst psub_st;
   Subtc psub_tc;

   Susst sigma_st;
   Sustc sigma_tc;

   int dim_str = 10000;

   char contactfile[dim_str];
   char datafile[dim_str];
   char classfile[dim_str];
   char introfilestudents[dim_str];
   char introfileteachers[dim_str];
   char adhesionfile[dim_str];
   char stvaxfile[dim_str];
   char sttrafile[dim_str];
   char stsubfile[dim_str];
   char stsusfile[dim_str]; 
   char tcvaxfile[dim_str];
   char tctrafile[dim_str];
   char tcsubfile[dim_str];
   char tcsusfile[dim_str];




   FILE *fp5;
   char outfile5[dim_str];

   FILE *fp16;
   char outfile16[dim_str];

   FILE *fp17;
   char outfile17[dim_str];

   FILE *fp18;
   char outfile18[dim_str];

   FILE *fp19;
   char outfile19[dim_str];

   FILE *fp20;
   char outfile20[dim_str];

   FILE *fp21;
   char outfile21[dim_str];

   FILE *fp22;
   char outfile22[dim_str];

   FILE *fp23;
   char outfile23[dim_str];

   FILE *fp24;
   char outfile24[dim_str];

   sprintf(contactfile,"./data_in/contacts_PS_short.txt");
   sprintf(datafile,"./data_in/metadata_PS.txt");
   sprintf(classfile,"./data_in/teach_class_PS.txt");

   sprintf(introfilestudents,"./data_in/intros_stn2000_b0.0153_11010_im0.35_0.55_Pd0.3_0.5_t1440_d7_f1_1.txt");
   sprintf(introfileteachers,"./data_in/intros_tcn2000_b0.0153_11010_im0.35_0.55_Pd0.3_0.5_t1440_d7_f1_1.txt");

   sprintf(adhesionfile,"./data_in/adhesion_0106_Rhone.txt");

   sprintf(stvaxfile,"./data_in/students_tovax_01_06_Rhone.txt");
   sprintf(tcvaxfile,"./data_in/teachers_toboost_01_06_Rhone.txt");

   sprintf(sttrafile,"./data_in/rbeta_students_01_06_VE.txt");
   sprintf(tctrafile,"./data_in/rbeta_teachers_01_06_VE.txt");

   sprintf(stsusfile,"./data_in/sigma_students_01_06_VE.txt");
   sprintf(tcsusfile,"./data_in/sigma_teachers_01_06_VE.txt");

   sprintf(stsubfile,"./data_in/pl_subclinical_students_01_06_VE.txt");
   sprintf(tcsubfile,"./data_in/pl_subclinical_teachers_01_06_VE.txt");


   int arr[2];



   read_metadata(datafile, &data);
   read_contactdata(contactfile, &ct, arr);
   read_teachclass(classfile, &tc);

   //Read weekly introduction
   read_intro_st(introfilestudents, &introst, &param);
   read_intro_tc(introfileteachers, &introtc, &param);

   // Read susceptibility (rows: weeks, columns: vax status (no vax, 2 doses, booster))
   read_susc_st(stsusfile, &sigma_st);
   read_susc_tc(tcsusfile, &sigma_tc);

   // Read transmissibility (rows: weeks, columns: vax status (no vax p/c, 2 doses p/c, booster pc/c))
   read_transm_st(sttrafile, &rbeta_st);
   read_transm_tc(tctrafile, &rbeta_tc);

   // Read probability to be subclinical (rows: weeks, columns: vax status (no vax, 2 doses, booster))
   read_sub_st(stsubfile, &psub_st);
   read_sub_tc(tcsubfile, &psub_tc);

   float *weekly_perc_adhesion;
   weekly_perc_adhesion=(float *)calloc(param.nweeks, sizeof(float));

   read_adhesion(adhesionfile, &param, weekly_perc_adhesion);


   int *students_tovax, *teachers_toboost;
   students_tovax=(int *)calloc(param.nweeks, sizeof(int));
   teachers_toboost=(int *)calloc(param.nweeks, sizeof(int));

   read_studentstovax(stvaxfile, &param, students_tovax);
   read_teacherstoboost(tcvaxfile, &param, teachers_toboost);
   //exit(0);

   setenv("GSL_RNG_SEED", "1", 1);
   gsl_rng_env_setup();
   RNG = gsl_rng_alloc(gsl_rng_default);


   /////////// SET INITIAL CONDITION 

   //MEASURE
   int symptomatic;
   int preventive;
   int preventivegrade;
   int regular;



   // PERCENTAGE OF IMMUNITY
   float perc_immunity_stud, perc_immunity_teac; 					// initial immunity


   int turn_time_min;
   int days_isolation;

   float p_detec_stud;
   float p_detec_teac;


   // Gamma shape for transition

   param.shape_latent = 3; //to check
   param.shape_prodro = 1; //to check
   param.shape_infect = 6; //to check
   param.shape_rp = 4;

   int *counter_latent, *counter_prodro, *counter_infect, *counter_rp;
   counter_latent=(int*)calloc(data.n, sizeof(int));
   counter_prodro=(int*)calloc(data.n, sizeof(int));
   counter_infect=(int*)calloc(data.n, sizeof(int));
   counter_rp=(int*)calloc(data.n, sizeof(int));

   // TEST SENSITIVITY

      
      float *theta_p, *theta_c, *theta_sc, *theta_rp;
      theta_p = (float *)calloc(param.shape_prodro, sizeof(float));
      theta_sc = (float *)calloc(param.shape_infect, sizeof(float));
      theta_c = (float *)calloc(param.shape_infect, sizeof(float));
      theta_rp = (float *)calloc(param.shape_rp, sizeof(float));
      
      theta_p[0]=0.778589307412566;
      
      theta_sc[0]=0.917756722231222;
      theta_sc[1]=0.944310633938105;
      theta_sc[2]=0.954272576237314;
      theta_sc[3]=0.948879062006511;
      theta_sc[4]=0.929471389943724;
      theta_sc[5]=0.897495644567351;
      
      theta_c[0]=0.954721526608823;
      theta_c[1]=0.94815376208867;
      theta_c[2]=0.939810030857176;
      theta_c[3]=0.92945104736374;
      theta_c[4]=0.916835293733786;
      theta_c[5]=0.90174719379345;
      
      theta_rp[0]=0.7541951651501196;
      theta_rp[1]=0.4841309758268747;
      theta_rp[2]=0.26597822883343347;
      theta_rp[3]=0.1319209349076362;
      
      
      float **theta_ps, **theta_cs, **theta_scs, **theta_rps;
      theta_ps = (float **)calloc(2, sizeof(float*));
      theta_scs = (float **)calloc(2, sizeof(float*));
      theta_cs = (float **)calloc(2, sizeof(float*));
      theta_rps = (float **)calloc(2, sizeof(float*));

      
      
      
      int a;
      a=0;
      for(a=0; a<2; a++){
      theta_ps[a]=(float *) calloc(param.shape_prodro, sizeof(float));
      theta_scs[a]=(float *) calloc(param.shape_infect, sizeof(float));
      theta_cs[a]=(float *) calloc(param.shape_infect, sizeof(float));
      theta_rps[a]=(float *) calloc(param.shape_rp, sizeof(float));
      
      }
      
      /*Children*/
      theta_ps[0][0]=0.793143865255565;
      
      theta_scs[0][0]=0.917756722231222;
      theta_scs[0][1]=0.944310633938105;
      theta_scs[0][2]=0.954272576237314;
      theta_scs[0][3]=0.948879062006511;
      theta_scs[0][4]=0.929471389943724;
      theta_scs[0][5]=0.897495644567351;

      theta_cs[0][0]=0.954721526608823;
      theta_cs[0][1]=0.94815376208867;
      theta_cs[0][2]=0.939810030857176;
      theta_cs[0][3]=0.92945104736374;
      theta_cs[0][4]=0.916835293733786;
      theta_cs[0][5]=0.90174719379345;

      theta_rps[0][0]=0.7157511179697935;
      theta_rps[0][1]=0.38455858175581425;
      theta_rps[0][2]=0.14544900400314162;
      theta_rps[0][3]=0.05276837396395447;
      
      
      
      /*Teachers*/

      theta_ps[1][0]=0.778589307412566;
      
      theta_scs[1][0]=0.917756722231222;
      theta_scs[1][1]=0.944310633938105;
      theta_scs[1][2]=0.954272576237314;
      theta_scs[1][3]=0.948879062006511;
      theta_scs[1][4]=0.929471389943724;
      theta_scs[1][5]=0.897495644567351;
      
      theta_cs[1][0]=0.954721526608823;
      theta_cs[1][1]=0.94815376208867;
      theta_cs[1][2]=0.939810030857176;
      theta_cs[1][3]=0.92945104736374;
      theta_cs[1][4]=0.916835293733786;
      theta_cs[1][5]=0.90174719379345;

      theta_rps[1][0]=0.7541951651501196;
      theta_rps[1][1]=0.4841309758268747;
      theta_rps[1][2]=0.26597822883343347;
      theta_rps[1][3]=0.1319209349076362;

   int ntimes;
   int nweek;
   int reactive;

   // INPUT FROM COMMAND LINE

   if(argc!=1){
      int narg = 1;

      num_run = atoi(argv[narg]);         		
      narg++;
      param.beta = atof(argv[narg]);         		
      narg++;
      param.initial_seeds = atoi(argv[narg]);         		
      narg++;
      param.n_pro = atoi(argv[narg]);         		
      narg++;
      param.n_cli = atoi(argv[narg]);         		
      narg++;
      param.n_sub = atoi(argv[narg]);         		
      narg++;
      param.n_rp = atoi(argv[narg]);         		
      narg++;

      /****PROTOCOLS****/
      symptomatic = atoi(argv[narg]);         		
      narg++;
      preventive = atoi(argv[narg]);         		
      narg++;
      preventivegrade = atoi(argv[narg]);         		
      narg++;
      regular = atoi(argv[narg]);         		
      narg++;
      reactive = atoi(argv[narg]);         		
      narg++;

      /****IMMUNITY****/
      perc_immunity_stud = atof(argv[narg]);		// Immunity in students
      narg++;
      perc_immunity_teac = atof(argv[narg]);		// Immunity in teachers
      narg++;  


      /****DETECTION****/
      p_detec_stud = atof(argv[narg]);   		// probability to be detected as clinical for students
      narg++;
      p_detec_teac = atof(argv[narg]);   		// probability to be detected as clinical for teachers
      narg++;   


      /****tURNAROUNDTIME&iSOLATION****/
      turn_time_min = atoi(argv[narg]);       // Turnaround time to receive test results [min]
      narg++;
      days_isolation = atoi(argv[narg]);      
      narg++;

      //perc_adhesion= atof(argv[narg]);      
      //narg++;
      ntimes= atoi(argv[narg]);      
      narg++;
      nweek=atoi(argv[narg]);      
      narg++;

      /****** REACTIVE SCREENING *******/
      /*********************************/

      param.alpha = atof(argv[narg]);          // Percentage of people inside the class screened 
      narg++;
      param.deltar = atoi(argv[narg]);         // Delay in performing reactive
      narg++;
      param.delay = atoi(argv[narg]);         // Delay in performing reactive
      narg++;

      param.sigma_omicron=atof(argv[narg]);
   }


   // TURNAROUND TIME AND ISOLATION
   param.tau_test = turn_time_min;			// turnaround time in min
   param.delta_q  = days_isolation * 24 * 60;		// day of isolation in min

   // INITIAL IMMUNITY

   int n_removed_stud, n_removed_teac;					

   // initial number of removed nodes
   n_removed_stud = perc_immunity_stud * (data.n - tc.nc);
   n_removed_teac = perc_immunity_teac * (tc.nc);

   float *pd;

   //ps=(float *)calloc(6, sizeof(float));
   pd=(float *)calloc(2, sizeof(float));



   pd[0]=p_detec_stud;				// probability to be detected as clinical for students
   pd[1]=p_detec_teac;				// probability to be detected as clinical for teachers




   param.tau=2.8*1440;				// incubation period
   param.gamma=1.0/param.tau;			
   param.tau_p=1.8*1440;				// prodromical period
   param.mu_p=1/param.tau_p;		
   param.tau_i=4*1440; 				// infectious period
   param.mu=1.0/param.tau_i;			
   param.eps_inv=param.tau-param.tau_p;		// latency period
   param.eps=1.0/param.eps_inv;		
   param.tau_rp= 6*1440;

   param.mu_rp=1.0/param.tau_rp;
   param.mu_rp1=1.0/(param.tau_rp-param.delta_q); // rate in exit from R+ rescaled for the time spent in isolation
  
  
   //VACCINATION 
   int *r_delta;
   r_delta=(int *)calloc(data.n, sizeof(int ));

   int *vaccinated_2doses, *vaccinated_booster, *suscep, l, i;	
   vaccinated_2doses=(int *)calloc(data.n, sizeof(int ));
   vaccinated_booster=(int *)calloc(data.n, sizeof(int ));

   int *exp_day;
   exp_day=(int*)calloc(data.n, sizeof(int));

   int *id_classes;
   id_classes=(int*)calloc(1, sizeof(int));
            

   //VECTOR AND MATRIX DECLARATION

   int *nweek_2doses, *nweek_booster;
   nweek_2doses=(int *)calloc(data.n, sizeof(int ));
   nweek_booster=(int *)calloc(data.n, sizeof(int ));

   int *status0;
   status0=(int *)calloc(data.n, sizeof(int ));  

   int *status;
   status=(int *)calloc(data.n, sizeof(int )); 

   int *pre_flag;
   pre_flag=(int *)calloc(data.n, sizeof(int )); 

   int k;

   int n_row=2;
   int **pre_time;
   pre_time=(int **)calloc(n_row, sizeof(int *));
   for(k=0; k<n_row; k++)
   pre_time[k]=(int *) calloc(data.n, sizeof(int));

   // SCHOOL HOURS	
   int ndays=42;				// numbers of synthetic days 
               

   int *start, *end;
   start=(int *)calloc(ndays, sizeof(int )); 
   end=(int *)calloc(ndays, sizeof(int )); 

   int val_start = arr[0];   // 15-minutes interval in which contacts start
   int val_end = arr[1];     // 15-minutes interval in which contacts end

   // LAST DAY
   param.tf=42; 	    //tf=120 in day

   times(&ct, &param, start, end, val_start, val_end);

   //Time step 
   param.deltat=15;			// 15 minutes // In a day, there 96 intervals of 15 minutes

   //COMPLIANCE

   int *comp_id;
   comp_id=(int *)calloc(data.n, sizeof(int ));


   // WRITE OUTPUT FILES

   char dir_out[100] = "data_out/";
   char str_param[20000] = "";


   snprintf(str_param, sizeof(str_param), "n%d_b%.4f_%d%d%d%d%d_im%.2f_%.2f_Pd%.1f_%.1f_t%d_d%d_f%d_%d", num_run, param.beta, symptomatic, preventive, preventivegrade, regular, reactive, perc_immunity_stud, perc_immunity_teac, p_detec_stud, p_detec_teac, turn_time_min, days_isolation, ntimes, nweek);


   // WRITE FILES

   
   sprintf(outfile5, "%sfsize_%s.txt", dir_out, str_param);
   fp5=fopen(outfile5, "w");

   sprintf(outfile16, "%sreact_%s.txt", dir_out, str_param);
   fp16=fopen(outfile16, "w");

   sprintf(outfile17, "%spreact_%s.txt", dir_out, str_param);
   fp17=fopen(outfile17, "w");

   sprintf(outfile18, "%stestr_%s.txt", dir_out, str_param);
   fp18=fopen(outfile18, "w");

   sprintf(outfile19, "%stestpr_%s.txt", dir_out, str_param);
   fp19=fopen(outfile19, "w");

   sprintf(outfile20, "%sposit_%s.txt", dir_out, str_param);
   fp20=fopen(outfile20, "w");

   sprintf(outfile21, "%strasm_st%s.txt", dir_out, str_param);
   fp21=fopen(outfile21, "w");

   sprintf(outfile22, "%sintros_st%s.txt", dir_out, str_param);
   fp22=fopen(outfile22, "w");

   sprintf(outfile23, "%sintros_tc%s.txt", dir_out, str_param);
   fp23=fopen(outfile23, "w");

   sprintf(outfile24, "%strasm_tc%s.txt", dir_out, str_param);
   fp24=fopen(outfile24, "w");

   int j, ns, ni, nnr, nnrp;
   int index_seed, index_seed1, index_seed2;
  
   for(nr=0; nr<num_run; nr++){
  
     
     //ASSIGN STATUS0
     
      for (j=0; j<data.n; j++){
         status0[j]=0;
         vaccinated_2doses[j]=0;
         vaccinated_booster[j]=0;
         
         nweek_2doses[j]=-100;
         nweek_booster[j]=-100;
      }
      
      
      assign_immunity(&data, &tc, status0, n_removed_stud, n_removed_teac, r_delta);
     
      for (j=0; j<data.n; j++){
         if(data.classes[j]==6){
         vaccinated_2doses[j]=1;
         nweek_2doses[j]=0;
         }
      }
     
      int nvax_2doses=0;
      for (j=0; j<data.n; j++){
         if (vaccinated_2doses[j]==1){
            nvax_2doses++;
         }
      }

      int nvax_booster=0;
      for (j=0; j<data.n; j++){
         if (vaccinated_booster[j]==1){
            nvax_booster++;
         }
      }

	  
      if (param.initial_seeds==1){

      suscep=(int *)calloc(1, sizeof(int ));

      l=0;
      for (i=0; i<data.n; i++){
         if ((status0[i]!=12) & (vaccinated_2doses[i]!=1)   & (vaccinated_booster[i]!=1)){
            suscep[l]=i;
            suscep=(int*)realloc(suscep,(l+2)*sizeof(int));
            l++;
         }
      }

      index_seed = suscep[(int)gsl_ran_flat(RNG,0,l)];
      status0[index_seed] = 1;

      free(suscep);

      }else if (param.initial_seeds>1){
     
         int *suscep_index, *suscep_id;
         suscep_index=(int *)calloc(1, sizeof(int ));
         suscep_id=(int *)calloc(1, sizeof(int ));
   
         l=0;
         for (i=0; i<data.n; i++){
            if ((status0[i]!=12) & (vaccinated_2doses[i]!=1)   & (vaccinated_booster[i]!=1) & (data.classes[i]!=6)){
               suscep_index[l]=i;
               suscep_id[l]=data.ID[i];
               suscep_index=(int*)realloc(suscep_index,(l+2)*sizeof(int));
               suscep_id=(int*)realloc(suscep_id,(l+2)*sizeof(int));
               l++;
            }
         }
         
         myshuffle(suscep_index, l);
     	
         int count_prodromic, count_clinical, count_subclinical, count_rp;
         count_prodromic=0;
         count_clinical=0;
         count_subclinical=0;
         count_rp=0;
     	
         for (i=0; i<param.n_pro+param.n_cli+param.n_sub+param.n_rp; i++){
            
            if ((i==0) & (param.n_pro!=0)){
            index_seed=index_nodes(&data, suscep_id[suscep_index[i]]);	
            status0[index_seed] = 2;
            count_prodromic++;
            //fprintf(stderr, "index seed %d\t id %d\t status %d\n", index_seed, data.ID[index_seed], status0[index_seed]);
            }
		
     		   if((i>=1) & (i<param.n_pro) & (param.n_pro!=0)){
     			index_seed=index_nodes(&data, suscep_id[suscep_index[i]]);	
     			status0[index_seed] = 2;
     			count_prodromic++;
     			//fprintf(stderr, "index seed %d\t id %d\t status %d\n", index_seed, data.ID[index_seed], status0[index_seed]);
     
     		   }else if ((i>=param.n_pro) & (i<param.n_pro+param.n_cli+param.n_sub) & ((param.n_cli!=0)  | (param.n_sub!=0))){
               if ((count_clinical<param.n_cli) & (param.n_cli!=0)){
                  index_seed=index_nodes(&data, suscep_id[suscep_index[i]]);	
                  status0[index_seed] = 4;
                  count_clinical++;
                  //fprintf(stderr, "index seed %d\t id %d\t status %d\n", index_seed, data.ID[index_seed], status0[index_seed]);
               }
               if ((i>=param.n_pro+param.n_cli) & (param.n_sub!=0)){
                  index_seed=index_nodes(&data, suscep_id[suscep_index[i]]);
                  status0[index_seed] = 5;
                  count_subclinical++;
                  //fprintf(stderr, "index seed %d\t id %d\t status %d\n", index_seed, data.ID[index_seed], status0[index_seed]);
               }
     		   }else if ((i>=param.n_pro+param.n_cli+param.n_sub) & (param.n_rp!=0)){
               if (count_rp<param.n_rp){
                  index_seed=index_nodes(&data, suscep_id[suscep_index[i]]);
                  status0[index_seed] = 8;
                  count_rp++;
                  //fprintf(stderr, "index seed %d\t id %d\t status %d\n", index_seed, data.ID[index_seed], status0[index_seed]);
               }
     			
     		   }
      	}	
     
         free(suscep_id);
         free(suscep_index);
     
      }

      // check
      ns=0; ni=0; nnr=0, nnrp=0;
      for (j=0; j<data.n; j++){
         if ((status0[j]==0) & (vaccinated_2doses[j]==0) & (vaccinated_booster[j]==0)){
            ns++;
         }else if (((status0[j]==1) & (param.initial_seeds==1)) | 
         (((status0[j]==2) | (status0[j]==4) | (status0[j]==5)) & (param.initial_seeds>1)) ){
            ni++;
         }else if (status0[j]==12){
            nnr++;
         }else if (status0[j]==8){
            nnrp++;
         }
      }
     
      if (nnr!= n_removed_stud+n_removed_teac){
         fprintf(stderr, "ERROR in assign status removed %d\t %d\t %d\n", nnr, n_removed_stud, n_removed_teac);
         exit(0);
      }else if (ni + nnrp != param.initial_seeds){
         fprintf(stderr, "No initial seed  %d\t %d\n", ni, param.initial_seeds);
         exit(0);
      }else if (l- param.initial_seeds!=ns){
         fprintf(stderr, "ERROR in assign status suscept. %d\t %d\t %d\t %d\n", ns, l-param.initial_seeds, ni, nnr);
         exit(0);
      }else if (param.n_rp!=nnrp){
         fprintf(stderr, "ERROR in assign rp %d\t %d\n", param.n_rp, nnrp);
         exit(0);
      }


      tseir(&data, &ct, &tc, &param, &introst,  &introtc, &sigma_st, &sigma_tc, &rbeta_st, &rbeta_tc, &psub_st, &psub_tc, symptomatic, preventive, preventivegrade, regular, reactive, pre_time, pre_flag, exp_day, status0, status, comp_id, start, end, pd, nr, students_tovax, teachers_toboost, vaccinated_2doses, vaccinated_booster, index_seed, theta_ps, theta_cs, theta_scs, theta_rps, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp, weekly_perc_adhesion, ntimes, nweek, r_delta, nweek_2doses, nweek_booster, fp5, fp16, fp17, fp18, fp19, fp20, fp21, fp22, fp23, fp24);

   }	  

   // CLOSE FILES
   
   fclose(fp5);
   fclose(fp16);
   fclose(fp17);
   fclose(fp18);
   fclose(fp19);
   fclose(fp20);
   fclose(fp21);
   fclose(fp22);
   fclose(fp23);
   fclose(fp24);


   free(status0);
   free(status);
   free(pre_time);
   free(pre_flag);
   free(start);
   free(end);

   free(pd);


}

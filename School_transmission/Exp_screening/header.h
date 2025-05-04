#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <time.h>


gsl_rng *RNG;


typedef struct{
  int *ID;
  int *classes;   
  unsigned short int *gender;
  int n;           // n: number od IDs
} metadata;

// Notation for gender: M=1, F=2, Teachers=0

typedef struct{
  int *days, *step;
  int *ID1, *ID2;
  int *class1, *class2; 
  float *weight;
  int n1;           // n1: number of contacts
} contact;

typedef struct{
  float *novax;
  float *twodoses;
  float *booster;
  int nweeks;           // nc: number of classes
} Susst;

typedef struct{
  float *novax;
  float *twodoses;
  float *booster;
  int nweeks;           // nc: number of classes
} Sustc;

typedef struct{
  float *novax_p, *novax_c;
  float *twodoses_p, *twodoses_c;
  float *booster_p, *booster_c;
  int nweeks;           // nc: number of classes
} Trast;

typedef struct{
  float *novax_p, *novax_c;
  float *twodoses_p, *twodoses_c;
  float *booster_p, *booster_c;
  int nweeks;           // nc: number of classes
} Tratc;

typedef struct{
  float *novax;
  float *twodoses;
  float *booster;
  int nweeks;           // nc: number of classes
} Subst;

typedef struct{
  float *novax;
  float *twodoses;
  float *booster;
  int nweeks;           // nc: number of classes
} Subtc;

// Notation for classes:
// '1A': 11,'1B':12, '2A':21, '2B': 22, '3A':31, '3B':32, '4A':41, '4B':42,'5A':51, '5B':52, 'Teachers':6

typedef struct{
  int *ID;
  int *clas;
  int nc;           // nc: number of classes
} Teach;



typedef struct{
  float *weeks; //from weeks 8 to 14
} Weeklyintro; 

typedef struct{
  float *weeks; //from weeks 8 to 14
} Weeklyintro1;
 
typedef struct{

  int tf;

  // Compartimental parameters
  float beta;
  float gamma;
  float mu_p; 
  float eps;
  float g;
  float mu;
  float mu_rp;
  float tau_rp;
  float tau_rp_ag;
  float mu_rp1;
  

  float sigma_omicron;
  
  float eps_inv;
  float tau;
  float tau_p;
  int tau_i;
  
  int initial_seeds, n_pro, n_cli, n_sub, n_rp;
  
  int shape_latent, shape_prodro, shape_infect, shape_rp;
  // TEST parameter 
  int tau_test;
  //float teta_p, teta_sc, teta_c, teta_rp, teta_rp_ag;
  
  // Parameters related to detection and isolation protocols 
  float delta_d;
  int delta_q, delta_r;
  
  //Time step
  int deltat;
  
  // Number of intro
  int weeklyintro;
  int nweeks;
  
   // reactive screening
  float alpha; // percentage of adherence within the class
  int deltar; // delay in performing reactive screening in minutes
  int delay;  // delay in performing a test after a reactive in days
  
} Param;


void read_metadata(char *file, metadata *data);
void read_contactdata(char *file, contact *ct, int arr[]);
void read_teachclass(char *file, Teach *tc);
int read_intro_st(char *file, Weeklyintro **introst, Param *param);
int read_intro_tc(char *file, Weeklyintro1 **introtc, Param *param);

void read_susc_st(char *file, Susst *sigma_st);
void read_susc_tc(char *file, Sustc *sigma_tc);

void read_sub_st(char *file, Subst *psub_st);
void read_sub_tc(char *file, Subtc *psub_tc);

void read_transm_st(char *file, Trast *rbeta_st);
void read_transm_tc(char *file, Tratc *rbeta_tc);

void assign_compliance(metadata *data, Teach *tc, int *comp_id, float percentage, int regular, int regular_classes, float adhesion_classes);
void selection_inf_nodes(metadata *data, Teach *tc, int *i0, int n_inf);
void selection(metadata *data, int *r_nodes, int n_removed, int n_inf, int *i0);

void assign_vaccination(metadata *data, Teach *tc, int nw, int n_vaccinated_stud_2doses, int n_vaccinated_teac_booster, int *vaccinated_2doses,  int *vaccinated_booster, int *nweek_2doses, int *nweek_booster);
void assign_immunity(metadata *data, Teach *tc, int *status0, int n_removed_stud, int n_removed_teac, int *r_delta);
void read_teacherstoboost(char *file, Param *param, int *v);
void read_studentstovax(char *file, Param *param, int *v);

void times(contact *ct, Param *param, int *start, int *end, int val_start, int val_end);
void initial_status(metadata *data, Teach *tc, int *status0, int n_inf, int n_removed, int index_seed);
int initial_status_single(metadata *data, Teach *tc, int *status0, int n_inf, int n_removed);
int day_selection_intro(int n, int *v1, int *v2, int *start, int *end, int day);
int weekly_introduction(metadata *data, Teach *tc, int *status, int *status0,  int n, int double_presence, int *rep, int *exp_day, int day_simulation);

int index_nodes(metadata *data, int k);
int index_class(Teach *tc, int n_class);
int index_t(Teach *tc, int teacher);
int find(Teach *tc, int id, int clas);

void count_inf(metadata *data, Teach *tc, int sim, int day, int index, int *isoclass, int *status0, FILE *fp9, FILE *fp10);
int preisolation(metadata *data, Teach *tc, Param *param, int id, int class_id, int day, int q, int *pre_flag, int *teach_rep, int **pre_time, int *class_iso, int *screenstatus, int preventivegrade);
void regular_screening(metadata *data, Param *param, Teach *tc, int *status0, int *teach_rep, int *teach_rtested, int *pre_flag, int *quarantine, int *detected, int *screenstatus, int *comp_id, int *start_q2, int *start_rq2, int quarter, int day, int sim, float **theta_ps, float **theta_cs, float **theta_scs, float **theta_rps, int *counter_prodro, int *counter_infect, int *counter_rp, int *num_pos_class, FILE *fp12, FILE *fp13);
int test_teach(Teach *tc, Param *param, int preventive, int symptomatic, int id, int index, int *teach_2q2, int *teach_2q2_day, int *teach_2q2_m, int *teach_rep, int *quarantine_t, int *pre_f, int nsim, int day_simulation, int min, int *status0, int *status, float *theta_p, float *theta_c, float *theta_sc, float *theta_rp, int *counter_latent, int *counter_prodro, int *counter_infect, int *counter_rp);
void count_positive_st(metadata *data, int sim, int day_simulation, int *counter_prodro, int *counter_infect, int *counter_rp, int *status, int *detected, int *quarantine, int *pre_flag, int *num_iso, FILE *fp20);
void assign_compliance_st(metadata *data, Teach *tc, int *comp_id, int *quarantine, int *detected, int *num_iso, int *pre_flag, float percentage, int regular);
void read_adhesion(char *file, Param *param, float *v);


void save_status(metadata *data,int *status, int *status0);
void tseir(metadata *data, contact *ct, Teach *tc, Param *param, Weeklyintro **introst,  Weeklyintro1 **introtc, Susst *sigma_st, Sustc *sigma_tc, Trast *rbeta_st, Tratc *rbeta_tc, Subst *psub_st, Subtc *psub_tc, int symptomatic, int preventive, int preventivegrade, int regular, int reactive, int **pre_time, int *pre_flag, int *exp_day, int *status0, int *status, int *comp_id, int *start, int *end, float *pd, int nr, int *students_tovax, int *teachers_toboost, int*vaccinated_2doses, int*vaccinated_booster, int index_seed, float **theta_ps, float **theta_cs, float **theta_scs, float **theta_rps, float *theta_p, float *theta_c, float *theta_sc, float *theta_rp, int *counter_latent, int *counter_prodro, int *counter_infect, int *counter_rp,  float *weekly_perc_adhesion, int ntimes, int nweek, int *r_delta, int *nweek_2doses, int *nweek_booster, FILE *fp12, FILE *fp13, FILE *fp20, FILE *fp21, FILE *fp22, FILE *fp23, FILE *fp24);

void myshuffle(int *newarray, int nnodes_tot);
int get_line(char **line,FILE *fp);

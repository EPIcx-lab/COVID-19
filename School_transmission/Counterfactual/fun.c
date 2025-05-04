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

void read_metadata(char *file, metadata *data){
  FILE *fp;
  char str[99];
  int col;

  if((fp=fopen(file,"r"))==NULL){
    perror("Error opening file metadata for reading :");
    exit(1);
  }
  // n for counting rows in the file

  data->n=0;                                      // n numbers of nodes
  data->ID=(int*)calloc(1,sizeof(int));
  data->classes=(int*)calloc(1,sizeof(int));
  data->gender=(unsigned short int*)calloc(1,sizeof(unsigned short int));
  
  // M:1, F:2, Unknown:0
  
  
  col=1;
  while (fscanf(fp, "%s", str)!=EOF){
    switch(col){
    case 1:
      data->ID[data->n]=atoi(str); 
      data->ID=(int*)realloc(data->ID,(data->n+2)*sizeof(int));
      col=2;
      break;
    case 2:
      data->classes[data->n]=atoi(str);
      data->classes=(int*)realloc(data->classes,(data->n+2)*sizeof(int));
      col=3; 
     break;
    case 3:
      data->gender[data->n]=atoi(str);
      data->gender=(unsigned short int*)realloc(data->gender,(data->n+2)*sizeof(unsigned short int));
      col=1;
      data->n++;
      break; 
   /* case 2:
      data->index[data->n]=atoi(str); 
      data->index=(int*)realloc(data->index,(data->n+2)*sizeof(int));
      col=1;
      data->n++;
      break; */	
    default:
      fprintf(stderr,"impossible\n");
      exit(1);
      break;
    }
  }
  fclose(fp);
}



void read_contactdata(char *file, contact *ct, int arr[]){

  FILE *fp;
  char str[99];
  int col;
  int massimo = -1;
  int minimo = 100;

  if((fp=fopen(file,"r"))==NULL){
    fprintf(stderr,"Error opening file %s for reading\n",file);
    exit(1);
  }
  
  //n1 for counting rows in the file

  ct->n1=0;                               
  
  ct->days=(int*)calloc(1,sizeof(int));
  ct->step=(int*)calloc(1,sizeof(int));
  ct->ID1=(int*)calloc(1,sizeof(int));
  ct->ID2=(int*)calloc(1,sizeof(int));
  ct->class1=(int*)calloc(1,sizeof(int));
  ct->class2=(int*)calloc(1,sizeof(int));
  ct->weight=(float*)calloc(1,sizeof(float));
  

  col=1;
  while (fscanf(fp, "%s", str)!=EOF){
    switch(col){
     case 1:
      ct->days[ct->n1]=atoi(str); 
      ct->days=(int*)realloc(ct->days,(ct->n1+2)*sizeof(unsigned int));
      col=2;
      break;
    case 2:
      ct->step[ct->n1]=atoi(str); 
      ct->step=(int*)realloc(ct->step,(ct->n1+2)*sizeof(int));
      if (atoi(str) < minimo){
      	minimo = atoi(str);
      }
      if (atoi(str) > massimo){
      	massimo = atoi(str);
      }
      col=3;
      break;
    case 3:
      ct->ID1[ct->n1]=atoi(str); 
      ct->ID1=(int*)realloc(ct->ID1,(ct->n1+2)*sizeof(int));
      col=4;
      break;
    case 4:
      ct->ID2[ct->n1]=atoi(str);
      ct->ID2=(int*)realloc(ct->ID2,(ct->n1+2)*sizeof(int));
      col=5;
      break;
    case 5:
      ct->class1[ct->n1]=atoi(str);
      ct->class1=(int*)realloc(ct->class1,(ct->n1+2)*sizeof(int));
      col=6;
      break;
    case 6:
      ct->class2[ct->n1]=atoi(str);
      ct->class2=(int*)realloc(ct->class2,(ct->n1+2)*sizeof(int));
      col=7;
      break;
    case 7:
      ct->weight[ct->n1]=atof(str); 
      ct->weight=(float*)realloc(ct->weight,(ct->n1+2)*sizeof(float)); 
      //ct->weight[ct->n1]=atoi(str); 
      //ct->weight=(float*)realloc(ct->weight,(ct->n1+2)*sizeof(float));
      col=1;
      ct->n1++;
      break;
    default:
      fprintf(stderr,"impossible\n");
      exit(1);
      break;
    }
  }
  // fprintf(stderr, "INTERVALLIIIII  %d  %d\n", minimo, massimo);
  arr[0] = minimo;
  arr[1] = massimo;
  fclose(fp);
}


void read_teachclass(char *file, Teach *tc){

  FILE *fp;
  char str[99];
  int col;

  if((fp=fopen(file,"r"))==NULL){
    fprintf(stderr,"Error opening file %s for reading\n",file);
    exit(1);
  }
  
  //n1 for counting rows in the file

  tc->nc=0;                               
  
  tc->ID=(int*)calloc(1,sizeof(int));
  tc->clas=(int*)calloc(1,sizeof(int));

  col=1;
  while (fscanf(fp, "%s", str)!=EOF){
    switch(col){
     case 1:
      tc->ID[tc->nc]=atoi(str); 
      tc->ID=(int*)realloc(tc->ID,(tc->nc+2)*sizeof(unsigned int));
      col=2;
      break;
    case 2:
      tc->clas[tc->nc]=atoi(str); 
      tc->clas=(int*)realloc(tc->clas,(tc->nc+2)*sizeof(float)); 
      col=1;
      tc->nc++;
      break;
    default:
      fprintf(stderr,"impossible\n");
      exit(1);
      break;
    }
  }
  fclose(fp);
}


int read_intro_st(char *file,Weeklyintro **introst, Param *param){
  char *line;
  int out_get_line=3;
  FILE *fp;
  short int tmpvalue;
  register int i=0, j;


  if(!(fp = fopen(file, "r"))){
    fprintf(stderr,"read_intro: error opening file %s for reading\n",file);
    return 1;
  }

  out_get_line=get_line(&line,fp);
  if(out_get_line<3){
    switch(out_get_line){
    case 2:
      fprintf(stderr,"read_intro: last line of file %s does not end in newline\n",file);
      break;
    case 1:
      fprintf(stderr,"read_intro: file %s contains an empty line\n",file);
      return 1;
      break;
    case 0:
      fclose(fp);
      return 0;
      break;
    case -1:
      fprintf(stderr,"read_intro: get_line error on file %s\n",file);
      return 1;
    default:
      fprintf(stderr,"read_intro: unrecognized exit status of get_line on file %s\n",file);
      return 1;
      break;
    }
  }
   
   
  while(line!=NULL){
    i++;
    line=(char *)strchr(line, '\t');
    //line= (char *)strchr(line, ',');
    if(line!=NULL)
      line++;
  }
  param->nweeks=i;
  
  fprintf(stderr, "param->weeks : %d\n", i);
  
  fclose(fp);
  fp = fopen(file, "r");
   
  i=0;
  
  while(out_get_line>=2){
    out_get_line=get_line(&line,fp);
    if(out_get_line<3){
      switch(out_get_line){
      case 2:
	fprintf(stderr,"read_intro: last line of file %s does not end in newline\n",file);
	break;
      case 1:
	fprintf(stderr,"read_intro: file %s contains an empty line\n",file);
	return 1;
	break;
      case 0:
	fclose(fp);
	//param->npatients=i;
	return 0;
	break;
      case -1:
	fprintf(stderr,"read_intro: get_line error on file %s\n", file);
	return 1;
      default:
	fprintf(stderr,"read_intro: unrecognized exit status of get_line on file %s\n",file);
	return 1;
	break;
      }
    }
    
    //param->nweeks = 7;
    (*introst)[i].weeks = (float *) calloc(param->nweeks, sizeof(float)); 
  
    
    for (j=0; j<param->nweeks;j++){
            //fprintf(stderr, "\t\t\t\t %d\n", j);
      sscanf(line,"%hd", &tmpvalue);
      (*introst)[i].weeks[j]=tmpvalue;
      line= (char *)strchr(line, '\t');
      //line= (char *)strchr(line, ',');
      line++;
    }
    i++;

    *introst = (Weeklyintro *) realloc(*introst, (i+1)*sizeof(Weeklyintro));   
     //fprintf(stderr, "i %d\t j%d\n", i, j);
  }
  //fprintf(stderr, "i %d\t j%d\n", i, j);
  
  return 2;
}

int read_intro_tc(char *file,Weeklyintro1 **introtc, Param *param){
  char *line;
  int out_get_line=3;
  FILE *fp;
  short int tmpvalue;
  register int i=0, j;


  if(!(fp = fopen(file, "r"))){
    fprintf(stderr,"read_intro: error opening file %s for reading\n",file);
    return 1;
  }

  out_get_line=get_line(&line,fp);
  if(out_get_line<3){
    switch(out_get_line){
    case 2:
      fprintf(stderr,"read_intro: last line of file %s does not end in newline\n",file);
      break;
    case 1:
      fprintf(stderr,"read_intro: file %s contains an empty line\n",file);
      return 1;
      break;
    case 0:
      fclose(fp);
      return 0;
      break;
    case -1:
      fprintf(stderr,"read_intro: get_line error on file %s\n",file);
      return 1;
    default:
      fprintf(stderr,"read_intro: unrecognized exit status of get_line on file %s\n",file);
      return 1;
      break;
    }
  }
   
   
  while(line!=NULL){
    i++;
    line=(char *)strchr(line, '\t');
    //line= (char *)strchr(line, ',');
    if(line!=NULL)
      line++;
  }
  param->nweeks=i;
  
  fprintf(stderr, "param->weeks : %d\n", i);
  
  fclose(fp);
  fp = fopen(file, "r");
   
  i=0;
  
  while(out_get_line>=2){
    out_get_line=get_line(&line,fp);
    if(out_get_line<3){
      switch(out_get_line){
      case 2:
	fprintf(stderr,"read_intro: last line of file %s does not end in newline\n",file);
	break;
      case 1:
	fprintf(stderr,"read_intro: file %s contains an empty line\n",file);
	return 1;
	break;
      case 0:
	fclose(fp);
	//param->npatients=i;
	return 0;
	break;
      case -1:
	fprintf(stderr,"read_intro: get_line error on file %s\n", file);
	return 1;
      default:
	fprintf(stderr,"read_intro: unrecognized exit status of get_line on file %s\n",file);
	return 1;
	break;
      }
    }
    
    //param->nweeks = 7;
    (*introtc)[i].weeks = (float *) calloc(param->nweeks, sizeof(float)); 
  
    
    for (j=0; j<param->nweeks;j++){
            //fprintf(stderr, "\t\t\t\t %d\n", j);
      sscanf(line,"%hd", &tmpvalue);
      (*introtc)[i].weeks[j]=tmpvalue;
      line= (char *)strchr(line, '\t');
      //line= (char *)strchr(line, ',');
      line++;
    }
    i++;

    *introtc = (Weeklyintro1 *) realloc(*introtc, (i+1)*sizeof(Weeklyintro1));   
     //fprintf(stderr, "i %d\t j%d\n", i, j);
  }
  //fprintf(stderr, "i %d\t j%d\n", i, j);
  
  return 2;
}

int index_nodes(metadata *data, int k){

	int i,id,index;

	for (i=0; i<data->n; i++){
		id=data->ID[i];
		if (id==k)
			index=i;
 	}
 	//fprintf(stderr, "index: %d\n", index);
 		
	return index;

}

void read_transm_st(char *file, Trast *rbeta_st){

  FILE *fp;
  char str[99];
  int col;
  

  if((fp=fopen(file,"r"))==NULL){
    fprintf(stderr,"Error opening file %s for reading\n",file);
    exit(1);
  }
  
  //n1 for counting rows in the file

  rbeta_st->nweeks=0;                               
  
  rbeta_st->novax_p=(float*)calloc(1,sizeof(float));
  rbeta_st->novax_c=(float*)calloc(1,sizeof(float));
  rbeta_st->twodoses_p=(float*)calloc(1,sizeof(float));
  rbeta_st->twodoses_c=(float*)calloc(1,sizeof(float));
  rbeta_st->booster_p=(float*)calloc(1,sizeof(float));
  rbeta_st->booster_c=(float*)calloc(1,sizeof(float));

  col=1;
  while (fscanf(fp, "%s", str)!=EOF){
    switch(col){
     case 1:
      rbeta_st->novax_p[rbeta_st->nweeks]=atof(str); 
      rbeta_st->novax_p=(float*)realloc(rbeta_st->novax_p,(rbeta_st->nweeks+2)*sizeof(float));
      col=2;
      break;
    case 2:
      rbeta_st->novax_c[rbeta_st->nweeks]=atof(str); 
      rbeta_st->novax_c=(float*)realloc(rbeta_st->novax_c,(rbeta_st->nweeks+2)*sizeof(float));
      col=3;
      break;
    case 3:
      rbeta_st->twodoses_p[rbeta_st->nweeks]=atof(str); 
      rbeta_st->twodoses_p=(float*)realloc(rbeta_st->twodoses_p,(rbeta_st->nweeks+2)*sizeof(float));
      col=4;
      break;
    case 4:
      rbeta_st->twodoses_c[rbeta_st->nweeks]=atof(str); 
      rbeta_st->twodoses_c=(float*)realloc(rbeta_st->twodoses_c,(rbeta_st->nweeks+2)*sizeof(float));
      col=5;
      break;
    case 5:
      rbeta_st->booster_p[rbeta_st->nweeks]=atof(str); 
      rbeta_st->booster_p=(float*)realloc(rbeta_st->booster_p,(rbeta_st->nweeks+2)*sizeof(float));
      col=6;
      break;
    case 6:
      rbeta_st->booster_c[rbeta_st->nweeks]=atof(str); 
      rbeta_st->booster_c=(float*)realloc(rbeta_st->booster_c,(rbeta_st->nweeks+2)*sizeof(float));
      col=1;
      rbeta_st->nweeks++;
      break;
    default:
      fprintf(stderr,"impossible\n");
      exit(1);
      break;
    }
  }
  
  fclose(fp);
}

void read_transm_tc(char *file, Tratc *rbeta_tc){

  FILE *fp;
  char str[99];
  int col;
  

  if((fp=fopen(file,"r"))==NULL){
    fprintf(stderr,"Error opening file %s for reading\n",file);
    exit(1);
  }
  
  //n1 for counting rows in the file

  rbeta_tc->nweeks=0;                               
  
  rbeta_tc->novax_p=(float*)calloc(1,sizeof(float));
  rbeta_tc->novax_c=(float*)calloc(1,sizeof(float));
  rbeta_tc->twodoses_p=(float*)calloc(1,sizeof(float));
  rbeta_tc->twodoses_c=(float*)calloc(1,sizeof(float));
  rbeta_tc->booster_p=(float*)calloc(1,sizeof(float));
  rbeta_tc->booster_c=(float*)calloc(1,sizeof(float));

  col=1;
  while (fscanf(fp, "%s", str)!=EOF){
    switch(col){
     case 1:
      rbeta_tc->novax_p[rbeta_tc->nweeks]=atof(str); 
      rbeta_tc->novax_p=(float*)realloc(rbeta_tc->novax_p,(rbeta_tc->nweeks+2)*sizeof(float));
      col=2;
      break;
    case 2:
      rbeta_tc->novax_c[rbeta_tc->nweeks]=atof(str); 
      rbeta_tc->novax_c=(float*)realloc(rbeta_tc->novax_c,(rbeta_tc->nweeks+2)*sizeof(float));
      col=3;
      break;
    case 3:
      rbeta_tc->twodoses_p[rbeta_tc->nweeks]=atof(str); 
      rbeta_tc->twodoses_p=(float*)realloc(rbeta_tc->twodoses_p,(rbeta_tc->nweeks+2)*sizeof(float));
      col=4;
      break;
    case 4:
      rbeta_tc->twodoses_c[rbeta_tc->nweeks]=atof(str); 
      rbeta_tc->twodoses_c=(float*)realloc(rbeta_tc->twodoses_c,(rbeta_tc->nweeks+2)*sizeof(float));
      col=5;
      break;
    case 5:
      rbeta_tc->booster_p[rbeta_tc->nweeks]=atof(str); 
      rbeta_tc->booster_p=(float*)realloc(rbeta_tc->booster_p,(rbeta_tc->nweeks+2)*sizeof(float));
      col=6;
      break;
    case 6:
      rbeta_tc->booster_c[rbeta_tc->nweeks]=atof(str); 
      rbeta_tc->booster_c=(float*)realloc(rbeta_tc->booster_c,(rbeta_tc->nweeks+2)*sizeof(float));
      col=1;
      rbeta_tc->nweeks++;
      break;
    default:
      fprintf(stderr,"impossible\n");
      exit(1);
      break;
    }
  }
  
  fclose(fp);
}




void read_susc_st(char *file, Susst *sigma_st){

  FILE *fp;
  char str[99];
  int col;
  

  if((fp=fopen(file,"r"))==NULL){
    fprintf(stderr,"Error opening file %s for reading\n",file);
    exit(1);
  }
  
  //n1 for counting rows in the file

  sigma_st->nweeks=0;                               
  
  sigma_st->novax=(float*)calloc(1,sizeof(float));
  sigma_st->twodoses=(float*)calloc(1,sizeof(float));
  sigma_st->booster=(float*)calloc(1,sizeof(float));

  col=1;
  while (fscanf(fp, "%s", str)!=EOF){
    switch(col){
     case 1:
      sigma_st->novax[sigma_st->nweeks]=atof(str); 
      sigma_st->novax=(float*)realloc(sigma_st->novax,(sigma_st->nweeks+2)*sizeof(float));
      col=2;
      break;
    case 2:
      sigma_st->twodoses[sigma_st->nweeks]=atof(str); 
      sigma_st->twodoses=(float*)realloc(sigma_st->twodoses,(sigma_st->nweeks+2)*sizeof(float));
      col=3;
      break;
    case 3:
      sigma_st->booster[sigma_st->nweeks]=atof(str); 
      sigma_st->booster=(float*)realloc(sigma_st->booster,(sigma_st->nweeks+2)*sizeof(float));
      col=1;
      sigma_st->nweeks++;
      break;
    default:
      fprintf(stderr,"impossible\n");
      exit(1);
      break;
    }
  }
  
  fclose(fp);
}

void read_susc_tc(char *file, Sustc *sigma_tc){

  FILE *fp;
  char str[99];
  int col;
  

  if((fp=fopen(file,"r"))==NULL){
    fprintf(stderr,"Error opening file %s for reading\n",file);
    exit(1);
  }
  
  //n1 for counting rows in the file

  sigma_tc->nweeks=0;                               
  
  sigma_tc->novax=(float*)calloc(1,sizeof(float));
  sigma_tc->twodoses=(float*)calloc(1,sizeof(float));
  sigma_tc->booster=(float*)calloc(1,sizeof(float));

  col=1;
  while (fscanf(fp, "%s", str)!=EOF){
    switch(col){
     case 1:
      sigma_tc->novax[sigma_tc->nweeks]=atof(str);
      sigma_tc->novax=(float*)realloc(sigma_tc->novax,(sigma_tc->nweeks+2)*sizeof(float));
      col=2;
      break;
    case 2:
      sigma_tc->twodoses[sigma_tc->nweeks]=atof(str);
      sigma_tc->twodoses=(float*)realloc(sigma_tc->twodoses,(sigma_tc->nweeks+2)*sizeof(float));
      col=3;
      break;
    case 3:
      sigma_tc->booster[sigma_tc->nweeks]=atof(str);
      sigma_tc->booster=(float*)realloc(sigma_tc->booster,(sigma_tc->nweeks+2)*sizeof(float));
      col=1;
      sigma_tc->nweeks++;
      break;
    default:
      fprintf(stderr,"impossible\n");
      exit(1);
      break;
    }
  }
  
  fclose(fp);
}

void read_sub_st(char *file, Subst *psub_st){

  FILE *fp;
  char str[99];
  int col;
  

  if((fp=fopen(file,"r"))==NULL){
    fprintf(stderr,"Error opening file %s for reading\n",file);
    exit(1);
  }
  
  //n1 for counting rows in the file

  psub_st->nweeks=0;                               
  
  psub_st->novax=(float*)calloc(1,sizeof(float));
  psub_st->twodoses=(float*)calloc(1,sizeof(float));
  psub_st->booster=(float*)calloc(1,sizeof(float));

  col=1;
  while (fscanf(fp, "%s", str)!=EOF){
    switch(col){
     case 1:
      psub_st->novax[psub_st->nweeks]=atof(str);
      psub_st->novax=(float*)realloc(psub_st->novax,(psub_st->nweeks+2)*sizeof(float));
      col=2;
      break;
    case 2:
      psub_st->twodoses[psub_st->nweeks]=atof(str);
      psub_st->twodoses=(float*)realloc(psub_st->twodoses,(psub_st->nweeks+2)*sizeof(float));
      col=3;
      break;
    case 3:
      psub_st->booster[psub_st->nweeks]=atof(str);
      psub_st->booster=(float*)realloc(psub_st->booster,(psub_st->nweeks+2)*sizeof(float));
      col=1;
      psub_st->nweeks++;
      break;
    default:
      fprintf(stderr,"impossible\n");
      exit(1);
      break;
    }
  }
  
  fclose(fp);
}



void read_sub_tc(char *file, Subtc *psub_tc){

  FILE *fp;
  char str[99];
  int col;
  

  if((fp=fopen(file,"r"))==NULL){
    fprintf(stderr,"Error opening file %s for reading\n",file);
    exit(1);
  }
  
  //n1 for counting rows in the file

  psub_tc->nweeks=0;                               
  
  psub_tc->novax=(float*)calloc(1,sizeof(float));
  psub_tc->twodoses=(float*)calloc(1,sizeof(float));
  psub_tc->booster=(float*)calloc(1,sizeof(float));

  col=1;
  while (fscanf(fp, "%s", str)!=EOF){
    switch(col){
     case 1:
      psub_tc->novax[psub_tc->nweeks]=atof(str);
      psub_tc->novax=(float*)realloc(psub_tc->novax,(psub_tc->nweeks+2)*sizeof(float));
      col=2;
      break;
    case 2:
      psub_tc->twodoses[psub_tc->nweeks]=atof(str);
      psub_tc->twodoses=(float*)realloc(psub_tc->twodoses,(psub_tc->nweeks+2)*sizeof(float));
      col=3;
      break;
    case 3:
      psub_tc->booster[psub_tc->nweeks]=atof(str);
      psub_tc->booster=(float*)realloc(psub_tc->booster,(psub_tc->nweeks+2)*sizeof(float));
      col=1;
      psub_tc->nweeks++;
      break;
    default:
      fprintf(stderr,"impossible\n");
      exit(1);
      break;
    }
  }
  
  fclose(fp);
}


void myshuffle(int *newarray, int nnodes_tot){
    
    int i;
    
    // cREATE A LIST OF NODES OF LENGTH EQUAL TO THE TOT. NUMBER OF NODES
    for (i = 0; i < nnodes_tot; i++) {
      newarray[i]=i;
    }
    
    // miX IT USING A SHUFFLE
    for (i = 0; i < nnodes_tot; i++) {
      //int j = i + rand() / (RAND_MAX / (nnodes_tot - i) + 1);
      int j=i+rand()%(nnodes_tot-i);
      int t = newarray[j];
      newarray[j] = newarray[i];
      newarray[i] = t;
      
    }
    
    
   
  
}




void assign_vaccination(metadata *data, Teach *tc, int nw, int n_vaccinated_stud_2doses, int n_vaccinated_teac_booster, int *vaccinated_2doses,  int *vaccinated_booster, int *nweek_2doses, int *nweek_booster){

	int i, j,  k_s, k_t, nteachers_boosted, nstudents_vacc;
	
	
	k_s=0;
	k_t=0;
	
	nteachers_boosted=0;
	nstudents_vacc=0;
	
	for (i=0; i<data->n; i++){
		if ((data->classes[i]==6) & (vaccinated_booster[i]==1)){	
			nteachers_boosted++;
		}else if ((data->classes[i]!=6) & (vaccinated_2doses[i]==1)){
			nstudents_vacc++;
		}
	}
	
	
	
	int *ids_s, *ids_t, *ind_s, *ind_t;

	ids_t=(int*)calloc(tc->nc-nteachers_boosted,sizeof(int));
	ind_t=(int*)calloc(tc->nc-nteachers_boosted,sizeof(int));
	
	ids_s=(int*)calloc(data->n - tc->nc - nstudents_vacc,sizeof(int));
	ind_s=(int*)calloc(data->n - tc->nc - nstudents_vacc,sizeof(int));
	
	fprintf(stderr, "STUDENTS ALREADY VAX %d\t TO VAX %d\t NO VAX %d\n", nstudents_vacc, n_vaccinated_stud_2doses, data->n - tc->nc - nstudents_vacc);
	fprintf(stderr, "TEACHERS ALREADY BOOSTED %d\t to BOOST %d\t NO BOOSTED %d\n", nteachers_boosted, n_vaccinated_teac_booster, tc->nc-nteachers_boosted);

	for (i=0; i<data->n; i++){
		if ((data->classes[i]==6) & (vaccinated_booster[i]==0)){	
			ids_t[k_t]=data->ID[i];
			//fprintf(stderr, "%d\t %d\n", ids[k], data->ID[i]);
			ind_t[k_t]=k_t;
			k_t++;
		}else if ((data->classes[i]!=6) & (vaccinated_2doses[i]==0)){
			ids_s[k_s]=data->ID[i];
			//fprintf(stderr, "%d\t %d\n", ids[k], data->ID[i]);
			ind_s[k_s]=k_s;
			k_s++;
		}
	}
	
	//fprintf(stderr, "%d\t %d\n", k, tc->nc);
	myshuffle(ind_t, tc->nc-nteachers_boosted);
	myshuffle(ind_s, data->n - tc->nc - nstudents_vacc);
	
	
	for (j=0; j<n_vaccinated_teac_booster; j++){
	
		if(vaccinated_booster[index_nodes(data, ids_t[ind_t[j]])]==1){
			fprintf(stderr, "ERROR IN SHUFFLING TEACHERS booster\n");
			exit(0);
		}
		
		vaccinated_booster[index_nodes(data, ids_t[ind_t[j]])]=1;
		nweek_booster[index_nodes(data, ids_t[ind_t[j]])]=nw;
		vaccinated_2doses[index_nodes(data, ids_t[ind_t[j]])]=0;
		nweek_2doses[index_nodes(data, ids_t[ind_t[j]])]=-100;
		
		fprintf(stderr, "ids %d\t type %d\n", data->ID[index_nodes(data, ids_t[ind_t[j]])], data->classes[index_nodes(data, ids_t[ind_t[j]])]);
		if (data->classes[index_nodes(data, ids_t[ind_t[j]])]!=6){
			fprintf(stderr, "ERROR IN VACCINATIon to TEACHERS booster\n");
			exit(0);
		}
		
	}
	
	int nvax_stud_2doses=0;
	int nvax_stud_booster=0;
	
	for (j=0; j<n_vaccinated_stud_2doses; j++){
		
		if(vaccinated_2doses[index_nodes(data, ids_s[ind_s[j]])]==1){
			fprintf(stderr, "ERROR IN SHUFFLING STUDENTS booster\n");
			exit(0);
		}
		
		vaccinated_2doses[index_nodes(data, ids_s[ind_s[j]])]=1;
		nweek_2doses[index_nodes(data, ids_s[ind_s[j]])]=nw-nw;
		//fprintf(stderr, "ids %d\t type %d\n", data->ID[index_nodes(data, ids_s[ind_s[j]])], data->classes[index_nodes(data, ids_s[ind_s[j]])]);
		nvax_stud_2doses++;
		
		if (data->classes[index_nodes(data, ids_s[ind_s[j]])]==6){
			fprintf(stderr, "ERROR IN VACCINATIon to STUDENTS 2doses\n");
			exit(0);
		}
		
		
	
	}
	
	if(nvax_stud_2doses!=n_vaccinated_stud_2doses){
		fprintf(stderr, "ERROR in number of VACCINATED STUDENTS 2 doses\n");
		exit(0);
	}else {
		fprintf(stderr, "number of VACCINATED STUDENTS 2 doses %d\t %d\n", nvax_stud_2doses, n_vaccinated_stud_2doses);
	}
	
	


}

void assign_immunity(metadata *data, Teach *tc, int *status0, int n_removed_stud, int n_removed_teac, int *r_delta){

	int i, index_s, index_t, n, nn;
	int *students, *students_id;
	int *teachers, *teachers_id;
	
	students=(int*)calloc(data->n - tc->nc,sizeof(int));
	teachers=(int*)calloc(tc->nc,sizeof(int));
	students_id=(int*)calloc(data->n - tc->nc,sizeof(int));
	teachers_id=(int*)calloc(tc->nc,sizeof(int));
	
	index_s=0;
	index_t=0;
	
	for (i=0; i<data->n; i++){
		r_delta[i]=0;
		if (data->classes[i]==6){
			teachers_id[index_t]=data->ID[i];
			teachers[index_t]=i;
			
			//fprintf(stderr, "%d\t %d\t %d\n", index_t, i, teachers[index_t]);
			index_t++;
		}else if (data->classes[i]!=6){
			students_id[index_s]=data->ID[i];
			students[index_s]=i;
			index_s++;
		}
	}
	
	
	myshuffle(teachers, tc->nc);
	myshuffle(students, data->n - tc->nc);
	
	
	
	
	for (n = 0; n < n_removed_stud; n++){
		status0[index_nodes(data, students_id[students[n]])]=12;
		r_delta[index_nodes(data, students_id[students[n]])]=1;
		if (data->classes[index_nodes(data, students_id[students[n]])]==6){
			fprintf(stderr, "ERROR in the shuffle students\n");
			fprintf(stderr, "ids %d\t role %d\n", data->ID[students[n]], data->classes[students[n]]);
			exit(0);
		}
	}
	
	for (nn = 0; nn < n_removed_teac; nn++){
		status0[index_nodes(data, teachers_id[teachers[nn]])]=12;
		r_delta[index_nodes(data, teachers_id[teachers[nn]])]=1;
		if (data->classes[index_nodes(data, teachers_id[teachers[nn]])]!=6){
			fprintf(stderr, "ERROR in the shuffle teachers %d\t %d\n", data->ID[index_nodes(data, teachers[nn])], data->classes[index_nodes(data, teachers[nn])]);
			exit(0);
		}
	}
	
	free(students);
	free(teachers);
	free(students_id);
	free(teachers_id);

}

// SINGLE INITIAL INFECTIOUS  NODE 
// Random selection of an initial exposed node that is introduced in the network as seed of the epidemic

void initial_status(metadata *data, Teach *tc, int *status0, int n_inf, int n_removed, int index_seed){

	// seed initial nodes selected at random as seed of the epidemics
	//fprintf(stderr, "inside initial status\n");
	int jjj, nsusc=0,  ni=0, nr=0;
	for (jjj=0; jjj<data->n; jjj++){
		status0[jjj]=0;
	}
	
	
	
	for (jjj=0; jjj<data->n; jjj++){
		if (jjj == index_seed){
			status0[index_seed]=1;
		}else{
			status0[jjj]=0;
		}
	}
	
	
	for (jjj=0; jjj<data->n; jjj++){
		if(status0[jjj]==0){
			nsusc++;
		}else if (status0[jjj]==1){
			ni++;
		}else if (status0[jjj]==9){
			nr++;
		}
	}
	
	if (nsusc> data->n -(n_removed + n_inf)){
		fprintf(stderr, "ERROR IN INITIAL NUMBER OF SUSCEPTIBLE\n");
		exit(0);
	}
	
	if (data->n!=nsusc + ni + nr){
		fprintf(stderr, "ERROR IN ASSIGN STATUS\n");
		exit(0);
	}
	
	if(nr != n_removed){
		fprintf(stderr, "ERROR IN ASSIGN STATUS --- n removed\n");
		exit(0);
	}
	
	//return gen;

}
				


int index_class(Teach *tc, int n_class){
	
	int index;
	int n;
	
	for(n=0; n<tc->nc; n++){
		if(n_class==tc->clas[n]){
			index=n;
		}
	}
	
 	//fprintf(stderr, "index: %d\n", index);
 		
	return index;

}

void save_status(metadata *data,int *status, int *status0){

	int i;
	for (i=0; i<data->n; i++){
		status0[i]=status[i];
	}
}


// Index_t return a number from 0 to 9 corresponding to the index class
int index_t(Teach *tc, int teacher){

int t, index;

	for(t=0; t<tc->nc; t++){
		//fprintf(stderr, "%d\t %d\n", t, tc->ID[t]);
		if(tc->ID[t]==teacher){
			index=t;
			//fprintf(stderr, "t%d\t tc_id %d\t t %d\t index %d\n", teacher , tc->ID[t], t, index);
			
		}
	}

	return index;
    
}


int weekly_introduction_st(metadata *data, Teach *tc, int *status, int *status0,  int n, int double_presence, int *rep, int *exp_day, int day_simulation){
	
	int i;
	int index, m;
	int *susceptibles;
	//int *indx;
	int role_id; 
	
	// Count the number of the susceptibles students at the moment of the intro
	susceptibles=(int*)calloc(1,sizeof(int));
	
	m=0;
	for(i=0; i<data->n; i++){
		if (((status0[i]==0)| (status0[i]==12)) & (data->classes[i]!=6)){
			susceptibles[m]=i;
			susceptibles=(int*)realloc(susceptibles,(m+2)*sizeof(int));
			m++;
		}	
	}
	
	//fprintf(stderr, "number of susceptible:%d\n", m);	

		if (double_presence==0){
		
			index=(int)gsl_ran_flat(RNG,0,m);
			status[susceptibles[index]]=1;
			exp_day[susceptibles[index]]=day_simulation;
		
		}else if (double_presence>0){
		
			int *selected;
			selected=(int*)calloc(m,sizeof(int));
			index=(int)gsl_ran_flat(RNG,0,m);
			if (selected[index]==0){
				selected[index]=1;
				status[susceptibles[index]]=1;
				exp_day[susceptibles[index]]=day_simulation;
			}else{
				while(selected[index]==1){
					index=(int)gsl_ran_flat(RNG,0,m);
					break;
				}
				status[susceptibles[index]]=1;
				exp_day[susceptibles[index]]=day_simulation;
			}
			
		}		
		
		if (data->classes[susceptibles[index]]!=6){
			role_id=1;	
		}else if (data->classes[susceptibles[index]]==6){
			if (rep[index_t(tc, data->ID[susceptibles[index]])]==0){
				role_id=2;
			}else if (rep[index_t(tc, data->ID[susceptibles[index]])]>=1){
				role_id=3;
			}
		}
		
		fprintf(stderr, "id picked:%d\t index:%d\t status:%d\t role: %d\t type:%d\n", data->ID[susceptibles[index]], susceptibles[index], status[susceptibles[index]], data->classes[susceptibles[index]], role_id);
		
		//fprintf(stderr, "role %d\n", role_id);
		
	
	return role_id;
		
	free(susceptibles);
	
	
}

int weekly_introduction_tc(metadata *data, Teach *tc, int *status, int *status0,  int n, int double_presence, int *rep, int *exp_day, int day_simulation){
	
	int i;
	int index, m;
	int *susceptibles;
	//int *indx;
	int role_id; 
	
	// Count the number of the susceptibles students at the moment of the intro
	susceptibles=(int*)calloc(1,sizeof(int));
	
	m=0;
	for(i=0; i<data->n; i++){
		if (((status0[i]==0) | (status0[i]==12)) & (data->classes[i]==6)){
			susceptibles[m]=i;
			susceptibles=(int*)realloc(susceptibles,(m+2)*sizeof(int));
			m++;
		}	
	}
	
	//fprintf(stderr, "number of susceptible:%d\n", m);	

		if (double_presence==0){
		
			index=(int)gsl_ran_flat(RNG,0,m);
			status[susceptibles[index]]=1;
			exp_day[susceptibles[index]]=day_simulation;
		
		}else if (double_presence>0){
		
			int *selected;
			selected=(int*)calloc(m,sizeof(int));
			index=(int)gsl_ran_flat(RNG,0,m);
			if (selected[index]==0){
				selected[index]=1;
				status[susceptibles[index]]=1;
				exp_day[susceptibles[index]]=day_simulation;
			}else{
				while(selected[index]==1){
					index=(int)gsl_ran_flat(RNG,0,m);
					break;
				}
				status[susceptibles[index]]=1;
				exp_day[susceptibles[index]]=day_simulation;
			}
			
		}		
		
		if (data->classes[susceptibles[index]]!=6){
			role_id=1;	
		}else if (data->classes[susceptibles[index]]==6){
			if (rep[index_t(tc, data->ID[susceptibles[index]])]==0){
				role_id=2;
			}else if (rep[index_t(tc, data->ID[susceptibles[index]])]>=1){
				role_id=3;
			}
		}
		
		fprintf(stderr, "id picked:%d\t index:%d\t status:%d\t role: %d\t type:%d\n", data->ID[susceptibles[index]], susceptibles[index], status[susceptibles[index]], data->classes[susceptibles[index]], role_id);
		
		//fprintf(stderr, "role %d\n", role_id);
		
	
	return role_id;
		
	free(susceptibles);
	
	
}

//Funzione per individuare l'inizio e fine di ogni giorno scolastico contenuto nel dataset in secondi

void times(contact *ct, Param *param, int *start, int *end, int val_start, int val_end){

	int i, k, x, j, w;
	int week=7;
	
	for (i=0; i<param->tf; i=i+week){
		start[i]=val_start;
		end[i]=val_end;
	}
	
	for (k=1; k<param->tf; k=k+week){
		start[k]=val_start;
		end[k]=val_end;
	}

	for (x=2; x<param->tf; x=x+week){
		start[x]=val_start;
		end[x]=val_end;
	}
	
	for (j=3; j<param->tf; j=j+week){
		start[j]=val_start;
		end[j]=val_end;
	}
	
	for (w=4; w<param->tf; w=w+week){
		start[w]=val_start;
		end[w]=val_end;
	}
	
	/* int f;
	
	for (f=0; f<param->tf; f++){
		fprintf(stderr, "%d\t%d\t%d\n", f, start[f], end[f]);
	}
	
	exit(0);*/

}


int preisolation(metadata *data, Teach *tc, Param *param, int id, int class_id, int day, int q, int *pre_flag, int *teach_rep, int **pre_time, int *class_iso, int *screenstatus, int preventivegrade){

int i, ii, ind, j, tt, s, mod, cls;
int class, dim;

	dim=0;
	
	fprintf(stderr, "\n");
	fprintf(stderr, "<<< trigger: %d\t type: %d >>>\n", id, class_id);
	fprintf(stderr, "\n");
	
	// a) If the the node triggering the isolation is a student
	if (class_id!=6){	
		
		for (i=0; i<data->n; i++){
			if ((data->ID[i]!=id) & (data->ID[i]>0) & (screenstatus[i]!=6) & (data->classes[i]==class_id)){
 			    	pre_flag[i]=1;
 			    	pre_time[0][i]=day;
 			 	pre_time[1][i]=q; 
 			 	
 			 	//fprintf(stderr,"%d\t %d\n", pre_time[0][i], pre_time[1][i]);
 				//fprintf(stderr, "id Q2: %d\t classmate in Q3: %d\t class in pre-iso: %d\t %d\n", id, data->ID[i], class_id, pre_flag[i]);
 				dim++;
 				
 			}
 			
 		}		
 
 		for (ii=0; ii< tc->nc; ii++){
 			if (tc->clas[ii]==class_id){
 				ind=index_nodes(data, tc->ID[ii]);
 				class_iso[ii]=1;
 				if  (screenstatus[ind]!=6){
	 				pre_flag[ind]=1;
	 				// Teachers go in isolation together with students
	 			    	// I save day and quarter of preiso startting in a matrix
	 			    	pre_time[0][ind]=day;
	 		  	  	pre_time[1][ind]=q;
	 		  	  	fprintf(stderr, "---CORRESPONDING TEACHER---");
	 		   	 	fprintf(stderr, "id Q2: %d\t teacher in Q3: %d\t class in pre-iso: %d\t %d\n", id, tc->ID[ii],  class_id, pre_flag[ind]);
	 		   	 	dim++;
 		   	 	}
 		   	 	
 			}
 			
 		}
 	// b) If the node triggerring the isolation is a teacher		
	} else if (class_id==6){
		for (tt=0; tt<tc->nc; tt++){
			if (tc->ID[tt]==id){
				class=tc->clas[tt];
				class_iso[tt]=1;
				// For the replaced teacher the flag on preisolation is ON
				// because I'm assuming that if the class go in Q3, the replaced teacher goes too
				if (teach_rep[index_t(tc, id)]>=1){
				
					pre_flag[index_nodes(data, id)]=1;
					pre_time[0][index_nodes(data, id)]=day;
 		  	  		pre_time[1][index_nodes(data, id)]=q;
				}
				
				for (j=0; j< data->n; j++){
					if ((data->classes[j]==class) & (screenstatus[j]!=6)){
						pre_flag[j]=1;
						pre_time[0][j]=day;
 		  	  			pre_time[1][j]=q;
 		  	  			//fprintf(stderr, "teach pos: %d\t stud: %d\t class in pre-iso: %d\t %d\n", id, data->ID[j],  class, pre_flag[j]);				
 		  	  			dim++;
 		  	  			
					}
				}
				
			}
			//fprintf(stderr, "-----------------flag classes %d\t %d\n", tc->clas[tt], class_iso[tt]);
 			
		}
	}
	
	if (preventivegrade==1){
	
 		if (class_id!=6){	
			mod=class_id/10;
		} else if (class_id==6){
			mod=tc->clas[index_t(tc, id)]/10;
		}
		
		
			
		for (cls=0; cls< tc->nc; cls++){	
		
			
			if (tc->clas[cls]/10 == mod){
				fprintf(stderr, "level %d\t level2 %d\t clas %d\n", mod, tc->clas[cls]/10, tc->clas[cls]);
				
				for (s=0; s<data->n; s++){
					if ((data->classes[s]!=6) & (data->classes[s]==tc->clas[cls]) & (class_iso[cls]!=1)){
					
					
						pre_flag[s]=1;
		 			    	pre_time[0][s]=day;
		 		  	  	pre_time[1][s]=q;
		 		  	  	
		 		  	  	
	 		   	 		
	 		   	 		dim++;
	 		   	 		
					}else if ((data->classes[s]==6) &  (class_iso[cls]!=1) & (tc->clas[index_t(tc, data->ID[s])] == tc->clas[cls])){
					
						
						pre_flag[s]=1;
						
		 				// Teachers go in isolation together with students
		 			    	// I save day and quarter of preiso startting in a matrix
		 			    	
		 			    	pre_time[0][s]=day;
		 		  	  	pre_time[1][s]=q;
		 		  	  	
		 		  	  	// INSIDE PREISOLATION
		 		  	  	// TEACHER
		 		  	  	fprintf(stderr, "---CORRESPONDING TEACHERS---");
		 		  	  	
		 		  	  	
	 		   	 		fprintf(stderr, "id Q2: %d\t of class %d\t teacher in Q3: %d\t classes in pre-iso: %d\t %d\n", id, class_id, data->ID[s], data->classes[s], tc->clas[index_t(tc, data->ID[s])]);
	 		   	 		
		 		  	  	class_iso[index_t(tc, data->ID[s])]=1;
		 		  	  	
	 		   	 		dim++;
					}
				}
			}
			fprintf(stderr, "-----------------flag classes %d\t %d\n", tc->clas[cls], class_iso[cls]);
 			
		}

	}
	
	//exit(0);
	//fprintf(stderr, "dim class in prev iso:%d\n", dim);		
 	return dim;	   				
}


// Find function find the teacher associated to the class
int find(Teach *tc, int id, int clas){

	int t, teacher;
	
	if (clas!=6){
		for (t=0; t<tc->nc; t++){
			if(tc->clas[t]==clas){
				teacher=tc->ID[t];
			}
		}
	} else if (clas==6){
		teacher=id;
	}
	
	//fprintf(stderr, "teacher: %d\n", teacher);
	return teacher;
	
}



int test_teach(Teach *tc, Param *param, int preventive, int symptomatic, int id, int index, int *teach_2q2, int *teach_2q2_day, int *teach_2q2_m, int *teach_rep, int *quarantine_t, int *pre_f, int nsim, int day_simulation, int min, int *status0, int *status, float *theta_p, float *theta_c, float *theta_sc, float *theta_rp, int *counter_latent, int *counter_prodro, int *counter_infect, int *counter_rp){

	int s_teach;
	double random;
	int reason;	
	int test_p=0;
	int test_n=0;
	int result;
	float teta;
		
		//fprintf(stderr, "inside test teach, teacher %d\t, s0: %d\n", id, status0[index]);
		// n = number of teachers in isolation
		// t = vectors of teachers in isolation
		// tt[index_t(tc, id)]==0 to identified the original teach
		
		// WE distinguish two big cases
		// 1) test at the of Q2
		// 2) test at the end of Q3	
		
		s_teach=status0[index];
		
		if((preventive==0) & (symptomatic==1)){
		
			if (quarantine_t[index_t(tc,id)]==1){
			
				random=gsl_ran_flat(RNG,0,1);
				
				teta = theta_rp[counter_rp[index]];	
				fprintf(stderr, "index substatus: %d\t theta: %f\n\n", counter_rp[index], theta_rp[counter_rp[index]]);	
				if(random<teta){
				
					// Teacher is detected as positive and rplace with a susceptible one
					// status[index]=0;
					// After a quarantine if the teacher is still positive, we use the same substitute
					// We do not alter the status of the substitute with a new susceptible
					
					
					
					teach_2q2_day[index_t(tc, id)]=day_simulation;
					teach_2q2_m[index_t(tc, id)]=min;
						
					teach_2q2[index_t(tc, id)]=1;
					teach_rep[index_t(tc,id)]=1;
					quarantine_t[index_t(tc, id)]=1;
				
					test_p++;
					
					//status[index_id]=status_replace;
				
					
					fprintf(stderr, "TEST ON TEACH AT THE END OF Q2: %d\t status the moment of the test: R+\t result: pos\n", id);
				} else{
				
					status[index]=8;
					teach_rep[index_t(tc,id)]=-9;
					quarantine_t[index_t(tc,id)]=0;	
					test_n++;
					
					fprintf(stderr, "TEST ON TEACH AT THE END OF Q2: %d\t status the moment of the test:R+\t result: neg\n", id);
					
				}	
			}			
		}else if ((preventive==1) & (symptomatic==1)){
			if ((quarantine_t[index_t(tc,id)]==1) & (pre_f[index]==1)){
			
				random=gsl_ran_flat(RNG,0,1);
				teta = theta_rp[counter_rp[index]];	
				fprintf(stderr, "index substatus: %d\t theta: %f\t", counter_rp[index], theta_rp[counter_rp[index]]);	
				//exit(0);	
				if(random<teta){
				
					// Teacher is detected as positive and rplace with a susceptible one
					// status[index]=0;
					// After a quarantine if the teacher is still positive, we use the same substitute
					// We do not alter the status of the substitute with a new susceptible
					
					
					
					teach_2q2_day[index_t(tc, id)]=day_simulation;
					teach_2q2_m[index_t(tc, id)]=min;
						
					teach_2q2[index_t(tc, id)]=1;
					teach_rep[index_t(tc,id)]=1;
					quarantine_t[index_t(tc, id)]=1;
				
					test_p++;
					
					//status[index_id]=status_replace;
				
					
					fprintf(stderr, "TEST ON TEACH AT THE END OF Q2: %d\t status the moment of the test: R+\t result: pos\n", id);
					
					if (pre_f[index]==1){
						
						fprintf(stderr, "***********************\n");
						fprintf(stderr, "Teacher is the trigger of the preventive\n");
						fprintf(stderr, "status of the replaced teacher: %d, replaced %d\n", status0[index], teach_rep[index_t(tc,id)]);
						fprintf(stderr, "***********************\n");
						
						if ((s_teach==0) | (s_teach==9) | (s_teach==1) | (s_teach==12)){
							test_n++;
						}else{
							random=gsl_ran_flat(RNG,0,1);
							
							if (s_teach==2){
								teta=theta_p[counter_prodro[index]];
							}else if (s_teach==4){
								teta=theta_c[counter_infect[index]];
							}else if (s_teach==5){
								teta=theta_sc[counter_infect[index]];
							}else if ((s_teach==8) | (s_teach==12)){
								teta=theta_rp[counter_rp[index]];
							}
							
							if (random<teta){
								test_p++;
								status[index]=0;
							}
						}
						
					}
					
			
				} else{
				
					status[index]=8;
					teach_rep[index_t(tc,id)]=-9;
					quarantine_t[index_t(tc,id)]=0;	
					test_n++;
					
					fprintf(stderr, "TEST ON TEACH AT THE END OF Q2: %d\t status the moment of the test:R+\t result: neg\n", id);
					
					if (pre_f[index]==1){
						pre_f[index]=0;
					}
				}
			
			}else if ((pre_f[index]==1) & (quarantine_t[index_t(tc,id)]==0)){
			
				fprintf(stderr, "TEST ON TEACH AT THE END OF Q3: %d\t status the moment of the test:%d\t replaced:%d\n", id, s_teach, teach_rep[index_t(tc,id)]);
				
				reason=3;
				
				if ((s_teach==0) | (s_teach==9) | (s_teach==1) | (s_teach==12)){
					test_n++;
				}else if(s_teach==2){
					random=gsl_ran_flat(RNG,0,1);
					teta=theta_p[counter_prodro[index]];
					
					if(random<teta){
					fprintf(stderr, "index substatus: %d\t theta: %f\t", counter_rp[index], teta);
					// Teacher is detected as positive and rplace with a susceptible one
						status[index]=0;
						
						if (teach_rep[index_t(tc,id)]==0){
							teach_2q2_day[index_t(tc, id)]=day_simulation;
							teach_2q2_m[index_t(tc, id)]=min;
								
							teach_2q2[index_t(tc, id)]=1;
							teach_rep[index_t(tc,id)]=1;
							quarantine_t[index_t(tc, id)]=1;
						}else{
							status[index]=0;
							teach_rep[index_t(tc,id)]=1;
						}
						//fprintf(stderr, "teach replac: %d\t day%d\t m%d\t s0%d\n", id, q2[index_t(tc, id)], q2_m[index_t(tc, id)], s_teach);
						// s_teach original status of the original teacher
						//fprintf(fp6, "%d\t %d\t %d\t %d\t %d\t %d\n", nsim, id, tc->clas[index_t(tc, id)], day_simulation, s_teach, reason);
						test_p++;
					}else{
						status[index]=2;
						
						teach_2q2[index_t(tc, id)]=0;
						test_n++;
								
					}
				}else if (s_teach==4){
					random=gsl_ran_flat(RNG,0,1);	
					teta=theta_c[counter_infect[index]];	
					if(random<teta){
					// Teacher is detected as positive and rplace with a susceptible one
						status[index]=0;
						fprintf(stderr, "index substatus: %d\t theta: %f\t", counter_rp[index], teta);
						if (teach_rep[index_t(tc,id)]==0){
							teach_2q2_day[index_t(tc, id)]=day_simulation;
							teach_2q2_m[index_t(tc, id)]=min;
								
							teach_2q2[index_t(tc, id)]=1;
							teach_rep[index_t(tc,id)]=1;
							quarantine_t[index_t(tc, id)]=1;
						}else{
							status[index]=0;
							teach_rep[index_t(tc,id)]=1;
						}
						//fprintf(stderr, "teach replac: %d\t day%d\t m%d\t s0%d\n", id, q2[index_t(tc, id)], q2_m[index_t(tc, id)], s_teach);
						// s_teach original status of the original teacher
						//fprintf(fp6, "%d\t %d\t %d\t %d\t %d\t %d\n", nsim, id, tc->clas[index_t(tc, id)], day_simulation, s_teach, reason);			
						test_p++;
					}else{
						status[index]=4;
						teach_2q2[index_t(tc, id)]=0;	
						test_n++;
					}	
				}else if (s_teach==5){
					random=gsl_ran_flat(RNG,0,1);	
					teta=theta_sc[counter_infect[index]];		
					if(random<teta){
					// Teacher is detected as positive and rplace with a susceptible one
						status[index]=0;
						fprintf(stderr, "index substatus: %d\t theta: %f\t", counter_rp[index], teta);
						if (teach_rep[index_t(tc,id)]==0){
							teach_2q2_day[index_t(tc, id)]=day_simulation;
							teach_2q2_m[index_t(tc, id)]=min;
								
							teach_2q2[index_t(tc, id)]=1;
							teach_rep[index_t(tc,id)]=1;
							quarantine_t[index_t(tc, id)]=1;
						}else{
							status[index]=0;
							teach_rep[index_t(tc,id)]=1;
						}
						//fprintf(stderr, "teach replac: %d\t day%d\t m%d\t s0%d\n", id, q2[index_t(tc, id)], q2_m[index_t(tc, id)], s_teach);
						// s_teach original status of the original teacher
						//fprintf(fp6, "%d\t %d\t %d\t %d\t %d\t %d\n", nsim, id, tc->clas[index_t(tc, id)], day_simulation, s_teach, reason);			
						test_p++;
					}else{
						status[index]=5;
						teach_2q2[index_t(tc, id)]=0;	
						test_n++;
					}	
				}else if (s_teach==8){
					random=gsl_ran_flat(RNG,0,1);
					teta=theta_rp[counter_rp[index]];	
					if(random<teta){
					
						// Teacher is detected as positive and rplace with a susceptible one
						status[index]=0;
						fprintf(stderr, "index substatus: %d\t theta: %f\t", counter_rp[index], teta);
						if (teach_rep[index_t(tc,id)]==0){
							teach_2q2_day[index_t(tc, id)]=day_simulation;
							teach_2q2_m[index_t(tc, id)]=min;
								
							teach_2q2[index_t(tc, id)]=1;
							teach_rep[index_t(tc,id)]=1;
							quarantine_t[index_t(tc, id)]=1;
						}else{
							status[index]=0;
							teach_rep[index_t(tc,id)]=1;
						}
						
						test_p++;
						//fprintf(stderr, "teach replac: %d\t day%d\t m%d\t s0%d\n", id, teach_2q2_day[index_t(tc, id)], teach_2q2_m[index_t(tc, id)], s_teach, reason);
						
						// s_teach original status of the original teacher
						//fprintf(fp6, "%d\t %d\t %d\t %d\t %d\t %d\n", nsim, id, tc->clas[index_t(tc, id)], day_simulation, s_teach, reason);
					
					} else{
						status[index]=8;
						teach_2q2[index_t(tc, id)]=0;	
						test_n++;
					}
					
				}
			}
		}
		
		if  (test_p > 0){
			result=1;
		}else{
			result=0;
		}
		
		return result;
			
}
	

int intro_moment(int n, int *v1, int *v2, int *start, int *end, int day){

	int ni, a, b, r_day, r_quarter, bern, m, i, j;
	float prob;
	int num_15min_day=96;
	int week=7;
	
	
	prob = 0.5;
	
       	for (ni=0; ni<n; ni++){
  		r_day=(int)gsl_ran_flat(RNG,1,week+1);
  		
  		//fprintf(stderr, "r%d\n", r_day);
  		// If r_day is a school day, the transmission by community occurs in the time interval 
  		// different from school schedule
  		if ((r_day!=6) & (r_day!=7) & (r_day!=3)){
  			// I pick a number to decide when the transition happens
  			// a end of the school, b midnight
  			// a midnight, b before school
  			bern=(int) gsl_ran_bernoulli(RNG,prob);
  			if (bern==1){
  				a=end[day];
  				b=num_15min_day;
  			} else if (bern==0){
  				a=0;
  				b=start[day];
  			}
  			r_quarter=(int)gsl_ran_flat(RNG,a,b);
  			v1[ni]=r_day;
  			v2[ni]=r_quarter;
  		} else {
  			a=0;
  			b=num_15min_day;
  			r_quarter=(int)gsl_ran_flat(RNG,a,b);
  			v1[ni]=r_day;
  			v2[ni]=r_quarter;
  		
  		}
  	fprintf(stderr, "number intro: %d\t day intro: %d\t quarter intro: %d\n", n, v1[ni], v2[ni]);		
  	}	
  	
  	
  	m=0;
  	for (i=0; i<n; i++){
  		for(j=0; j<i; j++){
  			if ((v1[i]==v1[j]) & (v2[i]==v2[j])){
  				m++;
  			}
  		}
  	}
  	
  	fprintf(stderr, "double presence %d\n", m);
  	return m;

}




void count_inf(metadata *data, Teach *tc, int sim, int day, int index, int *isoclass, int *status0, FILE *fp9, FILE *fp10){

int i, t, n, nn, m;
int classroom, teach;
		
		
 		teach=find(tc, data->ID[index], data->classes[index]);
 		classroom = tc->clas[index_t(tc, teach)];
	
		m=0;
		
		fprintf(stderr, "Class with a CONFIRMED CASE in Q2 %d\n", classroom);
		
		// m count active individuals in the class of the detected cases
		for (t=0; t<tc->nc; t++){
			if (tc->clas[t]==classroom){
				// Check the status of the teacher of the class with detected cases
				if ((status0[index_nodes(data, tc->ID[t])]==1) |
				(status0[index_nodes(data, tc->ID[t])]==2) | 
				(status0[index_nodes(data, tc->ID[t])]==4) |
				(status0[index_nodes(data, tc->ID[t])]==5)){
					m++;
				}
				// Check the status of the children of the class with detected cases
				for (i=0; i<data->n; i++){
					if (data->classes[i]==tc->clas[t]){
						if((status0[i]==1) | (status0[i]==2) | (status0[i]==4) | (status0[i]==5)){
 		    					m++;
 		    				}
					}
				}
			//fprintf(stderr, "%d\t %d\t %d\t %d\n", sim, day, classroom, m);
			}
		}	
		
		// n count the number number of individual per class
		// nn count the number of additional active classes
		nn=0;
		for (t=0; t<tc->nc; t++){
			// We count of the number of remaing class with active cases that are not in isolation Q3
			if ((tc->clas[t]!=classroom) & (isoclass[t]!=1)){
				n=0;
				// Check on teacher
				if ((status0[index_nodes(data, tc->ID[t])]==1) | 
				(status0[index_nodes(data, tc->ID[t])]==2) | 
				(status0[index_nodes(data, tc->ID[t])]==4) |
				(status0[index_nodes(data, tc->ID[t])]==5)){
					n++;
				}
				// Check on students
				for (i=0; i<data->n; i++){
					if (data->classes[i]==tc->clas[t]){
						//id=data->ID[i];
						if((status0[i]==1) | (status0[i]==2) | (status0[i]==4) | (status0[i]==5)){
 		    					n++;
 		    				}
					}
				}
				
				fprintf(fp9, "%d\t %d\t %d\t %d\t %d\t %d\n", sim, day, classroom, m, tc->clas[t], n);
				
				if (n>0)
					nn++;
			
			} 		
		}
		
		//fprintf(stderr, "%d\t %d\t %d\t %d\t %d\n", sim, day, classroom, m, nn);
		fprintf(fp10, "%d\t %d\t %d\t %d\t %d\n", sim, day, classroom, m, nn);
		
	
	    						
}

void regular_screening(metadata *data, Param *param, Teach *tc, int *status0, int *teach_rep, int *teach_rtested, int *pre_flag, int *quarantine, int *detected, int *screenstatus, int *comp_id, int *start_q2, int *start_rq2, int quarter, int day, int sim, float **theta_ps, float **theta_cs, float **theta_scs, float **theta_rps, int *counter_prodro, int *counter_infect, int *counter_rp, int *num_pos_class){

int i, s0, N_negative, N_negative_s, N_negative_t, N_negative_r, N_positive, N_tot, N_positive_s, N_positive_t, N_positive_r;
int role, result, a, j, jk1, N_pos_all;
float rand;

	//N_fnegative=0;
	N_negative=0;
	N_negative_s=0;
	N_negative_t=0;
	N_negative_r=0;
	
	N_positive=0;
	N_positive_s=0;
	N_positive_t=0;
	N_positive_r=0;
	
	N_tot=0;
	N_pos_all=0;
	jk1=0;
	fprintf(stderr, "REGULAR day %d\n", day);
	float teta;
	
	for (j=0; j<tc->nc; j++){
		num_pos_class[index_class(tc,tc->clas[j])]=0;
	}
	
	for (i=0; i<data->n; i++){
		if (data->classes[i]!=6){
			a=0;
		}else{
			a=1;
		}
		if(comp_id[i]==1){
		//if((comp_id[i]==1) & (quarantine[i]==0) & (detected[i]==0) & (pre_flag[i]==0)  & (screenstatus[i]!=6) ){
			s0=status0[i];
			N_tot++;
			jk1++;
			if ((s0==0) | (s0==9) | (s0==1) | (s0==12)){
				//N_tot++;
				N_negative++;
				result=0;
				if (data->classes[i]!=6){
					N_negative_s++;
					role=1;
					//fprintf(fp13, " %d\t %d\t %d\t %d\t %d\n", sim, day, data->ID[i], role, result);
				}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]==0)){
					N_negative_t++;
					role=2;
					
				}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]>=1)){
					N_negative_r++;
					role=3;
					
				}
			
			}else if(s0==2){
				N_pos_all++;
				//N_tot++;
				rand=gsl_ran_flat(RNG,0,1);
				teta = theta_ps[a][counter_prodro[i]];
				if (rand<teta){
				
					screenstatus[i]=6;
					
					N_positive++;
					
					
					
					result=1;
					// If teacher is positive/ he/she is replaced with a susceptible
					
					if (data->classes[i]!=6){
						N_positive_s++;
						
						num_pos_class[index_class(tc, data->classes[i])]=num_pos_class[index_class(tc, data->classes[i])]+1;
						
						start_q2[i]=quarter+param->tau_test/param->deltat;
						role=1;
						//fprintf(fp13, "%d\t %d\t %d\t %d\t %d\n", sim, day, data->ID[i], role, s0);
						
						fprintf(stderr, "\n TESTED POSITIVE THROUGH REGULAR TESTING\n");
						fprintf(stderr, "day %d\t quarter %d\t id pos to screen %d\t s0 %d\t screenstatus %d\t start_q2 %d\n", day, quarter, data->ID[i], s0, screenstatus[i], start_q2[i]);
						
					}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]==0)){
						N_positive_t++;
						start_q2[i]=quarter+param->tau_test/param->deltat;
						teach_rtested[index_t(tc, data->ID[i])]=1;
						
						role=2;
						//fprintf(fp13, "%d\t %d\t %d\t %d\t %d\n", sim, day, data->ID[i], role, s0);
						
						fprintf(stderr, "\n TESTED POSITIVE THROUGH REGULAR TESTING\n");
						fprintf(stderr, "day %d\t quarter %d\t id pos to screen %d\t s0 %d\t screenstatus %d\t start_q2 %d\n", day, quarter, data->ID[i], s0, screenstatus[i], start_q2[i]);
						
						
					// If the teacher has been already replace but also the treplacement is positive
					}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]>=1)){
						N_positive_r++;
						start_rq2[i]=quarter+param->tau_test/param->deltat;
						teach_rtested[index_t(tc, data->ID[i])]=2;
						role=3;
						//fprintf(fp13, "%d\t %d\t %d\t %d\t %d\n", sim, day, data->ID[i], role, s0);
						
						fprintf(stderr, "\n---------------------\n");
						
						fprintf(stderr, "\n TESTED POSITIVE THROUGH REGULAR TESTING\n");
						fprintf(stderr, "day %d\t quarter %d\n replaced teacher pos to screen %d\t s0 %d\t screenstatus %d\t start_rq2 %d\t flag %d\n", day, quarter, data->ID[i], s0, screenstatus[i], start_rq2[i], teach_rtested[index_t(tc, data->ID[i])]);
						fprintf(stderr, "\n---------------------\n");
						
					}
				}else{
					N_negative++;
					result=0;
					if (data->classes[i]!=6){
						N_negative_s++;
						role=1;
						
					}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]==0)){
						N_negative_t++;
						role=2;
						
					}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]>=1)){
						N_negative_r++;
						role=3;
						
					}
				}
			}else if ((s0==4) | (s0==5)){
				//N_tot++;
				N_pos_all++;
				rand=gsl_ran_flat(RNG,0,1);
				
				if(s0==4){
					teta = theta_cs[a][counter_infect[i]];
				}else if(s0==5){
					teta = theta_scs[a][counter_infect[i]];
				}
				//fprintf(stderr, "teta used in regular screening %f\t for status %d\n", teta, s0);
				
				if (rand<teta){
				
					screenstatus[i]=6;
					
					N_positive++;
					result=1;
					// If teacher is positive/ he/she is replaced with a susceptible
					
					if (data->classes[i]!=6){
						N_positive_s++;
						start_q2[i]=quarter+param->tau_test/param->deltat;
						
						num_pos_class[index_class(tc, data->classes[i])]=num_pos_class[index_class(tc, data->classes[i])]+1;
						role=1;
						//fprintf(fp13, "%d\t %d\t %d\t %d\t %d\n", sim, day, data->ID[i], role, s0);
						
						fprintf(stderr, "\n TESTED POSITIVE THROUGH REGULAR TESTING\n");
						fprintf(stderr, "REGULAR day %d\t quarter %d\t id pos to screen %d\t s0 %d\t screenstatus %d\t start_q2 %d\n", day, quarter, data->ID[i], s0, screenstatus[i], start_q2[i]);
						
					}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]==0)){
						N_positive_t++;
						start_q2[i]=quarter+param->tau_test/param->deltat;
						teach_rtested[index_t(tc, data->ID[i])]=1;
						
						role=2;
						//fprintf(fp13, "%d\t %d\t %d\t %d\t %d\n", sim, day, data->ID[i], role, s0);
						
						fprintf(stderr, "\n TESTED POSITIVE THROUGH REGULAR TESTING\n");
						fprintf(stderr, "REGULAR day %d\t quarter %d\t id pos to screen %d\t s0 %d\t screenstatus %d\t start_q2 %d\n", day, quarter, data->ID[i], s0, screenstatus[i], start_q2[i]);
						
					// If the teacher has been already replace but also the treplacement is positive
					}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]>=1)){
						N_positive_r++;
						start_rq2[i]=quarter+param->tau_test/param->deltat;
						teach_rtested[index_t(tc, data->ID[i])]=2;
						
						role=3;
						//fprintf(fp13, "%d\t %d\t %d\t %d\t %d\n", sim, day, data->ID[i], role, s0);
						
						fprintf(stderr, "\n---------------------\n");
						
						fprintf(stderr, "\n TESTED POSITIVE THROUGH REGULAR TESTING\n");
						fprintf(stderr, "day %d\t quarter %d\n replaced teacher pos to screen %d\t s0 %d\t screenstatus %d\t start_rq2 %d\t flag %d\n", day, quarter, data->ID[i], s0, screenstatus[i], start_rq2[i], teach_rtested[index_t(tc, data->ID[i])]);
						fprintf(stderr, "\n---------------------\n");
					}
				}else{
					N_negative++;
					result=0;
					if (data->classes[i]!=6){
						N_negative_s++;
						role=1;
						
					}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]==0)){
						N_negative_t++;
						role=2;
						
					}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]>=1)){
						N_negative_r++;
						role=3;
						
					}
				}
			
			}else if(s0==8){
				
				
				//N_tot++;
				N_pos_all++;
				rand=gsl_ran_flat(RNG,0,1);
				teta = theta_rps[a][counter_rp[i]];
				
				fprintf(stderr, "\n&&&&&&&&&&&&&&&\n");
				fprintf(stderr, "rand %f theta %f role %d index %d stage rp %d\n", rand, teta, a, i,  counter_rp[i]);
				fprintf(stderr, "\n&&&&&&&&&&&&&&&\n");
				
				if (rand<teta){
				
					screenstatus[i]=6;
					
					N_positive++;
					
					
					
					result=1;
					// If teacher is positive/ he/she is replaced with a susceptible
					
					if (data->classes[i]!=6){
						N_positive_s++;
						
						num_pos_class[index_class(tc, data->classes[i])]=num_pos_class[index_class(tc, data->classes[i])]+1;
						
						start_q2[i]=quarter+param->tau_test/param->deltat;
						role=1;
						//fprintf(fp13, "%d\t %d\t %d\t %d\t %d\n", sim, day, data->ID[i], role, s0);
						
						fprintf(stderr, "\n TESTED POSITIVE THROUGH REGULAR TESTING\n");
						fprintf(stderr, "day %d\t quarter %d\t id pos to screen %d\t s0 %d\t screenstatus %d\t start_q2 %d\n", day, quarter, data->ID[i], s0, screenstatus[i], start_q2[i]);
						
						
						
					}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]==0)){
						N_positive_t++;
						start_q2[i]=quarter+param->tau_test/param->deltat;
						teach_rtested[index_t(tc, data->ID[i])]=1;
						
						role=2;
						//fprintf(fp13, "%d\t %d\t %d\t %d\t %d\n", sim, day, data->ID[i], role, s0);
						
						fprintf(stderr, "\n TESTED POSITIVE THROUGH REGULAR TESTING\n");
						fprintf(stderr, "day %d\t quarter %d\t id pos to screen %d\t s0 %d\t screenstatus %d\t start_q2 %d\n", day, quarter, data->ID[i], s0, screenstatus[i], start_q2[i]);
						
						
					// If the teacher has been already replace but also the treplacement is positive
					}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]>=1)){
						N_positive_r++;
						start_rq2[i]=quarter+param->tau_test/param->deltat;
						teach_rtested[index_t(tc, data->ID[i])]=2;
						role=3;
						//fprintf(fp13, "%d\t %d\t %d\t %d\t %d\n", sim, day, data->ID[i], role, s0);
						
						fprintf(stderr, "\n---------------------\n");
						
						fprintf(stderr, "\n TESTED POSITIVE THROUGH REGULAR TESTING\n");
						fprintf(stderr, "day %d\t quarter %d\n replaced teacher pos to screen %d\t s0 %d\t screenstatus %d\t start_rq2 %d\t flag %d\n", day, quarter, data->ID[i], s0, screenstatus[i], start_rq2[i], teach_rtested[index_t(tc, data->ID[i])]);
						fprintf(stderr, "\n---------------------\n");
						
					}
				}else{
					N_negative++;
					result=0;
					if (data->classes[i]!=6){
						N_negative_s++;
						role=1;
						
					}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]==0)){
						N_negative_t++;
						role=2;
						
					}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]>=1)){
						N_negative_r++;
						role=3;
						
					}
				}
			}
		}
	}
	
	
	fprintf(stderr, "jk1 %d\t N pos all %d\n", jk1, N_pos_all);
	//fprintf(fp12, "%d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\n", sim, day, N_tot, N_positive, N_positive_s, N_positive_t, N_positive_r, N_negative, N_negative_s, N_negative_t, N_negative_r);
	
	fprintf(stderr, "***\n");
	for (j=0; j<tc->nc; j++){
		fprintf(stderr, "class: %d\t num pos within the class:%d\n", tc->clas[j], num_pos_class[index_class(tc,tc->clas[j])]);
	}
	fprintf(stderr, "***\n");
	
}

void count_positive_st(metadata *data, int sim, int day_simulation, int *counter_prodro, int *counter_infect, int *counter_rp, int *status0, int *detected, int *quarantine, int *pre_flag, int *num_iso, FILE *fp18){
	
	int jk, jk1, others;
	int N_ips1, N_ips11, N_ips12, N_ips13, N_ips14;
	int N_ics1, N_ics11, N_ics12, N_ics13, N_ics14, N_ics15, N_ics16;
	int N_iscs1, N_iscs11, N_iscs12, N_iscs13, N_iscs14, N_iscs15, N_iscs16;
	int N_rps1, N_rps11, N_rps12, N_rps13, N_rps14;
	
	
	N_ips1 = 0; N_ips11 = 0; N_ips12 = 0; N_ips13 = 0; N_ips14 = 0;
	N_ics1 = 0; N_ics11 = 0; N_ics12 = 0; N_ics13 = 0; N_ics14 = 0; N_ics15 = 0; N_ics16 = 0;
	N_iscs1 = 0; N_iscs11 = 0; N_iscs12 = 0; N_iscs13 = 0; N_iscs14 = 0; N_iscs15 = 0; N_iscs16 = 0;
	N_rps1 = 0; N_rps11 = 0; N_rps12 = 0; N_rps13 = 0; N_rps14 = 0;
	jk1=0; others=0;
	
	for (jk=0; jk<data->n; jk++){
 		if ((data->classes[jk]!=6) & (quarantine[jk]==0) & (detected[jk]==0) & (pre_flag[jk]==0) & (num_iso[jk]==0)){
 			jk1++;	
			if(status0[jk]==2){
				N_ips1++;
				if (counter_prodro[jk]==0){
					N_ips11++;
				}else if (counter_prodro[jk]==1){
					N_ips12++;
				}else if (counter_prodro[jk]==2){
					N_ips13++;
				}else if (counter_prodro[jk]==3){
					N_ips14++;
				}
			}else if(status0[jk]==4){
				N_ics1++;
				if (counter_infect[jk]==0){
					N_ics11++;
				}else if (counter_infect[jk]==1){
					N_ics12++;
				}else if (counter_infect[jk]==2){
					N_ics13++;
				}else if (counter_infect[jk]==3){
					N_ics14++;
				}else if (counter_infect[jk]==4){
					N_ics15++;
				}else if (counter_infect[jk]==5){
					N_ics16++;
				}
			}else if(status0[jk]==5){
				N_iscs1++;
				if (counter_infect[jk]==0){
					N_iscs11++;
				}else if (counter_infect[jk]==1){
					N_iscs12++;
				}else if (counter_infect[jk]==2){
					N_iscs13++;
				}else if (counter_infect[jk]==3){
					N_iscs14++;
				}else if (counter_infect[jk]==4){
					N_iscs15++;
				}else if (counter_infect[jk]==5){
					N_iscs16++;
				}
			}else if(status0[jk]==8){
				N_rps1++;
				if (counter_rp[jk]==0){
					N_rps11++;
				}else if (counter_rp[jk]==1){
					N_rps12++;
				}else if (counter_rp[jk]==2){
					N_rps13++;
				}else if (counter_rp[jk]==3){
					N_rps14++;
				}
			}else{
				others++;
			}
		}
	}
	
	
	fprintf(fp18, "%d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\n", sim, day_simulation, jk1, N_ips11, N_ips12, N_ips13, N_ips14, N_ics11, N_ics12, N_ics13, N_ics14, N_ics15, N_ics16, N_iscs11, N_iscs12, N_iscs13, N_iscs14, N_iscs15, N_iscs16, N_rps11, N_rps12, N_rps13, N_rps14);

}


void reactive_screening(metadata *data, Teach *tc, Param *param, float **theta_ps, float **theta_cs, float **theta_scs, float **theta_rps, int id, int day_simulation, int quarter, int sim, int *status0, int *screenstatus, int *start_q2, int *quarantine, int *detected, int *classtotest, int *indtotest, int *when, int *counter_prodro, int *counter_infect, int *counter_rp, FILE *fp16, FILE *fp18){
	
	
	int *index_class, *id_class, dly, a;
	int class, teacher, cl, n, t, index_teacher, d, n_class, n_test_st, n_test_tc;
	
	n=0;
	
	n_class=0;
	n_test_st=0;
	n_test_tc=0;
	
	class = data->classes[index_nodes(data, id)];
	index_class = (int*)calloc(1, sizeof(int));
	id_class = (int*)calloc(1, sizeof(int));
	
	//int week;
	//week = 7;
	
	//week_day = day_simulation % week + 1;
	
	fprintf(stderr, "INSIDE REACTIVE\n");
	
	
	
	if (class!=6){
		a=0;
		teacher = find(tc, id, class);
		index_teacher = index_nodes(data, teacher);
		
		
		n_class++;
		
		fprintf(stderr, "cl %d\t id %d\t index %d\t role: %d\n", class, teacher, index_teacher, data->classes[index_nodes(data, teacher)]);
		
		classtotest[index_t(tc, teacher)]=1;
		
		if (param->delay == 2){
			
			when[index_t(tc, teacher)] = quarter + param->delay * 1440/param->deltat;
			dly=param->delay;
			
		}
		
		
		
		fprintf(stderr, "CLASS TO TEST: %d\t index class %d\t y? %d\t when? %d\t %d\n", class, index_t(tc, teacher), classtotest[index_t(tc, teacher)], day_simulation+dly, when[index_t(tc, teacher)]);
		
		for (d = 0; d < data->n; d++){
			if ( (data->classes[d] == class) & (id != data->ID[d])){
				index_class[n] = d;
				id_class[n] = data->ID[d];
				id_class = (int*)realloc(id_class,(n+2)*sizeof(int));
				index_class = (int*)realloc(index_class,(n+2)*sizeof(int));
				n++;
				
				//fprintf(stderr, "cl %d\t id %d\t index %d\t role: %d\n", data->classes[d], data->ID[d], d, data->classes[d]);
			}
			
			if (data->classes[d] == class){
				n_class++;
			}
		}
		
		fprintf(stderr, "day: %d\t symp. id: %d\t class to screen: %d\t dim class no vax: %d\n", day_simulation, id, class, n);
		//fprintf(stderr, "n vax in the class: %d\n", n_vax);
		//fprintf(stderr, "dim tot class: %d\n", n_class);
		cl=class;
		
	}else if (class == 6){
		a=1;
		teacher = id;
		index_teacher = index_nodes(data, teacher);
		n_class++;
		
		
		for (t=0; t< tc->nc; t++){
			if (tc->ID[t] == teacher){
				cl = tc->clas[t];
				n_class++;
			}
		}
		
		fprintf(stderr, "cl %d\t id %d\t index %d\t role: %d\n", cl, teacher, index_teacher, class);
		
		classtotest[index_t(tc, teacher)]=1;
		
		if (param->delay == 2){
			
			when[index_t(tc, teacher)] = quarter + param->delay * 1440/param->deltat;
			dly=param->delay;
			
		}
		
		
		fprintf(stderr, "CLASS TO TEST: %d\t index class %d\t y? %d\t when? %d\t %d\n", class, index_t(tc, teacher), classtotest[index_t(tc, teacher)], day_simulation+dly, when[index_t(tc, teacher)]);
		
		//exit(0);
		
		for (d = 0; d < data->n; d++){
			if ( data->classes[d] == cl){
				index_class[n] = d;
				id_class[n] = data->ID[d];
				id_class = (int*)realloc(id_class,(n+2)*sizeof(int));
				index_class = (int*)realloc(index_class,(n+2)*sizeof(int));
				n++;
				
				//fprintf(stderr, "cl %d\t id %d\t index %d\t role: %d\n", data->classes[d], data->ID[d], d, data->classes[d]);
			}
			
			if (data->classes[d] == cl){
				n_class++;
			}
		}
		
		fprintf(stderr, "day: %d\t symp. id: %d\t class to screen: %d\t dim class all partecipants: %d\n", day_simulation, id, cl, n);
		//fprintf(stderr, "n vax in the class: %d\n", n_vax);
		//fprintf(stderr, "dim tot class: %d\n", n_class);
	}
	
	
	int num_to_test;
	num_to_test = param->alpha * n;
	
	fprintf(stderr, "people to test (all partecipants): %d\n", num_to_test);
	
	myshuffle(index_class, n);
		
	float teta, rand;
	int n_pos, j;
	
	n_pos=0;
	
	for (j=0; j<num_to_test; j++){
		
		if ((quarantine[index_nodes(data, id_class[index_class[j]])] == 0) & 
		(detected[index_nodes(data, id_class[index_class[j]])] == 0)){
		
			if (data->classes[index_nodes(data, id_class[index_class[j]])] == 6){
				a=1;
				n_test_tc++;
			}else{
				a=0;
				n_test_st++;
			}
			
			
			
			fprintf(stderr, "id %d\t index %d\t status0 %d\n", data->ID[index_nodes(data, id_class[index_class[j]])], index_nodes(data, id_class[index_class[j]]), status0[index_nodes(data, id_class[index_class[j]])]);
			
			// flag on ids to test some days later a reactive
			indtotest[index_nodes(data, id_class[index_class[j]])]=1;
			
			if (status0[index_nodes(data, id_class[index_class[j]])] == 2){
				teta=theta_ps[a][counter_prodro[j]];
			}else if (status0[index_nodes(data, id_class[index_class[j]])] == 4){
				teta=theta_cs[a][counter_infect[j]];
			}else if (status0[index_nodes(data, id_class[index_class[j]])] == 5){
				teta=theta_scs[a][counter_infect[j]];
                         }else if (status0[index_nodes(data, id_class[index_class[j]])] == 8){
				teta=theta_rps[a][counter_rp[j]];
			}else{
				teta=0;
			}
			
			
			
			rand=gsl_ran_flat(RNG,0,1);
			
			if (rand<teta){
			
				fprintf(stderr, "***************\n");
				
				screenstatus[index_nodes(data, id_class[index_class[j]])]=6;
				
				start_q2[index_nodes(data, id_class[index_class[j]])]=quarter+param->tau_test/param->deltat;
				
				//quarantine[index_nodes(data, id_class[index_class[j]])]=1;
				
				fprintf(stderr, "id %d\t index %d\t status0 %d\t screen status %d\t start q2 %d\n", data->ID[index_nodes(data, id_class[index_class[j]])], index_nodes(data, id_class[index_class[j]]), status0[index_nodes(data, id_class[index_class[j]])], screenstatus[index_nodes(data, id_class[index_class[j]])], start_q2[index_nodes(data, id_class[index_class[j]])]);
				n_pos++;
				
				fprintf(stderr, "***************\n");	
			}
		}
		
	}
	
	
	
	fprintf(fp16, "%d\t %d\t %d\t %d\t %d\n", sim, day_simulation, id, cl, n_pos);
	fprintf(fp18, "%d\t %d\t %d\t %d\n", sim, day_simulation, n_test_tc, n_test_st);
	fprintf(stderr, "%d\t %d\t %d\t %d\t %d\n", sim, day_simulation, n_test_tc, n_test_st, num_to_test);
	free(index_class);
	free(id_class);
	
	//exit(0);
}

void post_reactive(metadata *data, Teach *tc, Param *param, int clas, int day_simulation, int quarter, int sim, int *status0, int *screenstatus, int *start_q2, int *start_rq2, int *teach_rtested, int *teach_rep, int *quarantine, int *detected, int *indtotest, float **theta_ps, float **theta_cs, float **theta_scs, float **theta_rps, int *counter_prodro, int *counter_infect, int *counter_rp, FILE *fp17, FILE *fp19){

	int j, n_pos, a;
	int n_test_tc, n_test_st;
	n_test_tc=0;
	n_test_st=0;
	n_pos=0;
	
	float teta, rand;
	
	fprintf(stderr, "\n **************** \n");
	fprintf(stderr, "INSIDE POST REACTIVE day %d\n", day_simulation);

	
	
	for (j=0; j<data->n; j++){
		if ((data->classes[j]!=6) & (data->classes[j]==clas)){
			a=0;
			if ((indtotest[j]==1) & (quarantine[j]==0) & (detected[j]==0)){
				n_test_st++;
				
				
				fprintf(stderr, "id %d\t index %d\t status0 %d\t screen status %d\t start q2 %d\n", data->ID[j], j,  status0[j], screenstatus[j], start_q2[j]);
				
				//indtotest[j]=0;
				
				if (status0[j] == 2){
					teta=theta_ps[a][counter_prodro[j]];
				}else if (status0[j] == 4){
					teta=theta_cs[a][counter_infect[j]];
				}else if (status0[j] == 5){
					teta=theta_scs[a][counter_infect[j]];
                                  }else if (status0[j] == 8){
					teta=theta_rps[a][counter_rp[j]];
				}else{
					teta=0;
				}
				
				rand=gsl_ran_flat(RNG,0,1);
				
				if (rand<teta){
				
					fprintf(stderr, "***************\n");
					
					screenstatus[j]=6;
					
					if (data->classes[j]!=6){
						start_q2[j]=quarter + param->tau_test/param->deltat;
						
					}else if ((data->classes[j]==6) & (teach_rep[index_t(tc, data->ID[j])]==0)){
						start_q2[j]=quarter+param->tau_test/param->deltat;
						
					}else if ((data->classes[j]==6) & (teach_rep[index_t(tc, data->ID[j])]!=0)){
						start_rq2[j]=quarter +param->tau_test/param->deltat;
						teach_rtested[index_t(tc, data->ID[j])]=2;
						
					}
					
					fprintf(stderr, "id %d\t index %d\t status0 %d\t screen status %d\t start q2 %d\n", data->ID[j], j,  status0[j], screenstatus[j], start_q2[j]);
					n_pos++;
					
					fprintf(stderr, "***************\n");	
				}
			}
		}else if ((data->classes[j]==6) && (tc->clas[index_t(tc, data->ID[j])]==clas)){
			a=1;
			if ((indtotest[j]==1) & (quarantine[j]==0) & (detected[j]==0)){
				n_test_tc++;
				
			
				fprintf(stderr, "id %d\t index %d\t status0 %d\t screen status %d\t start q2 %d\n", data->ID[j], j,  status0[j], screenstatus[j], start_q2[j]);
				
				indtotest[j]=0;
				
				if (status0[j] == 2){
					teta=theta_ps[a][counter_prodro[j]];
				}else if (status0[j] == 4){
					teta=theta_cs[a][counter_infect[j]];
				}else if (status0[j] == 5){
					teta=theta_scs[a][counter_infect[j]];
				}else if (status0[j] == 8){
					teta=theta_rps[a][counter_rp[j]];
				}
				
				rand=gsl_ran_flat(RNG,0,1);
				
				if (rand<teta){
				
					fprintf(stderr, "***************\n");
					
					screenstatus[j]=6;
					
					if ((data->classes[j]==6) & (teach_rep[index_t(tc, data->ID[j])]==0)){
						start_q2[j]=quarter+param->tau_test/param->deltat;
						
					}else if ((data->classes[j]==6) & (teach_rep[index_t(tc, data->ID[j])]!=0)){
						start_rq2[j]=quarter+param->tau_test/param->deltat;
						teach_rtested[index_t(tc, data->ID[j])]=2;
						
					}
					
					fprintf(stderr, "id %d\t index %d\t status0 %d\t screen status %d\t start q2 %d\n", data->ID[j], j,  status0[j], screenstatus[j], start_q2[j]);
					n_pos++;
					
					fprintf(stderr, "***************\n");	
				}
			}
		}
		
	}
	
	fprintf(stderr, "\n **************** \n");
	
	fprintf(fp17, "%d\t %d\t %d\t %d\n", sim, day_simulation, clas, n_pos);
	fprintf(fp19, "%d\t %d\t %d\t %d\n", sim, day_simulation, n_test_tc, n_test_st);
	fprintf(stderr, "\n");
	//exit(0);

}

void assign_compliance_st(metadata *data, Teach *tc, int *comp_id, int *quarantine, int *detected, int *num_iso, int *pre_flag, float percentage, int regular){
	
	// Assign compliance only to epople at school
	int i, j, k, k_s, compliant, a;
	float p;
	
	p = percentage;
	k=0;
	k_s=0;
	compliant = 0;
	for(j=0; j<data->n; j++){
		comp_id[j]=0;
	}
	int absentes=0;
	int quarantin, detect, pst_iso, preiso;
	
	quarantin=0; 
	detect=0;
	pst_iso=0; 
	preiso=0;
	
	for (a=0; a<data->n; a++){
		if ((quarantine[a]==1) & (detected[a]==0) & (pre_flag[a]==0) & (data->classes[a]!=6)){
			absentes++;
			quarantin++;
		}
		if ((quarantine[a]==0) & (detected[a]==1) & (num_iso[a]==0) & (pre_flag[a]==0) & (data->classes[a]!=6)){
			absentes++;
			detect++;
		}
		if ((quarantine[a]==0) & (detected[a]==0) & (num_iso[a]==1) & (pre_flag[a]==0) & (data->classes[a]!=6)){
			absentes++;
			pst_iso++;
		}
		if ((quarantine[a]==0) & (detected[a]==0) & (pre_flag[a]==1) & (data->classes[a]!=6)){
			absentes++;
			preiso++;
		}
	}
	
	fprintf(stderr, "quarantine %d\t detected %d\t pst_iso %d\t preiso %d\n", quarantin, detect, pst_iso, preiso);
	
	
	int  *ids_s, *ind_s;
	ids_s=(int*)calloc(data->n - tc->nc,sizeof(int));
	ind_s=(int*)calloc(data->n - tc->nc,sizeof(int));
	
	float nodes_notcompliant;
	nodes_notcompliant=((data->n)-(tc->nc)-absentes)*(1-p);
	//ind_nod=(int*)calloc(data->n,sizeof(int));
			
	fprintf(stderr, "number of NOT compliant nodes: %f\n", nodes_notcompliant);
	

	for (i=0; i<data->n; i++){
		if ((data->classes[i]!=6) & (pre_flag[i]==0) & (detected[i]==0) & (quarantine[i]==0) & (num_iso[i]==0)){	
			ids_s[k_s]=data->ID[i];
			//fprintf(stderr, "%d\t %d\n", ids[k], data->ID[i]);
			ind_s[k_s]=k_s;
			k_s++;
		}
		
	}
	
	//fprintf(stderr, "%d\t %d\n", k, tc->nc);
	myshuffle(ind_s, (data->n)-(tc->nc)-absentes);
	
	fprintf(stderr, "c %f\n", (data->n) - (tc->nc) - (nodes_notcompliant) - absentes);
	
	for (j=0; j< round((data->n) - (tc->nc) - (nodes_notcompliant) - absentes); j++){
		
		comp_id[index_nodes(data, ids_s[ind_s[j]])]=1;
		//fprintf(stderr, "ids %d\t type %d\n", data->ID[index_nodes(data, ids_s[ind_s[j]])], data->classes[index_nodes(data, ids_s[ind_s[j]])]);
		compliant++;
		if (data->classes[index_nodes(data, ids_s[ind_s[j]])]==6){
			fprintf(stderr, "ERROR IN COMPLiANCE STUDENTS\n");
			exit(0);
		}
		
		
	}
	free(ids_s);
	free(ind_s);
	
	fprintf(stderr, "present at school %d\t adhesion %f\t compliant %d\n", (data->n)-(tc->nc)-absentes, percentage, compliant);

	




}

float compute_sigma(int status, int is_teacher, int is_boosted, int is_2doses, int nweek_boosted, int nweek_2doses, Susst *sigma_st, Sustc *sigma_tc, float omicron_factor, int nw) {
	
	float sigma = 0.0;

	if (!is_teacher) {
		if (!is_boosted && !is_2doses) {
		sigma = (status == 12) ? sigma_st->novax[nw] * omicron_factor
					: sigma_st->novax[nw];
		} else if (is_2doses) {
		sigma = (status == 12) ? sigma_st->twodoses[nweek_2doses] * omicron_factor
						: sigma_st->twodoses[nweek_2doses];
		} else if (is_boosted) {
		sigma = (status == 12) ? sigma_st->booster[nweek_boosted] * omicron_factor
						: sigma_st->booster[nweek_boosted];
		}
	} else {
		if (!is_boosted && !is_2doses) {
		sigma = (status == 12) ? sigma_tc->novax[nw] * omicron_factor
						: sigma_tc->novax[nw];
		} else if (is_2doses) {
		sigma = (status == 12) ? sigma_tc->twodoses[nweek_2doses] * omicron_factor
						: sigma_tc->twodoses[nweek_2doses];
		} else if (is_boosted) {
		sigma = (status == 12) ? sigma_tc->booster[nweek_boosted] * omicron_factor
						: sigma_tc->booster[nweek_boosted];
		}
	}

	return sigma;
}

float compute_rbeta(int status, int is_teacher, int is_boosted, int is_2doses, int delta, int nweek_boosted, int nweek_2doses,  Trast *rbeta_st, Tratc *rbeta_tc, int nw) {
	float rbeta = 0.0;

	int is_p = (status == 2 || status == 5);  // if true, use *_p arrays


	if (!is_teacher) {
		if (!is_boosted && !is_2doses && (delta == 0)) {
			rbeta = is_p ? rbeta_st->novax_p[nw] : rbeta_st->novax_c[nw];
		} else if (is_2doses) {
			rbeta = is_p ? rbeta_st->twodoses_p[nweek_2doses] : rbeta_st->twodoses_c[nweek_2doses];
		} else if (is_boosted) {
			rbeta = is_p ? rbeta_st->booster_p[nweek_boosted] : rbeta_st->booster_c[nweek_boosted];
		}
	} else {
		if (!is_boosted && !is_2doses && (delta == 0)) {
			rbeta = is_p ? rbeta_tc->novax_p[nw] : rbeta_tc->novax_c[nw];
		} else if (is_2doses) {
			rbeta = is_p ? rbeta_tc->twodoses_p[nweek_2doses] : rbeta_tc->twodoses_c[nweek_2doses];
		} else if (is_boosted) {
			rbeta = is_p ? rbeta_tc->booster_p[nweek_boosted] : rbeta_tc->booster_c[nweek_boosted];
		}
	}



	return rbeta;
}

void tseir(metadata *data, contact *ct, Teach *tc, Param *param, Weeklyintro **introst,  Weeklyintro1 **introtc, Susst *sigma_st, Sustc *sigma_tc, Trast *rbeta_st, Tratc *rbeta_tc, Subst *psub_st, Subtc *psub_tc, int symptomatic, int preventive, int preventivegrade, int regular, int reactive, int **pre_time, int *pre_flag, int *exp_day, int *status0, int *status, int *comp_id, int *start, int *end, float *pd, int nr, int *students_tovax, int *teachers_toboost, int*vaccinated_2doses, int*vaccinated_booster, int index_seed, float **theta_ps, float **theta_cs, float **theta_scs, float **theta_rps, float *theta_p, float *theta_c, float *theta_sc, float *theta_rp, int *counter_latent, int *counter_prodro, int *counter_infect, int *counter_rp,  float *weekly_perc_adhesion, int ntimes, int nweek, int *r_delta, int *nweek_2doses, int *nweek_booster, FILE *fp5, FILE *fp16, FILE *fp17, FILE *fp18, FILE *fp19, FILE *fp20, FILE *fp21, FILE *fp22, FILE *fp23, FILE *fp24){

	// Counters
	int z, vc;
	int weekly_transmission_st, weekly_transmission_tc;
	int g, hh, hg, i, ii, j, jjj, jk, t, ni, ncl;
	//int n_smp_d=0;
	
	float weight_intro;
	int typ, reason, status_orig;
	// Day and quarter, conversion
	int day_simulation, d;
	int m, num_15min_day=4*24;
	
	int m_absolute;
	int week=7;
	
	// Ids and index
	int id,id1,id2;
	int index_id, index_id1,index_id2;
	int s_id,s_id1,s_id2, class_q3, teach_q3;
	int role_id, role_id1, role_id2;
	float weight;
	
	int n_susc_s,  n_susc_t;
	int new_exposed, new_pro, new_ic, new_is, new_q3, new_ic_d;
	int new_exp_s, new_exp_t, new_exp_r, new_pro_s, new_pro_t, new_pro_r, new_ic_s, new_ic_t, new_ic_r;
	int new_ic_d_s, new_ic_d_t, new_ic_d_r, new_is_s, new_is_t, new_is_r, new_q2_s, new_q2_t, new_pos_r;
	int new_neg_s, new_neg_t, new_neg_r, new_q2_reg, new_q2_regs, new_q2_regt, new_q2_regr;
	
	int *id_reactive, *m_reactive, *reactive_class;
	id_reactive=(int*)calloc(1, sizeof(int));
	m_reactive=(int*)calloc(1, sizeof(int));
	int *classtotest, *indtotest, *when;
	classtotest=(int*)calloc(tc->nc, sizeof(int));
	when=(int*)calloc(tc->nc, sizeof(int));
	indtotest=(int*)calloc(data->n, sizeof(int));
	reactive_class=(int*)calloc(tc->nc, sizeof(int));
	//int trigger_role;
	int n_reactive=0;
	
	//float den;
	double random, random1, random2, random3;
	long double random4; 
	
	// Weekly intro
	//int num_intro=0;
	int double_presence;
	int *introductions_days_st, *introductions_quarter_st;
	int *introductions_days_tc, *introductions_quarter_tc;
	
	int dim, r;
	int *teach_2iso, *teach_2isoday, *teach_2isom, *num_pos_class;
	teach_2iso=(int*)calloc(tc->nc, sizeof(int));
	teach_2isoday=(int*)calloc(tc->nc, sizeof(int));
	teach_2isom=(int*)calloc(tc->nc, sizeof(int));
	num_pos_class=(int*)calloc(tc->nc, sizeof(int));
	
	// I used the vectors below to save when the transitions in the detection path happened
	//int *q1_day,*q2_day, *tested_negative_day;
	int *q1_quarter, *q2_quarter;
	int *q1_id, *q1_result, *q2_id;
	
	int n_transitions1=0;
	int n_transitions2=0;
	
	q1_id=(int*)calloc(1, sizeof(int));
	q1_quarter=(int*)calloc(1, sizeof(int));
	q1_result=(int*)calloc(1, sizeof(int));
	
	q2_id=(int*)calloc(1, sizeof(int));
	q2_quarter=(int*)calloc(1, sizeof(int));

	int *teach_rep;
	int *class_iso, tea;

	teach_rep=(int*)calloc(tc->nc, sizeof(int));
	class_iso=(int*)calloc(tc->nc, sizeof(int));
    	
	int role, result;

	//if (regular==1)

	// I used the vectors below to save when the Q2 starts, the status captured from test and when it has been performed
	int *start_q2, *start_rq2;
	int *screenstatus, *teach_neg_reg, *teach_rtested, *num_iso;
	int n_reg=0;

	// Quarantine because of a regular
	start_q2=(int*)calloc(data->n, sizeof(int));
	start_rq2=(int*)calloc(data->n, sizeof(int));
	screenstatus=(int*)calloc(data->n, sizeof(int));
	num_iso=(int*)calloc(data->n, sizeof(int));
	teach_rtested=(int*)calloc(tc->nc, sizeof(int));
	teach_neg_reg=(int*)calloc(1, sizeof(int));

	// screenstatus and status initialization
	int s;
	for (s=0; s<data->n; s++){
		screenstatus[s]=0;
	num_iso[s]=0;
	}
    	
	int l;
	for (l=0; l<data->n; l++){
		pre_flag[l]=0;
		pre_time[0][l]=0;
		pre_time[1][l]=0;
		status[l]=0;
		exp_day[l]=0;
	
	}

	int n_test_t, n_test_t_p, n_test_t_n, n_test_symp, n_test_symp_p, n_test_symp_n, n_test_symp_st, n_test_symp_tc;
    	
    	
	int *detected, *quarantine, *quarantine_t, *detected_t;
	detected=(int*)calloc(data->n, sizeof(int));
	quarantine=(int*)calloc(data->n, sizeof(int));
	detected_t=(int*)calloc(tc->nc, sizeof(int));
	quarantine_t=(int*)calloc(tc->nc, sizeof(int));

	int trigger_role;
	// flag to know if the node has been identified or if it is in quarantine

	float sigma, rbeta;
	
	// LEGEND:
	// S=0, E=1, IP=2, IC=4, ISC=5, R+=8, R=9
	
	// S susceptible nodes
	// E exposed nodes
	// IP infectious prodromics node 
	// Q1 detected clincial infectious node = suspected COVID19 case
	// IC clinical infectious
	// ISC subclinical infectious node = asymp. and mild COVID19 cases
	// Q2 detected clinical infectious node in isolation (QUARANTINE)
	// ICDN suspected infectious clinical node tested negative  
	// R+ nodes still positive to PCR test but not infectious anymore
	// R removed  
	
	//days_simulation for which the simulation is iterated
	//tf last day simulation
	int zk;
	for (zk=0;zk<data->n;zk++){
		status[zk]=status0[zk];
	}
	
	int d1, d2, d3, d4, d5;
	d1=1;
	d2=2;
	d3=3;
	d4=4;
	d5=5;
	
	int day_screen=1;
	//int n_test_teach=0;
	int nw=0;	
   	for (day_simulation=0; day_simulation<param->tf; day_simulation++){
	
	    d=day_simulation % week + 1; 		
	  						
          					
            switch(d){
    		case 1:	
    			weekly_transmission_st = 0;
				weekly_transmission_tc = 0;
    			
    			new_exposed=0;
    			new_pro=0;
    			new_ic_d=0;
    			new_ic=0;
    			new_is=0;
    			new_q3=0;
    		
    			// New intro in each compartments divided by type: STUDENTS/ TEACHERS/ REPLACED TEACHERS
    			new_exp_s=0;new_exp_t=0;new_exp_r=0;
				new_pro_s=0;new_pro_t=0;new_pro_r=0;new_ic_s=0;
    			new_ic_t=0;new_ic_r=0;new_ic_d_s=0;new_ic_d_t=0;new_ic_d_r=0;
				new_is_s=0;new_is_t=0;new_is_r=0;
				new_q2_s=0;new_q2_t=0;new_pos_r=0;new_q2_reg=0;new_q2_regs=0;new_q2_regt=0;new_q2_regr=0;new_neg_s=0;new_neg_t=0;new_neg_r=0;
				n_test_symp=0;n_test_symp_p=0; n_test_symp_n=0;n_test_symp_st=0; n_test_symp_tc=0;
      			n_test_t=0;n_test_t_p=0;n_test_t_n=0;
    		
    		
  			
    			assign_vaccination(data, tc, nw, students_tovax[nw], teachers_toboost[nw], vaccinated_2doses,  vaccinated_booster, nweek_2doses, nweek_booster);
    			
    			
	  			if (round((*introst)[nr].weeks[nw])>0){
	  			
	  				introductions_days_st=(int*)calloc(round((*introst)[nr].weeks[nw]), sizeof(int));
					introductions_quarter_st=(int*)calloc(round((*introst)[nr].weeks[nw]), sizeof(int));
					
	  				fprintf(stderr, "W-INTRO DAY %d\n", day_simulation);
	  				double_presence=intro_moment(round((*introst)[nr].weeks[nw]), introductions_days_st, introductions_quarter_st, start, end, day_simulation);
	  			}
	  			
	  			if (round((*introtc)[nr].weeks[nw])>0){
	  			
	  				introductions_days_tc=(int*)calloc(round((*introtc)[nr].weeks[nw]), sizeof(int));
					introductions_quarter_tc=(int*)calloc(round((*introtc)[nr].weeks[nw]), sizeof(int));
					
	  				fprintf(stderr, "W-INTRO DAY %d\n", day_simulation);
	  				double_presence=intro_moment(round((*introtc)[nr].weeks[nw]), introductions_days_tc, introductions_quarter_tc, start, end, day_simulation);
	  			}
  			//}
  			
  			
  			//fprintf(stderr, "a\n");
	  			
    			for (m=0; m<num_15min_day; m++){
    			
    				m_absolute=num_15min_day*day_simulation + m; //progression of 15 minutes step from the first day to the last one
	        
	        		
					n_susc_s=0;
					n_susc_t=0;
      				// Intro of 1 case if there are still susceptible individuals
      				
      			 	for (i=0; i<data->n; i++){
      					if (((status0[i]==0) | (status0[i]==12)) & (data->ID[i]!=6)){
      						n_susc_s++;
      					}else if (((status0[i]==0) | (status0[i]==12)) & (data->ID[i]==6)){
	  						n_susc_t++;
	  					}
      				}

					if ((n_susc_s!=0) & ((*introst)[nr].weeks[nw]>0) & (n_susc_s>=(*introst)[nr].weeks[nw])){
      			
						for (ni=0; ni<(*introst)[nr].weeks[nw]; ni++){
							if (((day_simulation % (week) + 1)==introductions_days_st[ni]) & (m==introductions_quarter_st[ni])){
 
								typ=weekly_introduction_st(data, tc, status, status0, (*introst)[nr].weeks[nw], double_presence, teach_rep, exp_day, day_simulation);
									
								new_exposed++;
								
								if (typ==1){
									new_exp_s++;
								}	
							}
						}
					}
					if ((n_susc_t!=0) & ((*introtc)[nr].weeks[nw]>0) & (n_susc_t>=(*introtc)[nr].weeks[nw])){
					
						for (ni=0; ni<(*introtc)[nr].weeks[nw]; ni++){
							if (((day_simulation % (week) + 1)==introductions_days_tc[ni]) & (m==introductions_quarter_tc[ni])){
								  
								typ=weekly_introduction_tc(data, tc, status, status0, (*introtc)[nr].weeks[nw], double_presence, teach_rep, exp_day, day_simulation);
									
								new_exposed++;
								
								if (typ==2){
									new_exp_t++;
								}else if (typ==3){
									new_exp_r++;
								}		
							}
						}
					}
      			
      				//fprintf(stderr, "a\n");
      			
					if ((m>=start[day_simulation]) & (m<=end[day_simulation])){
				
						if (((ntimes==1) &  (m==start[day_simulation]) & (nweek==1)) | ( (m==start[day_simulation]) & (reactive==1))){
						
							count_positive_st(data, nr, day_simulation, counter_prodro, counter_infect, counter_rp, status0, detected, quarantine, pre_flag, num_iso, fp20);

						}

						if (((((ntimes==1) | (ntimes==2)) & (m==start[day_simulation]) & (nweek==1)) 
	  		    		|
	  		    		((ntimes==1) & (m==start[day_simulation]) & (nweek==2) & (day_simulation==day_screen)) |
	  		    		((ntimes==4) &  (m==start[day_simulation]) & (nweek==1))) & (regular==1) ){
	  		    		
						assign_compliance_st(data, tc, comp_id, quarantine, detected, num_iso, pre_flag, weekly_perc_adhesion[nw], regular);
	  		    			regular_screening(data, param, tc, status0, teach_rep, teach_rtested, pre_flag, quarantine, detected, screenstatus, comp_id, start_q2, start_rq2, m_absolute, day_simulation, nr, theta_ps, theta_cs, theta_scs, theta_rps, counter_prodro, counter_infect, counter_rp, num_pos_class);//, fp12, fp13);
	  		    			
	  		    			//count_positive_st(data, nr, day_simulation, counter_prodro, counter_infect, counter_rp, status, detected, quarantine, pre_flag, num_iso, fp20);
	  		    			
	  		    		}
	  		    			
						for (i=0; i<ct->n1; i++){
 	  	
 		  	  	  			if ((ct->step[i]==m) & (ct->days[i]==d1)){
 		  	  	  
 		  	  	  			
 			
 		        				id1=ct->ID1[i];
 		        				id2=ct->ID2[i];
 		        		
 		        		
 		        				index_id1=index_nodes(data, id1); 
 		        				index_id2=index_nodes(data, id2); 
 		        
  		        				s_id1=status0[index_id1];
 		        				s_id2=status0[index_id2];
 		        		
 		        				role_id1=data->classes[index_id1];
 		        				role_id2=data->classes[index_id2];
 		        		
 		        				weight=ct->weight[i];
 		        		
 		        				// rbeta e sigma definition
 		        				// Reduction on susceptibility and infectiousness is different for adults and children
 		        				// class = 6 is used to identified teachers
 		        		
 		        				int is_teacher1 = (ct->class1[i] == 6);
								int is_teacher2 = (ct->class2[i] == 6);
							
								int v2dose1 = vaccinated_2doses[index_id1];
								int vboost1 = vaccinated_booster[index_id1];
								int n2dose1 = nweek_2doses[index_id1];
								int nboost1 = nweek_booster[index_id1];
							
								int v2dose2 = vaccinated_2doses[index_id2];
								int vboost2 = vaccinated_booster[index_id2];
								int n2dose2 = nweek_2doses[index_id2];
								int nboost2 = nweek_booster[index_id2];
								
								int delta2 = r_delta[index_id2];
								int delta1 = r_delta[index_id1];
 		        		
 		        				// rbeta e sigma definition
 		        				// Reduction on susceptibility and infectiousness is different for adults and children
 		        				// class = 6 is used to identified teachers
								if ((s_id1 == 0 || s_id1 == 12) && (s_id2 == 2 || s_id2 == 5 || s_id2 == 4)) {
									
									sigma = compute_sigma(s_id1, is_teacher1, vboost1, v2dose1, nboost1, n2dose1,
											sigma_st, sigma_tc, param->sigma_omicron, nw);
											
									rbeta = compute_rbeta(s_id2, is_teacher2, vboost2, v2dose2, delta2,
										nboost2, n2dose2, rbeta_st, rbeta_tc, nw);
										
								} else if ((s_id2 == 0 || s_id2 == 12) && (s_id1 == 2 || s_id1 == 5 || s_id1 == 4)) {
									sigma = compute_sigma(s_id2, is_teacher2, vboost2, v2dose2, nboost2, n2dose2,
										sigma_st, sigma_tc, param->sigma_omicron, nw);
										
									rbeta = compute_rbeta(s_id1, is_teacher1, vboost1, v2dose1, delta1,
									nboost1, n2dose1, rbeta_st, rbeta_tc, nw);
								}
 		        				
 		        				//TRANSITION FROM S->E
 		        				//if (((symptomatic==1) & (preventive==1)) | (symptomatic==1) | (regular==1)){
 		        		
 		        				// pre_flag is used to check if contacts are on on off 
 		        				// depending on presence of isolation or not 
 		        				// If contacts are on (pre_flag=0, individuals in q3) 
 		        				// this means that the infectious node not be in pre isolation
 		        				
 		        				
 		        			   	if((((pre_flag[index_id1]==0) & (detected[index_id1]==0) & (quarantine[index_id1]==0))
 		        				 & ((pre_flag[index_id2]==0) & (detected[index_id2]==0) & (quarantine[index_id2]==0)))
 		        				 ){
 		        						
 		        					if((s_id1==2) & ((s_id2==0) | (s_id2==12))){
 		        			
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id2]=1;  
 		           							exp_day[index_id2]=day_simulation;
 		           							new_exposed++;  
 		           							
 		           							
 		      
 		           							if ((role_id2==6) && (teach_rep[index_t(tc, id2)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id2==6) && (teach_rep[index_t(tc, id2)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id2!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							}
 		           						
 		           							
 		           						}
 		           						
 		        					} else if (((s_id1==0) | (s_id1==12))& (s_id2==2)){
 		        					
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        								
 		           							status[index_id1]=1;  
 		           							exp_day[index_id1]=day_simulation;
 		           							new_exposed++;
 		           							
 		           							
 		           								
 		           							if ((role_id1==6) && (teach_rep[index_t(tc, id1)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id1==6) && (teach_rep[index_t(tc, id1)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id1!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							}  
 		           							
 		           						}
 		           						
 		           				 	// 4 clinical
 		        					} else if ((s_id1==4) & ((s_id2==0) | (s_id2==12)) ){
 		        						
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id2]=1;  
 		           							exp_day[index_id2]=day_simulation;
 		           							new_exposed++; 
 		           							
 		           							if ((role_id2==6) && (teach_rep[index_t(tc, id2)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id2==6) && (teach_rep[index_t(tc, id2)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id2!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		          							} 
 		           							
 		           						}
 		           					
 		           							
 		        					}else if (((s_id1==0) | (s_id1==12)) & (s_id2==4)){
 		        						
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id1]=1; 
 		           							exp_day[index_id1]=day_simulation; 
 		           							new_exposed++; 
 		           							
 		           							if ((role_id1==6) && (teach_rep[index_t(tc, id1)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id1==6) && (teach_rep[index_t(tc, id1)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id1!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							} 
 		           							
 		           						}
 		           							
 		           					// 5 subclinical
 		        					} else if ((s_id1==5) & ((s_id2==0) | (s_id2==12))){
 		        				
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id2]=1; 
 		           							exp_day[index_id2]=day_simulation; 
 		           							new_exposed++;  
 		           							
 		           							if ((role_id2==6) && (teach_rep[index_t(tc, id2)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id2==6) && (teach_rep[index_t(tc, id2)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id2!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							} 
 		           							
 		           							 
 		           						}
 		           							
 		        					}else if (((s_id1==0) | (s_id1==12)) & (s_id2==5)){
 		        					
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id1]=1;  
 		           							exp_day[index_id1]=day_simulation;
 		           							new_exposed++; 
 		           							
 		           							if ((role_id1==6) && (teach_rep[index_t(tc, id1)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id1==6) && (teach_rep[index_t(tc, id1)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id1!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							}  
 		           														}
 		           					}	
 		   	   				}
 	         	    			}
	  				}			 
				}
				
				
				for (j=0; j<data->n; j++){
			
 		    			id=data->ID[j];
 		    			role_id=data->classes[j];
 		   			index_id=index_nodes(data, id);
 		    			s_id=status0[index_id];
 		    		
 		    			
 		    			// End of first isolation Q1
 		    			// From Q1 to Q2
 		    			// n_transitions1 = number of suspected nodes tested 
 		    			if ((symptomatic==1) & (n_transitions1>0) & (n_transitions2>=0)){
 		    		
 		    			 // n_transitions1 = number of symp collected
 		    			 // if there is only sympt ON (symptomatic==1)
 		    			 // in n_transitions1= stud + original teacher detected
 		    			 // If if there is sympt ON (symptomatic==1) + prevent. ON (preventive==1)
 		    			 // in n_transitions1= stud + original teacher detected + teacher replaced
 		    			 // I need the information on replced to know if they activate a PREVENTIVE or NOT
 		    			 
 		    		  		for (g=0; g<n_transitions1; g++){
 		    			
 		    					// q1_id=id tested because suspected
 		    					// At the time of the turnaround time, we known the result			
							if((id==q1_id[g]) & (m_absolute==q1_quarter[g]+(param->tau_test/param->deltat))){
 		    				
 		    						//If id is a student
 		    						if (role_id!=6){
 		    							// IF positive go in Q2 and its contacts are OFF
 		    							if (q1_result[g]==1){
 		    						
 		    								new_q2_s++;
 		    							
 		    								detected[index_id]=0;
 		    								quarantine[index_id]=1;
 		    								num_iso[index_id]=1;
 		    								q2_id[n_transitions2]=id;
 		   		        					q2_quarter[n_transitions2]=m_absolute;
 		    						
 		    								q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    								q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    								
 		    								
 		    								n_transitions2++;
 		    							
 		    								// Count the number of additional active classes
 		    								
 		    								if ((reactive == 1) & (reactive_class[index_class(tc, data->classes[index_id])]==0) & (screenstatus[index_id]!=6)){
 		    								
 		    									id_reactive[n_reactive] = id;
 		    									
 		    									/*if (param->deltar==1440){
 		    										m_reactive[n_reactive] = m_absolute + (num_15min_day - m) + start[0];
 		    									}
 		    									
 		    									if (param->deltar==0){
 		    										if (m>end[day_simulation])
 		    											m_reactive[n_reactive] = m_absolute + (num_15min_day - m) + start[0];
 		    										if (m<start[day_simulation])
 		    											m_reactive[n_reactive] = m_absolute + (start[0] - m);
 		    										if ((m>=start[day_simulation]) & (m<=end[day_simulation]))
 		    											m_reactive[n_reactive] = m_absolute;
 		    									}*/
 		    									
 		    									if (param->deltar==1440){
 		    										m_reactive[n_reactive] = m_absolute + (num_15min_day - m) + start[0];
 		    									}
 		    									
 		    									if (param->deltar==0){
 		    										
 		    										m_reactive[n_reactive] = m_absolute;
 		    									}
	 		    									
	 		    									
 		    									id_reactive=(int*)realloc(id_reactive,(n_reactive+2)*sizeof(int)); 		    										
 		    									m_reactive=(int*)realloc(m_reactive,(n_reactive+2)*sizeof(int));
	 		    									
 		    									
 		    									reactive_class[index_class(tc, data->classes[index_id])]=1;
 		    									n_reactive++;
	 		    									
	 		    								
 		    								}	
 		    								
 		    								if ((preventive==1) & (regular==0)){
 		    								
 		    									class_q3=data->classes[index_id];
 		    									teach_q3=find(tc, id, class_q3);
 		    								
 		    									// If the class in not already in Q3
 		    				 					if (class_iso[index_t(tc, teach_q3)]==0){
 		    							
 		    										// Turn on if the class is in pre iso
 		    										class_iso[index_t(tc, teach_q3)]=1;
 		    										
 		    										dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    										
 		    										trigger_role=1;
 		    									
 		    										
 		    							
 		    										new_q3=new_q3+dim;
 		    							
 		    									//fprintf(stderr, "teach in Q3: %d\t interval: %d\t %d\t day: %d\t s%d\n", teach_q1, m,m_absolute, day_simulation, status0[index_nodes(data, teach_q1)]);
 		    									}else{
 		    										pre_flag[index_id]=0;
 		    									}
 		    								}

 		    							}else{
 		    								detected[index_id]=0;
 		    								new_neg_s++;
 		    								// If a students is tested negative during preisolation
 		    								// he/she contin in Q3
 		    								if (preventive==1){
 		    									class_q3=data->classes[index_id];
 		    									teach_q3=find(tc, id, class_q3);
 		    									if (class_iso[index_t(tc, teach_q3)]==1){
 		    										pre_flag[index_id]=1;
 		    									}
 		    								}
 		    							}
 		    							// IF id is a teacher:
 		    							// a. original
 		    							// b. replaced
 		    						}else if (role_id==6){
 		    							// IN q1 the original teacher is already replaced
 		    							// At this point, teach_rep==1
 		    							if (teach_rep[index_t(tc, id)]==1){
 		    								// IF positive 
 		    								// Save the moment in which go in Q2
 		    								if (q1_result[g]==1){
 		    							
 		    									new_q2_t++;
 		    								
 		    									q2_id[n_transitions2]=id;
 		   		        						q2_quarter[n_transitions2]=m_absolute;
 		    									//q2_day[n_transitions1]=day_simulation;
 		    				
 		    									q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    									q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    									//q2_day=(int*)realloc(q2_day,(n_transitions2+2)*sizeof(int));
 		    									
 		    									fprintf(stderr, " q2 id: %d\t start Q2 interval: %d\t start Q2 day: %d\n", q2_id[n_transitions2], q2_quarter[n_transitions2], day_simulation);
 		    									
 		    									n_transitions2++;
 		    							
 		    									// Count the number of additional active classes
 		    									
 		    									detected_t[index_t(tc,id)]=0;
 		    									quarantine_t[index_t(tc, id)]=1;
 		    									
 		    									
 		    								
 		    									if ((preventive==1) & (regular==0)){
 		    					
 		    				 						if (class_iso[index_t(tc, id)]==0){
 		    							
 		    											// Turn on if the class is in pre iso
 		    											class_iso[index_t(tc, id)]=1;
 		    											
 		    											dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    											
 		    											trigger_role=2;
 		    						
 		    											
 		    											// If the teacher is the trigger, the 
 		    											// replace teacher is however in
 		    											// isolation with the class
 		    							
 		    											new_q3=new_q3+dim;
 		    										}
 		    										// If a teacher go in in Q2 during a preventive
 		    										// the pre_flag is ON for the replaced teacher
 		    									}
 		    								// Otherwise back to school as clinical
 		    								}else {
 		    							
 		    									status[index_id]=4;
 		    									detected_t[index_t(tc,id)]=0;
 		    									new_neg_t++;
 		    									
 		    									if (preventive==1){
 		    										
 		    										if (class_iso[index_t(tc, id)]==1){
 		    											pre_flag[index_id]=1;
 		    										}
 		    									}
 		    								}
 		    								// teach_rep >1 Sostituto del sostituo
 		    								// When the original back, teach_rep==-9
 		    							}else if (teach_rep[index_t(tc, id)]>1){
 		    								
 		    							
 		    								if (q1_result[g]==1){ 
 		    									new_pos_r++;
 		    								
 		    				 					if ((preventive==1) & (regular==0) & (class_iso[index_t(tc, id)]==0)){
 		    							
 		    										// Turn on if the class is in pre iso
 		    										class_iso[index_t(tc, id)]=1;
 		    						
 		    										dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    										
 		    										trigger_role=3;
 		    										
 		    							
 		    										new_q3=new_q3+dim;
 		    									}
 		    								}else{
 		    									new_neg_r++;
 		    									
 		    									if (preventive==1){
 		    										
 		    										if (class_iso[index_t(tc, id)]==1){
 		    											pre_flag[index_id]=1;
 		    										}
 		    									}
 		    								}
 		    							}
 		    						}
 		    					
 		    					}	
 		    				}
 		    				
 		    				if (reactive == 1){
 		    				
	 		    				for(hg=0; hg < n_reactive; hg++){
	 		    				
	 		    					if((id == id_reactive[hg]) & (m_absolute == m_reactive[hg])){
	 		    					
	 		    					
	 		    						//fprintf(stderr, "REACTIVE day %d\t m %d\t m_abs %d\n", day_simulation, m, m_absolute);
	 		    					
	 		    						reactive_screening(data, tc, param, theta_ps, theta_cs, theta_scs, theta_rps, id, day_simulation, m_absolute, nr, status0, screenstatus, start_q2, quarantine, detected, classtotest, indtotest, when, counter_prodro, counter_infect, counter_rp, fp16, fp18);
	 		    						
	 		    						if (data->classes[index_id]!=6){
	 		    							reactive_class[index_class(tc, data->classes[index_id])]=0;
	 		    						}else{
	 		    							reactive_class[index_t(tc, id)]=0;
	 		    						}
	 		    					}
	 		    					
	 		    		  		}
	 		    		  		
	 		    		  		if (param->delay!=0){
	 		    		  		
		 		    		  		for (ncl=0; ncl< tc->nc; ncl++){
		 		    		  			if (((classtotest[ncl]==1) & (m_absolute==when[ncl])) |
		 		    		  			 ((classtotest[ncl]==2) & (m_absolute==when[ncl]+ 2 * num_15min_day)) ){
		 		    		  				post_reactive(data, tc, param, tc->clas[ncl], day_simulation, m_absolute, nr, status0, screenstatus, start_q2, start_rq2, teach_rtested, teach_rep, quarantine, detected, indtotest, theta_ps, theta_cs, theta_scs, theta_rps, counter_prodro, counter_infect, counter_rp, fp17, fp19);
		 		    		  				if (classtotest[ncl]==1){
		 		    		  					classtotest[ncl]=2;
		 		    		  				}
		 		    		  				
		 		    		  				if(m_absolute==when[ncl]+2*num_15min_day){
		 		    		  					classtotest[ncl]=0;
		 		    		  					
		 		    		  					for (z=0; z<data->n; z++){
		 		    		  						if((data->classes[z]==tc->clas[ncl]) 
		 		    		  						& (indtotest[z]==1)){
		 		    		  							indtotest[z]=0;
		 		    		  						}
		 		    		  						
		 		    		  						if((data->classes[z]==6) 
		 		    		  						& (data->ID[z]!=tc->ID[ncl])
		 		    		  						& (indtotest[z]==1)){
		 		    		  							indtotest[z]=0;
		 		    		  						}
		 		    		  						
		 		    		  					}
		 		    		  				}
		 		    		  			}
		 		    		  		}
	 		    		  		}
 		    		  		}
 		    		  		
 		    		  		// End of quarantine for IC(d) in Q2
 		    		  	  	for(hh=0; hh<n_transitions2; hh++){
 		    			
 		    		  			if( (id==q2_id[hh]) & (m_absolute == q2_quarter[hh]+(param->delta_q/param->deltat) ) ){
 		   				
 		   						if (role_id!=6){ 
 		   					
 		   							quarantine[index_id]=0;
 		   							
 		   						
 		   						}else if(role_id==6){
 		    					
 		    							// We test the original teacher at the end of Q2, and we assume that 
 		    							// status of the original at the end is equal to R+ (8)
 		    							// So we test using a sensitivity teta_r+
 		    						
 		    							// Quarantine_t and teach_rep are turnd off only if the result of
 		    							// the test is negative
 		    						
 		    							r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		    							
 		    							
 		    							//fprintf(stderr, "q2 id:%d\t start Q2: %d\t end Q2: %d\t (day:%d)\t role: %d\t result:%d\n", id, q2_quarter[hh], m_absolute, day_simulation, role_id, r);
 		    						
 		    							// If during quarantine the replace teacher goes in isoaltion with 
 		    							// the class when the teacher come back (i.e the result of the test is negative),
 		    							// preflag is 0
 		    							//if((pre_flag[index_id]==1) & (r==0)){
 		    							//	pre_flag[index_id]=0;
 		    							//}
 		    						
 		    							// Counter on the number of tests on the original teacher
 		    							// before back to school
 		    							
 		    							n_test_t++;
 		    						
 		    							if (r==1){
 		    								n_test_t_p++;
 		    							}else{
 		    								n_test_t_n++;
 		    							}
 		    						
 		    						}
 		    					
 		    					}
 		    		  	  	}			
 		    			}
 		    		
 		    			// End of preventive isolation	
 		    			if (preventive==1){
 		    			
 		    				if ((pre_flag[index_id]==1) & (m_absolute==pre_time[1][index_id]+(param->delta_q/param->deltat))){
 		       			
 		       					if (role_id!=6){
 		       				
 		       						pre_flag[index_id]=0;
 		       						tea=find(tc, id, data->classes[index_id]);
 		        			
 		       						if(class_iso[index_t(tc, tea)]==1)
 		       							class_iso[index_t(tc, tea)]=0;
 		       						
 		       						
 		       					}else if ((role_id==6) & (quarantine_t[index_t(tc,id)]==0)){
 		       					
 		        					r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		        						
 		        					//fprintf(stderr, "teach id in Q3:%d\t start Q3: %d\t (day:%d)\t  end Q3: %d\t (day:%d)\t role: %d\t result:%d\n", id, pre_time[1][index_id], pre_time[0][index_id], m_absolute, day_simulation, role_id, r);
 		        					
 		        					// Counter on the number of tests on the original teacher 
 		        					// before back to school
 		        					n_test_t++;
 		        							
 		        					if (r==1){
 		    							n_test_t_p++;
 		    						}else{
 		    							n_test_t_n++;
 		    						}
 		        		
 		        			
 		       					}
 		       					pre_flag[index_id]=0;
 		       						
	       						/*for (ttts=0; ttts<tc->nc; ttts++){
	       							fprintf(stderr, "flag class in iso: %d\t %d\n", tc->clas[ttts], class_iso[ttts]);
	       							
	       						}*/
 		       						
 		       				}
 		       			}
 		       			if ((regular==1) | (reactive==1) ){
 		       				//start q2 hold only for original teach
 		       				
	  					if ((screenstatus[j]==6) & ((m_absolute==start_q2[j]) | (m_absolute==start_rq2[j]))){
	  					
	  						
	  						new_q2_reg++;
	  						if ((role_id!=6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
	  						
	  							
	  							
	  							new_q2_regs++;
	  							quarantine[j]=1;
	  							
	  							if ((preventive == 1) & 
	  							(num_pos_class[index_class(tc, data->classes[index_id])]>=3) &
	  							(class_iso[index_class(tc, data->classes[index_id])]==0)){
	  							
	  								
		 		    		  			 preisolation(data, tc, param, id, role_id, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
		 		    		  		}
	  							
	  						}else if ( (role_id==6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
	  						
	  							
	  							
	  							new_q2_regt++;
	  							quarantine_t[index_t(tc,id)]=1;
	  							status[j]=0;
	  						}else if ((m_absolute==start_rq2[j]) & (start_rq2[j]!=0) & (role_id==6) & (teach_rtested[index_t(tc, id)]>1)){
	  						
	  							new_q2_regr++;
	  							status[j]=0;
	  						
	  						}
	  						screenstatus[j]=-9;
	  						
	  					}else if ((m_absolute==start_q2[index_id]+param->delta_q/param->deltat) & (start_q2[j]!=0)){
 		       				 	
 		       				 	if (role_id!=6){
 		       				 		quarantine[index_id]=0;
 		       					}else if (role_id==6){
 		       					
 		       						r= test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		       				 		
 		        					
 		        					// Counter on the number of tests on the original teacher 
 		        					// before back to school
 		        					n_test_t++;
 		        							
 		        					if (r==1){
 		    							n_test_t_p++;
 		    						}else{
 		    							n_test_t_n++;
 		    							teach_neg_reg[n_reg]=id;
 		    							teach_neg_reg=(int*)realloc(teach_neg_reg,(n_reg+2)*sizeof(int));
 		    							n_reg++;
 		    						}
 		    						
 		    						
 		    						
 		    					}
 		       				 }
 		       			}
 		       			
 		    			 
 		    			// End of second isolation
 		    			if ((symptomatic==1) | ((symptomatic==1) & (preventive==1)) | (regular==1)){
 		    				if (role_id==6){
 		    					if ((teach_2iso[index_t(tc, id)]==1) 
 		    					& (m_absolute==teach_2isom[index_t(tc, id)]+param->delta_q/param->deltat)){
 		    					
 		    						status[index_id]=9;
 		    						teach_2iso[index_t(tc, id)]=-9;
 		    						quarantine_t[index_t(tc, id)]=0;
 		    						teach_rep[index_t(tc,id)]=-9;
 		    						
 		    						
 		    					}
 		    				}			
 		        		}

 		        		// INDIPENDENT TRANSITIONs:
 						// TRANSITION FROM E->IP 				
 						// TRANSITION FROM IP->IC_detected		
 						// TRANSITION FROM IP->IC
 						// TRANSITION FROM IP->ISC		
 						// TRANSITION FROM Q1->Q2
 						// TRANSITION FROM IC->R+ o ISC->R+ 	
 						// TRANSITION FORM ICDI-> R+		
 						// TRANSITION FORM R+ -> R
 					
 					
 		    			if( (s_id==1) & (counter_latent[index_id] < param->shape_latent)){
 		    				random = gsl_ran_flat(RNG,0,1);
 		    				if(random < (1 - exp(-(param->shape_latent * param->eps) * param->deltat))){
 		    				
 		    					status[index_id]=1;  // 2 for prodromic phas
 		    					counter_latent[index_id]=counter_latent[index_id]+1;
 		    					
 		    					
 		    					
 		    					
 		    					if (counter_latent[index_id] == param->shape_latent){
 		    						status[index_id]=2;
 		    						
 		    						new_pro++;
 		    						if (role_id!=6){
	 		    						new_pro_s++;
	 		    					}else if (role_id==6){
	 		    						if (teach_rep[index_t(tc,id)]==0){
	 		    							new_pro_t++;
	 		    						}else{
	 		    							new_pro_r++;
	 		    						}
	 		    					}
 		    						counter_latent[index_id]=0;
 		    						
 		    						
 		    					}
 		    				}
 		    			
 		    			} else if ((s_id==2) & (counter_prodro[index_id] < param->shape_prodro)){
 		    				random=gsl_ran_flat(RNG,0,1);
 		    				counter_latent[index_id]=0;
 		    				
	 		    			if (random< (1- exp(-(param->shape_prodro *param->mu_p)*param->deltat))){
	 		    				
 		    					status[index_id] = 2;
 		    					counter_prodro[index_id] = counter_prodro[index_id] +1;
	 		    				
	 		    				if (counter_prodro[index_id] == param->shape_prodro){
	 		    					
	 		    					
	 		    					
	 		    					//counter_prodro[index_id]=0;
	 		    					random1=gsl_ran_flat(RNG,0,1);
	 		    					// 6 to identified teachers
	 		    					if ( ((role_id==6) & (random1<psub_tc->novax[nw]) & (vaccinated_2doses[index_id]==0) 
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==0))|
	 		    					 ((role_id==6) & (random1<psub_tc->twodoses[nw]) & (vaccinated_2doses[index_id]==0) 
	 		    					 & (vaccinated_booster[index_id]==0)& (r_delta[index_id]==1))|
	 		    					((role_id==6) & (random1<psub_tc->twodoses[nweek_2doses[index_id]]) & (vaccinated_2doses[index_id]==1)) |
	 		    					((role_id==6) & (random1<psub_tc->booster[nweek_booster[index_id]]) & (vaccinated_booster[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->novax[nw]) & (vaccinated_2doses[index_id]==0)
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==0)) |
	 		    					 ((role_id!=6) & (random1<psub_st->twodoses[nw]) & (vaccinated_2doses[index_id]==0)
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->twodoses[nweek_2doses[index_id]]) & (vaccinated_2doses[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->booster[nweek_booster[index_id]]) & (vaccinated_booster[index_id]==1))
	 		    					  ){
	 		    						//fprintf(stderr, "Inside subclinical\n");
	 		    						status[index_id]=5; 
	 		    						new_is++;
	 		    						if (role_id!=6){
	 		    							new_is_s++;
	 		    						}else if (role_id==6){
	 		    							if (teach_rep[index_t(tc,id)]==0){
	 		    								new_is_t++;
	 		    							}else{
	 		    								new_is_r++;
	 		    							}
	 		    						}
	 		    					//5 for subclinical case
	 		    					} else {
	 		    					
	 		    						
	 								random2=gsl_ran_flat(RNG,0,1);
	 								// Clinical detected
	 									
	 		    						if (((role_id==6) & (random2<pd[1])) | ((role_id!=6) & (random2<pd[0])) ){
	 		    					
	 		    							
	 		    							if (role_id!=6){
	 		    								n_test_symp_st++;
	 		    							}else{
	 		    								n_test_symp_tc++;
	 		    							}
	 		    							new_ic_d++;
	 		    							
	 		    							random3=gsl_ran_flat(RNG,0,1);
	 		    							n_test_symp++;
	 		    							
	 		    							
	 		    							if (((random3<theta_cs[0][counter_infect[index_id]]) & (data->classes[index_id]!=6)) | ((random3<theta_cs[1][counter_infect[index_id]]) & (data->classes[index_id]==6))){
	 		    								
	 		    								if (role_id!=6){
	 		    									fprintf(stderr, "%d\t %f\t %f\n", counter_infect[index_id], theta_cs[0][counter_infect[index_id]], theta_cs[0][0]);
	 		    								}else{
	 		    									fprintf(stderr, "%d\t %f\t %f\n", counter_infect[index_id], theta_cs[1][counter_infect[index_id]], theta_cs[0][0]);
	 		    								}
	 		    					
	 		    								n_test_symp_p++;
	 		    								result=1;
	 		    								if (role_id!=6){
	 		    									role=1;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]==0)){
	 		    									role=2;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]>=1)){
	 		    									role=3;
	 		    								}
	 		    								
	 		    								
	 		    								//IF stud/teach/repl_teach
	 		    								if(role_id!=6){ 
	 		    								
	 		    									q1_id[n_transitions1]=id;
	 		    									q1_quarter[n_transitions1]=m_absolute;
	 		    									q1_result[n_transitions1]=1;
	 		    									
	 		    									//fprintf(stderr, "n_transitions1:%d\n", n_transitions1);
	 		   									
	 		   									
	 		    									q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    									q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
													q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
										
													n_transitions1++;
													
												
													status[index_id]=4;
													detected[index_id]=1;
													
	 		   										new_ic_d_s++;
	 		   									
													// If identified during a preventive	
													if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    										pre_flag[index_id]=0;
	 		    									}
	 		    									
	 		    								}else if (role_id==6){
	 		    								
	 		    									if (teach_rep[index_t(tc, id)]==0){
	 		    									
	 		    										q1_id[n_transitions1]=id;
	 		    										q1_quarter[n_transitions1]=m_absolute;
	 		    										q1_result[n_transitions1]=1;
	 		    									
	 		    										q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    										q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
														q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
														
														
														n_transitions1++;
	 		   										
	 		    										detected_t[index_t(tc,id)]=1;
														teach_rep[index_t(tc,id)]=1;
														status[index_id]=0;
														
														new_ic_d_t++;
	 		    										
	 		    										status_orig=4;
	 		    										reason=1;
	 		    										
	 		    										
	 		    										// A teacher can be replaced because symp.
	 		    										
	 		    										// If identified during a preventive	
	 		   										if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    											pre_flag[index_id]=0;
	 		    										}
	 		    									}else if (teach_rep[index_t(tc, id)]>=1){
	 		    									
	 		    										
	 		    										status[index_id]=0;
	 		    										new_ic_d_r++;
	 		    									
	 		    										status_orig=4;
	 		    										reason=4;
	 		    								
	 		    										teach_rep[index_t(tc,id)]=2;
	 		    										
	 		   										if (preventive==1){
	 		   										
	 		   											q1_id[n_transitions1]=id;
	 		    											q1_quarter[n_transitions1]=m_absolute;
	 		   											q1_result[n_transitions1]=1;
	 		   										
	 		   											q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    											q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   											q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));	
	 		   											
	 		   											n_transitions1++;
	 		   										}
	 		    									}
	 		    										
	 		   								}
	 		   								
	 		    							} else {
	 		    							
	 		    								n_test_symp_n++;
	 		    								/*result=0;
	 		    								if (role_id!=6){
	 		    									role=1;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]==0)){
	 		    									role=2;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]>=1)){
	 		    									role=3;
	 		    								}
	 		    								*/
	 		    								
	 		    								if(role_id!=6){
	 		    								 												q1_id[n_transitions1]=id;
	 		    									q1_quarter[n_transitions1]=m_absolute;
	 		    									q1_result[n_transitions1]=0;
	 		    									
	 		    									q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    									q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   									q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   									
	 		   									n_transitions1++;
	 		   									
	 		   									status[index_id]=4;
	 		   									detected[index_id]=1;
	 		   									new_ic_d_s++;
	 		   										
	 		   									if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    										pre_flag[index_id]=0;
	 		    									}
	 		    								}else if (role_id==6){
	 		   									
	 		   									if (teach_rep[index_t(tc, id)]==0){
	 		   									
	 		   										q1_id[n_transitions1]=id;
	 		    										q1_quarter[n_transitions1]=m_absolute;
	 		    										q1_result[n_transitions1]=0;
	 		    									
	 		    										q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    										q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   										q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   										
	 		   										n_transitions1++;
	 		   									
	 		   										detected_t[index_t(tc,id)]=1;
	 		   										status[index_id]=0;
	 		   										
	 		   										new_ic_d_t++;
	 		    										
	 		    										status_orig=4;
	 		    										reason=1;
	 		    										
	 		    										
	 		    										
	 		    								
	 		    										teach_rep[index_t(tc,id)]=1;
	 		    										
	 		    										if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    											pre_flag[index_id]=0;
	 		    										}
	 		   									}else if (teach_rep[index_t(tc, id)]>=1){
	 		    									
	 		    										status[index_id]=0;
	 		    										new_ic_d_r++;
	 		    									
	 		    										status_orig=4;
	 		    										reason=4;
	 		    								
	 		    										teach_rep[index_t(tc,id)]=2;
	 		    										
	 		   										if (preventive==1){
	 		   										
	 		   											q1_id[n_transitions1]=id;
	 		    											q1_quarter[n_transitions1]=m_absolute;
	 		   											q1_result[n_transitions1]=0;
	 		   										
	 		   											q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    											q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   											q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));			
	 		   											
	 		   											
	 		   											n_transitions1++;
	 		   										}
	 		   									}
	 		   								}
	 		    							
	 		    							}
	 		    						// Clinical not detected 
	 		    						}else{
	 		    						
	 		    							
	 		    							
	 		    							status[index_id]=4;
	 		    							new_ic++;
	 		    							
	 		    							if (role_id!=6){
	 		    								new_ic_s++;
	 		    								detected[index_id]=0;
	 		    							}else if (role_id==6){
	 		    								if (teach_rep[index_t(tc,id)]==0){
	 		    									new_ic_t++;
	 		    									detected_t[index_t(tc,id)]=0;
	 		    								}else{
	 		    									new_ic_r++;
	 		    								}
	 		    							}
	 		    						}   
	 							}			
	 					 	}
	 					 }
 					 	//fprintf(stderr, "end prodromic");
 		    			} else if(((s_id==4) & (counter_infect[index_id]<param->shape_infect)) | 
 		    			((s_id==5) & (counter_infect[index_id]<param->shape_infect))){	
 		    		        	random=gsl_ran_flat(RNG,0,1);
 		    		        	counter_prodro[index_id]=0;	
	    					if(random< 1 - exp(-(param->shape_infect * param->mu)*param->deltat)){
	    						if (s_id==4){
	    							status[index_id]=4;
	    						}else if (s_id==5){
	    							status[index_id]=5;
	    						}
	    						counter_infect[j]=counter_infect[j]+1;
	    						if (counter_infect[j] == param->shape_infect){
	    							status[index_id]=8;          // 8 for recovery but still positive to the te
	    							
	    						}
	    					}	

 		    			} else if ((s_id==8) & (counter_rp[index_id]<param->shape_rp)){
 		    				random4=gsl_ran_flat(RNG,0.0,1.0);
 		    				counter_infect[j] = 0;
 		    				if (role_id!=6){	
 		    					if(random4 < (1 - exp(-(param->shape_rp * param->mu_rp)*param->deltat))){
 		    						counter_rp[index_id]=counter_rp[index_id]+1;
 		    						if (counter_rp[index_id]==param->shape_rp){
 		    							status[index_id]=9;
 		    							counter_rp[index_id] = 0;
 		    						}
 		    					}
 		    				}else if(role_id==6){
 		    				
 		    					if (symptomatic==1){
 		    						for(hh=0; hh<n_transitions2; hh++){
 		    							if((id==q2_id[hh]) & (random4<1 - exp(-(param->shape_rp * param->mu_rp1)*param->deltat))){
 		    								counter_rp[index_id]=counter_rp[index_id]+1;
		 		    						if (counter_rp[index_id]==param->shape_rp){
		 		    							status[index_id]=9;
		 		    							counter_rp[index_id] = 0;
		 		    						}
		 		    								
 		    							}else if ((id!=q2_id[hh]) & (random4<1 - exp(-(param->shape_rp * param->mu_rp)*param->deltat))){
 		    								counter_rp[index_id]=counter_rp[index_id]+1;
		 		    						if (counter_rp[index_id]==param->shape_rp){
		 		    							status[index_id]=9;
		 		    							counter_rp[index_id] = 0;
		 		    						}
 		    							}
 		    						}
 		    					}
 		    					
 		    					
 		    				}
 		    			}    	
 		    		}	
 		

 					save_status(data,status,status0); 
	  	
	  			}

	  			fprintf(fp5, "%d\t %d\t %d\t %d\t %d\t %d\n", nr, day_simulation, new_exposed, new_exp_s, new_exp_t, new_exp_r);
	  			fflush(fp5);
	  			
	  	
      				
	  			d1=d1+week;
                
      			break;
      		case 2:	

    			new_exposed=0;
    			new_pro=0;
    			new_ic_d=0;
    			new_ic=0;
    			new_is=0;
    			new_q3=0;
    		
    			// New intro in each compartments divided by type: STUDENTS/ TEACHERS/ REPLACED TEACHERS
    			new_exp_s=0;new_exp_t=0;new_exp_r=0;
				new_pro_s=0;new_pro_t=0;new_pro_r=0;new_ic_s=0;
    			new_ic_t=0;new_ic_r=0;new_ic_d_s=0;new_ic_d_t=0;new_ic_d_r=0;
				new_is_s=0;new_is_t=0;new_is_r=0;
				new_q2_s=0;new_q2_t=0;new_pos_r=0;new_q2_reg=0;new_q2_regs=0;new_q2_regt=0;new_q2_regr=0;new_neg_s=0;new_neg_t=0;new_neg_r=0;
				n_test_symp=0;n_test_symp_p=0; n_test_symp_n=0;n_test_symp_st=0; n_test_symp_tc=0;
      			n_test_t=0;n_test_t_p=0;n_test_t_n=0;
    		
	  			
    			for (m=0; m<num_15min_day; m++){
    			
    				m_absolute=num_15min_day*day_simulation + m; //progression of 15 minutes step from the first day to the last one
	        
	        		n_susc_s=0;
					n_susc_t=0;
      				// Intro of 1 case if there are still susceptible individuals
      				
      			 	for (i=0; i<data->n; i++){
      					if (((status0[i]==0) | (status0[i]==12)) & (data->ID[i]!=6)){
      						n_susc_s++;
      					}else if (((status0[i]==0) | (status0[i]==12)) & (data->ID[i]==6)){
	  						n_susc_t++;
	  					}
      				}

					if ((n_susc_s!=0) & ((*introst)[nr].weeks[nw]>0) & (n_susc_s>=(*introst)[nr].weeks[nw])){
      			
						for (ni=0; ni<(*introst)[nr].weeks[nw]; ni++){
							if (((day_simulation % (week) + 1)==introductions_days_st[ni]) & (m==introductions_quarter_st[ni])){
 
								typ=weekly_introduction_st(data, tc, status, status0, (*introst)[nr].weeks[nw], double_presence, teach_rep, exp_day, day_simulation);
									
								new_exposed++;
								
								if (typ==1){
									new_exp_s++;
								}	
							}
						}
					}
					if ((n_susc_t!=0) & ((*introtc)[nr].weeks[nw]>0) & (n_susc_t>=(*introtc)[nr].weeks[nw])){
					
						for (ni=0; ni<(*introtc)[nr].weeks[nw]; ni++){
							if (((day_simulation % (week) + 1)==introductions_days_tc[ni]) & (m==introductions_quarter_tc[ni])){
								  
								typ=weekly_introduction_tc(data, tc, status, status0, (*introtc)[nr].weeks[nw], double_presence, teach_rep, exp_day, day_simulation);
									
								new_exposed++;
								
								if (typ==2){
									new_exp_t++;
								}else if (typ==3){
									new_exp_r++;
								}		
							}
						}
					}
      				
      			
					if ((m>=start[day_simulation]) & (m<=end[day_simulation])){
				
						if ((regular==1) & (ntimes==4) &  (m==start[day_simulation]) & (nweek==1)){
								
								regular_screening(data, param, tc, status0, teach_rep, teach_rtested, pre_flag, quarantine, detected, screenstatus, comp_id, start_q2, start_rq2, m_absolute, day_simulation, nr, theta_ps, theta_cs, theta_scs, theta_rps, counter_prodro, counter_infect, counter_rp, num_pos_class);//, fp12, fp13);
								
								count_positive_st(data, nr, day_simulation, counter_prodro, counter_infect, counter_rp, status, detected, quarantine, pre_flag, num_iso, fp20);	
								if (nweek==2){
									day_screen=day_screen+(2*week);
							}
						}
	  		    			
						for (i=0; i<ct->n1; i++){
 	  	
 		  	  	  			if ((ct->step[i]==m) & (ct->days[i]==d2)){
 		  	  	  
 		  	  	  			
 			
 		        				id1=ct->ID1[i];
 		        				id2=ct->ID2[i];
 		        		
 		        		
 		        				index_id1=index_nodes(data, id1); 
 		        				index_id2=index_nodes(data, id2); 
 		        
  		        				s_id1=status0[index_id1];
 		        				s_id2=status0[index_id2];
 		        		
 		        				role_id1=data->classes[index_id1];
 		        				role_id2=data->classes[index_id2];
 		        		
 		        				weight=ct->weight[i];
 		        		
 		        				// rbeta e sigma definition
 		        				// Reduction on susceptibility and infectiousness is different for adults and children
 		        				// class = 6 is used to identified teachers
 		        		
 		        				int is_teacher1 = (ct->class1[i] == 6);
								int is_teacher2 = (ct->class2[i] == 6);
							
								int v2dose1 = vaccinated_2doses[index_id1];
								int vboost1 = vaccinated_booster[index_id1];
								int n2dose1 = nweek_2doses[index_id1];
								int nboost1 = nweek_booster[index_id1];
							
								int v2dose2 = vaccinated_2doses[index_id2];
								int vboost2 = vaccinated_booster[index_id2];
								int n2dose2 = nweek_2doses[index_id2];
								int nboost2 = nweek_booster[index_id2];
								
								int delta2 = r_delta[index_id2];
								int delta1 = r_delta[index_id1];
 		        		
 		        				// rbeta e sigma definition
 		        				// Reduction on susceptibility and infectiousness is different for adults and children
 		        				// class = 6 is used to identified teachers
								if ((s_id1 == 0 || s_id1 == 12) && (s_id2 == 2 || s_id2 == 5 || s_id2 == 4)) {
									
									sigma = compute_sigma(s_id1, is_teacher1, vboost1, v2dose1, nboost1, n2dose1,
											sigma_st, sigma_tc, param->sigma_omicron, nw);
											
									rbeta = compute_rbeta(s_id2, is_teacher2, vboost2, v2dose2, delta2,
										nboost2, n2dose2, rbeta_st, rbeta_tc, nw);
										
								} else if ((s_id2 == 0 || s_id2 == 12) && (s_id1 == 2 || s_id1 == 5 || s_id1 == 4)) {
									sigma = compute_sigma(s_id2, is_teacher2, vboost2, v2dose2, nboost2, n2dose2,
										sigma_st, sigma_tc, param->sigma_omicron, nw);
										
									rbeta = compute_rbeta(s_id1, is_teacher1, vboost1, v2dose1, delta1,
									nboost1, n2dose1, rbeta_st, rbeta_tc, nw);
								}
 		        				
 		        				//TRANSITION FROM S->E
 		        				//if (((symptomatic==1) & (preventive==1)) | (symptomatic==1) | (regular==1)){
 		        		
 		        				// pre_flag is used to check if contacts are on on off 
 		        				// depending on presence of isolation or not 
 		        				// If contacts are on (pre_flag=0, individuals in q3) 
 		        				// this means that the infectious node not be in pre isolation
 		        				
 		        				
 		        			   	if((((pre_flag[index_id1]==0) & (detected[index_id1]==0) & (quarantine[index_id1]==0))
 		        				 & ((pre_flag[index_id2]==0) & (detected[index_id2]==0) & (quarantine[index_id2]==0)))
 		        				 ){
 		        						
 		        					if((s_id1==2) & ((s_id2==0) | (s_id2==12))){
 		        			
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id2]=1;  
 		           							exp_day[index_id2]=day_simulation;
 		           							new_exposed++;  
 		           							
 		           							
 		      
 		           							if ((role_id2==6) && (teach_rep[index_t(tc, id2)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id2==6) && (teach_rep[index_t(tc, id2)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id2!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							}
 		           						
 		           							
 		           						}
 		           						
 		        					} else if (((s_id1==0) | (s_id1==12))& (s_id2==2)){
 		        					
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        								
 		           							status[index_id1]=1;  
 		           							exp_day[index_id1]=day_simulation;
 		           							new_exposed++;
 		           							
 		           							
 		           								
 		           							if ((role_id1==6) && (teach_rep[index_t(tc, id1)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id1==6) && (teach_rep[index_t(tc, id1)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id1!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							}  
 		           							
 		           						}
 		           						
 		           				 	// 4 clinical
 		        					} else if ((s_id1==4) & ((s_id2==0) | (s_id2==12)) ){
 		        						
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id2]=1;  
 		           							exp_day[index_id2]=day_simulation;
 		           							new_exposed++; 
 		           							
 		           							if ((role_id2==6) && (teach_rep[index_t(tc, id2)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id2==6) && (teach_rep[index_t(tc, id2)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id2!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		          							} 
 		           							
 		           						}
 		           					
 		           							
 		        					}else if (((s_id1==0) | (s_id1==12)) & (s_id2==4)){
 		        						
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id1]=1; 
 		           							exp_day[index_id1]=day_simulation; 
 		           							new_exposed++; 
 		           							
 		           							if ((role_id1==6) && (teach_rep[index_t(tc, id1)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id1==6) && (teach_rep[index_t(tc, id1)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id1!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							} 
 		           							
 		           						}
 		           							
 		           					// 5 subclinical
 		        					} else if ((s_id1==5) & ((s_id2==0) | (s_id2==12))){
 		        				
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id2]=1; 
 		           							exp_day[index_id2]=day_simulation; 
 		           							new_exposed++;  
 		           						
 		           							if ((role_id2==6) && (teach_rep[index_t(tc, id2)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id2==6) && (teach_rep[index_t(tc, id2)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id2!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							} 
 		           							
 		           							 
 		           						}
 		           							
 		        					}else if (((s_id1==0) | (s_id1==12)) & (s_id2==5)){
 		        					
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id1]=1;  
 		           							exp_day[index_id1]=day_simulation;
 		           							new_exposed++; 
 		           							
 		           							if ((role_id1==6) && (teach_rep[index_t(tc, id1)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id1==6) && (teach_rep[index_t(tc, id1)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id1!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							}  
 		           						}
 		           					}	
 		   	   					}
 	         	    		}
	  					}			 
					}
 	        		        
					for (j=0; j<data->n; j++){
			
 		    			id=data->ID[j];
 		    			role_id=data->classes[j];
 		   				index_id=index_nodes(data, id);
 		    			s_id=status0[index_id];
 		    		
 		    			
 		    			// End of first isolation Q1
 		    			// From Q1 to Q2
 		    			// n_transitions1 = number of suspected nodes tested 
 		    			if ((symptomatic==1) & (n_transitions1>0) & (n_transitions2>=0)){
 		    		
 		    			 // n_transitions1 = number of symp collected
 		    			 // if there is only sympt ON (symptomatic==1)
 		    			 // in n_transitions1= stud + original teacher detected
 		    			 // If if there is sympt ON (symptomatic==1) + prevent. ON (preventive==1)
 		    			 // in n_transitions1= stud + original teacher detected + teacher replaced
 		    			 // I need the information on replced to know if they activate a PREVENTIVE or NOT
 		    			 
 		    		  		for (g=0; g<n_transitions1; g++){
 		    			
 		    					// q1_id=id tested because suspected
 		    					// At the time of the turnaround time, we known the result			
								if((id==q1_id[g]) & (m_absolute==q1_quarter[g]+(param->tau_test/param->deltat))){
 		    				
 		    						//If id is a student
 		    						if (role_id!=6){
 		    							// IF positive go in Q2 and its contacts are OFF
 		    							if (q1_result[g]==1){
 		    						
 		    								new_q2_s++;
 		    							
 		    								detected[index_id]=0;
 		    								quarantine[index_id]=1;
 		    								num_iso[index_id]=1;
 		    								q2_id[n_transitions2]=id;
 		   		        					q2_quarter[n_transitions2]=m_absolute;
 		    						
 		    								q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    								q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    								
 		    								
 		    								n_transitions2++;
 		    							
 		    								// Count the number of additional active classes
 		    								
 		    								if ((reactive == 1) & (reactive_class[index_class(tc, data->classes[index_id])]==0) & (screenstatus[index_id]!=6)){
 		    								
 		    									id_reactive[n_reactive] = id;
 		    									
 		    									if (param->deltar==1440){
 		    										m_reactive[n_reactive] = m_absolute + (num_15min_day - m) + start[0];
 		    									}
 		    									
 		    									if (param->deltar==0){
 		    										
 		    										m_reactive[n_reactive] = m_absolute;
 		    									}
	 		    									
 		    									
 		    									id_reactive=(int*)realloc(id_reactive,(n_reactive+2)*sizeof(int)); 		    										
 		    									m_reactive=(int*)realloc(m_reactive,(n_reactive+2)*sizeof(int));
	 		    									
 		    									
 		    									reactive_class[index_class(tc, data->classes[index_id])]=1;
 		    									n_reactive++;
	 		    									
	 		    								
 		    								}	
	 		    								
 		    								if ((preventive==1) & (regular==0)){
 		    								
 		    									class_q3=data->classes[index_id];
 		    									teach_q3=find(tc, id, class_q3);
 		    								
 		    									// If the class in not already in Q3
 		    				 					if (class_iso[index_t(tc, teach_q3)]==0){
 		    							
 		    										// Turn on if the class is in pre iso
 		    										class_iso[index_t(tc, teach_q3)]=1;
 		    						
 		    										dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    										
 		    										trigger_role=1;
 		    									
 		    										
 		    							
 		    										new_q3=new_q3+dim;
 		    							
 		    									//fprintf(stderr, "teach in Q3: %d\t interval: %d\t %d\t day: %d\t s%d\n", teach_q1, m,m_absolute, day_simulation, status0[index_nodes(data, teach_q1)]);
 		    									}else{
 		    										pre_flag[index_id]=0;
 		    									}
 		    								}

 		    							}else{
 		    								detected[index_id]=0;
 		    								new_neg_s++;
 		    								// If a students is tested negative during preisolation
 		    								// he/she contin in Q3
 		    								if (preventive==1){
 		    									class_q3=data->classes[index_id];
 		    									teach_q3=find(tc, id, class_q3);
 		    									if (class_iso[index_t(tc, teach_q3)]==1){
 		    										pre_flag[index_id]=1;
 		    									}
 		    								}
 		    							}
 		    							// IF id is a teacher:
 		    							// a. original
 		    							// b. replaced
 		    						}else if (role_id==6){
 		    							// IN q1 the original teacher is already replaced
 		    							// At this point, teach_rep==1
 		    							if (teach_rep[index_t(tc, id)]==1){
 		    								// IF positive 
 		    								// Save the moment in which go in Q2
 		    								if (q1_result[g]==1){
 		    							
 		    									new_q2_t++;
 		    								
 		    									q2_id[n_transitions2]=id;
 		   		        						q2_quarter[n_transitions2]=m_absolute;
 		    									//q2_day[n_transitions1]=day_simulation;
 		    				
 		    									q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    									q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    									//q2_day=(int*)realloc(q2_day,(n_transitions2+2)*sizeof(int));
 		    									
 		    									fprintf(stderr, " q2 id: %d\t start Q2 interval: %d\t start Q2 day: %d\n", q2_id[n_transitions2], q2_quarter[n_transitions2], day_simulation);
 		    									
 		    									n_transitions2++;
 		    							
 		    									// Count the number of additional active classes
 		    									
 		    									detected_t[index_t(tc,id)]=0;
 		    									quarantine_t[index_t(tc, id)]=1;
 		    								
 		    									
 		    									
	 		    								
 		    									if ((preventive==1) & (regular==0)){
 		    					
 		    				 						if (class_iso[index_t(tc, id)]==0){
 		    							
 		    											// Turn on if the class is in pre iso
 		    											class_iso[index_t(tc, id)]=1;
 		    											
 		    											dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    											
 		    											trigger_role=2;
 		    						
 		    											
 		    											// If the teacher is the trigger, the 
 		    											// replace teacher is however in
 		    											// isolation with the class
 		    							
 		    											new_q3=new_q3+dim;
 		    										}
 		    										// If a teacher go in in Q2 during a preventive
 		    										// the pre_flag is ON for the replaced teacher
 		    									}
 		    								// Otherwise back to school as clinical
 		    								}else {
 		    							
 		    									status[index_id]=4;
 		    									detected_t[index_t(tc,id)]=0;
 		    									new_neg_t++;
 		    									
 		    									if (preventive==1){
 		    										
 		    										if (class_iso[index_t(tc, id)]==1){
 		    											pre_flag[index_id]=1;
 		    										}
 		    									}
 		    								}
 		    								// teach_rep >1 Sostituto del sostituo
 		    								// When the original back, teach_rep==-9
 		    							}else if (teach_rep[index_t(tc, id)]>1){
 		    								
 		    							
 		    								if (q1_result[g]==1){ 
 		    									new_pos_r++;
 		    								
 		    				 					if ((preventive==1) & (regular==0) & (class_iso[index_t(tc, id)]==0)){
 		    							
 		    										// Turn on if the class is in pre iso
 		    										class_iso[index_t(tc, id)]=1;
 		    						
 		    										dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    										
 		    										trigger_role=3;
 		    										
 		    							
 		    										new_q3=new_q3+dim;
 		    									}
 		    								}else{
 		    									new_neg_r++;
 		    									
 		    									if (preventive==1){
 		    										
 		    										if (class_iso[index_t(tc, id)]==1){
 		    											pre_flag[index_id]=1;
 		    										}
 		    									}
 		    								}
 		    							}
 		    						}
 		    					
 		    					}	
 		    				}
 		    				
 		    				if (reactive == 1){
 		    				
	 		    				for(hg=0; hg < n_reactive; hg++){
	 		    				
	 		    					if((id == id_reactive[hg]) & (m_absolute == m_reactive[hg])){
	 		    					
	 		    					
	 		    						//fprintf(stderr, "REACTIVE day %d\t m %d\t m_abs %d\n", day_simulation, m, m_absolute);
	 		    					
	 		    						reactive_screening(data, tc, param, theta_ps, theta_cs, theta_scs, theta_rps, id, day_simulation, m_absolute, nr, status0, screenstatus, start_q2, quarantine, detected, classtotest, indtotest, when, counter_prodro, counter_infect, counter_rp, fp16, fp18);
	 		    						
	 		    						if (data->classes[index_id]!=6){
	 		    							reactive_class[index_class(tc, data->classes[index_id])]=0;
	 		    						}else{
	 		    							reactive_class[index_t(tc, id)]=0;
	 		    						}
	 		    					}
	 		    					
	 		    		  		}
	 		    		  		
	 		    		  		if (param->delay!=0){
	 		    		  		
		 		    		  		for (ncl=0; ncl< tc->nc; ncl++){
		 		    		  			if (((classtotest[ncl]==1) & (m_absolute==when[ncl])) |
		 		    		  			 ((classtotest[ncl]==2) & (m_absolute==when[ncl]+ 2 * num_15min_day)) ){
		 		    		  				post_reactive(data, tc, param, tc->clas[ncl], day_simulation, m_absolute, nr, status0, screenstatus, start_q2, start_rq2, teach_rtested, teach_rep, quarantine, detected, indtotest, theta_ps, theta_cs, theta_scs, theta_rps, counter_prodro, counter_infect, counter_rp, fp17, fp19);
		 		    		  				if (classtotest[ncl]==1){
		 		    		  					classtotest[ncl]=2;
		 		    		  				}
		 		    		  				
		 		    		  				if(m_absolute==when[ncl]+2*num_15min_day){
		 		    		  					classtotest[ncl]=0;
		 		    		  					
		 		    		  					for (z=0; z<data->n; z++){
		 		    		  						if((data->classes[z]==tc->clas[ncl]) 
		 		    		  						& (indtotest[z]==1)){
		 		    		  							indtotest[z]=0;
		 		    		  						}
		 		    		  						
		 		    		  						if((data->classes[z]==6) 
		 		    		  						& (data->ID[z]!=tc->ID[ncl])
		 		    		  						& (indtotest[z]==1)){
		 		    		  							indtotest[z]=0;
		 		    		  						}
		 		    		  						
		 		    		  					}
		 		    		  				}
		 		    		  			}
		 		    		  		}
	 		    		  		}
 		    		  		}
 		    		  		
 		    		  		// End of quarantine for IC(d) in Q2
 		    		  	  	for(hh=0; hh<n_transitions2; hh++){
 		    			
 		    		  			if( (id==q2_id[hh]) & (m_absolute == q2_quarter[hh]+(param->delta_q/param->deltat) ) ){
 		   				
 		   						if (role_id!=6){ 
 		   					
 		   							quarantine[index_id]=0;
 		   							
 		   						
 		   						}else if(role_id==6){
 		    					
 		    							// We test the original teacher at the end of Q2, and we assume that 
 		    							// status of the original at the end is equal to R+ (8)
 		    							// So we test using a sensitivity teta_r+
 		    						
 		    							// Quarantine_t and teach_rep are turnd off only if the result of
 		    							// the test is negative
 		    						
 		    							r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		    							
 		    							//fprintf(stderr, "q2 id:%d\t start Q2: %d\t end Q2: %d\t (day:%d)\t role: %d\t result:%d\n", id, q2_quarter[hh], m_absolute, day_simulation, role_id, r);
 		    						
 		    							// If during quarantine the replace teacher goes in isoaltion with 
 		    							// the class when the teacher come back (i.e the result of the test is negative),
 		    							// preflag is 0
 		    							//if((pre_flag[index_id]==1) & (r==0)){
 		    							//	pre_flag[index_id]=0;
 		    							//}
 		    						
 		    							// Counter on the number of tests on the original teacher
 		    							// before back to school
 		    							
 		    							n_test_t++;
 		    						
 		    							if (r==1){
 		    								n_test_t_p++;
 		    							}else{
 		    								n_test_t_n++;
 		    							}
 		    						
 		    						}
 		    					
 		    					}
 		    		  	  	}			
 		    			}
 		    		
 		    			// End of preventive isolation	
 		    			if (preventive==1){
 		    			
 		    				if ((pre_flag[index_id]==1) & (m_absolute==pre_time[1][index_id]+(param->delta_q/param->deltat))){
 		       			
 		       					if (role_id!=6){
 		       				
 		       						pre_flag[index_id]=0;
 		       						tea=find(tc, id, data->classes[index_id]);
 		        			
 		       						if(class_iso[index_t(tc, tea)]==1)
 		       							class_iso[index_t(tc, tea)]=0;
 		       							
 		       						
 		       					
 		       					}else if ((role_id==6) & (quarantine_t[index_t(tc,id)]==0)){
 		       					
 		        					r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		        					//fprintf(stderr, "teach id in Q3:%d\t start Q3: %d\t (day:%d)\t  end Q3: %d\t (day:%d)\t role: %d\t result:%d\n", id, pre_time[1][index_id], pre_time[0][index_id], m_absolute, day_simulation, role_id, r);
 		        					
 		        					// Counter on the number of tests on the original teacher 
 		        					// before back to school
 		        					n_test_t++;
 		        							
 		        					if (r==1){
 		    							n_test_t_p++;
 		    						}else{
 		    							n_test_t_n++;
 		    						}
 		        		
 		        			
 		       					}
 		       					pre_flag[index_id]=0;
 		       					
 		       					/*for (ttts=0; ttts<tc->nc; ttts++){
	       							fprintf(stderr, "flag class in iso: %d\t %d\n", tc->clas[ttts], class_iso[ttts]);
	       							
	       						}*/
 		       				}
 		       			}
 		       			if ((regular==1) | (reactive==1) ){
 		       				//start q2 hold only for original teach
 		       				
	  					if ((screenstatus[j]==6) & ((m_absolute==start_q2[j]) | (m_absolute==start_rq2[j]))){
	  					
	  						
	  						new_q2_reg++;
	  						if ((role_id!=6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
	  						
	  							
	  							
	  							new_q2_regs++;
	  							quarantine[j]=1;
	  							
	  							if ((preventive == 1) & 
	  							(num_pos_class[index_class(tc, data->classes[index_id])]>=3) &
	  							(class_iso[index_class(tc, data->classes[index_id])]==0)){
	  							
	  								
		 		    		  			 preisolation(data, tc, param, id, role_id, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
		 		    		  		}
	  							
	  						}else if ( (role_id==6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
	  						
	  							
	  							
	  							new_q2_regt++;
	  							quarantine_t[index_t(tc,id)]=1;
	  							status[j]=0;
	  						}else if ((m_absolute==start_rq2[j]) & (start_rq2[j]!=0) & (role_id==6) & (teach_rtested[index_t(tc, id)]>1)){
	  						
	  							new_q2_regr++;
	  							status[j]=0;
	  							
	  							
	  							
	  							
	  								
	  						}
	  						screenstatus[j]=-9;
	  						
	  					}else if ((m_absolute==start_q2[index_id]+param->delta_q/param->deltat) & (start_q2[j]!=0)){
 		       				 	
 		       				 	if (role_id!=6){
 		       				 		quarantine[index_id]=0;
 		       					}else if (role_id==6){
 		       					
 		       						r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		        					
 		        					// Counter on the number of tests on the original teacher 
 		        					// before back to school
 		        					n_test_t++;
 		        							
 		        					if (r==1){
 		    							n_test_t_p++;
 		    						}else{
 		    							n_test_t_n++;
 		    							teach_neg_reg[n_reg]=id;
 		    							teach_neg_reg=(int*)realloc(teach_neg_reg,(n_reg+2)*sizeof(int));
 		    							n_reg++;
 		    						}
 		    						
 		    					}
 		       				 }
 		       			}
 		       			
 		    			 
 		    			// End of second isolation
 		    			if ((symptomatic==1) | ((symptomatic==1) & (preventive==1)) | (regular==1)){
 		    				if (role_id==6){
 		    					if ((teach_2iso[index_t(tc, id)]==1) 
 		    					& (m_absolute==teach_2isom[index_t(tc, id)]+param->delta_q/param->deltat)){
 		    					
 		    						status[index_id]=9;
 		    						teach_2iso[index_t(tc, id)]=-9;
 		    						quarantine_t[index_t(tc, id)]=0;
 		    						teach_rep[index_t(tc,id)]=-9;
 		    						
 		    						
 		    					}
 		    				}			
 		        		}

 		        		// INDIPENDENT TRANSITIONs:
 						// TRANSITION FROM E->IP 				
 						// TRANSITION FROM IP->IC_detected		
 						// TRANSITION FROM IP->IC
 						// TRANSITION FROM IP->ISC		
 						// TRANSITION FROM Q1->Q2
 						// TRANSITION FROM IC->R+ o ISC->R+ 	
 						// TRANSITION FORM ICDI-> R+		
 						// TRANSITION FORM R+ -> R
 			
 		    			if( (s_id==1) & (counter_latent[index_id] < param->shape_latent)){
 		    				random = gsl_ran_flat(RNG,0,1);
 		    				//fprintf(stderr,"2HERE\n");
 		    				if(random < (1 - exp(-(param->shape_latent * param->eps) * param->deltat))){
 		    				
 		    					status[index_id]=1;  // 2 for prodromic phas
 		    					counter_latent[index_id]=counter_latent[index_id]+1;
 		    					
 		    					if (counter_latent[index_id] == param->shape_latent){
 		    						status[index_id]=2;
 		    						
 		    						new_pro++;
 		    						if (role_id!=6){
	 		    						new_pro_s++;
	 		    					}else if (role_id==6){
	 		    						if (teach_rep[index_t(tc,id)]==0){
	 		    							new_pro_t++;
	 		    						}else{
	 		    							new_pro_r++;
	 		    						}
	 		    					}
 		    						counter_latent[index_id]=0;
 		    						
 		    					}
 		    				}
 		    			
 		    			} else if ((s_id==2) & (counter_prodro[j] < param->shape_prodro)){
 		    				random=gsl_ran_flat(RNG,0,1);
 		    				counter_latent[index_id]=0;
 		    				
	 		    			if (random< (1- exp(-(param->shape_prodro *param->mu_p)*param->deltat))){
	 		    				
 		    					status[j] = 2;
 		    					counter_prodro[j] = counter_prodro[j] +1;
	 		    				
	 		    				if (counter_prodro[index_id] == param->shape_prodro){
	 		    					
	 		    					
	 		    					
	 		    					//counter_prodro[j]=0;
	 		    					random1=gsl_ran_flat(RNG,0,1);
	 		    					// 6 to identified teachers
	 		    					if ( ((role_id==6) & (random1<psub_tc->novax[nw]) & (vaccinated_2doses[index_id]==0) 
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==0))|
	 		    					 ((role_id==6) & (random1<psub_tc->twodoses[nw]) & (vaccinated_2doses[index_id]==0) 
	 		    					 & (vaccinated_booster[index_id]==0)& (r_delta[index_id]==1))|
	 		    					((role_id==6) & (random1<psub_tc->twodoses[nweek_2doses[index_id]]) & (vaccinated_2doses[index_id]==1)) |
	 		    					((role_id==6) & (random1<psub_tc->booster[nweek_booster[index_id]]) & (vaccinated_booster[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->novax[nw]) & (vaccinated_2doses[index_id]==0)
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==0)) |
	 		    					 ((role_id!=6) & (random1<psub_st->twodoses[nw]) & (vaccinated_2doses[index_id]==0)
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->twodoses[nweek_2doses[index_id]]) & (vaccinated_2doses[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->booster[nweek_booster[index_id]]) & (vaccinated_booster[index_id]==1))
	 		    					  ){
	 		    						//fprintf(stderr, "Inside subclinical\n");
	 		    						status[index_id]=5; 
	 		    						new_is++;
	 		    						if (role_id!=6){
	 		    							new_is_s++;
	 		    						}else if (role_id==6){
	 		    							if (teach_rep[index_t(tc,id)]==0){
	 		    								new_is_t++;
	 		    							}else{
	 		    								new_is_r++;
	 		    							}
	 		    						}
	 		    					//5 for subclinical case
	 		    					} else {
	 		    					
	 		    						
	 								random2=gsl_ran_flat(RNG,0,1);
	 								// Clinical detected
	 									
	 		    						if (((role_id==6) & (random2<pd[1])) | ((role_id!=6) & (random2<pd[0])) ){
	 		    					
	 		    							
	 		    							new_ic_d++;
	 		    							
	 		    							random3=gsl_ran_flat(RNG,0,1);
	 		    							if (role_id!=6){
	 		    								n_test_symp_st++;
	 		    							}else{
	 		    								n_test_symp_tc++;
	 		    							}
	 		    							n_test_symp++;
	 		    							//fprintf(stderr, "%d\t %f\n", counter_infect[index_id], theta_cs[counter_infect[index_id]]);
	 		    							if (((random3<theta_cs[0][counter_infect[index_id]]) & (data->classes[index_id]!=6)) | ((random3<theta_cs[1][counter_infect[index_id]]) & (data->classes[index_id]==6))){
	 		    							
	 		    								if (role_id!=6){
	 		    									fprintf(stderr, "%d\t %f\t %f\n", counter_infect[index_id], theta_cs[0][counter_infect[index_id]], theta_cs[0][0]);
	 		    								}else{
	 		    									fprintf(stderr, "%d\t %f\t %f\n", counter_infect[index_id], theta_cs[1][counter_infect[index_id]], theta_cs[0][0]);
	 		    								}
	 		    					
	 		    								n_test_symp_p++;
	 		    								result=1;
	 		    								if (role_id!=6){
	 		    									role=1;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]==0)){
	 		    									role=2;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]>=1)){
	 		    									role=3;
	 		    								}
	 		    								
	 		    								
	 		    								//IF stud/teach/repl_teach
	 		    								if(role_id!=6){ 
	 		    								
	 		    									q1_id[n_transitions1]=id;
	 		    									q1_quarter[n_transitions1]=m_absolute;
	 		    									q1_result[n_transitions1]=1;
	 		    									
	 		    									//fprintf(stderr, "n_transitions1:%d\n", n_transitions1);
	 		   									
	 		   									
	 		    									q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    									q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   									q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   						
	 		   									n_transitions1++;
	 		   									
	 		   								
	 		   									status[index_id]=4;
	 		   									detected[index_id]=1;
	 		   									
	 		   									new_ic_d_s++;
	 		   									
	 		   									// If identified during a preventive	
	 		   									if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    										pre_flag[index_id]=0;
	 		    									}
	 		    									
	 		    								}else if (role_id==6){
	 		    								
	 		    									if (teach_rep[index_t(tc, id)]==0){
	 		    									
	 		    										q1_id[n_transitions1]=id;
	 		    										q1_quarter[n_transitions1]=m_absolute;
	 		    										q1_result[n_transitions1]=1;
	 		    									
	 		    										q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    										q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   										q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   										
	 		   										
	 		   										n_transitions1++;
	 		   										
	 		    										detected_t[index_t(tc,id)]=1;
	 		   										teach_rep[index_t(tc,id)]=1;
	 		   										status[index_id]=0;
	 		   										
	 		   										new_ic_d_t++;
	 		    										
	 		    										status_orig=4;
	 		    										reason=1;
	 		    										
	 		    										
	 		    										// A teacher can be replaced because symp.
	 		    										
	 		    										// If identified during a preventive	
	 		   										if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    											pre_flag[index_id]=0;
	 		    										}
	 		    									}else if (teach_rep[index_t(tc, id)]>=1){
	 		    									
	 		    										
	 		    										status[index_id]=0;
	 		    										new_ic_d_r++;
	 		    									
	 		    										status_orig=4;
	 		    										reason=4;
	 		    								
	 		    										teach_rep[index_t(tc,id)]=2;
	 		    										
	 		   										if (preventive==1){
	 		   										
	 		   											q1_id[n_transitions1]=id;
	 		    											q1_quarter[n_transitions1]=m_absolute;
	 		   											q1_result[n_transitions1]=1;
	 		   										
	 		   											q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    											q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   											q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));	
	 		   											
	 		   											n_transitions1++;
	 		   										}
	 		    									}
	 		    										
	 		   								}
	 		   								
	 		    								
	 		    							
	 		    							// (that eventually triggers a protocol)
	 		    							
	 		    								
	 		    							} else {
	 		    							
	 		    								n_test_symp_n++;
	 		    								/*result=0;
	 		    								if (role_id!=6){
	 		    									role=1;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]==0)){
	 		    									role=2;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]>=1)){
	 		    									role=3;
	 		    								}
	 		    								*/
	 		    								
	 		    								if(role_id!=6){
	 		    								 												q1_id[n_transitions1]=id;
	 		    									q1_quarter[n_transitions1]=m_absolute;
	 		    									q1_result[n_transitions1]=0;
	 		    									
	 		    									q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    									q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   									q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   									
	 		   									n_transitions1++;
	 		   									
	 		   									status[index_id]=4;
	 		   									detected[index_id]=1;
	 		   									new_ic_d_s++;
	 		   										
	 		   									if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    										pre_flag[index_id]=0;
	 		    									}
	 		    								}else if (role_id==6){
	 		   									
	 		   									if (teach_rep[index_t(tc, id)]==0){
	 		   									
	 		   										q1_id[n_transitions1]=id;
	 		    										q1_quarter[n_transitions1]=m_absolute;
	 		    										q1_result[n_transitions1]=0;
	 		    									
	 		    										q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    										q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   										q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   										
	 		   										n_transitions1++;
	 		   									
	 		   										detected_t[index_t(tc,id)]=1;
	 		   										status[index_id]=0;
	 		   										
	 		   										new_ic_d_t++;
	 		    										
	 		    										status_orig=4;
	 		    										reason=1;
	 		    										
	 		    										
	 		    										
	 		    								
	 		    										teach_rep[index_t(tc,id)]=1;
	 		    										
	 		    										if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    											pre_flag[index_id]=0;
	 		    										}
	 		   									}else if (teach_rep[index_t(tc, id)]>=1){
	 		    									
	 		    										status[index_id]=0;
	 		    										new_ic_d_r++;
	 		    									
	 		    										status_orig=4;
	 		    										reason=4;
	 		    								
	 		    										teach_rep[index_t(tc,id)]=2;
	 		    										
	 		   										if (preventive==1){
	 		   										
	 		   											q1_id[n_transitions1]=id;
	 		    											q1_quarter[n_transitions1]=m_absolute;
	 		   											q1_result[n_transitions1]=0;
	 		   										
	 		   											q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    											q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   											q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));			
	 		   											
	 		   											
	 		   											n_transitions1++;
	 		   										}
	 		   									}
	 		   								}
	 		    							
	 		    							}
	 		    						// Clinical not detected 
	 		    						}else{
	 		    						
	 		    							
	 		    							
	 		    							status[index_id]=4;
	 		    							new_ic++;
	 		    							
	 		    							if (role_id!=6){
	 		    								new_ic_s++;
	 		    								detected[index_id]=0;
	 		    							}else if (role_id==6){
	 		    								if (teach_rep[index_t(tc,id)]==0){
	 		    									new_ic_t++;
	 		    									detected_t[index_t(tc,id)]=0;
	 		    								}else{
	 		    									new_ic_r++;
	 		    								}
	 		    							}
	 		    						}   
	 							}			
	 					 	}
	 					 }
 					 	//fprintf(stderr, "end prodromic");
 		    			} else if(((s_id==4) & (counter_infect[j]<param->shape_infect)) | 
 		    			((s_id==5) & (counter_infect[j]<param->shape_infect))){	
 		    		        	random=gsl_ran_flat(RNG,0,1);	
 		    		        	counter_prodro[index_id]=0;
	    					if(random< 1 - exp(-(param->shape_infect * param->mu)*param->deltat)){
	    						if (s_id==4){
	    							status[index_id]=4;
	    						}else if (s_id==5){
	    							status[index_id]=5;
	    						}
	    						counter_infect[j]=counter_infect[j]+1;
	    						if (counter_infect[j] == param->shape_infect){
	    							status[index_id]=8;          // 8 for recovery but still positive to the te
	    							//counter_infect[j] = 0;
	    						}
	    					}	

 		    			} else if ((s_id==8) & (counter_rp[index_id]<param->shape_rp)){
 		    				random4=gsl_ran_flat(RNG,0.0,1.0);
 		    				counter_infect[j] = 0;
 		    				if (role_id!=6){	
 		    					if(random4 < (1 - exp(-(param->shape_rp * param->mu_rp)*param->deltat))){
 		    						counter_rp[index_id]=counter_rp[index_id]+1;
 		    						if (counter_rp[index_id]==param->shape_rp){
 		    							status[index_id]=9;
 		    							counter_rp[index_id] = 0;
 		    						}
 		    					}
 		    				}else if(role_id==6){
 		    				
 		    					if (symptomatic==1){
 		    						for(hh=0; hh<n_transitions2; hh++){
 		    							if((id==q2_id[hh]) & (random4<1 - exp(-(param->shape_rp * param->mu_rp1)*param->deltat))){
 		    								counter_rp[index_id]=counter_rp[index_id]+1;
		 		    						if (counter_rp[index_id]==param->shape_rp){
		 		    							status[index_id]=9;
		 		    							counter_rp[index_id] = 0;
		 		    						}
		 		    								
 		    							}else if ((id!=q2_id[hh]) & (random4<1 - exp(-(param->shape_rp * param->mu_rp)*param->deltat))){
 		    								counter_rp[index_id]=counter_rp[index_id]+1;
		 		    						if (counter_rp[index_id]==param->shape_rp){
		 		    							status[index_id]=9;
		 		    							counter_rp[index_id] = 0;
		 		    						}
 		    							}
 		    						}
 		    					}
 		    					
 		    					
 		    				}
 		    			}    	
 		    		}	
 		
 				
 			
 			
 					
 		
 					save_status(data,status,status0); 
	  	
	  			}
	  	

	  			fprintf(fp5, "%d\t %d\t %d\t %d\t %d\t %d\n", nr, day_simulation, new_exposed, new_exp_s, new_exp_t, new_exp_r);
	  			fflush(fp5);
	  				
	  			
	  			d2=d2+week;
	  	                           
      			break;
      		case 3:	
    		
    			new_exposed=0;
    			new_pro=0;
    			new_ic_d=0;
    			new_ic=0;
    			new_is=0;
    			new_q3=0;
    		
    			// New intro in each compartments divided by type: STUDENTS/ TEACHERS/ REPLACED TEACHERS
    			new_exp_s=0;new_exp_t=0;new_exp_r=0;
				new_pro_s=0;new_pro_t=0;new_pro_r=0;new_ic_s=0;
    			new_ic_t=0;new_ic_r=0;new_ic_d_s=0;new_ic_d_t=0;new_ic_d_r=0;
				new_is_s=0;new_is_t=0;new_is_r=0;
				new_q2_s=0;new_q2_t=0;new_pos_r=0;new_q2_reg=0;new_q2_regs=0;new_q2_regt=0;new_q2_regr=0;new_neg_s=0;new_neg_t=0;new_neg_r=0;
				n_test_symp=0;n_test_symp_p=0; n_test_symp_n=0;n_test_symp_st=0; n_test_symp_tc=0;
      			n_test_t=0;n_test_t_p=0;n_test_t_n=0;
    		
	  			
    			for (m=0; m<num_15min_day; m++){
    			
    				m_absolute=num_15min_day*day_simulation + m; //progression of 15 minutes step from the first day to the last one
	        
					n_susc_s=0;
					n_susc_t=0;
      				// Intro of 1 case if there are still susceptible individuals
      				
      			 	for (i=0; i<data->n; i++){
      					if (((status0[i]==0) | (status0[i]==12)) & (data->ID[i]!=6)){
      						n_susc_s++;
      					}else if (((status0[i]==0) | (status0[i]==12)) & (data->ID[i]==6)){
	  						n_susc_t++;
	  					}
      				}

					if ((n_susc_s!=0) & ((*introst)[nr].weeks[nw]>0) & (n_susc_s>=(*introst)[nr].weeks[nw])){
      			
						for (ni=0; ni<(*introst)[nr].weeks[nw]; ni++){
							if (((day_simulation % (week) + 1)==introductions_days_st[ni]) & (m==introductions_quarter_st[ni])){
 
								typ=weekly_introduction_st(data, tc, status, status0, (*introst)[nr].weeks[nw], double_presence, teach_rep, exp_day, day_simulation);
									
								new_exposed++;
								
								if (typ==1){
									new_exp_s++;
								}	
							}
						}
					}
					if ((n_susc_t!=0) & ((*introtc)[nr].weeks[nw]>0) & (n_susc_t>=(*introtc)[nr].weeks[nw])){
					
						for (ni=0; ni<(*introtc)[nr].weeks[nw]; ni++){
							if (((day_simulation % (week) + 1)==introductions_days_tc[ni]) & (m==introductions_quarter_tc[ni])){
								  
								typ=weekly_introduction_tc(data, tc, status, status0, (*introtc)[nr].weeks[nw], double_presence, teach_rep, exp_day, day_simulation);
									
								new_exposed++;
								
								if (typ==2){
									new_exp_t++;
								}else if (typ==3){
									new_exp_r++;
								}		
							}
						}
					}
      			
					if ((m>=start[day_simulation]) & (m<=end[day_simulation])){
				
					
	  		    			
						for (i=0; i<ct->n1; i++){
 	  	
 		  	  	  			if ((ct->step[i]==m) & (ct->days[i]==d3)){
 		  	  	  
 		  	  	  			
 			
 		        				id1=ct->ID1[i];
 		        				id2=ct->ID2[i];
 		        		
 		        		
 		        				index_id1=index_nodes(data, id1); 
 		        				index_id2=index_nodes(data, id2); 
 		        
  		        				s_id1=status0[index_id1];
 		        				s_id2=status0[index_id2];
 		        		
 		        				role_id1=data->classes[index_id1];
 		        				role_id2=data->classes[index_id2];
 		        		
 		        				weight=ct->weight[i];
 		        		
 		        				// rbeta e sigma definition
 		        				// Reduction on susceptibility and infectiousness is different for adults and children
 		        				// class = 6 is used to identified teachers
 		        		
 		        				int is_teacher1 = (ct->class1[i] == 6);
								int is_teacher2 = (ct->class2[i] == 6);
							
								int v2dose1 = vaccinated_2doses[index_id1];
								int vboost1 = vaccinated_booster[index_id1];
								int n2dose1 = nweek_2doses[index_id1];
								int nboost1 = nweek_booster[index_id1];
							
								int v2dose2 = vaccinated_2doses[index_id2];
								int vboost2 = vaccinated_booster[index_id2];
								int n2dose2 = nweek_2doses[index_id2];
								int nboost2 = nweek_booster[index_id2];
								
								int delta2 = r_delta[index_id2];
								int delta1 = r_delta[index_id1];
 		        		
 		        				// rbeta e sigma definition
 		        				// Reduction on susceptibility and infectiousness is different for adults and children
 		        				// class = 6 is used to identified teachers
								if ((s_id1 == 0 || s_id1 == 12) && (s_id2 == 2 || s_id2 == 5 || s_id2 == 4)) {
									
									sigma = compute_sigma(s_id1, is_teacher1, vboost1, v2dose1, nboost1, n2dose1,
											sigma_st, sigma_tc, param->sigma_omicron, nw);
											
									rbeta = compute_rbeta(s_id2, is_teacher2, vboost2, v2dose2, delta2,
										nboost2, n2dose2, rbeta_st, rbeta_tc, nw);
										
								} else if ((s_id2 == 0 || s_id2 == 12) && (s_id1 == 2 || s_id1 == 5 || s_id1 == 4)) {
									sigma = compute_sigma(s_id2, is_teacher2, vboost2, v2dose2, nboost2, n2dose2,
										sigma_st, sigma_tc, param->sigma_omicron, nw);
										
									rbeta = compute_rbeta(s_id1, is_teacher1, vboost1, v2dose1, delta1,
									nboost1, n2dose1, rbeta_st, rbeta_tc, nw);
								}
 		        				
 		        				//TRANSITION FROM S->E
 		        				//if (((symptomatic==1) & (preventive==1)) | (symptomatic==1) | (regular==1)){
 		        		
 		        				// pre_flag is used to check if contacts are on on off 
 		        				// depending on presence of isolation or not 
 		        				// If contacts are on (pre_flag=0, individuals in q3) 
 		        				// this means that the infectious node not be in pre isolation
 		        				
 		        				
 		        			   	if((((pre_flag[index_id1]==0) & (detected[index_id1]==0) & (quarantine[index_id1]==0))
 		        				 & ((pre_flag[index_id2]==0) & (detected[index_id2]==0) & (quarantine[index_id2]==0)))
 		        				 ){
 		        						
 		        					if((s_id1==2) & ((s_id2==0) | (s_id2==12))){
 		        			
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id2]=1;  
 		           							exp_day[index_id2]=day_simulation;
 		           							new_exposed++;  
 		           							
 		           							
 		      
 		           							if ((role_id2==6) && (teach_rep[index_t(tc, id2)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id2==6) && (teach_rep[index_t(tc, id2)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id2!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							}
 		           						
 		           							
 		           						}
 		           						
 		        					} else if (((s_id1==0) | (s_id1==12))& (s_id2==2)){
 		        					
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        								
 		           							status[index_id1]=1;  
 		           							exp_day[index_id1]=day_simulation;
 		           							new_exposed++;
 		           								
 		           							
 		           								
 		           							if ((role_id1==6) && (teach_rep[index_t(tc, id1)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id1==6) && (teach_rep[index_t(tc, id1)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id1!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							}  
 		           							
 		           						}
 		           						
 		           				 	// 4 clinical
 		        					} else if ((s_id1==4) & ((s_id2==0) | (s_id2==12)) ){
 		        						
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id2]=1;  
 		           							exp_day[index_id2]=day_simulation;
 		           							new_exposed++; 
 		           							
 		           							if ((role_id2==6) && (teach_rep[index_t(tc, id2)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id2==6) && (teach_rep[index_t(tc, id2)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id2!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		          							} 
 		           							
 		           						}
 		           					
 		           							
 		        					}else if (((s_id1==0) | (s_id1==12)) & (s_id2==4)){
 		        						
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id1]=1; 
 		           							exp_day[index_id1]=day_simulation; 
 		           							new_exposed++; 
 		           							
 		           							if ((role_id1==6) && (teach_rep[index_t(tc, id1)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id1==6) && (teach_rep[index_t(tc, id1)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id1!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							} 
 		           							
 		           						}
 		           							
 		           					// 5 subclinical
 		        					} else if ((s_id1==5) & ((s_id2==0) | (s_id2==12))){
 		        				
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id2]=1; 
 		           							exp_day[index_id2]=day_simulation; 
 		           							new_exposed++;  
 		           						
 		           							if ((role_id2==6) && (teach_rep[index_t(tc, id2)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id2==6) && (teach_rep[index_t(tc, id2)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id2!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							} 
 		           							
 		           							 
 		           						}
 		           							
 		        					}else if (((s_id1==0) | (s_id1==12)) & (s_id2==5)){
 		        					
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id1]=1;  
 		           							exp_day[index_id1]=day_simulation;
 		           							new_exposed++; 
 		           						
 		           							if ((role_id1==6) && (teach_rep[index_t(tc, id1)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id1==6) && (teach_rep[index_t(tc, id1)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id1!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							}  
 		           						}
 		           					}	
 		   	   					}
 	         	    		}
	  					}			 
					}
 	        		        
					for (j=0; j<data->n; j++){
			
 		    			id=data->ID[j];
 		    			role_id=data->classes[j];
 		   				index_id=index_nodes(data, id);
 		    			s_id=status0[index_id];
 		    		
 		    			
 		    			// End of first isolation Q1
 		    			// From Q1 to Q2
 		    			// n_transitions1 = number of suspected nodes tested 
 		    			if ((symptomatic==1) & (n_transitions1>0) & (n_transitions2>=0)){
 		    		
 		    			 // n_transitions1 = number of symp collected
 		    			 // if there is only sympt ON (symptomatic==1)
 		    			 // in n_transitions1= stud + original teacher detected
 		    			 // If if there is sympt ON (symptomatic==1) + prevent. ON (preventive==1)
 		    			 // in n_transitions1= stud + original teacher detected + teacher replaced
 		    			 // I need the information on replced to know if they activate a PREVENTIVE or NOT
 		    			 
 		    		  	for (g=0; g<n_transitions1; g++){
 		    			
 		    					// q1_id=id tested because suspected
 		    					// At the time of the turnaround time, we known the result			
							if((id==q1_id[g]) & (m_absolute==q1_quarter[g]+(param->tau_test/param->deltat))){
 		    				
 		    						//If id is a student
 		    						if (role_id!=6){
 		    							// IF positive go in Q2 and its contacts are OFF
 		    							if (q1_result[g]==1){
 		    						
 		    								new_q2_s++;
 		    							
 		    								detected[index_id]=0;
 		    								quarantine[index_id]=1;
 		    								num_iso[index_id]=1;
 		    								q2_id[n_transitions2]=id;
 		   		        					q2_quarter[n_transitions2]=m_absolute;
 		    						
 		    								q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    								q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    								
 		    								
 		    								n_transitions2++;
 		    							
 		    								// Count the number of additional active classes
 		    								
 		    								
 		    								if ((reactive == 1) & (reactive_class[index_class(tc, data->classes[index_id])]==0) & (screenstatus[index_id]!=6)){
 		    								
 		    									id_reactive[n_reactive] = id;
 		    									
 		    									if (param->deltar==1440){
 		    										m_reactive[n_reactive] = m_absolute + (num_15min_day - m) + start[0];
 		    									}
 		    									
 		    									if (param->deltar==0){
 		    										
 		    										m_reactive[n_reactive] = m_absolute;
 		    									}
	 		    									
 		    									id_reactive=(int*)realloc(id_reactive,(n_reactive+2)*sizeof(int)); 		    										
 		    									m_reactive=(int*)realloc(m_reactive,(n_reactive+2)*sizeof(int));
	 		    									
 		    									
 		    									reactive_class[index_class(tc, data->classes[index_id])]=1;
 		    									n_reactive++;
	 		    									
	 		    								
 		    								}	
	 		    								
 		    								if ((preventive==1) & (regular==0)){
 		    								
 		    									class_q3=data->classes[index_id];
 		    									teach_q3=find(tc, id, class_q3);
 		    								
 		    									// If the class in not already in Q3
 		    				 					if (class_iso[index_t(tc, teach_q3)]==0){
 		    							
 		    										// Turn on if the class is in pre iso
 		    										class_iso[index_t(tc, teach_q3)]=1;
 		    						
 		    										dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    										
 		    										
 		    										trigger_role=1;
 		    									
 		    										
 		    							
 		    										new_q3=new_q3+dim;
 		    							
 		    									//fprintf(stderr, "teach in Q3: %d\t interval: %d\t %d\t day: %d\t s%d\n", teach_q1, m,m_absolute, day_simulation, status0[index_nodes(data, teach_q1)]);
 		    									}else{
 		    										pre_flag[index_id]=0;
 		    									}
 		    								}

 		    							}else{
 		    								detected[index_id]=0;
 		    								new_neg_s++;
 		    								// If a students is tested negative during preisolation
 		    								// he/she contin in Q3
 		    								if (preventive==1){
 		    									class_q3=data->classes[index_id];
 		    									teach_q3=find(tc, id, class_q3);
 		    									if (class_iso[index_t(tc, teach_q3)]==1){
 		    										pre_flag[index_id]=1;
 		    									}
 		    								}
 		    							}
 		    							// IF id is a teacher:
 		    							// a. original
 		    							// b. replaced
 		    						}else if (role_id==6){
 		    							// IN q1 the original teacher is already replaced
 		    							// At this point, teach_rep==1
 		    							if (teach_rep[index_t(tc, id)]==1){
 		    								// IF positive 
 		    								// Save the moment in which go in Q2
 		    								if (q1_result[g]==1){
 		    							
 		    									new_q2_t++;
 		    								
 		    									q2_id[n_transitions2]=id;
 		   		        						q2_quarter[n_transitions2]=m_absolute;
 		    									//q2_day[n_transitions1]=day_simulation;
 		    				
 		    									q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    									q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    									//q2_day=(int*)realloc(q2_day,(n_transitions2+2)*sizeof(int));
 		    									
 		    									fprintf(stderr, " q2 id: %d\t start Q2 interval: %d\t start Q2 day: %d\n", q2_id[n_transitions2], q2_quarter[n_transitions2], day_simulation);
 		    									
 		    									n_transitions2++;
 		    							
 		    									// Count the number of additional active classes
 		    									
 		    									detected_t[index_t(tc,id)]=0;
 		    									quarantine_t[index_t(tc, id)]=1;
 		    									
 		    									
 		    								
 		    									if ((preventive==1) & (regular==0)){
 		    					
 		    				 						if (class_iso[index_t(tc, id)]==0){
 		    							
 		    											// Turn on if the class is in pre iso
 		    											class_iso[index_t(tc, id)]=1;
 		    											
 		    											dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    											
 		    											trigger_role=2;
 		    						
 		    											
 		    											// If the teacher is the trigger, the 
 		    											// replace teacher is however in
 		    											// isolation with the class
 		    							
 		    											new_q3=new_q3+dim;
 		    										}
 		    										// If a teacher go in in Q2 during a preventive
 		    										// the pre_flag is ON for the replaced teacher
 		    									}
 		    								// Otherwise back to school as clinical
 		    								}else {
 		    							
 		    									status[index_id]=4;
 		    									detected_t[index_t(tc,id)]=0;
 		    									new_neg_t++;
 		    									
 		    									if (preventive==1){
 		    										
 		    										if (class_iso[index_t(tc, id)]==1){
 		    											pre_flag[index_id]=1;
 		    										}
 		    									}
 		    								}
 		    								// teach_rep >1 Sostituto del sostituo
 		    								// When the original back, teach_rep==-9
 		    							}else if (teach_rep[index_t(tc, id)]>1){
 		    								
 		    							
 		    								if (q1_result[g]==1){ 
 		    									new_pos_r++;
 		    								
 		    				 					if ((preventive==1) & (regular==0) & (class_iso[index_t(tc, id)]==0)){
 		    							
 		    										// Turn on if the class is in pre iso
 		    										class_iso[index_t(tc, id)]=1;
 		    						
 		    										dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    										
 		    										trigger_role=3;
 		    										
 		    							
 		    										new_q3=new_q3+dim;
 		    									}
 		    								}else{
 		    									new_neg_r++;
 		    									
 		    									if (preventive==1){
 		    										
 		    										if (class_iso[index_t(tc, id)]==1){
 		    											pre_flag[index_id]=1;
 		    										}
 		    									}
 		    								}
 		    							}
 		    						}
 		    					
 		    					}	
 		    				}
 		    				
 		    				if (reactive == 1){
 		    				
	 		    				for(hg=0; hg < n_reactive; hg++){
	 		    				
	 		    					if((id == id_reactive[hg]) & (m_absolute == m_reactive[hg])){
	 		    					
	 		    					
	 		    						//fprintf(stderr, "REACTIVE day %d\t m %d\t m_abs %d\n", day_simulation, m, m_absolute);
	 		    					
	 		    						reactive_screening(data, tc, param, theta_ps, theta_cs, theta_scs, theta_rps, id, day_simulation, m_absolute, nr, status0, screenstatus, start_q2, quarantine, detected, classtotest, indtotest, when, counter_prodro, counter_infect, counter_rp, fp16, fp18);
	 		    						
	 		    						if (data->classes[index_id]!=6){
	 		    							reactive_class[index_class(tc, data->classes[index_id])]=0;
	 		    						}else{
	 		    							reactive_class[index_t(tc, id)]=0;
	 		    						}
	 		    					}
	 		    					
	 		    		  		}
	 		    		  		
	 		    		  		if (param->delay!=0){
	 		    		  		
		 		    		  		for (ncl=0; ncl< tc->nc; ncl++){
		 		    		  			if (((classtotest[ncl]==1) & (m_absolute==when[ncl])) |
		 		    		  			 ((classtotest[ncl]==2) & (m_absolute==when[ncl]+ 2 * num_15min_day)) ){
		 		    		  				post_reactive(data, tc, param, tc->clas[ncl], day_simulation, m_absolute, nr, status0, screenstatus, start_q2, start_rq2, teach_rtested, teach_rep, quarantine, detected, indtotest, theta_ps, theta_cs, theta_scs, theta_rps, counter_prodro, counter_infect, counter_rp, fp17, fp19);
		 		    		  				if (classtotest[ncl]==1){
		 		    		  					classtotest[ncl]=2;
		 		    		  				}
		 		    		  				
		 		    		  				if(m_absolute==when[ncl]+2*num_15min_day){
		 		    		  					classtotest[ncl]=0;
		 		    		  					
		 		    		  					for (z=0; z<data->n; z++){
		 		    		  						if((data->classes[z]==tc->clas[ncl]) 
		 		    		  						& (indtotest[z]==1)){
		 		    		  							indtotest[z]=0;
		 		    		  						}
		 		    		  						
		 		    		  						if((data->classes[z]==6) 
		 		    		  						& (data->ID[z]!=tc->ID[ncl])
		 		    		  						& (indtotest[z]==1)){
		 		    		  							indtotest[z]=0;
		 		    		  						}
		 		    		  						
		 		    		  					}
		 		    		  				}
		 		    		  			}
		 		    		  		}
	 		    		  		}
 		    		  		}
 		    		  		
 		    		  		// End of quarantine for IC(d) in Q2
 		    		  	  	for(hh=0; hh<n_transitions2; hh++){
 		    			
 		    		  			if( (id==q2_id[hh]) & (m_absolute == q2_quarter[hh]+(param->delta_q/param->deltat) ) ){
 		   				
 		   						if (role_id!=6){ 
 		   					
 		   							quarantine[index_id]=0;
 		   							
 		   						
 		   						}else if(role_id==6){
 		    					
 		    							// We test the original teacher at the end of Q2, and we assume that 
 		    							// status of the original at the end is equal to R+ (8)
 		    							// So we test using a sensitivity teta_r+
 		    						
 		    							// Quarantine_t and teach_rep are turnd off only if the result of
 		    							// the test is negative
 		    						
 		    							r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		    							
 		    							//fprintf(stderr, "q2 id:%d\t start Q2: %d\t end Q2: %d\t (day:%d)\t role: %d\t result:%d\n", id, q2_quarter[hh], m_absolute, day_simulation, role_id, r);
 		    						
 		    							// If during quarantine the replace teacher goes in isoaltion with 
 		    							// the class when the teacher come back (i.e the result of the test is negative),
 		    							// preflag is 0
 		    							//if((pre_flag[index_id]==1) & (r==0)){
 		    							//	pre_flag[index_id]=0;
 		    							//}
 		    						
 		    							// Counter on the number of tests on the original teacher
 		    							// before back to school
 		    							
 		    							n_test_t++;
 		    						
 		    							if (r==1){
 		    								n_test_t_p++;
 		    							}else{
 		    								n_test_t_n++;
 		    							}
 		    						
 		    						}
 		    					
 		    					}
 		    		  	  	}			
 		    			}
 		    		
 		    			// End of preventive isolation	
 		    			if (preventive==1){
 		    			
 		    				if ((pre_flag[index_id]==1) & (m_absolute==pre_time[1][index_id]+(param->delta_q/param->deltat))){
 		       			
 		       					if (role_id!=6){
 		       				
 		       						pre_flag[index_id]=0;
 		       						tea=find(tc, id, data->classes[index_id]);
 		        			
 		       						if(class_iso[index_t(tc, tea)]==1)
 		       							class_iso[index_t(tc, tea)]=0;
 		       							
 		       						
 		       					
 		       					}else if ((role_id==6) & (quarantine_t[index_t(tc,id)]==0)){
 		       					
 		        					r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		        					//fprintf(stderr, "teach id in Q3:%d\t start Q3: %d\t (day:%d)\t  end Q3: %d\t (day:%d)\t role: %d\t result:%d\n", id, pre_time[1][index_id], pre_time[0][index_id], m_absolute, day_simulation, role_id, r);
 		        					
 		        					// Counter on the number of tests on the original teacher 
 		        					// before back to school
 		        					n_test_t++;
 		        							
 		        					if (r==1){
 		    							n_test_t_p++;
 		    						}else{
 		    							n_test_t_n++;
 		    						}
 		        		
 		        			
 		       					}
 		       					pre_flag[index_id]=0;
 		       					/*for (ttts=0; ttts<tc->nc; ttts++){
	       							fprintf(stderr, "flag class in iso: %d\t %d\n", tc->clas[ttts], class_iso[ttts]);
	       							
	       						}*/
 		       				}
 		       			}
 		       			if ((regular==1) | (reactive==1) ){
 		       				//start q2 hold only for original teach
 		       				
	  					if ((screenstatus[j]==6) & ((m_absolute==start_q2[j]) | (m_absolute==start_rq2[j]))){
	  					
	  						
	  						new_q2_reg++;
	  						if ((role_id!=6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
	  						
	  							
	  							
	  							new_q2_regs++;
	  							quarantine[j]=1;
	  							
	  							if ((preventive == 1) & 
	  							(num_pos_class[index_class(tc, data->classes[index_id])]>=3) &
	  							(class_iso[index_class(tc, data->classes[index_id])]==0)){
	  							
	  								
		 		    		  			 preisolation(data, tc, param, id, role_id, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
		 		    		  		}
	  							
	  						}else if ( (role_id==6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
	  						
	  							
	  							
	  							new_q2_regt++;
	  							quarantine_t[index_t(tc,id)]=1;
	  							status[j]=0;
	  						}else if ((m_absolute==start_rq2[j]) & (start_rq2[j]!=0) & (role_id==6) & (teach_rtested[index_t(tc, id)]>1)){
	  						
	  							new_q2_regr++;
	  							status[j]=0;
	  							
	  							
	  							
	  							
	  								
	  						}
	  						screenstatus[j]=-9;
	  						
	  					}else if ((m_absolute==start_q2[index_id]+param->delta_q/param->deltat) & (start_q2[j]!=0)){
 		       				 	
 		       				 	if (role_id!=6){
 		       				 		quarantine[index_id]=0;
 		       					}else if (role_id==6){
 		       					
 		       						r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		       						
 		        					
 		        					// Counter on the number of tests on the original teacher 
 		        					// before back to school
 		        					n_test_t++;
 		        							
 		        					if (r==1){
 		    							n_test_t_p++;
 		    						}else{
 		    							n_test_t_n++;
 		    							teach_neg_reg[n_reg]=id;
 		    							teach_neg_reg=(int*)realloc(teach_neg_reg,(n_reg+2)*sizeof(int));
 		    							n_reg++;
 		    						}
 		    						
 		    					}
 		       				 }
 		       			}
 		       			
 		    			 
 		    			// End of second isolation
 		    			if ((symptomatic==1) | ((symptomatic==1) & (preventive==1)) | (regular==1)){
 		    				if (role_id==6){
 		    					if ((teach_2iso[index_t(tc, id)]==1) 
 		    					& (m_absolute==teach_2isom[index_t(tc, id)]+param->delta_q/param->deltat)){
 		    					
 		    						status[index_id]=9;
 		    						teach_2iso[index_t(tc, id)]=-9;
 		    						quarantine_t[index_t(tc, id)]=0;
 		    						teach_rep[index_t(tc,id)]=-9;
 		    						
 		    						
 		    					}
 		    				}			
 		        		}

 		        		// INDIPENDENT TRANSITIONs:
 						// TRANSITION FROM E->IP 				
 						// TRANSITION FROM IP->IC_detected		
 						// TRANSITION FROM IP->IC
 						// TRANSITION FROM IP->ISC		
 						// TRANSITION FROM Q1->Q2
 						// TRANSITION FROM IC->R+ o ISC->R+ 	
 						// TRANSITION FORM ICDI-> R+		
 						// TRANSITION FORM R+ -> R
 			
 		    			if( (s_id==1) & (counter_latent[index_id] < param->shape_latent)){
 		    				random = gsl_ran_flat(RNG,0,1);
 		    				if(random < (1 - exp(-(param->shape_latent * param->eps) * param->deltat))){
 		    				
 		    					status[index_id]=1;  // 2 for prodromic phas
 		    					counter_latent[index_id]=counter_latent[index_id]+1;
 		    					
 		    					if (counter_latent[index_id] == param->shape_latent){
 		    						status[index_id]=2;
 		    						
 		    						new_pro++;
 		    						if (role_id!=6){
	 		    						new_pro_s++;
	 		    					}else if (role_id==6){
	 		    						if (teach_rep[index_t(tc,id)]==0){
	 		    							new_pro_t++;
	 		    						}else{
	 		    							new_pro_r++;
	 		    						}
	 		    					}
 		    						counter_latent[index_id]=0;
 		    						
 		    					}
 		    				}
 		    			
 		    			} else if ((s_id==2) & (counter_prodro[j] < param->shape_prodro)){
 		    				random=gsl_ran_flat(RNG,0,1);
 		    				counter_latent[index_id]=0;
	 		    			if (random< (1- exp(-(param->shape_prodro *param->mu_p)*param->deltat))){
	 		    				
 		    					status[j] = 2;
 		    					counter_prodro[index_id] = counter_prodro[index_id] +1;
	 		    				
	 		    				if (counter_prodro[index_id] == param->shape_prodro){
	 		    					
	 		    					
	 		    					
	 		    					//counter_prodro[j]=0;
	 		    					random1=gsl_ran_flat(RNG,0,1);
	 		    					// 6 to identified teachers
	 		    					if ( ((role_id==6) & (random1<psub_tc->novax[nw]) & (vaccinated_2doses[index_id]==0) 
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==0))|
	 		    					 ((role_id==6) & (random1<psub_tc->twodoses[nw]) & (vaccinated_2doses[index_id]==0) 
	 		    					 & (vaccinated_booster[index_id]==0)& (r_delta[index_id]==1))|
	 		    					((role_id==6) & (random1<psub_tc->twodoses[nweek_2doses[index_id]]) & (vaccinated_2doses[index_id]==1)) |
	 		    					((role_id==6) & (random1<psub_tc->booster[nweek_booster[index_id]]) & (vaccinated_booster[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->novax[nw]) & (vaccinated_2doses[index_id]==0)
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==0)) |
	 		    					 ((role_id!=6) & (random1<psub_st->twodoses[nw]) & (vaccinated_2doses[index_id]==0)
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->twodoses[nweek_2doses[index_id]]) & (vaccinated_2doses[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->booster[nweek_booster[index_id]]) & (vaccinated_booster[index_id]==1))
	 		    					  ){
	 		    						//fprintf(stderr, "Inside subclinical\n");
	 		    						status[index_id]=5; 
	 		    						new_is++;
	 		    						if (role_id!=6){
	 		    							new_is_s++;
	 		    						}else if (role_id==6){
	 		    							if (teach_rep[index_t(tc,id)]==0){
	 		    								new_is_t++;
	 		    							}else{
	 		    								new_is_r++;
	 		    							}
	 		    						}
	 		    					//5 for subclinical case
	 		    					} else {
	 		    					
	 		    						
	 								random2=gsl_ran_flat(RNG,0,1);
	 								// Clinical detected
	 									
	 		    						if (((role_id==6) & (random2<pd[1])) | ((role_id!=6) & (random2<pd[0])) ){
	 		    					
	 		    							
	 		    							new_ic_d++;
	 		    							
	 		    							random3=gsl_ran_flat(RNG,0,1);
	 		    							if (role_id!=6){
	 		    								n_test_symp_st++;
	 		    							}else{
	 		    								n_test_symp_tc++;
	 		    							}
	 		    							n_test_symp++;
	 		    							//fprintf(stderr, "%d\t %f\n", counter_infect[index_id], theta_cs[counter_infect[index_id]]);
	 		    							if (((random3<theta_cs[0][counter_infect[index_id]]) & (data->classes[index_id]!=6)) | ((random3<theta_cs[1][counter_infect[index_id]]) & (data->classes[index_id]==6))){
	 		    								
	 		    								if (role_id!=6){
	 		    									fprintf(stderr, "%d\t %f\t %f\n", counter_infect[index_id], theta_cs[0][counter_infect[index_id]], theta_cs[0][0]);
	 		    								}else{
	 		    									fprintf(stderr, "%d\t %f\t %f\n", counter_infect[index_id], theta_cs[1][counter_infect[index_id]], theta_cs[0][0]);
	 		    								}
	 		    								
	 		    								n_test_symp_p++;
	 		    								result=1;
	 		    								if (role_id!=6){
	 		    									role=1;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]==0)){
	 		    									role=2;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]>=1)){
	 		    									role=3;
	 		    								}
	 		    								
	 		    								
	 		    								//IF stud/teach/repl_teach
	 		    								if(role_id!=6){ 
	 		    								
	 		    									q1_id[n_transitions1]=id;
	 		    									q1_quarter[n_transitions1]=m_absolute;
	 		    									q1_result[n_transitions1]=1;
	 		    									
	 		    									//fprintf(stderr, "n_transitions1:%d\n", n_transitions1);
	 		   									
	 		   									
	 		    									q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    									q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   									q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   						
	 		   									n_transitions1++;
	 		   									
	 		   								
	 		   									status[index_id]=4;
	 		   									detected[index_id]=1;
	 		   									
	 		   									new_ic_d_s++;
	 		   									
	 		   									// If identified during a preventive	
	 		   									if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    										pre_flag[index_id]=0;
	 		    									}
	 		    									
	 		    								}else if (role_id==6){
	 		    								
	 		    									if (teach_rep[index_t(tc, id)]==0){
	 		    									
	 		    										q1_id[n_transitions1]=id;
	 		    										q1_quarter[n_transitions1]=m_absolute;
	 		    										q1_result[n_transitions1]=1;
	 		    									
	 		    										q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    										q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   										q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   										
	 		   										
	 		   										n_transitions1++;
	 		   										
	 		    										detected_t[index_t(tc,id)]=1;
	 		   										teach_rep[index_t(tc,id)]=1;
	 		   										status[index_id]=0;
	 		   										
	 		   										new_ic_d_t++;
	 		    										
	 		    										status_orig=4;
	 		    										reason=1;
	 		    										
	 		    										
	 		    										// A teacher can be replaced because symp.
	 		    										
	 		    										// If identified during a preventive	
	 		   										if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    											pre_flag[index_id]=0;
	 		    										}
	 		    									}else if (teach_rep[index_t(tc, id)]>=1){
	 		    									
	 		    										
	 		    										status[index_id]=0;
	 		    										new_ic_d_r++;
	 		    									
	 		    										status_orig=4;
	 		    										reason=4;
	 		    								
	 		    										teach_rep[index_t(tc,id)]=2;
	 		    										
	 		   										if (preventive==1){
	 		   										
	 		   											q1_id[n_transitions1]=id;
	 		    											q1_quarter[n_transitions1]=m_absolute;
	 		   											q1_result[n_transitions1]=1;
	 		   										
	 		   											q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    											q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   											q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));	
	 		   											
	 		   											n_transitions1++;
	 		   										}
	 		    									}
	 		    										
	 		   								}
	 		   								
	 		    								
	 		    							
	 		    							// (that eventually triggers a protocol)
	 		    							
	 		    								
	 		    							} else {
	 		    							
	 		    								n_test_symp_n++;
	 		    								/*result=0;
	 		    								if (role_id!=6){
	 		    									role=1;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]==0)){
	 		    									role=2;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]>=1)){
	 		    									role=3;
	 		    								}
	 		    								*/
	 		    								
	 		    								if(role_id!=6){
	 		    								 												q1_id[n_transitions1]=id;
	 		    									q1_quarter[n_transitions1]=m_absolute;
	 		    									q1_result[n_transitions1]=0;
	 		    									
	 		    									q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    									q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   									q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   									
	 		   									n_transitions1++;
	 		   									
	 		   									status[index_id]=4;
	 		   									detected[index_id]=1;
	 		   									new_ic_d_s++;
	 		   										
	 		   									if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    										pre_flag[index_id]=0;
	 		    									}
	 		    								}else if (role_id==6){
	 		   									
	 		   									if (teach_rep[index_t(tc, id)]==0){
	 		   									
	 		   										q1_id[n_transitions1]=id;
	 		    										q1_quarter[n_transitions1]=m_absolute;
	 		    										q1_result[n_transitions1]=0;
	 		    									
	 		    										q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    										q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   										q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   										
	 		   										n_transitions1++;
	 		   									
	 		   										detected_t[index_t(tc,id)]=1;
	 		   										status[index_id]=0;
	 		   										
	 		   										new_ic_d_t++;
	 		    										
	 		    										status_orig=4;
	 		    										reason=1;
	 		    										
	 		    										
	 		    										
	 		    								
	 		    										teach_rep[index_t(tc,id)]=1;
	 		    										
	 		    										if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    											pre_flag[index_id]=0;
	 		    										}
	 		   									}else if (teach_rep[index_t(tc, id)]>=1){
	 		    									
	 		    										status[index_id]=0;
	 		    										new_ic_d_r++;
	 		    									
	 		    										status_orig=4;
	 		    										reason=4;
	 		    								
	 		    										teach_rep[index_t(tc,id)]=2;
	 		    										
	 		   										if (preventive==1){
	 		   										
	 		   											q1_id[n_transitions1]=id;
	 		    											q1_quarter[n_transitions1]=m_absolute;
	 		   											q1_result[n_transitions1]=0;
	 		   										
	 		   											q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    											q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   											q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));			
	 		   											
	 		   											
	 		   											n_transitions1++;
	 		   										}
	 		   									}
	 		   								}
	 		    							
	 		    							}
	 		    						// Clinical not detected 
	 		    						}else{
	 		    						
	 		    							status[index_id]=4;
	 		    							new_ic++;
	 		    							
	 		    							if (role_id!=6){
	 		    								new_ic_s++;
	 		    								detected[index_id]=0;
	 		    							}else if (role_id==6){
	 		    								if (teach_rep[index_t(tc,id)]==0){
	 		    									new_ic_t++;
	 		    									detected_t[index_t(tc,id)]=0;
	 		    								}else{
	 		    									new_ic_r++;
	 		    								}
	 		    							}
	 		    						}   
	 								}			
	 					 		}
	 						}
 					 	//fprintf(stderr, "end prodromic");
 		    			} else if(((s_id==4) & (counter_infect[j]<param->shape_infect)) | 
 		    			((s_id==5) & (counter_infect[j]<param->shape_infect))){	
 		    		        	random=gsl_ran_flat(RNG,0,1);	
 		    		        	counter_prodro[index_id]=0;
	    					if(random< 1 - exp(-(param->shape_infect * param->mu)*param->deltat)){
	    						if (s_id==4){
	    							status[index_id]=4;
	    						}else if (s_id==5){
	    							status[index_id]=5;
	    						}
	    						counter_infect[j]=counter_infect[j]+1;
	    						if (counter_infect[j] == param->shape_infect){
	    							status[index_id]=8;          // 8 for recovery but still positive to the te
	    							//counter_infect[j] = 0;
	    						}
	    					}	

 		    			} else if ((s_id==8) & (counter_rp[index_id]<param->shape_rp)){
 		    				random4=gsl_ran_flat(RNG,0.0,1.0);
 		    				counter_infect[j] = 0;
 		    				if (role_id!=6){	
 		    					if(random4 < (1 - exp(-(param->shape_rp * param->mu_rp)*param->deltat))){
 		    						counter_rp[index_id]=counter_rp[index_id]+1;
 		    						if (counter_rp[index_id]==param->shape_rp){
 		    							status[index_id]=9;
 		    							counter_rp[index_id] = 0;
 		    						}
 		    					}
 		    				}else if(role_id==6){
 		    				
 		    					if (symptomatic==1){
 		    						for(hh=0; hh<n_transitions2; hh++){
 		    							if((id==q2_id[hh]) & (random4<1 - exp(-(param->shape_rp * param->mu_rp1)*param->deltat))){
 		    								counter_rp[index_id]=counter_rp[index_id]+1;
		 		    						if (counter_rp[index_id]==param->shape_rp){
		 		    							status[index_id]=9;
		 		    							counter_rp[index_id] = 0;
		 		    						}
		 		    								
 		    							}else if ((id!=q2_id[hh]) & (random4<1 - exp(-(param->shape_rp * param->mu_rp)*param->deltat))){
 		    								counter_rp[index_id]=counter_rp[index_id]+1;
		 		    						if (counter_rp[index_id]==param->shape_rp){
		 		    							status[index_id]=9;
		 		    							counter_rp[index_id] = 0;
		 		    						}
 		    							}
 		    						}
 		    					}
 		    					
 		    				}
 		    			}    	
 		    		}	
 		
 		
 				
 		
 					save_status(data,status,status0); 
	  	
	  			}
	  	
	  	
	  			
	  			fprintf(fp5, "%d\t %d\t %d\t %d\t %d\t %d\n", nr, day_simulation, new_exposed, new_exp_s, new_exp_t, new_exp_r);
	  			fflush(fp5);
	  			
	  			d3=d3+week;
	  	                           
      			break;
	  	                           
      		case 4:	
    		
    			new_exposed=0;
    			new_pro=0;
    			new_ic_d=0;
    			new_ic=0;
    			new_is=0;
    			new_q3=0;
    		
    			// New intro in each compartments divided by type: STUDENTS/ TEACHERS/ REPLACED TEACHERS
    			new_exp_s=0;new_exp_t=0;new_exp_r=0;
				new_pro_s=0;new_pro_t=0;new_pro_r=0;new_ic_s=0;
    			new_ic_t=0;new_ic_r=0;new_ic_d_s=0;new_ic_d_t=0;new_ic_d_r=0;
				new_is_s=0;new_is_t=0;new_is_r=0;
				new_q2_s=0;new_q2_t=0;new_pos_r=0;new_q2_reg=0;new_q2_regs=0;new_q2_regt=0;new_q2_regr=0;new_neg_s=0;new_neg_t=0;new_neg_r=0;
				n_test_symp=0;n_test_symp_p=0; n_test_symp_n=0;n_test_symp_st=0; n_test_symp_tc=0;
      			n_test_t=0;n_test_t_p=0;n_test_t_n=0;
    		
	  			
    			for (m=0; m<num_15min_day; m++){
    			
    				m_absolute=num_15min_day*day_simulation + m; //progression of 15 minutes step from the first day to the last one
	        
	        		
			
					n_susc_s=0;
					n_susc_t=0;
      				// Intro of 1 case if there are still susceptible individuals
      				
      			 	for (i=0; i<data->n; i++){
      					if (((status0[i]==0) | (status0[i]==12)) & (data->ID[i]!=6)){
      						n_susc_s++;
      					}else if (((status0[i]==0) | (status0[i]==12)) & (data->ID[i]==6)){
	  						n_susc_t++;
	  					}
      				}

					if ((n_susc_s!=0) & ((*introst)[nr].weeks[nw]>0) & (n_susc_s>=(*introst)[nr].weeks[nw])){
      			
						for (ni=0; ni<(*introst)[nr].weeks[nw]; ni++){
							if (((day_simulation % (week) + 1)==introductions_days_st[ni]) & (m==introductions_quarter_st[ni])){
 
								typ=weekly_introduction_st(data, tc, status, status0, (*introst)[nr].weeks[nw], double_presence, teach_rep, exp_day, day_simulation);
									
								new_exposed++;
								
								if (typ==1){
									new_exp_s++;
								}	
							}
						}
					}
					if ((n_susc_t!=0) & ((*introtc)[nr].weeks[nw]>0) & (n_susc_t>=(*introtc)[nr].weeks[nw])){
					
						for (ni=0; ni<(*introtc)[nr].weeks[nw]; ni++){
							if (((day_simulation % (week) + 1)==introductions_days_tc[ni]) & (m==introductions_quarter_tc[ni])){
								  
								typ=weekly_introduction_tc(data, tc, status, status0, (*introtc)[nr].weeks[nw], double_presence, teach_rep, exp_day, day_simulation);
									
								new_exposed++;
								
								if (typ==2){
									new_exp_t++;
								}else if (typ==3){
									new_exp_r++;
								}		
							}
						}
					}
      			
					if ((m>=start[day_simulation]) & (m<=end[day_simulation])){
				
						// If we are in the case: 4 test per week (ntimes=4, nweek=1) - > Perform everyday
						// If we are in the case: 1 test per week (ntimes=1, nweek=1) - > Perform test only Tuesday
	  		    		// If we are in the case: 2 test per week (ntimes=2, nweek=1) - > Perform test only Thursday
	  		    		// Tuersday
	  		    
	  		    		if (((regular==1) & (ntimes==2) & (m==start[day_simulation]) & (nweek==1)) | 
	  		    		((regular==1) & (ntimes==4) &  (m==start[day_simulation]) & (nweek==1))){
	  		    		
	  		    				regular_screening(data, param, tc, status0, teach_rep, teach_rtested, pre_flag, quarantine, detected, screenstatus, comp_id, start_q2, start_rq2, m_absolute, day_simulation, nr, theta_ps, theta_cs, theta_scs, theta_rps, counter_prodro, counter_infect, counter_rp, num_pos_class);//, fp12, fp13);
	  		    				
	  		    				count_positive_st(data, nr, day_simulation, counter_prodro, counter_infect, counter_rp, status, detected, quarantine, pre_flag, num_iso, fp20);
	  		    				
	  		    		}
	  		    			
						for (i=0; i<ct->n1; i++){
 	  	
 		  	  	  			if ((ct->step[i]==m) & (ct->days[i]==d4)){
 		  	  	  

 		        				id1=ct->ID1[i];
 		        				id2=ct->ID2[i];
 		        		
 		        		
 		        				index_id1=index_nodes(data, id1); 
 		        				index_id2=index_nodes(data, id2); 
 		        
  		        				s_id1=status0[index_id1];
 		        				s_id2=status0[index_id2];
 		        		
 		        				role_id1=data->classes[index_id1];
 		        				role_id2=data->classes[index_id2];
 		        		
 		        				weight=ct->weight[i];
 		        		
 		        				// rbeta e sigma definition
 		        				// Reduction on susceptibility and infectiousness is different for adults and children
 		        				// class = 6 is used to identified teachers
 		        		
 		        				int is_teacher1 = (ct->class1[i] == 6);
								int is_teacher2 = (ct->class2[i] == 6);
							
								int v2dose1 = vaccinated_2doses[index_id1];
								int vboost1 = vaccinated_booster[index_id1];
								int n2dose1 = nweek_2doses[index_id1];
								int nboost1 = nweek_booster[index_id1];
							
								int v2dose2 = vaccinated_2doses[index_id2];
								int vboost2 = vaccinated_booster[index_id2];
								int n2dose2 = nweek_2doses[index_id2];
								int nboost2 = nweek_booster[index_id2];
								
								int delta2 = r_delta[index_id2];
								int delta1 = r_delta[index_id1];
 		        		
 		        				// rbeta e sigma definition
 		        				// Reduction on susceptibility and infectiousness is different for adults and children
 		        				// class = 6 is used to identified teachers
								if ((s_id1 == 0 || s_id1 == 12) && (s_id2 == 2 || s_id2 == 5 || s_id2 == 4)) {
									
									sigma = compute_sigma(s_id1, is_teacher1, vboost1, v2dose1, nboost1, n2dose1,
											sigma_st, sigma_tc, param->sigma_omicron, nw);
											
									rbeta = compute_rbeta(s_id2, is_teacher2, vboost2, v2dose2, delta2,
										nboost2, n2dose2, rbeta_st, rbeta_tc, nw);
										
								} else if ((s_id2 == 0 || s_id2 == 12) && (s_id1 == 2 || s_id1 == 5 || s_id1 == 4)) {
									sigma = compute_sigma(s_id2, is_teacher2, vboost2, v2dose2, nboost2, n2dose2,
										sigma_st, sigma_tc, param->sigma_omicron, nw);
										
									rbeta = compute_rbeta(s_id1, is_teacher1, vboost1, v2dose1, delta1,
									nboost1, n2dose1, rbeta_st, rbeta_tc, nw);
								}
 		        				
 		        				//TRANSITION FROM S->E
 		        				//if (((symptomatic==1) & (preventive==1)) | (symptomatic==1) | (regular==1)){
 		        		
 		        				// pre_flag is used to check if contacts are on on off 
 		        				// depending on presence of isolation or not 
 		        				// If contacts are on (pre_flag=0, individuals in q3) 
 		        				// this means that the infectious node not be in pre isolation
 		        				
 		        				
 		        			   	if((((pre_flag[index_id1]==0) & (detected[index_id1]==0) & (quarantine[index_id1]==0))
 		        				 & ((pre_flag[index_id2]==0) & (detected[index_id2]==0) & (quarantine[index_id2]==0)))
 		        				 ){
 		        						
 		        					if((s_id1==2) & ((s_id2==0) | (s_id2==12))){
 		        			
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id2]=1;  
 		           							exp_day[index_id2]=day_simulation;
 		           							new_exposed++;  
 		           							
 		           							if ((role_id2==6) && (teach_rep[index_t(tc, id2)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id2==6) && (teach_rep[index_t(tc, id2)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id2!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							}
 		           						
 		           							
 		           						}
 		           						
 		        					} else if (((s_id1==0) | (s_id1==12))& (s_id2==2)){
 		        					
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        								
 		           							status[index_id1]=1;  
 		           							exp_day[index_id1]=day_simulation;
 		           							new_exposed++;
 		           								
 		           							
 		           								
 		           							if ((role_id1==6) && (teach_rep[index_t(tc, id1)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id1==6) && (teach_rep[index_t(tc, id1)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id1!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							}  
 		           							
 		           						}
 		           						
 		           				 	// 4 clinical
 		        					} else if ((s_id1==4) & ((s_id2==0) | (s_id2==12)) ){
 		        						
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id2]=1;  
 		           							exp_day[index_id2]=day_simulation;
 		           							new_exposed++; 
 		           							
 		           							if ((role_id2==6) && (teach_rep[index_t(tc, id2)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id2==6) && (teach_rep[index_t(tc, id2)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id2!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		          							} 
 		           							
 		           						}
 		           					
 		           							
 		        					}else if (((s_id1==0) | (s_id1==12)) & (s_id2==4)){
 		        						
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id1]=1; 
 		           							exp_day[index_id1]=day_simulation; 
 		           							new_exposed++; 
 		           							
 		           							if ((role_id1==6) && (teach_rep[index_t(tc, id1)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id1==6) && (teach_rep[index_t(tc, id1)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id1!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							} 
 		           							
 		           						}
 		           							
 		           					// 5 subclinical
 		        					} else if ((s_id1==5) & ((s_id2==0) | (s_id2==12))){
 		        				
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id2]=1; 
 		           							exp_day[index_id2]=day_simulation; 
 		           							new_exposed++;  
 		           							
 		           							if ((role_id2==6) && (teach_rep[index_t(tc, id2)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id2==6) && (teach_rep[index_t(tc, id2)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id2!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							} 
 		           							
 		           							 
 		           						}
 		           							
 		        					}else if (((s_id1==0) | (s_id1==12)) & (s_id2==5)){
 		        					
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id1]=1;  
 		           							exp_day[index_id1]=day_simulation;
 		           							new_exposed++; 
 		           							
 		           							if ((role_id1==6) && (teach_rep[index_t(tc, id1)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id1==6) && (teach_rep[index_t(tc, id1)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id1!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							}  
 		           						}
 		           					}	
 		   	   					}
 	         	    		}
	  					}			 
					}
				
					for (j=0; j<data->n; j++){
			
 		    			id=data->ID[j];
 		    			role_id=data->classes[j];
 		   				index_id=index_nodes(data, id);
 		    			s_id=status0[index_id];
 		    		
 		    			
 		    			// End of first isolation Q1
 		    			// From Q1 to Q2
 		    			// n_transitions1 = number of suspected nodes tested 
 		    			if ((symptomatic==1) & (n_transitions1>0) & (n_transitions2>=0)){
 		    		
 		    			 // n_transitions1 = number of symp collected
 		    			 // if there is only sympt ON (symptomatic==1)
 		    			 // in n_transitions1= stud + original teacher detected
 		    			 // If if there is sympt ON (symptomatic==1) + prevent. ON (preventive==1)
 		    			 // in n_transitions1= stud + original teacher detected + teacher replaced
 		    			 // I need the information on replced to know if they activate a PREVENTIVE or NOT
 		    			 
 		    		  		for (g=0; g<n_transitions1; g++){
 		    			
 		    					// q1_id=id tested because suspected
 		    					// At the time of the turnaround time, we known the result			
								if((id==q1_id[g]) & (m_absolute==q1_quarter[g]+(param->tau_test/param->deltat))){
 		    				
 		    						//If id is a student
 		    						if (role_id!=6){
 		    							// IF positive go in Q2 and its contacts are OFF
 		    							if (q1_result[g]==1){
 		    						
 		    								new_q2_s++;
 		    							
 		    								detected[index_id]=0;
 		    								quarantine[index_id]=1;
 		    								num_iso[index_id]=1;
 		    								q2_id[n_transitions2]=id;
 		   		        					q2_quarter[n_transitions2]=m_absolute;
 		    						
 		    								q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    								q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    								
 		    								
 		    								n_transitions2++;
 		    							
 		    								// Count the number of additional active classes
 		    								
 		    								if ((reactive == 1) & (reactive_class[index_class(tc, data->classes[index_id])]==0) & (screenstatus[index_id]!=6)){
 		    								
 		    									id_reactive[n_reactive] = id;
 		    									
 		    									if (param->deltar==1440){
 		    										m_reactive[n_reactive] = m_absolute + (num_15min_day - m) + start[0];
 		    									}
 		    									
 		    									if (param->deltar==0){
 		    										
 		    										m_reactive[n_reactive] = m_absolute;
 		    									}
 		    									
 		    									id_reactive=(int*)realloc(id_reactive,(n_reactive+2)*sizeof(int)); 		    										
 		    									m_reactive=(int*)realloc(m_reactive,(n_reactive+2)*sizeof(int));
	 		    									
 		    									
 		    									reactive_class[index_class(tc, data->classes[index_id])]=1;
 		    									n_reactive++;
	 		    									
	 		    								
 		    								}	
 		    								
 		    								if ((preventive==1) & (regular==0)){
 		    								
 		    									class_q3=data->classes[index_id];
 		    									teach_q3=find(tc, id, class_q3);
 		    								
 		    									// If the class in not already in Q3
 		    				 					if (class_iso[index_t(tc, teach_q3)]==0){
 		    							
 		    										// Turn on if the class is in pre iso
 		    										class_iso[index_t(tc, teach_q3)]=1;
 		    										
 		    										dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    										
 		    										trigger_role=1;
 		    									
 		    										
 		    							
 		    										new_q3=new_q3+dim;
 		    							
 		    									//fprintf(stderr, "teach in Q3: %d\t interval: %d\t %d\t day: %d\t s%d\n", teach_q1, m,m_absolute, day_simulation, status0[index_nodes(data, teach_q1)]);
 		    									}else{
 		    										pre_flag[index_id]=0;
 		    									}
 		    								}

 		    							}else{
 		    								detected[index_id]=0;
 		    								new_neg_s++;
 		    								// If a students is tested negative during preisolation
 		    								// he/she contin in Q3
 		    								if (preventive==1){
 		    									class_q3=data->classes[index_id];
 		    									teach_q3=find(tc, id, class_q3);
 		    									if (class_iso[index_t(tc, teach_q3)]==1){
 		    										pre_flag[index_id]=1;
 		    									}
 		    								}
 		    							}
 		    							// IF id is a teacher:
 		    							// a. original
 		    							// b. replaced
 		    						}else if (role_id==6){
 		    							// IN q1 the original teacher is already replaced
 		    							// At this point, teach_rep==1
 		    							if (teach_rep[index_t(tc, id)]==1){
 		    								// IF positive 
 		    								// Save the moment in which go in Q2
 		    								if (q1_result[g]==1){
 		    							
 		    									new_q2_t++;
 		    								
 		    									q2_id[n_transitions2]=id;
 		   		        						q2_quarter[n_transitions2]=m_absolute;
 		    									//q2_day[n_transitions1]=day_simulation;
 		    				
 		    									q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    									q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    									//q2_day=(int*)realloc(q2_day,(n_transitions2+2)*sizeof(int));
 		    									
 		    									fprintf(stderr, " q2 id: %d\t start Q2 interval: %d\t start Q2 day: %d\n", q2_id[n_transitions2], q2_quarter[n_transitions2], day_simulation);
 		    									
 		    									n_transitions2++;
 		    							
 		    									// Count the number of additional active classes
 		    									
 		    									detected_t[index_t(tc,id)]=0;
 		    									quarantine_t[index_t(tc, id)]=1;
 		    								
 		    									
	 		    								
 		    									if ((preventive==1) & (regular==0)){
 		    					
 		    				 						if (class_iso[index_t(tc, id)]==0){
 		    							
 		    											// Turn on if the class is in pre iso
 		    											class_iso[index_t(tc, id)]=1;
 		    											
 		    											dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    											
 		    											trigger_role=2;
 		    						
 		    											
 		    											// If the teacher is the trigger, the 
 		    											// replace teacher is however in
 		    											// isolation with the class
 		    							
 		    											new_q3=new_q3+dim;
 		    										}
 		    										// If a teacher go in in Q2 during a preventive
 		    										// the pre_flag is ON for the replaced teacher
 		    									}
 		    								// Otherwise back to school as clinical
 		    								}else {
 		    							
 		    									status[index_id]=4;
 		    									detected_t[index_t(tc,id)]=0;
 		    									new_neg_t++;
 		    									
 		    									if (preventive==1){
 		    										
 		    										if (class_iso[index_t(tc, id)]==1){
 		    											pre_flag[index_id]=1;
 		    										}
 		    									}
 		    								}
 		    								// teach_rep >1 Sostituto del sostituo
 		    								// When the original back, teach_rep==-9
 		    							}else if (teach_rep[index_t(tc, id)]>1){
 		    								
 		    							
 		    								if (q1_result[g]==1){ 
 		    									new_pos_r++;
 		    								
 		    				 					if ((preventive==1) & (regular==0) & (class_iso[index_t(tc, id)]==0)){
 		    							
 		    										// Turn on if the class is in pre iso
 		    										class_iso[index_t(tc, id)]=1;
 		    						
 		    										dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    										
 		    										trigger_role=3;
 		    										
 		    							
 		    										new_q3=new_q3+dim;
 		    									}
 		    								}else{
 		    									new_neg_r++;
 		    									
 		    									if (preventive==1){
 		    										
 		    										if (class_iso[index_t(tc, id)]==1){
 		    											pre_flag[index_id]=1;
 		    										}
 		    									}
 		    								}
 		    							}
 		    						}
 		    					
 		    					}	
 		    				}
 		    				
 		    				if (reactive == 1){
 		    				
	 		    				for(hg=0; hg < n_reactive; hg++){
	 		    				
	 		    					if((id == id_reactive[hg]) & (m_absolute == m_reactive[hg])){
	 		    					
	 		    					
	 		    						//fprintf(stderr, "REACTIVE day %d\t m %d\t m_abs %d\n", day_simulation, m, m_absolute);
	 		    					
	 		    						reactive_screening(data, tc, param, theta_ps, theta_cs, theta_scs, theta_rps, id, day_simulation, m_absolute, nr, status0, screenstatus, start_q2, quarantine, detected, classtotest, indtotest, when, counter_prodro, counter_infect, counter_rp, fp16, fp18);
	 		    						
	 		    						if (data->classes[index_id]!=6){
	 		    							reactive_class[index_class(tc, data->classes[index_id])]=0;
	 		    						}else{
	 		    							reactive_class[index_t(tc, id)]=0;
	 		    						}
	 		    					}
	 		    					
	 		    		  		}
	 		    		  		
	 		    		  		if (param->delay!=0){
	 		    		  		
		 		    		  		for (ncl=0; ncl< tc->nc; ncl++){
		 		    		  			if (((classtotest[ncl]==1) & (m_absolute==when[ncl])) |
		 		    		  			 ((classtotest[ncl]==2) & (m_absolute==when[ncl]+ 2 * num_15min_day)) ){
		 		    		  				post_reactive(data, tc, param, tc->clas[ncl], day_simulation, m_absolute, nr, status0, screenstatus, start_q2, start_rq2, teach_rtested, teach_rep, quarantine, detected, indtotest, theta_ps, theta_cs, theta_scs, theta_rps, counter_prodro, counter_infect, counter_rp, fp17, fp19);
		 		    		  				if (classtotest[ncl]==1){
		 		    		  					classtotest[ncl]=2;
		 		    		  				}
		 		    		  				
		 		    		  				if(m_absolute==when[ncl]+2*num_15min_day){
		 		    		  					classtotest[ncl]=0;
		 		    		  					
		 		    		  					for (z=0; z<data->n; z++){
		 		    		  						if((data->classes[z]==tc->clas[ncl]) 
		 		    		  						& (indtotest[z]==1)){
		 		    		  							indtotest[z]=0;
		 		    		  						}
		 		    		  						
		 		    		  						if((data->classes[z]==6) 
		 		    		  						& (data->ID[z]!=tc->ID[ncl])
		 		    		  						& (indtotest[z]==1)){
		 		    		  							indtotest[z]=0;
		 		    		  						}
		 		    		  						
		 		    		  					}
		 		    		  				}
		 		    		  			}
		 		    		  		}
	 		    		  		}
 		    		  		}
 		    		  		
 		    		  		// End of quarantine for IC(d) in Q2
 		    		  	  	for(hh=0; hh<n_transitions2; hh++){
 		    			
 		    		  			if( (id==q2_id[hh]) & (m_absolute == q2_quarter[hh]+(param->delta_q/param->deltat) ) ){
 		   				
 		   						if (role_id!=6){ 
 		   					
 		   							quarantine[index_id]=0;
 		   							
 		   						
 		   						}else if(role_id==6){
 		    					
 		    							// We test the original teacher at the end of Q2, and we assume that 
 		    							// status of the original at the end is equal to R+ (8)
 		    							// So we test using a sensitivity teta_r+
 		    						
 		    							// Quarantine_t and teach_rep are turnd off only if the result of
 		    							// the test is negative
 		    						
 		    							r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		    							//fprintf(stderr, "q2 id:%d\t start Q2: %d\t end Q2: %d\t (day:%d)\t role: %d\t result:%d\n", id, q2_quarter[hh], m_absolute, day_simulation, role_id, r);
 		    						
 		    							// If during quarantine the replace teacher goes in isoaltion with 
 		    							// the class when the teacher come back (i.e the result of the test is negative),
 		    							// preflag is 0
 		    							//if((pre_flag[index_id]==1) & (r==0)){
 		    							//	pre_flag[index_id]=0;
 		    							//}
 		    						
 		    							// Counter on the number of tests on the original teacher
 		    							// before back to school
 		    							
 		    							n_test_t++;
 		    						
 		    							if (r==1){
 		    								n_test_t_p++;
 		    							}else{
 		    								n_test_t_n++;
 		    							}
 		    						
 		    						}
 		    					
 		    					}
 		    		  	  	}			
 		    			}
 		    		
 		    			// End of preventive isolation	
 		    			if (preventive==1){
 		    			
 		    				if ((pre_flag[index_id]==1) & (m_absolute==pre_time[1][index_id]+(param->delta_q/param->deltat))){
 		       			
 		       					if (role_id!=6){
 		       				
 		       						pre_flag[index_id]=0;
 		       						tea=find(tc, id, data->classes[index_id]);
 		        			
 		       						if(class_iso[index_t(tc, tea)]==1)
 		       							class_iso[index_t(tc, tea)]=0;
 		       							
 		       						
 		       					
 		       					}else if ((role_id==6) & (quarantine_t[index_t(tc,id)]==0)){
 		       					
 		        					r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		        						
 		        					//fprintf(stderr, "teach id in Q3:%d\t start Q3: %d\t (day:%d)\t  end Q3: %d\t (day:%d)\t role: %d\t result:%d\n", id, pre_time[1][index_id], pre_time[0][index_id], m_absolute, day_simulation, role_id, r);
 		        					
 		        					// Counter on the number of tests on the original teacher 
 		        					// before back to school
 		        					n_test_t++;
 		        							
 		        					if (r==1){
 		    							n_test_t_p++;
 		    						}else{
 		    							n_test_t_n++;
 		    						}
 		        		
 		        			
 		       					}
 		       					pre_flag[index_id]=0;
 		       					
 		       					/*for (ttts=0; ttts<tc->nc; ttts++){
	       							fprintf(stderr, "flag class in iso: %d\t %d\n", tc->clas[ttts], class_iso[ttts]);
	       							
	       						}*/
 		       				}
 		       			}
 		       			if ((regular==1) | (reactive==1) ){
 		       				//start q2 hold only for original teach
 		       				
	  					if ((screenstatus[j]==6) & ((m_absolute==start_q2[j]) | (m_absolute==start_rq2[j]))){
	  					
	  						
	  						new_q2_reg++;
	  						if ((role_id!=6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
	  						
	  							
	  							
	  							new_q2_regs++;
	  							quarantine[j]=1;
	  							
	  							if ((preventive == 1) & 
	  							(num_pos_class[index_class(tc, data->classes[index_id])]>=3) &
	  							(class_iso[index_class(tc, data->classes[index_id])]==0)){
	  							
	  								
		 		    		  			 preisolation(data, tc, param, id, role_id, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
		 		    		  		}
	  							
	  						}else if ( (role_id==6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
	  						
	  							
	  							
	  							new_q2_regt++;
	  							quarantine_t[index_t(tc,id)]=1;
	  							status[j]=0;
	  						}else if ((m_absolute==start_rq2[j]) & (start_rq2[j]!=0) & (role_id==6) & (teach_rtested[index_t(tc, id)]>1)){
	  						
	  							new_q2_regr++;
	  							status[j]=0;
	  							
	  							
	  							
	  							
	  								
	  						}
	  						screenstatus[j]=-9;
	  						
	  					}else if ((m_absolute==start_q2[index_id]+param->delta_q/param->deltat) & (start_q2[j]!=0)){
 		       				 	
 		       				 	if (role_id!=6){
 		       				 		quarantine[index_id]=0;
 		       					}else if (role_id==6){
 		       					
 		       						r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		       						
 		       				 		
 		        					// Counter on the number of tests on the original teacher 
 		        					// before back to school
 		        					n_test_t++;
 		        							
 		        					if (r==1){
 		    							n_test_t_p++;
 		    						}else{
 		    							n_test_t_n++;
 		    							teach_neg_reg[n_reg]=id;
 		    							teach_neg_reg=(int*)realloc(teach_neg_reg,(n_reg+2)*sizeof(int));
 		    							n_reg++;
 		    						}
 		    						
 		    					}
 		       				 }
 		       			}
 		       			
 		    			 
 		    			// End of second isolation
 		    			if ((symptomatic==1) | ((symptomatic==1) & (preventive==1)) | (regular==1)){
 		    				if (role_id==6){
 		    					if ((teach_2iso[index_t(tc, id)]==1) 
 		    					& (m_absolute==teach_2isom[index_t(tc, id)]+param->delta_q/param->deltat)){
 		    					
 		    						status[index_id]=9;
 		    						teach_2iso[index_t(tc, id)]=-9;
 		    						quarantine_t[index_t(tc, id)]=0;
 		    						teach_rep[index_t(tc,id)]=-9;
 		    						
 		    						
 		    					}
 		    				}			
 		        		}

 		        		// INDIPENDENT TRANSITIONs:
 						// TRANSITION FROM E->IP 				
 						// TRANSITION FROM IP->IC_detected		
 						// TRANSITION FROM IP->IC
 						// TRANSITION FROM IP->ISC		
 						// TRANSITION FROM Q1->Q2
 						// TRANSITION FROM IC->R+ o ISC->R+ 	
 						// TRANSITION FORM ICDI-> R+		
 						// TRANSITION FORM R+ -> R
 			
 		    			if( (s_id==1) & (counter_latent[index_id] < param->shape_latent)){
 		    				random = gsl_ran_flat(RNG,0,1);
 		    				if(random < (1 - exp(-(param->shape_latent * param->eps) * param->deltat))){
 		    				
 		    					status[index_id]=1;  // 2 for prodromic phas
 		    					counter_latent[index_id]=counter_latent[index_id]+1;
 		    					
 		    					if (counter_latent[index_id] == param->shape_latent){
 		    						status[index_id]=2;
 		    						
 		    						new_pro++;
 		    						if (role_id!=6){
	 		    						new_pro_s++;
	 		    					}else if (role_id==6){
	 		    						if (teach_rep[index_t(tc,id)]==0){
	 		    							new_pro_t++;
	 		    						}else{
	 		    							new_pro_r++;
	 		    						}
	 		    					}
 		    						counter_latent[index_id]=0;
 		    						
 		    					}
 		    				}
 		    			
 		    			} else if ((s_id==2) & (counter_prodro[j] < param->shape_prodro)){
 		    				random=gsl_ran_flat(RNG,0,1);
 		    				counter_latent[index_id]=0;
	 		    			if (random< (1- exp(-(param->shape_prodro *param->mu_p)*param->deltat))){
	 		    				
 		    					status[j] = 2;
 		    					counter_prodro[index_id] = counter_prodro[index_id] +1;
	 		    				
	 		    				if (counter_prodro[index_id] == param->shape_prodro){
	 		    					
	 		    					
	 		    					
	 		    					
	 		    					//counter_prodro[j]=0;
	 		    					random1=gsl_ran_flat(RNG,0,1);
	 		    					// 6 to identified teachers
	 		    					if ( ((role_id==6) & (random1<psub_tc->novax[nw]) & (vaccinated_2doses[index_id]==0) 
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==0))|
	 		    					 ((role_id==6) & (random1<psub_tc->twodoses[nw]) & (vaccinated_2doses[index_id]==0) 
	 		    					 & (vaccinated_booster[index_id]==0)& (r_delta[index_id]==1))|
	 		    					((role_id==6) & (random1<psub_tc->twodoses[nweek_2doses[index_id]]) & (vaccinated_2doses[index_id]==1)) |
	 		    					((role_id==6) & (random1<psub_tc->booster[nweek_booster[index_id]]) & (vaccinated_booster[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->novax[nw]) & (vaccinated_2doses[index_id]==0)
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==0)) |
	 		    					 ((role_id!=6) & (random1<psub_st->twodoses[nw]) & (vaccinated_2doses[index_id]==0)
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->twodoses[nweek_2doses[index_id]]) & (vaccinated_2doses[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->booster[nweek_booster[index_id]]) & (vaccinated_booster[index_id]==1))
	 		    					  ){
	 		    						//fprintf(stderr, "Inside subclinical\n");
	 		    						status[index_id]=5; 
	 		    						new_is++;
	 		    						if (role_id!=6){
	 		    							new_is_s++;
	 		    						}else if (role_id==6){
	 		    							if (teach_rep[index_t(tc,id)]==0){
	 		    								new_is_t++;
	 		    							}else{
	 		    								new_is_r++;
	 		    							}
	 		    						}
	 		    					//5 for subclinical case
	 		    					} else {
	 		    					
	 		    						
	 								random2=gsl_ran_flat(RNG,0,1);
	 								// Clinical detected
	 									
	 		    						if (((role_id==6) & (random2<pd[1])) | ((role_id!=6) & (random2<pd[0])) ){
	 		    					
	 		    							
	 		    							new_ic_d++;
	 		    							
	 		    							random3=gsl_ran_flat(RNG,0,1);
	 		    							if (role_id!=6){
	 		    								n_test_symp_st++;
	 		    							}else{
	 		    								n_test_symp_tc++;
	 		    							}
	 		    							n_test_symp++;
	 		    							//fprintf(stderr, "%d\t %f\n", counter_infect[index_id], theta_cs[counter_infect[index_id]]);
	 		    							if (((random3<theta_cs[0][counter_infect[index_id]]) & (data->classes[index_id]!=6)) | ((random3<theta_cs[1][counter_infect[index_id]]) & (data->classes[index_id]==6))){
	 		    							
	 		    								if (role_id!=6){
	 		    									fprintf(stderr, "%d\t %f\t %f\n", counter_infect[index_id], theta_cs[0][counter_infect[index_id]], theta_cs[0][0]);
	 		    								}else{
	 		    									fprintf(stderr, "%d\t %f\t %f\n", counter_infect[index_id], theta_cs[1][counter_infect[index_id]], theta_cs[0][0]);
	 		    								}
	 		    					
	 		    								n_test_symp_p++;
	 		    								result=1;
	 		    								if (role_id!=6){
	 		    									role=1;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]==0)){
	 		    									role=2;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]>=1)){
	 		    									role=3;
	 		    								}
	 		    								
	 		    								
	 		    								//IF stud/teach/repl_teach
	 		    								if(role_id!=6){ 
	 		    								
	 		    									q1_id[n_transitions1]=id;
	 		    									q1_quarter[n_transitions1]=m_absolute;
	 		    									q1_result[n_transitions1]=1;
	 		    									
	 		    									//fprintf(stderr, "n_transitions1:%d\n", n_transitions1);
	 		   									
	 		   									
	 		    									q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    									q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   									q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   						
	 		   									n_transitions1++;
	 		   									
	 		   								
	 		   									status[index_id]=4;
	 		   									detected[index_id]=1;
	 		   									
	 		   									new_ic_d_s++;
	 		   									
	 		   									// If identified during a preventive	
	 		   									if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    										pre_flag[index_id]=0;
	 		    									}
	 		    									
	 		    								}else if (role_id==6){
	 		    								
	 		    									if (teach_rep[index_t(tc, id)]==0){
	 		    									
	 		    										q1_id[n_transitions1]=id;
	 		    										q1_quarter[n_transitions1]=m_absolute;
	 		    										q1_result[n_transitions1]=1;
	 		    									
	 		    										q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    										q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   										q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   										
	 		   										
	 		   										n_transitions1++;
	 		   										
	 		    										detected_t[index_t(tc,id)]=1;
	 		   										teach_rep[index_t(tc,id)]=1;
	 		   										status[index_id]=0;
	 		   										
	 		   										new_ic_d_t++;
	 		    										
	 		    										status_orig=4;
	 		    										reason=1;
	 		    										
	 		    										
	 		    										// A teacher can be replaced because symp.
	 		    										
	 		    										// If identified during a preventive	
	 		   										if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    											pre_flag[index_id]=0;
	 		    										}
	 		    									}else if (teach_rep[index_t(tc, id)]>=1){
	 		    									
	 		    										
	 		    										status[index_id]=0;
	 		    										new_ic_d_r++;
	 		    									
	 		    										status_orig=4;
	 		    										reason=4;
	 		    								
	 		    										teach_rep[index_t(tc,id)]=2;
	 		    										
	 		   										if (preventive==1){
	 		   										
	 		   											q1_id[n_transitions1]=id;
	 		    											q1_quarter[n_transitions1]=m_absolute;
	 		   											q1_result[n_transitions1]=1;
	 		   										
	 		   											q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    											q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   											q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));	
	 		   											
	 		   											n_transitions1++;
	 		   										}
	 		    									}
	 		    										
	 		   								}
	 		   								
	 		    								
	 		    							
	 		    							// (that eventually triggers a protocol)
	 		    							
	 		    								
	 		    							} else {
	 		    							
	 		    								n_test_symp_n++;
	 		    								/*result=0;
	 		    								if (role_id!=6){
	 		    									role=1;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]==0)){
	 		    									role=2;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]>=1)){
	 		    									role=3;
	 		    								}
	 		    								*/
	 		    								
	 		    								if(role_id!=6){
	 		    								 												q1_id[n_transitions1]=id;
	 		    									q1_quarter[n_transitions1]=m_absolute;
	 		    									q1_result[n_transitions1]=0;
	 		    									
	 		    									q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    									q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   									q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   									
	 		   									n_transitions1++;
	 		   									
	 		   									status[index_id]=4;
	 		   									detected[index_id]=1;
	 		   									new_ic_d_s++;
	 		   										
	 		   									if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    										pre_flag[index_id]=0;
	 		    									}
	 		    								}else if (role_id==6){
	 		   									
	 		   									if (teach_rep[index_t(tc, id)]==0){
	 		   									
	 		   										q1_id[n_transitions1]=id;
	 		    										q1_quarter[n_transitions1]=m_absolute;
	 		    										q1_result[n_transitions1]=0;
	 		    									
	 		    										q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    										q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   										q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   										
	 		   										n_transitions1++;
	 		   									
	 		   										detected_t[index_t(tc,id)]=1;
	 		   										status[index_id]=0;
	 		   										
	 		   										new_ic_d_t++;
	 		    										
	 		    										status_orig=4;
	 		    										reason=1;
	 		    										
	 		    										
	 		    										
	 		    								
	 		    										teach_rep[index_t(tc,id)]=1;
	 		    										
	 		    										if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    											pre_flag[index_id]=0;
	 		    										}
	 		   									}else if (teach_rep[index_t(tc, id)]>=1){
	 		    									
	 		    										status[index_id]=0;
	 		    										new_ic_d_r++;
	 		    									
	 		    										status_orig=4;
	 		    										reason=4;
	 		    								
	 		    										teach_rep[index_t(tc,id)]=2;
	 		    										
	 		   										if (preventive==1){
	 		   										
	 		   											q1_id[n_transitions1]=id;
	 		    											q1_quarter[n_transitions1]=m_absolute;
	 		   											q1_result[n_transitions1]=0;
	 		   										
	 		   											q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    											q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   											q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));			
	 		   											
	 		   											
	 		   											n_transitions1++;
	 		   										}
	 		   									}
	 		   								}
	 		    							
	 		    							}
	 		    						// Clinical not detected 
	 		    						}else{
	 		    						
	 		    							
	 		    							
	 		    							status[index_id]=4;
	 		    							new_ic++;
	 		    							
	 		    							if (role_id!=6){
	 		    								new_ic_s++;
	 		    								detected[index_id]=0;
	 		    							}else if (role_id==6){
	 		    								if (teach_rep[index_t(tc,id)]==0){
	 		    									new_ic_t++;
	 		    									detected_t[index_t(tc,id)]=0;
	 		    								}else{
	 		    									new_ic_r++;
	 		    								}
	 		    							}
	 		    						}   
	 							}			
	 					 	}
	 					 }
 					 	//fprintf(stderr, "end prodromic");
 		    			} else if(((s_id==4) & (counter_infect[j]<param->shape_infect)) | 
 		    			((s_id==5) & (counter_infect[j]<param->shape_infect))){	
 		    		        	random=gsl_ran_flat(RNG,0,1);	
 		    		        	counter_prodro[index_id]=0;
	    					if(random< 1 - exp(-(param->shape_infect * param->mu)*param->deltat)){
	    						if (s_id==4){
	    							status[index_id]=4;
	    						}else if (s_id==5){
	    							status[index_id]=5;
	    						}
	    						counter_infect[j]=counter_infect[j]+1;
	    						if (counter_infect[j] == param->shape_infect){
	    							status[index_id]=8;          // 8 for recovery but still positive to the te
	    							//counter_infect[j] = 0;
	    						}
	    					}	

 		    			} else if ((s_id==8) & (counter_rp[index_id]<param->shape_rp)){
 		    				random4=gsl_ran_flat(RNG,0.0,1.0);
 		    				counter_infect[j] = 0;
 		    				if (role_id!=6){	
 		    					if(random4 < (1 - exp(-(param->shape_rp * param->mu_rp)*param->deltat))){
 		    						counter_rp[index_id]=counter_rp[index_id]+1;
 		    						if (counter_rp[index_id]==param->shape_rp){
 		    							status[index_id]=9;
 		    							counter_rp[index_id] = 0;
 		    						}
 		    					}
 		    				}else if(role_id==6){
 		    				
 		    					if (symptomatic==1){
 		    						for(hh=0; hh<n_transitions2; hh++){
 		    							if((id==q2_id[hh]) & (random4<1 - exp(-(param->shape_rp * param->mu_rp1)*param->deltat))){
 		    								counter_rp[index_id]=counter_rp[index_id]+1;
		 		    						if (counter_rp[index_id]==param->shape_rp){
		 		    							status[index_id]=9;
		 		    							counter_rp[index_id] = 0;
		 		    						}
		 		    								
 		    							}else if ((id!=q2_id[hh]) & (random4<1 - exp(-(param->shape_rp * param->mu_rp)*param->deltat))){
 		    								counter_rp[index_id]=counter_rp[index_id]+1;
		 		    						if (counter_rp[index_id]==param->shape_rp){
		 		    							status[index_id]=9;
		 		    							counter_rp[index_id] = 0;
		 		    						}
 		    							}
 		    						}
 		    					}
 		    					
 		    					
 		    				}
 		    			}    	
 		    		}	
 		
 				
 		
 					save_status(data,status,status0); 
	  	
	  			}
	  	
	  	
	  			
	  			
	  			fprintf(fp5, "%d\t %d\t %d\t %d\t %d\t %d\n", nr, day_simulation, new_exposed, new_exp_s, new_exp_t, new_exp_r);
	  			fflush(fp5);
	  			
	  		
	  			
	  			d4=d4+week;
	  	                           
      			break;
      		case 5:	
    		
    			new_exposed=0;
    			new_pro=0;
    			new_ic_d=0;
    			new_ic=0;
    			new_is=0;
    			new_q3=0;
    		
    			// New intro in each compartments divided by type: STUDENTS/ TEACHERS/ REPLACED TEACHERS
    			new_exp_s=0;new_exp_t=0;new_exp_r=0;
				new_pro_s=0;new_pro_t=0;new_pro_r=0;new_ic_s=0;
    			new_ic_t=0;new_ic_r=0;new_ic_d_s=0;new_ic_d_t=0;new_ic_d_r=0;
				new_is_s=0;new_is_t=0;new_is_r=0;
				new_q2_s=0;new_q2_t=0;new_pos_r=0;new_q2_reg=0;new_q2_regs=0;new_q2_regt=0;new_q2_regr=0;new_neg_s=0;new_neg_t=0;new_neg_r=0;
				n_test_symp=0;n_test_symp_p=0; n_test_symp_n=0;n_test_symp_st=0; n_test_symp_tc=0;
      			n_test_t=0;n_test_t_p=0;n_test_t_n=0;
    		
	  			
    			for (m=0; m<num_15min_day; m++){
    			
    				m_absolute=num_15min_day*day_simulation + m; //progression of 15 minutes step from the first day to the last one

					n_susc_s=0;
					n_susc_t=0;
      				// Intro of 1 case if there are still susceptible individuals
      				
      			 	for (i=0; i<data->n; i++){
      					if (((status0[i]==0) | (status0[i]==12)) & (data->ID[i]!=6)){
      						n_susc_s++;
      					}else if (((status0[i]==0) | (status0[i]==12)) & (data->ID[i]==6)){
	  						n_susc_t++;
	  					}
      				}

					if ((n_susc_s!=0) & ((*introst)[nr].weeks[nw]>0) & (n_susc_s>=(*introst)[nr].weeks[nw])){
      			
						for (ni=0; ni<(*introst)[nr].weeks[nw]; ni++){
							if (((day_simulation % (week) + 1)==introductions_days_st[ni]) & (m==introductions_quarter_st[ni])){
 
								typ=weekly_introduction_st(data, tc, status, status0, (*introst)[nr].weeks[nw], double_presence, teach_rep, exp_day, day_simulation);
									
								new_exposed++;
								
								if (typ==1){
									new_exp_s++;
								}	
							}
						}
					}

					if ((n_susc_t!=0) & ((*introtc)[nr].weeks[nw]>0) & (n_susc_t>=(*introtc)[nr].weeks[nw])){
					
						for (ni=0; ni<(*introtc)[nr].weeks[nw]; ni++){
							if (((day_simulation % (week) + 1)==introductions_days_tc[ni]) & (m==introductions_quarter_tc[ni])){
								  
								typ=weekly_introduction_tc(data, tc, status, status0, (*introtc)[nr].weeks[nw], double_presence, teach_rep, exp_day, day_simulation);
									
								new_exposed++;
								
								if (typ==2){
									new_exp_t++;
								}else if (typ==3){
									new_exp_r++;
								}		
							}
						}
					}

					if ((m>=start[day_simulation]) & (m<=end[day_simulation])){
				
						// If we are in the case: 4 test per week (ntimes=4, nweek=1) - > Perform everyday
						// If we are in the case: 1 test per week (ntimes=1, nweek=1) - > Perform test only Tuesday
	  		    		// If we are in the case: 2 test per week (ntimes=2, nweek=1) - > Perform test only Thursday
	  		    		// Tuersday
	  		    
	  		    		if ((regular==1) & (ntimes==4) &  (m==start[day_simulation]) & (nweek==1)){
	  		    		
	  		    			regular_screening(data, param, tc, status0, teach_rep, teach_rtested, pre_flag, quarantine, detected, screenstatus, comp_id, start_q2, start_rq2, m_absolute, day_simulation, nr, theta_ps, theta_cs, theta_scs, theta_rps, counter_prodro, counter_infect, counter_rp, num_pos_class);//, fp12, fp13);
	  		    			
	  		    			count_positive_st(data, nr, day_simulation, counter_prodro, counter_infect, counter_rp, status, detected, quarantine, pre_flag, num_iso, fp20);
	  		    			
	  		    		}
	  		    			
						for (i=0; i<ct->n1; i++){
 	  	
 		  	  	  			if ((ct->step[i]==m) & (ct->days[i]==d5)){

 		        				id1=ct->ID1[i];
 		        				id2=ct->ID2[i];
 		        		
 		        		
 		        				index_id1=index_nodes(data, id1); 
 		        				index_id2=index_nodes(data, id2); 
 		        
  		        				s_id1=status0[index_id1];
 		        				s_id2=status0[index_id2];
 		        		
 		        				role_id1=data->classes[index_id1];
 		        				role_id2=data->classes[index_id2];
 		        		
 		        				weight=ct->weight[i];
 		        		
 		        				// rbeta e sigma definition
 		        				// Reduction on susceptibility and infectiousness is different for adults and children
 		        				// class = 6 is used to identified teachers
 		        		
 		        				int is_teacher1 = (ct->class1[i] == 6);
								int is_teacher2 = (ct->class2[i] == 6);
							
								int v2dose1 = vaccinated_2doses[index_id1];
								int vboost1 = vaccinated_booster[index_id1];
								int n2dose1 = nweek_2doses[index_id1];
								int nboost1 = nweek_booster[index_id1];
							
								int v2dose2 = vaccinated_2doses[index_id2];
								int vboost2 = vaccinated_booster[index_id2];
								int n2dose2 = nweek_2doses[index_id2];
								int nboost2 = nweek_booster[index_id2];
								
								int delta2 = r_delta[index_id2];
								int delta1 = r_delta[index_id1];
 		        		
 		        				// rbeta e sigma definition
 		        				// Reduction on susceptibility and infectiousness is different for adults and children
 		        				// class = 6 is used to identified teachers
								if ((s_id1 == 0 || s_id1 == 12) && (s_id2 == 2 || s_id2 == 5 || s_id2 == 4)) {
									
									sigma = compute_sigma(s_id1, is_teacher1, vboost1, v2dose1, nboost1, n2dose1,
											sigma_st, sigma_tc, param->sigma_omicron, nw);
											
									rbeta = compute_rbeta(s_id2, is_teacher2, vboost2, v2dose2, delta2,
										nboost2, n2dose2, rbeta_st, rbeta_tc, nw);
										
								} else if ((s_id2 == 0 || s_id2 == 12) && (s_id1 == 2 || s_id1 == 5 || s_id1 == 4)) {
									sigma = compute_sigma(s_id2, is_teacher2, vboost2, v2dose2, nboost2, n2dose2,
										sigma_st, sigma_tc, param->sigma_omicron, nw);
										
									rbeta = compute_rbeta(s_id1, is_teacher1, vboost1, v2dose1, delta1,
									nboost1, n2dose1, rbeta_st, rbeta_tc, nw);
								}
 		        				
 		        				//TRANSITION FROM S->E
 		        				//if (((symptomatic==1) & (preventive==1)) | (symptomatic==1) | (regular==1)){
 		        		
 		        				// pre_flag is used to check if contacts are on on off 
 		        				// depending on presence of isolation or not 
 		        				// If contacts are on (pre_flag=0, individuals in q3) 
 		        				// this means that the infectious node not be in pre isolation
 		        				
 		        				
 		        			   	if((((pre_flag[index_id1]==0) & (detected[index_id1]==0) & (quarantine[index_id1]==0))
 		        				 & ((pre_flag[index_id2]==0) & (detected[index_id2]==0) & (quarantine[index_id2]==0)))
 		        				 ){
 		        						
 		        					if((s_id1==2) & ((s_id2==0) | (s_id2==12))){
 		        			
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id2]=1;  
 		           							exp_day[index_id2]=day_simulation;
 		           							new_exposed++;  
 		           							
 		           							if ((role_id2==6) && (teach_rep[index_t(tc, id2)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id2==6) && (teach_rep[index_t(tc, id2)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id2!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							}
 		           						
 		           							
 		           						}
 		           						
 		        					} else if (((s_id1==0) | (s_id1==12))& (s_id2==2)){
 		        					
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        								
 		           							status[index_id1]=1;  
 		           							exp_day[index_id1]=day_simulation;
 		           							new_exposed++;
 		           							
 		           							
 		           								
 		           							if ((role_id1==6) && (teach_rep[index_t(tc, id1)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id1==6) && (teach_rep[index_t(tc, id1)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id1!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							}  
 		           							
 		           						}
 		           						
 		           				 	// 4 clinical
 		        					} else if ((s_id1==4) & ((s_id2==0) | (s_id2==12)) ){
 		        						
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id2]=1;  
 		           							exp_day[index_id2]=day_simulation;
 		           							new_exposed++; 
 		           							
 		           							if ((role_id2==6) && (teach_rep[index_t(tc, id2)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id2==6) && (teach_rep[index_t(tc, id2)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id2!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		          							} 
 		           							
 		           						}
 		           					
 		           							
 		        					}else if (((s_id1==0) | (s_id1==12)) & (s_id2==4)){
 		        						
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id1]=1; 
 		           							exp_day[index_id1]=day_simulation; 
 		           							new_exposed++; 
 		           							
 		           							if ((role_id1==6) && (teach_rep[index_t(tc, id1)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id1==6) && (teach_rep[index_t(tc, id1)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id1!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							} 
 		           							
 		           						}
 		           							
 		           					// 5 subclinical
 		        					} else if ((s_id1==5) & ((s_id2==0) | (s_id2==12))){
 		        				
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id2]=1; 
 		           							exp_day[index_id2]=day_simulation; 
 		           							new_exposed++;  
 		           							
 		           							if ((role_id2==6) && (teach_rep[index_t(tc, id2)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id2==6) && (teach_rep[index_t(tc, id2)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id2!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							} 
 		           							
 		           							 
 		           						}
 		           							
 		        					}else if (((s_id1==0) | (s_id1==12)) & (s_id2==5)){
 		        					
 		        						random=gsl_ran_flat(RNG,0,1);
 		        					
 		        						if (random<param->beta*weight*param->deltat*rbeta*sigma){  
 		        							
 		           							status[index_id1]=1;  
 		           							exp_day[index_id1]=day_simulation;
 		           							new_exposed++; 
 		           							
 		           							if ((role_id1==6) && (teach_rep[index_t(tc, id1)]==0)){
 		           								new_exp_t++;
											weekly_transmission_tc++;
 		           							}else if ((role_id1==6) && (teach_rep[index_t(tc, id1)]>=1)){
 		           								new_exp_r++;
											weekly_transmission_tc++;
 		           							}else if (role_id1!=6){
 		           								new_exp_s++;
											weekly_transmission_st++;
 		           							}  
 		           														}
 		           					}	
 		   	   				}
 	         	    			}
	  				}			 
				}
 	        		        
				for (j=0; j<data->n; j++){
			
 		    			id=data->ID[j];
 		    			role_id=data->classes[j];
 		   			index_id=index_nodes(data, id);
 		    			s_id=status0[index_id];
 		    		
 		    			
 		    			// End of first isolation Q1
 		    			// From Q1 to Q2
 		    			// n_transitions1 = number of suspected nodes tested 
 		    			if ((symptomatic==1) & (n_transitions1>0) & (n_transitions2>=0)){
 		    		
 		    			 // n_transitions1 = number of symp collected
 		    			 // if there is only sympt ON (symptomatic==1)
 		    			 // in n_transitions1= stud + original teacher detected
 		    			 // If if there is sympt ON (symptomatic==1) + prevent. ON (preventive==1)
 		    			 // in n_transitions1= stud + original teacher detected + teacher replaced
 		    			 // I need the information on replced to know if they activate a PREVENTIVE or NOT
 		    			 
 		    		  		for (g=0; g<n_transitions1; g++){
 		    			
 		    					// q1_id=id tested because suspected
 		    					// At the time of the turnaround time, we known the result			
							if((id==q1_id[g]) & (m_absolute==q1_quarter[g]+(param->tau_test/param->deltat))){
 		    				
 		    						//If id is a student
 		    						if (role_id!=6){
 		    							// IF positive go in Q2 and its contacts are OFF
 		    							if (q1_result[g]==1){
 		    						
 		    								new_q2_s++;
 		    							
 		    								detected[index_id]=0;
 		    								quarantine[index_id]=1;
 		    								num_iso[index_id]=1;
 		    								q2_id[n_transitions2]=id;
 		   		        					q2_quarter[n_transitions2]=m_absolute;
 		    						
 		    								q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    								q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    								
 		    								
 		    								n_transitions2++;
 		    							
 		    								// Count the number of additional active classes
 		    								
 		    								if ((reactive == 1) & (reactive_class[index_class(tc, data->classes[index_id])]==0) & (screenstatus[index_id]!=6)){
 		    								
 		    									id_reactive[n_reactive] = id;
 		    									
 		    									if (param->deltar==1440){
 		    										m_reactive[n_reactive] = m_absolute + (num_15min_day - m) + start[0];
 		    									}
 		    									
 		    									if (param->deltar==0){
 		    										
 		    										m_reactive[n_reactive] = m_absolute;
 		    									}
 		    									
 		    									id_reactive=(int*)realloc(id_reactive,(n_reactive+2)*sizeof(int)); 		    										
 		    									m_reactive=(int*)realloc(m_reactive,(n_reactive+2)*sizeof(int));
	 		    									
 		    									
 		    									reactive_class[index_class(tc, data->classes[index_id])]=1;
 		    									n_reactive++;
	 		    									
	 		    								
 		    								}	
 		    								
 		    								if ((preventive==1) & (regular==0)){
 		    								
 		    									class_q3=data->classes[index_id];
 		    									teach_q3=find(tc, id, class_q3);
 		    								
 		    									// If the class in not already in Q3
 		    				 					if (class_iso[index_t(tc, teach_q3)]==0){
 		    							
 		    										// Turn on if the class is in pre iso
 		    										class_iso[index_t(tc, teach_q3)]=1;
 		    										
 		    										
 		    										dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    										
 		    										trigger_role=1;
 		    									
 		    										
 		    							
 		    										new_q3=new_q3+dim;
 		    							
 		    									//fprintf(stderr, "teach in Q3: %d\t interval: %d\t %d\t day: %d\t s%d\n", teach_q1, m,m_absolute, day_simulation, status0[index_nodes(data, teach_q1)]);
 		    									}else{
 		    										pre_flag[index_id]=0;
 		    									}
 		    								}

 		    							}else{
 		    								detected[index_id]=0;
 		    								new_neg_s++;
 		    								// If a students is tested negative during preisolation
 		    								// he/she contin in Q3
 		    								if (preventive==1){
 		    									class_q3=data->classes[index_id];
 		    									teach_q3=find(tc, id, class_q3);
 		    									if (class_iso[index_t(tc, teach_q3)]==1){
 		    										pre_flag[index_id]=1;
 		    									}
 		    								}
 		    							}
 		    							// IF id is a teacher:
 		    							// a. original
 		    							// b. replaced
 		    						}else if (role_id==6){
 		    							// IN q1 the original teacher is already replaced
 		    							// At this point, teach_rep==1
 		    							if (teach_rep[index_t(tc, id)]==1){
 		    								// IF positive 
 		    								// Save the moment in which go in Q2
 		    								if (q1_result[g]==1){
 		    							
 		    									new_q2_t++;
 		    								
 		    									q2_id[n_transitions2]=id;
 		   		        						q2_quarter[n_transitions2]=m_absolute;
 		    									//q2_day[n_transitions1]=day_simulation;
 		    				
 		    									q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    									q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    									//q2_day=(int*)realloc(q2_day,(n_transitions2+2)*sizeof(int));
 		    									
 		    									fprintf(stderr, " q2 id: %d\t start Q2 interval: %d\t start Q2 day: %d\n", q2_id[n_transitions2], q2_quarter[n_transitions2], day_simulation);
 		    									
 		    									n_transitions2++;
 		    							
 		    									// Count the number of additional active classes
 		    									
 		    									detected_t[index_t(tc,id)]=0;
 		    									quarantine_t[index_t(tc, id)]=1;
 		    									
 		    									
	 		    								
 		    									if ((preventive==1) & (regular==0)){
 		    					
 		    				 						if (class_iso[index_t(tc, id)]==0){
 		    							
 		    											// Turn on if the class is in pre iso
 		    											class_iso[index_t(tc, id)]=1;
 		    											
 		    											dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    											
 		    											trigger_role=2;
 		    						
 		    											
 		    											// If the teacher is the trigger, the 
 		    											// replace teacher is however in
 		    											// isolation with the class
 		    							
 		    											new_q3=new_q3+dim;
 		    										}
 		    										// If a teacher go in in Q2 during a preventive
 		    										// the pre_flag is ON for the replaced teacher
 		    									}
 		    								// Otherwise back to school as clinical
 		    								}else {
 		    							
 		    									status[index_id]=4;
 		    									detected_t[index_t(tc,id)]=0;
 		    									new_neg_t++;
 		    									
 		    									if (preventive==1){
 		    										
 		    										if (class_iso[index_t(tc, id)]==1){
 		    											pre_flag[index_id]=1;
 		    										}
 		    									}
 		    								}
 		    								// teach_rep >1 Sostituto del sostituo
 		    								// When the original back, teach_rep==-9
 		    							}else if (teach_rep[index_t(tc, id)]>1){
 		    								
 		    							
 		    								if (q1_result[g]==1){ 
 		    									new_pos_r++;
 		    								
 		    				 					if ((preventive==1) & (regular==0) & (class_iso[index_t(tc, id)]==0)){
 		    							
 		    										// Turn on if the class is in pre iso
 		    										class_iso[index_t(tc, id)]=1;
 		    						
 		    										dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    										
 		    										trigger_role=3;
 		    										
 		    							
 		    										new_q3=new_q3+dim;
 		    									}
 		    								}else{
 		    									new_neg_r++;
 		    									
 		    									if (preventive==1){
 		    										
 		    										if (class_iso[index_t(tc, id)]==1){
 		    											pre_flag[index_id]=1;
 		    										}
 		    									}
 		    								}
 		    							}
 		    						}
 		    					
 		    					}	
 		    				}
 		    				
 		    				
 		    				if (reactive == 1){
 		    				
	 		    				for(hg=0; hg < n_reactive; hg++){
	 		    				
	 		    					if((id == id_reactive[hg]) & (m_absolute == m_reactive[hg])){
	 		    					
	 		    					
	 		    						//fprintf(stderr, "REACTIVE day %d\t m %d\t m_abs %d\n", day_simulation, m, m_absolute);
	 		    					
	 		    						reactive_screening(data, tc, param, theta_ps, theta_cs, theta_scs, theta_rps, id, day_simulation, m_absolute, nr, status0, screenstatus, start_q2, quarantine, detected, classtotest, indtotest, when, counter_prodro, counter_infect, counter_rp, fp16, fp18);
	 		    						
	 		    						if (data->classes[index_id]!=6){
	 		    							reactive_class[index_class(tc, data->classes[index_id])]=0;
	 		    						}else{
	 		    							reactive_class[index_t(tc, id)]=0;
	 		    						}
	 		    					}
	 		    					
	 		    		  		}
	 		    		  		
	 		    		  		if (param->delay!=0){
	 		    		  		
		 		    		  		for (ncl=0; ncl< tc->nc; ncl++){
		 		    		  			if (((classtotest[ncl]==1) & (m_absolute==when[ncl])) |
		 		    		  			 ((classtotest[ncl]==2) & (m_absolute==when[ncl]+ 2 * num_15min_day)) ){
		 		    		  				post_reactive(data, tc, param, tc->clas[ncl], day_simulation, m_absolute, nr, status0, screenstatus, start_q2, start_rq2, teach_rtested, teach_rep, quarantine, detected, indtotest, theta_ps, theta_cs, theta_scs, theta_rps, counter_prodro, counter_infect, counter_rp,  fp17, fp19);
		 		    		  				if (classtotest[ncl]==1){
		 		    		  					classtotest[ncl]=2;
		 		    		  				}
		 		    		  				
		 		    		  				if(m_absolute==when[ncl]+2*num_15min_day){
		 		    		  					classtotest[ncl]=0;
		 		    		  					
		 		    		  					for (z=0; z<data->n; z++){
		 		    		  						if((data->classes[z]==tc->clas[ncl]) 
		 		    		  						& (indtotest[z]==1)){
		 		    		  							indtotest[z]=0;
		 		    		  						}
		 		    		  						
		 		    		  						if((data->classes[z]==6) 
		 		    		  						& (data->ID[z]!=tc->ID[ncl])
		 		    		  						& (indtotest[z]==1)){
		 		    		  							indtotest[z]=0;
		 		    		  						}
		 		    		  						
		 		    		  					}
		 		    		  				}
		 		    		  			}
		 		    		  		}
	 		    		  		}
 		    		  		}
 		    		  		
 		    		  		// End of quarantine for IC(d) in Q2
 		    		  	  	for(hh=0; hh<n_transitions2; hh++){
 		    			
 		    		  			if( (id==q2_id[hh]) & (m_absolute == q2_quarter[hh]+(param->delta_q/param->deltat) ) ){
 		   				
 		   						if (role_id!=6){ 
 		   					
 		   							quarantine[index_id]=0;
 		   							
 		   						
 		   						}else if(role_id==6){
 		    					
 		    							// We test the original teacher at the end of Q2, and we assume that 
 		    							// status of the original at the end is equal to R+ (8)
 		    							// So we test using a sensitivity teta_r+
 		    						
 		    							// Quarantine_t and teach_rep are turnd off only if the result of
 		    							// the test is negative
 		    						
 		    							r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		    							//fprintf(stderr, "q2 id:%d\t start Q2: %d\t end Q2: %d\t (day:%d)\t role: %d\t result:%d\n", id, q2_quarter[hh], m_absolute, day_simulation, role_id, r);
 		    						
 		    							// If during quarantine the replace teacher goes in isoaltion with 
 		    							// the class when the teacher come back (i.e the result of the test is negative),
 		    							// preflag is 0
 		    							//if((pre_flag[index_id]==1) & (r==0)){
 		    							//	pre_flag[index_id]=0;
 		    							//}
 		    						
 		    							// Counter on the number of tests on the original teacher
 		    							// before back to school
 		    							
 		    							n_test_t++;
 		    						
 		    							if (r==1){
 		    								n_test_t_p++;
 		    							}else{
 		    								n_test_t_n++;
 		    							}
 		    						
 		    						}
 		    					
 		    					}
 		    		  	  	}			
 		    			}
 		    		
 		    			// End of preventive isolation	
 		    			if (preventive==1){
 		    			
 		    				if ((pre_flag[index_id]==1) & (m_absolute==pre_time[1][index_id]+(param->delta_q/param->deltat))){
 		       			
 		       					if (role_id!=6){
 		       				
 		       						pre_flag[index_id]=0;
 		       						tea=find(tc, id, data->classes[index_id]);
 		        			
 		       						if(class_iso[index_t(tc, tea)]==1)
 		       							class_iso[index_t(tc, tea)]=0;
 		       							
 		       						
 		       					
 		       					}else if ((role_id==6) & (quarantine_t[index_t(tc,id)]==0)){
 		       					
 		        					r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		        					//fprintf(stderr, "teach id in Q3:%d\t start Q3: %d\t (day:%d)\t  end Q3: %d\t (day:%d)\t role: %d\t result:%d\n", id, pre_time[1][index_id], pre_time[0][index_id], m_absolute, day_simulation, role_id, r);
 		        					
 		        					// Counter on the number of tests on the original teacher 
 		        					// before back to school
 		        					n_test_t++;
 		        							
 		        					if (r==1){
 		    							n_test_t_p++;
 		    						}else{
 		    							n_test_t_n++;
 		    						}
 		        		
 		        			
 		       					}
 		       					pre_flag[index_id]=0;
 		       					/*for (ttts=0; ttts<tc->nc; ttts++){
	       							fprintf(stderr, "flag class in iso: %d\t %d\n", tc->clas[ttts], class_iso[ttts]);
	       							
	       						}*/
 		       				}
 		       			}
 		       			if ((regular==1) | (reactive==1) ){
 		       				//start q2 hold only for original teach
 		       				
	  					if ((screenstatus[j]==6) & ((m_absolute==start_q2[j]) | (m_absolute==start_rq2[j]))){
	  					
	  						
	  						new_q2_reg++;
	  						if ((role_id!=6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
	  						
	  							
	  							
	  							new_q2_regs++;
	  							quarantine[j]=1;
	  							
	  							if ((preventive == 1) & 
	  							(num_pos_class[index_class(tc, data->classes[index_id])]>=3) &
	  							(class_iso[index_class(tc, data->classes[index_id])]==0)){
	  							
	  								
		 		    		  			 preisolation(data, tc, param, id, role_id, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
		 		    		  		}
	  							
	  						}else if ( (role_id==6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
	  						
	  							
	  							
	  							new_q2_regt++;
	  							quarantine_t[index_t(tc,id)]=1;
	  							status[j]=0;
	  						}else if ((m_absolute==start_rq2[j]) & (start_rq2[j]!=0) & (role_id==6) & (teach_rtested[index_t(tc, id)]>1)){
	  						
	  							new_q2_regr++;
	  							status[j]=0;
	  							
	  							
	  							
	  							
	  								
	  						}
	  						screenstatus[j]=-9;
	  						
	  					}else if ((m_absolute==start_q2[index_id]+param->delta_q/param->deltat) & (start_q2[j]!=0)){
 		       				 	
 		       				 	if (role_id!=6){
 		       				 		quarantine[index_id]=0;
 		       					}else if (role_id==6){
 		       					
 		       						r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		        					// Counter on the number of tests on the original teacher 
 		        					// before back to school
 		        					n_test_t++;
 		        							
 		        					if (r==1){
 		    							n_test_t_p++;
 		    						}else{
 		    							n_test_t_n++;
 		    							teach_neg_reg[n_reg]=id;
 		    							teach_neg_reg=(int*)realloc(teach_neg_reg,(n_reg+2)*sizeof(int));
 		    							n_reg++;
 		    						}
 		    						
 		    					}
 		       				 }
 		       			}
 		       			
 		    			 
 		    			// End of second isolation
 		    			if ((symptomatic==1) | ((symptomatic==1) & (preventive==1)) | (regular==1)){
 		    				if (role_id==6){
 		    					if ((teach_2iso[index_t(tc, id)]==1) 
 		    					& (m_absolute==teach_2isom[index_t(tc, id)]+param->delta_q/param->deltat)){
 		    					
 		    						status[index_id]=9;
 		    						teach_2iso[index_t(tc, id)]=-9;
 		    						quarantine_t[index_t(tc, id)]=0;
 		    						teach_rep[index_t(tc,id)]=-9;
 		    						
 		    						
 		    					}
 		    				}			
 		        		}

 		        		// INDIPENDENT TRANSITIONs:
 						// TRANSITION FROM E->IP 				
 						// TRANSITION FROM IP->IC_detected		
 						// TRANSITION FROM IP->IC
 						// TRANSITION FROM IP->ISC		
 						// TRANSITION FROM Q1->Q2
 						// TRANSITION FROM IC->R+ o ISC->R+ 	
 						// TRANSITION FORM ICDI-> R+		
 						// TRANSITION FORM R+ -> R
 			
 		    			if( (s_id==1) & (counter_latent[index_id] < param->shape_latent)){
 		    				random = gsl_ran_flat(RNG,0,1);
 		    				if(random < (1 - exp(-(param->shape_latent * param->eps) * param->deltat))){
 		    				
 		    					status[index_id]=1;  // 2 for prodromic phas
 		    					counter_latent[index_id]=counter_latent[index_id]+1;
 		    					
 		    					if (counter_latent[index_id] == param->shape_latent){
 		    						status[index_id]=2;
 		    						
 		    						new_pro++;
 		    						if (role_id!=6){
	 		    						new_pro_s++;
	 		    					}else if (role_id==6){
	 		    						if (teach_rep[index_t(tc,id)]==0){
	 		    							new_pro_t++;
	 		    						}else{
	 		    							new_pro_r++;
	 		    						}
	 		    					}
 		    						counter_latent[index_id]=0;
 		    						
 		    					}
 		    				}
 		    			
 		    			} else if ((s_id==2) & (counter_prodro[index_id] < param->shape_prodro)){
 		    				random=gsl_ran_flat(RNG,0,1);
 		    				counter_latent[index_id]=0;
	 		    			if (random< (1- exp(-(param->shape_prodro *param->mu_p)*param->deltat))){
	 		    				
 		    					status[j] = 2;
 		    					counter_prodro[index_id] = counter_prodro[index_id] +1;
	 		    				
	 		    				if (counter_prodro[index_id] == param->shape_prodro){
	 		    					
	 		    					
	 		    					
	 		    					
	 		    					//counter_prodro[j]=0;
	 		    					random1=gsl_ran_flat(RNG,0,1);
	 		    					// 6 to identified teachers
	 		    					if ( ((role_id==6) & (random1<psub_tc->novax[nw]) & (vaccinated_2doses[index_id]==0) 
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==0))|
	 		    					 ((role_id==6) & (random1<psub_tc->twodoses[nw]) & (vaccinated_2doses[index_id]==0) 
	 		    					 & (vaccinated_booster[index_id]==0)& (r_delta[index_id]==1))|
	 		    					((role_id==6) & (random1<psub_tc->twodoses[nweek_2doses[index_id]]) & (vaccinated_2doses[index_id]==1)) |
	 		    					((role_id==6) & (random1<psub_tc->booster[nweek_booster[index_id]]) & (vaccinated_booster[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->novax[nw]) & (vaccinated_2doses[index_id]==0)
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==0)) |
	 		    					 ((role_id!=6) & (random1<psub_st->twodoses[nw]) & (vaccinated_2doses[index_id]==0)
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->twodoses[nweek_2doses[index_id]]) & (vaccinated_2doses[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->booster[nweek_booster[index_id]]) & (vaccinated_booster[index_id]==1))
	 		    					  ){
	 		    						//fprintf(stderr, "Inside subclinical\n");
	 		    						status[index_id]=5; 
	 		    						new_is++;
	 		    						if (role_id!=6){
	 		    							new_is_s++;
	 		    						}else if (role_id==6){
	 		    							if (teach_rep[index_t(tc,id)]==0){
	 		    								new_is_t++;
	 		    							}else{
	 		    								new_is_r++;
	 		    							}
	 		    						}
	 		    					//5 for subclinical case
	 		    					} else {
	 		    					
	 		    						
	 								random2=gsl_ran_flat(RNG,0,1);
	 								// Clinical detected
	 									
	 		    						if (((role_id==6) & (random2<pd[1])) | ((role_id!=6) & (random2<pd[0])) ){
	 		    					
	 		    							
	 		    							new_ic_d++;
	 		    							
	 		    							random3=gsl_ran_flat(RNG,0,1);
	 		    							if (role_id!=6){
	 		    								n_test_symp_st++;
	 		    							}else{
	 		    								n_test_symp_tc++;
	 		    							}
	 		    							n_test_symp++;
	 		    							//fprintf(stderr, "%d\t %f\n", counter_infect[index_id], theta_cs[counter_infect[index_id]]);
	 		    							if (((random3<theta_cs[0][counter_infect[index_id]]) & (data->classes[index_id]!=6)) | ((random3<theta_cs[1][counter_infect[index_id]]) & (data->classes[index_id]==6))){
	 		    							
	 		    								if (role_id!=6){
	 		    									fprintf(stderr, "%d\t %f\t %f\n", counter_infect[index_id], theta_cs[0][counter_infect[index_id]], theta_cs[0][0]);
	 		    								}else{
	 		    									fprintf(stderr, "%d\t %f\t %f\n", counter_infect[index_id], theta_cs[1][counter_infect[index_id]], theta_cs[0][0]);
	 		    								}
	 		    					
	 		    								n_test_symp_p++;
	 		    								result=1;
	 		    								if (role_id!=6){
	 		    									role=1;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]==0)){
	 		    									role=2;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]>=1)){
	 		    									role=3;
	 		    								}
	 		    								
	 		    								
	 		    								//IF stud/teach/repl_teach
	 		    								if(role_id!=6){ 
	 		    								
	 		    									q1_id[n_transitions1]=id;
	 		    									q1_quarter[n_transitions1]=m_absolute;
	 		    									q1_result[n_transitions1]=1;
	 		    									
	 		    									//fprintf(stderr, "n_transitions1:%d\n", n_transitions1);
	 		   									
	 		   									
	 		    									q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    									q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   									q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   						
	 		   									n_transitions1++;
	 		   									
	 		   								
	 		   									status[index_id]=4;
	 		   									detected[index_id]=1;
	 		   									
	 		   									new_ic_d_s++;
	 		   									
	 		   									// If identified during a preventive	
	 		   									if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    										pre_flag[index_id]=0;
	 		    									}
	 		    									
	 		    								}else if (role_id==6){
	 		    								
	 		    									if (teach_rep[index_t(tc, id)]==0){
	 		    									
	 		    										q1_id[n_transitions1]=id;
	 		    										q1_quarter[n_transitions1]=m_absolute;
	 		    										q1_result[n_transitions1]=1;
	 		    									
	 		    										q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    										q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   										q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   										
	 		   										
	 		   										n_transitions1++;
	 		   										
	 		    										detected_t[index_t(tc,id)]=1;
	 		   										teach_rep[index_t(tc,id)]=1;
	 		   										status[index_id]=0;
	 		   										
	 		   										new_ic_d_t++;
	 		    										
	 		    										status_orig=4;
	 		    										reason=1;
	 		    										
	 		    										
	 		    										// A teacher can be replaced because symp.
	 		    										
	 		    										// If identified during a preventive	
	 		   										if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    											pre_flag[index_id]=0;
	 		    										}
	 		    									}else if (teach_rep[index_t(tc, id)]>=1){
	 		    									
	 		    										
	 		    										status[index_id]=0;
	 		    										new_ic_d_r++;
	 		    									
	 		    										status_orig=4;
	 		    										reason=4;
	 		    								
	 		    										teach_rep[index_t(tc,id)]=2;
	 		    										
	 		   										if (preventive==1){
	 		   										
	 		   											q1_id[n_transitions1]=id;
	 		    											q1_quarter[n_transitions1]=m_absolute;
	 		   											q1_result[n_transitions1]=1;
	 		   										
	 		   											q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    											q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   											q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));	
	 		   											
	 		   											n_transitions1++;
	 		   										}
	 		    									}
	 		    										
	 		   								}
	 		   								
	 		    								
	 		    							
	 		    							// (that eventually triggers a protocol)
	 		    							
	 		    								
	 		    							} else {
	 		    							
	 		    								n_test_symp_n++;
	 		    								/*result=0;
	 		    								if (role_id!=6){
	 		    									role=1;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]==0)){
	 		    									role=2;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]>=1)){
	 		    									role=3;
	 		    								}
	 		    								*/
	 		    								
	 		    								if(role_id!=6){
	 		    								 												q1_id[n_transitions1]=id;
	 		    									q1_quarter[n_transitions1]=m_absolute;
	 		    									q1_result[n_transitions1]=0;
	 		    									
	 		    									q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    									q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   									q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   									
	 		   									n_transitions1++;
	 		   									
	 		   									status[index_id]=4;
	 		   									detected[index_id]=1;
	 		   									new_ic_d_s++;
	 		   										
	 		   									if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    										pre_flag[index_id]=0;
	 		    									}
	 		    								}else if (role_id==6){
	 		   									
	 		   									if (teach_rep[index_t(tc, id)]==0){
	 		   									
	 		   										q1_id[n_transitions1]=id;
	 		    										q1_quarter[n_transitions1]=m_absolute;
	 		    										q1_result[n_transitions1]=0;
	 		    									
	 		    										q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    										q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   										q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   										
	 		   										n_transitions1++;
	 		   									
	 		   										detected_t[index_t(tc,id)]=1;
	 		   										status[index_id]=0;
	 		   										
	 		   										new_ic_d_t++;
	 		    										
	 		    										status_orig=4;
	 		    										reason=1;
	 		    										
	 		    										
	 		    										
	 		    								
	 		    										teach_rep[index_t(tc,id)]=1;
	 		    										
	 		    										if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    											pre_flag[index_id]=0;
	 		    										}
	 		   									}else if (teach_rep[index_t(tc, id)]>=1){
	 		    									
	 		    										status[index_id]=0;
	 		    										new_ic_d_r++;
	 		    									
	 		    										status_orig=4;
	 		    										reason=4;
	 		    								
	 		    										teach_rep[index_t(tc,id)]=2;
	 		    										
	 		   										if (preventive==1){
	 		   										
	 		   											q1_id[n_transitions1]=id;
	 		    											q1_quarter[n_transitions1]=m_absolute;
	 		   											q1_result[n_transitions1]=0;
	 		   										
	 		   											q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    											q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   											q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));			
	 		   											
	 		   											
	 		   											n_transitions1++;
	 		   										}
	 		   									}
	 		   								}
	 		    							
	 		    							}
	 		    						// Clinical not detected 
	 		    						}else{
	 		    						
	 		    							
	 		    							
	 		    							status[index_id]=4;
	 		    							new_ic++;
	 		    							
	 		    							if (role_id!=6){
	 		    								new_ic_s++;
	 		    								detected[index_id]=0;
	 		    							}else if (role_id==6){
	 		    								if (teach_rep[index_t(tc,id)]==0){
	 		    									new_ic_t++;
	 		    									detected_t[index_t(tc,id)]=0;
	 		    								}else{
	 		    									new_ic_r++;
	 		    								}
	 		    							}
	 		    						}   
	 							}			
	 					 	}
	 					 }
 					 	//fprintf(stderr, "end prodromic");
 		    			} else if(((s_id==4) & (counter_infect[j]<param->shape_infect)) | 
 		    			((s_id==5) & (counter_infect[j]<param->shape_infect))){	
 		    		        	random=gsl_ran_flat(RNG,0,1);	
 		    		        	counter_prodro[index_id]=0;
	    					if(random< 1 - exp(-(param->shape_infect * param->mu)*param->deltat)){
	    						if (s_id==4){
	    							status[index_id]=4;
	    						}else if (s_id==5){
	    							status[index_id]=5;
	    						}
	    						counter_infect[j]=counter_infect[j]+1;
	    						if (counter_infect[j] == param->shape_infect){
	    							
	    							status[index_id]=8;          // 8 for recovery but still positive to the te
	    							//counter_infect[j] = 0;
	    						}
	    					}	

 		    			} else if ((s_id==8) & (counter_rp[index_id]<param->shape_rp)){
 		    				random4=gsl_ran_flat(RNG,0.0,1.0);
 		    				counter_infect[j] = 0;
 		    				if (role_id!=6){	
 		    					if(random4 < (1 - exp(-(param->shape_rp * param->mu_rp)*param->deltat))){
 		    						counter_rp[index_id]=counter_rp[index_id]+1;
 		    						if (counter_rp[index_id]==param->shape_rp){
 		    							status[index_id]=9;
 		    							counter_rp[index_id] = 0;
 		    						}
 		    					}
 		    				}else if(role_id==6){
 		    				
 		    					if (symptomatic==1){
 		    						for(hh=0; hh<n_transitions2; hh++){
 		    							if((id==q2_id[hh]) & (random4<1 - exp(-(param->shape_rp * param->mu_rp1)*param->deltat))){
 		    								counter_rp[index_id]=counter_rp[index_id]+1;
		 		    						if (counter_rp[index_id]==param->shape_rp){
		 		    							status[index_id]=9;
		 		    							counter_rp[index_id] = 0;
		 		    						}
		 		    								
 		    							}else if ((id!=q2_id[hh]) & (random4<1 - exp(-(param->shape_rp * param->mu_rp)*param->deltat))){
 		    								counter_rp[index_id]=counter_rp[index_id]+1;
		 		    						if (counter_rp[index_id]==param->shape_rp){
		 		    							status[index_id]=9;
		 		    							counter_rp[index_id] = 0;
		 		    						}
 		    							}
 		    						}
 		    					}
 		    					
 		    					
 		    					
 		    				}
 		    			}    	
 		    		}	
 		
 		
 				
 		
 					save_status(data,status,status0); 
	  	
	  			}
	  	
	
	  			
	  			fprintf(fp5, "%d\t %d\t %d\t %d\t %d\t %d\n", nr, day_simulation, new_exposed, new_exp_s, new_exp_t, new_exp_r);
	  			fflush(fp5);
	  				
	  	
	  			d5=d5+week;
	  	                           
      			break;
      		case 6:	
    		
    			new_exposed=0;
    			new_pro=0;
    			new_ic_d=0;
    			new_ic=0;
    			new_is=0;
    			new_q3=0;
    		
    			// New intro in each compartments divided by type: STUDENTS/ TEACHERS/ REPLACED TEACHERS
    			new_exp_s=0;new_exp_t=0;new_exp_r=0;
				new_pro_s=0;new_pro_t=0;new_pro_r=0;new_ic_s=0;
    			new_ic_t=0;new_ic_r=0;new_ic_d_s=0;new_ic_d_t=0;new_ic_d_r=0;
				new_is_s=0;new_is_t=0;new_is_r=0;
				new_q2_s=0;new_q2_t=0;new_pos_r=0;new_q2_reg=0;new_q2_regs=0;new_q2_regt=0;new_q2_regr=0;new_neg_s=0;new_neg_t=0;new_neg_r=0;
				n_test_symp=0;n_test_symp_p=0; n_test_symp_n=0;n_test_symp_st=0; n_test_symp_tc=0;
      			n_test_t=0;n_test_t_p=0;n_test_t_n=0;
    		
	  			
    			for (m=0; m<num_15min_day; m++){
    			
    				m_absolute=num_15min_day*day_simulation + m; //progression of 15 minutes step from the first day to the last one
	        		
			
					n_susc_s=0;
					n_susc_t=0;
      				// Intro of 1 case if there are still susceptible individuals
      				
      			 	for (i=0; i<data->n; i++){
      					if (((status0[i]==0) | (status0[i]==12)) & (data->ID[i]!=6)){
      						n_susc_s++;
      					}else if (((status0[i]==0) | (status0[i]==12)) & (data->ID[i]==6)){
	  						n_susc_t++;
	  					}
      				}

					if ((n_susc_s!=0) & ((*introst)[nr].weeks[nw]>0) & (n_susc_s>=(*introst)[nr].weeks[nw])){
      			
						for (ni=0; ni<(*introst)[nr].weeks[nw]; ni++){
							if (((day_simulation % (week) + 1)==introductions_days_st[ni]) & (m==introductions_quarter_st[ni])){
 
								typ=weekly_introduction_st(data, tc, status, status0, (*introst)[nr].weeks[nw], double_presence, teach_rep, exp_day, day_simulation);
									
								new_exposed++;
								
								if (typ==1){
									new_exp_s++;
								}	
							}
						}
					}
					if ((n_susc_t!=0) & ((*introtc)[nr].weeks[nw]>0) & (n_susc_t>=(*introtc)[nr].weeks[nw])){
					
						for (ni=0; ni<(*introtc)[nr].weeks[nw]; ni++){
							if (((day_simulation % (week) + 1)==introductions_days_tc[ni]) & (m==introductions_quarter_tc[ni])){
								  
								typ=weekly_introduction_tc(data, tc, status, status0, (*introtc)[nr].weeks[nw], double_presence, teach_rep, exp_day, day_simulation);
									
								new_exposed++;
								
								if (typ==2){
									new_exp_t++;
								}else if (typ==3){
									new_exp_r++;
								}		
							}
						}
					}
 	        		        
					for (j=0; j<data->n; j++){
			
 		    			id=data->ID[j];
 		    			role_id=data->classes[j];
 		   				index_id=index_nodes(data, id);
 		    			s_id=status0[index_id];
 		    		
 		    			
 		    			// End of first isolation Q1
 		    			// From Q1 to Q2
 		    			// n_transitions1 = number of suspected nodes tested 
 		    			if ((symptomatic==1) & (n_transitions1>0) & (n_transitions2>=0)){
 		    		
 		    			 // n_transitions1 = number of symp collected
 		    			 // if there is only sympt ON (symptomatic==1)
 		    			 // in n_transitions1= stud + original teacher detected
 		    			 // If if there is sympt ON (symptomatic==1) + prevent. ON (preventive==1)
 		    			 // in n_transitions1= stud + original teacher detected + teacher replaced
 		    			 // I need the information on replced to know if they activate a PREVENTIVE or NOT
 		    			 
 		    		  		for (g=0; g<n_transitions1; g++){
 		    			
 		    					// q1_id=id tested because suspected
 		    					// At the time of the turnaround time, we known the result			
							if((id==q1_id[g]) & (m_absolute==q1_quarter[g]+(param->tau_test/param->deltat))){
 		    				
 		    						//If id is a student
 		    						if (role_id!=6){
 		    							// IF positive go in Q2 and its contacts are OFF
 		    							if (q1_result[g]==1){
 		    						
 		    								new_q2_s++;
 		    							
 		    								detected[index_id]=0;
 		    								quarantine[index_id]=1;
 		    								num_iso[index_id]=1;
 		    								q2_id[n_transitions2]=id;
 		   		        					q2_quarter[n_transitions2]=m_absolute;
 		    						
 		    								q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    								q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    								
 		    								
 		    								n_transitions2++;
 		    							
 		    								// Count the number of additional active classes
 		    								
 		    								if ((reactive == 1) 
 		    								& (reactive_class[index_class(tc, data->classes[index_id])]==0) & (screenstatus[index_id]!=6)){
 		    								
 		    									id_reactive[n_reactive] = id;
 		    									
 		    									if (param->deltar==1440){
 		    										m_reactive[n_reactive] = m_absolute + (num_15min_day - m) + start[0];
 		    									}
 		    									
 		    									if (param->deltar==0){
 		    										
 		    										m_reactive[n_reactive] = m_absolute;
 		    									}
 		    									
 		    									id_reactive=(int*)realloc(id_reactive,(n_reactive+2)*sizeof(int)); 		    										
 		    									m_reactive=(int*)realloc(m_reactive,(n_reactive+2)*sizeof(int));
	 		    									
 		    									
 		    									reactive_class[index_class(tc, data->classes[index_id])]=1;
 		    									n_reactive++;
	 		    									
	 		    								
 		    								}	
	 		    							
 		    								if ((preventive==1) & (regular==0)){
 		    								
 		    									class_q3=data->classes[index_id];
 		    									teach_q3=find(tc, id, class_q3);
 		    								
 		    									// If the class in not already in Q3
 		    				 					if (class_iso[index_t(tc, teach_q3)]==0){
 		    							
 		    										// Turn on if the class is in pre iso
 		    										class_iso[index_t(tc, teach_q3)]=1;
 		    										
 		    									
 		    										dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    										
 		    										
 		    										trigger_role=1;
 		    									
 		    										
 		    							
 		    										new_q3=new_q3+dim;
 		    							
 		    									//fprintf(stderr, "teach in Q3: %d\t interval: %d\t %d\t day: %d\t s%d\n", teach_q1, m,m_absolute, day_simulation, status0[index_nodes(data, teach_q1)]);
 		    									}else{
 		    										pre_flag[index_id]=0;
 		    									}
 		    								}

 		    							}else{
 		    								detected[index_id]=0;
 		    								new_neg_s++;
 		    								// If a students is tested negative during preisolation
 		    								// he/she contin in Q3
 		    								if (preventive==1){
 		    									class_q3=data->classes[index_id];
 		    									teach_q3=find(tc, id, class_q3);
 		    									if (class_iso[index_t(tc, teach_q3)]==1){
 		    										pre_flag[index_id]=1;
 		    									}
 		    								}
 		    							}
 		    							// IF id is a teacher:
 		    							// a. original
 		    							// b. replaced
 		    						}else if (role_id==6){
 		    							// IN q1 the original teacher is already replaced
 		    							// At this point, teach_rep==1
 		    							if (teach_rep[index_t(tc, id)]==1){
 		    								// IF positive 
 		    								// Save the moment in which go in Q2
 		    								if (q1_result[g]==1){
 		    							
 		    									new_q2_t++;
 		    								
 		    									q2_id[n_transitions2]=id;
 		   		        						q2_quarter[n_transitions2]=m_absolute;
 		    									//q2_day[n_transitions1]=day_simulation;
 		    				
 		    									q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    									q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    									//q2_day=(int*)realloc(q2_day,(n_transitions2+2)*sizeof(int));
 		    									
 		    									fprintf(stderr, " q2 id: %d\t start Q2 interval: %d\t start Q2 day: %d\n", q2_id[n_transitions2], q2_quarter[n_transitions2], day_simulation);
 		    									
 		    									n_transitions2++;
 		    							
 		    									// Count the number of additional active classes
 		    									
 		    									detected_t[index_t(tc,id)]=0;
 		    									quarantine_t[index_t(tc, id)]=1;
 		    									
 		    									
	 		    								
 		    									if ((preventive==1) & (regular==0)){
 		    					
 		    				 						if (class_iso[index_t(tc, id)]==0){
 		    							
 		    											// Turn on if the class is in pre iso
 		    											class_iso[index_t(tc, id)]=1;
 		    											
 		    											dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    											
 		    											trigger_role=2;
 		    						
 		    											
 		    											// If the teacher is the trigger, the 
 		    											// replace teacher is however in
 		    											// isolation with the class
 		    							
 		    											new_q3=new_q3+dim;
 		    										}
 		    										// If a teacher go in in Q2 during a preventive
 		    										// the pre_flag is ON for the replaced teacher
 		    									}
 		    								// Otherwise back to school as clinical
 		    								}else {
 		    							
 		    									status[index_id]=4;
 		    									detected_t[index_t(tc,id)]=0;
 		    									new_neg_t++;
 		    									
 		    									if (preventive==1){
 		    										
 		    										if (class_iso[index_t(tc, id)]==1){
 		    											pre_flag[index_id]=1;
 		    										}
 		    									}
 		    								}
 		    								// teach_rep >1 Sostituto del sostituo
 		    								// When the original back, teach_rep==-9
 		    							}else if (teach_rep[index_t(tc, id)]>1){
 		    								
 		    							
 		    								if (q1_result[g]==1){ 
 		    									new_pos_r++;
 		    								
 		    				 					if ((preventive==1) & (regular==0) & (class_iso[index_t(tc, id)]==0)){
 		    							
 		    										// Turn on if the class is in pre iso
 		    										class_iso[index_t(tc, id)]=1;
 		    						
 		    										dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    										
 		    										trigger_role=3;
 		    										
 		    							
 		    										new_q3=new_q3+dim;
 		    									}
 		    								}else{
 		    									new_neg_r++;
 		    									
 		    									if (preventive==1){
 		    										
 		    										if (class_iso[index_t(tc, id)]==1){
 		    											pre_flag[index_id]=1;
 		    										}
 		    									}
 		    								}
 		    							}
 		    						}
 		    					
 		    					}	
 		    				}
 		    				
 		    				if (reactive == 1){
 		    				
	 		    				for(hg=0; hg < n_reactive; hg++){
	 		    				
	 		    					if((id == id_reactive[hg]) & (m_absolute == m_reactive[hg])){
	 		    					
	 		    					
	 		    						//fprintf(stderr, "REACTIVE day %d\t m %d\t m_abs %d\n", day_simulation, m, m_absolute);
	 		    					
	 		    						reactive_screening(data, tc, param, theta_ps, theta_cs, theta_scs, theta_rps, id, day_simulation, m_absolute, nr, status0, screenstatus, start_q2, quarantine, detected, classtotest, indtotest, when, counter_prodro, counter_infect, counter_rp, fp16, fp18);
	 		    						
	 		    						if (data->classes[index_id]!=6){
	 		    							reactive_class[index_class(tc, data->classes[index_id])]=0;
	 		    						}else{
	 		    							reactive_class[index_t(tc, id)]=0;
	 		    						}
	 		    					}
	 		    					
	 		    		  		}
	 		    		  		
	 		    		  		if (param->delay!=0){
	 		    		  		
		 		    		  		for (ncl=0; ncl< tc->nc; ncl++){
		 		    		  			if (((classtotest[ncl]==1) & (m_absolute==when[ncl])) |
		 		    		  			 ((classtotest[ncl]==2) & (m_absolute==when[ncl]+ 2 * num_15min_day)) ){
		 		    		  				post_reactive(data, tc, param, tc->clas[ncl], day_simulation, m_absolute, nr, status0, screenstatus, start_q2, start_rq2, teach_rtested, teach_rep, quarantine, detected, indtotest, theta_ps, theta_cs, theta_scs, theta_rps, counter_prodro, counter_infect, counter_rp, fp17, fp19);
		 		    		  				if (classtotest[ncl]==1){
		 		    		  					classtotest[ncl]=2;
		 		    		  				}
		 		    		  				
		 		    		  				if(m_absolute==when[ncl]+2*num_15min_day){
		 		    		  					classtotest[ncl]=0;
		 		    		  					
		 		    		  					for (z=0; z<data->n; z++){
		 		    		  						if((data->classes[z]==tc->clas[ncl]) 
		 		    		  						& (indtotest[z]==1)){
		 		    		  							indtotest[z]=0;
		 		    		  						}
		 		    		  						
		 		    		  						if((data->classes[z]==6) 
		 		    		  						& (data->ID[z]!=tc->ID[ncl])
		 		    		  						& (indtotest[z]==1)){
		 		    		  							indtotest[z]=0;
		 		    		  						}
		 		    		  						
		 		    		  					}
		 		    		  				}
		 		    		  			}
		 		    		  		}
	 		    		  		}
 		    		  		}
 		    		  		
 		    		  		// End of quarantine for IC(d) in Q2
 		    		  	  	for(hh=0; hh<n_transitions2; hh++){
 		    			
 		    		  			if( (id==q2_id[hh]) & (m_absolute == q2_quarter[hh]+(param->delta_q/param->deltat) ) ){
 		   				
 		   						if (role_id!=6){ 
 		   					
 		   							quarantine[index_id]=0;
 		   							
 		   						
 		   						}else if(role_id==6){
 		    					
 		    							// We test the original teacher at the end of Q2, and we assume that 
 		    							// status of the original at the end is equal to R+ (8)
 		    							// So we test using a sensitivity teta_r+
 		    						
 		    							// Quarantine_t and teach_rep are turnd off only if the result of
 		    							// the test is negative
 		    						
 		    							r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		    							//fprintf(stderr, "q2 id:%d\t start Q2: %d\t end Q2: %d\t (day:%d)\t role: %d\t result:%d\n", id, q2_quarter[hh], m_absolute, day_simulation, role_id, r);
 		    						
 		    							// If during quarantine the replace teacher goes in isoaltion with 
 		    							// the class when the teacher come back (i.e the result of the test is negative),
 		    							// preflag is 0
 		    							//if((pre_flag[index_id]==1) & (r==0)){
 		    							//	pre_flag[index_id]=0;
 		    							//}
 		    						
 		    							// Counter on the number of tests on the original teacher
 		    							// before back to school
 		    							
 		    							n_test_t++;
 		    						
 		    							if (r==1){
 		    								n_test_t_p++;
 		    							}else{
 		    								n_test_t_n++;
 		    							}
 		    						
 		    						}
 		    					
 		    					}
 		    		  	  	}			
 		    			}
 		    		
 		    			// End of preventive isolation	
 		    			if (preventive==1){
 		    			
 		    				if ((pre_flag[index_id]==1) & (m_absolute==pre_time[1][index_id]+(param->delta_q/param->deltat))){
 		       			
 		       					if (role_id!=6){
 		       				
 		       						pre_flag[index_id]=0;
 		       						tea=find(tc, id, data->classes[index_id]);
 		        			
 		       						if(class_iso[index_t(tc, tea)]==1)
 		       							class_iso[index_t(tc, tea)]=0;
 		       							
 		       						
 		       					
 		       					}else if ((role_id==6) & (quarantine_t[index_t(tc,id)]==0)){
 		       					
 		        					r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		        					//fprintf(stderr, "teach id in Q3:%d\t start Q3: %d\t (day:%d)\t  end Q3: %d\t (day:%d)\t role: %d\t result:%d\n", id, pre_time[1][index_id], pre_time[0][index_id], m_absolute, day_simulation, role_id, r);
 		        					
 		        					// Counter on the number of tests on the original teacher 
 		        					// before back to school
 		        					n_test_t++;
 		        							
 		        					if (r==1){
 		    							n_test_t_p++;
 		    						}else{
 		    							n_test_t_n++;
 		    						}
 		        		
 		        			
 		       					}
 		       					pre_flag[index_id]=0;
 		       					/*for (ttts=0; ttts<tc->nc; ttts++){
	       							fprintf(stderr, "flag class in iso: %d\t %d\n", tc->clas[ttts], class_iso[ttts]);
	       							
	       						}*/
 		       				}
 		       			}
 		       			if ((regular==1) | (reactive==1) ){
 		       				//start q2 hold only for original teach
 		       				
	  					if ((screenstatus[j]==6) & ((m_absolute==start_q2[j]) | (m_absolute==start_rq2[j]))){
	  					
	  						
	  						new_q2_reg++;
	  						if ((role_id!=6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
	  						
	  							
	  							
	  							new_q2_regs++;
	  							quarantine[j]=1;
	  							
	  							if ((preventive == 1) & 
	  							(num_pos_class[index_class(tc, data->classes[index_id])]>=3) &
	  							(class_iso[index_class(tc, data->classes[index_id])]==0)){
	  							
	  								
		 		    		  			 preisolation(data, tc, param, id, role_id, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
		 		    		  		}
	  							
	  						}else if ( (role_id==6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
	  						
	  							
	  							
	  							new_q2_regt++;
	  							quarantine_t[index_t(tc,id)]=1;
	  							status[j]=0;
	  						}else if ((m_absolute==start_rq2[j]) & (start_rq2[j]!=0) & (role_id==6) & (teach_rtested[index_t(tc, id)]>1)){
	  						
	  							new_q2_regr++;
	  							status[j]=0;
	  							
	  							
	  							
	  							
	  								
	  						}
	  						screenstatus[j]=-9;
	  						
	  					}else if ((m_absolute==start_q2[index_id]+param->delta_q/param->deltat) & (start_q2[j]!=0)){
 		       				 	
 		       				 	if (role_id!=6){
 		       				 		quarantine[index_id]=0;
 		       					}else if (role_id==6){
 		       					
 		       						r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		        					// Counter on the number of tests on the original teacher 
 		        					// before back to school
 		        					n_test_t++;
 		        							
 		        					if (r==1){
 		    							n_test_t_p++;
 		    						}else{
 		    							n_test_t_n++;
 		    							teach_neg_reg[n_reg]=id;
 		    							teach_neg_reg=(int*)realloc(teach_neg_reg,(n_reg+2)*sizeof(int));
 		    							n_reg++;
 		    						}
 		    						
 		    					}
 		       				 }
 		       			}
 		       			
 		    			 
 		    			// End of second isolation
 		    			if ((symptomatic==1) | ((symptomatic==1) & (preventive==1)) | (regular==1)){
 		    				if (role_id==6){
 		    					if ((teach_2iso[index_t(tc, id)]==1) 
 		    					& (m_absolute==teach_2isom[index_t(tc, id)]+param->delta_q/param->deltat)){
 		    					
 		    						status[index_id]=9;
 		    						teach_2iso[index_t(tc, id)]=-9;
 		    						quarantine_t[index_t(tc, id)]=0;
 		    						teach_rep[index_t(tc,id)]=-9;
 		    						
 		    						
 		    					}
 		    				}			
 		        		}

 		        		// INDIPENDENT TRANSITIONs:
 						// TRANSITION FROM E->IP 				
 						// TRANSITION FROM IP->IC_detected		
 						// TRANSITION FROM IP->IC
 						// TRANSITION FROM IP->ISC		
 						// TRANSITION FROM Q1->Q2
 						// TRANSITION FROM IC->R+ o ISC->R+ 	
 						// TRANSITION FORM ICDI-> R+		
 						// TRANSITION FORM R+ -> R
 			
 		    			if( (s_id==1) & (counter_latent[index_id] < param->shape_latent)){
 		    				random = gsl_ran_flat(RNG,0,1);
 		    				if(random < (1 - exp(-(param->shape_latent * param->eps) * param->deltat))){
 		    				
 		    					status[index_id]=1;  // 2 for prodromic phas
 		    					counter_latent[index_id]=counter_latent[index_id]+1;
 		    					
 		    					if (counter_latent[index_id] == param->shape_latent){
 		    						status[index_id]=2;
 		    						
 		    						new_pro++;
 		    						if (role_id!=6){
	 		    						new_pro_s++;
	 		    					}else if (role_id==6){
	 		    						if (teach_rep[index_t(tc,id)]==0){
	 		    							new_pro_t++;
	 		    						}else{
	 		    							new_pro_r++;
	 		    						}
	 		    					}
 		    						counter_latent[index_id]=0;
 		    						
 		    					}
 		    				}
 		    			
 		    			} else if ((s_id==2) & (counter_prodro[j] < param->shape_prodro)){
 		    				random=gsl_ran_flat(RNG,0,1);
 		    				counter_latent[index_id]=0;
	 		    			if (random< (1- exp(-(param->shape_prodro *param->mu_p)*param->deltat))){
	 		    				
 		    					status[j] = 2;
 		    					counter_prodro[index_id] = counter_prodro[index_id] +1;
	 		    				
	 		    				if (counter_prodro[index_id] == param->shape_prodro){
	 		    					
	 		    					
	 		    					
	 		    					//counter_prodro[j]=0;
	 		    					random1=gsl_ran_flat(RNG,0,1);
	 		    					// 6 to identified teachers
	 		    					if ( ((role_id==6) & (random1<psub_tc->novax[nw]) & (vaccinated_2doses[index_id]==0) 
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==0))|
	 		    					 ((role_id==6) & (random1<psub_tc->twodoses[nw]) & (vaccinated_2doses[index_id]==0) 
	 		    					 & (vaccinated_booster[index_id]==0)& (r_delta[index_id]==1))|
	 		    					((role_id==6) & (random1<psub_tc->twodoses[nweek_2doses[index_id]]) & (vaccinated_2doses[index_id]==1)) |
	 		    					((role_id==6) & (random1<psub_tc->booster[nweek_booster[index_id]]) & (vaccinated_booster[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->novax[nw]) & (vaccinated_2doses[index_id]==0)
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==0)) |
	 		    					 ((role_id!=6) & (random1<psub_st->twodoses[nw]) & (vaccinated_2doses[index_id]==0)
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->twodoses[nweek_2doses[index_id]]) & (vaccinated_2doses[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->booster[nweek_booster[index_id]]) & (vaccinated_booster[index_id]==1))
	 		    					  ){
	 		    						//fprintf(stderr, "Inside subclinical\n");
	 		    						status[index_id]=5; 
	 		    						new_is++;
	 		    						if (role_id!=6){
	 		    							new_is_s++;
	 		    						}else if (role_id==6){
	 		    							if (teach_rep[index_t(tc,id)]==0){
	 		    								new_is_t++;
	 		    							}else{
	 		    								new_is_r++;
	 		    							}
	 		    						}
	 		    					//5 for subclinical case
	 		    					} else {
	 		    					
	 		    						
	 								random2=gsl_ran_flat(RNG,0,1);
	 								// Clinical detected
	 									
	 		    						if (((role_id==6) & (random2<pd[1])) | ((role_id!=6) & (random2<pd[0])) ){
	 		    					
	 		    							
	 		    							new_ic_d++;
	 		    							
	 		    							random3=gsl_ran_flat(RNG,0,1);
	 		    							if (role_id!=6){
	 		    								n_test_symp_st++;
	 		    							}else{
	 		    								n_test_symp_tc++;
	 		    							}
	 		    							n_test_symp++;
	 		    							//fprintf(stderr, "%d\t %f\n", counter_infect[index_id], theta_cs[counter_infect[index_id]]);
	 		    							if (((random3<theta_cs[0][counter_infect[index_id]]) & (data->classes[index_id]!=6)) | ((random3<theta_cs[1][counter_infect[index_id]]) & (data->classes[index_id]==6))){
	 		    							
	 		    								if (role_id!=6){
	 		    									fprintf(stderr, "%d\t %f\t %f\n", counter_infect[index_id], theta_cs[0][counter_infect[index_id]], theta_cs[0][0]);
	 		    								}else{
	 		    									fprintf(stderr, "%d\t %f\t %f\n", counter_infect[index_id], theta_cs[1][counter_infect[index_id]], theta_cs[0][0]);
	 		    								}
	 		    					
	 		    								n_test_symp_p++;
	 		    								result=1;
	 		    								if (role_id!=6){
	 		    									role=1;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]==0)){
	 		    									role=2;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]>=1)){
	 		    									role=3;
	 		    								}
	 		    								
	 		    								
	 		    								//IF stud/teach/repl_teach
	 		    								if(role_id!=6){ 
	 		    								
	 		    									q1_id[n_transitions1]=id;
	 		    									q1_quarter[n_transitions1]=m_absolute;
	 		    									q1_result[n_transitions1]=1;
	 		    									
	 		    									//fprintf(stderr, "n_transitions1:%d\n", n_transitions1);
	 		   									
	 		   									
	 		    									q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    									q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   									q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   						
	 		   									n_transitions1++;
	 		   									
	 		   								
	 		   									status[index_id]=4;
	 		   									detected[index_id]=1;
	 		   									
	 		   									new_ic_d_s++;
	 		   									
	 		   									// If identified during a preventive	
	 		   									if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    										pre_flag[index_id]=0;
	 		    									}
	 		    									
	 		    								}else if (role_id==6){
	 		    								
	 		    									if (teach_rep[index_t(tc, id)]==0){
	 		    									
	 		    										q1_id[n_transitions1]=id;
	 		    										q1_quarter[n_transitions1]=m_absolute;
	 		    										q1_result[n_transitions1]=1;
	 		    									
	 		    										q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    										q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   										q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   										
	 		   										
	 		   										n_transitions1++;
	 		   										
	 		    										detected_t[index_t(tc,id)]=1;
	 		   										teach_rep[index_t(tc,id)]=1;
	 		   										status[index_id]=0;
	 		   										
	 		   										new_ic_d_t++;
	 		    										
	 		    										status_orig=4;
	 		    										reason=1;
	 		    										
	 		    										
	 		    										// A teacher can be replaced because symp.
	 		    										
	 		    										// If identified during a preventive	
	 		   										if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    											pre_flag[index_id]=0;
	 		    										}
	 		    									}else if (teach_rep[index_t(tc, id)]>=1){
	 		    									
	 		    										
	 		    										status[index_id]=0;
	 		    										new_ic_d_r++;
	 		    									
	 		    										status_orig=4;
	 		    										reason=4;
	 		    								
	 		    										teach_rep[index_t(tc,id)]=2;
	 		    										
	 		   										if (preventive==1){
	 		   										
	 		   											q1_id[n_transitions1]=id;
	 		    											q1_quarter[n_transitions1]=m_absolute;
	 		   											q1_result[n_transitions1]=1;
	 		   										
	 		   											q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    											q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   											q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));	
	 		   											
	 		   											n_transitions1++;
	 		   										}
	 		    									}
	 		    										
	 		   								}
	 		   								
	 		    								
	 		    							
	 		    							// (that eventually triggers a protocol)
	 		    							
	 		    								
	 		    							} else {
	 		    							
	 		    								n_test_symp_n++;
	 		    								/*result=0;
	 		    								if (role_id!=6){
	 		    									role=1;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]==0)){
	 		    									role=2;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]>=1)){
	 		    									role=3;
	 		    								}
	 		    								*/
	 		    								
	 		    								if(role_id!=6){
	 		    								 												q1_id[n_transitions1]=id;
	 		    									q1_quarter[n_transitions1]=m_absolute;
	 		    									q1_result[n_transitions1]=0;
	 		    									
	 		    									q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    									q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   									q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   									
	 		   									n_transitions1++;
	 		   									
	 		   									status[index_id]=4;
	 		   									detected[index_id]=1;
	 		   									new_ic_d_s++;
	 		   										
	 		   									if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    										pre_flag[index_id]=0;
	 		    									}
	 		    								}else if (role_id==6){
	 		   									
	 		   									if (teach_rep[index_t(tc, id)]==0){
	 		   									
	 		   										q1_id[n_transitions1]=id;
	 		    										q1_quarter[n_transitions1]=m_absolute;
	 		    										q1_result[n_transitions1]=0;
	 		    									
	 		    										q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    										q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   										q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   										
	 		   										n_transitions1++;
	 		   									
	 		   										detected_t[index_t(tc,id)]=1;
	 		   										status[index_id]=0;
	 		   										
	 		   										new_ic_d_t++;
	 		    										
	 		    										status_orig=4;
	 		    										reason=1;
	 		    										
	 		    										
	 		    										
	 		    								
	 		    										teach_rep[index_t(tc,id)]=1;
	 		    										
	 		    										if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    											pre_flag[index_id]=0;
	 		    										}
	 		   									}else if (teach_rep[index_t(tc, id)]>=1){
	 		    									
	 		    										status[index_id]=0;
	 		    										new_ic_d_r++;
	 		    									
	 		    										status_orig=4;
	 		    										reason=4;
	 		    								
	 		    										teach_rep[index_t(tc,id)]=2;
	 		    										
	 		   										if (preventive==1){
	 		   										
	 		   											q1_id[n_transitions1]=id;
	 		    											q1_quarter[n_transitions1]=m_absolute;
	 		   											q1_result[n_transitions1]=0;
	 		   										
	 		   											q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    											q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   											q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));			
	 		   											
	 		   											
	 		   											n_transitions1++;
	 		   										}
	 		   									}
	 		   								}
	 		    							
	 		    							}
	 		    						// Clinical not detected 
	 		    						}else{
	 		    						
	 		    							
	 		    							
	 		    							status[index_id]=4;
	 		    							new_ic++;
	 		    							
	 		    							if (role_id!=6){
	 		    								new_ic_s++;
	 		    								detected[index_id]=0;
	 		    							}else if (role_id==6){
	 		    								if (teach_rep[index_t(tc,id)]==0){
	 		    									new_ic_t++;
	 		    									detected_t[index_t(tc,id)]=0;
	 		    								}else{
	 		    									new_ic_r++;
	 		    								}
	 		    							}
	 		    						}   
	 							}			
	 					 	}
	 					 }
 					 	//fprintf(stderr, "end prodromic");
 		    			} else if(((s_id==4) & (counter_infect[j]<param->shape_infect)) | 
 		    			((s_id==5) & (counter_infect[j]<param->shape_infect))){	
 		    		        	random=gsl_ran_flat(RNG,0,1);	
 		    		        	counter_prodro[index_id]=0;
	    					if(random< 1 - exp(-(param->shape_infect * param->mu)*param->deltat)){
	    						if (s_id==4){
	    							status[index_id]=4;
	    						}else if (s_id==5){
	    							status[index_id]=5;
	    						}
	    						counter_infect[j]=counter_infect[j]+1;
	    						if (counter_infect[j] == param->shape_infect){
	    							status[index_id]=8;          // 8 for recovery but still positive to the te
	    							//counter_infect[j] = 0;
	    						}
	    					}	

 		    			} else if ((s_id==8) & (counter_rp[index_id]<param->shape_rp)){
 		    				random4=gsl_ran_flat(RNG,0.0,1.0);
 		    				counter_infect[j] = 0;
 		    				if (role_id!=6){	
 		    					if(random4 < (1 - exp(-(param->shape_rp * param->mu_rp)*param->deltat))){
 		    						counter_rp[index_id]=counter_rp[index_id]+1;
 		    						if (counter_rp[index_id]==param->shape_rp){
 		    							status[index_id]=9;
 		    							counter_rp[index_id] = 0;
 		    						}
 		    					}
 		    				}else if(role_id==6){
 		    				
 		    					if (symptomatic==1){
 		    						for(hh=0; hh<n_transitions2; hh++){
 		    							if((id==q2_id[hh]) & (random4<1 - exp(-(param->shape_rp * param->mu_rp1)*param->deltat))){
 		    								counter_rp[index_id]=counter_rp[index_id]+1;
		 		    						if (counter_rp[index_id]==param->shape_rp){
		 		    							status[index_id]=9;
		 		    							counter_rp[index_id] = 0;
		 		    						}
		 		    								
 		    							}else if ((id!=q2_id[hh]) & (random4<1 - exp(-(param->shape_rp * param->mu_rp)*param->deltat))){
 		    								counter_rp[index_id]=counter_rp[index_id]+1;
		 		    						if (counter_rp[index_id]==param->shape_rp){
		 		    							status[index_id]=9;
		 		    							counter_rp[index_id] = 0;
		 		    						}
 		    							}
 		    						}
 		    					}
 		    					
 		    				}
 		    			}    	
 		    		}	
 		
 		
 				
 		
 					save_status(data,status,status0); 
	  	
	  			}
	  	
	  			
	  			fprintf(fp5, "%d\t %d\t %d\t %d\t %d\t %d\n", nr, day_simulation, new_exposed, new_exp_s, new_exp_t, new_exp_r);
	  			fflush(fp5);
	  			        
      			break;
      		case 7:	
    		
    			new_exposed=0;
    			new_pro=0;
    			new_ic_d=0;
    			new_ic=0;
    			new_is=0;
    			new_q3=0;
    		
    			// New intro in each compartments divided by type: STUDENTS/ TEACHERS/ REPLACED TEACHERS
    			new_exp_s=0;new_exp_t=0;new_exp_r=0;
				new_pro_s=0;new_pro_t=0;new_pro_r=0;new_ic_s=0;
    			new_ic_t=0;new_ic_r=0;new_ic_d_s=0;new_ic_d_t=0;new_ic_d_r=0;
				new_is_s=0;new_is_t=0;new_is_r=0;
				new_q2_s=0;new_q2_t=0;new_pos_r=0;new_q2_reg=0;new_q2_regs=0;new_q2_regt=0;new_q2_regr=0;new_neg_s=0;new_neg_t=0;new_neg_r=0;
				n_test_symp=0;n_test_symp_p=0; n_test_symp_n=0;n_test_symp_st=0; n_test_symp_tc=0;
      			n_test_t=0;n_test_t_p=0;n_test_t_n=0;
    		
	  			
    			for (m=0; m<num_15min_day; m++){
    			
    				m_absolute=num_15min_day*day_simulation + m; //progression of 15 minutes step from the first day to the last one
	        
	        		n_susc_s=0;
					n_susc_t=0;
      				// Intro of 1 case if there are still susceptible individuals
      				
      			 	for (i=0; i<data->n; i++){
      					if (((status0[i]==0) | (status0[i]==12)) & (data->ID[i]!=6)){
      						n_susc_s++;
      					}else if (((status0[i]==0) | (status0[i]==12)) & (data->ID[i]==6)){
	  						n_susc_t++;
	  					}
      				}

					if ((n_susc_s!=0) & ((*introst)[nr].weeks[nw]>0) & (n_susc_s>=(*introst)[nr].weeks[nw])){
      			
						for (ni=0; ni<(*introst)[nr].weeks[nw]; ni++){
							if (((day_simulation % (week) + 1)==introductions_days_st[ni]) & (m==introductions_quarter_st[ni])){
 
								typ=weekly_introduction_st(data, tc, status, status0, (*introst)[nr].weeks[nw], double_presence, teach_rep, exp_day, day_simulation);
									
								new_exposed++;
								
								if (typ==1){
									new_exp_s++;
								}	
							}
						}
					}
					if ((n_susc_t!=0) & ((*introtc)[nr].weeks[nw]>0) & (n_susc_t>=(*introtc)[nr].weeks[nw])){
					
						for (ni=0; ni<(*introtc)[nr].weeks[nw]; ni++){
							if (((day_simulation % (week) + 1)==introductions_days_tc[ni]) & (m==introductions_quarter_tc[ni])){
								  
								typ=weekly_introduction_tc(data, tc, status, status0, (*introtc)[nr].weeks[nw], double_presence, teach_rep, exp_day, day_simulation);
									
								new_exposed++;
								
								if (typ==2){
									new_exp_t++;
								}else if (typ==3){
									new_exp_r++;
								}		
							}
						}
					}
 	        		        
					for (j=0; j<data->n; j++){
			
 		    			id=data->ID[j];
 		    			role_id=data->classes[j];
 		   				index_id=index_nodes(data, id);
 		    			s_id=status0[index_id];
 		    		
 		    			
 		    			// End of first isolation Q1
 		    			// From Q1 to Q2
 		    			// n_transitions1 = number of suspected nodes tested 
 		    			if ((symptomatic==1) & (n_transitions1>0) & (n_transitions2>=0)){
 		    		
 		    			 // n_transitions1 = number of symp collected
 		    			 // if there is only sympt ON (symptomatic==1)
 		    			 // in n_transitions1= stud + original teacher detected
 		    			 // If if there is sympt ON (symptomatic==1) + prevent. ON (preventive==1)
 		    			 // in n_transitions1= stud + original teacher detected + teacher replaced
 		    			 // I need the information on replced to know if they activate a PREVENTIVE or NOT
 		    			 
 		    		  		for (g=0; g<n_transitions1; g++){
 		    			
 		    					// q1_id=id tested because suspected
 		    					// At the time of the turnaround time, we known the result			
							if((id==q1_id[g]) & (m_absolute==q1_quarter[g]+(param->tau_test/param->deltat))){
 		    				
 		    						//If id is a student
 		    						if (role_id!=6){
 		    							// IF positive go in Q2 and its contacts are OFF
 		    							if (q1_result[g]==1){
 		    						
 		    								new_q2_s++;
 		    							
 		    								detected[index_id]=0;
 		    								quarantine[index_id]=1;
 		    								num_iso[index_id]=1;
 		    								q2_id[n_transitions2]=id;
 		   		        					q2_quarter[n_transitions2]=m_absolute;
 		    						
 		    								q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    								q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    								
 		    								
 		    								n_transitions2++;
 		    							
 		    								// Count the number of additional active classes
 		    								
 		    								if ((reactive == 1) & (reactive_class[index_class(tc, data->classes[index_id])]==0) & (screenstatus[index_id]!=6)){
 		    								
 		    									id_reactive[n_reactive] = id;
 		    									
 		    									if (param->deltar==1440){
 		    										m_reactive[n_reactive] = m_absolute + (num_15min_day - m) + start[0];
 		    									}
 		    									
 		    									if (param->deltar==0){
 		    										
 		    										m_reactive[n_reactive] = m_absolute;
 		    									}
 		    									
 		    									id_reactive=(int*)realloc(id_reactive,(n_reactive+2)*sizeof(int)); 		    										
 		    									m_reactive=(int*)realloc(m_reactive,(n_reactive+2)*sizeof(int));
	 		    									
 		    									
 		    									reactive_class[index_class(tc, data->classes[index_id])]=1;
 		    									n_reactive++;
	 		    									
	 		    								
 		    								}	
	 		    								
 		    								if ((preventive==1) & (regular==0)){
 		    								
 		    									class_q3=data->classes[index_id];
 		    									teach_q3=find(tc, id, class_q3);
 		    								
 		    									// If the class in not already in Q3
 		    				 					if (class_iso[index_t(tc, teach_q3)]==0){
 		    							
 		    										// Turn on if the class is in pre iso
 		    										class_iso[index_t(tc, teach_q3)]=1;
 		    										
 		    										dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    										
 		    										trigger_role=1;
 		    									
 		    										
 		    							
 		    										new_q3=new_q3+dim;
 		    							
 		    									//fprintf(stderr, "teach in Q3: %d\t interval: %d\t %d\t day: %d\t s%d\n", teach_q1, m,m_absolute, day_simulation, status0[index_nodes(data, teach_q1)]);
 		    									}else{
 		    										pre_flag[index_id]=0;
 		    									}
 		    								}

 		    							}else{
 		    								detected[index_id]=0;
 		    								new_neg_s++;
 		    								// If a students is tested negative during preisolation
 		    								// he/she contin in Q3
 		    								if (preventive==1){
 		    									class_q3=data->classes[index_id];
 		    									teach_q3=find(tc, id, class_q3);
 		    									if (class_iso[index_t(tc, teach_q3)]==1){
 		    										pre_flag[index_id]=1;
 		    									}
 		    								}
 		    							}
 		    							// IF id is a teacher:
 		    							// a. original
 		    							// b. replaced
 		    						}else if (role_id==6){
 		    							// IN q1 the original teacher is already replaced
 		    							// At this point, teach_rep==1
 		    							if (teach_rep[index_t(tc, id)]==1){
 		    								// IF positive 
 		    								// Save the moment in which go in Q2
 		    								if (q1_result[g]==1){
 		    							
 		    									new_q2_t++;
 		    								
 		    									q2_id[n_transitions2]=id;
 		   		        						q2_quarter[n_transitions2]=m_absolute;
 		    									//q2_day[n_transitions1]=day_simulation;
 		    				
 		    									q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    									q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    									//q2_day=(int*)realloc(q2_day,(n_transitions2+2)*sizeof(int));
 		    									
 		    									fprintf(stderr, " q2 id: %d\t start Q2 interval: %d\t start Q2 day: %d\n", q2_id[n_transitions2], q2_quarter[n_transitions2], day_simulation);
 		    									
 		    									n_transitions2++;
 		    							
 		    									// Count the number of additional active classes
 		    									
 		    									detected_t[index_t(tc,id)]=0;
 		    									quarantine_t[index_t(tc, id)]=1;
 		    								
 		    									
 		    									
	 		    								
 		    									if ((preventive==1) & (regular==0)){
 		    					
 		    				 						if (class_iso[index_t(tc, id)]==0){
 		    							
 		    											// Turn on if the class is in pre iso
 		    											class_iso[index_t(tc, id)]=1;
 		    											
 		    											dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    											
 		    											trigger_role=2;
 		    						
 		    											
 		    											// If the teacher is the trigger, the 
 		    											// replace teacher is however in
 		    											// isolation with the class
 		    							
 		    											new_q3=new_q3+dim;
 		    										}
 		    										// If a teacher go in in Q2 during a preventive
 		    										// the pre_flag is ON for the replaced teacher
 		    									}
 		    								// Otherwise back to school as clinical
 		    								}else {
 		    							
 		    									status[index_id]=4;
 		    									detected_t[index_t(tc,id)]=0;
 		    									new_neg_t++;
 		    									
 		    									if (preventive==1){
 		    										
 		    										if (class_iso[index_t(tc, id)]==1){
 		    											pre_flag[index_id]=1;
 		    										}
 		    									}
 		    								}
 		    								// teach_rep >1 Sostituto del sostituo
 		    								// When the original back, teach_rep==-9
 		    							}else if (teach_rep[index_t(tc, id)]>1){
 		    								
 		    							
 		    								if (q1_result[g]==1){ 
 		    									new_pos_r++;
 		    								
 		    				 					if ((preventive==1) & (regular==0) & (class_iso[index_t(tc, id)]==0)){
 		    							
 		    										// Turn on if the class is in pre iso
 		    										class_iso[index_t(tc, id)]=1;
 		    						
 		    										dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
		    										
 		    										trigger_role=3;
 		    										
 		    							
 		    										new_q3=new_q3+dim;
 		    									}
 		    								}else{
 		    									new_neg_r++;
 		    									
 		    									if (preventive==1){
 		    										
 		    										if (class_iso[index_t(tc, id)]==1){
 		    											pre_flag[index_id]=1;
 		    										}
 		    									}
 		    								}
 		    							}
 		    						}
 		    					
 		    					}	
 		    				}
 		    				
 		    				if (reactive == 1){
 		    				
	 		    				for(hg=0; hg < n_reactive; hg++){
	 		    				
	 		    					if((id == id_reactive[hg]) & (m_absolute == m_reactive[hg])){
	 		    					
	 		    					
	 		    						//fprintf(stderr, "REACTIVE day %d\t m %d\t m_abs %d\n", day_simulation, m, m_absolute);
	 		    					
	 		    						reactive_screening(data, tc, param, theta_ps, theta_cs, theta_scs, theta_rps, id, day_simulation, m_absolute, nr, status0, screenstatus, start_q2, quarantine, detected, classtotest, indtotest, when, counter_prodro, counter_infect, counter_rp, fp16, fp18);
	 		    						
	 		    						if (data->classes[index_id]!=6){
	 		    							reactive_class[index_class(tc, data->classes[index_id])]=0;
	 		    						}else{
	 		    							reactive_class[index_t(tc, id)]=0;
	 		    						}
	 		    					}
	 		    					
	 		    		  		}
	 		    		  		
	 		    		  		if (param->delay!=0){
	 		    		  		
		 		    		  		for (ncl=0; ncl< tc->nc; ncl++){
		 		    		  			if (((classtotest[ncl]==1) & (m_absolute==when[ncl])) |
		 		    		  			 ((classtotest[ncl]==2) & (m_absolute==when[ncl]+ 2 * num_15min_day)) ){
		 		    		  				post_reactive(data, tc, param, tc->clas[ncl], day_simulation, m_absolute, nr, status0, screenstatus, start_q2, start_rq2, teach_rtested, teach_rep, quarantine, detected, indtotest, theta_ps, theta_cs, theta_scs, theta_rps, counter_prodro, counter_infect, counter_rp, fp17, fp19);
		 		    		  				if (classtotest[ncl]==1){
		 		    		  					classtotest[ncl]=2;
		 		    		  				}
		 		    		  				
		 		    		  				if(m_absolute==when[ncl]+2*num_15min_day){
		 		    		  					classtotest[ncl]=0;
		 		    		  					
		 		    		  					for (z=0; z<data->n; z++){
		 		    		  						if((data->classes[z]==tc->clas[ncl]) 
		 		    		  						& (indtotest[z]==1)){
		 		    		  							indtotest[z]=0;
		 		    		  						}
		 		    		  						
		 		    		  						if((data->classes[z]==6) 
		 		    		  						& (data->ID[z]!=tc->ID[ncl])
		 		    		  						& (indtotest[z]==1)){
		 		    		  							indtotest[z]=0;
		 		    		  						}
		 		    		  						
		 		    		  					}
		 		    		  				}
		 		    		  			}
		 		    		  		}
	 		    		  		}
 		    		  		}
 		    				
 		    		  		// End of quarantine for IC(d) in Q2
 		    		  	  	for(hh=0; hh<n_transitions2; hh++){
 		    			
 		    		  			if( (id==q2_id[hh]) & (m_absolute == q2_quarter[hh]+(param->delta_q/param->deltat) ) ){
 		   				
 		   						if (role_id!=6){ 
 		   					
 		   							quarantine[index_id]=0;
 		   							
 		   						
 		   						}else if(role_id==6){
 		    					
 		    							// We test the original teacher at the end of Q2, and we assume that 
 		    							// status of the original at the end is equal to R+ (8)
 		    							// So we test using a sensitivity teta_r+
 		    						
 		    							// Quarantine_t and teach_rep are turnd off only if the result of
 		    							// the test is negative
 		    						
 		    							r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		    							//fprintf(stderr, "q2 id:%d\t start Q2: %d\t end Q2: %d\t (day:%d)\t role: %d\t result:%d\n", id, q2_quarter[hh], m_absolute, day_simulation, role_id, r);
 		    						
 		    							// If during quarantine the replace teacher goes in isoaltion with 
 		    							// the class when the teacher come back (i.e the result of the test is negative),
 		    							// preflag is 0
 		    							//if((pre_flag[index_id]==1) & (r==0)){
 		    							//	pre_flag[index_id]=0;
 		    							//}
 		    						
 		    							// Counter on the number of tests on the original teacher
 		    							// before back to school
 		    							
 		    							n_test_t++;
 		    						
 		    							if (r==1){
 		    								n_test_t_p++;
 		    							}else{
 		    								n_test_t_n++;
 		    							}
 		    						
 		    						}
 		    					
 		    					}
 		    		  	  	}			
 		    			}
 		    		
 		    			// End of preventive isolation	
 		    			if (preventive==1){
 		    			
 		    				if ((pre_flag[index_id]==1) & (m_absolute==pre_time[1][index_id]+(param->delta_q/param->deltat))){
 		       			
 		       					if (role_id!=6){
 		       				
 		       						pre_flag[index_id]=0;
 		       						tea=find(tc, id, data->classes[index_id]);
 		        			
 		       						if(class_iso[index_t(tc, tea)]==1)
 		       							class_iso[index_t(tc, tea)]=0;
 		       						
 		       					
 		       					}else if ((role_id==6) & (quarantine_t[index_t(tc,id)]==0)){
 		       					
 		        					r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		        					//fprintf(stderr, "teach id in Q3:%d\t start Q3: %d\t (day:%d)\t  end Q3: %d\t (day:%d)\t role: %d\t result:%d\n", id, pre_time[1][index_id], pre_time[0][index_id], m_absolute, day_simulation, role_id, r);
 		        					
 		        					// Counter on the number of tests on the original teacher 
 		        					// before back to school
 		        					n_test_t++;
 		        							
 		        					if (r==1){
 		    							n_test_t_p++;
 		    						}else{
 		    							n_test_t_n++;
 		    						}
 		        		
 		        			
 		       					}
 		       					pre_flag[index_id]=0;
 		       					/*for (ttts=0; ttts<tc->nc; ttts++){
	       							fprintf(stderr, "flag class in iso: %d\t %d\n", tc->clas[ttts], class_iso[ttts]);
	       							
	       						}*/
 		       				}
 		       			}
 		       			if ((regular==1) | (reactive==1) ){
 		       				//start q2 hold only for original teach
 		       				
	  					if ((screenstatus[j]==6) & ((m_absolute==start_q2[j]) | (m_absolute==start_rq2[j]))){
	  					
	  						
	  						new_q2_reg++;
	  						if ((role_id!=6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
	  						
	  							
	  							
	  							new_q2_regs++;
	  							quarantine[j]=1;
	  							
	  							if ((preventive == 1) & 
	  							(num_pos_class[index_class(tc, data->classes[index_id])]>=3) &
	  							(class_iso[index_class(tc, data->classes[index_id])]==0)){
	  							
	  								
		 		    		  			 preisolation(data, tc, param, id, role_id, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
		 		    		  		}
	  							
	  						}else if ( (role_id==6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
	  						
	  							
	  							
	  							new_q2_regt++;
	  							quarantine_t[index_t(tc,id)]=1;
	  							status[j]=0;
	  						}else if ((m_absolute==start_rq2[j]) & (start_rq2[j]!=0) & (role_id==6) & (teach_rtested[index_t(tc, id)]>1)){
	  						
	  							new_q2_regr++;
	  							status[j]=0;
	  							
	  							
	  							
	  							
	  								
	  						}
	  						screenstatus[j]=-9;
	  						
	  					}else if ((m_absolute==start_q2[index_id]+param->delta_q/param->deltat) & (start_q2[j]!=0)){
 		       				 	
 		       				 	if (role_id!=6){
 		       				 		quarantine[index_id]=0;
 		       					}else if (role_id==6){
 		       					
 		       						r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);//, fp6);
 		       				 		
 		        					// Counter on the number of tests on the original teacher 
 		        					// before back to school
 		        					n_test_t++;
 		        							
 		        					if (r==1){
 		    							n_test_t_p++;
 		    						}else{
 		    							n_test_t_n++;
 		    							teach_neg_reg[n_reg]=id;
 		    							teach_neg_reg=(int*)realloc(teach_neg_reg,(n_reg+2)*sizeof(int));
 		    							n_reg++;
 		    						}
 		    						
 		    					}
 		       				 }
 		       			}
 		       			
 		    			 
 		    			// End of second isolation
 		    			if ((symptomatic==1) | ((symptomatic==1) & (preventive==1)) | (regular==1)){
 		    				if (role_id==6){
 		    					if ((teach_2iso[index_t(tc, id)]==1) 
 		    					& (m_absolute==teach_2isom[index_t(tc, id)]+param->delta_q/param->deltat)){
 		    					
 		    						status[index_id]=9;
 		    						teach_2iso[index_t(tc, id)]=-9;
 		    						quarantine_t[index_t(tc, id)]=0;
 		    						teach_rep[index_t(tc,id)]=-9;
 		    						
 		    						
 		    					}
 		    				}			
 		        		}

 		        		// INDIPENDENT TRANSITIONs:
 						// TRANSITION FROM E->IP 				
 						// TRANSITION FROM IP->IC_detected		
 						// TRANSITION FROM IP->IC
 						// TRANSITION FROM IP->ISC		
 						// TRANSITION FROM Q1->Q2
 						// TRANSITION FROM IC->R+ o ISC->R+ 	
 						// TRANSITION FORM ICDI-> R+		
 						// TRANSITION FORM R+ -> R
 			
 		    			if( (s_id==1) & (counter_latent[j] < param->shape_latent)){
 		    				random = gsl_ran_flat(RNG,0,1);
 		    				if(random < (1 - exp(-(param->shape_latent * param->eps) * param->deltat))){
 		    				
 		    					status[index_id]=1;  // 2 for prodromic phas
 		    					counter_latent[index_id]=counter_latent[index_id]+1;
 		    					
 		    					if (counter_latent[index_id] == param->shape_latent){
 		    						status[index_id]=2;
 		    						
 		    						new_pro++;
 		    						if (role_id!=6){
	 		    						new_pro_s++;
	 		    					}else if (role_id==6){
	 		    						if (teach_rep[index_t(tc,id)]==0){
	 		    							new_pro_t++;
	 		    						}else{
	 		    							new_pro_r++;
	 		    						}
	 		    					}
 		    						
 		    						
 		    					}
 		    				}
 		    			
 		    			} else if ((s_id==2) & (counter_prodro[j] < param->shape_prodro)){
 		    				random=gsl_ran_flat(RNG,0,1);
 		    				counter_latent[index_id]=0;
 		    				
	 		    			if (random< (1- exp(-(param->shape_prodro *param->mu_p)*param->deltat))){
	 		    				
 		    					status[index_id] = 2;
 		    					counter_prodro[index_id] = counter_prodro[index_id] +1;
 		    					
	 		    				
	 		    				if (counter_prodro[index_id] == param->shape_prodro){
	 		    					
	 		    					
	 		    					
	 		    					//counter_prodro[j]=0;
	 		    					random1=gsl_ran_flat(RNG,0,1);
	 		    					// 6 to identified teachers
	 		    					if ( ((role_id==6) & (random1<psub_tc->novax[nw]) & (vaccinated_2doses[index_id]==0) 
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==0))|
	 		    					 ((role_id==6) & (random1<psub_tc->twodoses[nw]) & (vaccinated_2doses[index_id]==0) 
	 		    					 & (vaccinated_booster[index_id]==0)& (r_delta[index_id]==1))|
	 		    					((role_id==6) & (random1<psub_tc->twodoses[nweek_2doses[index_id]]) & (vaccinated_2doses[index_id]==1)) |
	 		    					((role_id==6) & (random1<psub_tc->booster[nweek_booster[index_id]]) & (vaccinated_booster[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->novax[nw]) & (vaccinated_2doses[index_id]==0)
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==0)) |
	 		    					 ((role_id!=6) & (random1<psub_st->twodoses[nw]) & (vaccinated_2doses[index_id]==0)
	 		    					 & (vaccinated_booster[index_id]==0) & (r_delta[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->twodoses[nweek_2doses[index_id]]) & (vaccinated_2doses[index_id]==1)) |
	 		    					 ((role_id!=6) & (random1<psub_st->booster[nweek_booster[index_id]]) & (vaccinated_booster[index_id]==1))
	 		    					  ){
	 		    						
	 		    						//fprintf(stderr, "Inside subclinical\n");
	 		    						status[index_id]=5; 
	 		    						new_is++;
	 		    						if (role_id!=6){
	 		    							new_is_s++;
	 		    						}else if (role_id==6){
	 		    							if (teach_rep[index_t(tc,id)]==0){
	 		    								new_is_t++;
	 		    							}else{
	 		    								new_is_r++;
	 		    							}
	 		    						}
	 		    					//5 for subclinical case
	 		    					} else {
	 		    					
	 		    						
	 								random2=gsl_ran_flat(RNG,0,1);
	 								// Clinical detected
	 									
	 		    						if (((role_id==6) & (random2<pd[1])) | ((role_id!=6) & (random2<pd[0])) ){
	 		    					
	 		    							
	 		    							new_ic_d++;
	 		    							
	 		    							random3=gsl_ran_flat(RNG,0,1);
	 		    							if (role_id!=6){
	 		    								n_test_symp_st++;
	 		    							}else{
	 		    								n_test_symp_tc++;
	 		    							}
	 		    							n_test_symp++;
	 		    							//fprintf(stderr, "%d\t %f\n", counter_infect[index_id], theta_cs[counter_infect[index_id]]);
	 		    							if (((random3<theta_cs[0][counter_infect[index_id]]) & (data->classes[index_id]!=6)) | ((random3<theta_cs[1][counter_infect[index_id]]) & (data->classes[index_id]==6))){
	 		    							
	 		    								if (role_id!=6){
	 		    									fprintf(stderr, "%d\t %f\t %f\n", counter_infect[index_id], theta_cs[0][counter_infect[index_id]], theta_cs[0][0]);
	 		    								}else{
	 		    									fprintf(stderr, "%d\t %f\t %f\n", counter_infect[index_id], theta_cs[1][counter_infect[index_id]], theta_cs[0][0]);
	 		    								}
	 		    					
	 		    								n_test_symp_p++;
	 		    								result=1;
	 		    								if (role_id!=6){
	 		    									role=1;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]==0)){
	 		    									role=2;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]>=1)){
	 		    									role=3;
	 		    								}
	 		    								
	 		    								
	 		    								//IF stud/teach/repl_teach
	 		    								if(role_id!=6){ 
	 		    								
	 		    									q1_id[n_transitions1]=id;
	 		    									q1_quarter[n_transitions1]=m_absolute;
	 		    									q1_result[n_transitions1]=1;
	 		    									
	 		    									//fprintf(stderr, "n_transitions1:%d\n", n_transitions1);
	 		   									
	 		   									
	 		    									q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    									q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   									q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   						
	 		   									n_transitions1++;
	 		   									
	 		   								
	 		   									status[index_id]=4;
	 		   									detected[index_id]=1;
	 		   									
	 		   									new_ic_d_s++;
	 		   									
	 		   									// If identified during a preventive	
	 		   									if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    										pre_flag[index_id]=0;
	 		    									}
	 		    									
	 		    								}else if (role_id==6){
	 		    								
	 		    									if (teach_rep[index_t(tc, id)]==0){
	 		    									
	 		    										q1_id[n_transitions1]=id;
	 		    										q1_quarter[n_transitions1]=m_absolute;
	 		    										q1_result[n_transitions1]=1;
	 		    									
	 		    										q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    										q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   										q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   										
	 		   										
	 		   										n_transitions1++;
	 		   										
	 		    										detected_t[index_t(tc,id)]=1;
	 		   										teach_rep[index_t(tc,id)]=1;
	 		   										status[index_id]=0;
	 		   										
	 		   										new_ic_d_t++;
	 		    										
	 		    										status_orig=4;
	 		    										reason=1;
	 		    										
	 		    										
	 		    										// A teacher can be replaced because symp.
	 		    										
	 		    										// If identified during a preventive	
	 		   										if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    											pre_flag[index_id]=0;
	 		    										}
	 		    									}else if (teach_rep[index_t(tc, id)]>=1){
	 		    									
	 		    										
	 		    										status[index_id]=0;
	 		    										new_ic_d_r++;
	 		    									
	 		    										status_orig=4;
	 		    										reason=4;
	 		    								
	 		    										teach_rep[index_t(tc,id)]=2;
	 		    										
	 		   										if (preventive==1){
	 		   										
	 		   											q1_id[n_transitions1]=id;
	 		    											q1_quarter[n_transitions1]=m_absolute;
	 		   											q1_result[n_transitions1]=1;
	 		   										
	 		   											q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    											q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   											q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));	
	 		   											
	 		   											n_transitions1++;
	 		   										}
	 		    									}
	 		    										
	 		   								}
	 		   								
	 		    								
	 		    							
	 		    							// (that eventually triggers a protocol)
	 		    							
	 		    								
	 		    							} else {
	 		    							
	 		    								n_test_symp_n++;
	 		    								/*result=0;
	 		    								if (role_id!=6){
	 		    									role=1;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]==0)){
	 		    									role=2;
	 		    								}else if ((role_id==6) && (teach_rep[index_t(tc, id)]>=1)){
	 		    									role=3;
	 		    								}
	 		    								*/
	 		    								
	 		    								if(role_id!=6){
	 		    								 												q1_id[n_transitions1]=id;
	 		    									q1_quarter[n_transitions1]=m_absolute;
	 		    									q1_result[n_transitions1]=0;
	 		    									
	 		    									q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    									q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   									q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   									
	 		   									n_transitions1++;
	 		   									
	 		   									status[index_id]=4;
	 		   									detected[index_id]=1;
	 		   									new_ic_d_s++;
	 		   										
	 		   									if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    										pre_flag[index_id]=0;
	 		    									}
	 		    								}else if (role_id==6){
	 		   									
	 		   									if (teach_rep[index_t(tc, id)]==0){
	 		   									
	 		   										q1_id[n_transitions1]=id;
	 		    										q1_quarter[n_transitions1]=m_absolute;
	 		    										q1_result[n_transitions1]=0;
	 		    									
	 		    										q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    										q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   										q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));
	 		   										
	 		   										n_transitions1++;
	 		   									
	 		   										detected_t[index_t(tc,id)]=1;
	 		   										status[index_id]=0;
	 		   										
	 		   										new_ic_d_t++;
	 		    										
	 		    										status_orig=4;
	 		    										reason=1;
	 		    										
	 		    										
	 		    										
	 		    								
	 		    										teach_rep[index_t(tc,id)]=1;
	 		    										
	 		    										if ((preventive==1) & (pre_flag[index_id]==1)){
	 		    											pre_flag[index_id]=0;
	 		    										}
	 		   									}else if (teach_rep[index_t(tc, id)]>=1){
	 		    									
	 		    										status[index_id]=0;
	 		    										new_ic_d_r++;
	 		    									
	 		    										status_orig=4;
	 		    										reason=4;
	 		    								
	 		    										teach_rep[index_t(tc,id)]=2;
	 		    										
	 		   										if (preventive==1){
	 		   										
	 		   											q1_id[n_transitions1]=id;
	 		    											q1_quarter[n_transitions1]=m_absolute;
	 		   											q1_result[n_transitions1]=0;
	 		   										
	 		   											q1_id=(int*)realloc(q1_id,(n_transitions1+2)*sizeof(int));
	 		    											q1_quarter=(int*)realloc(q1_quarter,(n_transitions1+2)*sizeof(int));
	 		   											q1_result=(int*)realloc(q1_result,(n_transitions1+2)*sizeof(int));			
	 		   											
	 		   											
	 		   											n_transitions1++;
	 		   										}
	 		   									}
	 		   								}
	 		    							
	 		    							}
	 		    						// Clinical not detected 
	 		    						}else{
	 		    						
	 		    							
	 		    							
	 		    							status[index_id]=4;
	 		    							new_ic++;
	 		    							
	 		    							if (role_id!=6){
	 		    								new_ic_s++;
	 		    								detected[index_id]=0;
	 		    							}else if (role_id==6){
	 		    								if (teach_rep[index_t(tc,id)]==0){
	 		    									new_ic_t++;
	 		    									detected_t[index_t(tc,id)]=0;
	 		    								}else{
	 		    									new_ic_r++;
	 		    								}
	 		    							}
	 		    						}   
	 							}			
	 					 	}
	 					 }
 					 	//fprintf(stderr, "end prodromic");
 		    			} else if(((s_id==4) & (counter_infect[j]<param->shape_infect)) | 
 		    			((s_id==5) & (counter_infect[j]<param->shape_infect))){	
 		    		        	random=gsl_ran_flat(RNG,0,1);	
 		    		        	counter_prodro[index_id]=0;
	    					if(random< 1 - exp(-(param->shape_infect * param->mu)*param->deltat)){
	    						if (s_id==4){
	    							status[index_id]=4;
	    						}else if (s_id==5){
	    							status[index_id]=5;
	    						}
	    						counter_infect[j]=counter_infect[j]+1;
	    						if (counter_infect[j] == param->shape_infect){
	    							status[index_id]=8;          // 8 for recovery but still positive to the te
	    							//counter_infect[j] = 0;
	    						}
	    					}	

 		    			} else if ((s_id==8) & (counter_rp[index_id]<param->shape_rp)){
 		    				random4=gsl_ran_flat(RNG,0.0,1.0);
 		    				counter_infect[j] = 0;
 		    				if (role_id!=6){	
 		    					if(random4 < (1 - exp(-(param->shape_rp * param->mu_rp)*param->deltat))){
 		    						counter_rp[index_id]=counter_rp[index_id]+1;
 		    						if (counter_rp[index_id]==param->shape_rp){
 		    							status[index_id]=9;
 		    							counter_rp[index_id] = 0;
 		    						}
 		    					}
 		    				}else if(role_id==6){
 		    				
 		    					if (symptomatic==1){
 		    						for(hh=0; hh<n_transitions2; hh++){
 		    							if((id==q2_id[hh]) & (random4<1 - exp(-(param->shape_rp * param->mu_rp1)*param->deltat))){
 		    								counter_rp[index_id]=counter_rp[index_id]+1;
		 		    						if (counter_rp[index_id]==param->shape_rp){
		 		    							status[index_id]=9;
		 		    							counter_rp[index_id] = 0;
		 		    						}
		 		    								
 		    							}else if ((id!=q2_id[hh]) & (random4<1 - exp(-(param->shape_rp * param->mu_rp)*param->deltat))){
 		    								counter_rp[index_id]=counter_rp[index_id]+1;
		 		    						if (counter_rp[index_id]==param->shape_rp){
		 		    							status[index_id]=9;
		 		    							counter_rp[index_id] = 0;
		 		    						}
 		    							}
 		    						}
 		    					}
 		    					
 		    					
 		    				}
 		    			}    	
 		    		}	
 		
 				
 		
 					save_status(data,status,status0); 
	  	
	  			}
	  	
	  	
	  			
	  			fprintf(fp5, "%d\t %d\t %d\t %d\t %d\t %d\n", nr, day_simulation, new_exposed, new_exp_s, new_exp_t, new_exp_r);
	  			fflush(fp5);
	  				
	  			
	  			//fprintf(stderr, "intro w st %f\t transmis %d\n", (*introst)[nr].weeks[nw], weekly_transmission_st);
	  			//fprintf(stderr, "intro w tc %f\t transmis %d\n", (*introtc)[nr].weeks[nw], weekly_transmission_tc);

				
	  			
	  			for (j=0; j<data->n; j++){
	  				if(nweek_2doses[j]!=100){
	  					nweek_2doses[j]= nweek_2doses[j]+1;
	  				}
	  				
	  				if(nweek_booster[j]!=100){
	  					nweek_booster[j]= nweek_booster[j]+1;
	  				}
	  			}

				for(vc=0; vc<data->n; vc++){
					if((data->classes[vc]!=6) & (vaccinated_2doses[vc]==1)){
						nweek_2doses[vc] = nweek_2doses[vc] +1 ;
					}
				} 
	  			
	  	        nw = nw + 1;
	  	                
				fprintf(fp21, "%d\t",  weekly_transmission_st);
				if (nw==6)
					fprintf(fp21, "\n");

			    fprintf(fp24, "%d\t",  weekly_transmission_tc);
				if (nw==6)
					fprintf(fp24, "\n");
    
	  	        weekly_transmission_st=0;
			    weekly_transmission_tc=0;
	  	        //exit(0); 

				  
      			break;
    		
		}
    }
    
    int y;
    for (y=0; y<param->nweeks; y++){
    	fprintf(fp22, "%f\t",  round((*introst)[nr].weeks[y]));
    }
    fprintf(fp22, "\n");

    for (y=0; y<param->nweeks; y++){
    	fprintf(fp23, "%f\t",  round((*introtc)[nr].weeks[y]));
    }
    fprintf(fp23, "\n");
    
    //int l;
    for (l=0; l<data->n; l++){
    	pre_flag[l]=0;
    	pre_time[0][l]=0;
    	pre_time[1][l]=0;
    	exp_day[l]=0;
    	status[l]=0;
    	counter_latent[l]=0;
    	counter_prodro[l]=0;
    	counter_infect[l]=0;
    	counter_rp[l]=0;
    	//comp_id[l]=0;
    	//day_lost[l]=0;
    	
    }
    
    //fprintf(fp5, "%d\t %d\n", nr, n_smp_d);
    
    //free(q1_day);
    //free(q2_day);
    //free(tested_negative_day);
   
    free(q1_quarter);
    free(q1_result);
    free(q2_quarter);
    //free(tested_negative_quarter);
	
    free(q1_id);
    free(q2_id);
    //free(negative);	
    
    free(teach_2iso);
    free(teach_2isoday);
    free(teach_2isom);
    
    
    
    free(detected);
    free(quarantine);
    
    free(detected_t);
    free(quarantine_t);
    free(teach_rep);  
    free(start_q2);
    free(start_rq2);
    free(screenstatus);
   
    free(class_iso);
    free(teach_rtested);
    free(teach_neg_reg);
    free(num_iso);   
    //free(exp_day);  
    
}

void read_adhesion(char *file, Param *param, float *v){
  FILE *fp;
  int i;

  if((fp=fopen(file,"r"))==NULL){
    fprintf(stderr,"Error opening file %s for reading\n",file);
    exit(1);  
  } 
  
  for(i=0; i<param->nweeks; i++)
    fscanf(fp, "%f", &v[i]);

  fclose(fp);
}

void read_studentstovax(char *file, Param *param, int *v){
  FILE *fp;
  int i;

  if((fp=fopen(file,"r"))==NULL){
    fprintf(stderr,"Error opening file %s for reading\n",file);
    exit(1);  
  } 
  
  for(i=0; i<param->nweeks; i++)
    fscanf(fp, "%d", &v[i]);

  fclose(fp);
}

void read_teacherstoboost(char *file, Param *param, int *v){
  FILE *fp;
  int i;

  if((fp=fopen(file,"r"))==NULL){
    fprintf(stderr,"Error opening file %s for reading\n",file);
    exit(1);  
  } 
  
  for(i=0; i<param->nweeks; i++)
    fscanf(fp, "%d", &v[i]);

  fclose(fp);
}

void read_saliva_sensitivity(char *file, Param *param) {

  FILE *fp;
  char str[20];
  
  if((fp=fopen(file,"r"))==NULL){
    fprintf(stderr,"Error opening file %s for reading\n", file);
    exit(1);  
  } 
  
  fscanf(fp, "%s", str);
  //param->N_adm=atoi(str);

  fclose(fp);   

}

void read_nasal_sensitivity(char *file, Param *param) {

  FILE *fp;
  char str[20];
  
  if((fp=fopen(file,"r"))==NULL){
    fprintf(stderr,"Error opening file %s for reading\n", file);
    exit(1);  
  } 
  
  fscanf(fp, "%s", str);
  //param->N_adm=atoi(str);

  fclose(fp);   

}

#ifdef BLOCKSIZE
#undef BLOCKSIZE
#endif
#define BLOCKSIZE 500

int get_line(char **line,FILE *fp)
/*
  Reads a line from the stream pointer fp

  Return values:
  3 non empty line ending in newline
  2 non empty line not ending in newline
  1 empty line not ending in EOF
  0 empty line ending in EOF
  -1 error
*/

{
  int c;
  int pos=0;
  int nblocks=1;
  int maxsize=BLOCKSIZE+1;

  (*line)=calloc(maxsize,sizeof(char));

  while((c=fgetc(fp)) != EOF && c != '\n'){
    if(pos==maxsize-1){
      maxsize=(++nblocks)*BLOCKSIZE+1;
      if(!((*line)=(char *)realloc(*line,maxsize*sizeof(char)))){
	fprintf(stderr,"get_line: out of memory\n");
	return -1;
      }
    }
    (*line)[pos++]=c;
  }
  if(c=='\n'){
    (*line)[pos]='\0';
    if(strlen(*line)>0)
      return 3;
    else
      return 1;
  }else if(c==EOF){
    (*line)[pos]='\0';
    if(strlen(*line)>0){
      return 2;
    }
    else
      return 0;
  }

  fprintf(stderr,"get_line: WARniNG: unusual exit status\n");
  return -1;
}

#undef BLOCKSIZE



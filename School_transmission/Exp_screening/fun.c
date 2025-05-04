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


void assign_compliance(metadata *data, Teach *tc, int *comp_id, float percentage, int regular, int regular_classes, float adhesion_classes){

	
	float p;
	p=percentage;
	
	
	
	// p percentage of compliance to regular screening o quenched
	if ((regular==1) & (regular_classes==0)){
		int i, j, nodes_compliant;
		int *nodes_ids, *nodes_idx, n, m;
		
		nodes_ids=(int*)calloc(1,sizeof(int));
		nodes_idx=(int*)calloc(1,sizeof(int));
		
		
			
		//Initialization
		m=0;
		for (i=0; i<data->n; i++){
			comp_id[i]=0;
			nodes_ids[m]=data->ID[i];
			nodes_idx[m]=i;
			nodes_ids=(int*)realloc(nodes_ids,(m+2)*sizeof(int));
			nodes_idx=(int*)realloc(nodes_idx,(m+2)*sizeof(int));
			//fprintf(stderr, "%d\t i%d\t id %d\n ", m, i, data->ID[i]);
			m++;
		}
		
		nodes_compliant=data->n*p;
		
		myshuffle(nodes_idx, data->n);
		
		
		n=0;
		for (j=0; j<nodes_compliant; j++){
			
			comp_id[index_nodes(data, nodes_ids[nodes_idx[j]])]=1;
			n++;
		}
		
		if (n!=nodes_compliant){
			fprintf(stderr, "ERROR IN ASSIGN COMPLIANCE TO ALL PARTICIPANTS\n");
			exit(0);
		}else{
			fprintf(stderr, " ASSIGN COMPLIANCE TO ALL PARTICIPANTS %d\t %d\n", n, nodes_compliant);
		}
		
		
		free(nodes_ids);
		free(nodes_idx);
		
		
	}else if((regular==1) & (regular_classes==1)){
	
		int j, c, h, t, z, cl;
		int *originalclasses, nclasses, *originalindexes;
		nclasses = percentage * (tc -> nc);
		
		
		originalclasses=(int*)calloc(tc->nc,sizeof(int));
		originalindexes=(int*)calloc(data->n,sizeof(int));
		
		int dim=0;
		int *selected_classes;
		selected_classes=(int*)calloc(nclasses,sizeof(int));
		
		// Initialize comp_id
		for (z=0; z<data->n; z++){
			comp_id[z]=0;
		}
		
		for(h=0; h<data->n; h++){
			originalindexes[h]=h;
		} 
		myshuffle(originalindexes, data->n);
		
		for (t=0; t<tc->nc; t++){
			originalclasses[t]=tc->clas[t];
			
		}
		
		myshuffle(originalclasses, tc->nc);
		
		for (c=0; c<nclasses; c++){
				fprintf(stderr, "SELECTED CLASSES: %d\n", tc->clas[originalclasses[c]]);
				if (adhesion_classes!=1){
					selected_classes[c]=tc->clas[originalclasses[c]];
				}
		}
			
		
		if (adhesion_classes==1){
		
			// Select all the id the choseen class and make them compliant
			for(j=0; j<data->n; j++){
				for (c=0; c<nclasses; c++){
					if ((data->classes[j]!=6) & (data->classes[j]==tc->clas[originalclasses[c]])){
					
						comp_id[j]=1;
						dim++;
						
					}else if((data->classes[j]==6) & (tc->clas[index_t(tc,data->ID[j])]==tc->clas[originalclasses[c]])){
					
						comp_id[j]=1;
						dim++;
					}
				}
			}
			
			//CHECK
			int l, mm=0, mn=0;
			for (l=0; l<data->n; l++){
				if (comp_id[l]==0){
					mm++;
				}else{
					mn++;
				}
			}
			
			fprintf(stderr, "0 adhesion: %d\t 1 adhesion: %d\n", mm, mn);
			
			if(mm!=(data->n)-(dim)){
				fprintf(stderr, "EXIT, error in adhesion assignment\n");
				fprintf(stderr, "0 adhesion: %d\t 1 adhesion:%d\n", mm, mn);
				exit(0);
			}
		
		}else{
			int *saveids, *saveid_index, nn;
			saveids=(int*)calloc(1,sizeof(int));
			saveid_index=(int*)calloc(1,sizeof(int));
			
			int jj, n_adh, nn_tot=0, n_compl=0;
			
			
			for (cl=0; cl<nclasses; cl++){
				nn=0;
				n_compl=0;
				for(j=0; j<data->n; j++){
					if (((data->classes[j]!=6) && (data->classes[j]==selected_classes[cl])) | 
					((data->classes[j]==6) && (tc->clas[index_t(tc, data->ID[j])]==selected_classes[cl]))){
				
						saveids[nn]=data->ID[j];
						saveid_index[nn]=j;
						saveids=(int*)realloc(saveids,(nn+2)*sizeof(int));
						saveid_index=(int*)realloc(saveid_index,(nn+2)*sizeof(int));
						nn++;
						
						
					}
				}
				
				myshuffle(saveid_index, nn);
				n_adh=nn*adhesion_classes;
				
				for (jj=0; jj<n_adh; jj++){
		
					comp_id[index_nodes(data, saveids[saveid_index[jj]])]=1;
					n_compl++;
					nn_tot++;
				}
				fprintf(stderr, "class: %d\t dim classes: %d\t numb adh: %d\t %d\n", selected_classes[cl], nn, n_adh, n_compl);
				if (n_adh!=n_compl){
					fprintf(stderr, "ERROR in assign adhesion within the class\n");
					exit(0);
				}
			}
			
			
			//CHECK
			int l, mm=0, mn=0;
			for (l=0; l<data->n; l++){
				if (comp_id[l]==0){
					mm++;
				}else{
					mn++;
				}
			}
			
			fprintf(stderr, "0 adhesion: %d\t 1 adhesion: %d\n", mm, mn);
			
			if(mm!=(data->n)-nn_tot){
				fprintf(stderr, "EXIT, error in adhesion assignment\n");
				fprintf(stderr, "0 adhesion: %d\t 1 adhesion:%d\n", mm, mn);
				exit(0);
			}
			
			
		}
	
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



//test_teach(tc, param, id, index_id, n_q1, teach_iso, teach_2iso, pre_flag, teach_2isoday, teach_2isom, nr, day_simulation, m_absolute, status0, status, fp6);

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
					//fprintf(fp7, "%d\t %d\t %d\t %d\t %d\t %d\t %d\n", nr, id1,id2,s_id1, s_id2, exp_day[index_id1], day_simulation); 
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
						//fprintf(fp7, "%d\t %d\t %d\t %d\t %d\t %d\t %d\n", nr, id1,id2,s_id1, s_id2, exp_day[index_id1], day_simulation); 
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
						//fprintf(fp7, "%d\t %d\t %d\t %d\t %d\t %d\t %d\n", nr, id1,id2,s_id1, s_id2, exp_day[index_id1], day_simulation); 
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
						//fprintf(fp7, "%d\t %d\t %d\t %d\t %d\t %d\t %d\n", nr, id1,id2,s_id1, s_id2, exp_day[index_id1], day_simulation); 
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
  		
  	}	
  	
  	
  	m=0;
  	for (i=0; i<n; i++){
  		for(j=0; j<i; j++){
  			if ((v1[i]==v1[j]) & (v2[i]==v2[j])){
  				m++;
  			}
  		}
  	}
  	
  	
  	return m;

}






void regular_screening(metadata *data, Param *param, Teach *tc, int *status0, int *teach_rep, int *teach_rtested, int *pre_flag, int *quarantine, int *detected, int *screenstatus, int *comp_id, int *start_q2, int *start_rq2, int quarter, int day, int sim, float **theta_ps, float **theta_cs, float **theta_scs, float **theta_rps, int *counter_prodro, int *counter_infect, int *counter_rp, int *num_pos_class, FILE *fp12, FILE *fp13){

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
						fprintf(fp13, "%d\t %d\t %d\t %d\t %d\n", sim, day, data->ID[i], role, s0);
						
						  
						  
						
					}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]==0)){
						N_positive_t++;
						start_q2[i]=quarter+param->tau_test/param->deltat;
						teach_rtested[index_t(tc, data->ID[i])]=1;
						
						role=2;
						fprintf(fp13, "%d\t %d\t %d\t %d\t %d\n", sim, day, data->ID[i], role, s0);
						
						  
						  
						
						
					// If the teacher has been already replace but also the treplacement is positive
					}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]>=1)){
						N_positive_r++;
						start_rq2[i]=quarter+param->tau_test/param->deltat;
						teach_rtested[index_t(tc, data->ID[i])]=2;
						role=3;
						fprintf(fp13, "%d\t %d\t %d\t %d\t %d\n", sim, day, data->ID[i], role, s0);
						
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
						fprintf(fp13, "%d\t %d\t %d\t %d\t %d\n", sim, day, data->ID[i], role, s0);
						
						  
						  
						
					}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]==0)){
						N_positive_t++;
						start_q2[i]=quarter+param->tau_test/param->deltat;
						teach_rtested[index_t(tc, data->ID[i])]=1;
						
						role=2;
						fprintf(fp13, "%d\t %d\t %d\t %d\t %d\n", sim, day, data->ID[i], role, s0);
						
						  
						  
						
					// If the teacher has been already replace but also the treplacement is positive
					}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]>=1)){
						N_positive_r++;
						start_rq2[i]=quarter+param->tau_test/param->deltat;
						teach_rtested[index_t(tc, data->ID[i])]=2;
						
						role=3;
						fprintf(fp13, "%d\t %d\t %d\t %d\t %d\n", sim, day, data->ID[i], role, s0);
						
						  
						
						  
						  
						  
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
						fprintf(fp13, "%d\t %d\t %d\t %d\t %d\n", sim, day, data->ID[i], role, s0);
						
						  
						  
						
						
						
					}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]==0)){
						N_positive_t++;
						start_q2[i]=quarter+param->tau_test/param->deltat;
						teach_rtested[index_t(tc, data->ID[i])]=1;
						
						role=2;
						fprintf(fp13, "%d\t %d\t %d\t %d\t %d\n", sim, day, data->ID[i], role, s0);
						
						  
						  
						
						
					// If the teacher has been already replace but also the treplacement is positive
					}else if ((data->classes[i]==6) & (teach_rep[index_t(tc, data->ID[i])]>=1)){
						N_positive_r++;
						start_rq2[i]=quarter+param->tau_test/param->deltat;
						teach_rtested[index_t(tc, data->ID[i])]=2;
						role=3;
						fprintf(fp13, "%d\t %d\t %d\t %d\t %d\n", sim, day, data->ID[i], role, s0);
						
						  
						
						  
						  
						  
						
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
	
	
	
	fprintf(fp12, "%d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\n", sim, day, N_tot, N_positive, N_positive_s, N_positive_t, N_positive_r, N_negative, N_negative_s, N_negative_t, N_negative_r);
	
	
	
}

void count_positive_st(metadata *data, int sim, int day_simulation, int *counter_prodro, int *counter_infect, int *counter_rp, int *status0, int *detected, int *quarantine, int *pre_flag, int *num_iso, FILE *fp20){
	
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
	
	
	fprintf(fp20, "%d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\n", sim, day_simulation, jk1, N_ips11, N_ips12, N_ips13, N_ips14, N_ics11, N_ics12, N_ics13, N_ics14, N_ics15, N_ics16, N_iscs11, N_iscs12, N_iscs13, N_iscs14, N_iscs15, N_iscs16, N_rps11, N_rps12, N_rps13, N_rps14);

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
		
	fprintf(stderr, "number of NOT compliant nodes: %f\n", nodes_notcompliant);
	

	for (i=0; i<data->n; i++){
		if ((data->classes[i]!=6) & (pre_flag[i]==0) & (detected[i]==0) & (quarantine[i]==0) & (num_iso[i]==0)){	
			ids_s[k_s]=data->ID[i];
			//fprintf(stderr, "%d\t %d\n", ids[k], data->ID[i]);
			ind_s[k_s]=k_s;
			k_s++;
		}
		
	}
	
	
	myshuffle(ind_s, (data->n)-(tc->nc)-absentes);
	
	fprintf(stderr, "c %f\n", (data->n) - (tc->nc) - (nodes_notcompliant) - absentes);
	
	for (j=0; j< round((data->n) - (tc->nc) - (nodes_notcompliant) - absentes); j++){
		
		comp_id[index_nodes(data, ids_s[ind_s[j]])]=1;
		
		compliant++;
		if (data->classes[index_nodes(data, ids_s[ind_s[j]])]==6){
			fprintf(stderr, "ERROR IN COMPLiANCE STUDENTS\n");
			exit(0);
		}
		
		
	}
	free(ids_s);
	free(ind_s);
	
	

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



void tseir(metadata *data, contact *ct, Teach *tc, Param *param, Weeklyintro **introst,  Weeklyintro1 **introtc, Susst *sigma_st, Sustc *sigma_tc, Trast *rbeta_st, Tratc *rbeta_tc, Subst *psub_st, Subtc *psub_tc, int symptomatic, int preventive, int preventivegrade, int regular, int reactive, int **pre_time, int *pre_flag, int *exp_day, int *status0, int *status, int *comp_id, int *start, int *end, float *pd, int nr, int *students_tovax, int *teachers_toboost, int*vaccinated_2doses, int*vaccinated_booster, int index_seed, float **theta_ps, float **theta_cs, float **theta_scs, float **theta_rps, float *theta_p, float *theta_c, float *theta_sc, float *theta_rp, int *counter_latent, int *counter_prodro, int *counter_infect, int *counter_rp,  float *weekly_perc_adhesion, int ntimes, int nweek, int *r_delta, int *nweek_2doses, int *nweek_booster, FILE *fp12, FILE *fp13, FILE *fp20, FILE *fp21, FILE *fp22, FILE *fp23, FILE *fp24){


	
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
	
	// Number of nodes by compartiments	
	
	int N_q1, N_q2, N_q3;
	int N_s1, N_e1, N_ip1, N_ic1, N_isc1, N_rp1, N_r1, N_rd;
	int N_ss1, N_es1, N_ips1, N_ics1, N_iscs1, N_rps1, N_rs1;
	int N_st1, N_et1, N_ipt1, N_ict1, N_isct1, N_rpt1, N_rt1;
	int N_s, N_e, N_ip, N_ic, N_isc, N_rp, N_r; 
	int N_st, N_et, N_ipt, N_ict, N_isct, N_rpt, N_rt; // among teachers
	int N_ips11, N_ips12, N_ips13, N_ips14, N_ics11, N_ics12, N_ics13, N_ics14, N_ics15, N_ics16, N_iscs11, N_iscs12, N_iscs13, N_iscs14, N_iscs15, N_iscs16; // among studnets
	int N_ipt11, N_ipt12, N_ipt13, N_ipt14, N_ict11, N_ict12, N_ict13, N_ict14, N_ict15, N_ict16, N_isct11, N_isct12, N_isct13, N_isct14, N_isct15, N_isct16;// among teachers
	
	int n_susc_s,  n_susc_t;
	int N_q1_t, N_q2_t, N_q3_t, N_q1_s, N_q2_s, N_q3_s;
	int new_exposed, new_pro, new_ic, new_is, new_q3, new_ic_d;
	int new_exp_s, new_exp_t, new_exp_r, new_pro_s, new_pro_t, new_pro_r, new_ic_s, new_ic_t, new_ic_r;
	int new_ic_d_s, new_ic_d_t, new_ic_d_r, new_is_s, new_is_t, new_is_r, new_q2_s, new_q2_t, new_pos_r;
	int new_neg_s, new_neg_t, new_neg_r, new_q2_reg, new_q2_regs, new_q2_regt, new_q2_regr;
	

	
	double random, random1, random2, random3;
	long double random4; 

	// Weekly intro
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
	// Q2 detected clinical infectious node in isolation 
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
	int nw=0;	

   	for (day_simulation=0; day_simulation<param->tf; day_simulation++){
	
	    d=day_simulation % week + 1; 		
							
			// Monday, Tuesday , Thursday, Friday <-school --> beta 
			// Saturday and Sunday <-no school 
			// + weekly introductions
			
	    
          					
            switch(d){
    		case 1:	

    			weekly_transmission_st = 0;
				weekly_transmission_tc = 0;
    			
    			new_exposed=0; new_pro=0; new_ic_d=0; new_ic=0; new_is=0; new_q3=0;new_q2_reg=0; n_test_symp=0;n_test_symp_p=0; n_test_symp_n=0; n_test_t=0; n_test_t_p=0;
      			n_test_t_n=0;
    		
    			// New cases in each compartments divided by role: STUDENTS/ TEACHERS/ REPLACED TEACHERS
    			new_exp_s=0; new_exp_t=0; new_exp_r=0; 
				new_pro_s=0; new_pro_t=0; new_pro_r=0; 
				new_ic_s=0; new_ic_t=0; new_ic_r=0;
    			new_ic_d_s=0;new_ic_d_t=0; new_ic_d_r=0;
    			new_is_s=0; new_is_t=0; new_is_r=0;
    		    new_q2_s=0; new_q2_t=0; new_pos_r=0;
    			
    			new_q2_regs=0;new_q2_regt=0;new_q2_regr=0;
    			new_neg_s=0;new_neg_t=0;new_neg_r=0;
				n_test_symp_st=0; n_test_symp_tc=0;
      			
    			//Assign_weekly_vaccinaion_coverage
    			assign_vaccination(data, tc, nw, students_tovax[nw], teachers_toboost[nw], vaccinated_2doses,  vaccinated_booster, nweek_2doses, nweek_booster);
    			
    			// Each Monday, I choose when add new cases from community
  				
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
  			
	  			
    			for (m=0; m<num_15min_day; m++){
    			
    				m_absolute=num_15min_day*day_simulation + m; //progression of 15 minutes step from the first day to the last one
	        
	        		N_s=0; N_e=0;N_ip=0;N_ic=0;N_isc=0;N_rp=0;N_r=0;N_q1=0;N_q2=0;N_q3=0;
					N_st=0;N_et=0;N_ipt=0;N_ict=0;N_isct=0;N_rpt=0;N_rt=0; N_q1_t=0;N_q2_t=0;N_q3_t=0;N_q1_s=0;N_q2_s=0;N_q3_s=0;
					
					//Prevalence by sub-compartments and role
					N_ss1=0;N_es1=0;N_ips1=0;N_ics1=0;N_iscs1=0;N_rps1=0;N_rs1=0;
					N_st1=0;N_et1=0;N_ipt1=0;N_ict1=0;N_isct1=0;N_rpt1=0;N_rt1=0;
					N_ips11=0;N_ips12=0;N_ips13=0;N_ips14=0;N_ics11=0;N_ics12=0;N_ics13=0;N_ics14=0;N_ics15=0;N_ics16=0;
					N_iscs11=0;N_iscs12=0;N_iscs13=0;N_iscs14=0;N_iscs15=0;N_iscs16=0;
					N_ipt11=0;N_ipt12=0;N_ipt13=0;N_ipt14=0;N_ict11=0;N_ict12=0;N_ict13=0;N_ict14=0;N_ict15=0;N_ict16=0;
					N_isct11=0;N_isct12=0;N_isct13=0;N_isct14=0;N_isct15=0;N_isct16=0;

					N_rd=0;

      				
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
				
						if ((ntimes==1) &  (m==start[day_simulation]) & (nweek==1)){
						
							count_positive_st(data, nr, day_simulation, counter_prodro, counter_infect, counter_rp, status0, detected, quarantine, pre_flag, num_iso, fp20);

						}

						if (((((ntimes==1) | (ntimes==2)) & (m==start[day_simulation]) & (nweek==1)) 
							|
							((ntimes==1) & (m==start[day_simulation]) & (nweek==2) & (day_simulation==day_screen)) |
							((ntimes==4) &  (m==start[day_simulation]) & (nweek==1))) & (regular==1) ){
							
							assign_compliance_st(data, tc, comp_id, quarantine, detected, num_iso, pre_flag, weekly_perc_adhesion[nw], regular);
							regular_screening(data, param, tc, status0, teach_rep, teach_rtested, pre_flag, quarantine, detected, screenstatus, comp_id, start_q2, start_rq2, m_absolute, day_simulation, nr, theta_ps, theta_cs, theta_scs, theta_rps, counter_prodro, counter_infect, counter_rp, num_pos_class, fp12, fp13);
								
						}
	  		    			
						for (i=0; i<ct->n1; i++){
 	  	
 		  	  	  			if ((ct->step[i]==m) & (ct->days[i]==d1)){
 		  	  	  
 		  	  	  			
								// Extract individual IDs from contact
 		        				id1=ct->ID1[i];
 		        				id2=ct->ID2[i];
 		        		
								// Map IDs to internal indices
 		        				index_id1=index_nodes(data, id1); 
 		        				index_id2=index_nodes(data, id2); 
 		        
								// Retrieve current health/status values
  		        				s_id1=status0[index_id1];
 		        				s_id2=status0[index_id2];

								// Retrieve roles/classes (e.g., student, teacher)
 		        				role_id1=data->classes[index_id1];
 		        				role_id2=data->classes[index_id2];
 		        		
								// Get contact weight
 		        				weight=ct->weight[i];

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
 		        				 & ((pre_flag[index_id2]==0) & (detected[index_id2]==0) & (quarantine[index_id2]==0)))){
 		        						
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
 		    		
 		    			
 		    			// From Q1 to Q2
 		    			// n_transitions1 = number of suspected nodes tested 
 		    			if ((symptomatic==1) & (n_transitions1>0) & (n_transitions2>=0)){
 		    		
 		    			
						// n_transitions1: Represents the number of symptomatic cases detected.
						// Case 1: If only symptom. testing is active (symptomatic == 1):
						//         - n_transitions1 includes cases detected among students and the original teacher.
						// Case 2: If both symptoms (symptomatic == 1) and preventive measures (preventive == 1) are active:
						//         - n_transitions1 includes cases detected among students, the original teacher, and the replacement teacher.
						
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
 		    							
 		    								  
 		    								  
 		    								
 		    								
 		    								if ((preventive==1) & (regular==0)){
 		    								
 		    									class_q3=data->classes[index_id];
 		    									teach_q3=find(tc, id, class_q3);
 		    								
 		    									// If the class in not already in Q3
 		    				 					if (class_iso[index_t(tc, teach_q3)]==0){
 		    							
 		    										// Turn on if the class is in pre iso
 		    										class_iso[index_t(tc, teach_q3)]=1;
 		    										
 		    										dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    										
 		    										/*if (preventivegrade==1){
 		    										    mod=class_q3%10;
 		    										    for (ttc=0; ttc< tc->nc; ttc++){
 		    										      if(tc->clas[ttc]%10==mod){ 													class_iso[ttc]=1;		
 		    										       }
 		    										     }
 		    										}*/
 		    										
 		    										// SAVE when preventive is and from whom the prev
 		    										// is trigger
 		    										// trigger_role
 		    										// 1:students, 2: teacher, 3: replaced teacher
 		    										trigger_role=1;
 		    									
 		    										 
 		    							
 		    										new_q3=new_q3+dim;
 		    							
 		    									  
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
 		    									  
 		    				
 		    									q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    									q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    									  
 		    									
 		    									  
 		    									
 		    									n_transitions2++;
 		    							
 		    									  
 		    									  
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
 		    						
 		    							r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);
 		    							
 		    							
 		    							fprintf(stderr, "q2 id:%d\t start Q2: %d\t end Q2: %d\t (day:%d)\t role: %d\t result:%d\n", id, q2_quarter[hh], m_absolute, day_simulation, role_id, r);
 		    						
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
 		       					
 		        					r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);
 		        					
 		        					n_test_t++;
 		        							
 		        					if (r==1){
 		    							n_test_t_p++;
 		    						}else{
 		    							n_test_t_n++;
 		    						}
 		        		
 		        			
 		       					}
 		       					pre_flag[index_id]=0;
 		       						
	       						
 		       						
 		       				}
 		       			}
 		       			if ((regular==1) | (reactive==1) ){
 		       				  
 		       				
							if ((screenstatus[j]==6) & ((m_absolute==start_q2[j]) | (m_absolute==start_rq2[j]))){
							
								
								new_q2_reg++;
								if ((role_id!=6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
								
									fprintf(stderr, "Quarantine - student id %d\t start %d\n", id, day_simulation);
									
									new_q2_regs++;
									quarantine[j]=1;
									
									if ((preventive == 1) & 
									(num_pos_class[index_class(tc, data->classes[index_id])]>=3) &
									(class_iso[index_class(tc, data->classes[index_id])]==0)){
									
										fprintf(stderr, "\n PREISO %d\n", day_simulation);
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
 		       					
 		       						r= test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);
 		       				 		
 		        					
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
	 		   								
	 		    								
	 		    							// q1_id_pos collects suspected nodes tested positive
	 		    							// (that eventually triggers a protocol)
	 		    							
	 		    								
	 		    							} else {
	 		    							
	 		    								n_test_symp_n++;
	 		    								
	 		    								
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
	  	
	  			
	  				
      				
	  			d1=d1+week;

	  	                           
      			break;
      		case 2:	
    			
				new_exposed=0; new_pro=0; new_ic_d=0; new_ic=0; new_is=0; new_q3=0;new_q2_reg=0; n_test_symp=0;n_test_symp_p=0; n_test_symp_n=0; 
				n_test_t=0; n_test_t_p=0; n_test_t_n=0;
    			// New cases in each compartments divided by role: STUDENTS/ TEACHERS/ REPLACED TEACHERS
    			new_exp_s=0; new_exp_t=0; new_exp_r=0; 
				new_pro_s=0; new_pro_t=0; new_pro_r=0; 
				new_ic_s=0; new_ic_t=0; new_ic_r=0;
    			new_ic_d_s=0;new_ic_d_t=0; new_ic_d_r=0;
    			new_is_s=0; new_is_t=0; new_is_r=0;
    		    new_q2_s=0; new_q2_t=0; new_pos_r=0;
    			
    			new_q2_regs=0;new_q2_regt=0;new_q2_regr=0;
    			new_neg_s=0;new_neg_t=0;new_neg_r=0;
				n_test_symp_st=0; n_test_symp_tc=0;
    		
	  			
    			for (m=0; m<num_15min_day; m++){
    			
    				m_absolute=num_15min_day*day_simulation + m; //progression of 15 minutes step from the first day to the last one
	        
	        		N_s=0; N_e=0;N_ip=0;N_ic=0;N_isc=0;N_rp=0;N_r=0;N_q1=0;N_q2=0;N_q3=0;
					N_st=0;N_et=0;N_ipt=0;N_ict=0;N_isct=0;N_rpt=0;N_rt=0; N_q1_t=0;N_q2_t=0;N_q3_t=0;N_q1_s=0;N_q2_s=0;N_q3_s=0;
					
					//Prevalence by sub-compartments and role
					N_ss1=0;N_es1=0;N_ips1=0;N_ics1=0;N_iscs1=0;N_rps1=0;N_rs1=0;
					N_st1=0;N_et1=0;N_ipt1=0;N_ict1=0;N_isct1=0;N_rpt1=0;N_rt1=0;
					N_ips11=0;N_ips12=0;N_ips13=0;N_ips14=0;N_ics11=0;N_ics12=0;N_ics13=0;N_ics14=0;N_ics15=0;N_ics16=0;
					N_iscs11=0;N_iscs12=0;N_iscs13=0;N_iscs14=0;N_iscs15=0;N_iscs16=0;
					N_ipt11=0;N_ipt12=0;N_ipt13=0;N_ipt14=0;N_ict11=0;N_ict12=0;N_ict13=0;N_ict14=0;N_ict15=0;N_ict16=0;
					N_isct11=0;N_isct12=0;N_isct13=0;N_isct14=0;N_isct15=0;N_isct16=0;

					N_rd=0;
			
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
								
								regular_screening(data, param, tc, status0, teach_rep, teach_rtested, pre_flag, quarantine, detected, screenstatus, comp_id, start_q2, start_rq2, m_absolute, day_simulation, nr, theta_ps, theta_cs, theta_scs, theta_rps, counter_prodro, counter_infect, counter_rp, num_pos_class, fp12, fp13);
								
								count_positive_st(data, nr, day_simulation, counter_prodro, counter_infect, counter_rp, status, detected, quarantine, pre_flag, num_iso, fp20);	
								if (nweek==2){
									day_screen=day_screen+(2*week);
							}
						}
	  		    			
						for (i=0; i<ct->n1; i++){
 	  	
 		  	  	  			if ((ct->step[i]==m) & (ct->days[i]==d2)){
 		  	  	  
 		  	  	  			//fprintf(stderr, "Insidecontact");
 			
 		        				id1=ct->ID1[i];
 		        				id2=ct->ID2[i];
 		        		
 		        		
 		        				index_id1=index_nodes(data, id1); 
 		        				index_id2=index_nodes(data, id2); 
 		        
  		        				s_id1=status0[index_id1];
 		        				s_id2=status0[index_id2];
 		        		
 		        				role_id1=data->classes[index_id1];
 		        				role_id2=data->classes[index_id2];
 		        		
 		        				weight=ct->weight[i];
 		        		
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
 		           							//fprintf(fp7, "%d\t %d\t %d\t %d\t %d\t %d\t %d\n", nr, id1,id2,s_id1, s_id2, exp_day[index_id1], day_simulation); 
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
 		    		
 		    			//fprintf(stderr, "inside transition");
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
 		    									  
 		    				
 		    									q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    									q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    									  
 		    									
 		    									  
 		    									
 		    									n_transitions2++;
 		    							
 		    									  
 		    									  
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
 		    										/*if (preventivegrade==1){
 		    											mod=tc->clas[index_t(tc, id)]%10;
	 		    										for (ttc=0; ttc< tc->nc; ttc++){
	 		    											
	 		    											if (tc->clas[ttc]%10==mod){
	 		    													class_iso[ttc]=1;	
	 		    											}
	 		    										}
	 		    									}*/
	 		    									
 		    										// SAVE when preventive and from whom the prev
 		    										// is trigger
 		    										// trigger_role
 		    										// 1:students, 2: teacher, 3: replaced teacher
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
 		    						
 		    							r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);
 		    							
 		    							fprintf(stderr, "q2 id:%d\t start Q2: %d\t end Q2: %d\t (day:%d)\t role: %d\t result:%d\n", id, q2_quarter[hh], m_absolute, day_simulation, role_id, r);
 		    						
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
 		       					
 		        					r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);
 		        					  
 		        					
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
 		       					
 		       				}
 		       			}
 		       			if ((regular==1) | (reactive==1) ){
 		       				  
 		       				
	  					if ((screenstatus[j]==6) & ((m_absolute==start_q2[j]) | (m_absolute==start_rq2[j]))){
	  					
	  						
	  						new_q2_reg++;
	  						if ((role_id!=6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
	  						
	  							fprintf(stderr, "Quarantine - student id %d\t start %d\n", id, day_simulation);
	  							
	  							new_q2_regs++;
	  							quarantine[j]=1;
	  							
	  							if ((preventive == 1) & 
	  							(num_pos_class[index_class(tc, data->classes[index_id])]>=3) &
	  							(class_iso[index_class(tc, data->classes[index_id])]==0)){
	  							
	  								 fprintf(stderr, "\n PREISO %d\n", day_simulation);
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
 		       					
 		       						r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);
 		        					
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
	 		   								
	 		    								
	 		    							// q1_id_pos collects suspected nodes tested positive
	 		    							// (that eventually triggers a protocol)
	 		    							
	 		    								
	 		    							} else {
	 		    							
	 		    								n_test_symp_n++;
	 		    								
	 		    								
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
	  		
	  			
	  			d2=d2+week;
	  	                           
      			break;
      		case 3:	
    		
    			new_exposed=0; new_pro=0; new_ic_d=0; new_ic=0; new_is=0; new_q3=0;new_q2_reg=0; n_test_symp=0;n_test_symp_p=0; n_test_symp_n=0; 
				n_test_t=0; n_test_t_p=0; n_test_t_n=0;
    			// New cases in each compartments divided by role: STUDENTS/ TEACHERS/ REPLACED TEACHERS
    			new_exp_s=0; new_exp_t=0; new_exp_r=0; 
				new_pro_s=0; new_pro_t=0; new_pro_r=0; 
				new_ic_s=0; new_ic_t=0; new_ic_r=0;
    			new_ic_d_s=0;new_ic_d_t=0; new_ic_d_r=0;
    			new_is_s=0; new_is_t=0; new_is_r=0;
    		    new_q2_s=0; new_q2_t=0; new_pos_r=0;
    			
    			new_q2_regs=0;new_q2_regt=0;new_q2_regr=0;
    			new_neg_s=0;new_neg_t=0;new_neg_r=0;
				n_test_symp_st=0; n_test_symp_tc=0;
    		
	  			
    			for (m=0; m<num_15min_day; m++){
    			
    				m_absolute=num_15min_day*day_simulation + m; //progression of 15 minutes step from the first day to the last one
	        
	        		N_s=0; N_e=0;N_ip=0;N_ic=0;N_isc=0;N_rp=0;N_r=0;N_q1=0;N_q2=0;N_q3=0;
					N_st=0;N_et=0;N_ipt=0;N_ict=0;N_isct=0;N_rpt=0;N_rt=0; N_q1_t=0;N_q2_t=0;N_q3_t=0;N_q1_s=0;N_q2_s=0;N_q3_s=0;
					
					//Prevalence by sub-compartments and role
					N_ss1=0;N_es1=0;N_ips1=0;N_ics1=0;N_iscs1=0;N_rps1=0;N_rs1=0;
					N_st1=0;N_et1=0;N_ipt1=0;N_ict1=0;N_isct1=0;N_rpt1=0;N_rt1=0;
					N_ips11=0;N_ips12=0;N_ips13=0;N_ips14=0;N_ics11=0;N_ics12=0;N_ics13=0;N_ics14=0;N_ics15=0;N_ics16=0;
					N_iscs11=0;N_iscs12=0;N_iscs13=0;N_iscs14=0;N_iscs15=0;N_iscs16=0;
					N_ipt11=0;N_ipt12=0;N_ipt13=0;N_ipt14=0;N_ict11=0;N_ict12=0;N_ict13=0;N_ict14=0;N_ict15=0;N_ict16=0;
					N_isct11=0;N_isct12=0;N_isct13=0;N_isct14=0;N_isct15=0;N_isct16=0;

					N_rd=0;
			
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
 		           							//fprintf(fp7, "%d\t %d\t %d\t %d\t %d\t %d\t %d\n", nr, id1,id2,s_id1, s_id2, exp_day[index_id1], day_simulation); 
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
 		    									
 		    				
 		    									q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    									q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    									
 		    									n_transitions2++;
 		    							
 		    									  
 		    									  
 		    									detected_t[index_t(tc,id)]=0;
 		    									quarantine_t[index_t(tc, id)]=1;
 		    									
 		    									
 		    								
 		    									if ((preventive==1) & (regular==0)){
 		    					
 		    				 						if (class_iso[index_t(tc, id)]==0){
 		    							
 		    											// Turn on if the class is in pre iso
 		    											class_iso[index_t(tc, id)]=1;
 		    											
 		    											dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    											/*if (preventivegrade==1){
 		    												mod=tc->clas[index_t(tc, id)]%10;
 		    												 		    															for (ttc=0; ttc< tc->nc; ttc++){
	 		    											
	 		    												if (tc->clas[ttc]%10==mod){
	 		    													class_iso[ttc]=1;	
	 		    												}
	 		    											}
	 		    										}*/
 		    											// SAVE when preventive and from whom the prev
 		    											// is trigger
 		    											// trigger_role
 		    											// 1:students, 2: teacher, 3: replaced teacher
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
 		    										/*if (preventivegrade==1){
 		    											mod=tc->clas[index_t(tc, id)]%10;
	 		    										for (ttc=0; ttc< tc->nc; ttc++){
	 		    											
	 		    											if (tc->clas[ttc]%10==mod){
	 		    													class_iso[ttc]=1;	
	 		    											}
	 		    										}
	 		    									}*/
 		    										// SAVE when preventive and from whom the prev
 		    										// is trigger
 		    										// trigger_role
 		    										// 1:students, 2: teacher, 3: replaced teacher
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
 		    				
 		    				
 		    		  		// End of quarantine for IC(d) in Q2
 		    		  	  	for(hh=0; hh<n_transitions2; hh++){
 		    			
 		    		  			if( (id==q2_id[hh]) & (m_absolute == q2_quarter[hh]+(param->delta_q/param->deltat) ) ){
 		   				
 		   						if (role_id!=6){ 
 		   					
 		   							quarantine[index_id]=0;
 		   							
 		   						}else if(role_id==6){

 		    							r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);

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
 		       					
 		        					r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);
 		        					
 		        					n_test_t++;
 		        							
 		        					if (r==1){
 		    							n_test_t_p++;
 		    						}else{
 		    							n_test_t_n++;
 		    						}
 		        		
 		        			
 		       					}
 		       					pre_flag[index_id]=0;
 		       					
 		       				}
 		       			}
 		       			if ((regular==1) | (reactive==1) ){
 		       				  
 		       				
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
 		       					
 		       						r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);
 		       						
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
	 		    										
	 		    										  
	 		    										//n_replace_t++;
	 		    								
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
	  	

	  			
	  			d3=d3+week;
	  	                           
      			break;
	  	                           
      		case 4:	
    		
    			new_exposed=0; new_pro=0; new_ic_d=0; new_ic=0; new_is=0; new_q3=0;new_q2_reg=0; n_test_symp=0;n_test_symp_p=0; n_test_symp_n=0; 
				n_test_t=0; n_test_t_p=0; n_test_t_n=0;
    			// New cases in each compartments divided by role: STUDENTS/ TEACHERS/ REPLACED TEACHERS
    			new_exp_s=0; new_exp_t=0; new_exp_r=0; 
				new_pro_s=0; new_pro_t=0; new_pro_r=0; 
				new_ic_s=0; new_ic_t=0; new_ic_r=0;
    			new_ic_d_s=0;new_ic_d_t=0; new_ic_d_r=0;
    			new_is_s=0; new_is_t=0; new_is_r=0;
    		    new_q2_s=0; new_q2_t=0; new_pos_r=0;
    			
    			new_q2_regs=0;new_q2_regt=0;new_q2_regr=0;
    			new_neg_s=0;new_neg_t=0;new_neg_r=0;
				n_test_symp_st=0; n_test_symp_tc=0;
    		
	  			
    			for (m=0; m<num_15min_day; m++){
    			
    				m_absolute=num_15min_day*day_simulation + m; //progression of 15 minutes step from the first day to the last one
	        
	        		N_s=0; N_e=0;N_ip=0;N_ic=0;N_isc=0;N_rp=0;N_r=0;N_q1=0;N_q2=0;N_q3=0;
					N_st=0;N_et=0;N_ipt=0;N_ict=0;N_isct=0;N_rpt=0;N_rt=0; N_q1_t=0;N_q2_t=0;N_q3_t=0;N_q1_s=0;N_q2_s=0;N_q3_s=0;
					
					//Prevalence by sub-compartments and role
					N_ss1=0;N_es1=0;N_ips1=0;N_ics1=0;N_iscs1=0;N_rps1=0;N_rs1=0;
					N_st1=0;N_et1=0;N_ipt1=0;N_ict1=0;N_isct1=0;N_rpt1=0;N_rt1=0;
					N_ips11=0;N_ips12=0;N_ips13=0;N_ips14=0;N_ics11=0;N_ics12=0;N_ics13=0;N_ics14=0;N_ics15=0;N_ics16=0;
					N_iscs11=0;N_iscs12=0;N_iscs13=0;N_iscs14=0;N_iscs15=0;N_iscs16=0;
					N_ipt11=0;N_ipt12=0;N_ipt13=0;N_ipt14=0;N_ict11=0;N_ict12=0;N_ict13=0;N_ict14=0;N_ict15=0;N_ict16=0;
					N_isct11=0;N_isct12=0;N_isct13=0;N_isct14=0;N_isct15=0;N_isct16=0;

					N_rd=0;
			
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
	  		    		
	  		    				regular_screening(data, param, tc, status0, teach_rep, teach_rtested, pre_flag, quarantine, detected, screenstatus, comp_id, start_q2, start_rq2, m_absolute, day_simulation, nr, theta_ps, theta_cs, theta_scs, theta_rps, counter_prodro, counter_infect, counter_rp, num_pos_class, fp12, fp13);
	  		    				
	  		    				count_positive_st(data, nr, day_simulation, counter_prodro, counter_infect, counter_rp, status, detected, quarantine, pre_flag, num_iso, fp20);
	  		    				
	  		    		}
	  		    			
						for (i=0; i<ct->n1; i++){
 	  	
 		  	  	  			if ((ct->step[i]==m) & (ct->days[i]==d4)){
 		  	  	  
 		  	  	  				//fprintf(stderr, "Insidecontact");
 			
 		        				id1=ct->ID1[i];
 		        				id2=ct->ID2[i];
 		        		
 		        		
 		        				index_id1=index_nodes(data, id1); 
 		        				index_id2=index_nodes(data, id2); 
 		        
  		        				s_id1=status0[index_id1];
 		        				s_id2=status0[index_id2];
 		        		
 		        				role_id1=data->classes[index_id1];
 		        				role_id2=data->classes[index_id2];
 		        		
 		        				weight=ct->weight[i];
 		        		
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
 		           							//fprintf(fp7, "%d\t %d\t %d\t %d\t %d\t %d\t %d\n", nr, id1,id2,s_id1, s_id2, exp_day[index_id1], day_simulation); 
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
 		    		
 		    			//fprintf(stderr, "inside transition");
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
 		    									  
 		    				
 		    									q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    									q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    									  
 		    									
 		    									  
 		    									
 		    									n_transitions2++;
 		    							
 		    									  
 		    									  
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
 		    										/*if (preventivegrade==1){
 		    											mod=tc->clas[index_t(tc, id)]%10;
	 		    										for (ttc=0; ttc< tc->nc; ttc++){
	 		    											
	 		    											if (tc->clas[ttc]%10==mod){
	 		    													class_iso[ttc]=1;	
	 		    											}
	 		    										}
	 		    									}*/
 		    										
 		    										// SAVE when preventive and from whom the prev
 		    										// is trigger
 		    										// trigger_role
 		    										// 1:students, 2: teacher, 3: replaced teacher
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
 		    						
 		    							r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);
 		    							fprintf(stderr, "q2 id:%d\t start Q2: %d\t end Q2: %d\t (day:%d)\t role: %d\t result:%d\n", id, q2_quarter[hh], m_absolute, day_simulation, role_id, r);
 		    						
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
 		       					
 		        					r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);
 		        						
 		        					  
 		        					
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
 		       					
 		       				}
 		       			}
 		       			if ((regular==1) | (reactive==1) ){
 		       				  
 		       				
	  					if ((screenstatus[j]==6) & ((m_absolute==start_q2[j]) | (m_absolute==start_rq2[j]))){
	  					
	  						
	  						new_q2_reg++;
	  						if ((role_id!=6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
	  						
	  							fprintf(stderr, "Quarantine - student id %d\t start %d\n", id, day_simulation);
	  							
	  							new_q2_regs++;
	  							quarantine[j]=1;
	  							
	  							if ((preventive == 1) & 
	  							(num_pos_class[index_class(tc, data->classes[index_id])]>=3) &
	  							(class_iso[index_class(tc, data->classes[index_id])]==0)){
	  							
	  								 fprintf(stderr, "\n PREISO %d\n", day_simulation);
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
 		       					
 		       						r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);
 		       						
 		       				 		
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
	 		   								
	 		    								
	 		    							// q1_id_pos collects suspected nodes tested positive
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
	 		    										
	 		    										  
	 		    										//n_replace_t++;
	 		    								
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
	  	

	  			d4=d4+week;
	  	                           
      			break;
      		case 5:	
    		
    			new_exposed=0; new_pro=0; new_ic_d=0; new_ic=0; new_is=0; new_q3=0;new_q2_reg=0; n_test_symp=0;n_test_symp_p=0; n_test_symp_n=0; 
				n_test_t=0; n_test_t_p=0; n_test_t_n=0;
    			// New cases in each compartments divided by role: STUDENTS/ TEACHERS/ REPLACED TEACHERS
    			new_exp_s=0; new_exp_t=0; new_exp_r=0; 
				new_pro_s=0; new_pro_t=0; new_pro_r=0; 
				new_ic_s=0; new_ic_t=0; new_ic_r=0;
    			new_ic_d_s=0;new_ic_d_t=0; new_ic_d_r=0;
    			new_is_s=0; new_is_t=0; new_is_r=0;
    		    new_q2_s=0; new_q2_t=0; new_pos_r=0;
    			
    			new_q2_regs=0;new_q2_regt=0;new_q2_regr=0;
    			new_neg_s=0;new_neg_t=0;new_neg_r=0;
				n_test_symp_st=0; n_test_symp_tc=0;
    		
	  			
    			for (m=0; m<num_15min_day; m++){
    			
    				m_absolute=num_15min_day*day_simulation + m; //progression of 15 minutes step from the first day to the last one
	        
	        		N_s=0; N_e=0;N_ip=0;N_ic=0;N_isc=0;N_rp=0;N_r=0;N_q1=0;N_q2=0;N_q3=0;
					N_st=0;N_et=0;N_ipt=0;N_ict=0;N_isct=0;N_rpt=0;N_rt=0; N_q1_t=0;N_q2_t=0;N_q3_t=0;N_q1_s=0;N_q2_s=0;N_q3_s=0;
					
					//Prevalence by sub-compartments and role
					N_ss1=0;N_es1=0;N_ips1=0;N_ics1=0;N_iscs1=0;N_rps1=0;N_rs1=0;
					N_st1=0;N_et1=0;N_ipt1=0;N_ict1=0;N_isct1=0;N_rpt1=0;N_rt1=0;
					N_ips11=0;N_ips12=0;N_ips13=0;N_ips14=0;N_ics11=0;N_ics12=0;N_ics13=0;N_ics14=0;N_ics15=0;N_ics16=0;
					N_iscs11=0;N_iscs12=0;N_iscs13=0;N_iscs14=0;N_iscs15=0;N_iscs16=0;
					N_ipt11=0;N_ipt12=0;N_ipt13=0;N_ipt14=0;N_ict11=0;N_ict12=0;N_ict13=0;N_ict14=0;N_ict15=0;N_ict16=0;
					N_isct11=0;N_isct12=0;N_isct13=0;N_isct14=0;N_isct15=0;N_isct16=0;

					N_rd=0;
			
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
						
							regular_screening(data, param, tc, status0, teach_rep, teach_rtested, pre_flag, quarantine, detected, screenstatus, comp_id, start_q2, start_rq2, m_absolute, day_simulation, nr, theta_ps, theta_cs, theta_scs, theta_rps, counter_prodro, counter_infect, counter_rp, num_pos_class, fp12, fp13);
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
 		           							//fprintf(fp7, "%d\t %d\t %d\t %d\t %d\t %d\t %d\n", nr, id1,id2,s_id1, s_id2, exp_day[index_id1], day_simulation); 
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
 		    		
 		    			//fprintf(stderr, "inside transition");
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
 		    									  
 		    				
 		    									q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    									q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    									  
 		    									
 		    									  
 		    									
 		    									n_transitions2++;
 		    							
 		    									  
 		    									  
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
 		    										/*if (preventivegrade==1){
 		    											mod=tc->clas[index_t(tc, id)]%10;
	 		    										for (ttc=0; ttc< tc->nc; ttc++){
	 		    											
	 		    											if (tc->clas[ttc]%10==mod){
	 		    													class_iso[ttc]=1;	
	 		    											}
	 		    										}
	 		    									}*/
	 		    										
 		    										// SAVE when preventive and from whom the prev
 		    										// is trigger
 		    										// trigger_role
 		    										// 1:students, 2: teacher, 3: replaced teacher
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
 		    						
 		    							r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);
 		    							fprintf(stderr, "q2 id:%d\t start Q2: %d\t end Q2: %d\t (day:%d)\t role: %d\t result:%d\n", id, q2_quarter[hh], m_absolute, day_simulation, role_id, r);
 		    						
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
 		       					
 		        					r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);
 		        					  
 		        					
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
 		       					
 		       				}
 		       			}
 		       			if ((regular==1) | (reactive==1) ){
 		       				  
 		       				
	  					if ((screenstatus[j]==6) & ((m_absolute==start_q2[j]) | (m_absolute==start_rq2[j]))){
	  					
	  						
	  						new_q2_reg++;
	  						if ((role_id!=6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
	  						
	  							fprintf(stderr, "Quarantine - student id %d\t start %d\n", id, day_simulation);
	  							
	  							new_q2_regs++;
	  							quarantine[j]=1;
	  							
	  							if ((preventive == 1) & 
	  							(num_pos_class[index_class(tc, data->classes[index_id])]>=3) &
	  							(class_iso[index_class(tc, data->classes[index_id])]==0)){
	  							
	  								 fprintf(stderr, "\n PREISO %d\n", day_simulation);
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
 		       					
 		       						r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);
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
															
	 		    										  
	 		    										//n_replace_t++;
	 		    								
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
	  	

	  	                           
      			break;
      		case 6:	
    		
    			new_exposed=0; new_pro=0; new_ic_d=0; new_ic=0; new_is=0; new_q3=0;new_q2_reg=0; n_test_symp=0;n_test_symp_p=0; n_test_symp_n=0; 
				n_test_t=0; n_test_t_p=0; n_test_t_n=0;
    			// New cases in each compartments divided by role: STUDENTS/ TEACHERS/ REPLACED TEACHERS
    			new_exp_s=0; new_exp_t=0; new_exp_r=0; 
				new_pro_s=0; new_pro_t=0; new_pro_r=0; 
				new_ic_s=0; new_ic_t=0; new_ic_r=0;
    			new_ic_d_s=0;new_ic_d_t=0; new_ic_d_r=0;
    			new_is_s=0; new_is_t=0; new_is_r=0;
    		    new_q2_s=0; new_q2_t=0; new_pos_r=0;
    			
    			new_q2_regs=0;new_q2_regt=0;new_q2_regr=0;
    			new_neg_s=0;new_neg_t=0;new_neg_r=0;
				n_test_symp_st=0; n_test_symp_tc=0;
    		
	  			
    			for (m=0; m<num_15min_day; m++){
    			
    				m_absolute=num_15min_day*day_simulation + m; //progression of 15 minutes step from the first day to the last one
	        		
					N_s=0; N_e=0;N_ip=0;N_ic=0;N_isc=0;N_rp=0;N_r=0;N_q1=0;N_q2=0;N_q3=0;
					N_st=0;N_et=0;N_ipt=0;N_ict=0;N_isct=0;N_rpt=0;N_rt=0; N_q1_t=0;N_q2_t=0;N_q3_t=0;N_q1_s=0;N_q2_s=0;N_q3_s=0;
					
					//Prevalence by sub-compartments and role
					N_ss1=0;N_es1=0;N_ips1=0;N_ics1=0;N_iscs1=0;N_rps1=0;N_rs1=0;
					N_st1=0;N_et1=0;N_ipt1=0;N_ict1=0;N_isct1=0;N_rpt1=0;N_rt1=0;
					N_ips11=0;N_ips12=0;N_ips13=0;N_ips14=0;N_ics11=0;N_ics12=0;N_ics13=0;N_ics14=0;N_ics15=0;N_ics16=0;
					N_iscs11=0;N_iscs12=0;N_iscs13=0;N_iscs14=0;N_iscs15=0;N_iscs16=0;
					N_ipt11=0;N_ipt12=0;N_ipt13=0;N_ipt14=0;N_ict11=0;N_ict12=0;N_ict13=0;N_ict14=0;N_ict15=0;N_ict16=0;
					N_isct11=0;N_isct12=0;N_isct13=0;N_isct14=0;N_isct15=0;N_isct16=0;

					N_rd=0;
			
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
 		    		
 		    			
 		    			if ((symptomatic==1) & (n_transitions1>0) & (n_transitions2>=0)){
 		    		
 		    			
 		    		  		for (g=0; g<n_transitions1; g++){
 		    					
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
 		    									  
 		    				
 		    									q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    									q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    									  
 		    									
 		    									  
 		    									
 		    									n_transitions2++;
 		    							
 		    									  
 		    									  
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
 		    				
 		    				
 		    		  		
 		    		  		// End of quarantine for IC(d) in Q2
 		    		  	  	for(hh=0; hh<n_transitions2; hh++){
 		    			
 		    		  			if( (id==q2_id[hh]) & (m_absolute == q2_quarter[hh]+(param->delta_q/param->deltat) ) ){
 		   				
 		   						if (role_id!=6){ 
 		   					
 		   							quarantine[index_id]=0;
 		   							  
 		   						
 		   						}else if(role_id==6){
 		    					
 		    						
 		    							
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
 		       					
 		        					r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);
 		        					  
 		        					
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
 		       					
 		       				}
 		       			}
 		       			if ((regular==1) | (reactive==1) ){
 		       				  
 		       				
	  					if ((screenstatus[j]==6) & ((m_absolute==start_q2[j]) | (m_absolute==start_rq2[j]))){
	  					
	  						
	  						new_q2_reg++;
	  						if ((role_id!=6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
	  						
	  							fprintf(stderr, "Quarantine - student id %d\t start %d\n", id, day_simulation);
	  							
	  							new_q2_regs++;
	  							quarantine[j]=1;
	  							
	  							if ((preventive == 1) & 
	  							(num_pos_class[index_class(tc, data->classes[index_id])]>=3) &
	  							(class_iso[index_class(tc, data->classes[index_id])]==0)){
	  							
	  								 fprintf(stderr, "\n PREISO %d\n", day_simulation);
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
 		       					
 		       						r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);
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
	 		   								
	 		    								
	 		    							// q1_id_pos collects suspected nodes tested positive
	 		    							// (that eventually triggers a protocol)
	 		    							
	 		    								
	 		    							} else {
	 		    							
	 		    								n_test_symp_n++;
	 		    								
	 		    								
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
	 		    										
	 		    										  
	 		    										//n_replace_t++;
	 		    								
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
	  	
	  	
	  				
	  			          
      			break;
      		case 7:	
    		
    			new_exposed=0; new_pro=0; new_ic_d=0; new_ic=0; new_is=0; new_q3=0;new_q2_reg=0; n_test_symp=0;n_test_symp_p=0; n_test_symp_n=0; 
				n_test_t=0; n_test_t_p=0; n_test_t_n=0;
    			// New cases in each compartments divided by role: STUDENTS/ TEACHERS/ REPLACED TEACHERS
    			new_exp_s=0; new_exp_t=0; new_exp_r=0; 
				new_pro_s=0; new_pro_t=0; new_pro_r=0; 
				new_ic_s=0; new_ic_t=0; new_ic_r=0;
    			new_ic_d_s=0;new_ic_d_t=0; new_ic_d_r=0;
    			new_is_s=0; new_is_t=0; new_is_r=0;
    		    new_q2_s=0; new_q2_t=0; new_pos_r=0;
    			
    			new_q2_regs=0;new_q2_regt=0;new_q2_regr=0;
    			new_neg_s=0;new_neg_t=0;new_neg_r=0;
				n_test_symp_st=0; n_test_symp_tc=0;
    		
	  			
    			for (m=0; m<num_15min_day; m++){
    			
    				m_absolute=num_15min_day*day_simulation + m; //progression of 15 minutes step from the first day to the last one
	        
	        		N_s=0; N_e=0;N_ip=0;N_ic=0;N_isc=0;N_rp=0;N_r=0;N_q1=0;N_q2=0;N_q3=0;
					N_st=0;N_et=0;N_ipt=0;N_ict=0;N_isct=0;N_rpt=0;N_rt=0; N_q1_t=0;N_q2_t=0;N_q3_t=0;N_q1_s=0;N_q2_s=0;N_q3_s=0;
					
					//Prevalence by sub-compartments and role
					N_ss1=0;N_es1=0;N_ips1=0;N_ics1=0;N_iscs1=0;N_rps1=0;N_rs1=0;
					N_st1=0;N_et1=0;N_ipt1=0;N_ict1=0;N_isct1=0;N_rpt1=0;N_rt1=0;
					N_ips11=0;N_ips12=0;N_ips13=0;N_ips14=0;N_ics11=0;N_ics12=0;N_ics13=0;N_ics14=0;N_ics15=0;N_ics16=0;
					N_iscs11=0;N_iscs12=0;N_iscs13=0;N_iscs14=0;N_iscs15=0;N_iscs16=0;
					N_ipt11=0;N_ipt12=0;N_ipt13=0;N_ipt14=0;N_ict11=0;N_ict12=0;N_ict13=0;N_ict14=0;N_ict15=0;N_ict16=0;
					N_isct11=0;N_isct12=0;N_isct13=0;N_isct14=0;N_isct15=0;N_isct16=0;

					N_rd=0;
			
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
 		    		
 		    			
 		    			if ((symptomatic==1) & (n_transitions1>0) & (n_transitions2>=0)){
 		    			 
 		    		  		for (g=0; g<n_transitions1; g++){
 		    					
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
 		    						
 		    									}else{
 		    										pre_flag[index_id]=0;
 		    									}
 		    								}

 		    							}else{
 		    								detected[index_id]=0;
 		    								new_neg_s++;
 		    								
 		    								if (preventive==1){
 		    									class_q3=data->classes[index_id];
 		    									teach_q3=find(tc, id, class_q3);
 		    									if (class_iso[index_t(tc, teach_q3)]==1){
 		    										pre_flag[index_id]=1;
 		    									}
 		    								}
 		    							}
 		    							
 		    						}else if (role_id==6){
 		    							
 		    							if (teach_rep[index_t(tc, id)]==1){
 		    								
 		    								if (q1_result[g]==1){
 		    							
 		    									new_q2_t++;
 		    								
 		    									q2_id[n_transitions2]=id;
 		   		        						q2_quarter[n_transitions2]=m_absolute;
 		    									
 		    									q2_id=(int*)realloc(q2_id,(n_transitions2+2)*sizeof(int));
 		    									q2_quarter=(int*)realloc(q2_quarter,(n_transitions2+2)*sizeof(int));
 		    									
 		    									n_transitions2++;
 		    							
 		    									  
 		    									  
 		    									detected_t[index_t(tc,id)]=0;
 		    									quarantine_t[index_t(tc, id)]=1;
 		    								
 		    									
 		    									
	 		    								
 		    									if ((preventive==1) & (regular==0)){
 		    					
 		    				 						if (class_iso[index_t(tc, id)]==0){
 		    							
 		    											// Turn on if the class is in pre iso
 		    											class_iso[index_t(tc, id)]=1;
 		    											
 		    											dim=preisolation(data, tc, param, id, class_q3, day_simulation, m_absolute, pre_flag, teach_rep, pre_time, class_iso, screenstatus, preventivegrade);
 		    											
 		    											trigger_role=2;
 		    						
 		    											 
 		    											
 		    											new_q3=new_q3+dim;
 		    										}
 		    										
 		    									}
 		    								
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
 		    				
 		    				
 		    				
 		    		  		// End of quarantine for IC(d) in Q2
 		    		  	  	for(hh=0; hh<n_transitions2; hh++){
 		    			
 		    		  			if( (id==q2_id[hh]) & (m_absolute == q2_quarter[hh]+(param->delta_q/param->deltat) ) ){
 		   				
 		   						if (role_id!=6){ 
 		   					
 		   							quarantine[index_id]=0;
 		   							
 		   						}else if(role_id==6){
 		    					
 		    							r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);
 		    							
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
 		       					
 		        					r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);
 		        					  
 		        					
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
 		       					
 		       				}
 		       			}
 		       			if ((regular==1) | (reactive==1) ){
 		       				
	  						if ((screenstatus[j]==6) & ((m_absolute==start_q2[j]) | (m_absolute==start_rq2[j]))){
	  					
	  						
								new_q2_reg++;
								if ((role_id!=6) & (m_absolute==start_q2[j]) & (start_q2[j]!=0)){
								
									fprintf(stderr, "Quarantine - student id %d\t start %d\n", id, day_simulation);
									
									new_q2_regs++;
									quarantine[j]=1;
									
									if ((preventive == 1) & 
									(num_pos_class[index_class(tc, data->classes[index_id])]>=3) &
									(class_iso[index_class(tc, data->classes[index_id])]==0)){
									
										fprintf(stderr, "\n PREISO %d\n", day_simulation);
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
 		       					
 		       						r=test_teach(tc, param, preventive, symptomatic, id, index_id, teach_2iso, teach_2isoday, teach_2isom, teach_rep, quarantine_t, pre_flag, nr, day_simulation, m_absolute, status0, status, theta_p, theta_c, theta_sc, theta_rp, counter_latent, counter_prodro, counter_infect, counter_rp);
 		       				 		
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
														
														  
														//n_replace_t++;
												
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
	  	
	  	
	  	
	  			


				if (((*introst)[nr].weeks[nw] + weekly_transmission_st) != 0){
	  				weight_intro =  (*introst)[nr].weeks[nw]/ ((*introst)[nr].weeks[nw] + weekly_transmission_st);
	  				if (nw<6){
		  				(*introst)[nr].weeks[nw+1] = (*introst)[nr].weeks[nw+1]* weight_intro;
		  	            }  
	  			}else{
	  				(*introst)[nr].weeks[nw+1] = (*introst)[nr].weeks[nw+1];
	  			}
	  			
	  			if (((*introtc)[nr].weeks[nw] + weekly_transmission_tc) != 0){
	  				weight_intro =  (*introtc)[nr].weeks[nw]/ ((*introtc)[nr].weeks[nw] + weekly_transmission_tc);
	  				if (nw<6){
		  				(*introtc)[nr].weeks[nw+1] = (*introtc)[nr].weeks[nw+1]* weight_intro;
		  	        }  
	  			}else{
	  				(*introtc)[nr].weeks[nw+1] = (*introtc)[nr].weeks[nw+1];
	  			}
	  			
	  			for (j=0; j<data->n; j++){
	  				if(nweek_2doses[j]!=100){
	  					nweek_2doses[j]= nweek_2doses[j]+1;
	  				}
	  				
	  				if(nweek_booster[j]!=100){
	  					nweek_booster[j]= nweek_booster[j]+1;
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

				for(vc=0; vc<data->n; vc++){
					if((data->classes[vc]!=6) & (vaccinated_2doses[vc]==1)){
						nweek_2doses[vc] = nweek_2doses[vc] +1 ;
					}
				}
	  	                
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
    	
    }
    
   
    free(q1_quarter);
    free(q1_result);
    free(q2_quarter);
    
    free(q1_id);
    free(q2_id);
    
    
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





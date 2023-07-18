#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <dirent.h>
#include <string.h>
#include <math.h>

#define BETA_DECAY 1
#define EC_DECAY 2
#define ALPHA_DECAY 3
#define DELAYED_DECAY 4
#define IT_DECAY 5
#define B_A_DECAY 6

#define ln2 0.69314718056
#define MAXROOTS 2000
#define MAXCHILDREN 100
#define MAXDECAYS 2000
#define MAXTIMES 100
#define MAXPARENTS 2000

struct DecayNode {
  int node_lvl;
  int id_idx;
  int decay_type;
  char parent_estr[255];
  char child_estr[255];
  int parent_xflag;
  int child_xflag;
  float dr; /* decay rate in 1/secs = ln(2)/t */
  float br; /* branching ratio */
  char parent_nucid[255];
  char child_nucid[255];
  struct DecayNode *parent;
  struct DecayNode* child_list[MAXCHILDREN];
  int num_children;
};

struct DecaySum {
  char dsid[32];
  char dname[100];
  int id_idx;
  char estr[30];
  int xflag;
  double sum;
  int sum_idx;
  float dr;
  float br;
};

void RemoveSpaces(char* s) {
  char* d = s;

  do {
    while (*d == ' ') {
      ++d;
    }
  } while ((*s++ = *d++));
}

int compar(const void *arg1,const void *arg2) {
  struct DecaySum *ptr1;
  struct DecaySum *ptr2;
  float e1,e2;

  ptr1 = (struct DecaySum *)arg1;
  ptr2 = (struct DecaySum *)arg2;
  if (ptr1->id_idx == ptr2->id_idx) {
    e1 = strtof(ptr1->estr,NULL);
    e2 = strtof(ptr2->estr,NULL);
    if (e1 == e2) {
      return ptr1->xflag - ptr2->xflag;
    } else {
      return e1 - e2;
    }
  } else {
    return ptr1->id_idx - ptr2->id_idx;
  }
}

void BuildDecayList(sqlite3 *db, struct DecayNode *Node,struct DecaySum *DecayList,int *numdecays) {
  struct DecayNode *ptr;
  int i;
  int j;
  char str1[100];
  char str2[100];
  int uniquedecay;
  int rc;
  sqlite3_stmt *res;
  const char *tail;
  int rec_count;
  char select_str[255];

  str2[0] = '\0';
  
  ptr = Node;
  if (strstr(ptr->parent_nucid,"ROOT") == NULL) {
    uniquedecay = 1;
    for (i=0;i<*numdecays;i++) {
      printf("estrs %s %s %s %s\n",ptr->parent_nucid,ptr->child_nucid,ptr->child_estr,DecayList[i].estr);
      if (ptr->id_idx == DecayList[i].id_idx && strtof(ptr->child_estr,NULL) == strtof(DecayList[i].estr,NULL) && ptr->child_xflag == DecayList[i].xflag) {
	uniquedecay = 0;
      }
    }
    if (uniquedecay) {
      printf("decay nucid = %s %s %d\n",ptr->parent_nucid,ptr->child_nucid,*numdecays);
      sprintf(DecayList[*numdecays].dname,"%s(%.2f)%s(%.2f)",ptr->parent_nucid,strtof(ptr->parent_estr,NULL),ptr->child_nucid,strtof(ptr->child_estr,NULL));
      DecayList[*numdecays].id_idx = ptr->id_idx;
      sprintf(DecayList[*numdecays].estr,"%s",ptr->child_estr);
      DecayList[*numdecays].xflag = ptr->child_xflag;
      DecayList[*numdecays].sum = 0.0;
      DecayList[*numdecays].dr = ptr->dr;
      DecayList[*numdecays].br = ptr->br;
      sprintf(select_str,"SELECT DSID FROM ID WHERE ID_IDX==%d;",ptr->id_idx);
      rc = sqlite3_prepare_v2(db, select_str, 1000, &res, &tail);
      while (sqlite3_step(res) == SQLITE_ROW) {
	sprintf(DecayList[*numdecays].dsid,"%s",sqlite3_column_text(res,0));
      }
      sqlite3_finalize(res);

      *numdecays = *numdecays + 1;
    }
    for (i=0; i<ptr->num_children; i++) {
      BuildDecayList(db,ptr->child_list[i],DecayList,numdecays);
    }
  } else {
    for (i=0; i<ptr->num_children; i++) {
      BuildDecayList(db,ptr->child_list[i],DecayList,numdecays);
    }
  }  
}

void CreateLeafList(struct DecayNode *Node,struct DecayNode **LeafList,int *numleaves) {
  struct DecayNode *ptr;
  int i;
  int j;
  char str1[100];
  char str2[100];

  str2[0] = '\0';
  
  ptr = Node;
  for (i=0; i<ptr->num_children; i++) {
    CreateLeafList(ptr->child_list[i],LeafList,numleaves);
  }
  if (ptr->num_children == 0) {
     LeafList[*numleaves] = ptr;
     *numleaves = *numleaves + 1;
  }
}

void TraverseTree(struct DecayNode *Node,char *oname,char *zname) {
  struct DecayNode *ptr;
  int i;
  int j;
  char str1[100];
  char str2[100];
  FILE *ofile;
  char cname[100];

  str2[0] = '\0';
    
  ptr = Node;
  if (strstr(ptr->parent_nucid,"ROOT") == NULL) {
    for (j=0; j<ptr->node_lvl-1;j++) {
      if (j == 0) {
	sprintf(str1,"└──%s",str2);
      } else {
	sprintf(str1,"   %s",str2);
      }
      strcpy(str2,str1);
    }
    ofile = fopen(zname,"a");
    fprintf(ofile,"%s%s(%.2f)%s(%.2f) %5e\n",str2,ptr->parent_nucid,strtof(ptr->parent_estr,NULL),ptr->child_nucid,strtof(ptr->child_estr,NULL),ptr->br);
    fclose(ofile);
    for (i=0; i<ptr->num_children; i++) {
      TraverseTree(ptr->child_list[i],oname,zname);
    }
  } else {
     if (Node->child_xflag == 0) {
       sprintf(zname,"%s.%s(%s).tree",oname,Node->child_nucid,Node->child_estr);
       RemoveSpaces(zname);
     } else {
       sprintf(zname,"%s.%s(%s).%d.tree",oname,Node->child_nucid,Node->child_estr,Node->child_xflag);
       RemoveSpaces(zname);
     }  
     for (i=0; i<ptr->num_children; i++) {
       TraverseTree(ptr->child_list[i],oname,zname);
     }
  }
} 


int GetDecayType(char *dsid) {
  if (strstr(dsid,"B- DECAY") != NULL) {
    return BETA_DECAY;
  } else if (strstr(dsid," EC DECAY") != NULL || strstr(dsid,"B+ DECAY") != NULL) {
    return EC_DECAY;
  } else if (strstr(dsid," A DECAY") != NULL) {
    return ALPHA_DECAY;
  } else if (strstr(dsid," IT DECAY") != NULL) {
    return IT_DECAY;
  } else if (strstr(dsid," DELAYED") != NULL) {
    return DELAYED_DECAY;
  } else if (strstr(dsid, "B-P DECAY") != NULL || strstr(dsid," B-A DECAY") != NULL || strstr(dsid,"B-P DECAY") != NULL ||  strstr(dsid,"B-N DECAY") != NULL || strstr(dsid,"B+P DECAY")!= NULL || strstr(dsid,"B+A DECAY") != NULL || strstr(dsid,"B+N DECAY") != NULL) {
  return DELAYED_DECAY;
  } else {
    return 0;
  }
}

void BranchDecayNode(sqlite3 *db,struct DecayNode *parent_node) {
  char parent_nucid[255];
  char parent_estr[255];
  int parent_xflag;
  int child_idx_list[MAXCHILDREN];
  char child_nucid_list[MAXCHILDREN][255];
  char child_estr_list[MAXCHILDREN][255];
  int child_xflag_list[MAXCHILDREN];
  int child_decay_type_list[MAXCHILDREN];
  float child_decay_rate_list[MAXCHILDREN];
  float child_br_list[MAXCHILDREN];
  int num_children;
  int rc;
  sqlite3_stmt *res_parent;
  sqlite3_stmt *res_idx;
  sqlite3_stmt *res_norm;
  sqlite3_stmt *res_level;
  sqlite3_stmt *res_beta;
  sqlite3_stmt *res_alpha;
  sqlite3_stmt *res_ec;
  sqlite3_stmt *res_delayed;
  const char *tail;
  int rec_count;
  char select_str[255];
  char estr[255];
  float br;
  float e;
  int isomer_idx;
  int idx;
  float dr;
  int level_idx;
  int level_xflag;
  int decay_type;
  float c_isomer_br;
  float isomer_br;
  char nucid[255];
  char dsid[255];
  int child_idx;
  int has_levels;
  struct DecayNode *new_node_ptr;
  struct UniqueDecayNode *unique_decays;
  struct UniqueDecayNode *decay_ptr;
    
  strcpy(parent_nucid,parent_node->child_nucid);
  strcpy(parent_estr,parent_node->child_estr);
  parent_xflag = parent_node->child_xflag;
  
  sprintf(select_str,"SELECT PARENT.ID_IDX,PARENT.T,ID.NUCID,ID.DSID,NORMALIZATION.BR FROM PARENT JOIN ID,NORMALIZATION ON PARENT.NUCID=='%s' AND PARENT.E==%f AND PARENT.ID_IDX==ID.ID_IDX AND PARENT.XFLAG==%d AND NORMALIZATION.ID_IDX==PARENT.ID_IDX;",parent_nucid,strtof(parent_estr,NULL),parent_xflag);
  printf("%s\n",select_str);
  rc = sqlite3_prepare_v2(db, select_str, 1000, &res_parent, &tail);
  num_children = 0;
  while (sqlite3_step(res_parent) == SQLITE_ROW) {
    idx = sqlite3_column_int(res_parent,0);
    dr = ln2/sqlite3_column_double(res_parent,1);
    sprintf(nucid,"%s",sqlite3_column_text(res_parent,2));
    sprintf(dsid,"%s",sqlite3_column_text(res_parent,3));
    decay_type = GetDecayType(dsid);
    printf("dsid = %s %d\n",dsid,decay_type);
    br = sqlite3_column_double(res_parent,4);
    printf("br = %f\n",br);
    if (decay_type > 0) {
       sprintf(select_str,"SELECT LEVEL_IDX,ESTR,XFLAG,ISOMER_IDX FROM LEVEL WHERE ID_IDX==%d;",idx);
       sqlite3_prepare_v2(db, select_str, 1000, &res_level, &tail);
       has_levels = 0;
       while (sqlite3_step(res_level) == SQLITE_ROW) {
         has_levels = 1;
	 level_idx = sqlite3_column_int(res_level,0);
	 sprintf(estr,"%s",sqlite3_column_text(res_level,1));
	 /*	 RemoveSpaces(estr); */
	 level_xflag = sqlite3_column_int(res_level,2);
	 isomer_idx = sqlite3_column_int(res_level,3);
	 printf("dsid = %s level_idx = %d e = %s level_xflag = %d isomer_idx = %d\n",dsid,level_idx,estr,level_xflag,isomer_idx);
         if (strtof(estr,NULL) == 0.0 && level_xflag == 0) {
            child_idx_list[num_children] = idx;
            strcpy(child_nucid_list[num_children],nucid);
	    strcpy(child_estr_list[num_children],estr);
	    child_xflag_list[num_children] = level_xflag;
	    child_decay_type_list[num_children] = decay_type;	
	    child_decay_rate_list[num_children] = dr;
	    child_br_list[num_children] = -1.0;
	    c_isomer_br = 0.0;
	    num_children = num_children + 1;
	 } else {
	    if (isomer_idx > 0) {
	       if (decay_type != IT_DECAY) {
	          child_idx_list[num_children] = idx;
                  strcpy(child_nucid_list[num_children],nucid);
	          strcpy(child_estr_list[num_children],estr);
		  child_xflag_list[num_children] = level_xflag;
	          child_decay_type_list[num_children] = decay_type;	
	          child_decay_rate_list[num_children] = dr;
	          if (decay_type == BETA_DECAY) {
           	     printf("BETA_DECAY\n");
          	     sprintf(select_str,"SELECT IB FROM BETA WHERE LEVEL_IDX==%d;",level_idx);
	             rc = sqlite3_prepare_v2(db, select_str, 1000, &res_beta, &tail);
	             while (sqlite3_step(res_beta) == SQLITE_ROW) {    
		        isomer_br = 0.01*sqlite3_column_double(res_beta,0);
		     }

		     printf("%f %d\n",isomer_br,level_idx);
		     sqlite3_finalize(res_beta);
		  }
	          if (decay_type == ALPHA_DECAY) {
		     printf("ALPHA_DECAY\n");
          	     sprintf(select_str,"SELECT IA FROM ALPHA WHERE LEVEL_IDX==%d;",level_idx);
	             rc = sqlite3_prepare_v2(db, select_str, 1000, &res_alpha, &tail);
	             while (sqlite3_step(res_beta) == SQLITE_ROW) {    
		        isomer_br = 0.01*sqlite3_column_double(res_alpha,0);
		     }
		     sqlite3_finalize(res_beta);
		  }
	          if (decay_type == EC_DECAY) {
		     printf("EC_DECAY\n");
          	     sprintf(select_str,"SELECT IB FROM EC WHERE LEVEL_IDX==%d;",level_idx);
	             rc = sqlite3_prepare_v2(db, select_str, 1000, &res_ec, &tail);
	             while (sqlite3_step(res_ec) == SQLITE_ROW) {    
		        isomer_br = 0.01*sqlite3_column_double(res_ec,0);
		     }
		     sqlite3_finalize(res_ec);
		  }
       	          if (decay_type == DELAYED_DECAY) {
      		     printf("DELAYED_DECAY\n");
          	     sprintf(select_str,"SELECT IP FROM DELAYED WHERE LEVEL_IDX==%d;",level_idx);
	             rc = sqlite3_prepare_v2(db, select_str, 1000, &res_delayed, &tail);
	             while (sqlite3_step(res_delayed) == SQLITE_ROW) {    
		       isomer_br = 0.01*sqlite3_column_double(res_delayed,0);
		     }
		     sqlite3_finalize(res_delayed);
		  }
		  printf("isomer_br = %f\n",isomer_br);
  	          child_br_list[num_children] = br*isomer_br;		 
		  c_isomer_br = c_isomer_br + isomer_br;
                  num_children = num_children + 1;
	       } 
	    }
	 } 
       }
       sqlite3_finalize(res_level);
       if (has_levels == 0) {
	 printf("DOES NOT HAVE LEVELS %s\n",nucid);
         child_idx_list[num_children] = idx;
         strcpy(child_nucid_list[num_children],nucid);
	 strcpy(child_estr_list[num_children],"0.0");
	 child_xflag_list[num_children] = 0;
	 child_decay_type_list[num_children] = decay_type;	
	 child_decay_rate_list[num_children] = dr;
	 child_br_list[num_children] = -1.0;
	 c_isomer_br = 0.0;
	 num_children = num_children + 1;
       }
       child_idx = 0;
       while (child_idx <= num_children) {
          if (child_br_list[child_idx] == -1.0) {
	     child_br_list[child_idx] = br*(1.0-c_isomer_br);
	  }
	  child_idx = child_idx + 1;
       }
    }
  }
  sqlite3_finalize(res_parent);
  child_idx = 0;
  while (child_idx < num_children) {
    new_node_ptr = (struct DecayNode *)malloc(sizeof(struct DecayNode));
    parent_node->child_list[parent_node->num_children] = new_node_ptr;
    new_node_ptr->id_idx = child_idx_list[child_idx];
    new_node_ptr->dr = child_decay_rate_list[child_idx];
    new_node_ptr->decay_type = child_decay_type_list[child_idx];
    new_node_ptr->br = child_br_list[child_idx];
    printf("%f\n",child_br_list[child_idx]);
    strcpy(new_node_ptr->parent_nucid,parent_node->child_nucid);
    strcpy(new_node_ptr->parent_estr,parent_node->child_estr);
    new_node_ptr->parent_xflag = new_node_ptr->child_xflag;
    new_node_ptr->parent = parent_node;
    new_node_ptr->num_children = 0;
    strcpy(new_node_ptr->child_nucid,child_nucid_list[child_idx]);
    strcpy(new_node_ptr->child_estr,child_estr_list[child_idx]);
    new_node_ptr->child_xflag = child_xflag_list[child_idx];
    new_node_ptr->node_lvl = parent_node->node_lvl + 1;
    parent_node->num_children = parent_node->num_children + 1;
    child_idx = child_idx + 1;
    printf("%s(%s)%s(%s)\n",new_node_ptr->parent_nucid,new_node_ptr->parent_estr,new_node_ptr->child_nucid,new_node_ptr->child_estr);
    BranchDecayNode(db,new_node_ptr);
  }
}

void BatemanSolver(double N0,int ntimes,double *tlist,int nlvls,float *BR_list,float *DR_list,double *ActMatrix) {
  double brcoeff,drcoeff;
  double drdiff,dsum;
  int i,j,k,t,l,m;

  brcoeff = 1.0;
  for (i=0; i<nlvls; i++) {
    printf("BR_list[i] = %e\n",BR_list[i]);
    brcoeff = brcoeff*BR_list[i];
  }
  for (j=0;j<nlvls;j++) {
    drcoeff = 1.0;
    if (j > 0) {
      for (k = 0; k<j; k++) {
	drcoeff = drcoeff*DR_list[k];
      }
    }
    for (t=0;t<ntimes;t++) { 
      dsum = 0.0;
      for (l=0; l<j+1; l++) {
	drdiff = 1.0;
	for (m=0; m<j+1; m++) {
	  if (l != m) {
	    if (DR_list[m] != DR_list[l]) {
	      drdiff = drdiff*(DR_list[m] - DR_list[l]);
	    } else {
	      drdiff = drdiff*1e-30;
	    }
	  }
	}
	printf("dsum = %f %f %f\n",DR_list[l],tlist[t],drdiff);  
        dsum = dsum+exp(-DR_list[l]*tlist[t])/drdiff;
      }
      printf("N0 = %e brcoeff = %e drcoeff = %e dsum = %e j = %d nlvls = %d t = %d\n",N0,brcoeff,drcoeff,dsum,j,nlvls,t);
      printf("Here ActMatrix[t*nlvls+j] %f\n",ActMatrix[t*nlvls+j]);
      if (N0*brcoeff*drcoeff*dsum < 0.0) {
	ActMatrix[t*nlvls+j] = 0.0;
      } else {
	ActMatrix[t*nlvls+j] = N0*brcoeff*drcoeff*dsum;
      }
    }
  }
}

int PrintDecaysWithParent(sqlite3 *db,char *parent_str) {
  int rc;
  sqlite3_stmt *res;
  const char *tail;
  int rec_count;
  char select_str[255];

  rec_count=0;
  sprintf(select_str,"SELECT ID_IDX FROM PARENT WHERE NUCID=='%s';",parent_str);
  printf("%s\n",select_str);
  rc = sqlite3_prepare_v2(db, select_str, 1000, &res, &tail);
  while (sqlite3_step(res) == SQLITE_ROW) {
    printf("%d\n",sqlite3_column_int(res, 0));
    rec_count++;
  }
  printf("We received %d records\n",rec_count);
  sqlite3_finalize(res);   
  return 0;
}

int main(int argc, char **argv){
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  float **Node;
  int i;
  sqlite3_stmt *res;
  const char *tail;
  int rec_count;
  struct DecayNode RootDecayNode[MAXROOTS];
  struct DecaySum DecayList[MAXDECAYS];
  struct DecaySum *decay_ptr;
  char DecayListName[MAXDECAYS][31];
  int numdecays;
  int numleaves;
  struct DecayNode *LeafList[MAXDECAYS];
  struct DecayNode *node_ptr;
  int j;
  float BR_list[MAXDECAYS];
  float DR_list[MAXDECAYS];
  struct DecaySum ID_list[MAXDECAYS];
  int nlvls;
  double *tlist;
  double *ActMatrix;
  double N0;
  int t;
  double scale;
  double *UniqueActMatrix;
  char root_nucid[MAXROOTS][7];
  float root_e[MAXROOTS];
  float root_i[MAXROOTS];
  float root_n0[MAXROOTS];
  FILE *infile;
  int numroots;
  char root_str[40];
  int pidx;
  char tstr[100];
  int tidx;
  int didx;
  char ofname[100];
  FILE *ofile;
  float decay_nr;
  float decay_br;
  float decay_bratio;
  float qval;
  float norm_fact;
  char select_str[255];
  char brtxt[40];
  char nrtxt[40];
  int decay_type;
  char beta_nucid[10];
  char gamma_nucid[10];
  float level_e;
  float beta_ib;
  float gamma_ri;
  float gamma_e;
  float child_e_level;
  int child_xflag_level;
  int iso_idx;
  char MSstr[3];
  char zname[100];
  FILE *decay_file;
  char dfname[100];
  float t0;
  float tf;
  int tnum;
  float delt;

  if (argc != 3) {
    printf("esndfspec [input file name] [output file name]");
    return(1);
  }
  
  rc = sqlite3_open("ensdfdb", &db);
  if( rc ){  
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return(1);
  }

  infile = fopen(argv[1],"r");
  numroots = 0;
  fscanf(infile,"%f %f %d",&t0,&tf,&tnum);
  delt = (tf-t0)/((float)(tnum));
  while (!feof(infile)) {
    fgets(root_nucid[numroots],6,infile);
    if (strlen(root_nucid[numroots]) > 2) {
       fgets(root_str,30,infile);
       sscanf(root_str,"%f %f",&(root_i[numroots]),&(root_n0[numroots]));
       printf("'%s' %e %e\n",root_nucid[numroots],root_i[numroots],root_n0[numroots]);
       if (root_i[numroots] == 0.0) {
	 printf("ISOMER ZERO\n");
	 root_e[numroots] = 0.0;
       } else {
	 if (root_i[numroots] == 1.0) {
	   strcpy(MSstr,"M1");
	 }
	 if (root_i[numroots] == 2.0) {
	   strcpy(MSstr,"M2");
	 }
	 sprintf(select_str,"SELECT E FROM LEVEL WHERE NUCID == '%s' AND MS == '%s' AND ISOMER_IDX > 0;",root_nucid[numroots],MSstr);
         printf("%s\n",select_str);
         rc = sqlite3_prepare_v2(db, select_str, 1000, &res, &tail);
         while (sqlite3_step(res) == SQLITE_ROW) {  
    	   root_e[numroots] = sqlite3_column_double(res,0);
	   printf("ISOMER = %s %s %f\n",root_nucid[numroots],MSstr,root_e[numroots]);
         }
         sqlite3_finalize(res);
       }
       numroots++;
    }
  }
  fclose(infile);

  for (pidx=0; pidx < numroots; pidx++) {
  
    RootDecayNode[pidx].node_lvl = 0;
    RootDecayNode[pidx].dr = -1.0; /* decay rate in 1/secs = ln(2)/t */
    RootDecayNode[pidx].br = 1.0; /* branching ratio */
    strcpy(RootDecayNode[pidx].parent_nucid," ROOT");
    strcpy(RootDecayNode[pidx].child_nucid,root_nucid[pidx]);
    strcpy(RootDecayNode[pidx].parent_estr,"0.0");
    sprintf(RootDecayNode[pidx].child_estr,"%.4f",root_e[pidx]);
    RootDecayNode[pidx].parent_xflag = 0;
    RootDecayNode[pidx].child_xflag = 0;
    RootDecayNode[pidx].parent = NULL;
    RootDecayNode[pidx].num_children = 0;

    BranchDecayNode(db,&(RootDecayNode[pidx]));

    TraverseTree(&(RootDecayNode[pidx]),argv[2],zname);

  }
  
  numdecays = 0;

  for (pidx=0; pidx<numroots; pidx++) {
     BuildDecayList(db,&(RootDecayNode[pidx]),DecayList,&numdecays);
  }

  qsort(DecayList, numdecays, sizeof(struct DecaySum), compar);

  for (i=0; i<numdecays; i++) {
    printf("DecayList[%d] = %d\n",i,DecayList[i].id_idx);
    DecayList[i].sum_idx = i;
  }

  tlist = (double *)malloc(tnum*sizeof(double));
  for (i=0; i<tnum; i++) {
    tlist[i] = ((float)(i))*delt+t0;
  }

  UniqueActMatrix = (double *)malloc(tnum*numdecays*sizeof(double));
  for (i=0; i<numdecays; i++) {
    for (j=0; j<tnum; j++) {
      UniqueActMatrix[numdecays*j+i] = 0.0;
    }
  }

  for (pidx=0; pidx<numroots;pidx++) {
    numleaves = 0;
    CreateLeafList(&(RootDecayNode[pidx]),LeafList,&numleaves);
  
    for (i=0; i<numleaves; i++) {
      printf("leaf[%d] = %s %s %d\n",i,LeafList[i]->parent_nucid,LeafList[i]->child_nucid,LeafList[i]->node_lvl);
      nlvls = LeafList[i]->node_lvl;
      node_ptr = LeafList[i];
      for (j=nlvls-1;j>=0;j--) {
	BR_list[j] = node_ptr->br;
	DR_list[j] = node_ptr->dr;
	ID_list[j].id_idx = node_ptr->id_idx;
	ID_list[j].sum = 0.0;
	sprintf(ID_list[j].estr,"%s",node_ptr->child_estr);
	printf("node pnucid cnucid = %s %s %d %f %f %d\n",node_ptr->parent_nucid,node_ptr->child_nucid,ID_list[j].id_idx,BR_list[j],DR_list[j],j);
	node_ptr = node_ptr->parent;
      }
      ActMatrix = (double *)malloc(tnum*nlvls*sizeof(double));

      for (j=0; j<nlvls; j++) {
	printf("id_idx %d\n",ID_list[j].id_idx);
      }

      BatemanSolver(root_n0[pidx],tnum,tlist,nlvls,BR_list,DR_list,ActMatrix);
      for (t=0; t<tnum; t++) {
	for (j=0; j<nlvls; j++) {
	  decay_ptr = (struct DecaySum *)bsearch(&(ID_list[j]),&DecayList, numdecays, sizeof(struct DecaySum), compar);
	  if (decay_ptr == NULL) {
	    printf("Decay ptr NULL\n");
	  } else {
	    printf("%d %e %e %e %d %d %d\n",ID_list[j].id_idx,BR_list[j],DR_list[j],ActMatrix[t*nlvls+j],decay_ptr->id_idx,decay_ptr->sum_idx,numdecays);	  
	    UniqueActMatrix[t*numdecays+decay_ptr->sum_idx] += ActMatrix[t*nlvls+j];
	  }
	}
      }
      free(ActMatrix);
    }
  }

  sprintf(dfname,"%s.ult",argv[2]);
  decay_file = fopen(dfname,"w");
  for (j=0; j<numdecays; j++) {
    fprintf(decay_file,"# %s %s vs. time (s)\n",DecayList[j].dname,DecayList[j].dsid);
    for (t=0; t<tnum; t++) {
      fprintf(decay_file," %e %e\n",tlist[t],UniqueActMatrix[t*numdecays+j]);
    }
    fprintf(decay_file,"end\n");
  }
  fclose(decay_file);

  for (tidx=0; tidx<tnum; tidx++) {
    if (tidx < 10) {
      sprintf(tstr,"000%d",tidx);
    } else if (tidx < 100) {
      sprintf(tstr,"00%d",tidx);
    } else if (tidx < 1000) {
      sprintf(tstr,"0%d",tidx);
    } else {
      sprintf(tstr,"%d",tidx);
    }
    sprintf(ofname,"%s.%s",argv[2],tstr);
    ofile = fopen(ofname,"w");
    fprintf(ofile,"# t = %f seconds\n",tlist[tidx]);
    for (didx=0; didx<numdecays; didx++) {
      norm_fact = UniqueActMatrix[tidx*numdecays+didx];
      child_e_level = strtof(DecayList[didx].estr,NULL);
      child_xflag_level = DecayList[didx].xflag;
      fprintf(ofile,"# %s %s\n",DecayList[didx].dname,DecayList[didx].dsid);
      sprintf(select_str,"SELECT NORMALIZATION.BR,NORMALIZATION.NR,PARENT.QP FROM NORMALIZATION JOIN PARENT ON NORMALIZATION.ID_IDX == %d AND PARENT.ID_IDX == %d;",DecayList[didx].id_idx,DecayList[didx].id_idx);
      printf("%s\n",select_str);
      rc = sqlite3_prepare_v2(db, select_str, 1000, &res, &tail);
      while (sqlite3_step(res) == SQLITE_ROW) {
	sprintf(brtxt,"%s",sqlite3_column_text(res,0));
	sprintf(nrtxt,"%s",sqlite3_column_text(res,1));
	printf("%s %s\n",brtxt,nrtxt);
	decay_br = strtof(brtxt,NULL);
	decay_nr = strtof(nrtxt,NULL);
	qval = sqlite3_column_double(res,2); 
      }
      sqlite3_finalize(res);
       if (decay_nr == 0.0) {
	decay_nr = 1.0;
      }
      if (decay_br == 0.0) {
	decay_br = 1.0;
      }
      decay_type = GetDecayType(DecayList[didx].dsid);
      if (child_e_level == 0.0 && child_xflag_level == 0) {
         sprintf(select_str,"SELECT BETA.NUCID,BETA.IB,LEVEL.E,LEVEL.ISOMER_IDX FROM BETA JOIN LEVEL ON BETA.LEVEL_IDX == LEVEL.LEVEL_IDX AND BETA.ID_IDX == %d AND LEVEL.ID_IDX == %d;",DecayList[didx].id_idx,DecayList[didx].id_idx);
         printf("%s\n",select_str);
         rc = sqlite3_prepare_v2(db, select_str, 1000, &res, &tail);
         while (sqlite3_step(res) == SQLITE_ROW) {  
    	   sprintf(beta_nucid,"%s",sqlite3_column_text(res,0));
	   beta_ib = sqlite3_column_double(res,1);
	   level_e = sqlite3_column_double(res,2);
	   iso_idx = sqlite3_column_int(res,3);
	   printf("%s %e %e\n",beta_nucid,DecayList[didx].dr,DecayList[didx].br);
	   if (beta_ib > 0.0 && iso_idx == 0) {
	     fprintf(ofile,"B- %s ZZZ %.5e %.5e %.5e %.5e %.5e %.5e %.5e\n",beta_nucid,level_e,qval,norm_fact,DecayList[didx].dr,beta_ib,decay_br,DecayList[didx].br);
	   }
         }
         sqlite3_finalize(res);
         sprintf(select_str,"SELECT EC.NUCID,EC.IB,LEVEL.E,LEVEL.ISOMER_IDX FROM EC JOIN LEVEL ON EC.LEVEL_IDX == LEVEL.LEVEL_IDX AND EC.ID_IDX == %d AND LEVEL.ID_IDX == %d;",DecayList[didx].id_idx,DecayList[didx].id_idx);
         printf("%s\n",select_str);
         rc = sqlite3_prepare_v2(db, select_str, 1000, &res, &tail);
         while (sqlite3_step(res) == SQLITE_ROW) {  
    	   sprintf(beta_nucid,"%s",sqlite3_column_text(res,0));
	   beta_ib = sqlite3_column_double(res,1);
	   level_e = sqlite3_column_double(res,2);
	   iso_idx = sqlite3_column_int(res,3);
	   printf("%s %e %e\n",beta_nucid,DecayList[didx].dr,DecayList[didx].br);
	   if (beta_ib > 0.0 && iso_idx == 0) {
	     fprintf(ofile,"EC %s ZZZ %.5e %.5e %.5e %.5e %.5e %.5e %.5e\n",beta_nucid,level_e,qval,norm_fact,DecayList[didx].dr,beta_ib,decay_br,DecayList[didx].br);
	   }
         }
         sqlite3_finalize(res);
	 sprintf(select_str,"SELECT NUCID,RI,E FROM GAMMA WHERE ID_IDX == %d;",DecayList[didx].id_idx);
         printf("%s\n",select_str);
         rc = sqlite3_prepare_v2(db, select_str, 1000, &res, &tail);
         while (sqlite3_step(res) == SQLITE_ROW) {  
    	   sprintf(gamma_nucid,"%s",sqlite3_column_text(res,0));
	   gamma_ri = sqlite3_column_double(res,1);
	   gamma_e = sqlite3_column_double(res,2);
	   printf("%s %e %e\n",gamma_nucid,DecayList[didx].dr,DecayList[didx].br);
	   if (gamma_ri > 0.0) {
	     fprintf(ofile,"G %s %.5e %.5e %.5e %.5e %.5e %.5e\n",gamma_nucid,gamma_e,norm_fact,DecayList[didx].dr,gamma_ri,decay_br,DecayList[didx].br);
	   }
         }
         sqlite3_finalize(res);
      } else {
         sprintf(select_str,"SELECT BETA.NUCID,BETA.IB,LEVEL.E FROM BETA JOIN LEVEL ON BETA.LEVEL_IDX == LEVEL.LEVEL_IDX AND BETA.ID_IDX == %d AND LEVEL.ID_IDX == %d;",DecayList[didx].id_idx,DecayList[didx].id_idx);
         printf("%s\n",select_str);
         rc = sqlite3_prepare_v2(db, select_str, 1000, &res, &tail);
         while (sqlite3_step(res) == SQLITE_ROW) {  
    	   sprintf(beta_nucid,"%s",sqlite3_column_text(res,0));
	   beta_ib = sqlite3_column_double(res,1);
	   level_e = sqlite3_column_double(res,2);
	   printf("%s %e %e\n",beta_nucid,DecayList[didx].dr,DecayList[didx].br);
	   if (beta_ib > 0.0 && level_e == child_e_level && child_e_level > 0.0) {
	     fprintf(ofile,"B- %s ZZZ %.5e %.5e %.5e %.5e %.5e %.5e %.5e\n",beta_nucid,level_e,qval,norm_fact,DecayList[didx].dr,beta_ib,decay_br,DecayList[didx].br);
	   }
         }
         sqlite3_finalize(res);
         sprintf(select_str,"SELECT EC.NUCID,EC.IB,LEVEL.E FROM EC JOIN LEVEL ON BETA.LEVEL_IDX == LEVEL.LEVEL_IDX AND EC.ID_IDX == %d AND LEVEL.ID_IDX == %d;",DecayList[didx].id_idx,DecayList[didx].id_idx);
         printf("%s\n",select_str);
         rc = sqlite3_prepare_v2(db, select_str, 1000, &res, &tail);
         while (sqlite3_step(res) == SQLITE_ROW) {  
    	   sprintf(beta_nucid,"%s",sqlite3_column_text(res,0));
	   beta_ib = sqlite3_column_double(res,1);
	   level_e = sqlite3_column_double(res,2);
	   printf("%s %e %e\n",beta_nucid,DecayList[didx].dr,DecayList[didx].br);
	   if (beta_ib > 0.0 && level_e == child_e_level && child_e_level > 0.0) {
	     fprintf(ofile,"EC %s ZZZ %.5e %.5e %.5e %.5e %.5e %.5e %.5e\n",beta_nucid,level_e,qval,norm_fact,DecayList[didx].dr,beta_ib,decay_br,DecayList[didx].br);
	   }
         }
         sqlite3_finalize(res);
      }
    }
  }
  
  sqlite3_close(db);
  return 0;
}

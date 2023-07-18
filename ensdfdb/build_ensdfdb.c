#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <dirent.h>
#include <string.h>

#define INCLUDE_COMMENTS 0
#define INCLUDE_LEVELS 1

#define ID_READ_DEBUG 0
#define H_READ_DEBUG 0
#define Q_READ_DEBUG 0
#define CROSSREF_READ_DEBUG 0
#define COMMENT_READ_DEBUG 0
#define PARENT_READ_DEBUG 0
#define NORMAL_READ_DEBUG 0
#define PRODNORMAL_READ_DEBUG 0
#define LEVEL_READ_DEBUG 0
#define BETA_READ_DEBUG 0
#define EC_READ_DEBUG 0
#define ALPHA_READ_DEBUG 0
#define DELAYED_READ_DEBUG 0
#define GAMMA_READ_DEBUG 0

#define MAXNUM 100000

typedef unsigned int uint_type;

struct id_record {
  char NUCID[6];
  char DSID[32];
  char DSREF[27];
  char PUB[10];
  char DATE[7];
};

struct history_record {
  char NUCID[6];
  char History[72];  
};

struct qvalue_record {
  char NUCID[6];
  char QMINUS[11];
  char DQMINUS[3];
  char SN[9];
  char DSN[3];
  char SP[9];
  char DSP[3];
  char QA[9];
  char DQA[7];
  char QREF[26];
};

struct crossref_record {
  char NUCID[6];
  char DSSYM[2];
  char DSID[31];
};

struct comment_record {
  char NUCID[6];
  char C[2];
  char RTYPE[2];
  char PSYM[2];
  char CTEXT[72];
};

struct parent_record {
  char NUCID[6];
  char PNUM[2];
  char E[11];
  char DE[3];
  char J[19];
  char T[11];
  char DT[7];
  char QP[11];
  char DQP[3];
  char ION[5];
  int XFLAG;
};

struct normalization_record {
  char NUCID[6];
  char PNUM[2];
  char NR[11];
  char DNR[3];
  char NT[9];
  char DNT[3];
  char BR[9];
  char DBR[3];
  char NB[9];
  char DNB[7];
  char NP[8];
  char DNP[3];
};

struct prodnormal_record {
  char NUCID[6];
  char NRXBR[11];
  char NRXBRUNC[3];
  char NTXBR[9];
  char NTXBRUNC[3];
  char NBXBR[9];
  char NBXBRUNC[7];
  char NP[8];
  char NPUNC[3];
  char COM[2];
  char OPT[2];
};

struct level_record {
  char NUCID[6];
  char E[11];
  char DE[3];
  char J[19];
  char T[9];
  char DT[7];
  char L[10];
  char S[11];
  char DS[3];
  char C[2];
  char MS[3];
  char Q[2];
  int XFLAG;
};

struct beta_record {
  char NUCID[6];
  char E[11];
  char DE[3];
  char IB[9];
  char DIB[3];
  char LOGFT[9];
  char DFT[7];
  char C[2];
  char UN[3];
  char Q[2];
};

struct ec_record {
  char NUCID[6];
  char E[11];
  char DE[3];
  char IB[9];
  char DIB[3];
  char IE[9];
  char DIE[3];
  char LOGFT[9];
  char DFT[7];
  char TI[11];
  char DTI[3];
  char C[2];
  char UN[3];
  char Q[2];
};

struct alpha_record {
  char NUCID[6];
  char E[11];
  char DE[3];
  char IA[9];
  char DIA[3];
  char HF[9];
  char DHF[3];
  char C[2];
  char Q[2];
};

struct delayed_record {
  char NUCID[6];
  char Particle[2];
  char E[11];
  char DE[3];
  char IP[9];
  char DIP[3];
  char EI[9];
  char T[11];
  char DT[7];
  char L[10];
  char C[2];
  char COIN[2];
  char Q[2];
};

struct gamma_record {
  char NUCID[6];
  char E[11];
  char DE[3];
  char RI[9];
  char DRI[3];
  char M[11];
  char MR[9];
  char DMR[7];
  char CC[8];
  char DCC[3];
  char TI[11];
  char DTI[3];
  char C[2];
  char COIN[2];
  char Q[2];
};

void RemoveSpaces(char* s) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while ((*s++ = *d++));
}

float TtoSec(char *t) {
  if (strstr(t,"MS") != NULL) {
    return 1e-3*strtof(t,NULL);
  } else if (strstr(t,"US") != NULL) {
    return 1e-6*strtof(t,NULL);
  } else if (strstr(t,"NS") != NULL) {
    return 1e-9*strtof(t,NULL);
  } else if (strstr(t,"PS") != NULL) {
    return 1e-12*strtof(t,NULL);
  } else if (strstr(t,"FS") != NULL) {
    return 1e-15*strtof(t,NULL);
  } else if (strstr(t,"AS") != NULL) {
    return 1e-18*strtof(t,NULL);
  } else if (strstr(t,"Y") != NULL) {
    return 86400.0*365.0*strtof(t,NULL);
  } else if (strstr(t,"D") != NULL) {
    return 86400.0*strtof(t,NULL);
  } else if (strstr(t,"H") != NULL) {
    return 3600.0*strtof(t,NULL);
  } else if (strstr(t,"M") != NULL) {
    return 60.0*strtof(t,NULL);
  } else if (strstr(t,"S") != NULL) {
    return strtof(t,NULL);
  } else {
    return 0;
  }
}
      
static int RemoveApostrophes(char *str) {
  char *cidx;

  cidx = strchr(str,'\"');
  while (cidx != NULL) {
    cidx[0] = ' ';
    cidx = strchr(str,'\"');
  }
  return 0;
}
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

int CopySubStr(char *dstr,char *source,uint_type start,uint_type end)
{
  uint_type idx;

  idx = start;
  while (idx <= end) {
    dstr[idx-start] = source[idx-1];
    idx = idx + 1;
  }
  dstr[end-start+1] = '\0';
  return 1;
}

int FillENSDFDB(sqlite3 *db) 
{
  struct dirent *entry;
  DIR *dp;
  FILE *infile;
  char fnamestr[255];
  char textline[127];
  char insert_str[512];
  uint_type idx;
  uint_type last_id_idx;
  uint_type last_level_idx;
  uint_type last_comment_idx;
  uint_type last_norm_idx;
  struct id_record id;
  struct history_record hist;
  struct qvalue_record qvalue;
  struct crossref_record crossref;
  struct comment_record comment;
  struct parent_record parent;
  struct normalization_record normal;
  struct prodnormal_record pnormal;
  struct level_record level;
  struct beta_record beta;
  struct ec_record ec;
  struct alpha_record alpha;
  struct delayed_record delayed;
  struct gamma_record gamma;
  char *zErrMsg = 0;
  int rc;
  sqlite3_stmt *res;
  sqlite3_stmt *res_iso;
  const char *tail;
  int rec_count;
  char parent_nucid_list[MAXNUM][6];
  char parent_e_list[MAXNUM][12];
  int parent_xflag_list[MAXNUM];
  int parent_idx_list[MAXNUM];
  int parent_cnt;
  char select_str[255];
  char select_str_iso[255];
  int jj;
  uint_type isomer_level_idx;
  
  parent_cnt = 0;
  last_level_idx = 0;
  
  dp = opendir("/Users/kevinkramer/ensdfdb/ensdf");
  if (dp == NULL) 
  {
    perror("opendir");
    return -1;
  }
  idx = 0;
  last_comment_idx = 0;
  while((entry = readdir(dp))) {
    if (entry->d_name[0] != '.') {
       sprintf(fnamestr, "/Users/kevinkramer/ensdfdb/ensdf/%s", entry->d_name);
        infile = fopen(fnamestr,"r");
       if (infile == NULL) {
         perror("Failed to open file");
         return -1;
       }
       fgets(textline,127,infile);
       while(!feof(infile)) {
         if (strlen(textline) > 2) {
	   /* Identification Record */
	   if (!strncmp(&(textline[5]),"    ",4)) {
	     last_id_idx = idx;
	     CopySubStr(id.NUCID,textline,1,5);
	     CopySubStr(id.DSID,textline,10,39);
	     CopySubStr(id.DSREF,textline,40,65);
	     CopySubStr(id.PUB,textline,66,74);
	     CopySubStr(id.DATE,textline,75,80);
 	     if (ID_READ_DEBUG == 1 && !strncmp(id.NUCID," 64NI",5)) { 
	       printf("nucid = %s\n",id.NUCID);
	       printf("dsid = %s\n",id.DSID);
	       printf("dsref = %s\n",id.DSREF);
	       printf("pub = %s\n",id.PUB);
	       printf("date = %s\n",id.DATE);
	     }
	     sprintf(insert_str,"INSERT INTO ID VALUES (%d, \"%s\", \"%s\", \"%s\", \"%s\", \"%s\");",idx,id.NUCID,id.DSID,id.DSREF,id.PUB,id.DATE);
      	     rc = sqlite3_exec(db, insert_str, callback, 0, &zErrMsg);
             if( rc!=SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
	     } 
	   }
	   /* History Record */
	   if (!strncmp(&(textline[5]),"  H ",4)) {
	     CopySubStr(hist.NUCID,textline,1,5);
	     CopySubStr(hist.History,textline,10,80);
	     if (H_READ_DEBUG == 1 && !strncmp(hist.NUCID," 64NI",5)) { 
	       printf("nucid = %s\n",hist.NUCID);
	       printf("history = %s\n",hist.History);
	     }
	     RemoveApostrophes(hist.History);
	     sprintf(insert_str,"INSERT INTO HISTORY VALUES (%d,\"%s\",\"%s\",%d);",idx, hist.NUCID, hist.History, last_id_idx); 
	     rc = sqlite3_exec(db, insert_str, callback, 0, &zErrMsg); 
	     if( rc!=SQLITE_OK ){
	       fprintf(stderr, "SQL error: %s %d\n", zErrMsg,idx);
	       sqlite3_free(zErrMsg);
	     }
	   }
	   /* Q-value Record */
	   if (!strncmp(&(textline[5]),"  Q ",4)) {
	     CopySubStr(qvalue.NUCID,textline,1,5);
	     CopySubStr(qvalue.QMINUS,textline,10,19);
	     CopySubStr(qvalue.DQMINUS,textline,20,21);
	     CopySubStr(qvalue.SN,textline,22,29);
	     CopySubStr(qvalue.DSN,textline,30,31);
	     CopySubStr(qvalue.SP,textline,32,39);
	     CopySubStr(qvalue.DSP,textline,40,41);
	     CopySubStr(qvalue.QA,textline,42,49);
	     CopySubStr(qvalue.DQA,textline,50,55);
	     CopySubStr(qvalue.QREF,textline,56,80);	     
	     if (Q_READ_DEBUG == 1 && !strncmp(qvalue.NUCID," 64NI",5)) { 
               printf("nucid = %s\n",qvalue.NUCID);
	       printf("qminus = %s\n",qvalue.QMINUS);
	       printf("dqminus = %s\n",qvalue.DQMINUS);
	       printf("sn = %s\n",qvalue.SN);
	       printf("dsn = %s\n",qvalue.DSN);
	       printf("sp = %s\n",qvalue.SP);
	       printf("dsp = %s\n",qvalue.DSP);
	       printf("qa = %s\n",qvalue.QA);
	       printf("dqa = %s\n",qvalue.DQA);
	       printf("qref = %s\n",qvalue.QREF);
	     }
	     sprintf(insert_str,"INSERT INTO QVALUE VALUES (%d,\"%s\",%f,%f,%f,%f,%f,%f,%f,%f,\"%s\",%d);",idx,qvalue.NUCID,strtof(qvalue.QMINUS,NULL),strtof(qvalue.DQMINUS,NULL),strtof(qvalue.SN,NULL),strtof(qvalue.DSN,NULL),strtof(qvalue.SP,NULL),strtof(qvalue.DSP,NULL),strtof(qvalue.QA,NULL),strtof(qvalue.DQA,NULL),qvalue.QREF,last_id_idx); 
	     rc = sqlite3_exec(db, insert_str, callback, 0, &zErrMsg); 
	     if( rc!=SQLITE_OK ){
	       fprintf(stderr, "SQL error: %s %d\n", zErrMsg,idx);
	       sqlite3_free(zErrMsg);
	     }
	   }
	   /* Cross Ref Record */
	   if (!strncmp(&(textline[5]),"  X",3)) {
	     CopySubStr(crossref.NUCID,textline,1,5);
	     CopySubStr(crossref.DSSYM,textline,9,9);
	     CopySubStr(crossref.DSID,textline,10,39);
	     if (CROSSREF_READ_DEBUG == 1 && !strncmp(qvalue.NUCID," 64NI",5)) { 
               printf("nucid = %s\n",crossref.NUCID);
	       printf("dssym = %s\n",crossref.DSSYM);
	       printf("dsid = %s\n",crossref.DSID);
	     }
	     sprintf(insert_str,"INSERT INTO CROSSREF VALUES (%d,\"%s\",\"%s\",\"%s\",%d);",idx,crossref.NUCID,crossref.DSSYM,crossref.DSID,last_id_idx); 
	     rc = sqlite3_exec(db, insert_str, callback, 0, &zErrMsg); 
	     if( rc!=SQLITE_OK ){
	       fprintf(stderr, "SQL error: %s %d\n", zErrMsg,idx);
	       sqlite3_free(zErrMsg);
	     }
	   }
	   if (INCLUDE_COMMENTS) {
	     /* Comment Record */
	     if (!strncmp(&(textline[6]),"C",1) ||
		 !strncmp(&(textline[6]),"c",1) ||
		 !strncmp(&(textline[6]),"T",1) ||
		 !strncmp(&(textline[6]),"D",1) ||
		 !strncmp(&(textline[6]),"t",1)) {
	       if (textline[5] == ' ') {
		 last_comment_idx = 0;
	       }
	       CopySubStr(comment.NUCID,textline,1,5);
	       CopySubStr(comment.C,textline,7,7);
	       CopySubStr(comment.RTYPE,textline,8,8);
	       CopySubStr(comment.PSYM,textline,9,9);
	       CopySubStr(comment.CTEXT,textline,10,80);
	       if (COMMENT_READ_DEBUG == 1 && !strncmp(comment.NUCID," 64NI",5)) { 
		 printf("nucid = %s\n",comment.NUCID);
		 printf("c = %s\n",comment.C);
		 printf("rtype = %s\n",comment.RTYPE);
		 printf("psym = %s\n",comment.PSYM);
		 printf("ctext = %s\n",comment.CTEXT);
	       }
	       RemoveApostrophes(comment.CTEXT);
	       sprintf(insert_str,"INSERT INTO COMMENT VALUES (%d,\"%s\",\"%s\",\"%s\",\"%s\",%d,%d);",idx,comment.NUCID,comment.C,comment.RTYPE,comment.CTEXT,last_comment_idx,last_id_idx); 
	       rc = sqlite3_exec(db, insert_str, callback, 0, &zErrMsg); 
	       if( rc!=SQLITE_OK ){
		 fprintf(stderr, "SQL error: %s %d\n", zErrMsg,idx);
		 sqlite3_free(zErrMsg);
	       }
	       last_comment_idx = idx;
	     }
	   }
	   /* Parent Record */
	   if (!strncmp(&(textline[5]),"  P",3)) {
	     CopySubStr(parent.NUCID,textline,1,5);
	     CopySubStr(parent.PNUM,textline,9,9);
	     CopySubStr(parent.E,textline,10,19);
	     CopySubStr(parent.DE,textline,20,21);
	     CopySubStr(parent.J,textline,22,39);
	     CopySubStr(parent.T,textline,40,49);
	     CopySubStr(parent.DT,textline,50,55);
	     CopySubStr(parent.QP,textline,65,74);
	     CopySubStr(parent.DQP,textline,75,76);
	     CopySubStr(parent.ION,textline,77,80);
	     if (PARENT_READ_DEBUG == 1 && !strncmp(qvalue.NUCID," 64NI",5)) { 
               printf("nucid = %s\n",parent.NUCID);
	       printf("pnum = %s\n",parent.PNUM);
	       printf("e = %s\n",parent.E);
	       printf("de = %s\n",parent.DE);
	       printf("j = %s\n",parent.J);
	       printf("t = %s\n",parent.T);
	       printf("dt = %s\n",parent.DT);
	       printf("qp = %s\n",parent.QP);
	       printf("dqp = %s\n",parent.DQP);
	       printf("ion = %s\n",parent.ION);
	     }
	     RemoveSpaces(parent.E);
	     if (strstr(parent.E,"+X") != NULL) {
	       parent.XFLAG = 1;
	     } else {
	       parent.XFLAG = 0;
	     }
	     if (strstr(parent.E,"+Y") != NULL) {
	       parent.XFLAG = parent.XFLAG + 1;
	     }
	     if (strstr(parent.ION,"+") != NULL) {
	       parent.XFLAG = parent.XFLAG + 5;
	     }
             sprintf(insert_str,"INSERT INTO PARENT VALUES (%d,\"%s\",\"%s\",\"%s\",%f,%f,\"%s\",%f,%f,%f,%f,\"%s\",%d,%d);",idx,parent.NUCID,parent.PNUM,parent.E,strtof(parent.E,NULL),strtof(parent.DE,NULL),parent.J,TtoSec(parent.T),strtof(parent.DT,NULL),strtof(parent.QP,NULL),strtof(parent.DQP,NULL),parent.ION,parent.XFLAG,last_id_idx);
	     rc = sqlite3_exec(db, insert_str, callback, 0, &zErrMsg); 
	     if( rc!=SQLITE_OK ){
	       fprintf(stderr, "SQL error: %s %d\n", zErrMsg,idx);
	       sqlite3_free(zErrMsg);
	     }
             strcpy(parent_nucid_list[parent_cnt],parent.NUCID);
             strcpy(parent_e_list[parent_cnt],parent.E);
	     parent_xflag_list[parent_cnt] = parent.XFLAG;
             parent_idx_list[parent_cnt] = idx;
	     parent_cnt = parent_cnt + 1;
	   }
	   /* Normalization Record */
	   if (!strncmp(&(textline[5]),"  N",3)) {
	     CopySubStr(normal.NUCID,textline,1,5);
	     CopySubStr(normal.PNUM,textline,9,9);
	     CopySubStr(normal.NR,textline,10,19);
	     CopySubStr(normal.DNR,textline,20,21);
	     CopySubStr(normal.NT,textline,22,29);
	     CopySubStr(normal.DNT,textline,30,31);
	     CopySubStr(normal.BR,textline,32,39);
	     CopySubStr(normal.DBR,textline,40,41);
	     CopySubStr(normal.NB,textline,42,49);
	     CopySubStr(normal.DNB,textline,50,55);
	     CopySubStr(normal.NP,textline,56,62);
	     CopySubStr(normal.DNP,textline,63,64);
	     if (NORMAL_READ_DEBUG == 1 && !strncmp(qvalue.NUCID," 64NI",5)) { 
               printf("nucid = %s\n",normal.NUCID);
	       printf("pnum = %s\n",normal.PNUM);
	       printf("nr = %s\n",normal.NR);
	       printf("dnr = %s\n",normal.DNR);
	       printf("nt = %s\n",normal.NT);
	       printf("dnt = %s\n",normal.DNT);
	       printf("br = %s\n",normal.BR);
	       printf("dbr = %s\n",normal.DBR);
	       printf("nb = %s\n",normal.NB);
	       printf("dnb = %s\n",normal.DNB);
	       printf("np = %s\n",normal.NP);
	       printf("dnp = %s\n",normal.DNP);
	     }
	     last_norm_idx = idx;
	     sprintf(insert_str,"INSERT INTO NORMALIZATION VALUES (%d,\"%s\",\"%s\",%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d);",idx,normal.NUCID,normal.PNUM,strtof(normal.NR,NULL),strtof(normal.DNR,NULL),strtof(normal.NT,NULL),strtof(normal.DNT,NULL),strtof(normal.BR,NULL),strtof(normal.DBR,NULL),strtof(normal.NB,NULL),strtof(normal.DNB,NULL),strtof(normal.NP,NULL),strtof(normal.DNP,NULL),last_id_idx);
	     rc = sqlite3_exec(db, insert_str, callback, 0, &zErrMsg); 
	     if( rc!=SQLITE_OK ){
	       fprintf(stderr, "SQL error: %s %d\n", zErrMsg,idx);
	       sqlite3_free(zErrMsg);
	     }
	   }
	   /* Production Normalization Record */
	   if (!strncmp(&(textline[5])," PN ",4)) {
	     CopySubStr(pnormal.NUCID,textline,1,5);
	     CopySubStr(pnormal.NRXBR,textline,10,19);
	     CopySubStr(pnormal.NRXBRUNC,textline,20,21);
	     CopySubStr(pnormal.NTXBR,textline,22,29);
	     CopySubStr(pnormal.NTXBRUNC,textline,30,31);
	     CopySubStr(pnormal.NBXBR,textline,42,49);
	     CopySubStr(pnormal.NBXBRUNC,textline,50,55);
	     CopySubStr(pnormal.NP,textline,56,62);
	     CopySubStr(pnormal.NPUNC,textline,63,64);
	     CopySubStr(pnormal.COM,textline,77,77);
	     CopySubStr(pnormal.OPT,textline,78,78);
	     if (PRODNORMAL_READ_DEBUG == 1 && !strncmp(qvalue.NUCID," 99MO",5)) { 
               printf("nucid = %s\n",pnormal.NUCID);
	       printf("nrxbr = %s\n",pnormal.NRXBR);
	       printf("nrxbrunc = %s\n",pnormal.NRXBRUNC);
	       printf("ntxbr = %s\n",pnormal.NTXBR);
	       printf("ntxbrunc = %s\n",pnormal.NTXBRUNC);
	       printf("nbxbr = %s\n",pnormal.NBXBR);
	       printf("nbxbrunc = %s\n",pnormal.NBXBRUNC);
	       printf("np = %s\n",pnormal.NP);
	       printf("npunc = %s\n",pnormal.NPUNC);
	       printf("com = %s\n",pnormal.COM);
	       printf("opt = %s\n",pnormal.OPT);
	     }
	     sprintf(insert_str,"INSERT INTO PRODNORMALIZATION VALUES (%d,\"%s\",%f,%f,%f,%f,%f,%f,%f,%f,\"%s\",\"%s\",%d,%d);",idx,pnormal.NUCID,strtof(pnormal.NRXBR,NULL),strtof(pnormal.NRXBRUNC,NULL),strtof(pnormal.NTXBR,NULL),strtof(pnormal.NTXBRUNC,NULL),strtof(pnormal.NBXBR,NULL),strtof(pnormal.NBXBRUNC,NULL),strtof(pnormal.NP,NULL),strtof(pnormal.NPUNC,NULL),pnormal.COM,pnormal.OPT,last_norm_idx,last_id_idx);
	     rc = sqlite3_exec(db, insert_str, callback, 0, &zErrMsg); 
	     if( rc!=SQLITE_OK ){
	       fprintf(stderr, "SQL error: %s %d\n", zErrMsg,idx);
	       sqlite3_free(zErrMsg);
	     }
	   }
	   /* Level Record */
	   if (INCLUDE_LEVELS) {
	     if (!strncmp(&(textline[5]),"  L ",4)) {
	       CopySubStr(level.NUCID,textline,1,5);
	       CopySubStr(level.E,textline,10,19);
	       CopySubStr(level.DE,textline,20,21);
	       CopySubStr(level.J,textline,22,39);
	       CopySubStr(level.T,textline,40,49);
	       CopySubStr(level.DT,textline,50,55);
	       CopySubStr(level.L,textline,56,64);
	       CopySubStr(level.S,textline,65,74);
	       CopySubStr(level.DS,textline,75,76);
	       CopySubStr(level.C,textline,77,77);
	       CopySubStr(level.MS,textline,78,79);
	       CopySubStr(level.Q,textline,80,80);
	       if (LEVEL_READ_DEBUG == 1 && !strncmp(qvalue.NUCID," 99MO",5)) { 
		 printf("nucid = %s\n",level.NUCID);
		 printf("e = %s\n",level.E);
		 printf("de = %s\n",level.DE);
		 printf("j = %s\n",level.J);
		 printf("t = %s\n",level.T);
		 printf("dt = %s\n",level.DT);
		 printf("l = %s\n",level.L);
		 printf("s = %s\n",level.S);
		 printf("ds = %s\n",level.DS);
		 printf("c = %s\n",level.C);
		 printf("ms = %s\n",level.MS);
		 printf("q = %s\n",level.Q);
	       }
	       last_level_idx = idx;
	       RemoveSpaces(level.E);
	       if (strstr(level.E,"+X") != NULL) {
		 level.XFLAG = 1;
	       } else {
		 level.XFLAG = 0;
	       }
	       sprintf(insert_str,"INSERT INTO LEVEL VALUES (%d,\"%s\",\"%s\",%f,%f,\"%s\",%f,%f,%f,%f,%f,\"%s\",\"%s\",\"%s\",%d,%d,%d);",idx,level.NUCID,level.E,strtof(level.E,NULL),strtof(level.DE,NULL),level.J,TtoSec(level.T),strtof(level.DT,NULL),strtof(level.L,NULL),strtof(level.S,NULL),strtof(level.DS,NULL),level.C,level.MS,level.Q,level.XFLAG,0,last_id_idx);
	       rc = sqlite3_exec(db, insert_str, callback, 0, &zErrMsg); 
	       if( rc!=SQLITE_OK ){
		 fprintf(stderr, "SQL error: %s %d\n", zErrMsg,idx);
		 sqlite3_free(zErrMsg);
	       }
	     }
	   }
	   /* Beta Record */
	   if (!strncmp(&(textline[5]),"  B ",4)) {
	     CopySubStr(beta.NUCID,textline,1,5);
	     CopySubStr(beta.E,textline,10,19);
	     CopySubStr(beta.DE,textline,20,21);
	     CopySubStr(beta.IB,textline,22,29);
	     CopySubStr(beta.DIB,textline,30,31);
	     CopySubStr(beta.LOGFT,textline,42,49);
	     CopySubStr(beta.DFT,textline,50,55);
	     CopySubStr(beta.C,textline,77,77);
	     CopySubStr(beta.UN,textline,78,79);
	     CopySubStr(beta.Q,textline,80,80);
	     if (BETA_READ_DEBUG == 1 && !strncmp(qvalue.NUCID," 99MO",5)) { 
               printf("nucid = %s\n",beta.NUCID);
	       printf("e = %s\n",beta.E);
	       printf("de = %s\n",beta.DE);
	       printf("ib = %s\n",beta.IB);
	       printf("dib = %s\n",beta.DIB);
	       printf("logft = %s\n",beta.LOGFT);
	       printf("dft = %s\n",beta.DFT);
	       printf("c = %s\n",beta.C);
	       printf("un = %s\n",beta.UN);
	       printf("q = %s\n",beta.Q);
	     }
	     sprintf(insert_str,"INSERT INTO BETA VALUES (%d,\"%s\",%f,%f,%f,%f,%f,%f,\"%s\",\"%s\",\"%s\",%d,%d);",idx,beta.NUCID,strtof(beta.E,NULL),strtof(beta.DE,NULL),strtof(beta.IB,NULL),strtof(beta.DIB,NULL),strtof(beta.LOGFT,NULL),strtof(beta.DFT,NULL),beta.C,beta.UN,beta.Q,last_level_idx,last_id_idx);
	     rc = sqlite3_exec(db, insert_str, callback, 0, &zErrMsg); 
	     if( rc!=SQLITE_OK ){
	       fprintf(stderr, "SQL error: %s %d\n", zErrMsg,idx);
	       sqlite3_free(zErrMsg);
	     }
	   }
	   /* EC Record */
	   if (!strncmp(&(textline[5]),"  E ",4)) {
	     CopySubStr(ec.NUCID,textline,1,5);
	     CopySubStr(ec.E,textline,10,19);
	     CopySubStr(ec.DE,textline,20,21);
	     CopySubStr(ec.IB,textline,22,29);
	     CopySubStr(ec.DIB,textline,30,31);
	     CopySubStr(ec.IE,textline,32,39);
	     CopySubStr(ec.DIE,textline,40,41);
	     CopySubStr(ec.LOGFT,textline,42,49);
	     CopySubStr(ec.DFT,textline,50,55);
	     CopySubStr(ec.TI,textline,65,74);
	     CopySubStr(ec.DTI,textline,75,76);
	     CopySubStr(ec.C,textline,77,77);
	     CopySubStr(ec.UN,textline,78,79);
	     CopySubStr(ec.Q,textline,80,80);
	     if (EC_READ_DEBUG == 1 && !strncmp(qvalue.NUCID," 64NI",5)) { 
               printf("nucid = %s\n",ec.NUCID);
	       printf("e = %s\n",ec.E);
	       printf("de = %s\n",ec.DE);
	       printf("ib = %s\n",ec.IB);
	       printf("dib = %s\n",ec.DIB);
	       printf("ie = %s\n",ec.IE);
	       printf("die = %s\n",ec.DIE);
	       printf("logft = %s\n",ec.LOGFT);
	       printf("dft = %s\n",ec.DFT);
	       printf("ti = %s\n",ec.TI);
	       printf("dti = %s\n",ec.DTI);
	       printf("c = %s\n",ec.C);
	       printf("un = %s\n",ec.UN);
	       printf("q = %s\n",ec.Q);
	     }
	     sprintf(insert_str,"INSERT INTO EC VALUES (%d,\"%s\",%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,\"%s\",\"%s\",\"%s\",%d,%d);",idx,ec.NUCID,strtof(ec.E,NULL),strtof(ec.DE,NULL),strtof(ec.IB,NULL),strtof(ec.DIB,NULL),strtof(ec.IE,NULL),strtof(ec.DIE,NULL),strtof(ec.LOGFT,NULL),strtof(ec.DFT,NULL),strtof(ec.TI,NULL),strtof(ec.DTI,NULL),ec.C,ec.UN,ec.Q,last_level_idx,last_id_idx);
	     rc = sqlite3_exec(db, insert_str, callback, 0, &zErrMsg); 
	     if( rc!=SQLITE_OK ){
	       fprintf(stderr, "SQL error: %s %d\n", zErrMsg,idx);
	       sqlite3_free(zErrMsg);
	     }
	   }
	   /* Alpha Record */
	   if (!strncmp(&(textline[5]),"  A ",4)) {
	     CopySubStr(alpha.NUCID,textline,1,5);
	     CopySubStr(alpha.E,textline,10,19);
	     CopySubStr(alpha.DE,textline,20,21);
	     CopySubStr(alpha.IA,textline,22,29);
	     CopySubStr(alpha.DIA,textline,30,31);
	     CopySubStr(alpha.HF,textline,32,39);
	     CopySubStr(alpha.DHF,textline,40,41);
	     CopySubStr(alpha.C,textline,77,77);
	     CopySubStr(alpha.Q,textline,80,80);
	     if (ALPHA_READ_DEBUG == 1 && !strncmp(qvalue.NUCID,"222RN",5)) { 
               printf("nucid = %s\n",alpha.NUCID);
	       printf("e = %s\n",alpha.E);
	       printf("de = %s\n",alpha.DE);
	       printf("ia = %s\n",alpha.IA);
	       printf("dia = %s\n",alpha.DIA);
	       printf("hf = %s\n",alpha.HF);
	       printf("dhf = %s\n",alpha.DHF);
	       printf("c = %s\n",alpha.C);
	       printf("q = %s\n",alpha.Q);
	     }
	     sprintf(insert_str,"INSERT INTO ALPHA VALUES (%d,\"%s\",%f,%f,%f,%f,%f,%f,\"%s\",\"%s\",%d,%d);",idx,ec.NUCID,strtof(alpha.E,NULL),strtof(alpha.DE,NULL),strtof(alpha.IA,NULL),strtof(alpha.DIA,NULL),strtof(alpha.HF,NULL),strtof(alpha.DHF,NULL),alpha.C,alpha.Q,last_level_idx,last_id_idx);
	     rc = sqlite3_exec(db, insert_str, callback, 0, &zErrMsg); 
	     if( rc!=SQLITE_OK ){
	       fprintf(stderr, "SQL error: %s %d\n", zErrMsg,idx);
	       sqlite3_free(zErrMsg);
	     }
	   }
	   /* Delayed Record */
	   if (!strncmp(&(textline[5]),"  D",3)) {
	     CopySubStr(delayed.NUCID,textline,1,5);
	     CopySubStr(delayed.Particle,textline,9,9);
	     CopySubStr(delayed.E,textline,10,19);
	     CopySubStr(delayed.DE,textline,20,21);
	     CopySubStr(delayed.IP,textline,22,29);
	     CopySubStr(delayed.DIP,textline,30,31);
	     CopySubStr(delayed.EI,textline,32,39);
	     CopySubStr(delayed.T,textline,40,49);
	     CopySubStr(delayed.DT,textline,50,55);
	     CopySubStr(delayed.L,textline,56,64);
	     CopySubStr(delayed.C,textline,77,77);
	     CopySubStr(delayed.COIN,textline,78,78);
	     CopySubStr(delayed.Q,textline,80,80);
	     if (DELAYED_READ_DEBUG == 1) { 
               printf("nucid = %s\n",delayed.NUCID);
	       printf("particle = %s\n",delayed.Particle);
	       printf("e = %s\n",delayed.E);
	       printf("de = %s\n",delayed.DE);
	       printf("ip = %s\n",delayed.IP);
	       printf("dip = %s\n",delayed.DIP);
	       printf("ei = %s\n",delayed.EI);
	       printf("t = %s\n",delayed.T);
	       printf("dt = %s\n",delayed.DT);
	       printf("l = %s\n",delayed.L);
	       printf("c = %s\n",delayed.C);
	       printf("coin = %s\n",delayed.COIN);
	       printf("q = %s\n",delayed.Q);
	     }
             sprintf(insert_str,"INSERT INTO DELAYED VALUES (%d,\"%s\",\"%s\",%f,%f,%f,%f,%f,%f,%f,%f,\"%s\",\"%s\",\"%s\",%d,%d);",idx,delayed.NUCID,delayed.Particle,strtof(delayed.E,NULL),strtof(delayed.DE,NULL),strtof(delayed.IP,NULL),strtof(delayed.DIP,NULL),strtof(delayed.EI,NULL),strtof(delayed.T,NULL),strtof(delayed.DT,NULL),strtof(delayed.L,NULL),delayed.C,delayed.COIN,delayed.Q,last_level_idx,last_id_idx);
	     rc = sqlite3_exec(db, insert_str, callback, 0, &zErrMsg); 
	     if( rc!=SQLITE_OK ){
	       fprintf(stderr, "SQL error: %s %d\n", zErrMsg,idx);
	       sqlite3_free(zErrMsg);
	     }
	   }
	   /* Gamma Record */
	   if (!strncmp(&(textline[5]),"  G ",4)) {
	     CopySubStr(gamma.NUCID,textline,1,5);
	     CopySubStr(gamma.E,textline,10,19);
	     CopySubStr(gamma.DE,textline,20,21);
	     CopySubStr(gamma.RI,textline,22,29);
	     CopySubStr(gamma.DRI,textline,30,31);
	     CopySubStr(gamma.M,textline,32,41);
	     CopySubStr(gamma.MR,textline,42,49);
	     CopySubStr(gamma.DMR,textline,50,55);
	     CopySubStr(gamma.CC,textline,56,62);
	     CopySubStr(gamma.DCC,textline,63,64);
	     CopySubStr(gamma.TI,textline,65,74);
	     CopySubStr(gamma.DTI,textline,75,76);
	     CopySubStr(gamma.C,textline,77,77);
	     CopySubStr(gamma.COIN,textline,78,78);
	     CopySubStr(gamma.Q,textline,80,80);
	     if (GAMMA_READ_DEBUG == 1 && !strncmp(qvalue.NUCID," 64ZN",5)) { 
               printf("nucid = %s\n",gamma.NUCID);
	       printf("e = %s\n",gamma.E);
	       printf("de = %s\n",gamma.DE);
	       printf("ri = %s\n",gamma.RI);
	       printf("dri = %s\n",gamma.DRI);
	       printf("m = %s\n",gamma.M);
	       printf("mr = %s\n",gamma.MR);
	       printf("dmr = %s\n",gamma.DMR);
	       printf("cc = %s\n",gamma.CC);
	       printf("dcc = %s\n",gamma.DCC);
	       printf("ti = %s\n",gamma.TI);
	       printf("dti = %s\n",gamma.DTI);
	       printf("c = %s\n",gamma.C);
	       printf("coin = %s\n",gamma.COIN);
	       printf("q = %s\n",gamma.Q);
	     }
             sprintf(insert_str,"INSERT INTO GAMMA VALUES (%d,\"%s\",%f,%f,%f,%f,\"%s\",%f,%f,%f,%f,%f,%f,\"%s\",\"%s\",\"%s\",%d,%d);",idx,gamma.NUCID,strtof(gamma.E,NULL),strtof(gamma.DE,NULL),strtof(gamma.RI,NULL),strtof(gamma.DRI,NULL),gamma.M,strtof(gamma.MR,NULL),strtof(gamma.DMR,NULL),strtof(gamma.CC,NULL),strtof(gamma.DCC,NULL),strtof(gamma.TI,NULL),strtof(gamma.DTI,NULL),gamma.C,gamma.COIN,gamma.Q,last_level_idx,last_id_idx);
	     rc = sqlite3_exec(db, insert_str, callback, 0, &zErrMsg); 
	     if( rc!=SQLITE_OK ){
	       fprintf(stderr, "SQL error: %s %d\n", zErrMsg,idx);
	       sqlite3_free(zErrMsg);
	     }
	   }
	 }
	 idx = idx + 1;
	 fgets(textline,127,infile);
       }
       fclose(infile);
   }
  }
  /* Match Level records with non-ground state isomers which have their own card. */
  for (jj=0; jj<parent_cnt; jj++) {
    if (strtof(parent_e_list[jj],NULL) > 0.0 || parent_xflag_list[jj] > 0) {
      sprintf(select_str,"SELECT LEVEL_IDX FROM LEVEL WHERE ESTR='%s' AND NUCID='%s' AND XFLAG==%d;",parent_e_list[jj],parent_nucid_list[jj],parent_xflag_list[jj]);
      printf("isomer %s\n",select_str);
      rc = sqlite3_prepare_v2(db, select_str, 1000, &res, &tail);
      while (sqlite3_step(res) == SQLITE_ROW) { 
        isomer_level_idx = sqlite3_column_int(res,0); 
        sprintf(select_str_iso,"UPDATE LEVEL SET ISOMER_IDX=%d WHERE LEVEL_IDX==%d;",parent_idx_list[jj],isomer_level_idx);
        printf("%s\n",select_str_iso); 
        rc = sqlite3_prepare_v2(db, select_str_iso, 1000, &res_iso, &tail);
        sqlite3_step(res_iso);      
        sqlite3_finalize(res_iso);     
      }
      sqlite3_finalize(res);
    }
  }
  printf("parent cnt = %d\n",parent_cnt);

  /* closedir(dp); */
  return 0;
}

void CreateENSDFTables(sqlite3 *db){
  char *zErrMsg = 0;
  int rc;
    
  rc = sqlite3_exec(db, "CREATE TABLE ID(id_idx INTEGER PRIMARY KEY ASC, nucid TEXT, dsid TEXT, dsref TEXT, pub TEXT, date TEXT)", callback, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
 
  rc = sqlite3_exec(db, "CREATE TABLE HISTORY(history_idx INTEGER PRIMARY KEY ASC, nucid TEXT, history TEXT, id_idx INTEGER)", callback, 0, &zErrMsg); 
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

  rc = sqlite3_exec(db, "CREATE TABLE QVALUE(qvalue_idx INTEGER PRIMARY KEY ASC, nucid TEXT, qminus REAL, dqminus REAL, sn REAL, dsn REAL, sp REAL, dsp REAL, qa REAL, dqa REAL, qref TEXT, id_idx INTEGER)", callback, 0, &zErrMsg); 
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

  rc = sqlite3_exec(db, "CREATE TABLE CROSSREF(crossref_idx INTEGER PRIMARY KEY ASC, nucid TEXT, dssym TEXT, dsid TEXT, id_idx INTEGER)", callback, 0, &zErrMsg); 
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

  rc = sqlite3_exec(db, "CREATE TABLE COMMENT(comment_idx INTEGER PRIMARY KEY ASC, nucid TEXT, rtype TEXT, psym TEXT, ctext TEXT, prev_comment_idx INTEGER, id_idx INTEGER)", callback, 0, &zErrMsg); 
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

  rc = sqlite3_exec(db, "CREATE TABLE PARENT(parent_idx INTEGER PRIMARY KEY ASC, nucid TEXT, pnum TEXT, estr TEXT, e REAL, de REAL, j TEXT, t REAL, dt REAL, qp REAL, dqp REAL, ion TEXT, xflag INTEGER, id_idx INTEGER)", callback, 0, &zErrMsg); 
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

  rc = sqlite3_exec(db, "CREATE TABLE NORMALIZATION(norm_idx INTEGER PRIMARY KEY ASC, nucid TEXT, pnum TEXT, nr REAL, dnr REAL, nt REAL, dnt REAL, br REAL, dbr REAL, nb REAL, dnb REAL, np REAL, dnp REAL, id_idx INTEGER)", callback, 0, &zErrMsg); 
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

  rc = sqlite3_exec(db, "CREATE TABLE PRODNORMALIZATION(pnorm_idx INTEGER PRIMARY KEY ASC, nucid TEXT, nrxbr REAL, nrxbrunc REAL, ntxbr REAL, ntxbrunc REAL, nbxbr REAL, nbxbrunc REAL, np REAL, dnp REAL, com TEXT, opt TEXT, norm_idx INTEGER, id_idx INTEGER)", callback, 0, &zErrMsg); 
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

   rc = sqlite3_exec(db, "CREATE TABLE LEVEL(level_idx INTEGER PRIMARY KEY ASC, nucid TEXT, estr TEXT, e REAL, de REAL, j TEXT, t REAL, dt REAL, l REAL, s REAL, ds REAL, c TEXT, ms TEXT, q TEXT, xflag INTEGER, isomer_idx INTEGER, id_idx INTEGER)", callback, 0, &zErrMsg); 
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

   rc = sqlite3_exec(db, "CREATE TABLE BETA(beta_idx INTEGER PRIMARY KEY ASC, nucid TEXT, e REAL, de REAL, ib REAL, dib REAL, logft REAL, dft REAL, c TEXT, un TEXT, q TEXT, level_idx INTEGER, id_idx INTEGER)", callback, 0, &zErrMsg); 
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
   rc = sqlite3_exec(db, "CREATE TABLE EC(ec_idx INTEGER PRIMARY KEY ASC, nucid TEXT, e REAL, de REAL, ib REAL, dib REAL, ie REAL, die REAL, logft REAL, dft REAL, ti REAL, dti REAL, c TEXT, un TEXT, q TEXT, level_idx INTEGER, id_idx INTEGER)", callback, 0, &zErrMsg); 
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
   rc = sqlite3_exec(db, "CREATE TABLE ALPHA(alpha_idx INTEGER PRIMARY KEY ASC, nucid TEXT, e REAL, de REAL, ia REAL, dia REAL, hf REAL, dhf REAL, c TEXT, q TEXT, level_idx INTEGER, id_idx INTEGER)", callback, 0, &zErrMsg); 
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
   rc = sqlite3_exec(db, "CREATE TABLE DELAYED(delayed_idx INTEGER PRIMARY KEY ASC, nucid TEXT, particle TEXT, e REAL, de REAL, ip REAL, dip REAL, ei REAL, t REAL, dt REAL, l REAL, c TEXT, coin TEXT, q TEXT, level_idx INTEGER, id_idx INTEGER)", callback, 0, &zErrMsg); 
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
   rc = sqlite3_exec(db, "CREATE TABLE GAMMA(gamma_idx INTEGER PRIMARY KEY ASC, nucid TEXT, e REAL, de REAL, ri REAL, dri REAL, m TEXT, mr REAL, dmr REAL, cc REAL, dcc REAL, ti REAL, dti REAL, c TEXT, coin TEXT, q TEXT, level_idx INTEGER, id_idx INTEGER)", callback, 0, &zErrMsg); 
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
  rc = sqlite3_exec(db, "CREATE TABLE REFERENCE(reference_idx INTEGER PRIMARY KEY ASC, mass TEXT, keynum TEXT, reference TEXT, id_idx INTEGER)", callback, 0, &zErrMsg); 
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

}

int main(int argc, char **argv){
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  rc = sqlite3_open("ensdfdb", &db);
  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return(1);
  }

  CreateENSDFTables(db);
  
  FillENSDFDB(db);
  
  sqlite3_close(db);
  return 0;
}

#ifndef __progresso_H__
#define __progresso_H__

typedef struct struct_progresso{
         char  cfase[1024];
         int   pmin;
         int   pdist;
         int   ntotal;
         int   indperc;
         float perc;
         float percprint;
} progresso_str;

#define PROGRESSO_INIT      1
#define PROGRESSO_ATUAL     2
#define PROGRESSO_FINALIZA  3
#define atualiza_progresso(st,p) (imprime_progresso((st),"",PROGRESSO_ATUAL   ,(p),-1,-1,1))
#define finaliza_progresso(st)   (imprime_progresso((st),"",PROGRESSO_FINALIZA, -1,-1,-1,1))

#ifdef __cplusplus
extern "C"{
#endif

   progresso_str inicia_progresso(char *cfase, const int imin, const int imax, const int idist);
   void imprime_progresso(progresso_str *st, const char *cfase, const int init, const int pos_atual, const int imin, const int imax, const int idist);

#ifdef __cplusplus
}
#endif


#endif

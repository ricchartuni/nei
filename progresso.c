#include "progresso.h"

//#define _DEBUG___
#ifdef _DEBUG___
#endif

#include "constantes.h"
#include "tempo.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAXPERC         .1f
// ** RChartuni >>#define PROG_ENG

//======================================================================
   progresso_str inicia_progresso(char *cfase, const int imin, const int imax, const int idist)
   {
//---------------------------------------------------------------------
// Descricao:
//---------------------------------------------------------------------
//         *** Criado por: .............. Ricardo Chartuni
//         *** Criado em: ..................... 14.07.2020
//======================================================================
      progresso_str st;
      imprime_progresso(&st, cfase, PROGRESSO_INIT, -1, imin, imax, idist);
      return st;
   }
//======================================================================

//======================================================================
   void imprime_progresso(progresso_str *st, const char *cfase, const int init, const int pos_atual, const int imin, const int imax, const int idist)
   {
//---------------------------------------------------------------------
// Descricao:
//---------------------------------------------------------------------
//         *** Criado por: .............. Ricardo Chartuni
//         *** Criado em: ..................... 14.07.2020
//======================================================================
      char      chora[NTAMHORA];
      char      msg[256];
      int       ind;
      float     percpronto;

      switch(init){
         case PROGRESSO_INIT:
#ifdef PROG_ENG
            sprintf(msg, "   > %s - Beginning", horario(chora));
#else
            sprintf(msg, "   > %s - Iniciando", horario(chora));
#endif
            fprintf(stderr,"   ============================================\n");
            fprintf(stderr,"%s %s\n", msg, cfase);

            strcpy(st->cfase, cfase);
            st->pmin      = imin;
            st->pdist     = idist;
            st->ntotal    = (int) (floor(((float)imax-(float)imin)/(float)idist) + 1);
            st->indperc   = 1;
            st->perc      = mymax(MAXPERC, 1./st->ntotal);
            st->percprint = (float) st->indperc * st->perc;
         break;
         case PROGRESSO_ATUAL:
            ind        = mymin(mymax(0,floor(((float)pos_atual-(float)st->pmin)/(float)st->pdist)),st->ntotal-1);
            percpronto = (float)(ind)/(float)(st->ntotal);
            if(percpronto >= st->percprint){
#ifdef PROG_ENG
               sprintf(msg, "   > %s -", horario(chora));
               fprintf(stderr,"%s %5.1f%% of %s completed\n", msg, 100.*percpronto, st->cfase);
#else
               sprintf(msg, "   > %s - Completou", horario(chora));
               fprintf(stderr,"%s %5.1f%% da %s\n", msg, 100.*percpronto, st->cfase);
#endif
               st->indperc    = st->indperc + 1;
               st->percprint  = (float)(st->indperc*st->perc);
            }
         break;
         default:
#ifdef PROG_ENG
            sprintf(msg, "   > %s -", horario(chora));
            fprintf(stderr,"%s %s finished\n", msg, st->cfase);
#else
            sprintf(msg, "   > %s - Finalizou", horario(chora));
            fprintf(stderr,"%s %s\n", msg, st->cfase);
#endif
            fprintf(stderr,"   ============================================\n");
      }
   }
//======================================================================


#include "tempo.h"

//#define _DEBUG___
#ifdef _DEBUG___
#endif

#include <time.h>

//======================================================================
   char *horario(char *horabuff)
   {
//---------------------------------------------------------------------
// Descricao:
//---------------------------------------------------------------------
//         *** Criado por: .............. Ricardo Chartuni
//         *** Criado em: ..................... 14.07.2020
//======================================================================
      time_t tHora;
      time(&tHora);
      strftime(horabuff, NTAMHORA, "%d/%m/%Y %H:%M:%S", localtime(&tHora));

      return horabuff;
   }
//======================================================================

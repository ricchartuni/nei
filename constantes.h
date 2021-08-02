#ifndef __constantes_H__
#define __constantes_H__

#ifndef         _pi
#define         _pi 3.141592653589793116
#endif
#ifndef         mymax
#define    mymax(x,y) ((x)>(y)?(x):(y))
#endif
#ifndef         mymin
#define    mymin(x,y) ((x)<(y)?(x):(y))
#endif
#ifndef        nint
#define     nint(x) ((int)((x)>0.0?(x)+0.5:(x)-0.5))
#endif
#ifndef         sqr
#define      sqr(x) ((x)*(x))
#endif
#ifndef     deg2rad
#define  deg2rad(x) ((  .0174532925199)*(x))
#endif
#ifndef     rad2deg
#define  rad2deg(x) ((57.2957795131   )*(x))
#endif
#ifndef        myswap
#define   myswap(x,y) {typeof(x) dum=(x); (x)=(y); (y)=dum;}
#endif
#ifndef      mysim
#define      mysim 1
#endif
#ifndef      mynao
#define      mynao 0
#endif
#ifndef      inulo
#define      inulo -99999
#endif
#ifndef      lnulo
#define      lnulo (long) inulo
#endif
#ifndef      rnulo
#define      rnulo -99999.
#endif
#ifndef      dnulo
#define      dnulo (double) rnulo
#endif


#ifndef ind2D
#define ind2D(i1,i2,             n1            ) (((n1)*(i2)                    ) +      (i1                        ))
#endif
#ifndef ind3D
#define ind3D(i1,i2,i3,          n1,n2         ) (((n1)*(n2)*(i3)               ) + ind2D(i1,i2,      n1            ))
#endif
#ifndef ind4D
#define ind4D(i1,i2,i3,i4,       n1,n2,n3      ) (((n1)*(n2)*(n3)*(i4)          ) + ind3D(i1,i2,i3,   n1,n2         ))
#endif
#ifndef ind5D
#define ind5D(i1,i2,i3,i4,i5,    n1,n2,n3,n4   ) (((n1)*(n2)*(n3)*(n4)*(i5)     ) + ind4D(i1,i2,i3,i4,n1,n2,n3      ))
#endif
#ifndef ind6D
#define ind6D(i1,i2,i3,i4,i5,i6, n1,n2,n3,n4,n5) (((n1)*(n2)*(n3)*(n4)*(n5)*(i6)) + ind5D(i1,i2,i3,i4,i5,n1,n2,n3,n4))
#endif

//---------------------------------------
//para arrays alocados estaticamente
#ifndef tamanho
#define tamanho(arr) (sizeof arr/sizeof *arr)
#endif
#ifndef preenche
#define preenche(arr, val) for(int i_##arr=0;i_##arr<tamanho(arr);i_##arr++)arr[i_##arr]=val;
#endif
//---------------------------------------


#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif


#endif


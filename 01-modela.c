#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define nx 101           /*dimensao x do modelo*/
#define nz 101           /*dimensao z do modelo*/
#define na 30            /*caixa para absorcao*/
#define fat 0.0025       /*fator de amortecimento*/
#define ntotal 701       /*numero de amostras no tempo do dado modelado*/
#define nf 118           /*numero total de amostras na fonte - lido na execucao do programa da fonte*/              
#define dt 0.002         /*amostragem em segundos*/
#define h 12.5           /*dimensao da malha em metros dx=dz=h*/
#define izf 5            /*posicao z fonte no modelo*/
#define jxf 51           /*posicao x fonte no modelo*/

int main()
{
	
	int i,j,k;
	int ilz,jlx;

	float P1[nz][nx];                     /*Campo de onda no passado*/
	float P2[nz][nx];                     /*Campo de onda no presente*/
	float P3[nz][nx],P3T[nx][nz];         /*Campo de onda no futuro*/
	float vT[nx][nz];                     /*Campo de velocidades lido (input) ---> vem invertido (ver codigo velocidade)*/
	float v[nz][nx];
	float c[nz][nx];                      /*variavel auxiliar 1*/
	float A[nz][nx];                      /*variavel auxiliar 2*/
	float source[nf];                     /*Vetor fonte*/
	float cof1[na+1],fat1[na+1];
	float cof2[na+1],fat2[na+1];
	float cof3[na+1],fat3[na+1];
	float sis[ntotal][nx];
	float sisT[nx][ntotal];
	
	FILE *vel;        /*Campo de velocidades*/
	FILE *fonte;      /*Fonte*/
	//FILE *snap;     /*snapshot*/
	FILE *tiro;       /*tiro*/

	/*-------IMPORTA VELOCIDADE-------------------------*/
	vel = fopen("ZZ1-campo_vel_101x101.bin","rb");

	if (vel == NULL)
	{
		printf("Erro na abertura do arquivo!");
		exit(1);
	}

	for (i = 0; i < nx; i++)
	{
		for (j = 0; j < nz; j++)
		{
			fread(&vT[i][j],sizeof(float),nx*nz,vel);
		}
	}
	/*--------------------------------------------------*/
	
	/*--------------------------------------------------------------------------------*/
	/*CONTROLE DE QUALIDADE - EXPORTANDO O ARQUIVO IMPORTADO PARA VISUALIZAR DE NOVO*/
	/*--------------------------------------------------------------------------------*/
	/*FILE *velT_teste;*/
	/*velT_teste = fopen("ZZ3-campo_vel_teste_301x201.bin","wb");

	if (velT_teste == NULL)
	{
		printf("Erro na abertura do arquivo!");
		exit(1);
	}

	for (i = 0; i < nx; i++)
	{
		for (j = 0; j < nz; j++)
		{
			fwrite(&vT[i][j],sizeof(float),nx*nz,velT_teste);
		}
	}
	fclose(velT_teste);/*
	/*-----------------------------------------------------------------------------------------*/

	/*----------------------------------------------*/
	/*----------------IMPORTA FONTE-----------------*/
	fonte = fopen("ZZ2-tempo_fonte-fc30_dt2-neg.dat","rb");

	if (fonte == NULL)
	{
		printf("Erro na abertura do arquivo!");
		exit(1);
	}

	for (i = 0; i < nf; i++)
	{
		fread(&source[i],sizeof(float),nf,fonte);
	}
	/*------------------------------------------------------*/
	/*---------------CONTROLE DE QUALIDADE------------------*/
	/*FILE *testa_fonte;
	testa_fonte = fopen("ZZ4-tempo_fonte-fc60_dt2.txt","w");

	if (testa_fonte == NULL)
	{
		printf("Erro na abertura do arquivo!");
		exit(1);
	}

	for (i = 0; i < (nf+1); i++)
	{
		fprintf(testa_fonte,"%6d\t%6.3f\n",i,source[i]);
	}

	fclose(testa_fonte);*/
	/*----------------------------------------------------------------*/
	/*Transpondo vT para voltar a logica de velocidade de leitura do C*/
	for (i = 0; i < nx; i++)
		for (j = 0; j < nz; j++)
			v[j][i] = vT[i][j];
	/*----------------------------------------------------------------*/
	/*CALCULO DA VARIAVEL AUXILIAR 1 C*/
	/*----------------------------------*/
	for (i = 0; i < nz; i++)
	{
		for (j = 0; j < nx; j++)
		{
			c[i][j] = -((pow((v[i][j]*dt)/h,2.0))/12.0);
		}
	}

	/*----------------------------------------------------------------*/
	/*CALCULO DA VARIAVEL AUXILIAR 2 (A) para absorcao*/
	/*----------------------------------*/
	for (i = 0; i < nz; i++)
	{
		for (j = 0; j < nx; j++)
		{
			A[i][j] = (v[i][j]*dt)/h;
		}
	}

	/*CONDICOES INICIAIS t <= 0 -----Zerando matrizes*/
	for (i = 0; i < nz; i++)
	{
		for (j = 0; j < nx; j++)
		{
			P1[i][j] = 0.0;
			P2[i][j] = 0.0;
			P3[i][j] = 0.0;
		}
	}
	/*--------------------------------------------------------------------------------------------------*/
	/*--------------------------------------------------------------------------------------------------*/
	/*Fatores multiplicativos borda inferior*/
	for (i = (nz - na - 1); i < nz; i++)
	{
		j = i - (nz - na - 1);
		cof1[j] = fat;                        
	}

	for (i = (nz - na - 1); i < nz; i++)
	{
		j = i - (nz - na - 1);
		fat1[j] = exp(-(cof1[j]*(i - (nz - na - 1)))*(cof1[j]*(i - (nz - na - 1))));
	}

	/*Fatores multiplicativos para borda superior e lateral esquerda*/

	for (i = 0; i < na + 1; i++)
		cof2[i] = fat;                

	for (i = 0; i < na + 1; i++)
	{
		fat2[i] = exp(-(cof2[i]*(i - na))*(cof2[i]*(i - na)));   
	}

	/*Fatores multiplicativos para borda lateral direita*/

	for (i = nx - 1 - na; i < nx; i++)
	{
		j = i - (nx - na - 1);
		cof3[j] = fat;                    
	}
		
	for (i = nx - na - 1; i < nx; i++)
	{
		j = i - (nx - na - 1);
		fat3[j] = exp(-(cof3[j]*(i - (nx - na - 1)))*(cof3[j]*(i-(nx - na - 1))));
	}
	/*----------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------*/
	/*---------------------------LOOP TEMPORAL------------------------------------------*/
	/*----------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------*/	
	for (k = 0;k < ntotal;k++)   /*Inicio do loop temporal*/
	{
		if (k < nf)
		{
			P2[izf][jxf] = P2[izf][jxf] + source[k];
		}
		
		for (i = 2; i < (nz - 2); i++)
		{
			for (j = 2; j < (nx - 2); j++)
			{
				P3[i][j] = c[i][j] * (P2[i][j + 2] + P2[i][j - 2] 
					     + P2[i + 2][j] + P2[i - 2][j] 
					     - 16 * (P2[i][j + 1] + P2[i][j - 1] + P2[i + 1][j] + P2[i - 1][j]) 
					     + 60 * P2[i][j]) 
				         + 2 * P2[i][j] - P1[i][j];
			}
		}
		/*-----------------------------------------------------------*/
		/*-----------------------------------------------------------*/
		/*-----------------------------------------------------------*/
		/*---------BORDAS NAO-REFLEXIVAS---------------*/
		for (i = 0; i < 2; i++)                                            /*---->TOPO*/
		{
			for (j = 2; j < (nx - 2); j++)
			{
				P3[i][j] = P2[i][j] + A[i][j]*(P2[i+1][j] - P2[i][j]);
			}
		}	
		for (i = (nz - 2); i < nz; i++)                                   /*----->BASE*/
		{
			for (j = 2; j < (nx - 2); j++)
			{
				P3[i][j] = P2[i][j] - A[i][j]*(P2[i][j] - P2[i-1][j]);
			}	
		}

		for (i = 2; i < (nz - 2); i++)                                  /*-------->ESQUERDA*/
		{
			for (j = 0; j < 2; j++)
			{
				P3[i][j] = P2[i][j] + A[i][j]*(P2[i][j+1] - P2[i][j]);
			}
		}

		for (i = 2; i < (nz - 2); i++)                               /*------------>DIREITA*/
		{
			for (j = (nx - 2); j < nx; j++)
			{
				P3[i][j] = P2[i][j] - A[i][j]*(P2[i][j] - P2[i][j-1]);
			}
		}
		/*--------------------------------------------------*/
		/*------------BORDAS ABSORTIVAS---------------------*/
		/*Borda superior*/
		for (i = 0; i < (na + 1); i++)
		{
			for (j = na + 1; j < (nx - na - 1); j++)
			{
				P3[i][j] = P3[i][j]*fat2[i];
				P2[i][j] = P2[i][j]*fat2[i];
			}
		}
		/*Borda inferior*/
		for (i = (nz - na - 1); i < nz; i++)
		{
			for (j = 0; j < nx; j++)
			{
				P3[i][j] = P3[i][j]*fat1[i - (nz - na - 1)];
				P2[i][j] = P2[i][j]*fat1[i - (nz - na - 1)];
			}
		}
		/*Borda lateral esquerda*/
		for (i = 0; i < (nz - na - 1); i++)
		{
			for (j = 0; j < (na + 1); j++)
			{
				P3[i][j] = P3[i][j]*fat2[j];
				P2[i][j] = P2[i][j]*fat2[j];
			}
		}
		/*Borda lateral direita*/
		for (i = 0; i < (nz - na - 1); i++)
		{
			for (j = (nx - na - 1);j < nx; j++)
			{
				P3[i][j] = P3[i][j]*fat3[j - (nx - na - 1)];
				P2[i][j] = P2[i][j]*fat3[j - (nx - na - 1)];
			}
		}
		/*--------------------------------------------------*/
		/*----------ATUALIZACAO DO CAMPO--------------------*/
		for (i = 0; i < nz; i++)
		{	
			for (j = 0; j < nx; j++)
			{
				P1[i][j] = P2[i][j];   /*Presente torna-se passado*/
				P2[i][j] = P3[i][j];   /*Futuro torna-se presente*/
			}
		}

		/*----------CALCULA SISMOGRAMA----------------------*/

		for (j = 0; j < nx; j++)
		{
			sis[k][j] = P3[0][j];
		}

		/*-------------------------------------------------*/
		/*----------------GERA SNAPSHOOT---------------------------------*/
		/*if (k == 200)
		{
			/*Transposta de P3 para voltar a logica anterior para visualizacao*/
			/*for (i = 0; i < nz; i++)
				for (j = 0; j < nx; j++)
					P3T[j][i] = P3[i][j];        
			/*-----------------------------------------------*/

			/*OPCIONAL: COLOCAR VELOCIDADE NO FUNDO PARA VIZUALIZACAO*/
			/*for (i = 0; i < nx; i++)
			{
				for (j = 0; j < nz; j++)
				{
					P3T[i][j] = 350*P3T[i][j]+ vT[i][j]; /*dando ganho no campo e somando vt para colocar cor no fundo*/
			/*	}
			}
			/*-------------------------------------------------------*/
			/*-------------------------------------------------------*/
			/*snap = fopen("SNP05-snap200_101x101_h12.5_fonte30-bordas30-velFUNDO.bin","wb");

			if (snap == NULL)
			{
				printf("Erro na abertura do arquivo!");
				exit(1);
			}

			for (i = 0; i < nx; i++)
			{
				for (j = 0; j < nz; j++)
				{
					fwrite(&P3T[i][j],sizeof(float),nx*nz,snap);
				}
			}
		}*/
	}
	
	/*--------------------------------------------------*/
	/*TRANSPOSIÇÃO DE SISMOGRAMA PARA VIZUALIZACAO*/
	for (i = 0; i < ntotal; i++)
		for (j = 0; j < nx; j++)
			sisT[j][i] = sis[i][j];
	/*--------------------------------------------------*/

	/*-----------GERA TIRO---------------*/

	tiro = fopen("SHOT01-Z0_701x101_h12.5_fonte30-neg-borda30-fat35.bin","wb");
	//tiro = fopen("SHOT01-Z0_701x101_h12.5_fonte30-neg.bin","wb");

	if (tiro == NULL)
	{
		printf("Erro na abertura do arquivo!");
		exit(1);
	}

	for (i = 0;i < nx;i++)
	{
		for (j = 0; j < ntotal; j++)
		{
			fwrite(&sisT[i][j],sizeof(float),nx*ntotal,tiro);
		}
	}
	/*--------------------------------------------------*/
	/*--------------------------------------------------*/
	fclose(vel);
	fclose(fonte);
	//fclose(snap);
	fclose(tiro);
	return(0);
}

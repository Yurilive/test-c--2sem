 	#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>

struct listaProd{
	int codP, anoP, diaP;
	char modeC, mesP[2];
	double qtdeP, prcU, margemL;

};

/* Ano bisexto ou nao ? */
bool is_leap_year(int y) {
	return y % 400 == 0 || (y % 4 == 0 && y && 100 != 0);
}

/* Quantidade de dias para o mes escolhido */
int mesD(int Qmes, int Qano){
	
	switch(Qmes) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
			break;
			
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
			break;
		
		case 2:
			if(is_leap_year(Qano));
	}
}

int Qano(){
	int ano;
	
	do{
		printf("Qual o ano de vendas?\n");
		scanf("%d", &ano);
		if(ano < 2016 || ano > 2022){
			system("cls");
			printf("O ano digitado e invalido...\n\n");
		}
	}while(ano < 2016 || ano > 2022);
	
	return ano;
}

int Qmes(){
	int mes;
	
	do{
		printf("Qual o mes de vendas?\n");
		scanf("%d", &mes);
		if(mes < 1 || mes > 12){
			system ("cls");
			printf("O mes digitado e invalido...\n\n");
		}
	}while(mes < 1 || mes > 12);
	
	return mes;
}

int Qdia(){
	int qDia;
	
	do{
		printf("Quantas vendas diarias foram realizadas?\n");
		scanf("%d", &qDia);
		if(qDia <= 0){
			system("cls");
			printf("Quantidade invalida...\n\n");
		}
	}while(qDia < 0);
	
	return qDia;
}

int main() {
	/*variaveis para produto */
	FILE *lanc, *arq_saida;
	listaProd produto, produtoVend;
	listaProd *relGeral, *venda; 
	
	int NDiasMes, QtdeVendasG, QtdeVendasO; // quantidade de vendas totais e oscila??o
	int anoV, mesV, diaV = 1, vendasPDia; // registradores de data
	int i, z, t, cont = 0, tam_vet = 0; // contadores
	int numSort, oscillation = 0, positivo = 0; //

	relGeral = NULL;
	venda = NULL;
	t = 0;
	

	printf("Desenvolvedores:\n");
	printf("Yuri Ribeiro de Oliveira\n");
	printf("Matheus Cesar do Amaral\n");
	printf("Lucas Rosseti Fortes\n");
	printf("Andr? Gomes Balbastro\n\n");
	
	/* Leitura do arquivo de produtos e gravando no vetor de struct */
	lanc = fopen("produtos.txt", "r");
	
	if(lanc == NULL){
		printf("Problemas na leitura do arquivo \n");
		return 0;
	}

	while(fscanf(lanc, "%d;%c;%lf;%lf;%lf;", &produto.codP, &produto.modeC, &produto.qtdeP, &produto.prcU, &produto.margemL) != EOF) {
		t++;
		relGeral = (listaProd *) realloc(relGeral, sizeof(listaProd) * t);
		relGeral[t-1] = produto;
	}	
	fclose(lanc);
	
	// Inser??o e valida??o de data
	printf("Bem vindo \n\n");
	anoV = Qano();
	mesV = Qmes();
	vendasPDia = Qdia();
	system("cls");
	
	// Calculo de vendas totais
	NDiasMes = mesD(mesV, anoV);
	QtdeVendasG = vendasPDia * NDiasMes;
	QtdeVendasO = QtdeVendasG * 0.35;
	
	for(i=0;i<NDiasMes;i++){
		printf("VENDAS DO DIA %2d/%2d/%d :\n\n", diaV, mesV, anoV);
		for(z=01;z<=vendasPDia;z++){
			numSort = rand() % 15 + 1;
			produtoVend.anoP = anoV;
			sprintf(produtoVend.mesP, "%02d", mesV);
			produtoVend.diaP = diaV;
			produtoVend.codP = relGeral[numSort].codP;
			produtoVend.qtdeP = relGeral[numSort].qtdeP;
			produtoVend.margemL = (relGeral[numSort].margemL / 100 + 1) * relGeral[numSort].prcU;
			
			if (oscillation == cont && (QtdeVendasO*2) >= cont){
				oscillation = oscillation + 2;
				if (positivo == 0){
					positivo = 1;
					produtoVend.margemL = produtoVend.margemL + produtoVend.margemL * 0.08;
				}else{
					positivo = 0;
					produtoVend.margemL = produtoVend.margemL - produtoVend.margemL * 0.08;
				} 
			}
			
			printf(" VENDA %d\n", z);
			printf(" -Ano da venda: %2d\n", produtoVend.anoP);
			printf(" -Mes da venda: %02d\n", mesV);
			printf(" -Dia da venda: %02d\n", produtoVend.diaP);
			printf(" -Codigo do produto: %d\n", produtoVend.codP);
			printf(" -Quantidade: %.2lf\n", produtoVend.qtdeP);
			printf(" -Venda: R$ %.2lf\n\n", produtoVend.margemL);
			
			tam_vet++;
			venda = (listaProd *)realloc(venda, sizeof(listaProd) * tam_vet);
			venda[tam_vet-1] = produtoVend;
			cont++;
		}
		printf("_________________________________________________\n\n");
		diaV++;
	}
	
	//registrando em arquvio
	arq_saida = fopen("vendas.txt", "w");
	printf("gravacao do arquivo\n");
	for (i = 0; i < t; i++)
	{
		fprintf(arq_saida, "%d;%s;%d;%d;%.2lf;%.2lf\n", venda[i].anoP, venda[i].mesP, venda[i].diaP, venda[i].codP, venda[i].qtdeP, venda[i].margemL);
	}
	fclose(arq_saida);
	free(venda);
	free(relGeral);
	printf("\n\nFim do programa");
	printf("\n Total vendas %d", QtdeVendasG);
	
	return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <time.h>



struct calcSal{
	double salBruto; //valor de entrada 
	double aliqINSS, aliqIR; //aliquotas
	double valINSS, valIR, baseIR;//valores de desconto
	double salLiquido;//Resultado
};


int main(){
	FILE *salFile, *calcFile;
	
	bool flag;
	
	double aliqr_i, deducaoR; 
	
	int tam_vet_s = 0;
	int temp;
	int i, j;
	
	calcSal info_sal;
	calcSal *s;
	
	calcSal org_sal;

	s = NULL;
	
	//Leitura do arquivo de salarios
	printf("Lendo salarios brutos...");
	salFile = fopen("salario.txt", "r");
	while (fscanf(salFile, "%lf", &info_sal.salBruto) != EOF){
		printf("1");
		if(info_sal.salBruto < 1751.81){
			info_sal.aliqINSS = 8.00 / 100;
		}else if(info_sal.salBruto < 2919.72){
			info_sal.aliqINSS = 9.00 / 100;
		}else if(info_sal.salBruto < 5839.45){
			info_sal.aliqINSS = 11.00 / 100;
		}else{
			info_sal.aliqINSS = 0.00;
		}
		printf("2");
		if (info_sal.aliqINSS == 0.00){
			info_sal.valINSS = 642.34;
		}else{
			info_sal.valINSS = info_sal.salBruto * info_sal.aliqINSS;
		}
		printf("3");
		aliqr_i = info_sal.salBruto - info_sal.valINSS;
		info_sal.baseIR = aliqr_i;
		printf("4");
		if(aliqr_i < 1903.98){
			info_sal.aliqIR = 0,00;
			deducaoR = 0.00;
		}else if(aliqr_i < 2826.65){
			info_sal.aliqIR = 7.5 / 100;
			deducaoR = 142.80;
		}else if(aliqr_i < 3751.05){
			info_sal.aliqIR = 15.0 / 100;
			deducaoR = 354.80; 
		}else if(aliqr_i < 4664.68){
			info_sal.aliqIR = 22.5 / 100;
			deducaoR = 636.13;
		}else{
			info_sal.aliqIR = 27.5 / 100;
			deducaoR = 869.36;
		}
		printf("5");
		info_sal.valIR = ((info_sal.salBruto - info_sal.valINSS) * info_sal.aliqIR) - deducaoR;
		if (info_sal.valIR < 10.00){
			info_sal.valIR = 0.00;
		} 
		printf("6");
		info_sal.salLiquido = info_sal.salBruto - info_sal.valINSS - info_sal.valIR;
		printf("7");
		tam_vet_s++;
		s = (calcSal *) realloc(s, sizeof(calcSal) * tam_vet_s);
		s[tam_vet_s - 1] = info_sal;
		printf("8");
	}	
	fclose(salFile);
	
	//Organizando valores
	for(i=0;i<tam_vet_s;i++){
		for(j=0;j<(tam_vet_s-i-1);j++){
			if(s[j].salBruto > s[j+1].salBruto){
				org_sal = s[j];
				s[j] = s[j+1];
				s[j+1] = org_sal;
				flag = true;
			}
		}
	}

	
	//Registrando no arquvio de calculos
	printf("Gravando em arquivos...\n\n");
	calcFile = fopen("calculos.txt", "w");
	fprintf(calcFile, "Bruto     AliqINSS     Val.INSS     Base I.R.     AliqIR     Val.IR     Liquido\n");
	for(i=0;i<tam_vet_s;i++){
		fprintf(calcFile, "%-9.2lf %-12.1lf %-12.2lf %-13.2lf %-10.1lf %-10.2lf %.2lf\n", s[i].salBruto, s[i].aliqINSS * 100, s[i].valINSS, s[i].baseIR, s[i].aliqIR * 100, s[i].valIR, s[i].salLiquido);	
	}
	fclose(calcFile);
	
	//Liberando valores dinamicos
	free(s);
	printf("Fim do programa");
	return 0;
	

}

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

struct Plist{
	int codP, qtdeP, qtdeCO, valor_total_v = 0; //var para produto
	int qtdeV, situV, canalV; //var de venda
	int QtCO, QtMin, QtV, Estoq_AV, necess, transfCO, cont_vendas = 0; //var de transferrencia
};

//Funções para possiveis divergencias
int diver1(int lin, int cod){
	FILE *diverFile;
	
	diverFile = fopen("divergencias.txt", "a");
	fprintf(diverFile, "Linha %d - Código de Produto não encontrado: %d\n", lin, cod); 
	
	return 0;
}

int diver2(int lin){
	FILE *diverFile;
	
	diverFile = fopen("divergencias.txt", "a");
	fprintf(diverFile, "Linha %d - Venda cancelada\n", lin); 
	
	return 0;
}

int diver3(int lin){
	FILE *diverFile;
	
	diverFile = fopen("divergencias.txt", "a");
	fprintf(diverFile, "Linha %d - Venda não finalizada\n", lin); 
	
	return 0;
}

int diver4(int lin){
	FILE *diverFile;
	
	diverFile = fopen("divergencias.txt", "a");
	fprintf(diverFile, "Linha %d - Erro desconhecido. Acionar equipe de TI\n", lin); 
	
	return 0;
}

int main(){
	setlocale(LC_ALL, "");
	
	printf("Desenvolvedores:\n");
	printf("Yuri Ribeiro de Oliveira\n");
	printf("Matheus Cesar do Amaral\n");
	printf("Lucas Rosseti Fortes\n");
	printf("André Gomes Balbastro\n\n");
	
	int tam_vet_p = 0, tam_vet_v = 0, tam_vet_t = 0;
	int i, t;
	
	Plist info;
	Plist *p;
	
	Plist infoV;
	Plist *v;
	
	Plist infoT;
	Plist *trans_conf;
	
	FILE *prodFile, *vendasFile, *transFile, *diverFile;
	
	p = NULL;
	v = NULL;
	trans_conf = NULL;
	
	//Leitura dos arquivos de produtos e vendas
	
	//Iniciando arquivo de divergencias
	
	//Iniciando Arquivo de produtos
	printf("Lendo produtos...\n");
	prodFile = fopen("c1_produtos.txt", "r");
	while(fscanf(prodFile, "%d;%d;%d", &info.codP, &info.qtdeP, &info.qtdeCO) != EOF){
		tam_vet_p++;
		p = (Plist *) realloc(p, sizeof(Plist) * tam_vet_p);
		p[tam_vet_p - 1] = info;
	}
	fclose(prodFile);
	
	//Arquivo de venda
	printf("Lendo vendas...\n");
	vendasFile = fopen("c1_vendas.txt", "r");
	diverFile = fopen("divergencias.txt", "w");
	while(fscanf(vendasFile, "%d;%d;%d;%d", &infoV.codP, &infoV.qtdeV, &infoV.situV, &infoV.canalV) != EOF){
		for(i=0;i<tam_vet_p;i++){
			if(infoV.codP == p[i].codP){
				if(infoV.situV == 100 || infoV.situV == 102){
					p[i].cont_vendas += 1;
					p[i].qtdeV = infoV.qtdeV;
					p[i].valor_total_v = p[i].valor_total_v + p[i].qtdeV;
					break;
				}else if(infoV.situV == 135){
					diver2(tam_vet_v);
					break;
				}else if(infoV.situV == 190){
					diver3(tam_vet_v);
					break;
				}else if(infoV.situV == 999){
					diver4(tam_vet_v);
					break;
				}
			}else if(i + 1 == tam_vet_p){
				diver1(tam_vet_v, infoV.codP);
			}
		}
		tam_vet_v++;
	}
	fclose(diverFile);
	fclose(vendasFile);
	
	//Verificação de transferências
	printf("Transferindo valores...\n");	
	for(i=0;i<tam_vet_p;i++){
		infoT.codP = p[i].codP;
		infoT.QtCO = p[i].qtdeP;
		infoT.QtMin = p[i].qtdeCO;
		infoT.QtV = p[i].cont_vendas;
		infoT.Estoq_AV = infoT.QtCO - p[i].valor_total_v;
		
		if(infoT.Estoq_AV < infoT.QtMin)
			infoT.necess = infoT.QtMin - infoT.Estoq_AV;
			
		if(infoT.necess > 1 && infoT.necess < 10){
			infoT.transfCO = 10;
		}else{
			infoT.transfCO = infoT.necess;
		}
		tam_vet_t++;
		trans_conf = (Plist *) realloc(trans_conf, sizeof(Plist) * tam_vet_t);
		trans_conf[tam_vet_t - 1] = infoT;
	}
	
	//Registro no arquivo de saida
	printf("Gravando em arquivos...\n\n");
	transFile = fopen("transfere.txt", "w");
	fprintf(transFile, "Necessidade de Transferência Armazém para CO\n\n");
	fprintf(transFile, "Produto     Qtco     QtMin     QtVendas     Estq.após     Necess.     Transf. de\n");
	fprintf(transFile, "                                            vendas                     Arm p/ CO\n");
	for(i=0;i<tam_vet_p;i++){
		fprintf(transFile, "%-11d %-8d %-9d %-12d %-13d %-11d %d\n", trans_conf[i].codP, trans_conf[i].QtCO, trans_conf[i].QtMin, trans_conf[i].QtV, trans_conf[i].Estoq_AV, trans_conf[i].necess, trans_conf[i].transfCO);
	}
	fclose(transFile);
	
	//Liberando vetores dinamicos da memória
	free(p);
	free(v);
	free(trans_conf);	
	printf("Fim do progama\n");
	return 0;
}

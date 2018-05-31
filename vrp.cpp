/*
				UNIVERSIDADE FEDERAL DO PAMPA - UNIPAMPA
		
		TRABALHO I DA DISCIPLINA DE INTELIGÊNCIA ARTIFICIAL - VRP
					
					Nome: Rafael Fernandes
																		*/

#include <iostream>
#include <fstream>
#include <deque>
#include <cmath>
#include <vector>

using namespace std;

struct no
{
	int cordX;
	int cordY;
	int demanda;
	int indice;
	bool visitado;
	float* distancias;
};
typedef no No;

class Grafo
{
public:
	Grafo();
	~Grafo();
	deque<No*> nos;
	No* criaNo(int cordX, int cordY, int demanda, int indice, int noConsumidores);
	float resolve(deque<No*>nos);		
	void leArq();
	float calculaDist(int x1, int y1, int x2, int y2);
	int noConsumidores;
	int capacidadeVeiculo;
private:
	

};

Grafo::Grafo()
{

}

Grafo::~Grafo()
{

}

//Método de criar nó.
No* Grafo:: criaNo(int cordX, int cordY, int demanda, int indice, int noConsumidores)
{
	No *g = (no*)malloc(sizeof(no));
	g->cordX = cordX;
	g->cordY = cordY;
	g->demanda = demanda;
	g->indice = indice;
	g->visitado = false;
	g->distancias = (float*)malloc(noConsumidores*sizeof(float));
	return g;
}

//Método de ler arquivo.
void Grafo :: leArq()
{
	int i;
	No* nodo;
	int aux, aux2, aux3; //Variáveis para auxiliar a leitura


	//Atribui o número de consumidores e capacidade do veículo.
	//puts("Digite o número de consumidres e capacidade do veículo");
	scanf("%d %d", &noConsumidores, & capacidadeVeiculo);

	//puts("Digite a coordenada X e Y do depósito");
	scanf("%d %d", &aux, &aux2);
	nodo = criaNo(aux, aux2, 0, 0, noConsumidores+1);
	nos.push_back(nodo);

	for(i = 0; i < noConsumidores; i++)
	{
		//puts("Digite a coordenda X e Y e a demanda do depósito");
		scanf("%d %d %d", &aux, &aux2, &aux3);
		nodo = criaNo(aux, aux2, aux3, i+1, noConsumidores+1);
		nos.push_back(nodo);			
	}
	
	//Calcula distancia entre consumidores e distância do depósito.
	//Percorre todos os nós.
	for(int i = 0; i < nos.size(); i++)
	{
		//Cálcula distância entre o consumidor atual e os outros consumidores.
		for(int j = 0; j < noConsumidores+1; j++)
		{
			//Testa se é o nó atual, se não for cálcula a distância.
			if(i != j)
			{
				nos.at(i)->distancias[j] = calculaDist(nos.at(i)->cordX, nos.at(i)->cordY, nos.at(j)->cordX, nos.at(j)->cordY);
				printf("A distância entre %d e %d é = %f \n", i, j, nos.at(i)->distancias[j]);	
			}
			//Se for o nó atual, a distância é 0.
			else 
			{
				printf("A distância entre %d e %d é = %f \n", i, j, nos.at(i)->distancias[j]);
				nos.at(i)->distancias[j] = 0;
			}			
		}	
		printf("A demanda de %d é %d \n \n", nos.at(i)->indice, nos.at(i)->demanda);
	}

	float resultado;
	resultado = resolve(nos);
	printf("\n \nO resultado é %f\n", resultado);
}

//Método para cálcular distância euclidiana.
float Grafo :: calculaDist(int x1, int y1, int x2, int y2)
{
	//Retorna o resultado da distância euclidiana.
	return sqrt((pow(x1 - x2, 2)) + (pow(y1 - y2, 2)));
}

deque<No*> sort(float *vetor, deque<No*> no)
{
	for(int i = 0; i < no.size(); i++)
	{
		for(int j = 0; j < no.size()-1; j++)
		{
			if(vetor[j] > vetor[j+1])
			{
				swap(vetor[j],vetor[j+1]);
				swap(no.at(j), no.at(j+1));
			}
		}
	}

	/*for(int i = 0; i < no.size(); i++)
	{
		printf("Vetor[%d] = %f \t indice = %d \n", i, vetor[i],no.at(i)->indice);
	}*/

	/*for(int i=0; i< no.size(); i++)
	{
		printf("Vetor[%d] = %f \n", i, vetor[i]);
	}*/

	return no; 
}

bool checaVisitado(deque<No*> nos)
{
	for(int i = 0; i < nos.size(); i++)
	{
		if(nos.at(i)->visitado == false)
		{
			return true;
		}
	}
	return false;
}
float Grafo :: resolve(deque<No*> nos)
{
	float custo = 0;
	float auxDist = 9999;
	int j = 0;	
	int i, atual = 0, prox = 0, dist;
	int contador = 0;
	int auxCapacidadeVeiculo = capacidadeVeiculo;
	vector<int> auxCaminhos;
	vector<vector<int>> caminhos;	

	while(checaVisitado(nos))
	{		
		nos.at(atual)->visitado = true;
		prox = atual;
		for(i = 1; i < nos.size(); i++)
		{
			if(nos.at(i)->visitado == false)
			{
				if(auxCapacidadeVeiculo - nos.at(i)->demanda >= 0)
				{
					dist = nos.at(atual)->distancias[i];
					if(auxDist > dist)
					{
						auxDist = dist;
						prox = i;
					}
				}	
			}		
		}

		if(atual != prox)
		{
			auxCaminhos.push_back(atual);
			//printf("Caminho %d \n",auxCaminhos.at(atual));
			auxCapacidadeVeiculo -= nos.at(prox)->demanda;
			atual = prox;
			auxDist = 99999;
		}
		else
		{
			auxCaminhos.push_back(atual);
			auxCapacidadeVeiculo = capacidadeVeiculo;
			atual = 0;
			auxDist = 99999;
			auxCaminhos.push_back(0);	
		}
	}

	for(i = 0; i < auxCaminhos.size(); i++)
	{
		if(auxCaminhos.at(i) == 0)
			contador++;
		printf("%d ", auxCaminhos.at(i));
		if(contador >=2)
		{
			printf("\n");
			contador =0;
		}
	}
	for(i = 0; i < auxCaminhos.size()-1; i++)
	{
		//printf("%d ", auxCaminhos.at(i));
		custo += nos.at(auxCaminhos.at(i))->distancias[auxCaminhos.at(i+1)];
	}
	//printf("tamanho do vetor %d \n", auxCaminhos.size());
	return custo;
}

int main(int argc, char** argv)
{
	//Instancia um objeto da classe grafo
	Grafo* graf = new Grafo();
	//Testa se o arquivo a ser lido foi passado

	//Manda o arquivo para o método de leitura
	graf->leArq();

	return 0;
}

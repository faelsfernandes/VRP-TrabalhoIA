/*
				UNIVERSIDADE FEDERAL DO PAMPA - UNIPAMPA
		
		TRABALHO I DA DISCIPLINA DE INTELIGÊNCIA ARTIFICIAL - VRP
					
					Nome: Rafael Fernandes
					
	
Descrição:
Heurística de construção:
Vizinho mais pŕoximo:
Para construir as rotas foi utilizado o algoritmo de "vizinho mais próximo"
que pega a menor distância a partir de uma determinada cidade.

Heurística de melhoramento:
Busca tabu:
Após gerar as rotas foi aplicada a busca tabu, esta que faz a troca de consumidores
entre entre uma mesma rota ou entre duas rotas distintas de forma aleatória ou buscando
diminuir a distância final.	
																					*/

#include <iostream>
#include <fstream>
#include <deque>
#include <cmath>
#include <vector>
#include <time.h>

using namespace std;

struct no
{
	int cordX;
	int cordY;
	int demanda;
	bool visitado;
	float* distancias;
};
typedef no No;

class Grafo
{
public:
	Grafo();
	~Grafo();
	deque<No*> nos; //Deque de nós (representam os consumidores);
	int noConsumidores; //Número de consumidores;
	int capacidadeVeiculo; //Capacidade do veículo;
	No* criaNo(int cordX, int cordY, int demanda, int noConsumidores);
	bool checaVisitado(deque<No*> nos);
	float resolve(deque<No*>nos);		
	float calculaDist(int x1, int y1, int x2, int y2);
	void leArq();
	vector<vector<int>> criaRota(deque <No*> nos);
	vector<vector<int>> tabu(vector<vector<int>> rotas);
};

Grafo::Grafo(){};

Grafo::~Grafo(){};

//Método de criar nó que representa um consumidor;
No* Grafo:: criaNo(int cordX, int cordY, int demanda, int noConsumidores)
{
	No *g = (no*)malloc(sizeof(no)); //Aloca memoria para o nó;
	g->cordX = cordX; // Coordenada x do consumidor;
	g->cordY = cordY; // Coordenada y do consumidor;
	g->demanda = demanda; // Demanda do consumidor;
	g->visitado = false; // Define o consumidor inicialmente como não visitado;
	g->distancias = (float*)malloc(noConsumidores*sizeof(float)); // Aloca memória para o vetor de distâncias a partir deste consumidor;
	return g;
}

//Método de ler arquivo;
void Grafo :: leArq()
{
	int i = 0;
	No* no;
	float resultado = 0;
	int aux = 0, aux2 = 0, aux3 = 0; //Variáveis para auxiliar a leitura;

	//Atribui o número de consumidores e capacidade do veículo;
	//puts("Digite o número de consumidres e capacidade do veículo");
	scanf("%d %d", &noConsumidores, & capacidadeVeiculo);

	//puts("Digite a coordenada X e Y do depósito");
	scanf("%d %d", &aux, &aux2);
	no = criaNo(aux, aux2, 0, noConsumidores+1);
	nos.push_back(no);

	for(i = 0; i < noConsumidores; i++)
	{
		//puts("Digite a coordenda X e Y e a demanda do depósito");
		scanf("%d %d %d", &aux, &aux2, &aux3);
		no = criaNo(aux, aux2, aux3, noConsumidores+1);
		nos.push_back(no);			
	}
	
	//Calcula distancia entre consumidores e distância do depósito;
	//Percorre todos os nós;
	for(int i = 0; i < nos.size(); i++)
	{
		//Calcula distância entre o consumidor atual e os outros consumidores;
		for(int j = 0; j < noConsumidores+1; j++)
		{
			//Testa se é o nó atual, se não for calcula a distância;
			if(i != j)
			{
				nos.at(i)->distancias[j] = calculaDist(nos.at(i)->cordX, nos.at(i)->cordY, nos.at(j)->cordX, nos.at(j)->cordY);
				//printf("A distância entre %d e %d é = %f \n", i, j, nos.at(i)->distancias[j]);	
			}
			//Se for o nó atual, a distância é 0;
			else 
			{
				//printf("A distância entre %d e %d é = %f \n", i, j, nos.at(i)->distancias[j]);
				nos.at(i)->distancias[j] = 0;
			}			
		}	
		//printf("A demanda de %d é %d \n \n", nos.at(i)->indice, nos.at(i)->demanda);
	}
}

// Método pai que chama os métodos de resolução do problema;
float Grafo :: resolve(deque<No*> nos)
{
	float custo = 0;
	int i = 0, j = 0;
	vector<vector<int>> rotas;

	rotas = criaRota(nos);//Invocação do método de criação de rotas;
	
	//Teste para garantir que a tabu seja aplicada apenas se houver mais de uma rota;
	if(rotas.size() > 1)
		rotas = tabu(rotas);

	//Imprime as rotas;
	for(i = 0; i < rotas.size(); i++)
	{
		for(j = 0; j < rotas.at(i).size(); j++)
		{
			printf("%d ", rotas.at(i).at(j));
		}
		cout<<endl;
	}
	
	//Calcula o custo;
	for(i = 0; i < rotas.size(); i++)
	{
		for(j = 0; j < rotas.at(i).size()-1; j++)
		{
			custo += nos.at(rotas.at(i).at(j))->distancias[rotas.at(i).at(j+1)];
		}
	}
	rotas.clear();
	//Retorna o custo;
	return custo;
}

//Método para calcular distância euclidiana.
float Grafo :: calculaDist(int x1, int y1, int x2, int y2)
{
	//Retorna o resultado da distância euclidiana;
	return sqrt((pow(x1 - x2, 2)) + (pow(y1 - y2, 2)));
}

//Método que checa se há algum nó possível a ser visitado;
bool Grafo :: checaVisitado(deque<No*> nos)
{
	for(int i = 0; i < nos.size(); i++)
	{
		if(nos.at(i)->visitado == false) //Se ainda há algum nó possível, retorna verdadeiro;
		{
			return true; 
		}
	}
	return false; //Se não retorna falso;
}

//Método de criação de rotas;
vector<vector<int>> Grafo :: criaRota(deque <No*> nos)
{
	int i = 0, j = 0, k = 0;	
	int atual = 0, prox = 0, dist = 0;
	int auxCapacidadeVeiculo = capacidadeVeiculo;
	float auxDist = 9999;
	vector<int> auxCaminhos;
	vector<vector<int>> rotas;

	while(checaVisitado(nos))
	{	
		nos.at(atual)->visitado = true; // Marca o consumidor atual como visitado;
		prox = atual; // Atualiza o próximo consumidor a ser visitado;
		for(i = 1; i < nos.size(); i++)
		{
			if(nos.at(i)->visitado == false) //Percorre os consumidores ligados ao consumidor atual;
			{
				if(auxCapacidadeVeiculo - nos.at(i)->demanda >= 0) //Testa se é possível colocar carga no caminhão;
				{
					dist = nos.at(atual)->distancias[i]; //Salva o valor da distância do consumidor atual para o selecionado;
					if(auxDist > dist) // Verifica se a distância é menor que a anterior;
					{
						auxDist = dist; //Atualiza para a melhor distância;
						prox = i; // Atualiza a próxima cidade a ser visitada;
					}
				}	
			}		
		}

		if(atual != prox) // Adiciona o consumidor na rota atual caso tenha passado nos testes acima;
		{
			auxCaminhos.push_back(atual); //Adiciona o consumidor ao vetor de caminhos;
			auxCapacidadeVeiculo -= nos.at(prox)->demanda; //Subtrai o valor da demanda da cidade adicionada da capacidade do caminhão;
			atual = prox; // Atualizada a cidade atual;
			auxDist = 99999; //Reseta o auxiliar de distância;
		}
		else //Reseta os auxiliares e fecha a nova rota;
		{
			auxCaminhos.push_back(atual);
			auxCapacidadeVeiculo = capacidadeVeiculo;
			atual = 0;
			auxDist = 99999;
			auxCaminhos.push_back(0);
			rotas.push_back(auxCaminhos);
			auxCaminhos.clear();
		}
	}//Fim do algoritmo de criação de rotas;
	return rotas;
}

//Método de busca tabu;
vector<vector<int>> Grafo:: tabu(vector<vector<int>> rotas)
{
	int i = 0, j = 0, k = 0;
	int auxRand, auxRand2; //Auxiliares para gerar índices para selecionar um consumidor randômico;
	float auxDistancia1, auxDistancia2; //Auxiliares de distância para antes da troca;
	float auxDistancia3, auxDistancia4; //Auxiliares de distância para depois da troca;
	int capacidade1, capacidade2; //Auxiliares de capacidade do veículo;
			
	while(k < 10000)
	{
		for(i = 0; i < rotas.size(); i++) //Percorre as rotas;
		{
			for(j = 1; j < rotas.at(i).size(); j++)//Percorre as consumidores;
			{
				auxRand = 0; auxRand2 = 0; //Auxiliares para gerar índices para selecionar um consumidor randômico;
				auxDistancia1 = 0; auxDistancia2 = 0; //Auxiliares de distância para antes da troca;
				auxDistancia3 = 0; auxDistancia4 = 0; //Auxiliares de distância para depois da troca;
				capacidade1 = 0; capacidade2 = 0; //Auxiliares de capacidade do veículo;*/

				auxRand = (rand() % (rotas.size()-1))+1; //Gera uma rota aleatória baseada na quantidade de rotas;
				auxRand2 = (rand() % (rotas.at(auxRand).size()-2))+1;//Gera um consumidor aleatório baseado no tamanho da rota gerada acima;
			
				//Calcula distância da rota 1 antes da alteração;
				for(int l = 0; l < rotas.at(i).size()-1; l++)
				{
					auxDistancia1 += nos.at(rotas.at(i).at(l))->distancias[rotas.at(i).at(l+1)];
				}

				//Calcula distância da rota 2 antes da alteração;
				for(int l = 0; l < rotas.at(auxRand).size()-1; l++)
				{
					auxDistancia2 += nos.at(rotas.at(auxRand).at(l))->distancias[rotas.at(auxRand).at(l+1)];
				}
				
				if((rotas.at(i).at(j) == 0) || (rotas.at(auxRand).at(auxRand2) == 0)) //Testa se realmente foram selecionadaos consumidores e não o depósito;
					break;					
				else
					swap(rotas.at(i).at(j), rotas.at(auxRand).at(auxRand2)); //Faz a troca dos consumidores caso tenha passado no teste/

				//Faz o cálculo da demanda da rota 1 após ser alterada;
				for(int l = 0; l < rotas.at(i).size(); l++)
				{
					capacidade1 += nos.at(rotas.at(i).at(l))->demanda;
				}
				
				//Faz o cálculo da demanda da rota 2 após ser alterada;
				for(int l = 0; l < rotas.at(i).size(); l++)
				{
					capacidade2 += nos.at(rotas.at(i).at(l))->demanda;
				}
				
				//Testa se alguma das rotas têm a capacidade maior que a suportada pelo veículo;
				if((capacidade1 > capacidadeVeiculo) || (capacidade2 > capacidadeVeiculo))
				{
					swap(rotas.at(i).at(j), rotas.at(auxRand).at(auxRand2)); //Se tiver, a troca entre de cidades entre as rotas é desfeita;
					break;
				}
				else //Caso contrário, calcula o custo para as duas novas rotas;
				{
					for(int l = 0; l < rotas.at(i).size()-1; l++) //Calcula nova distância da primeira rota;
					{
						auxDistancia3 += nos.at(rotas.at(i).at(l))->distancias[rotas.at(i).at(l+1)];
					}

					for(int l = 0; l < rotas.at(auxRand).size()-1; l++) //Calcula nova distância da segunda rota;
					{
						auxDistancia4 += nos.at(rotas.at(auxRand).at(l))->distancias[rotas.at(auxRand).at(l+1)];
					}
				}

				//Testa se a soma dos custos das rotas antigas é maior que a soma dos custos das novas rotas;
				if((auxDistancia1 + auxDistancia2) >= (auxDistancia3 + auxDistancia4)) //Se for maior, a troca de cidades entre rotas é realmente feita;
					continue;
				else // Se não for, a troca é desfeita;
					swap(rotas.at(i).at(j), rotas.at(auxRand).at(auxRand2));
			}
		}
		k++;
	}//Fim da busca tabu;
	return rotas;
}

int main(void)
{
 	//Inicializa a semente para geração de números randômicos;
	srand (time(NULL));
	//Instancia um objeto da classe grafo;
	Grafo* graf = new Grafo();
	//Chama a função de ler arquivo e a partir dela todo o problema é resolvido;
	graf->leArq();
	float resultado = graf->resolve(graf->nos);
	printf("\n%f", resultado);
	return 0;
}

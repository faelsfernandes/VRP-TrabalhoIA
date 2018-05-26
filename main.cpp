/*

	Nome: Rafael Fernandes

*/

#include <iostream>
#include <fstream>
#include <deque>
#include <cmath>

using namespace std;

struct node
{
	int cordX;
	int cordY;
	int demanda;
	float* distancias;
};
typedef node Node;

class Grafo
{
	public:
		Grafo();
		~Grafo();
		Node* criaNo(int cordX, int cordY, int demanda, int noConsumidores);
		void leArq(string arquivo);
		float calculaDist(int x1, int y1, int x2, int y2);
		deque<Node*> nodos;
		
	private:
		int noConsumidores;
		int capacidadeVeiculo;

};

Grafo::Grafo()
{

}

Grafo::~Grafo()
{

}

//Método de criar nó.
Node* Grafo:: criaNo(int cordX, int cordY, int demanda, int noConsumidores)
{
	Node *g = (Node*)malloc(sizeof(Node));
	g->cordX = cordX;
	g->cordY = cordY;
	g->demanda = demanda;
	g->distancias = (float*)malloc(noConsumidores*sizeof(float));
	return g;
}

//Método de ler arquivo.
void Grafo :: leArq(string arquivo)
{
	fstream arq;
	Node* nodo;
	int aux, aux2, aux3;

	//Abre arquivo.
	arq.open(arquivo, ios::in | ios::out );
	//Testa se foi possível abrir arquivo.
	if(!arq.is_open())
	{
		puts("Problema ao ler arquivo!");
		exit(1);
	}

	//Atribui o número de consumidores e capacidade do veículo.
	arq >> noConsumidores;
	arq >> capacidadeVeiculo;

	//Lê coordenadas do depósito.
	arq >> aux; //
	arq >> aux2;
	//Cria um nó para o depósito.
	nodo = criaNo(aux, aux2, 0, noConsumidores+1);
	//Adiciona ao deque de nós.
	nodos.push_back(nodo);
	//Lê o resto do arquivo, criando nós para os consumidores e adicionando-os ao deque de nós.
	while(!arq.eof())
	{
		arq >> aux;
		arq >> aux2;
		arq >> aux3;
		nodo = criaNo(aux, aux2, aux3, noConsumidores+1);
		nodos.push_back(nodo);
	}

	//Calcula distancia entre consumidores e distância do depósito.
	//Percorre todos os nós.
	for(int i = 0; i < nodos.size(); i++)
	{
		//Cálcula distância entre o consumidor atual e os outros consumidores.
		for(int j = 0; j < noConsumidores+1; j++)
		{
			//Testa se é o nó atual, se não for cálcula a distância.
			if(i != j)
			{
				nodos.at(i)->distancias[j] = calculaDist(nodos.at(i)->cordX, nodos.at(i)->cordY, nodos.at(j)->cordX, nodos.at(j)->cordY);
				printf("A distância entre %d e %d é = %f \n", i, j, nodos.at(i)->distancias[j]);	
			}
			//Se for o nó atual, a distância é 0.
			else 
			{
				printf("A distância entre %d e %d é = %f \n", i, j, nodos.at(i)->distancias[j]);
				nodos.at(i)->distancias[j] = 0;
			}
			
		}	
	}
}

//Método para cálcular distância euclidiana.
float Grafo :: calculaDist(int x1, int y1, int x2, int y2)
{
	//Cálcula a distância euclidiana.
	return sqrt((pow(x1 - x2, 2)) + (pow(y1 - y2, 2)));
}	

int main(int argc, char** argv)
{
	Grafo* graf = new Grafo();

	if(argc < 2)
	{
		puts("Falta o arquivo");
		exit(1);
	}
	graf->leArq(argv[1]);
}
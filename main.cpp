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
		Node* createNode(int cordX, int cordY, int demanda, int noConsumidores);
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

Node* Grafo:: createNode(int cordX, int cordY, int demanda, int noConsumidores)
{
	Node *g = (Node*)malloc(sizeof(Node));
	g->cordX = cordX;
	g->cordY = cordY;
	g->demanda = demanda;
	g->distancias = (float*)malloc(noConsumidores*sizeof(float));
	return g;
}

void Grafo :: leArq(string arquivo)
{
	fstream arq;
	Node* nodo;
	int aux, aux2, aux3;

	arq.open(arquivo, ios::in | ios::out );
	if(!arq.is_open())
	{
		puts("Problema ao ler arquivo!");
		exit(1);
	}

	arq >> noConsumidores;
	arq >> capacidadeVeiculo;

	//Lê coordenadas do depósito
	arq >> aux; //
	arq >> aux2;
	nodo = createNode(aux, aux2, 0, noConsumidores+1);

	nodos.push_back(nodo);

	while(!arq.eof())
	{
		arq >> aux;
		arq >> aux2;
		arq >> aux3;
		nodo = createNode(aux, aux2, aux3, noConsumidores+1);
		nodos.push_back(nodo);
	}

	//Calcula distancia entre consumidores e distância do depósito
	for(int i = 0; i < nodos.size(); i++)
	{
		for(int j = 0; j < noConsumidores+1; j++)
		{
			if(i != j)
			{
				nodos.at(i)->distancias[j] = calculaDist(nodos.at(i)->cordX, nodos.at(i)->cordY, nodos.at(j)->cordX, nodos.at(j)->cordY);
				printf("A distância entre %d e %d é = %f \n", i, j, nodos.at(i)->distancias[j]);	
			}
			else 
			{
				printf("A distância entre %d e %d é = %f \n", i, j, nodos.at(i)->distancias[j]);
				nodos.at(i)->distancias[j] = 0;
			}
			
		}	
	}
}

float Grafo :: calculaDist(int x1, int y1, int x2, int y2)
{
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
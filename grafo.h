/*
 *  GRAFO.h - definicion de la clase GRAFO
 *
 *  Autores : Antonio Sedeno Noda, Sergio Alonso
 *  Cursos  : 2012-2021
 */

#ifndef _GRAPH_H
#define _GRAPH_H


#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <deque>


using namespace std;

//Solo para ubuntu, una forma de borrar la pantalla
#define gotoxy(x,y)  printf("\33[%d;%dH",y,x)
#define clrscr()     gotoxy(1,1); printf("\33[2J")

const unsigned UERROR = 65000; // Para ciertos algoritmos es necesario codificar este valor de error
const int maxint      = 1000000; // Para ciertos algoritmos es necesario codificar este valor de maximo

// Definicion de un elemento de la lista de adyacencia

typedef struct
{
	unsigned j; // nodo
	int      c; // atributo para expresar los costes, pesos, las longitudes, las inversiones, etc...
} ElementoLista;

// definicion del tipo de una lista de adyacencia de un nodo
typedef vector<ElementoLista> LA_nodo;

class GRAFO {
	unsigned dirigido;  				// 0 si el grafo es no dirigido y 1 si es dirigido
	unsigned n;         				// numero de NODOS
	unsigned m;         				// numero de ARCOS
  vector<LA_nodo> LS;         // Lista de adyacencia de los sucesores: es una matriz donde cada fila posiblemente es posiblemente de distinto tamano
	vector<LA_nodo> LP;         // Lista de adyacencia de los predecesores: es una matriz dode cada fila posiblemente es posiblemente de distinto tamano
	void destroy();             // Destructor del LS, y, en su caso, LP
	void build (char nombrefichero[85], int &errorapertura); // Crea LS, A y, en su caso, LP, desde la lectura del fichero
	void dfs_cc(unsigned i, vector<bool> &visitado); // Recorrido en profundidad recursivo con solo marca de visitado para el calculo de las componentes conexas
	void dfs_postnum(unsigned i, vector<bool> &visitado, vector<unsigned> &postnum, unsigned &postnum_ind); // Recorrido en profundidad recursivo para calcular postnum
	void dfs_cfc(unsigned i, vector<bool> &visitado); // Recorrido en profundidad recursivo con solo marca de visitado para el calculo de las componentes fuertemente conexas
	
	public:
    GRAFO(char nombrefichero[], int &errorapertura);
    void actualizar (char nombrefichero[], int &errorapertura);
    unsigned Es_dirigido(); // devuelve 0 si el grafo es no dirigido y 1 si es dirigido
    void Info_Grafo(); // devuelve informacion b�sica del grafo
    void Mostrar_Listas(int l);  // Muestra la lista de adyacencia
    void ListaPredecesores(); // Recorre la lista de sucesores LS para construir la de predecesores, LP
    void ComponentesConexas(); // Construye el vector de componentes conexas de un grafo no dirigido
    void ComponentesFuertementeConexas(); //
		// PRACTICA 4
		void Prim();
		void Prim_bis();
    ~GRAFO(); //Destructor del objeto grafo
};

#endif
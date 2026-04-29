 /*
 *  GRAFO.CPP - Plantilla para la implementaci�n de la clase GRAFOS
 *
 *
 *  Autores : Antonio Sedeno Noda, Sergio Alonso
 *  Cursos   : 2012-2021
 */

#include "grafo.h"

void GRAFO::destroy() 
{
	for (unsigned i = 0; i < n; i++) {
		LS[i].clear();
		if (dirigido == 1) LP[i].clear();
	}
	LS.clear();
	if (dirigido == 1) LP.clear();

}

void GRAFO::build(char nombrefichero[85], int &errorapertura) 
{
  ElementoLista dummy;
	ifstream textfile;
	textfile.open(nombrefichero);
	if (textfile.is_open()) {
		unsigned i, j, k;
		// leemos por conversion implicita el numero de nodos, arcos y el atributo dirigido
		textfile >> (unsigned &) n >> (unsigned &) m >> (unsigned &) dirigido;
		// los nodos internamente se numeran desde 0 a n-1
		// creamos las n listas de sucesores
		LS.resize(n);
    // leemos los m arcos
		for (k = 0; k < m; k++) {
			textfile >> (unsigned &) i  >> (unsigned &) j >> (int &) dummy.c;
			dummy.j = j - 1;
			LS[i - 1].push_back(dummy);
			if (dirigido == 0 && i != j) {
				dummy.j = i - 1;            
				LS[j - 1].push_back(dummy);
			}
		}
		textfile.close();
		errorapertura = 0;
		if (dirigido == 1) {
			ListaPredecesores();
		}
	} else {
		errorapertura = 1;
		cerr << "No se ha podido abrir el fichero indicado correctamente." << endl;
	}
}

GRAFO::~GRAFO()
{
	destroy();
}

GRAFO::GRAFO(char nombrefichero[85], int &errorapertura)
{
	build (nombrefichero, errorapertura);
}

void GRAFO::actualizar(char nombrefichero[85], int &errorapertura)
{
  //Limpiamos la memoria dinamica asumida en la carga previa, como el destructor
  destroy();
  //Leemos del fichero y actualizamos G con nuevas LS y, en su caso, LP
  build(nombrefichero, errorapertura);
}

unsigned GRAFO::Es_dirigido()
{
	return dirigido;
}

void GRAFO::Info_Grafo()
{
	cout << "Numero total de nodos: " << n << endl;
	cout << "Numero total de arcos: " << m << endl;
	cout << "¿Es dirigido?: " << ((Es_dirigido()) ? "Si" : "No")  << endl;
}

void Mostrar_Lista(vector<LA_nodo> L)
{
	for (unsigned i = 0; i < L.size(); i++) {
		cout << "Nodo " << i + 1 << ": ";
		for (unsigned j = 0; j < L[i].size(); j++) {
			cout << "(" << L[i][j].j + 1 << ", " << L[i][j].c << ") ";
		}
		cout << endl;
	}
}

void GRAFO::Mostrar_Listas(int l)
{
	switch (l)
	{
	case 1:
		Mostrar_Lista(LS);
		break;
	case 0:
		Mostrar_Lista(LS);
		break;
	case -1:
		Mostrar_Lista(LP);
		break;
	default:
		cout << "El grafo introducido no tiene un valor esperado (1, 0, -1)." << endl;
		break;
	}
}

void GRAFO::ListaPredecesores()  //Recorre la lista de sucesores LS para construir la de predecesores, LP
{
	LP.resize(n);
	ElementoLista dummy;
	for (unsigned i = 0; i < n; i++) {
		for (unsigned j = 0; j < LS[i].size(); j++) {
			dummy.j = i;
			dummy.c = LS[i][j].c;
			LP[LS[i][j].j].push_back(dummy);
		}
	}
}

void GRAFO::dfs_cc(unsigned i, vector<bool> &visitado) //Este recorrido esta� hecho adhoc para mostrar el ritmo de nodos visitados, para su uso en la construccion de Componentes Conexas
{
  visitado[i] = true; //visitamos el nodo i+1
	cout << i+1;
	for (unsigned j = 0; j < LS[i].size(); j++) { //recorremos la adyacencia del nodo visitado, esto es, i+1
		if (!visitado[LS[i][j].j]) {
			cout << ", ";
			dfs_cc(LS[i][j].j, visitado);
		}
	}
}

void GRAFO::ComponentesConexas()
{
  unsigned i, componentesconexas = 0;
	vector<bool> visitado;
	visitado.resize(n,false);
	i = 0;
	while (i < n) {
		if (visitado[i] == false) {
			componentesconexas++;
			cout << "Componente Conexa " << componentesconexas << " : { ";
			dfs_cc(i,visitado);
			cout << " }" << endl;
		}
		++i;
	}
}

void GRAFO::dfs_cfc(unsigned i, vector<bool> &visitado) //Este recorrido esta� hecho adhoc para mostrar el ritmo de nodos visitados, para su uso en la construccion de Componentes fuertemente Conexas
{
  visitado[i] = true; //visitamos el nodo i+1
	cout << i+1;
	for (unsigned j = 0; j < LP[i].size(); ++j) { //recorremos la adyacencia del nodo visitado, esto es, i+1
		if (!visitado[LP[i][j].j]) {
			cout << ", ";
			dfs_cfc(LP[i][j].j, visitado);
		}
	}
}

void GRAFO::dfs_postnum(unsigned i, vector<bool> &visitado, vector<unsigned> &postnum, unsigned &postnum_ind) //Este recorrido esta� hecho adhoc para calcular el orden postnumeraci�n de los nodos
{
	visitado[i] = true;
	for (unsigned j = 0; j < LS[i].size(); ++j) {
		if (!visitado[LS[i][j].j]) dfs_postnum(LS[i][j].j, visitado, postnum, postnum_ind);
	}
	postnum[postnum_ind++] = i;
}

void GRAFO::ComponentesFuertementeConexas() 
{
  unsigned i, postnum_ind, componentesfuertementeconexas = 0; 
  vector<bool> visitado; 
  vector<unsigned> postnum;  
  visitado.resize(n, false); 
  postnum.resize(n, UERROR);
  postnum_ind = 0;
  i = 0;
  while (i < n) {
    if (visitado[i] == false) {
      dfs_postnum(i, visitado, postnum, postnum_ind);  
    }
    i++;
  } 
  visitado.assign(n, false); 
  i = 0;
  while (i < n) {
    if (visitado[postnum[i]] == false) {   
      componentesfuertementeconexas++;
      cout << "Componente Fuertemente Conexa " << componentesfuertementeconexas << " : { ";
      dfs_cfc(postnum[i], visitado);
      cout << " }" << endl;
    }
    i++;
  }
}

void GRAFO::Prim() {
	vector<bool> visitado;
	vector<int> coste;
	vector<unsigned> pred;
	
	visitado.resize(n, false);
	coste.resize(n, maxint);
	pred.resize(n, UERROR);
	
	int nodo;
	cout << "Introduzca un nodo de partida: "; 
	cin >> nodo;

	unsigned r = nodo - 1; 
	coste[r] = 0;
	pred[r] = r;
	unsigned u = r; 
	visitado[u] = true;    
	
	vector<unsigned> orden_nodos;
	orden_nodos.push_back(u);
	
	int coste_total = 0; 
	unsigned nodos_descubiertos = 1;
	
	for (unsigned i = 1; i < n; i++) {
		for (unsigned j = 0; j < LS[u].size(); j++) {
			unsigned adyacente = LS[u][j].j;
			int coste_arista = LS[u][j].c;	
			if (visitado[adyacente] == false) {
				if (coste[adyacente] == maxint) {
					nodos_descubiertos++; 
				}
				if (coste[adyacente] > coste_arista) {
					coste[adyacente] = coste_arista; 
					pred[adyacente] = u;                
				}
			}
		}
		if (orden_nodos.size() == nodos_descubiertos) {
			cout << endl << "No es conexo..." << endl;
			break; 
		}
		int min_coste = maxint;
		for (unsigned k = 0; k < n; k++) {
			if (visitado[k] == false && coste[k] < min_coste) {
				min_coste = coste[k];
				u = k; 
			}
		}
		visitado[u] = true;
		coste_total += coste[u];
		orden_nodos.push_back(u);
	} 
	
	cout << "Orden de inclusion de los nodos: ";
	for (unsigned i = 0; i < orden_nodos.size(); i++) {
		cout << orden_nodos[i] + 1; 
		if (i < orden_nodos.size() - 1) {
			cout << " -> ";
		}
	}
	cout << endl;
	cout << "Aristas que forman el arbol de minimo coste: " << endl;
	for (unsigned i = 1; i < orden_nodos.size(); i++) {
		unsigned nodo_i = orden_nodos[i];
		cout << "Arista (" << pred[nodo_i] + 1 << ", " << nodo_i + 1 << ") con coste " << coste[nodo_i] << endl;
	}
	
	cout << endl;
	cout << "El coste total del arbol generador de minimo coste es: " << coste_total << endl;
}

void GRAFO::Prim_bis() {
	vector<bool> visitado;
	vector<int> coste;
	vector<unsigned> pred;
	
	visitado.resize(n, false);
	coste.resize(n, -maxint);
	pred.resize(n, UERROR);
	
	int nodo;
	cout << "Introduzca un nodo de partida: "; 
	cin >> nodo;

	unsigned r = nodo - 1; 
	coste[r] = 0;
	pred[r] = r;
	unsigned u = r; 
	visitado[u] = true;    
	
	vector<unsigned> orden_nodos;
	orden_nodos.push_back(u);
	
	int beneficio_total = 0;
	unsigned nodos_descubiertos = 1;
	for (unsigned i = 1; i < n; i++) {
		for (unsigned j = 0; j < LS[u].size(); j++) {
			unsigned adyacente = LS[u][j].j;
			int coste_arista = LS[u][j].c;	
			if (visitado[adyacente] == false) {
				if (coste[adyacente] == -maxint) {
					nodos_descubiertos++; 
				}
				if (coste[adyacente] < coste_arista) {
					coste[adyacente] = coste_arista; 
					pred[adyacente] = u;                
				}
			}
		}
		if (orden_nodos.size() == nodos_descubiertos) {
			cout << endl << "No es conexo..." << endl;
			break; 
		}
		int max_beneficio = -maxint; 
		for (unsigned k = 0; k < n; k++) {
			if (visitado[k] == false && coste[k] > max_beneficio) {
				max_beneficio = coste[k];
				u = k; 
			}
		}
		visitado[u] = true;
		beneficio_total += coste[u];
		orden_nodos.push_back(u);
	} 
	
	cout << "Orden de inclusion de los nodos: ";
	for (unsigned i = 0; i < orden_nodos.size(); i++) {
		cout << orden_nodos[i] + 1; 
		if (i < orden_nodos.size() - 1) {
			cout << " -> ";
		}
	}
	cout << endl;
	
	cout << "Aristas que forman el Arbol Generador de Beneficio Maximo:" << endl;
	for (unsigned i = 1; i < orden_nodos.size(); i++) {
		unsigned nodo_i = orden_nodos[i];
		cout << "Arista (" << pred[nodo_i] + 1 << ", " << nodo_i + 1  << ") con beneficio " << coste[nodo_i] << endl;
	}
	cout << endl << "El beneficio total del arbol generador maximo es: " << beneficio_total << endl;
}

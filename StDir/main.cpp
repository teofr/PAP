#include<vector>
#include <iostream>

using namespace std;


vector<vector<int>> ady;
vector<int> low, level;

void dfs(int v, int lev = 0, int p = -1){
	level[v] = low[v] = lev;

	for (auto n : ady[v]){
		if (level[n] == -1){
			// nuevo
			cout << v + 1 << ' ' << n + 1 << '\n';
			dfs(n, lev+1, v);

			low[v] = min(low[v], low[n]);

			if (low[n] == level[n]){
				// vn es puente, tiene que ser doble mano
				cout << n+1 << ' ' << v+1 << '\n';
			}

		} else if(n != p){
			low[v] = min(low[v], level[n]);
			if (level[n] < level[v]){
				// se que esta arista es nueva ya que:
				//  si n esta negro, ya me deberian haber 
				//  	visitado siendo el mi padre
				//  si n esta gris, es mi ancestro pero 
				//  	no mi padre
				cout << v+1 << ' ' << n+1 << '\n';
			}
			// si level[n] >= level[v] significa que no esta
			// gris, ya que si no, seria un ancestro, y mi
			// level debería ser mayor, tampoco esta blanco
			// luego esta negro, pero no es mi padre y el grafo
			// es no dirigido (no visito todas sus aristas)
		}
	}
}

int main(){
	cin.tie(0); ios_base::sync_with_stdio(0);
	int n, m, cnt = 1;
	ady = vector<vector<int>> (1001, vector<int> ());
	low = vector<int> (1001, n+n);
	level = vector<int> (1001, -1);
	while (cin >> n >> m, n != 0){
		for (int i = 0; i < n; i++){ // pequena optimización
			ady[i].clear();
			low[i] = 2*n;
			level[i] = -1;
		}

		for (int i = 0; i < m; i++){
			int a, b;
			cin  >> a>>b;
			a--; b--;
			ady[a].push_back(b);
			ady[b].push_back(a);
		}
		cout << cnt++ << "\n\n";
		dfs(0);
		cout << "#\n";
	}

}


/* Explicación de la solución
 *
 * La idea viene de que las aristas de cortes de un grafo particionan a los verticos en componentes 2 arista conexas,
 * entonces, en este problema en particular, si uno tiene el grafo G de calles, y quita todas las aristas de cortes,
 * quedan componentes de nodos que son 2 aristas conexas, en particular si uno corriese el dfs para buscar puentes 
 * en estas componentes, tendria que siempre se puede subir más arriba que el nivel de su padre, entonces puedo darle 
 * dirección a las aristas del arbol DFS (en mi caso para abajo), y a cada arista que vuelve a un ancestro, para arriba.
 * 
 * Esto para cada componente 2 arista conexa, y los puentes los necesito doble mano para unirlas en ambos sentidos.
 * 
 * Complejidad:
 * 
 * Para cada caso, la complejidad de leer la entrada es O(n+m), y luego corremos un DFS que tiene complejidad O(n+m),
 * por lo que, para cada caso, la solución toma O(n + m). Más aún, cada nodo tiene grado a lo sumo 4 (por enunciado),
 * entonces m = O(n), por lo que la complejidad queda como O(n).
 * 
 */
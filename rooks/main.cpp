#include "bits/stdc++.h"

using namespace std;

// struct cacho{
// 	int from, to;
// }

namespace dinics {

constexpr int noparent = -1;
    
/**
 * Author: Francisco Soulignac
 */
/** 
 * Algoritmo de Dinitz (llamado usualmente dinics).  Observaciones:
 * - los nodos se numeran de 0 a n-1.  En caso en que se numeren de 1 a n, simplemente generar un flowgraph con n+1 vertices
 *   sin conectar nada al vertice 0.
 * 
 * COMPLEJIDAD
 * - Este algoritmo tiene complejidad O(n^2m), lo cual es mas que suficiente para competencias (salvo push relabel para grafos densos).
 * - En el caso en que todas las capacidades sean 1, cuesta O(min(m^1/2, n^2/3)m) lo cual es O(n^{2/3}m) para caso denso O(m^{3/2}) para ralos .
 * - Cuando el grafo es bipartito cuesta O(sqrt(n)m), lo cual matchea con el que se usa en competencia (de Hopcroft y Karp) y es
 * mejor que el de Berge.
 * 
 * NOTAS:
 * Esta implementacion es basica, separada en subtareas, sin mezclar nada para mostrar una implementacion casi directa.
 * (ver dinics-fast.cpp)
 */
class flowgraph {
    struct edge {
        long long from,         //vertice del que parte la arista; solo por simetria y para simplificar, se puede sacar
                  to,           //vertice al que llega la arista
                  capacity,     //capacidad de la arista
                  flow,         //flujo de la arista, el residual es capacity - flow
                  reverse;      //indice a la arista reversa en el grafo residual
    };
    template<class T>
    using digraph = vector<vector<T>>;
    
    digraph<edge> residual;

    inline edge& reverse(const edge& e) {
        return residual[e.to][e.reverse];
    }

    
    /** 
     * Computa el grafo de niveles usando BFS a partir del residual.
     * Retorna un grafo donde por cada vertice se tienen los indices de los vecinos
     * que pertenecen al level graph en el grafo residual.
     */
    digraph<int> find_level_graph(int source) {
        digraph<int> level_graph(residual.size());
        vector<int> level(residual.size(), noparent);
        deque<int> q(1, source);
        level[source] = 0;
        
        while(not q.empty()) {
            auto v = q.front(); q.pop_front();
            for(int idx = 0; idx < residual[v].size(); ++idx) {
                auto& e = residual[v][idx];
                if(e.flow == e.capacity) continue;
                if(level[e.to] == noparent) {
                    q.push_back(e.to);
                    level[e.to] = level[v] + 1;
                }
                if(level[e.to] > level[v]) level_graph[v].push_back(idx);
            }
        }
        return level_graph;
    }
    
    /** 
     * Aplica DFS para encontrar un augementing path mientras se busca el blocking flow.
     * Retorna el valor del augmenting path, que es valido cuando dicho valor es mayor a 0.
     * En parent e index deja anotada la arista con la que se llega a cada vertice hasta sink
     * Anula los dead-ends del level_graph cuando los encuentra.
     */
    long long find_augmenting_path(digraph<int>* level_graph, int from, int sink, vector<int>* parent, vector<int>* index) {
        while(not level_graph->at(from).empty()) {
            auto& e = residual[from][level_graph->at(from).back()];
            auto success = e.capacity-e.flow;
            if(success > 0 && e.to != sink) success = min(success, find_augmenting_path(level_graph, e.to, sink, parent, index));
            if(success == 0) {
                //arista saturada! o dead end!
                level_graph->at(from).pop_back();
                continue;
            }
            //camino encontrado.  Guardamos la informacion y retornamos el flujo
            parent->at(e.to) = e.from;
            index->at(e.to) = level_graph->at(from).back();
            return min(success, e.capacity - e.flow);
        }
        //no habia augmenting path
        return 0;
    }
    
    /**
     * Busca iterativamente los augmenting paths, aumentandolos hasta tener un blocking flow.  Retorna
     * el valor del flujo aumentado.
     * Requiere: que ninguna arista este en el level graph, ya que se calcula en esta etapa.
     */
    long long find_blocking_flow(int source, int sink) {
        auto level_graph = find_level_graph(source);
        
        vector<int> parent(residual.size(), noparent);
        vector<int> index(residual.size(), 0);
        long long ans=0, volume;
        
        while((volume = find_augmenting_path(&level_graph, source, sink, &parent, &index)) > 0) {
            for(int to = sink; parent[to] != noparent; to = parent[to]) {
                auto& e = residual[parent[to]][index[to]];
                e.flow += volume; 
                reverse(e).flow -= volume;
            }
            ans += volume;
        }
        return ans;
    }
    
    
public:
    flowgraph(int n = 0) {
        residual.assign(n, vector<edge>());
    }
    
    void add_edge(int from, int to, long long capacity) {
        if(capacity <= 0) return;
        residual.resize(max(residual.size(), (size_t)max(from+1, to+1))); // tamano dinámico
        auto idxfrom = (int)residual[from].size(), idxto = (int)residual[to].size();
        residual[from].push_back(edge{from, to, capacity, 0, idxto});
        residual[to].push_back(edge{to, from, 0, 0, idxfrom});
    }
    
    /**
     * Carga en this el flujo maximo de source a sink.  Notar que this podria
     * tener un flujo precargado y lo modifica para tener el flujo maximo.
     * Retorna todo el flujo que se pudo agregar.
     */
    long long maxflow(int source, int sink) {
        long long res = 0, step;        
        while((step = find_blocking_flow(source, sink)) > 0) {
            res += step;
        }
        return res;
    }

    void print(ostream& out) {
        for(int f = 0; f < residual.size(); ++f) {
            out << f << ": ";
            for(auto e : residual[f]) {
                auto& rev = reverse(e);
                out << "(" << e.from << "," << e.to << "," << e.capacity << "," << e.flow << ") {" 
                    << rev.from << "," << rev.to << ","  << rev.capacity << "," << rev.flow << "}  -  ";
            }
            out << endl;
        }
    }
};
}

int main(){
	ios::sync_with_stdio(0); cin.tie(0);
	int n;
	while(cin >> n){
		int src = 0, sink = 1, nxt = 2;

		dinics::flowgraph g;

		vector<int> columnas(n);
		for (auto& ci : columnas){
			ci = nxt++;
			g.add_edge(ci, sink, 1);
		}
		int lastF;
		for(int i = 0; i < n; i++){
			lastF = nxt++;
			g.add_edge(src, lastF, 1);
			for (int j = 0; j < n; j++){
				char c;
				cin >> c;
				if (c == '.'){
					g.add_edge(lastF, columnas[j], 1);
				}else{
					columnas[j] = nxt;
					g.add_edge(nxt++, sink, 1);
					lastF = nxt;
					g.add_edge(src, nxt++, 1);
				}
			}
		}

		cout << g.maxflow(src, sink) << '\n';

	}
	return 0;
}

/* Explicacion
 *
 * La idea del algoritmo es crear un grafo sobre el cuál buscar el flujo
 * máximo, donde una unidad de flujo simboliza una torre.
 * 
 * Si uno no tuviese los peones, podría tener un grafo bipartito con 2*n nodos,
 * donde un grupo de nodos simbolizan las filas, y otro grupo las columnas.
 * Y una arista entre la fila i y la col j, sería la celda (i, j). Luego, 
 * un flujo máximo sobre esta red (que es un matching máximo) es matchear la fila
 * i con la columna i, para cada 1 <= i <= n, como se dice en el enunciado.
 * 
 * La idea es llevar esto a un tablero con peones, donde, en vez de tener
 * filas o columnas, tendremos cachos de filas o de columnas, entonces, por ej,
 * si nuestro tablero es así:
 *  ..X..
 *  ..XX.		(rellenado con X para que sea cuadrado)
 *  
 * En vez de tener la fila 1, tendremos dos pedazos de la fila 1, el primero
 * se conecta con las columnas 1 y 2, y el segundo solo con la columna 5. Y de forma
 * similar para la srgunda fila, surge entonces que cada cacho, solo puede tener una torre,
 * y, intuitivamente, cualquier posicionamiento maximo de torres se condice con un flujo válido
 * en este grafo, y cualquier flujo válido sobre el grafo es un posicionamiento 
 * válido de las torres, por lo cual, basta con buscar un flujo máximo sobre
 * este grafo.
 * 
 * Al hacerlo con Dinics, y tener un grafo bipartito con capacidades iguales
 * a 1, obtenemos que la complejidad es O(sqrt(n)m), donde n es la cant de cachos
 * y m la cantidad de aristas.
 * 
 * Por un lado tenemos que la cant de cachos está acotada por N*N, es decir
 * 10000, pero además, la cant de aristas también, ya que, si tomo los
 * cachos de a grupos, según a que fila pertenezcam, cada grupo se conecta
 * con a lo sumo N cachos de columnas distintas, y tengo a lo sumo N grupos
 * de cachos por filas, por lo que m es O(N*N). La O está ya que cada cacho tiene,
 * además un eje a la fuente o al sumidero.
 * 
 * Luego, la complejidad por caso de test es de O(N*N*N).
 * 
 * Se uso el Dinics mandado por mail, haciendo que el tamaño
 * del grafo se expanda dinámicamente al agregar ejes.
 */
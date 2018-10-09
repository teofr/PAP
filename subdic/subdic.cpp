#include <vector>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <set>
#include <queue>
#include <algorithm>

using namespace std;

#define debug(v) //cerr << #v << ": " << v << endl;

int main(){
	int n;
	while(cin >> n, n > 0){
		unordered_map<string, vector<string>> g;
		unordered_map<string, int> indeg;

		// Leo
		for(int i = 0; i < n; i++){
			string line, word, def;
			cin >> word;
			getline(cin, line);
			stringstream is(line);
			auto& v = g[word];
			indeg[word];
			while(is >> def) {
				v.push_back(def);
				indeg[def]++;
			}
		}

		// TopoSort
		queue<string> prox;
		for(auto& n : indeg){
			if (!n.second){
				prox.push(n.first);
				debug(n.first);
			}
		}
		while(!prox.empty()){
			auto& s = prox.front();
			debug(s);
			for(auto& n : g[s]){
				debug(n);
				indeg[n]--;
				if (indeg[n] == 0)
					prox.push(n);
			}
			prox.pop();
		}

		// Solo quedan tipos en ciclos o que tienen tipos en ciclos que dependen de ellos
		// Recuperar
		vector<string> ans;

		for(auto& v : indeg){
			if (v.second)
				ans.push_back(v.first);
		}

		cout << ans.size() << endl;

		sort(ans.begin(), ans.end());

		for(int i = 0; i < ans.size(); i++){
			cout << ans[i] << (i < ans.size()-1 ? ' ' : '\n');
		}

	}
	return 0;
}


/* Explicacion
 * 
 * La idea del algoritmos viene de plantear el siguiente digrafo = {V, E}
 *  V son las palabras definidas por el diccionario,
 *  st \in E si s depende de t para ser definida, ie, la explicacion de s usa
 *  	a t
 *  	
 * Este grafo no define un orden, ya que las palabras puede tener dependencias ciclicas,
 * no es un DAG, de todas formas, pensemos que pasa si tomo los nodos s que tienen grado 
 * de entrada 0, nadie depende de ellos, por lo que los puedo sacar del grafo.
 * 
 * Teniendo G\{s}, que es un subdiccionario válido según el enunciado, puedo aplicar
 * la misma transformación y quitar algún nodo con grado de entrada 0.
 * 
 * En algún punto, llego a que, G' es vacio (sin nodos), o tengo todos los nodos
 * con grado de entrada mayor a 0, notemos lo siguiente:
 * 		+ es un subdiccionario válido, ya que a cada paso, parto de un diccionario, y
 * 			obtengo un subdicc válido (un subdicc es un dicc)
 * 		+ puedo generar el dicc original, agregando las palabras/nodos en el orden inverso
 * 			al que las saque.
 * 		+ es el más chico que cumple esas dos cosas, ya que todo lo que me queda, o
 * 			pertenece a un ciclo de palabras, que no puedo sacar de a una [1], o algún ciclo
 * 			depende de él (directamente o por transitividad), por lo que,
 * 			para sacarlo, tengo que sacar primero al ciclo.
 * 			Más en detalle, se ve que los ciclos de palabras siempre tienen que estar (porque
 * 			no hay forma de formarlos agregando de a una palabra), y como tienen que estar,
 * 			también deben estar los que necesitan cada palabra del ciclo (transitivamente), y los que quedan son
 * 			exactamente los que quedaron.
 * 	
 * Complejidad
 * 
 * La complejidad es sencilla, veamosla por partes del codigo:
 *  + Lectura:
 *  	O(n * 30 * 25), ya que, tengo n definiciones, cada una con a lo sumo 30*25 caracteres.
 *  + TopoSort (hasta que me trabo):
 *  	O(n) accesos al hash de tamaño n, se podría mejorar, pero n es chico, así que no importa.
 *  	proceso n nodos, y 30*n aristas, a lo sumo
 *  + Recuperar solucion
 *  	O(nlogn) comparaciones de string de largo <= 25, 
 *  	ya que tengo que recoger los valores resultantes, y ordenarlos.
 *  	
 * 
 * 	+ Luego, la complejidad por caso es de O(nlogn), con una constante relativamente alta (ya que son)
 * 	  O(nlogn) operaciones con strings.
 * 			
 * 			 
 * 			  
 * [1] según el enunciado no hay ejes entre un nodo (ss \not\in E) 
 */
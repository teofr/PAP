#include "bits/stdc++.h"

using namespace std;

#define debug(v) cerr << #v << ": " << v << endl;

struct node{
	int fail, set, check;
	int nxt[2];
};

vector<node> trie;

vector<int> roots;

struct nodetonto{
	int set;
	int nxt[2];
};

vector<nodetonto> trietonto;

void rebuild(){
	queue<pair<int, int>> q;
	int start = roots.back();
	q.push({start, start});
	trie[start].fail = start;
	trie[start].check = start;
	while(!q.empty()){
		pair<int, int> proxp = q.front(); q.pop();
		int prox = proxp.first; int p = proxp.second;
		for (int i = 0; i < 2; i++){
			if (trie[prox].nxt[i] != -1)
				q.push({trie[prox].nxt[i], prox});
		}
		// calculo el fail
		int fail = trie[p].fail;
		int move = trie[p].nxt[1] == prox ? 1 : 0;

		while(prox != start){
			if (trie[fail].nxt[move] != -1 and trie[fail].nxt[move] != prox){
				trie[prox].fail = trie[fail].nxt[move];
				break;
			}
			if (fail == start){
				trie[prox].fail = start;
				break;
			}
			fail = trie[fail].fail;
		}

		// Calculo el check
		int check = trie[prox].fail;
		if (trie[check].set){
			trie[prox].check = check;
		}else {
			trie[prox].check = trie[check].check;
		}
		
	}
}

int aho(const vector<int>& s){
	int ans = 0;
	for (auto r : roots){
		int idx = 0;
		int prox = r;
		while(idx < s.size()){
			while(trie[prox].nxt[s[idx]] == -1 and prox != r){
				prox = trie[prox].fail;
			}
			if (trie[prox].nxt[s[idx]] != -1){
				prox = trie[prox].nxt[s[idx]];
			}
			if (trie[prox].set){
				ans++;
			}
			int checks = trie[prox].check;
			while(checks != r){
				ans++;
				checks = trie[checks].check;
			}
			idx++;
		}
	}
	return ans;
}
#ifndef NN
	#define NN 3300
#endif
// unordered_set<string> existen;
int main(){
	ios::sync_with_stdio(0); cin.tie(0);
	int t, caso = 1;
	cin >> t;
	const int N = NN;
	vector<string> q;
		vector<int> ss;
	// roots.reserve(2*N);
	// trie.reserve(2*N*N);
	while(t--){
		cout << "Case #" << caso++ << ":\n";
		roots.clear();
		trie.clear();
		trietonto.clear();
		trietonto.push_back({0, -1, -1});
		// existen.clear();
		int n; 
		cin >> n;
		roots.push_back(0);
		node clr;
		clr.fail = 0;
		clr.check = 0;
		clr.set = 0;
		clr.nxt[0] = clr.nxt[1] = -1;
		trie.push_back(clr);
		int nxt = 1;
		// unordered_map<string> exists;
		int l = 0;
		bool broken = true; // true sii el ultimo trie esta roto
		string s; 
		q.clear();
		long long nmas=0, npreg=0, smas=0, spreg=0;
		while(n--){
			cin >> s;
			q.push_back(s);
			if (s[0] == '+'){
				nmas++;
				smas += s.size() -1;
			}else{
				npreg++;
				spreg += s.size() - 1;
			}
		}
		long long B2 = (smas*spreg)/npreg;
		B2 /= 3; // Da buenos tiempos...
		// long long B2 = N*N;
		debug(B2);
		for(auto& s : q){
			ss.clear();
			int start = l % (s.size() -1);
			for(int i = start+1; i < s.size(); i++){
				ss.push_back(s[i] - '0');
			}
			for (int i = 1; i < start+1; i++)
				ss.push_back(s[i] - '0');
			// for(auto si : ss)
				// cerr << si;
			// cerr << endl;
			if (s[0] == '+'){
				int found = -1;
				int prox, proxs;
				// for(int i = 0; i < roots.size() and found == -1; i++){
				// 	// lo busco
				// 	prox = roots[i];
				// 	proxs = 0;
				// 	while(proxs < ss.size() and trie[prox].nxt[ss[proxs]] != -1){
				// 		prox = trie[prox].nxt[ss[proxs]];
				// 		proxs++;
				// 		if (proxs == ss.size() and trie[prox].set == 1){
				// 			found = i;
				// 			break;
				// 		}
				// 	}
				// }
				// int prox2 = prox, proxs2 = proxs;
				// string sss;
				// for (auto si : ss){
					// sss += to_string(si);

				// }
				// cerr << sss << endl;
				// if (existen.find(sss) == existen.end()){
					// existen.insert(sss);
				prox = 0; proxs = 0;
				while(proxs < ss.size() and trietonto[prox].nxt[ss[proxs]] != -1){
					prox = trietonto[prox].nxt[ss[proxs]];
					proxs++;
				}
				found = proxs == ss.size() and trietonto[prox].set;
				if (!found){
					prox = 0; proxs = 0;
					while(proxs < ss.size()){
						if (trietonto[prox].nxt[ss[proxs]] == -1){
							trietonto.push_back({0, -1, -1});
							trietonto[prox].nxt[ss[proxs]] = trietonto.size() - 1;
						}
						prox = trietonto[prox].nxt[ss[proxs]];
						proxs++;
					}
					trietonto[prox].set = 1;
					// lo agrego en el último
					prox = roots.back();
					proxs = 0; // Creo que ya deberían estar seteados bien, pero bue
					while(proxs < ss.size() and trie[prox].nxt[ss[proxs]] != -1){
						prox = trie[prox].nxt[ss[proxs]];
						proxs++;
					} 
					// assert(prox == prox2);
					// assert(proxs == proxs2);
					while(proxs < ss.size()){
						trie.push_back(clr);
						trie.back().fail = roots.back();
						trie.back().check = roots.back();
						// trie.back().set = 1;
						trie[prox].nxt[ss[proxs]] = nxt;
						prox = nxt++;
						proxs++;

					}
					trie[prox].set = 1;
					broken = true;
				}
				
			}else{
				

				if (broken){
					rebuild();
					broken = false;
				}
				


				l = aho(ss);
				cout << l << '\n';
				if ((nxt - roots.back())*(nxt - roots.back()) > B2){
					trie.push_back(clr);
					roots.push_back(nxt++);
					trie.back().check = roots.back();
					trie.back().fail = roots.back();
				}
			}
		}

	}

	return 0;
}

/* Explicacion
 *
 * La idea del algoritmo es casi igual a la vista en clase, pero levemente
 * distinta, con el objetivo de no tener que combinar nuestros tries.
 * 
 * Para hacer la explicación más fácil:
 *   - s+ es la suma total de las palabras que se agregan, s+ < 10^5
 *   - s? es la suma total de las palabras que se preguntan, s? < 5*10^6
 *   - n+ es la cant de queries de tipo +
 *   - n? es la cant de queries de tipo ?
 *   		n+ + n? < 10^5
 * 
 * Entonces, la idea es fijar un número B, que es el tamaño máximo que vamos
 * a permitir que tengan nuestros tries (más o menos) y, siempre vamos a tener
 * un trie activo, sobre el cual trabajaremos, y otros terminados (con por lo
 * menos B nodos). Luego, para responder cada query:
 * 		- + tomamos la palabra que debemos agregar, buscamos que no este
 * 		 	definida, si no está la agregamos al trie activo (rompiendo 
 * 		 	el invariante de este). [1]
 * 		- ? tomamos el parrafo, si el trie activo esta roto, lo arreglamos
 * 		  	y buscamos cuantas palabras definidas en todos los tries
 * 		  	aparecen en mi parrafo. Además, si el parrafo activo tiene más
 * 		  	de B elementos, creamos uno nuevo, que pasa a ser el activo.
 * 
 * Luego, responder cada query de tipo + cuesta 
 * |s|*s+/B		  				+ |s|
 * (buscar si ya se definio)	  (definirlo en el trie activo)
 * Entonces, todas las queries + juntas cuestan s+*s+/B + s+
 * 
 * Para las queries de tipo ?
 * B 						+ |s|*s+/B
 * (arreglar el trie activo)  (buscar la palabra en los s+/B tries)
 * Entonces, para todas las ? cuesta n?*B + s?*s+/B
 * 
 * Entonces, al buscar el óptimo para B (derivando y probando valores
 * cercanos experimentalmente), llegamos aprox a 3500.[2]
 * 
 * Pero, para calcular la complejidad tomamos B = sqrt(s+), y
 * otemenos que la complejidad por caso de prueba es de:
 * s+*sqrt(s+) + s+ + n?*sqrt(s+) + s?*sqrt(s+)
 * Donde el término que domina es s?*sqrt(s+)  <-- En el caso mayor
 * y este ronda los 5*10^8. 
 * 
 * El sentido de por qué conviene un numero mayor a sqrt(s+) debe ser
 * porque el termino dominante tiene a B dividiendo, y el único término
 * con B multiplicando no agrega mucho valor.
 * 		 	
 * 		 	
 * [1]: A modo de optimización se hizo un trie tonto para guardar todas las 
 * palabras definidas, pero no el calulo de compl sobre esto.
 * 
 * [2]: También se intento calcularlo dinámicamente y mejoró un poco 
 * la complejidad. Se tuvo que evitar sqrt porque era muy lento.
 */
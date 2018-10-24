#include "bits/stdc++.h"

using namespace std;


struct grupo{
	vector<char> pir;
	int buc, mult, inv;
};

void cut(map<int, grupo>& g, int idx){
	auto it = g.upper_bound(idx);
	it--; // el mas grande menor o igual
	if (it->first != idx){
		// si no esta cortado justo en idx, lo corto ahí
		grupo g3 = it->second;
		int step = idx - it->first;
		// while(!it->second.pir.size());
		int g0_mult = step / it->second.pir.size();
		g3.mult = it->second.mult - g0_mult;
		it->second.mult = g0_mult;

		if (step % it->second.pir.size()){
			// si el ind corta el vector por la mitad...
			int rest = step % it->second.pir.size();
			grupo g1, g2;
			g1.mult = g2.mult = 1;
			g1.buc = g2.buc = 0;
			g1.inv = g2.inv = it->second.inv;
			for (int i = 0; i < rest; i++){
				g1.pir.push_back(g3.pir[i]);
				if((!g1.inv and g1.pir.back()) or 
					(g1.inv and !g1.pir.back())) {
					g1.buc++;
				}
			}
			for (int i = rest; i < g3.pir.size(); i++){
				g2.pir.push_back((g3.pir[i]));
				if((!g2.inv and g2.pir.back()) or 
					(g2.inv and !g2.pir.back())) {
					g2.buc++;
				}
			}
			g3.mult--;

			g[it->first + it->second.pir.size()*it->second.mult + g1.pir.size()] = g2;
			if (g3.mult)
				g[it->first + it->second.pir.size()*it->second.mult + g1.pir.size() + g2.pir.size()] = g3;
			g[it->first + it->second.pir.size()*it->second.mult] = g1;

		}else{
			g[it->first + it->second.pir.size()*it->second.mult] = g3;
		}
	}
}

int main(){
	ios::sync_with_stdio(0); cin.tie(0);
	int t;
	cin >> t;
	int caso = 0;
	map<int, grupo> interv;
	while(++caso <= t){
		cout << "Case " << caso << ":\n";

		int m; cin >> m;
		interv.clear();
		int from = 0;

		string t;
		for(int i = 0; i < m; i++){
			grupo g;
			g.buc = 0;
			g.inv = 0;
			cin >> g.mult;
			cin >> t;
			for (auto& xi : t){
				g.pir.push_back(xi - '0');
				g.buc += xi - '0';
			}

			interv[from] = g;
			from += t.size()*g.mult;
		}
		grupo gtrampa;
		interv[from] = gtrampa;

		int q; cin >> q;
		int godcnt = 1;
		while(q--){
			// cerr << i<< endl;
			char c;
			int a, b;
			cin >> c >> a >> b; 
			b++; // busco [a, b)
			cut(interv, a);
			cut(interv, b); // mi map de intervalos esta cortado en a y b
			auto itb = interv.find(b);
			if (c == 'F'){
				auto it = interv.find(a);
				it->second.mult = b - a;
				it->second.pir = {1};
				it->second.buc = 1;
				it->second.inv = 0;
				it++;
				while(it != itb){
					it = interv.erase(it);
				}
			}else if (c == 'E'){
				auto it = interv.find(a);
				it->second.mult = b - a;
				it->second.pir = {0};
				it->second.buc = 0;
				it->second.inv = 0;
				it++;
				while(it != itb){
					it = interv.erase(it);
				}

			}else if (c == 'I'){
				auto it = interv.find(a);
				while(it != itb){
					it->second.inv = 1 - it->second.inv;
					it->second.buc = it->second.pir.size() - it->second.buc;
					it++;
				}

			}else{
				int res = 0;
				auto it = interv.find(a);
				while(it != itb){
					// cerr << it->first << endl;
					// long long tmp = it->second.mult*it->second.buc;
					res += it->second.mult*it->second.buc;
					it++;
				}
				cout << "Q" << godcnt++ << ": " << res << '\n';

			}
		// while(1);
		}

	}
	return 0;
}

/* Explicacion
 * La idea es hacer el algoritmo fuerza bruta inteligentemente, primero
 * veamos las cotas:
 * 		M <= 100: número de grupos de piratas distintos
 * 		T <= 200: cuantas veces repito cada grupo
 * 		|s| <= 50: tamaño del string que representa cada grupo
 * 		Q <= 1000: queries
 * 
 * La idea va a ser guardar el string resultado que simboliza toda la tierra
 * de piratas de forma particionada, donde cada partición es un grupo cómo se
 * describe en la entrada, es decir, cada grupo tiene como información su string,
 * su multiplcididad (cuantas veces aparece ese string) y su posición inicial en
 * el string resultante (Pirate's land). Entonces, antes de comenzar con las queries,
 * tenemos a lo sumo M grupos, donde cada uno tiene un string de tamaño menor a 50.
 * 
 * Para responder la query c a b haremos lo siguiente:
 * 		+ Primero vamos a cortar mis particiones, para que tenga un grupo
 * 			que comience en a y uno que comience en b+1, al hacer esto, 
 * 			responder/actualizar es fácil
 * 		+ Si la query requiere cambiar el tipo de los piratas a Buc/Bar, 
 * 			simplemente elimino todos los grupos que había entre a y b+1
 * 			y creo uno nuevo, donde el string que lo representa es 1 o 0,
 * 			y la cant de veces que se repite es b-a. Este grupo comienza en a.
 * 		+ Si la query requiere invertir, simplemente invierto todos los grupos
 * 			entre a y b.
 * 		+ Si la query requiere contar, cuento todos los grupos entre a y b.
 * 		
 * 
 * Primero notamos que por query, yo agrego a lo sumo 6 grupos, que salen de 
 * la operación de corte. Es decir, que a lo sumo puedo tener 6*Q+M grupos (6100).
 * Luego, cada query cuesta a lo sumo O(Q+M) operaciones de O(1), mas dos operaciones
 * logarítmicas (que es lo que cuesta cortar usando un map). Al tener a lo sumo Q quieries,
 * cada caso lo respondo en O(Q(Q+M+log(Q+M))) = O(Q*(Q+M)), es decir que es lo suficientemente bueno.
 * 
 * Sin hacer un análisis profundo, uno podría ver que la complejidad amortizada es mucho mejor,
 * a partir de estas observaciones:
 * 		+ Si tomo solo las dos primeras queries, el costo amortizado por query es O(log(...)),
 * 			ya que cada grupo entra y sale una vez.
 * 		+ Las queries caras son las otras dos, pero que esa query cueste caro, requiere 
 * 			tener muchas veces queries no tan caras.
 * 		+ Cortar en un índice puede generar 0, 1 o 3 grupos, el costo de 3 solo se puede dar
 * 			cuando la mult > 2 y el tamaño del string > 1. Y, si hago 3 cortes, quedan
 * 			dos grupos con mult == 1. Además, las queries F y E me dejan grupos con
 * 			tamaño del string == 1. Con intuición y fé, se ve que la mayoría de 
 * 			los cortes solo genera 1 grupo extra.
 * 
 * En main2.cpp se intentó optimizar la implementación notando que cada string
 * de cada grupo es un número binario de a lo sumo 50 dígitos, por lo que entra 
 * en un int de 64 bits, y podemos contar la cant de 1, invertirlo y copiarlo
 * en tiempo constante.
 */
#include <bits/stdc++.h>

using namespace std;

vector<vector<long long>> memo; // memo[i][j] precio de llegar a j, con i vuelos
vector<unordered_map<int, long long>> ady;

const long long INF = 1e17;

long long f(int fl, int d){
	if (fl < 0) return INF;
	if (d == 0) return 0;
	// cerr << fl << endl << d << endl;
	if (memo[fl][d] == INF){
		memo[fl][d] = f(fl-1, d);
		for(auto p : ady[d]){
			memo[fl][d] = min(memo[fl][d], p.second + f(fl-1, p.first));

		}
	}

	return memo[fl][d];
}


int main(){
	ios::sync_with_stdio(0); cin.tie(0);
	int k;
	cin >> k;

	int sc = 1;


	while(k--){
		int n; cin >> n;

		// memo = vector<vector<int>> (n+10, vector<int> (n, -1));

		unordered_map<string, int> m;

		for(int i = 0; i < n; i++){
			string t;
			cin >> t;
			m[t] = i;
		}

		// ady = vector<vector<pair<int, long long>>> (n);
		// memo = vector<vector<long long>> (n+10, vector<long long> (n, INF));
		ady.clear(); ady.resize(n);
		memo.clear(); memo.resize(n+10, vector<long long> (n, INF));

		int M; cin >> M;
		for(int i = 0; i < M; i++){
			string a, b;
			long long w;
			cin >> a >> b >> w;
			int aa = m[a], bb = m[b];
			auto it = ady[bb].find(aa);
			if (it == ady[bb].end()) ady[bb][aa] = INF;
			ady[bb][aa] = min(ady[bb][aa], w);
			// ady[bb].push_back({aa, w});

		}

		cout << "Scenario #" << sc++ << '\n';

		int q; cin >> q;
		for(int i = 0; i < q; i++){
			int ans;
			int fl; cin >> fl; fl++;
			fl = min(fl, n+1);
			if (f(fl, n-1) < INF){
				cout << "Total cost of flight(s) is $" << f(fl, n-1) << '\n';
			}else{
				cout << "No satisfactory flights" << '\n';
			}
		}

		if (k) cout << '\n';
	}

	return 0;
}

/* EXPLICACION
 * 
 * La idea del algoritmo viene de que tenemos que calcular distancias minimas en un DAG
 * (que ya lo tenemos ordenado), para una cantidad de vuelos fija.
 * 
 * Como las ciudades vienen en un orden A, B, C, ... E, y nunca hay vuelos "hacia atras",
 * yo puedo plantear una dinámica que se fija el costo de volar a cierta ciudad
 * x usando exactamente fl vuelos, y la defino de esta forma:
 * 
 * f(fl, x) = min(min_{y tq existe vuelo de y a x, de costo w}(w + f(fl-1, y)), f(fl-1, x))
 * 
 * Es decir, en cada paso puedo, o perder un vuelo, o usar uno desde alguna ciudad anterior a la mia.
 * 
 * 
 * La complejidad de esto es sencillo, por haber visto ya los temas de DP, ya que tengo
 * 100 ciudades y a lo sumo 1000 vuelos (aunque se ve que solo puedo usar hasta 100 vuelos),
 * luego, me queda una complejidad de O(100*1000). Para esto, obviamente, preciso memoizar.
 * 
 * Además, hay cierto cómputo que no estoy contando que viene de tomar los nombres de las ciudades como strings y pasarlos
 * a enteros, pero esto es poco, ya que los nombres de las cidades tienen a lo sumo 100 caracteres. 
 * 
 * 
 */
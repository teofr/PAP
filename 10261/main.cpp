#include <vector>
#include <map>
#include <iostream>
#include <string>

using namespace std;

#define debug(v) cerr << #v << v << endl;

vector<int> sum_autos;
map<pair<int, int>, int> memo;
vector<vector<int>> memo2;
int long_ferry;

int max_autos(int k1, int i){
	if (i > sum_autos.size() - 1){
		return i-1;
	}
	int k2 = sum_autos[i-1] - k1;

	int long_auto = sum_autos[i] - sum_autos[i-1];

	// if (memo.find(make_pair(k1, i)) == memo.end()){
	// 	if (long_auto + k1 <= long_ferry and long_auto + k2 <= long_ferry){
	// 		memo[make_pair(k1, i)] = max(max_autos(k1, i+1), max_autos(k1 + long_auto, i+1));
	// 	} else if (long_auto + k1 > long_ferry and long_auto + k2 <= long_ferry){
	// 		memo[make_pair(k1, i)] = max_autos(k1, i+1);
	// 	} else if (long_auto + k1 <= long_ferry and long_auto + k2 > long_ferry){
	// 		memo[make_pair(k1, i)] = max_autos(k1 + long_auto, i+1);
	// 	} else {
	// 		memo[make_pair(k1, i)] = i-1;
	// 	}

	// }

	if (memo2[k1][i] == -1){
		if (long_auto + k1 <= long_ferry and long_auto + k2 <= long_ferry){
			memo2[k1][i] = max(max_autos(k1, i+1), max_autos(k1 + long_auto, i+1));
		} else if (long_auto + k1 > long_ferry and long_auto + k2 <= long_ferry){
			memo2[k1][i] = max_autos(k1, i+1);
		} else if (long_auto + k1 <= long_ferry and long_auto + k2 > long_ferry){
			memo2[k1][i] = max_autos(k1 + long_auto, i+1);
		} else {
			memo2[k1][i] = i-1;
		}
	}

	return memo2[k1][i];
	
}

void imprimir(int k1, int i){ // Imprime donde pongo el auto i si ya llene k1 del primer carril
	if (i > sum_autos.size() - 1){
		return;
	}
	int k2 = sum_autos[i-1] - k1;

	int long_auto = sum_autos[i] - sum_autos[i-1];

		if (long_auto + k1 <= long_ferry and long_auto + k2 <= long_ferry){
			if (max_autos(k1, i+1) >= max_autos(k1 + long_auto, i+1)){
				cout << "starboard" << '\n';
				imprimir(k1, i+1);
			} else {
				cout << "port" << '\n';
				imprimir(k1+long_auto, i+1);
			}

		} else if (long_auto + k1 > long_ferry and long_auto + k2 <= long_ferry){
			cout << "starboard" << '\n';
			imprimir(k1, i+1);
		} else if (long_auto + k1 <= long_ferry and long_auto + k2 > long_ferry){
			cout << "port" << '\n';
			imprimir(k1+long_auto, i+1);
		}


}

int main() {
	int casos;
	cin >> casos;
	while (casos-- > 0){
		sum_autos.clear();
		memo.clear();

		cin >> long_ferry;
		long_ferry *= 100; // Lo paso a cm

		sum_autos.push_back(0);
		int long_auto;
		cin >> long_auto;
		while (long_auto > 0){
			sum_autos.push_back(long_auto + *(--sum_autos.end()));
			cin >> long_auto;
		}
		memo2= vector<vector<int>> (long_ferry+10, vector<int> (sum_autos.size(), -1));


		int ans;

		cout << (ans = max_autos(0, 1)) << '\n';

		// Ahora voy a recuperar el camino maximo, viendo la memoizacion
		imprimir(0,1);		

		if (casos) cout << '\n';
	}

	return 0;
}
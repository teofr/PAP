#include <vector>
#include <iostream>

using namespace std;



int main(){
	ios_base::sync_with_stdio(0); 
	cin.tie(0);  
	int c, n;
	cin >> c >> n;

	vector<vector<int>> ady (c, vector<int> (c, 0));

	for (int i = 0; i < n; i++){
		int a, b;
		cin >> a >> b;
		a--; b--;
		// ady[a].push_back(b);
		// ady[b].push_back(a);
		ady[a][b] = ady[b][a] = 1;
	}

	vector<vector<vector<int>>> memo (c, vector<vector<int>> (2, vector<int> (c, 0)));


	for (int i = 0; i < c; i++){
		// memo[prox].push_back({i, 1});
		memo[0][0][i] = memo[0][1][i] = 1;
	}
	for (int tam = 2; tam <= c; tam++){
		int last = tam -2;
		int prox = tam -1;
		for (int i = 0; i < c; i++){
			memo[prox][0][i] = (ady[i][(i-1+c)%c] and memo[last][0][(i-1+c)%c]) or
								(ady[i][(i-tam+c)%c] and memo[last][1][(i-tam+c)%c]);  // el coso [i, i-tam]

			memo[prox][1][i] =  (ady[i][(i+1)%c] and memo[last][1][(i+1)%c]) or
								(ady[i][(i+tam)%c] and memo[last][0][(i+tam)%c]); // el coso [i, i+tam]
		}

		

	}
	int i;

	for ( i = 0; i < c; i++){
		if (memo[c -1][0][i]){
			break;
		}
	}

	if (i == c){
		cout << -1 << endl;
		return 0;
	}

	int dir = 0;
	for (int tam = c-1; tam > 0; tam--){
		cout << i+1 << endl;
		int sg = dir ? 1 : -1;
		if (ady[i][(i+sg+c)%c] and memo[tam][dir][(i+sg+c)%c]){
			i = (i+sg+c)%c;
		}else{
			dir = 1 - dir;
			i = (i+sg*(tam+1)+c)%c;
		}

	}

	cout << i+1;




	return 0;
}
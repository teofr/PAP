#include <vector>
#include <iostream>
#include <bitset>

#define INF 1e8

using namespace std;
#define CMAX 1000

// vector<vector<int>> ady ;
bitset<CMAX> ady [CMAX];


int c, n;

#define debug(v) //cerr << #v << ": " << v << endl;
// vector<vector<vector<int>>> memo;
short int memo[CMAX][CMAX][2];
// memo codifica cual sigue en el camino
// 0 => [l, r-1, ultR]
// 1 => [l, r-1, !ultR]
// 2 => [l+1, r, !ultR]
// 3 => [l+1, r, ultR]

bool f(int l, int r, bool ultR){
	if (memo[l][r][ultR] == -2){
		if (l == r){
			memo[l][r][ultR] = 4;
		}else{
			int ln, rn;
			if (ultR){
				ln = l;
				rn = (r-1+c)%c;
			}else{
				ln = (l+1)%c;
				rn = r;
			}
			if (ady[l][r] && f(ln, rn, !ultR)){
				memo[l][r][ultR] = (ultR ? 1 : 3);
			}else if (ady[ultR ? r : l][ultR ? rn : ln] && f(ln, rn, ultR)){
				memo[l][r][ultR] = (ultR ? 0 : 2);
			}else{
				memo[l][r][ultR] = -1;
			}
		}

	}
	return memo[l][r][ultR] >= 0;
}

// bool f(int l, int r, bool ultR){ // Puedo haber visitado todos los [l,r] siendo el ultimo ultR => r, !ultR => l
// 	if (l == r) {
// 		memo[l][r][ultR] = 4;
// 		return true;
// 	}
// 	if (memo[l][r][ultR] == -2){
// 		if (ultR){
// 			if (ady[r][(r-1+c)%c] && f(l, (r-1+c)%c, true)){
// 				memo[l][r][ultR] = 0;
// 			}else if (ady[l][r] && f(l, (r-1+c)%c, false)){
// 				memo[l][r][ultR] = 1;
// 			}else{

// 				memo[l][r][ultR] = -1;
// 			}
// 		}else{
// 			if (ady[l][(l+1)%c] && f((l+1)%c, r, false)){
// 				memo[l][r][ultR] = 2;
// 			}else if (ady[l][r] && f((l+1)%c, r, true)){
// 				memo[l][r][ultR] = 3;
// 			}else{
// 				memo[l][r][ultR] = -1;
				
// 			}

// 		}
// 	}

// 	return memo[l][r][ultR] >= 0;
// }

int main(){
	ios_base::sync_with_stdio(0); 
	cin.tie(0);  
	cin >> c >> n;

	// ady.resize(c, vector<int>(c, 0));
	for (int i = 0; i < c; i++){
		for (int j = 0; j < c; j++){
			ady[i][j] = 0;
			memo[i][j][0] = memo[i][j][1] = -2;
		}
	}

	// memo.resize(c, vector<vector<int>> (c, vector<int> (2, -2)));
	// return 0;
	for(int i = 0; i < n; ++i){
		int a, b;
		cin >>a >>b ;
		a--; b--;
		ady[a][b] = ady[b][a] = 1;
	}


	int i;
	for ( i = 0; i < c; i++){
		if (f(i, (i-1+c)%c, true) /* or f(i, (i-1+c)%c, false)*/){
			break;
		}
	}

	if (i == c){
		cout << -1 << endl;
		return 0;
	}

	bool ultR = f(i, (i-1+c)%c, true);

	int l = i, r = (i-1+c)%c;

	while (l!=r){
		int next = memo[l][r][ultR];
		if (next < 2){
			cout << r+1 << '\n';
			r = (r-1+c)%c;
			ultR = next == 0;
		}else{
			cout << l+1 << '\n';
			l = (l+1)%c;
			ultR = next == 3;
		}
	}

	cout << l+1;



	return 0;
}
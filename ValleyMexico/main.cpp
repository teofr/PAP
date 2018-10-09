#include <vector>
#include <iostream>

using namespace std;

vector<vector<int>> ady ;


int c, n;

vector<vector<vector<int>>> memo;
// memo[l][r][ultR] codifica cual sigue en el camino
// 0 => [l, r-1, true]
// 1 => [l, r-1, false]
// 2 => [l+1, r, false]
// 3 => [l+1, r, true]

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

	ady.resize(c, vector<int>(c, 0));
	memo.resize(c, vector<vector<int>> (c, vector<int> (2, -2)));
	for (int i = 0; i < c; i++){
		for (int j = 0; j < c; j++){
			ady[i][j] = 0;
			memo[i][j][0] = memo[i][j][1] = -2;
		}
	}

	for(int i = 0; i < n; ++i){
		int a, b;
		cin >> a >>b ;
		a--; b--;
		ady[a][b] = ady[b][a] = 1;
	}


	int i;
	for (i = 0; i < c; i++){
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

	cout << (l+1)%c;



	return 0;
}


/***
	EXPLICACION

	la idea es plantear la siguiente funcion recursiva

	f(int l, int r, bool ultR), que devuelve true sii yo puedo visitar todas las ciudades en [l, r] 
	(con aritmetica modular) habiendo visitado ultimo r si ultR o l si !ultR.

	esta funcion se puede definir de la siguiente forma:

	f(l, r, ultR) = { true si l == r
					{ (ady[r][r-1] y f(l, r-1, true)) o (ady[r][l] y f(l, r-1, false)) si ultR
					{ (ady[l][l+1] y f(l+1, r, false)) o (ady[r][l] y f(l+1, r, true)) si !ultR

	Donde estamos haciendo backtracking en todas las posibles formas de continuar el camino, si tengo que visitar
	las ciudades [l, r] saliendo de (ultR ? r : l).

	Para resolver el problema, tenemos que simplemente considerar la f que sale de cada ciudad y las visita todas,
	es decir, que el problema tiene solucion sii Existe i tq f(i+1, i, true)

	Al analizar la f, vemos que tiene una complejidad exponencial, ya que en el peor caso por cada llamado a f vuelvo a, 
	llamar dos veces a f con l-r decrementado por uno (una profundiad de backtracking de c), pero solo hay |c|*|c|*2
	subinstancias posibles, es decir O(c**2) instancias posibles, es decir que memoizando bajamos la complejidad del problema
	a cuadratica, ya que cada instancia se calcula a lo sumo una vez, y toma tiempo constante si ya tiene sus subinstancias ya
	calculadas (memoizacion).

	Implementativamente, cuando memoizamos guardamos mas informacion que simplemente true/false, ya que nos ayuda a recuperar
	el camino final, ademas se dejo comentada una version que muestra mas claramente cada caso.

	Ademas, recuperar el camino nos cuesta linear en c.



	Por que usamos matriz de adyacencia:

	Teniendo en cuenta que tengo que memoizar O(c**2) subinstancias, y esto si tiene sentido hacerlo en una matriz, ya tengo
	un tiempo de O(c**2) de inicializacion, por lo cual no me molesta inicializar tambien la matriz de adyacencia, que
	luego hace que preguntar si dos ciudades son adyacentes mas facilmente (y mas rapidamente)

*/
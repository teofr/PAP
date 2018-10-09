#include <vector>
#include <iostream>

using namespace std;


#define N 0
#define R 1
#define G 2
#define B 3

#define H 10
#define W 15


int board[H][W], vis[H][W];

int xd[] = {0, 0, 1,-1};
int yd[] = {1,-1, 0, 0};

char colors[] = {'N', 'R', 'G', 'B'};

inline bool inrange(int i, int j){
	return i >= 0 and i < H and j >= 0 and j < W;
}

inline void clean(int a[H][W]){ 
	for (int i = 0; i < H; i++){
			for (int j = 0; j < W; j++) a[i][j] = N;
		}
}

int dfs(int i, int j, bool clear = false){
	if (!vis[i][j]){
		vis[i][j] = 1;
		int res = 1;
		for (int h = 0; h < 4; h++){
			int ni = i + xd[h], nj = j + yd[h];
			if (inrange(ni, nj) and board[i][j] == board[ni][nj]){
				res += dfs(ni, nj, clear);
			}
		}

		if (clear) board[i][j] = N;

		return res;
	}
	return 0;
}

inline int arreglarCol(int j){
	int prox = H-1;
	for (int i = H-1; i >= 0; i--){
		if (board[i][j]){
			board[prox][j] = board[i][j];
			if (prox != i) board[i][j] = 0;
			prox--;
		}
	}
	return H - 1 - prox; // cuantos elementos no nulos tiene
}

inline void arreglar(){
	int prox = 0;
	for(int j = 0; j < W; j++){
		if (arreglarCol(j)){
			for (int i = 0; i < H; i++){
				board[i][prox] = board[i][j];
				if (prox != j) board[i][j] = 0;
			}
			prox++;
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0); 
	cin.tie(0);
	int cases, cnt = 0;
	cin >> cases;
	while(cases--){
		cout << "Game " << ++cnt << ":\n\n";
		// Limpiar O(150)
		clean(board);

		// Leo O(150)
		for (int i = 0; i < H; i++) for (int j = 0; j < W; j++){
			char c;
			cin >> c;

			if (c == 'R') board[i][j] = R;
			else if (c == 'G') board[i][j] = G;
			else board[i][j] = B;
		}

		// Elijo el mejor
		int mi = -1, mj = -1, mejor = 1;
		int score = 0, move = 1;
		do{
			mi = -1; mj = -1; mejor = 1;
			clean(vis); // O(150)
			for (int j = 0; j < W; j++){ // O(150)
				for (int i = H-1; i >= 0; i--){
					int prox;
					if (board[i][j] != N and mejor < (prox = dfs(i, j))){
						mi = i; mj = j; mejor = prox;
					}
				}
			}
			if (mi != -1){
				clean(vis); // O(150)
				int color = board[mi][mj];
				int tam = dfs(mi, mj, true); // O(150) (total)
				score += (tam-2)*(tam-2);
				cout << "Move " << move++ << " at (" << H - mi << "," << mj+1 << "): removed " << tam 
				<< " balls of color " << colors[color] <<", got " << (tam-2)*(tam-2) << " points.\n";
			}
			// Arreglo
			arreglar(); // O(150)

		}while (mi != -1);

		int balls = 0;
		for(int i = 0; i < H; i++) for (int j = 0; j < W; j++){
			if (board[i][j]) balls++;
		}
		if (!balls) score += 1000;
		cout << "Final score: " << score << ", with " << balls << " balls remaining.\n";
		if (cases) cout << '\n';
		




	}


	return 0;

}

/*
 * La solucion planteada es, algoritmicamente, muy sencilla, ya que sigue la estrategia descripta en 
 * el enunciado al pie de la letra.
 * 
 * La idea es, mientras haya algun grupo de fichas conectadas entre si (del mismo color y conexas en el tablero), elijo
 * el grupo mas grande (la componente conexa con mas nodos), eligiendo la de mas a la izq y mas abajo de haber mas de una.
 * 
 * Una vez que la encontre, la quito del tablero, calculo cuantos puntos consegui, registro el movimiento y
 * arreglo el tablero.
 * 
 * Como el tablero es extremadamente chico, nos tomamos la libertad de hacer un arreglo a lo bruto, sin considerar mucho
 * ideas mas sofisticadas.
 * 
 * Para encontrar la comp conexa mas grande, utilizamos un DFS que recorre el grafo desde un nodo en particular, y retorna
 * el tamaño de la com conexa que contiene a dicho nodo. Como no hace falta tener los 3 colores (blanco, gris, negro), nos
 * alcanza con tener un vector de visitado, donde marcamos los nodos que ya descubrimos.
 * 
 * Teniendo en cuenta que el grafo tiene un tamaño acotado, todo el algoritmo es constante, pero para calcular la complejidad
 * un poco más en profundidad, consideremos un tablero de tamaño N (en este caso N=150), que contiene al rededor de 4*N aristas,
 * es decir, M = O(N).
 * 
 * Eso significa que, hasta que no encuentre un grupo de nodos con mayor tamaño que uno, debo hacer otro paso de la estrategia del juego,
 * pero cada vez que entro elimino por lo menos 2 fichas (las seteo como negras), es decir que el ciclo de las lineas [99, 121] se
 * ejecuta O(N) veces. 
 * 
 * Luego, cada vez que ejecuto el ciclo, debo:
 * 		+ Limpiar el vector de visitados O(N)
 * 		+ Recorrer todas las comp conexas y encontrar la mayor, que es O(N) ya que proceso cada nodo una sola vez y M = O(N)
 * 		+ Eliminar la comp más grande y arreglar el tablero O(N), notar que podríamos achicar esta complejidad, ya que 
 * 		  solo elimino cada nodo una vez, pero no hace diferencia en la cuenta final.
 * 
 * Es decir que este ciclo tiene una complejidad de O(N^2). Como dijimos arriba, seguro se puede mejorar, pero teniendo en
 * cuenta que N = 150, probablemente no lo valga.
 * 
 * Fuera de este ciclo, se hacen varios calculos de O(N) de complejidad.
 * 
 * Por lo tanto, la complejidad por caso de prueba es de O(N^2)
 */
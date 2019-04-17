#include <bits/stdc++.h>

using namespace std;

int main(){
	int n, l, k; cin >> n >> l >> k;
	vector<double> prob(n);
	vector<int> prize(n);
	for (int i = 0; i < n; i++){
		cin >> prob[i];
		prob[i] /= 100.0;
	}
	for (int i = 0; i < n; i++){
		cin >> prize[i];
	}

	const int KMAX = 420;

	double dp[n+10][KMAX][n+10]; // dp[i][j][k] es la prob de haber llegado al iesimo, sin combatirlo
				// magos, habiendo llegado a ese punto con j - 210 de capacidad y habiendo ganado 
				// k encuentros

	for(int i = 0; i < n+10; i++){
		for (int j = 0; j < KMAX; j++){
			for (int k = 0; k < n+10; k++){
				dp[i][j][k] = 0;
			}
		}
	}
	dp[0][k + 210][0] = 1.0;

	for (int i = 0; i < n; i++){
		for (int j = 0; j < KMAX; j++){
			for (int k = 0; k < n + 10; k++){
				double proba = dp[i][j][k];
				dp[i+1][j][k] += proba*(1.0 - prob[i]);
				dp[i+1][min(410, j + prize[i])][k+1] += proba*prob[i];
				// dp[i][j][k] = (1.0 - prob[i-1])*dp[i-1][j][k] + prob[i-1]*dp[i-1][j - prize[i-1]][k-1];
			}
		}
	}

	double ans = 0;
	for (int j = 210; j < KMAX; j++){
		for (int k = l; k < n+10; k++){
			ans += dp[n][j][k];
		}

	}
	cout << setprecision(20) << fixed << ans << endl;

	return 0;
}

/* EXPLICACION
 * 
 * La idea es sencilla, primero notamos que no importa con cuanta capacidad terminamos,
 * solamente que terminemos con cap no negativa, entonces, tener 2000 de capacidad es
 * lo mismo que tener 200, ya que 200 es lo maximo que podemos necesitar.
 * 
 * A partir de esto, planteo una DP[i][j][k], que me dice la probabilidad de haber 
 * peleado contra los [0, i) magos, llegando con una capacidad de j y con k duelos ganados.
 * 
 * Trivialmente, si K es la capacidad inicial DP[0][K][0] = 1.0, y todo lo demas 0.
 * 
 * Y luego de completar esta matriz, el resultado es la suma de las probabilidades
 * de DP[n][j][k], para k >= l y j >= 0.
 * 
 * 
 * Complejidad
 * 
 * La complejidad de llenar toda nuestra matriz de subinstancias es de O(n*n*KMAX) = O(200*400*200)
 * tanto en tiempo como en memoria. Rearmar la solución cuesta O(KMAX*n), por lo que la
 * complejidad final es O(n*n*KMAX).
 */
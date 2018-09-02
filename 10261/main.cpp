#include <vector>
#include <map>
#include <iostream>
#include <string>

using namespace std;

vector<int> sum_autos; // vector aditivo con las long de los autos
vector<vector<int>> memo2;
int long_ferry;

int max_autos(int k1, int i){
	if (i > sum_autos.size() - 1){
		// Si no me quedan mas autos
		return i-1;
	}
	int k2 = sum_autos[i-1] - k1;

	int long_auto = sum_autos[i] - sum_autos[i-1];

	// k1 = long ocupada en uno de los lados
	// k2 = long ocupada en el otro
	// i  = el proximo auto a meter
	// long_auto = la long del auto iesimo


	if (memo2[k1][i] == -1){  // Si no esta memoizado
		if (long_auto + k1 <= long_ferry and long_auto + k2 <= long_ferry){  
			// Si el auto entra en cualquier carril
			memo2[k1][i] = max(max_autos(k1, i+1), max_autos(k1 + long_auto, i+1));
		} else if (long_auto + k1 > long_ferry and long_auto + k2 <= long_ferry){
			// Si el auto solo entra en el lado k2
			memo2[k1][i] = max_autos(k1, i+1);
		} else if (long_auto + k1 <= long_ferry and long_auto + k2 > long_ferry){
			// si el auto solo entra en el auto k1
			memo2[k1][i] = max_autos(k1 + long_auto, i+1);
		} else {
			// si el auto no entra en ningun lado (no puedo seguir metiendo)
			memo2[k1][i] = i-1;
		}
	}

	return memo2[k1][i];
	
}

void imprimir(int k1, int i){ // Imprime donde pongo el auto i si ya llene k1 del primer carril
	// Hace de nuevo todas las llamadas a max_autos, pero todo ya esta memoizado
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

		cin >> long_ferry;
		long_ferry *= 100; // Lo paso a cm

		sum_autos.push_back(0);
		int long_auto;
		cin >> long_auto;
		while (long_auto > 0){
			sum_autos.push_back(long_auto + *(--sum_autos.end()));
			cin >> long_auto;
		}
		memo2 = vector<vector<int>> (long_ferry+10, vector<int> (sum_autos.size(), -1));


		cout << max_autos(0, 1) << '\n';

		// Ahora voy a recuperar el camino maximo, viendo la memoizacion
		imprimir(0,1);		

		if (casos) cout << '\n';
	}

	return 0;
}


/***************************************************************
 ******** EXPLICACION ******************************************
 *        ===========
 * 
 * Primero definimos la siguiente funcion f que resuelve el problema,
 * f toma k1, k2 e i, que representan cuantos cm se llenaron
 * del primer carril, cuantos del segundo y cual es el proximo auto 
 * a insertar. Ademas, K indica la long total del barco.
 * f(k1, k2, i) =  	{ si l[i] + k1 > K y l[i] + k2 > K entonces i - 1 
 * 						(no puedo insertar el iesimo auto, inserte i-1)
 *					{ si l[i] + k1 <= K y l[i]+ k2 > K entonces f(k1 + l[i], k2, i+1)
 *						(solo lo puedo insertar en el primer carril)
 *				 	{ si l[i] + k2 <= K y l[i] + k1 > K entonces f(k1, k2 + l[i], i+1)
 *				 		(solo lo puedo insertar en el segundo carril)
 *					{ sino max(f(k1 + l[i], k2, i+1), f(k1, k2 + l[i], i+1))
 *						(lo inserto donde me convenga)
 *						
 * Es trivial ver que esta funcion soluciona el problema, ya que recorre todas
 * las subinstancias validas y siempre se queda con la mejor (solo el caso donde usa max).
 * La funcion max_autos implementa a f.
 *						
 * Como las capacidades las mido en cm, 0 <= k1,k2 <= 10000, ademas, trivialmente 
 * se ve que a lo sumo puedo usar 10000*2/100 autos, ya que cada auto mide por lo
 * menos 100cm 0 <= i <= 200
 * 
 * Luego, puedo acotar la cant de instancias por 10000\*10000\*200 = 2*10^10
 * Este numero es demasiado grande como para calcular todas las subinstancias del problema, 
 * es decir que incluso usando PD y calculando cada instancia una sola vez, tardaria demasiado.
 * 
 * Pero notemos que si f(k1, k2, i) es una instancia a la que puedo llegar a traves de la recurrencia,
 * una instancia valida, entonces k1 + k2 = sum l[j], j \in [1..i), 
 * entonces la instancia (k1, k2, i) la puedo codificar como (k1, i) (y calcular k2 a partir de estos), 
 * luego, la cant de subinstancias (validas) son 10000\*200 = 2*10^6, un numero manejable.
 * 
 * De este calculo podemos deducir facilmente la complejidad, que viene de calcular cada subinstancia a lo sumo una vez,
 * y tener O(long_ferry*cant_autos) subinstancias, ademas, por lo visto arriba vemos que podemos restringir la cant de autos
 * a a lo sumo, 200 [2]. Como esto es para cada caso, nos queda O(casos*long_ferry*cant_autos).
 * 
 * Un tema a tener en cuenta fue como imprimir la salida, ya que debiamos recuperar el camino recorrido
 * por la funcion, para esto implementamos la funcion imprimir, que tiene se llama una sola vez a si misma
 * y se ejecuta a lo sumo la maxima cant de autos que entran, es decir que se ejecuta O(f(0,1)) veces. Ademas, 
 * tiene varios llamados a la funcion f, pero que son respondidos en tiempo constante, ya que en este punto ya
 * esta todo memoizado.
 * 
 * 
 * [1] Cabe destacar que si utilizamos la primera recursion, la que toma instancias de la pinta (k1, k2, i)
 * y la hubiesemos implementado con la tecnica top down, utilizando algun diccionario bueno para claves ralas 
 * (map) la cantidad de subinstancias que hubiese calculado (y guardado) hubiesen sido acotadas por 2*10^6.
 * 
 * [2] Esto no lo hicimos ya que no fue necesario para conseguir el Accepted en UVA, y asumimos que no van a
 * pasar como entrada muchos mas autos de los que podrian entrar.
 * 
 */
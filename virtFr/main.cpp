#include "bits/stdc++.h"

using namespace std;

class UF{ 
public:
	UF(int n = 0): p(n, -1), sz(n, 1){};

	int Find(int c){return (p[c] == -1 ? c : p[c] = Find(p[c]));};

	int Union(int a, int b){
		int ra = Find(a), rb = Find(b);
		if (ra == rb) return sz[ra];
		if (sz[ra] < sz[rb]) swap(ra, rb);
		p[rb] = ra;
		return sz[ra] = sz[ra] + sz[rb];
	};

private:
	vector<int> p, sz;
};


int main(){
	ios::sync_with_stdio(0); cin.tie(0);
	int t;
	cin >> t;
	int m, cnt;
	unordered_map<string, int> idx;
	vector<pair<int, int>> edges;
	while(t--){
		idx.clear();
		edges.clear();
		cin >> m;
		cnt = 1;

		for(int i = 0; i < m; ++i){
			string a, b;
			cin >> a >> b;
			if (!idx[a]) idx[a] = cnt++;
			if (!idx[b]) idx[b] = cnt++;

			edges.push_back({idx[a], idx[b]});
		}

		UF uf(cnt+1);
		for (auto e : edges){

			cout << uf.Union(e.first, e.second) << '\n';
		}
	}
}

/* Explicacion
 *
 * El problema es exactamente Union Find, donde queremos saber (para cada componente),
 * la cantidad de elementos en él.
 * 
 * Dividimos el problema en dos, la primera parte consta en otorgarle a cada nombre un numero,
 * esto lo hacemos con un hash, donde cada vez que aparece un nombre por primera vez, le genero
 * un nuevo codigo (fresco) y guardo esa información en el hash. Si ya habia aparecido, recupero
 * su codigo. Luego, agrego ese eje a un vector de ejes, cambiando nombres por números.
 * Esto tiene un costo de O(m) accesos al hash, donde tomamos como m a la cantidad de amistades
 * formadas, en principio no tenemos un límite a la cantidad de máxima de personas distintas,
 * pero sabemos que, cómo mucho, es 2m.
 * 
 * Luego, vamos a crear una estructura Union Find con la cant de nombres distintos encontrados
 * y, para cada eje en orden, vamos a unir esos grupos de amigos y retornar el tamaño del grupo
 * recién creado.
 * 
 * La complejidad temporal por caso es O(m(acc + ack^-1)), donde acc son accesos a hash, y ack^-1
 * es la inversa de Ackerman. Al tener t casos, obtengo una complejidad de O(tm(acc+ack^-1)).
 */
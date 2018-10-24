#include "bits/stdc++.h"

using namespace std;


struct grupo{
	unsigned long long pir; 
	int sz, mult;
};
map<int, grupo> interv;

void cut(int idx){
	auto it = interv.upper_bound(idx);
	it--; // el mas grande menor o igual
	int itidx = it->first;
	// cerr << "CUT: " << idx << endl;
	if (itidx != idx){
		// si no esta cortado justo en idx, lo corto ahí
		grupo& g0 = it->second;
		grupo g3 = g0;
		int step = idx - itidx;
		int g0_mult = step / g0.sz;
		g3.mult = g0.mult - g0_mult;
		g0.mult = g0_mult;

		if (step % g0.sz){
			g3.mult--;
			// si el ind corta el vector por la mitad...
			int rest = step % g0.sz;
			grupo g1, g2;
			g1.mult = g2.mult = 1;
			g1.sz = rest;
			// while(!g1.sz);
			g2.sz = g0.sz - rest;
			// while(!g2.sz);
			g2.pir = (g0.pir & ((1ll << (g2.sz)) - 1ll));
			g1.pir = ((g0.pir) >> (g2.sz));


			if (g3.mult){
				interv[itidx + g0.sz*g0.mult + g1.sz + g2.sz] = move(g3);
			}
			interv[itidx + g0.sz*g0.mult + g1.sz] = move(g2);
			interv[itidx + g0.sz*g0.mult] = move(g1);

		}else{
			// cerr << "OPT2: " << it->first + g0.sz*g0.mult << endl; 
			interv[itidx + g0.sz*g0.mult] = move(g3);
		}
	}
}

int main(){
	ios::sync_with_stdio(0); cin.tie(0);
	int t;
	cin >> t;

	int caso = 0;
	while(++caso <= t){
		cout << "Case " << caso << ":\n";

		int m; cin >> m;
		interv.clear();
		int from = 0;

		for(int i = 0; i < m; i++){
			grupo g;
			g.pir = 0;
			cin >> g.mult;
			string t;
			cin >> t;
			g.sz = t.size();
			for (auto& xi : t){
				g.pir = g.pir << 1;
				g.pir = g.pir | (xi - '0');
			}

			interv[from] = g;
			from += g.sz*g.mult;
		}
		grupo gtrampa;
		gtrampa.sz = 0;
		interv[from] = gtrampa;

		int q; cin >> q;
		int godcnt = 1;
		while(q--){
			// cerr << i<< endl;
			char c;
			int a, b;
			cin >> c >> a >> b; 
			b++; // busco [a, b)
			if (interv.find(a) == interv.end()) cut(a);
			if (interv.find(b) == interv.end()) cut(b); // mi map de intervalos esta cortado en a y b
			auto itb = interv.find(b);
			if (c == 'F'){
				auto it = interv.find(a);
				it->second.mult = b - a;
				it->second.pir = 1;
				it->second.sz = 1;
				it++;
				while(it != itb){
					it = interv.erase(it);
				}
			}else if (c == 'E'){
				auto it = interv.find(a);
				it->second.mult = b - a;
				it->second.pir = 0;
				it->second.sz = 1;
				it++;
				while(it != itb){
					it = interv.erase(it);
				}

			}else if (c == 'I'){
				auto it = interv.find(a);
				while(it != itb){
					it->second.pir = (~(it->second.pir)) & ((1ll << it->second.sz) -1ll);
					it++;
				}

			}else{
				int res = 0;
				auto it = interv.find(a);
				while(it != itb){
					// cerr << it->first << endl;
					// long long tmp = it->second.mult*it->second.buc;
					res += it->second.mult*(__builtin_popcountll(it->second.pir));
					it++;
				}
				cout << "Q" << godcnt++ << ": " << res << '\n';

			}
		// while(1);
		}

	}
	return 0;
}
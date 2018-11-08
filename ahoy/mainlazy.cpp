#include "bits/stdc++.h"

using namespace std;

#define DNSET 0
#define DNCLR 1
#define DNINV 2

#define MAXN 1048576

struct nodo{
	int dirty, cnt, down;
};

nodo segtree [2*MAXN + 10];

void clean(int node, int ll, int rr){
	// assert((rr-ll) == 1 or (rr - ll) %2 == 0);
	if (rr - ll <= 1) 
		return;
	if (segtree[node].dirty){
		// switch(segtree[node].down){
		// 	case DNINV:
		// 		segtree[node].cnt = ll - rr - segtree[node].cnt;
		// 		break;
		// 	default:
		// 		segtree[node].cnt = (ll - rr)*(1-segtree[node].down);
		// 		break;
		// }
		int m = (ll + rr)/2;
		clean(node*2, ll, m);
		clean(node*2+1, m, rr);
		segtree[node*2].dirty = 1;
		segtree[node*2].down = segtree[node].down;
		switch(segtree[node].down){
			case DNINV:
				segtree[node*2].cnt = m - ll - segtree[node*2].cnt;
				break;
			case DNSET:
				segtree[node*2].cnt = m - ll;
				break;
			default:
				// assert(segtree[node].down == DNCLR);
				segtree[node*2].cnt = 0;
				break;
		}
		segtree[node*2+1].dirty = 1;
		segtree[node*2+1].down = segtree[node].down;
		switch(segtree[node].down){
			case DNINV:
				segtree[node*2+1].cnt = rr - m - segtree[node*2+1].cnt;
				break;
			case DNSET:
				segtree[node*2+1].cnt = rr - m;
				break;
			default:
				// assert(segtree[node].down == DNCLR);
				segtree[node*2+1].cnt = 0;
				break;
		}
		// for(int i = node*2; i < node*2+2; i++){
		// 	segtree[i].dirty = 1;
		// 	segtree[i].down = segtree[node].down;
		// 	// cerr << "CLN ll: " << ll << " rr: " << m << endl;
		// 	switch(segtree[node].down){
		// 		case DNINV:
		// 			segtree[i].cnt = m - ll - segtree[i].cnt;
		// 			break;
		// 		default:
		// 			segtree[i].cnt = (m - ll)*(1-segtree[i].down);
		// 			// break;
		// 	}
		// 	ll = m;
		// 	m = rr;
		// }
		segtree[node].dirty = 0;
	}
}

int query(int l, int r, int node = 1, int ll = 0, int rr = MAXN){
	if (l >= rr or r <= ll or l >= r)
		return 0;
	clean(node, ll, rr);
	if (l == ll and r == rr){
		// cerr << "QUE, l: " << l << " r: " << r << " ll: " << ll << " rr: " 
	// << rr << " result: " << segtree[node].cnt << endl;
		return segtree[node].cnt;
	}
	int m = (ll+rr)/2;
	int ans = query(l, min(r, m), node*2, ll, m) + query(max(l, m), r, node*2+1, m, rr);
	// cerr << "QUE, l: " << l << " r: " << r << " ll: " << ll << " rr: " 
	// << rr << " result: " << ans << endl;

	return ans;
}



void upd(int l, int r, int change, int node = 1, int ll = 0, int rr = MAXN){
	if (l >= rr or r <= ll or l >= r)
		return;
	clean(node, ll, rr);
	if (l == ll and r == rr){
		segtree[node].dirty = 1;
		segtree[node].down = change;
		switch(change){
			case DNINV:
				segtree[node].cnt = rr - ll - segtree[node].cnt;
				break;
			default:
				segtree[node].cnt = (rr - ll)*(1-segtree[node].down);
				break;
		}
	// cerr << "UPD, l: " << l << " r: " << r << " ll: " << ll << " rr: " << rr << " result: " << segtree[node].cnt << endl;
		return;
	}
	int m = (ll+rr)/2;
	upd(l, min(r, m), change,  node*2, ll, m);
	upd(max(l, m), r, change, node*2+1, m, rr);
	segtree[node].cnt = segtree[node*2].cnt + segtree[node*2+1].cnt;
	// cerr << "UPD, l: " << l << " r: " << r << " ll: " << ll << " rr: " << rr << " result: " << segtree[node].cnt << endl;

	return;
}




int main(){
	ios::sync_with_stdio(false);
	cin.tie(0);
	int t;
	cin >> t;
	int caso = 0;
	while(caso++ < t){
		cout << "Case " << caso << ":" << endl;
		// for (int i = 0; i < 4*MAXN+10; i++)
			// segtree[i] = {0, 0, 0};
		int m; cin >> m;
		int idx = 0;
		while(m--){
			int t; cin >> t;
			string p; cin >> p;
			while(t--){
				for (auto& x : p){
					int a = (x == '1' ? DNSET : DNCLR);
					upd(idx, idx+1, a);
					idx++;
				}
			}
		}


		int q; cin >> q;
		int godcnt = 1;
		while(q--){
			// // cerr << i<< endl;
			char c;
			int a, b;
			cin >> c >> a >> b; 
			b++; // busco [a, b)
			if (c == 'F'){
				upd(a, b, DNSET);
			}else if (c == 'E'){
				upd(a, b, DNCLR);

			}else if (c == 'I'){
				upd(a, b, DNINV);

			}else{
				cout << "Q" << godcnt++ << ": " << query(a, b) << '\n';

			}
		}


	}


	return 0;
}

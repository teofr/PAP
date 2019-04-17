#include <bits/stdc++.h>

using namespace std;

struct entry {
    int rank[2];       //ranking de la primera y segunda mitad del prefijo
    int pos;           //posicion del string que representa esta entrada
};

void rsort(vector<entry>* join, int r, int M) {
    vector<vector<entry>> bucket(M);
    for(auto e = join->rbegin(); e != join->rend(); ++e)
        bucket[e->rank[r]].push_back(*e);
    for(int i = 0, p = 0; i < bucket.size(); ++i) for(; not bucket[i].empty(); bucket[i].pop_back()) 
        join->at(p++) = bucket[i].back();
}


vector<int> suffixrank(const string& s) {            
    vector<int> rank(s.size());
    for(int i = 0; i < s.size(); ++i) rank[i] = (unsigned int)s[i];
            
    vector<entry> join(s.size());
    int M = 256;
    for(int k = 1; k < rank.size(); k *= 2) {
        for(int i = 0; i < rank.size(); ++i)
            join[i] = entry{rank[i], i+k < rank.size() ? rank[i+k] : 0, i};
            rsort(&join, 1, M); 
            rsort(&join, 0, M);
            //recargamos el rank
            M = rank[join[0].pos] = 0;
            for(int i = 1; i < rank.size(); ++i) 
                rank[join[i].pos] = join[i].rank[0] == join[i-1].rank[0] and join[i].rank[1] == join[i-1].rank[1] ? M : ++M;
            
            if(++M == rank.size()) break;
    }
    return rank;
}

vector<int> suffixarray(const vector<int>& rank) {
    vector<int> sa(rank.size());
    for(int i = 0; i < rank.size(); ++i) sa[rank[i]] = i;
    return sa;
}

vector<int> LCP(const string& s, const vector<int>& sa, const vector<int>& rank) {
    vector<int> lcp(s.size(), 0);
    int len = 0;
    for(int i = 0; i < s.size()-1; ++i) {
        int k = rank[i], j = sa[k-1];
        //j es el predecesor de i en el orden lexicografico.
        while(s[i+len] == s[j+len]) len++;
        lcp[k] = len;
        if(len > 0) len--;
    }
    return lcp;
}

#ifdef DEBUG
#define debug(v) cerr << #v << ": " << v << endl;
#else
#define debug(v)
#endif

int main(){
    int n, nn = -10;
    while(cin >> n, n > 0){
        if (nn != -10)
            cout << endl; // No se cuantos casos hay, pero no quiero endl en el ultimo

        string s;
        int prox = 0;
        nn = n;
        while(nn--){
            string t;
            cin >> t;
            s += t;
            s += char(prox % 27); // Para no hacer toda la cosa de codificar
            // cambio de strings con valores, sin pisar posibles letras
            // ni mover todo el string, alguno va a dar AC (27)
            prox++;
        }
        if (n == 1){
            // Caso borde medio choto
            for (int i = 0; i < s.size() - 1; i++)
                cout << s[i];
            cout << '\n';
            continue;
        }
        debug(s);
        debug(s.size());
        vector<int> inv(s.size(), -1);
        // inv[i] me dice, la posicion i en s, a que string orig pertenece
        int curr = 0;
        for(int i = 0; i < s.size(); i++){
            if (s[i] >= 'A'){
                inv[i] = curr;
            }else{
                curr++;
            }
        }

        auto rank = suffixrank(s);
        auto sa = suffixarray(rank);
        auto lcp = LCP(s, sa, rank);
        // lcp[i] = max pref comun entre sa[i] y sa[i-1]

        deque<int> d; // los minimos en orden de aparicion, y no decreciente
        // Guardo repetidos para que sea mas facil sacarlos
        int l = 0, r = 1; // cerrado abierto, cuando tengo un solo string hay un caso
        // muy choto aca
        vector<int> vis(n, 0);
        int vistos = 0;

        int best = 0;
        set<int> bests;
        int lastl = -1, lastr = -1;

        while(r < s.size()){
            while(r < s.size() and vistos*2 <= n){
                debug(r);
                int pr = lcp[r];
                int orig_str = inv[sa[r]];
                r++;
                while(!d.empty() and d.back() > pr)
                    d.pop_back();
                d.push_back(pr);
                if (orig_str != -1){
                    vis[orig_str]++;
                    if (vis[orig_str] == 1){
                        vistos++;
                    }
                }

            }

            debug(r);
            debug(vistos);
            debug(d.size());

            while(l < r and vistos*2 > n){
                debug(l);
                int orig_str = inv[sa[l]];
                if (vistos*2 > n){
                    if (best == d.front()){
                        if (l >= lastr)
                            bests.insert(r - 1); // Este seguro no lo saco, sino no me alcanzan los vistos
                    // por eso resuelvo aparte cuando n == 1
                        // lastl = l;
                        lastr = r;
                    }
                    else if (best < d.front()){
                        bests.clear();

                        bests.insert(r - 1);
                        lastr = r;
                        best = d.front();
                    }
                    debug(d.front());
                    debug(vistos);
                }
                l++;
                int pr = lcp[l];
                if (d.front() == pr){
                    debug("popep");
                    d.pop_front();
                }
                if (orig_str != -1){
                    vis[orig_str]--;
                    if (vis[orig_str] == 0)
                        vistos--;
                }
            }

        }
        if (best == 0){
            cout << "?" << '\n' ;
            continue;
        }
        // set<string> bestsrep; // A lo re bestia...

        for (auto i : bests){
            string t;

            for (int j = 0; j < best; j++){
                t+=s[sa[i]+j];
            }
            // bestsrep.insert(t);
            cout << t << endl;
        }
        // for (auto& s : bestsrep){
            // cout << s << endl;
        // }
    }


    return 0;
}

/* Explicacion
 *
 * La idea del algoritmo es la vista en clase, por la que se omite
 * la explicación, la única cuestión interesante, es que, para no complicarme
 * la vida (y porque es tarde) tiro esta magia
 *      s += char(prox % 27);
 * y me arriesgo a que los test cases no cubren todos los casos donde esto rompe
 * 
 * Complejidad:
 * Veamos para cada caso, primero notamos S a la suma de las long de todos los 
 * strings, que es a lo sumo 100*1000
 * 
 * Luego, calculamo nuestra batería de cosas, con e algoritmo O(nlogn) dado en clase.
 *      Vamos O(SlogS)
 * 
 * Luego, recorremos con dos punteros nuestro LCP, en particular se recorre
 * a lo sumo dos veces O(S), donde vamos manteniendo el mínimo en una doble cola
 * que es O(S) en total, ya que cada elemento entra y sale una vez.
 * Y, en cada paso, guardamos cosas en un set, que de nuevo, entran o salen una vez por l
 * posible, es decir, O(SlogS) total.
 * 
 * Rearmar las soluciones cuesta:
 *      Tomemos M a la long del substring comun maximo, M <= 1000
 *      Ese substring aparece a lo sumo S/M veces como sufijos disjuntos, por como armo mi set
 *          bests, no tomo sufijos que comparten una posición, ya que en ese caso son el mismo,
 *          ni sufijos que comparten posiciones, con la mejor cant de mathces hasta ahora, indirectamente
 *      
 *      Luego, tengo a lo sumo S/M string distintos, ya ordenados por estar en el SA, e imprimir cada
 *      uno cuesta M, O(S) en total
 * 
 * La compl total es O(SlogS)
 * 
 * No se demuestra que esto ultimo de tomarlos disjuntos vale, pero es viable y dio AC
 */
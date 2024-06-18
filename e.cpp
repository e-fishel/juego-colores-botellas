#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using vll = vector <ll>;

ll SIZE = 4;
const ll X = 2999999, X2 = 2983597, MOD = 1E9+7;
map <ll, string> rmp;
map <string, ll> mp;
ll c = 1;
vll freq(100, 0);

struct Botella {
    bool mc;
    vll col;
    ll hash;
    ll id;

    Botella (): mc(true), col(0) { assert(false); }
    Botella (vll col, ll id): mc(col.size() == SIZE || col.size() == 0), col(col), id(id) {
        for (ll i : col) mc &= i == col.back();
        hash = 1;
        ll acX = X2;
        for (ll i : col) {
            (hash += i*acX) %= MOD;
            (acX *= X2) %= MOD;
        }
    }

    bool operator< (const Botella &o) const {
        return hash < o.hash;
    }

    void print () {
        cout << col.size() << ": ";
        for (ll i : col) cout << rmp[i] << ' ';
        cout << '\n';
    }
};

pair <Botella, Botella> pasar (Botella a, Botella b) {
    vll acol = a.col, bcol = b.col;
    if (acol.size() > 0 && bcol.size() == 0) { // kickstart the process
        bcol.push_back(acol.back());
        acol.pop_back();
    }
    while (acol.size() > 0 && bcol.size() < SIZE && acol.back() == bcol.back()) {
        bcol.push_back(acol.back());
        acol.pop_back();
    }
    if (bcol.size() > SIZE) return { a, b }; // exceeds bottle cap
    return { Botella(acol, a.id), Botella(bcol, b.id) };
}

unordered_map <ll, bool> vis;
unordered_map <ll, vector <Botella> > unhash;
unordered_map <ll, ll> from;
unordered_map <ll, ll> dis;
unordered_map <ll, pair <ll, ll> > trans;
ll start;
struct Estado {
    vector <Botella> ve;
    ll n;
    ll hash;

    Estado (vector <Botella> ve): ve(ve), n(ve.size()) {
        vector <Botella> th = ve;
        sort(th.begin(), th.end());
        hash = 0;
        ll acX = X;
        for (Botella i : th) {
            (hash += acX*i.hash) %= MOD;
            (acX *= X) %= MOD;
        }
    }

    vector <Estado> transiciones () {
        vector <Estado> ans;
        for (ll i = 0; i < n; i++) {
            for (ll j = 0; j < n; j++) {
                if (i == j) continue;
                auto [a, b] = pasar(ve[i], ve[j]);
                if (a.hash == ve[i].hash) continue; // invalid move
                swap(ve[i], a);
                swap(ve[j], b);
                Estado th(ve);
                if (!vis[th.hash]) {
                    vis[th.hash] = true;
                    from[th.hash] = hash;
                    unhash[th.hash] = th.ve;
                    trans[th.hash] = { i, j };
                    ans.push_back(th);
                }
                swap(ve[i], a);
                swap(ve[j], b);
            }
        }
        return ans;
    }

    void print () {
        cout << setw(2) << n << '\n';
        vector <Botella> th = ve;
        sort(th.begin(), th.end(), [&](Botella a, Botella b) {
            return a.id < b.id;
        });
        for (Botella i : th) {
            cout << "  ";
            i.print();
        }
        cout << '\n';
    }

    bool mc () {
        bool ans = true;
        for (Botella i : ve) ans &= i.mc;
        return ans;
    }
};

Estado read () {
    ll n;
    cin >> n;
    vector <Botella> ve(n, Botella(vll({}), -15));
    ll id = 1;
    map <ll, ll> cou;
    for (Botella &i : ve) {
        ll k;
        cin >> k;
        vll col(k);
        for (ll &j : col) {
            string str;
            cin >> str;
            if (mp.count(str)) j = mp[str];
            else rmp[c] = str, j = (mp[str] = c++);
            cou[j]++;
        }
        // reverse(col.begin(), col.end());
        i = Botella(col, id++);
    }
    cout << "done reading\n" << flush;
    bool die = false;
    for (auto [a, b] : cou) {
        if (b%SIZE == 0) continue;
        cout << "uneven amount of " << rmp[a] << " (" << b << ") not a multiple of " << SIZE << '\n' << flush;
        die = true;
    }
    if (die) exit(0);
    cout << "correct input\n" << flush;
    Estado th(ve);
    start = th.hash;
    vis[th.hash] = true;
    return th;
}

void reconstruct (ll u) {
    // Estado(unhash[u]).print();
    if (u == start) return;
    reconstruct(from[u]);
    auto [i, j] = trans[u];
    cout << i+1 << " -> " << j+1 << '\n';
}

ll fin = -1;
void bfs () {
    queue <Estado> q;
    q.push(read());
    dis[q.front().hash] = 0;
    while (q.size()) {
        Estado u = q.front(); q.pop();
        if (freq[dis[u.hash]]++ == 0 && dis[u.hash])
            cout << "explored all " << setw(2) << dis[u.hash]-1 << "-dis states: " << setw(4) << freq[dis[u.hash]-1] << '\n' << flush;
        if (u.mc()) {
            fin = dis[u.hash];
            reconstruct(u.hash);
            cout << "solvable in " << dis[u.hash] << " moves\n";
            return;
        }
        for (Estado v : u.transiciones()) {
            q.push(v);
            dis[v.hash] = dis[u.hash]+1;
        }
    }
}

int main () {
    cin >> SIZE;
    bfs();
    if (fin == -1) cout << "no sol found\n" << flush;
    // for (ll i = 0; i <= fin; i++) cout << setw(2) << i << ": " << setw(4) << freq[i] << '\n';
    return 0;
}

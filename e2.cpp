#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using vll = vector <ll>;

ll SIZE = 4;
ll N = 20, FILL = N-1;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int main () {
    cin >> N;
    cin >> SIZE;
    cin >> FILL;
    vll ve(SIZE*FILL);
    for (ll i = 0; i < SIZE*FILL; i++) ve[i] = i/SIZE;
    shuffle(ve.begin(), ve.end(), rng);
    cout << SIZE << '\n' << flush;
    cerr << SIZE << '\n' << flush;
    cout << N << '\n' << flush;
    cerr << N << '\n' << flush;
    for (ll i = 0; i < FILL; i++) {
        cout << SIZE << ' ';
        cerr << SIZE << ' ';
        for (ll j = 0; j < SIZE; j++) {
            cout << ve[i*SIZE+j] << ' ';
            cerr << ve[i*SIZE+j] << ' ';
        }
        cout << '\n' << flush;
        cerr << '\n' << flush;
    }
    for (ll i = FILL; i < N; i++) {
        cout << "0\n" << flush;
        cerr << "0\n" << flush;
    }
    return 0;
}

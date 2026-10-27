#include<bits/stdc++.h>
using namespace std;

using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;

using i128 = __int128;
using u128 = unsigned __int128;

using f64 = double;
using ld = long double;

const int N = 1e4 + 5;
const int M = 5e4 + 5;
const int mod = 1e9 + 7;
const int INF = -1e9 - 7;
const f64 gama = 1e-9;

int n, m, q;

struct edg1 {
    int u, v;
    int limit;
    edg1(int a = 0, int b = 0, int c = 0) : u(a), v(b), limit(c) {}
    bool operator< (const edg1 &other) const {
        return limit > other.limit;
    }
}edge1[M];

int father[N];
int siz[N];
stack<int> st;

struct edg {
    int to, nxt;
    int limit;
}edge[M * 2];
int head[N], cnt = 0;

int power;
int deep[N];
int stjump[15][N];
int stmin[15][N];
int vis[N];

void init() {
    for (int i = 0; i <= n; ++i) {
        head[i] = 0;
    }
    cnt = 0;
}

void addEdge(int u, int v, int l) {
    edge[++cnt].to = v;
    edge[cnt].limit = l;
    edge[cnt].nxt = head[u];
    head[u] = cnt;
}

void build() {
    for (int i = 0; i <= n; ++i) {
        father[i] = i;
        siz[i] = 1;
    }
}

int find(int x) {
    while (x != father[x]) {
        st.push(x);
        x = father[x];
    }
    while (!st.empty()) {
        int t = st.top(); st.pop();
        father[t] = x;
    }
    return x;
}

void merge_set(int x, int y) {
    int fx = find(x);
    int fy = find(y);
    if (fx != fy) {
        if (siz[fx] >= siz[fy]) {
            siz[fx] += siz[fy];
            father[fy] = fx;
        } else {
            siz[fy] += siz[fx];
            father[fx] = fy;
        }
    }
}

void kruskal() {
    init();
    build();

    for (int i = 1; i <= m; ++i) {
        int u = edge1[i].u;
        int v = edge1[i].v;
        int lim = edge1[i].limit;
        if (find(u) == find(v)) continue;
        merge_set(u, v);
        addEdge(u, v, lim);
        addEdge(v, u, lim);
    }
}

void clean() {
    for (int i = 0; i <= n; ++i) {
        vis[i] = 0;
        for (int p = 0; p <= power; ++p) {
            stjump[p][i] = 0;
            stmin[p][i] = INT_MAX;
        }
    }
}

int lg2(int n) {
    int ans = 0;
    while ((1 << ans) <= (n >> 1)) {
        ans++;
    }
    return ans;
}

void dfs(int u, int fa, int l) {
    vis[u] = 1;
    if (fa == 0) {
        deep[u] = 1;
    } else {
        deep[u] = deep[fa] + 1;
    }
    stjump[0][u] = fa;
    stmin[0][u] = l;
    for (int p = 1; p <= power; ++p) {
        stjump[p][u] = stjump[p - 1][stjump[p - 1][u]];
        stmin[p][u] = min(stmin[p - 1][u], stmin[p - 1][stjump[p - 1][u]]);
    }
    for (int i = head[u]; i; i = edge[i].nxt) {
        int v = edge[i].to;
        int lim = edge[i].limit;
        if (v == fa) continue;
        dfs(v, u, lim);
    }
}

int query(int a, int b) {
    int dis = abs(deep[a] - deep[b]);
    int mn = INT_MAX;
    if (deep[a] > deep[b]) {
        for (int p = power; p >= 0; --p) {
            if ((1 << p) > dis) continue;
            mn = min(mn, stmin[p][a]);
            a = stjump[p][a];
            dis -= (1 << p);
            if (!dis) break;
        }
    }
    if (deep[b] > deep[a]) {
        for (int p = power; p >= 0; --p) {
            if ((1 << p) > dis) continue;
            mn = min(mn, stmin[p][b]);
            b = stjump[p][b];
            dis -= (1 << p);
            if (!dis) break;
        }
    }
    if (a == b) {
        return mn;
    }
    int d = deep[a] - 1;
    for (int p = power; p >= 0; --p) {
        if ((1 << p) > d) continue;
        if (stjump[p][a] == stjump[p][b]) continue;
        mn = min(mn, stmin[p][a]);
        mn = min(mn, stmin[p][b]);
        a = stjump[p][a];
        b = stjump[p][b];
        d -= (1 << p);
    }
    mn = min(mn, stmin[0][a]);
    mn = min(mn, stmin[0][b]);
    return mn;
}

void solve() {
    cin >> n >> m;

    for (int i = 1; i <= m; ++i) {
        int x, y, z;
        cin >> x >> y >> z;
        edge1[i].u = x;
        edge1[i].v = y;
        edge1[i].limit = z;
    }
    sort(edge1 + 1, edge1 + m + 1);

    kruskal();

    power = lg2(n);
    clean();

    for (int i = 1; i <= n; ++i) {
        if (vis[i]) continue;
        dfs(i, 0, INT_MAX);
    }

    cin >> q;
    while (q--) {
        int x, y;
        cin >> x >> y;
        if (find(x) != find(y)) {
            cout << -1 << "\n";
            continue;
        }
        cout << query(x, y) << "\n";
    }
}

int main() {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    int T = 1; //cin >> T;
    while (T--) {
        solve();
    }
    return 0;
}

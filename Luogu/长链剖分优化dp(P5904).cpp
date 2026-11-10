#include<bits/stdc++.h>
using namespace std;

// #pragma GCC optimize("Ofast")
// #pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")

#define rep(i, a, b) for(int i=(a);i<=static_cast<int>(b);++i)
#define dep(i, a, b) for(int i=(a);i>=static_cast<int>(b);--i)
#define Rep(i, a, b) for(int i=(a);i<static_cast<int>(b);++i)
#define mem(x, v, sz) memset(x,v,((sz)+1)*sizeof((x)[0]))
#define debug(x) cout << #x" = " << (x) << "\n"
#define pp(x, y) cout << "pp: " << (x) << " " << (y) << "\n"

#define int long long

#define fi first
#define se second
#define pir pair<int, int>
#define i64 long long
#define u64 unsigned long long
#define u32 unsigned
#define i128 __int128
#define u128 unsigned __int128
#define f64 double
#define ld long double

const int N = 1e5 + 5;
const int M = 2e5 + 5;
const int mod = 998244353;
const int INF = 1e9 + 7;
const int NINF = -1e9 - 7;
const f64 EPS = 1e-9;

int n;
int ans;

struct trEdge {
    int to, nxt;
}edge[N * 2];
int head[N], cnt;

void addEdge(int u, int v) {
    edge[++cnt].to = v;
    edge[cnt].nxt = head[u];
    head[u] = cnt;
}

struct tree {
    int fa, son;
    int len;
    int fid, gid;
}tr[N];
int f[N], g[N * 2], cntt;

void setf(int u, int i, int v) {
    f[tr[u].fid + i] = v;
}

int getf(int u, int i) {
    return f[tr[u].fid + i];
}

void setg(int u, int i, int v) {
    g[tr[u].gid + i] = v;
}

int getg(int u, int i) {
    return g[tr[u].gid + i];
}

void dfs1(int u, int fa) {
    tr[u].fa = fa;
    tr[u].len = 1;
    tr[u].son = 0;
    int tmp = 0;
    for (int i = head[u]; i; i = edge[i].nxt) {
        int v = edge[i].to;
        if (v == fa) continue;
        dfs1(v, u);
        if (tr[v].len > tmp) {
            tmp = tr[v].len;
            tr[u].son = v;
        }
    }
    tr[u].len += tmp;
}

void dfs2(int u, int t) {
    tr[u].fid = ++cntt;
    if (!tr[u].son) {
        tr[u].gid = 2 * tr[t].fid;
        return;
    }
    dfs2(tr[u].son, t);
    for (int i = head[u]; i; i = edge[i].nxt) {
        int v = edge[i].to;
        if (v == tr[u].fa || v == tr[u].son) continue;
        dfs2(v, v);
    }
    tr[u].gid = tr[tr[u].son].gid + 1;
}

void dfs3(int u) {
    setf(u, 0, 1);
    if (!tr[u].son) return;
    dfs3(tr[u].son);
    for (int i = head[u]; i; i = edge[i].nxt) {
        int v = edge[i].to;
        if (v == tr[u].fa || v == tr[u].son) continue;
        dfs3(v);
    }
    for (int i = head[u]; i; i = edge[i].nxt) {
        int v = edge[i].to;
        if (v == tr[u].fa || v == tr[u].son) continue;
        for (int i = 0; i <= tr[v].len; ++i) {
            if (i < tr[u].len && i - 1 >= 0) {
                ans += getg(u, i) * getf(v, i - 1);
            }
            if (i > 0 && i + 1 < tr[v].len) {
                ans += getf(u, i) * getg(v, i + 1);
            }
        }
        for (int i = 0; i <= tr[v].len; ++i) {
            if (i + 1 < tr[v].len) {
                setg(u, i, getg(u, i) + getg(v, i + 1));
            }
            if (i - 1 >= 0) {
                setg(u, i, getg(u, i) + getf(u, i) * getf(v, i - 1));
            }
        }
        for (int i = 0; i <= tr[v].len; ++i) {
            if (i - 1 >= 0) {
                setf(u, i, getf(u, i) + getf(v, i - 1));
            }
        }
    }
    ans += getg(u, 0);
}


void init() {
    rep(i, 0, n) {
        head[i] = 0;
    }
    cnt = 0;
    cntt = 0;
    ans = 0;
}


void solve() {
    cin >> n;
    init();
    rep(i, 2, n) {
        int a, b;
        cin >> a >> b;
        addEdge(a, b);
        addEdge(b, a);
    }
    dfs1(1, 0);
    dfs2(1, 1);
    dfs3(1);
    cout << ans << "\n";
}

signed main() {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    int T = 1; //cin >> T;
    while (T--) {
        solve();
    }
    return 0;
}
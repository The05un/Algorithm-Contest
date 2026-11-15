#include<bits/stdc++.h>
using namespace std;

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

const int N = 2e5 + 5;
const int M = 2000 + 5;
const int mod = 998244353;
const int INF = 1e9 + 7;
const int NINF = -1e9 - 7;
const f64 EPS = 1e-9;

int n, m, liml, limr;
int col[N];
int ans;

struct tedge {
    int to, nxt;
    int c;
}edge[N << 1];
int head[N], cnt;

void addEdge(int u, int v, int c) {
    edge[++cnt].to = v;
    edge[cnt].c = c;
    edge[cnt].nxt = head[u];
    head[u] = cnt;
}

int siz[N];
int vis[N];

void getsiz(int u, int fa) {
    siz[u] = 1;
    for (int i = head[u]; i; i = edge[i].nxt) {
        int v = edge[i].to;
        if (v == fa || vis[v]) continue;
        getsiz(v, u);
        siz[u] += siz[v];
    }
}

int getcen(int u, int fa) {
    getsiz(u, fa);
    int hf = siz[u] >> 1;
    int find = 0;
    while (!find) {
        find = 1;
        for (int i = head[u]; i; i = edge[i].nxt) {
            int v = edge[i].to;
            if (v == fa || vis[v]) continue;
            if (siz[v] > hf) {
                fa = u;
                u = v;
                find = 0;
                break;
            }
        }
    }
    return u;
}

int pretr[N << 2];
int curtr[N << 2];

int ls(int p) { return p << 1; }
int rs(int p) { return p << 1 | 1; }

void push_up(int *tr, int p) {
    tr[p] = max(tr[ls(p)], tr[rs(p)]);
}

void build(int *tr, int p, int pl, int pr) {
    if (pl == pr) {
        tr[p] = -1e18;
        return;
    }
    int mid = (pl + pr) >> 1;
    build(tr, ls(p), pl, mid);
    build(tr, rs(p), mid + 1, pr);
    push_up(tr, p);
}

void clear(int *tr, int p, int pl, int pr) {
    if (tr[p] == -1e18) {
        return;
    }
    tr[p] = -1e18;
    if (pl == pr) {
        return;
    }
    int mid = (pl + pr) >> 1;
    clear(tr, ls(p), pl, mid);
    clear(tr, rs(p), mid + 1, pr);
    push_up(tr, p);
}

void update(int *tr, int p, int pl, int pr, int x, int v) {
    if (pl == pr) {
        tr[p] = max(tr[p], v);
        return;
    }
    int mid = (pl + pr) >> 1;
    if (x <= mid) update(tr, ls(p), pl, mid, x, v);
    else update(tr, rs(p), mid + 1, pr, x, v);
    push_up(tr, p);
}

int query(int *tr, int p, int pl, int pr, int L, int R) {
    if (L <= pl && pr <= R) {
        return tr[p];
    }
    int mid = (pl + pr) >> 1;
    int res = -1e18;
    if (L <= mid) res = max(res, query(tr, ls(p), pl, mid, L, R));
    if (R > mid) res = max(res, query(tr, rs(p), mid + 1, pr, L, R));
    return res;
}

int edgecnt[N];
int pathsum[N];
int subtree[N], cnts;

void dfs(int u, int fa, int prec, int egn, int sum) {
    if (egn > limr) {
        return;
    }
    edgecnt[u] = egn;
    pathsum[u] = sum;
    subtree[++cnts] = u;
    for (int i = head[u]; i; i = edge[i].nxt) {
        int v = edge[i].to;
        int c = edge[i].c;
        if (v == fa || vis[v]) continue;
        dfs(v, u, c, egn + 1, sum + (prec == c ? 0 : col[c]));
    }
}

struct cal {
    int to;
    int c;
    bool operator< (const cal &other) {
        return c < other.c;
    }
}cedge[N];
int cnte;

int cntc;
int colornode[N];

void calc(int u) {
    cnte = 0;
    for (int i = head[u]; i; i = edge[i].nxt) {
        int v = edge[i].to;
        int c = edge[i].c;
        if (vis[v]) continue;
        cedge[++cnte].c = c;
        cedge[cnte].to = v;
    }
    sort(cedge + 1, cedge + cnte + 1);
    update(pretr, 1, 0, n, 0, 0);
    cntc = 0;
    for (int k = 1; k <= cnte; ++k) {
        int v = cedge[k].to;
        int c = cedge[k].c;
        if (k > 1 && cedge[k - 1].c != c) {
            clear(curtr, 1, 0, n);
            for (int i = 1; i <= cntc; ++i) {
                int node = colornode[i];
                update(pretr, 1, 0, n, edgecnt[node], pathsum[node]);
            }
            cntc = 0;
        }
        cnts = 0;
        dfs(v, u, c, 1, col[c]);
        for (int i = 1; i <= cnts; ++i) {
            int node = subtree[i];
            int l = max(liml - edgecnt[node], 0LL);
            int r = limr - edgecnt[node];
            ans = max(ans, query(pretr, 1, 0, n, l, r) + pathsum[node]);
            ans = max(ans, query(curtr, 1, 0, n, l, r) + pathsum[node] - col[c]);
        }
        for (int i = 1; i <= cnts; ++i) {
            int node = subtree[i];
            colornode[++cntc] = node;
            update(curtr, 1, 0, n, edgecnt[node], pathsum[node]);
        }
    }
    clear(pretr, 1, 0, n);
    clear(curtr, 1, 0, n);
}

void compute(int u) {
    vis[u] = 1;
    calc(u);
    for (int i = head[u]; i; i = edge[i].nxt) {
        int v = edge[i].to;
        if (vis[v]) continue;
        compute(getcen(v, u));
    }
}

void init() {
    for (int i = 1; i <= n; ++i) {
        head[i] = 0;
        vis[i] = 0;
    }
    cnt = 0;
    ans = -1e18;
}

void solve() {
    cin >> n >> m >> liml >> limr;
    init();
    for (int i = 1; i <= m; ++i) {
        cin >> col[i];
    }
    for (int i = 1; i < n; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        addEdge(a, b, c);
        addEdge(b, a, c);
    }
    build(pretr, 1, 0, n);
    build(curtr, 1, 0, n);
    int cen = getcen(1, 0);
    compute(cen);
    cout << ans << "\n";
}

signed main() {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    int T = 1; 
    // cin >> T;
    while (T--) {
        solve();
    }
    return 0;
}
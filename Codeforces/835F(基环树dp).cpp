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
const int mod = 1e9 + 7;
const int INF = 1e9 + 7;
const int NINF = -1e9 - 7;
const f64 EPS = 1e-9;

int n;

struct tedge {
    int to, nxt;
    int w;
}edge[N << 1];
int head[N], cnt;

void addEdge(int u, int v, int w) {
    edge[++cnt].to = v;
    edge[cnt].w = w;
    edge[cnt].nxt = head[u];
    head[u] = cnt;
}

int dfn[N], cntd;
int fromnode[N];
int fromweight[N];
int crcl[N];
int arr[N], val[N], cnta;

void dfs(int u) {
    dfn[u] = ++cntd;
    for (int i = head[u]; i; i = edge[i].nxt) {
        int v = edge[i].to;
        int w = edge[i].w;
        if (!dfn[v]) {
            fromnode[v] = u;
            fromweight[v] = w;
            dfs(v);
        } else if (dfn[u] < dfn[v]) {
            crcl[u] = 1;
            arr[++cnta] = u;
            val[cnta] = w;
            for (int i = v; i != u; i = fromnode[i]) {
                crcl[i] = 1;
                arr[++cnta] = i;
                val[cnta] = fromweight[i];
            }
        }
    }
}

int d;
int dist[N];

void treedp(int u, int fa) {
    for (int i = head[u]; i; i = edge[i].nxt) {
        int v = edge[i].to;
        int w = edge[i].w;
        if (v == fa || crcl[v]) continue;
        treedp(v, u);
        d = max(d, dist[u] + dist[v] + w);
        dist[u] = max(dist[u], dist[v] + w);
    }
}

int h[N];
int pm[N], sm[N];
int pd[N], sd[N];

int crcldp() {
    int best = 0;
    int sum = 0;
    for (int i = 1; i <= cnta; ++i) {
        pm[i] = max(pm[i - 1], h[i] + sum);
        pd[i] = max(pd[i - 1], h[i] + best + sum);
        best = max(best, h[i] - sum);
        sum += val[i];
    }
    sm[cnta] = h[cnta] + val[cnta];
    sd[cnta] = h[cnta];
    best = h[cnta] - val[cnta];
    sum = val[cnta] + val[cnta - 1];
    for (int i = cnta - 1; i; --i) {
        sm[i] = max(sm[i + 1], h[i] + sum);
        sd[i] = max(sd[i + 1], h[i] + best + sum);
        best = max(best, h[i] - sum);
        sum += val[i - 1];
    }
    int ans = LLONG_MAX;
    for (int i = 1; i < cnta; ++i) {
        ans = min(ans, (max(pm[i] + sm[i + 1], max(pd[i], sd[i + 1]))));
    }
    ans = min(ans, pd[cnta]);
    return ans;
}

void compute() {
    dfs(1);
    int ans1 = 0;
    for (int i = 1; i <= cnta; ++i) {
        d = 0;
        treedp(arr[i], 0);
        ans1 = max(ans1, d);
        h[i] = dist[arr[i]];
    }
    int ans2 = crcldp();
    int ans = max(ans1, ans2);
    printf("%.1f\n", (double) ans / 2.0);
}


void init() {
    for (int i = 0; i <= n; ++i) {
        head[i] = 0;
        dfn[i] = 0;
        crcl[i] = 0;
        fromnode[i] = fromweight[i] = 0;
        dist[i] = h[i] = 0;
    }
    cnt = cntd = cnta = 0;
}


void solve() {
    cin >> n;
    init();
    for (int i = 1; i <= n; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        addEdge(a, b, c);
        addEdge(b, a, c);
    }
    compute();
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
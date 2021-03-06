#include <stdio.h>
#include <stdlib.h>

int Mod;
int **p;
int *res;
int **ksm;
int *prime;
int tot;
int *v;
int **q;
int ***g;
int ***Sum;
int *m;
int *b;
int **Comb;

int C;
int M;
int N;
int *fn;
int *fc;
int **fm;

void init() {
  tot = 0;
  C = 0;
  M = 0;
  N = 0;
  int i;
  int j;
  int k;
  g = malloc(sizeof(int **) * 6);
  for (i = 0; i < 6; i++) {
    g[i] = malloc(sizeof(int *) * 100001);
    for (j = 0; j < 100001; j++) {
      g[i][j] = malloc(sizeof(int) * 15);
      for (k = 0; k < 15; k++)
        g[i][j][k] = 0;
    }
  }
  Sum = malloc(sizeof(int **) * 6);
  for (i = 0; i < 6; i++) {
    Sum[i] = malloc(sizeof(int *) * 100001);
    for (j = 0; j < 100001; j++) {
      Sum[i][j] = malloc(sizeof(int) * 15);
      for (k = 0; k < 15; k++)
        Sum[i][j][k] = 0;
    }
  }
  fm = malloc(sizeof(int *) * 1001);
  for (i = 0; i < 1001; i++) {
    fm[i] = malloc(sizeof(int) * 13);
    for (j = 0; j < 13; j++)
      fm[i][j] = 0;
  }
  ksm = malloc(sizeof(int *) * 100001);
  for (i = 0; i < 100001; i++) {
    ksm[i] = malloc(sizeof(int) * 21);
    for (j = 0; j < 21; j++)
      ksm[i][j] = 0;
  }
  p = malloc(sizeof(int *) * 21);
  for (i = 0; i < 21; i++) {
    p[i] = malloc(sizeof(int) * 21);
    for (j = 0; j < 21; j++)
      p[i][j] = 0;
  }
  q = malloc(sizeof(int *) * 21);
  for (i = 0; i < 21; i++) {
    q[i] = malloc(sizeof(int) * 100001);
    for (j = 0; j < 100001; j++)
      q[i][j] = 0;
  }
  Comb = malloc(sizeof(int *) * 100001);
  for (i = 0; i < 100001; i++) {
    Comb[i] = malloc(sizeof(int) * 21);
    for (j = 0; j < 21; j++)
      Comb[i][j] = 0;
  }
  fn = malloc(sizeof(int) * 1001);
  for (i = 0; i < 1001; i++)
    fn[i] = 0;
  fc = malloc(sizeof(int) * 1001);
  for (i = 0; i < 1001; i++)
    fc[i] = 0;
  m = malloc(sizeof(int) * 1001);
  for (i = 0; i < 1001; i++)
    m[i] = 0;
  res = malloc(sizeof(int) * 1001);
  for (i = 0; i < 1001; i++)
    res[i] = 0;
  b = malloc(sizeof(int) * 1001);
  for (i = 0; i < 1001; i++)
    b[i] = 0;
  v = malloc(sizeof(int) * 100001);
  for (i = 0; i < 100001; i++)
    v[i] = 0;
  prime = malloc(sizeof(int) * 100001);
  for (i = 0; i < 100001; i++)
    prime[i] = 0;
}
int Ksm(int P, int x) {
  if (x == 0)
    return 1;
  if (x == 1)
    return P % Mod;
  int tmp;
  tmp = Ksm(P, x >> 1);
  if (x % 2 == 1)
    return tmp * tmp % Mod * P % Mod;
  else
    return tmp * tmp % Mod;
}

void Calculate_p() {
  int i;
  int j;
  p[0][0] = 1;
  p[1][1] = 1;
  p[1][0] = Mod - 1;
  for (i = 2; i <= C - 2; i++) {
    int tmp;
    tmp = Ksm(i, Mod - 2);
    for (j = 0; j < i; j++)
      p[i][j + 1] = p[i - 1][j];
    for (j = 0; j <= i; j++)
      p[i][j] = (p[i][j] - p[i - 1][j] * i % Mod + Mod) * tmp % Mod;
  }
}
void Euler(int x) {

  tot = 0;
  q[x][1] = 1;
  int i;
  int j;
  for (i = 0; i < 100001; i++)
    v[i] = 0;
  for (i = 2; i <= M; i++) {
    if (v[i] == 0) {
      prime[++tot] = i;
      q[x][i] = (ksm[i][x] + Mod - 1) % Mod;
    }
    for (j = 1; j <= tot && prime[j] * i <= M; j++) {
      v[prime[j] * i] = 1;
      if (i % prime[j] == 0) {
        q[x][i * prime[j]] = q[x][i] * ksm[prime[j]][x] % Mod;
        break;
      } else
        q[x][i * prime[j]] = q[x][i] * q[x][prime[j]] % Mod;
    }
  }
}
void Calculate_q() {
  int i;
  for (i = 0; i <= C - 2; i++)
    Euler(i);
}
void Calculate_Ksm() {
  int i;
  int j;
  for (i = 1; i <= M; i++) {
    ksm[i][0] = 1;
    for (j = 1; j <= C - 2; j++)
      ksm[i][j] = ksm[i][j - 1] * i % Mod;
  }
}
void Calculate_G() {
  Calculate_Ksm();
  Calculate_p();
  Calculate_q();
  int i;
  int j;
  int k;
  for (i = 1; i <= M; i++)
    for (j = 2; j <= C; j++) {
      for (k = 0; k <= j - 2; k++)
        g[0][i][j] = (g[0][i][j] + p[j - 2][k] * q[k][i]) % Mod;
      for (k = 1; k <= N; k++)
        g[k][i][j] = g[k - 1][i][j] * i % Mod;
    }
  for (k = 0; k <= N; k++) {
    for (i = 2; i <= C; i++)
      for (j = 1; j <= M; j++) {
        Sum[k][j][i] = Sum[k][j - 1][i] + g[k][j][i];
        if (Sum[k][j][i] >= Mod)
          Sum[k][j][i] = Sum[k][j][i] - Mod;
      }
  }
}
void Calculate_Comb() {
  int i;
  int j;
  for (i = 0; i <= M; i++)
    Comb[i][0] = 1;
  for (i = 1; i <= M; i++)
    for (j = 1; j <= C; j++) {
      Comb[i][j] = Comb[i - 1][j] + Comb[i - 1][j - 1];
      if (Comb[i][j] >= Mod)
        Comb[i][j] = Comb[i][j] - Mod;
    }
}
int main() {
  int T;
  int n;
  int c;

  scanf("%d", &T);
  init();
  fn[1] = 2;
  fc[1] = 3;
  fm[1][1] = 3;
  fm[1][2] = 4;
  fn[2] = 3;
  fc[2] = 3;
  fm[2][1] = 3;
  fm[2][2] = 4;
  fm[2][3] = 4;
  fn[3] = 4;
  fc[3] = 4;
  fm[3][1] = 5;
  fm[3][2] = 7;
  fm[3][3] = 8;
  fm[3][4] = 9;
  C = 4;
  M = 9;
  N = 4;
  Mod = 10007;
  Calculate_G();
  Calculate_Comb();
  int rp;
  for (rp = 1; rp <= T; rp++) {
    n = fn[rp];
    c = fc[rp];
    int ii;
    for (ii = 1; ii <= n; ii++)
      m[ii] = fm[rp][ii];
    if (n == 1)
      printf("%d\n", Comb[m[1]][c]);
    else {
      int ans;
      int l;
      int r;
      int temp;
      ans = 0;
      temp = 0;
      l = 0;
      r = 1;

      for (; r <= m[1]; r++) {
        int tmp;
        tmp = (m[1] / (m[1] / r));
        int i;
        for (i = 2; i <= n; i++) {
          int get;
          get = m[i] / (m[i] / r);
          if (get < tmp)
            tmp = get;
        }
        if (m[1] < tmp)
          tmp = m[1];
        r = tmp;
        b[0] = 1;
        for (i = 1; i <= n; i++) {
          int tmp1;
          int tmp2;
          int tmp3;
          tmp1 = (m[i] / r) % Mod;
          tmp2 = tmp1 * (tmp1 + 1) / 2 % Mod;
          tmp3 = m[i] * tmp1 % Mod;
          int j;
          for (j = 0; j < i; j++)
            res[j + 1] = b[j] * (Mod - tmp2) % Mod;
          res[0] = 0;
          for (j = 0; j < i; j++)
            res[j] = (res[j] + tmp3 * b[j]) % Mod;
          for (j = 0; j <= i; j++)
            b[j] = res[j];
        }
        for (i = 0; i <= n; i++)
          ans = (ans + b[i] * (Sum[i][r][c] - Sum[i][l][c])) % Mod;
        l = r;
      }
      if (ans < 0)
        ans = ans + Mod;
      printf("%d\n", ans);
    }
  }
  return 0;
}


// lunatic-515030910648-yangzhuolin.txt
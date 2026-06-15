int n = ...;
int origem = ...;
int destino = ...;
range nos = 1..n;
int c[nos][nos] = ...;

dvar boolean arco[nos][nos];
dvar int+ ordem[nos];

minimize
  sum(i in nos, j in nos: i != j) c[i][j] * arco[i][j];

subject to {
  sum(j in nos: j != origem) arco[origem][j] == 1;
  sum(i in nos: i != destino) arco[i][destino] == 1;

  forall(k in nos: k != origem && k != destino)
    sum(i in nos: i != k) arco[i][k]
      == sum(j in nos: j != k) arco[k][j];

  ordem[origem] == 0;

  forall(k in nos: k != origem)
    ordem[k] >= 1;
  forall(k in nos: k != origem && k != destino)
    ordem[k] <= n - 1;

  forall(i in nos, j in nos: i != j && j != origem)
    ordem[j] >= ordem[i] + 1 - n * (1 - arco[i][j]);

  forall(i in nos)
    arco[i][i] == 0;
}

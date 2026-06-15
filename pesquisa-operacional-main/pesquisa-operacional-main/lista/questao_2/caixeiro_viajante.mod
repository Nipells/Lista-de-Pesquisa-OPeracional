int n = ...;
range cidades = 1..n;
int d[cidades][cidades] = ...;

dvar boolean rota[cidades][cidades];
dvar int+ ordem[cidades];

minimize
  sum(i in cidades, j in cidades: i != j) d[i][j] * rota[i][j];

subject to {
  forall(i in cidades)
    sum(j in cidades: j != i) rota[i][j] == 1;

  forall(j in cidades)
    sum(i in cidades: i != j) rota[i][j] == 1;

  ordem[1] == 1;

  forall(i in cidades: i >= 2)
    ordem[i] >= 2;
  forall(i in cidades: i >= 2)
    ordem[i] <= n;

  forall(i in cidades, j in cidades: i != j && j >= 2)
    ordem[i] - ordem[j] + n * rota[i][j] <= n - 1;

  forall(i in cidades)
    rota[i][i] == 0;
}

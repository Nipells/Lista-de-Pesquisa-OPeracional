int n = ...;
int m = ...;
int capacidade = ...;

range clientes = 1..n;
range nos = 0..n;
range veiculos = 1..m;

int demanda[clientes] = ...;
int d[nos][nos] = ...;

dvar boolean rota[nos][nos][veiculos];
dvar int+ carga[nos][veiculos];

minimize
  sum(i in nos, j in nos, k in veiculos: i != j)
    d[i][j] * rota[i][j][k];

subject to {
  forall(i in clientes)
    sum(k in veiculos, j in nos: j != i) rota[i][j][k] == 1;

  forall(k in veiculos) {
    sum(j in clientes) rota[0][j][k] <= 1;
    sum(i in clientes) rota[i][0][k] <= 1;
    sum(j in clientes) rota[0][j][k] == sum(i in clientes) rota[i][0][k];
  }

  forall(k in veiculos, i in clientes)
    sum(j in nos: j != i) rota[i][j][k]
      == sum(j in nos: j != i) rota[j][i][k];

  forall(k in veiculos)
    carga[0][k] == 0;

  forall(i in clientes, k in veiculos) {
    carga[i][k] >= demanda[i];
    carga[i][k] <= capacidade;
  }

  forall(i in nos, j in clientes, k in veiculos: i != j)
    carga[j][k] >= carga[i][k] + demanda[j]
      - capacidade * (1 - rota[i][j][k]);

  forall(i in nos, j in nos, k in veiculos: i == j)
    rota[i][j][k] == 0;
}

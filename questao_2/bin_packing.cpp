#include <ilcplex/ilocplex.h> //chamando a biblio do cplex
#include <iostream>
#include <vector>

//IL_STD_ARRAY_TO_STREAM(std::cout); //macro obrigatprio pro cplex em c++

int main(){

IloEnv env; //gerenciador de mamoria do cplex

try { //pq usa o try aki?

    IloModel modelo(env); //para guardar a funcao, obj e restrições

    //dados do problema
    int itens = 4, caixas = 4;

    std::vector<double> tamanhos = {0.4, 0.6, 0.5, 0.3}; //tamanho entre 0 e 1

    double capacidade = 1.0;

    //var de decisao
    //var y[j]: bonaria, indica se a caixa esta ativa
    IloNumVarArray y(env, caixas, 0, 1, ILOINT);

    //var x[i][j]: matriz binaria,indica se o item esta na caixa j
    //aqui ele cria a matriz usando cplex

    typedef IloNumVarArray IloNumVarMatrix;

    IloArray<IloNumVarArray> x(env, itens);

    for (int i = 0; i < itens; i++){
        x[i] = IloNumVarArray(env, caixas, 0, 1, ILOINT);
    }

    //funcao objetivo
    //minimizacao de y[i]
    IloExpr objExpr(env);

    for (int j = 0; j< caixas; j++){
        objExpr += y[j];
    }
    modelo.add(IloMinimize(env, objExpr));
    objExpr.end(); //liberando expresssoes pra nao vazra memoria

    //restricao 1, cada item em 1 caixa
    for (int i = 0; i < itens; i++){
        IloExpr restricao1(env);
        for (int j = 0; j < caixas; j++){
            restricao1 += x[i][j];
        }
        modelo.add(restricao1 == 1);
        restricao1.end();
    }

    //restricao 2, capacidade da caixa
    for (int j = 0; j < caixas; j++){
        IloExpr restricao2(env);
        for (int i = 0; i <itens; i++){
            restricao2 += tamanhos[i] * x[i][j];
        }

        //a soma dos pesos deve ser =< a capacidade *y[j]
        modelo.add(restricao2 <= capacidade * y[j]);
        restricao2.end();
    }

    //resolvendo o modelo

    IloCplex cplex(modelo);

    if(cplex. solve()){
        std::cout << "\n=== Solução encontrada ===" << std::endl;
        std::cout << "Status: " <<cplex.getStatus() << std::endl;
        std::cout << "Número mínimo de caixas: " << cplex.getObjValue() << std::endl;
        

        //veridicando onde cada item ficou
        for (int i = 0; i < itens; i++){
            for (int j = 0; j < caixas; j++){
                if (cplex.getValue(x[i][j]) > 0.5) { //se for 1, com toleancia numerica
                    std::cout << "Item " << i << " (tam: " << tamanhos[i] << ") -> Caixa " << j << std::endl;
                }
            }
        }
    }

    else {
        std::cout << "Não foi possivel encontrar uma solução" <<std::endl;
    }
}

catch(IloException& e){
    std::cerr << "Erro no Cplrex: " << e << std::endl;
}

catch (...){
    std::cerr << "Erro desconhecido" << std::endl;
}

//libera a memoria alocada pelo ambiente do cplex
env.end();

    return 0;
}
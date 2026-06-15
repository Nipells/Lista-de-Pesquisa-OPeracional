#include <ilcplex/ilocplex.h>
#include <iostream>

int main() {
    IloEnv env;

    try {
        IloModel modelo(env);

        //cenário, origem e destino respectivamente: 2 fábricas e 3 clientes
        int clientes = 3;
        int fabricas = 2;

        //dados
        double capacidade[] = {500, 700}; // oferta máxima da fabrica 1 e fabrica 2
        double demanda[] = {300, 400, 500}; // necessidade do cliente 1, 2 e 3

        //matriz de custo: custo[fabrica][cliente]
        double custo[2][3] = {
            {4.0, 2.0, 5.0},  //saindo da fabrica 1 para clientes 1, 2, 3
            {3.0, 6.0, 1.0}   //saindo da fabrica 2 para clientes 1, 2, 3
        };

        //variaveis de decisao (ontínuas >= 0)
        // x[i][j] = quantidade transportada da fabrica i para o cliente j

        IloArray<IloNumVarArray> x(env, fabricas);

        for (int i = 0; i < fabricas; i++) {

            x[i] = IloNumVarArray(env, clientes, 0, IloInfinity, ILOFLOAT);

        }

        //fo: minimizar o custo total de transporte
        
        IloExpr custo_total(env);

        for (int i = 0; i < fabricas; i++) {

            for (int j = 0; j < clientes; j++) {

                custo_total += custo[i][j] * x[i][j];

            }
        }

        modelo.add(IloMinimize(env, custo_total));

        custo_total.end();

        //restrições

        //restrições de oferta (Não estourar a capacidade das fábricas)

        for (int i = 0; i < fabricas; i++) {

            IloExpr total_saindo(env);

            for (int j = 0; j < clientes; j++) {

                total_saindo += x[i][j];
            }

            modelo.add(total_saindo <= capacidade[i]);

            total_saindo.end();
        }

        //restrições (garantir que os clientes recebam o que precisam)
        for (int j = 0; j < clientes; j++) {

            IloExpr total_chegando(env);

            for (int i = 0; i < fabricas; i++) {

                total_chegando += x[i][j];

            }

            modelo.add(total_chegando >= demanda[j]);
            total_chegando.end();
        }

        //solver
        IloCplex cplex(modelo);

        if (cplex.solve()) {

            std::cout << "\n=== Plano de Transporte Otimo Encontrado ===" << std::endl;
            std::cout << "Status: " << cplex.getStatus() << std::endl;
            std::cout << "Custo Logistico Minimo: R$ " << cplex.getObjValue() << std::endl;
            
            for (int i = 0; i < fabricas; i++) {

                std::cout << "\n[ Fabrica " << i + 1 << " ]" << std::endl;

                for (int j = 0; j < clientes; j++) {

                    double qtd = cplex.getValue(x[i][j]);

                    if (qtd > 0.01) {

                        std::cout << " -> Enviar " << qtd << " unidades para o Cliente " << j + 1 
                                  << " (Custo unitario: R$ " << custo[i][j] << ")" << std::endl;
                    }
                }
            }
        } 
        
        else {
            std::cout << "Nao foi possivel encontrar uma solucao viavel." << std::endl;
        }

    } 
    
    catch (IloException& e) {

        std::cerr << "Erro no CPLEX: " << e << std::endl;

    } 
    
    catch (...) {

        std::cerr << "Erro desconhecido." << std::endl;

    }

    env.end();

    return 0;
}
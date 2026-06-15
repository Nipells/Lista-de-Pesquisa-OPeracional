#include <ilcplex/ilocplex.h>
#include <iostream>

int main(){

    IloEnv env;

    try{

        IloModel modelo(env);

        /*
        variaveis de decisao (0 || 1)
        um indicador para cada um dos 5 vertices do grafo
        */

        //cri pos de 0 a 5b para facilitar uso dos ids originais
        IloNumVarArray x(env, 6, 0, 1, ILOINT);

        //fo

        IloExpr fo(env);

        for(int v = 1; v <= 5; v++){
            fo = fo + x[v];
        }

        modelo.add(IloMaximize(env, fo));
        fo.end();

        //restricoes
        //vertices sem conexao direta nao poem ficar juntos

        //seguindo as notes do pdf

        modelo.add(x[1] + x[4] <= 1);
        modelo.add(x[1] + x[5] <= 1);
        modelo.add(x[2] + x[5] <= 1);
        modelo.add(x[3] + x[4] <= 1);

        //solver

        IloCplex cplex(modelo);

        if (cplex.solve()){

            std::cout << "\n === Solucao otima ===" << std::endl;
            std::cout << "status: " << cplex.getStatus() << std::endl;
            std::cout << "tamanho da maior dlique: " << cplex.getObjValue() << std::endl;

            std::cout << "vertices selecionados: ";

            for (int v = 1; v <= 5; v++){

                std::cout << "v" << v << " ";

            }

            std::cout << std::endl;

        }

        else {

            std::cout << "não foi possivel enontrar solução" << std::endl;

        }

        }

        catch (IloException& e){

            std::cerr << "Erro no cplex: " << e << std::endl;
        }

        catch (...){

            std::cerr << "Erro desconhecido" << std::endl;
        }

    env.end();
    
    return 0;

    }
//codigo baseado na resolucao da aula remota do prof rian

#include <ilcplex/ilocplex.h>
#include <iostream>

int main(){

    IloEnv env;

    try {
        IloModel modelo(env);

        /*
        variaveis de decisão => e >= 0
        */

        //tinta semibrilho  natural - sn
        IloNumVar x_SolA_SN(env, 0, IloInfinity, ILOFLOAT);
        IloNumVar x_SolB_SN(env, 0, IloInfinity, ILOFLOAT);
        IloNumVar x_Sec_SN(env, 0, IloInfinity, ILOFLOAT);
        IloNumVar x_Cor_SN(env, 0, IloInfinity, ILOFLOAT);

        //tinta super resistente - SR
        IloNumVar x_SolA_SR(env, 0, IloInfinity, ILOFLOAT);
        IloNumVar x_SolB_SR(env, 0, IloInfinity, ILOFLOAT);
        IloNumVar x_Sec_SR(env, 0, IloInfinity, ILOFLOAT);
        IloNumVar x_Cor_SR(env, 0, IloInfinity, ILOFLOAT);

        /*
        função objetivo = fo:
        Preços por litro: SolA = 5.5, SolB = 1.0, Sec = 4.0, Cor = 6.0
        */

        IloExpr fo(env);

        fo = fo + (5.5 * (x_SolA_SN + x_SolA_SR));
        fo = fo + (1.5 * (x_SolA_SN + x_SolB_SR));
        fo = fo + (4.0 * (x_SolA_SN + x_Sec_SR));
        fo = fo + (6.0 * (x_SolA_SN + x_Cor_SR));

        modelo.add(IloMinimize(env, fo)); //ordenacao para o valor ser o menor possivel
        fo.end();

        //rstricoes

        //secante
        modelo.add(x_Sec_SR + 0.3 * x_SolA_SR + 0.6 * x_SolB_SR >= 250); //sec - sr
        modelo.add(x_Sec_SN + 0.3 * x_SolA_SN + 0.6 * x_SolB_SN >= 50); //sec - sn

        //cor
        modelo.add(x_Cor_SR + 0.1 * x_Sec_SR + 0.4 * x_SolB_SR >= 500); //cor - sr
        modelo.add(x_Cor_SN + 0.1 * x_Sec_SN + 0.4 * x_SolB_SN >= 525); //cor - sn

        //solver
        IloCplex cplex(modelo);

        if (cplex.solve()){

            std::cout << "\n === Solução ótima encontrada ===" << std::endl;
            std::cout << "status do solver: " <<cplex.getStatus() << std::endl;
            std::cout << "Custo total minimo: R$ " << cplex.getObjValue() << std::endl;

            std::cout << "\n[ Tinta semibrilho natural - SN ]" << std::endl;
            std::cout << "-> Solvente A: " << cplex.getValue(x_SolA_SN) << " L" << std::endl;
            std::cout << "-> Solvente B: " << cplex.getValue(x_SolB_SN) << " L" << std::endl;
            std::cout << " -> Secante:    " << cplex.getValue(x_Sec_SN) << " L" << std::endl;
            std::cout << " -> Corante:    " << cplex.getValue(x_Cor_SN) << " L" << std::endl;
        
            std::cout << "\n[ Tinta super resistente - SR ]" << std::endl;
            std::cout << "-> Solvente A: " << cplex.getValue(x_SolA_SR) << " L" << std::endl;
            std::cout << "-> Solvente B: " << cplex.getValue(x_SolB_SR) << " L" << std::endl;
            std::cout << " -> Secante:    " << cplex.getValue(x_Sec_SR) << " L" << std::endl;
            std::cout << " -> Corante:    " << cplex.getValue(x_Cor_SR) << " L" << std::endl;
        
        }

        else{

            std::cout << "Não foi possivel encontrar uma solução viavel" <<std::endl;

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
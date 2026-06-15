#include <ilcplex/ilocplex.h>
#include <iostream>

int main(){

    IloEnv env;

    try{

        IloModel modelo(env);

        //nomes
        //fazendas (0 = f1, 1 = f2, 2 = f3)
        //culturas (0 = milho; 1 = soja; 2 = arroz)

        //bariaveis de decisao
        //x[f][c] = Quantidade de hectares da cultura 'c' na fazenda 'f'

        IloArray<IloNumVarArray> x(env, 3);

        for(int f = 0; f < 3; f++){

            x[f] = IloNumVarArray(env, 3, 0, IloInfinity, ILOFLOAT);
        }

        //fo
        //maximizar os liucros totais = milho = 500, soja = 700, arroz = 400

        IloExpr lucro_total(env);

        for(int f = 0; f < 3; f++){

            lucro_total += 500 * x[f][0]; //milho
            lucro_total += 700 * x[f][1]; //soja
            lucro_total += 400 * x[f][2]; //arroz
        }

        modelo.add(IloMaximize(env, lucro_total));

        lucro_total.end();

        //restrições

        //hectares disponiveis
        modelo.add(x[0][0] + x[0][1] + x[0][2] <= 400); // Fazenda 1
        modelo.add(x[1][0] + x[1][1] + x[1][2] <= 600); // Fazenda 2
        modelo.add(x[2][0] + x[2][1] + x[2][2] <= 450); // Fazenda 3

        //consumo de agua
        IloExpr agua_usada(env);

        for (int f = 0; f < 3; f++) {
            agua_usada += 3 * x[f][0]; // Milho gasta 3L
            agua_usada += 4 * x[f][1]; // Soja gasta 4L
            agua_usada += 5 * x[f][2]; // Arroz gasta 5L
        }

        modelo.add(agua_usada <= 5000);
        agua_usada.end();

        //maximo de vendas por cultutra
        modelo.add(x[0][0] + x[1][0] + x[2][0] <= 600); // max. milho
        modelo.add(x[0][1] + x[1][1] + x[2][1] <= 500); // max. soja
        modelo.add(x[0][2] + x[1][2] + x[2][2] <= 325); // max. arroz

        //solver

        IloCplex cplex(modelo);

        if (cplex.solve()) {

            std::cout << "\n=== Planejamento de Plantio Otimo Encontrado ===" << std::endl;
            std::cout << "Status: " << cplex.getStatus() << std::endl;
            std::cout << "Lucro Maximo Possivel: R$ " << cplex.getObjValue() << std::endl;
            
            const char* nomes_culturas[] = {"Milho", "Soja", "Arroz"};
            
            for (int f = 0; f < 3; f++) {

                std::cout << "\n[ Fazenda " << f + 1 << " ]" << std::endl;

                for (int c = 0; c < 3; c++) {

                    double valor = cplex.getValue(x[f][c]);

                    if (valor > 0.01) { 
                        
                        // Só mostra o que realmente foi plantado
                        std::cout << " -> " << nomes_culturas[c] << ": " << valor << " hectares" << std::endl;
                    }
                }
            }
        }

        else {

            std::cout << "Nao foi possivel encontrar uma solucao viavel." << std::endl;
        }

    } 
    //fechamento do try
    catch (IloException& e) {

        std::cerr << "Erro no cplex: " << e << std::endl;
    }

    catch (...) {

        std::cerr << "Erro desconhecido" << std::endl;
    }

    env.end();
    
    return 0;
}
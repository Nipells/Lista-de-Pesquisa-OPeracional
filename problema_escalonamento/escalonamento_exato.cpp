#include <ilcplex/ilocplex.h>
#include <iostream>
#include <vector>

int main() {

    IloEnv env;

    try {

        IloModel modelo(env);

        // defini os tempos de processamento das 4 tarefas

        int tarefas = 4;
        double tempos[] = {15.0, 5.0, 8.0, 12.0};
        
        // m é um número muito grande usado para ativar/desativar as restricoes
        double M = 1000.0; 

        //variaveis
        
        // t[i] = tempo de termino da tarefa i (continua >= 0)
        IloNumVarArray t(env, tarefas, 0, IloInfinity, ILOFLOAT);

        // y[i][j] = 1 se a tarefa i for executada antes da j, e 0 caso contrario

        IloArray<IloNumVarArray> y(env, tarefas);

        for (int i = 0; i < tarefas; i++) {

            y[i] = IloNumVarArray(env, tarefas, 0, 1, ILOINT);

        }

        //fo - minimizar a soma dos tempos de termino de todas as tarefas

        IloExpr soma_terminos(env);

        for (int i = 0; i < tarefas; i++) {

            soma_terminos += t[i];

        }

        modelo.add(IloMinimize(env, soma_terminos));
        soma_terminos.end();

        //restrições
        //nenhuma tarefa deve terminar antes do seu proprio tempo de execucao

        for (int i = 0; i < tarefas; i++) {

            modelo.add(t[i] >= tempos[i]);

        }

        // crias as restricoes de nao-sobreposicao usando a lógica do big-m

        for (int i = 0; i < tarefas; i++) {

            for (int j = 0; j < tarefas; j++) {

                if (i != j) {

                    // se i roda antes de j (y[i][j]=1), entao t[j] >= t[i] + tempos[j]
                    modelo.add(t[j] >= t[i] + tempos[j] - M * (1 - y[i][j]));
                    
                    // garanto que ou i roda antes de j, ou j roda antes de i
                    if (i < j) {
                        modelo.add(y[i][j] + y[j][i] == 1);

                    }
                }
            }
        }

        // solver
        IloCplex cplex(modelo);

        if (cplex.solve()) {

            std::cout << "\n=== Escalonamento Otimo Encontrado ===" << std::endl;
            std::cout << "Status: " << cplex.getStatus() << std::endl;
            std::cout << "Tempo Total de Fluxo: " << cplex.getObjValue() << " min" << std::endl;

            // exibo o tempo de conclusao de cada tarefa
            for (int i = 0; i < tarefas; i++) {
                
                std::cout << " -> Tarefa " << i + 1 
                          << " | Concluida no minuto: " << cplex.getValue(t[i]) << std::endl;
            }

        } 
        
        else {

            std::cout << "Nao foi possivel encontrar uma solucao viavel." << std::endl;
        
        }

    } 
    
    catch (IloException& e) {

        std::cerr << "Erro no cplex: " << e << std::endl;

    } 
    
    catch (...) {

        std::cerr << "Erro desconhecido" << std::endl;

    }

    env.end();
    
    return 0;
}
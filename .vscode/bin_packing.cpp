#include <ilcplex/ilocplex.h> //chamando a biblio do cplex
#include <iostream>
#include <vector>
#include <fstream> //biblio para manipulacao de arquivos

//IL_STD_ARRAY_TO_STREAM(std::cout); //macro obrigatprio pro cplex em c++

int main(int argc, char* argv[]){
//validacao da linha de comando: usuario passa o nome do arquivo

if(argc < 2) {
    std::cerr << "Erro: Use assim: " <<argv[0] << " <nome_do_arquivo_de_instancia>" << std::endl;
    return 1;
}

//abre o arquivo passando por argumento
std::ifstream arquivo(argv[1]);
if (!arquivo.is_open()) {
    std::cerr << "Erro ao abrir o arquivo: " << argv[1] << std::endl;
    return 1;
}

//leitura dos dados
int itens;
double capacidade;

arquivo >> itens; //le o primeiro numero do arquivo
arquivo >> capacidade; //le o segundo numero do aruivo

std::vector<double> tamanhos(itens);

for(int i=0; i < itens; i++){
    arquivo >> tamanhos[i]; //le o tamanho de cada um dos itens
}

arquivo.close();

int caixas = itens; //por caso seria uma caixa pra cada item

IloEnv env; //gerenciador de mamoria do cplex

try { 
    
    /*pq usa o try aki?

    "Tente rodar esse modelo do CPLEX. Se der qualquer problema lá dentro, capture o erro (no catch) e me mostre o que aconteceu de forma amigável no terminal"
    */

    IloModel modelo(env); //para guardar a funcao, obj e restrições

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
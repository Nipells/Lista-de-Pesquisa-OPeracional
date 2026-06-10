# Caminhos do CPLEX
CPLEXDIR = /opt/ibm/ILOG/CPLEX_Studio2211/cplex
CONCERTDIR = /opt/ibm/ILOG/CPLEX_Studio2211/concert

# Compilador e Flags
CC = g++
CFLAGS = -O3 -DIL_STD -I$(CPLEXDIR)/include -I$(CONCERTDIR)/include
LDFLAGS = -L$(CPLEXDIR)/lib/x86-64_linux/static_pic -L$(CONCERTDIR)/lib/x86-64_linux/static_pic -lilocplex -lcplex -lconcert -lpthread -ldl

# Comando padrão para compilar qualquer arquivo .cpp
%: %.cpp
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f *.out
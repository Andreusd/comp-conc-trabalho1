default:
	gcc procura_substr_seq.c -o seq.out -lpthread
	gcc procura_substr_conc.c -o conc.out -lpthread	
	gcc setup/gerador_testes.c -o setup/gerador_testes.out

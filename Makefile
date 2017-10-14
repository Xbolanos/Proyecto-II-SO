all: clean inicializador productor finalizador espia 


inicializador: Inicializador/main.c
	gcc -o inicializador Inicializador/main.c
productor: Productor/main.c
	gcc -o productor Productor/main.c

espia: Espia/main.c
	gcc -o espia Espia/main.c

finalizador: Finalizador/main.c
	gcc -o finalizador Finalizador/main.c

clean:
	rm -f *.o inicializador finalizador espia productor

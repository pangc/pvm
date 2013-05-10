Pvm:vm.o main.o
	g++ vm.o main.o -o Pvm
vm.o:vm.cpp vm.h
	g++ -c vm.cpp -o vm.o
main.o:
	g++ -c main.cpp -o main.o
clean:
	rm -rf *.o Pvm
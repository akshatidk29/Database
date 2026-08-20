database: build/main.o build/database.o build/store.o build/transaction.o
	g++ build/main.o build/database.o build/store.o build/transaction.o -o database

build/main.o: src/main.cpp
	g++ -c src/main.cpp -Iinclude -o build/main.o

build/store.o: src/store.cpp
	g++ -c src/store.cpp -Iinclude -o build/store.o

build/database.o: src/database.cpp
	g++ -c src/database.cpp -Iinclude -o build/database.o

build/transaction.o: src/transaction.cpp
	g++ -c src/transaction.cpp -Iinclude -o build/transaction.o
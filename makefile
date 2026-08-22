database: build/main.o build/database.o build/store.o build/transaction.o build/method.o build/logger.o
	g++ build/main.o build/database.o build/store.o build/transaction.o build/method.o build/logger.o -o database

build/main.o: src/main.cpp
	g++ -c src/main.cpp -Iinclude -o build/main.o

build/store.o: src/store.cpp
	g++ -c src/store.cpp -Iinclude -o build/store.o

build/database.o: src/database.cpp
	g++ -c src/database.cpp -Iinclude -o build/database.o

build/method.o: src/method.cpp
	g++ -c src/method.cpp -Iinclude -o build/method.o

build/transaction.o: src/transaction.cpp
	g++ -c src/transaction.cpp -Iinclude -o build/transaction.o

build/logger.o: src/logger.cpp
	g++ -c src/logger.cpp -Iinclude -o build/logger.o
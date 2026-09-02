database: build/main.o build/database.o build/store.o build/transaction.o build/method.o build/logger.o build/index.o
	g++ build/main.o build/database.o build/store.o build/transaction.o build/method.o build/logger.o build/index.o -o database

build/main.o: src/main.cpp
	g++ -c src/main.cpp -Iinclude -o build/main.o

build/store.o: src/storage/store.cpp
	g++ -c src/storage/store.cpp -Iinclude -o build/store.o

build/database.o: src/core/database.cpp
	g++ -c src/core/database.cpp -Iinclude -o build/database.o

build/method.o: src/utils/method.cpp
	g++ -c src/utils/method.cpp -Iinclude -o build/method.o

build/transaction.o: src/core/transaction.cpp
	g++ -c src/core/transaction.cpp -Iinclude -o build/transaction.o

build/logger.o: src/storage/logger.cpp
	g++ -c src/storage/logger.cpp -Iinclude -o build/logger.o

build/index.o: src/storage/index.cpp
	g++ -c src/storage/index.cpp -Iinclude -o build/index.o

clean:
	rm -f build/*.o database
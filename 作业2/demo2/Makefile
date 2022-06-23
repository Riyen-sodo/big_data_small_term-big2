CC = g++
cflags = -std=c++17
obj = main.o database.o tables.o 
target = main
db_folder_name = ./db_folder

$(target):$(obj)


main.o:main.cpp heads.h utils.h
	@$(CC) main.cpp -o main.o $(cflags)

database.o:heads.h utils.h
	@$(CC) database.cpp -o database.o $(cflags)

tables.o:heads.h utils.h
	@$(CC) tables.cpp -o tables.o $(cflags)

clean:
	@rm $(obj) $(target)

install:
	@$(CC) $(obj) -o $(target) $(cflags)
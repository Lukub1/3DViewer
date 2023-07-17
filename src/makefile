CC=gcc
CFLAGS=-c -Wall -Wextra -Werror -std=c11
PROGRAM_NAME=3DViewer_v1
INCLUDE_LIBS=-lcheck -lrt -lpthread -lm
INSTALL_DIRECTORY=3DViewer_v1/
COMPILATION_DIRECTORY=mybuild/
FORMATS_FOR_DEL=*.out *.gz *.o *.gch  log.txt logTest.txt 3DViewer_v1 *dSYM coverage *.a *.gcda *gcno *.info Test *.cfg .clang-format $(COMPILATION_DIRECTORY)
all: clean test gcov_report


3DViewer_v1.o: parse.c parse.h
	@$(CC) $(CFLAGS) parse.c parse.h -lm
	
3DViewer_v1.a: 3DViewer_v1.o
	@ar -rcs 3DViewer_v1.a 3DViewer_v1.o
	@ranlib $@
	@cp $@ lib$@
	
clean:
	@rm -rf $(FORMATS_FOR_DEL)

OS = $(shell uname)
ifeq ($(OS), Linux)
	LINUX_LIBS=-lsubunit -lrt -lpthread -lm
	CC += -D OS_LINUX -g
	CHECK_LEAKS=valgrind --leak-check=full -s --log-file=log.txt
	OPEN_GCOV=firefox coverage/src/index.html
	OUT_LEAKS= grep log.txt
else
	LINUX_LIBS=
	CC += -D OS_MAC
	CHECK_LEAKS=CK_FORK=no leaks --atExit --
	OPEN_GCOV=open coverage/index.html
endif

ERRORS=grep -v "0 errors" log.txt | grep -c errors | awk '{print $1}'

test:	clean style 3DViewer_v1.o
	@cd test_file/ && sh rename.sh
	@$(CC) -g parse.c test.c -lcheck $(LINUX_LIBS) -o ./Test 
	@$(CHECK_LEAKS) ./Test
	@echo -n "leaks = "
	@ $(ERRORS)
	@rm -rf test_file/*.obj

style: 
	@cp ../materials/linters/.clang-format .clang-format
	@clang-format -n *.c *.h S21_3DViewer_v1/*.cpp S21_3DViewer_v1/*.h
	@rm -rf .clang-format

gcov_report: parse.c test.c parse.h
	@cd test_file/ && sh rename.sh
	@$(CC) -fprofile-arcs -ftest-coverage test.c parse.c -o Test -lcheck $(LINUX_LIBS) $(GCOV)
	@./Test
	@lcov -c -d . -o coverage.info
	@genhtml coverage.info -o coverage
	@$(OPEN_GCOV)

install: clean build
	@mkdir $(INSTALL_DIRECTORY)
	@cd $(COMPILATION_DIRECTORY) && qmake 3Dviewer.pro
	@cd $(COMPILATION_DIRECTORY) && make all
	@cp $(COMPILATION_DIRECTORY)3Dviewer $(INSTALL_DIRECTORY)$(PROGRAM_NAME)
	@rm -rf $(COMPILATION_DIRECTORY)

uninstall:
	@rm -rf $(INSTALL_DIRECTORY)

build:
	@mkdir $(COMPILATION_DIRECTORY)
	@mkdir $(COMPILATION_DIRECTORY)QtGifImage-master
	@cp *.c $(COMPILATION_DIRECTORY)
	@cp *.h $(COMPILATION_DIRECTORY)
	@cp S21_3DViewer_v1/3Dviewer.pro $(COMPILATION_DIRECTORY)
	@cp S21_3DViewer_v1/*.cpp $(COMPILATION_DIRECTORY)
	@cp S21_3DViewer_v1/*.h $(COMPILATION_DIRECTORY)
	@cp S21_3DViewer_v1/*.ui $(COMPILATION_DIRECTORY)
	@cp -r QtGifImage-master/* $(COMPILATION_DIRECTORY)QtGifImage-master

	


dist: clean build
	@cp README.html $(COMPILATION_DIRECTORY)
	@tar -czf 3DViewer_v1.tar.gz $(COMPILATION_DIRECTORY)

dvi:
	@firefox README.html



CC = gcc -gdwarf-3
LEX = flex -d
YACC = bison -d
HDRS = parse.tab.h
LDFLAGS = -DYYERROR_VERBOSE -lfl -ly
LIBS = 
OBJS = parse.tab.o lex.yy.o ast.o bst.o main.o
PROGRAM = mycompiler

all:	$(PROGRAM)
$(PROGRAM):	$(OBJS) $(HDRS)
				$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM)

lex.yy.c: lex.l
	$(LEX) lex.l
parse.tab.c: parse.y
	$(YACC) parse.y

clean:;		rm -f *.o*~

###
lex.yy.o: lex.yy.c parse.tab.h
parse.tab.o: parse.tab.c parse.tab.h ast.h
ast.o: ast.c ast.h parse.tab.h
bst.o: bst.c ast.h
main.o: main.c ast.h parse.tab.h

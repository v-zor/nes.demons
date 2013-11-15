LIBS = -lfl
LEX=lex
YACC=yacc

OBJECTS = lex.yy.o\
	y.tab.o\

all: shasm 

clean:
	rm *.o shasm lex.yy.* y.tab.*


shasm:
	$(LEX) scanner.l 
	$(YACC) -d parser.y

	cc -c lex.yy.c
	cc -c y.tab.c
	cc -o shasm *.o $(LIBS)

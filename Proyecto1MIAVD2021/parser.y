%{
    #include "scanner.h"
    #include "Node.h"
    #include <iostream>

    extern int yylineno; //linea actual donde se encuentra el parser (analisis lexico) lo maneja BISON
    extern int columna; //columna actual donde se encuentra el parser (analisis lexico) lo maneja BISON
    extern char *yytext; //lexema actual donde esta el parser (analisis lexico) lo maneja BISON
    extern QList<Node> *listNodos; // lista de Nodos que contienen parametros

    int yyerror(const char* mens){
        std::cout<<mens<<std::endl;
        return 0;
    }


%}

%defines "parser.h"
%output "parser.cpp"
%define parse.error verbose
%union
    {        
        class Node *Node;
        char text[399];
    }

/*----------------------Terminales--------------------*/
%token <text> mkdisk
%token <text> rmdisk
%token <text> fdisk
%token <text> mount
%token <text> umount
%token <text> rep
%token <text> exec
%token <text> size
%token <text> unit
%token <text> path
%token <text> fit
%token <text> name
%token <text> type
%token <text> del
%token <text> add
%token <text> id
%token <text> idmount
%token <text> bf
%token <text> ff
%token <text> wf
%token <text> fast
%token <text> full
%token <text> mbr
%token <text> disk
%token <text> igual
%token <text> extension
%token <text> num
%token <text> caracter
%token <text> cadena
%token <text> identificador
%token <text> ruta


%token <text> directorio

/*----------Not terminales------------*/
%type <Node> INICIO
%type <Node> COMANDO
%type <Node> MKDISK
%type <Node> PARAMETROMKD
%type <Node> AJUSTE
%type <Node> RMDISK


%start INICIO

%%

INICIO:  COMANDO {listNodos= new QList<Node>();};

COMANDO: mkdisk MKDISK {$$=new Node("mkdisk",""); }
        | RMDISK   {$$=new Node("rmdisk",""); };



MKDISK: MKDISK PARAMETROMKD 
        |PARAMETROMKD ;

PARAMETROMKD: size igual num  {$$=new Node("size",$3); }
            |fit igual AJUSTE {$$=new Node("fit",""); };
            |unit igual caracter {$$=new Node("unit",$3); }
            |path igual cadena {$$=new Node("path",$3); }
            |path igual ruta {$$=new Node("path",$3); };
AJUSTE: bf {$$=new Node("fit","bf"); }
        | ff {$$=new Node("fit","ff"); }
        | wf {$$=new Node("fit","wf"); };
RMDISK: rmdisk path igual ruta {$$=new Node("path",$4); }
         | rmdisk path igual cadena {$$=new Node("path",$4); };

%{
    #include "scanner.h"
    #include "Node.h"
    #include <iostream>

    extern int yylineno; //linea actual donde se encuentra el parser (analisis lexico) lo maneja BISON
    extern int columna; //columna actual donde se encuentra el parser (analisis lexico) lo maneja BISON
    extern char *yytext; //lexema actual donde esta el parser (analisis lexico) lo maneja BISON
    extern Node *listNodos; // Nodo raiz de lista de Nodos que contienen parametros

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
%type <Node> FDISK
%type <Node> PARAMETROFDK


%start INICIO

%%

INICIO:  COMANDO {listNodos= new Node("",""); listNodos=$$;};

COMANDO: mkdisk MKDISK {$$=new Node("mkdisk",""); $$->agregar(*$2); }
        | RMDISK  { $$ = $1; }
        | fdisk FDISK { $$ = new Node("fdisk","");
                        $$->agregar(*$2);
                        };



MKDISK: MKDISK PARAMETROMKD {$$ = $1; 
                            $$->agregar(*$2);}
        |PARAMETROMKD {$$ = new Node("",""); 
                        $$->agregar(*$1); };

PARAMETROMKD: size igual num { $$= new Node("size",$3); }
            |fit igual AJUSTE {$$ = new Node ("fit", ""); 
                                $$->agregar(*$3);}
            |unit igual caracter { $$ = new Node("unit",$3);}
            |path igual cadena {$$ = new Node("path",$3);}
            |path igual ruta {$$ = new Node("path",$3);};
AJUSTE: bf { $$ = new Node("AJUSTE", "bf"); }
        | ff { $$ = new Node("AJUSTE", "ff"); }
        | wf { $$ = new Node("AJUSTE", "wf"); };
RMDISK: rmdisk path igual ruta {
                                $$ = new Node("rmdisk","");
                                Node *nodo = new Node("path",$4);
                                $$->agregar(*nodo);
                               }
         | rmdisk path igual cadena {
                                      $$ = new Node("rmdisk","");
                                      Node *ruta = new Node("path",$4);
                                      $$->agregar(*ruta);
                                    };
FDISK: FDISK PARAMETROFDK {
                            $$ = $1;
                            $$->agregar(*$2);
                          }
        | PARAMETROFDK
     {
                        $$ = new Node("","");
                        $$->agregar(*$1);
                      };
PARAMETROFDK: PARAMETROMKD { $$ = $1; }
              | type igual caracter { $$ = new Node("type",$3); }
              | del igual fast { $$ = new Node("delete", "fast"); }
              | del igual full { $$ = new Node("delete", "full"); }
              | name igual identificador { $$ = new Node("name", $3); }
              | name igual cadena { $$ = new Node("name", $3); }
              | add igual num { $$ = new Node("add", $3); };

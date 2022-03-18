%{
    #include "scanner.h"
    #include "clases.h"
    #include <iostream>

    extern int yylineno; //linea actual donde se encuentra el parser (analisis lexico) lo maneja BISON
    extern int columna; //columna actual donde se encuentra el parser (analisis lexico) lo maneja BISON
    extern char *yytext; //lexema actual donde esta el parser (analisis lexico) lo maneja BISON
    extern Node *listNodos; // Nodo raiz de lista de Nodos que contienen parametrosdsada

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
%token <text> mkfs
%token <text> login
%token <text> logout
%token <text> mkgrp
%token <text> rmgrp
%token <text> mkusr
%token <text> rmusr
%token <text> Rchmod
%token <text> mkfile
%token <text> cat
%token <text> rem
%token <text> edit
%token <text> ren
%token <text> Rmkdir
%token <text> cp
%token <text> mv
%token <text> find
%token <text> Rchown
%token <text> chgrp
%token <text> pausa
%token <text> recovery
%token <text> loss
%token <text> fs
%token <text> fs2
%token <text> fs3
%token <text> usr
%token <text> pwd
%token <text> grp
%token <text> ugo
%token <text> r
%token <text> p


%token <text> rutaR

%token <text> inode
%token <text> journaling
%token <text> block
%token <text> bm_inode
%token <text> bm_block
%token <text> tree
%token <text> sb
%token <text> fileRep
%token <text> ls

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
%type <Node> MOUNT
%type <Node> PARAMETRO_M
%type <Node> UMOUNT
%type <Node> MKFS
%type <Node> PARAM_MKFS
%type <Node> EXEC
%type <Node> REP
%type <Node> PARAMETRO_REP

%start INICIO

%%

INICIO:  COMANDO {listNodos= new Node("",""); listNodos=$$;};

COMANDO: mkdisk MKDISK {$$=new Node("mkdisk",""); $$->agregar(*$2); }
        | RMDISK  { $$ = $1; }
        | fdisk FDISK { $$ = new Node("fdisk","");
                        $$->agregar(*$2);
                        }
        | mount MOUNT {
                         $$ = new Node("mount", "");
                         $$->agregar(*$2);
                       }
        | UMOUNT { $$ = $1; }
        | mkfs MKFS {
                        $$ = new Node("mkfs","");
                        $$->agregar(*$2);
                     }
        | EXEC { $$ = $1; }
        | rep REP { $$ = new Node("rep","");
                     $$->agregar(*$2);
                   }
        | pausa {
                        $$ = new Node("pause","");
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

MOUNT: MOUNT PARAMETRO_M {
                           $$ = $1;
                           $$->agregar(*$2);
                         }
       | PARAMETRO_M {
                        $$ = new Node("parametros","");
                        $$->agregar(*$1);
                      };

PARAMETRO_M: path igual cadena { $$ = new Node("path",$3); }
             | path igual ruta { $$ = new Node("path", $3); }
             | name igual identificador { $$ = new Node("name", $3); }
             | name igual cadena { $$ = new Node("name",$3); };

UMOUNT: umount id igual idmount {
                                          $$ = new Node("umount", "");
                                          Node *n = new Node("id",$4);
                                          $$->agregar(*n);
                                        };


MKFS: MKFS PARAM_MKFS {
                        $$ = $1;
                        $$->agregar(*$2);
                      }
      | PARAM_MKFS {
                      $$ = new Node("parametros", "");
                      $$->agregar(*$1);
                   };

PARAM_MKFS: id igual idmount{ $$ = new Node("ident",$3); }
            | type igual fast { $$ = new Node("type", "fast"); }
            | type igual full { $$ = new Node("type", "full"); }
            | fs igual fs2 { $$ = new Node("fs", "2fs"); }
            | fs igual fs3 { $$ = new Node("fs", "3fs"); };

EXEC: exec path igual cadena {
              $$ = new Node("exec","");
              Node *path = new Node("path", $4);
              $$->agregar(*path);
              }
        | exec path igual ruta {
              $$ = new Node("exec","");
              Node *n = new Node("path", $4);
              $$->agregar(*n);
              };
              
REP: REP PARAMETRO_REP{
                     $$ = $1;
                     $$->agregar(*$2);
                    }
     | PARAMETRO_REP
    {
                    $$ = new Node("parametros", "");
                    $$->agregar(*$1);
                  };

PARAMETRO_REP: name igual mbr { $$ = new Node("name","mbr"); }
             | name igual disk { $$ = new Node("name","disk"); }
             | path igual cadena{ $$ = new Node("path", $3); }
             | path igual ruta { $$ = new Node("path",$3); }
             | id igual idmount { $$ = new Node("ident", $3); }
             | name igual inode { $$ = new Node("name", "inode"); }
             | name igual journaling { $$ = new Node("name", "journal"); }
             | name igual block { $$ = new Node("name", "block"); }
             | name igual bm_inode { $$ = new Node("name", "bm_inode"); }
             | name igual bm_block { $$ = new Node("name", "bm_block"); }
             | name igual sb { $$ = new Node("name", "superbloque"); }
             | name igual fileRep { $$ = new Node("name", "file"); }
             | name igual ls { $$ = new Node("name", "ls"); }
             | rutaR igual ruta { $$ = new Node("ruta", $3); }
             | rutaR igual cadena { $$ = new Node("ruta", $3); };

%{
    #include "scanner.h"
    #include "Node.h"
    #include <iostream>

    extern int yylineno; 
    extern int columna;
    extern char *y <cadenas;
    extern Node *listNodos; 

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
        char cadenas[399];
    }

/*----------------------Terminales--------------------*/


%token  <cadenas> mkdisk
%token  <cadenas> rmdisk
%token  <cadenas> fdisk
%token  <cadenas> mount
%token  <cadenas> umount
%token  <cadenas> rep
%token  <cadenas> exec
%token  <cadenas> size
%token  <cadenas> unit
%token  <cadenas> path
%token  <cadenas> fit
%token  <cadenas> name
%token  <cadenas> type
%token  <cadenas> del
%token  <cadenas> add
%token  <cadenas> id
%token  <cadenas> idmount
%token  <cadenas> bf
%token  <cadenas> ff
%token  <cadenas> wf
%token  <cadenas> fast
%token  <cadenas> full
%token  <cadenas> mbr
%token  <cadenas> disk
%token  <cadenas> igual
%token  <cadenas> extension
%token  <cadenas> num
%token  <cadenas> caracter
%token  <cadenas> cadena
%token  <cadenas> identificador
%token  <cadenas> ruta
%token  <cadenas> mkfS
%token  <cadenas> fs
%token  <cadenas> fs2
%token  <cadenas> fs3
%token  <cadenas> rutaR
%token  <cadenas> inode
%token  <cadenas> journaling
%token  <cadenas> block
%token  <cadenas> bm_inode
%token  <cadenas> bm_block
%token  <cadenas> tree
%token  <cadenas> sb
%token  <cadenas> fileRep
%token  <cadenas> ls
%token  <cadenas> xrt
%token  <cadenas> int
%token  <cadenas> actual
%token  <cadenas> reserv
%token  <cadenas> col
%token  <cadenas> part
%token  <cadenas> paramet
%token  <cadenas> inicial
%token  <cadenas> final
%token  <cadenas> par
%token  <cadenas> impar
%token  <cadenas> dev
%token  <cadenas> por
%token  <cadenas> mas
%token  <cadenas> menos
%token  <cadenas> var
%token  <cadenas> tamanio
%token  <cadenas> numero
%token  <cadenas> letra
%token  <cadenas> directorio

/*----------Not terminales------------*/
%type <Node> COMINICIO
%type <Node> COMCOMANDO
%type <Node> COMMKDISK
%type <Node> COMPARAMETROMKD
%type <Node> COMAJUSTE
%type <Node> COMRMDISK
%type <Node> COMFDISK
%type <Node> COMPARAMETROFDK
%type <Node> COMMOUNT
%type <Node> COMPARAMETRO_M
%type <Node> COMUMOUNT
%type <Node> COMMKFS
%type <Node> COMPARAM_MKFS
%type <Node> COMEXEC
%type <Node> COMREP
%type <Node> COMPARAMETRO_REP

%start COMINICIO

%%

COMINICIO:  COM {listNodos= new Node("",""); listNodos=$$;};

COM: mkdisk COMMKDISK {$$=new Node("mkdisk",""); $$->agregar(*$2); }
        | COMRMDISK  { $$ = $1; }
        | COMfdisk COMFDISK { $$ = new Node("COMfdisk","");
                        $$->agregar(*$2);
                        }
        | mount COMMOUNT {
                         $$ = new Node("mount", "");
                         $$->agregar(*$2);
                       }
        | COMUMOUNT { $$ = $1; }
        | mkfs COMMKFS {
                        $$ = new Node("mkfs","");
                        $$->agregar(*$2);
                     }
        | COMEXEC { $$ = $1; }
        | rep COMREP { $$ = new Node("rep","");
                     $$->agregar(*$2);
                   }
        | pausa {
                        $$ = new Node("pause","");
                     };



COMMKDISK: COMMKDISK COMPARAMETROMKD {$$ = $1; 
                            $$->agregar(*$2);}
        |COMPARAMETROMKD {$$ = new Node("",""); 
                        $$->agregar(*$1); };

COMPARAMETROMKD: size igual num { $$= new Node("size",$3); }
            |fit igual COMAJUSTE {$$ = new Node ("fit", ""); 
                                $$->agregar(*$3);}
            |unit igual caracter { $$ = new Node("unit",$3);}
            |path igual cadena {$$ = new Node("path",$3);}
            |path igual ruta {$$ = new Node("path",$3);};
COMAJUSTE: bf { $$ = new Node("COMAJUSTE", "bf"); }
        | ff { $$ = new Node("COMAJUSTE", "ff"); }
        | wf { $$ = new Node("COMAJUSTE", "wf"); };
COMRMDISK: rmdisk path igual ruta {
                                $$ = new Node("rmdisk","");
                                Node *nodo = new Node("path",$4);
                                $$->agregar(*nodo);
                               }
         | rmdisk path igual cadena {
                                      $$ = new Node("rmdisk","");
                                      Node *ruta = new Node("path",$4);
                                      $$->agregar(*ruta);
                                    };
COMFDISK: COMFDISK COMPARAMETROFDK {
                            $$ = $1;
                            $$->agregar(*$2);
                          }
        | COMPARAMETROFDK
     {
                        $$ = new Node("","");
                        $$->agregar(*$1);
                      };
COMPARAMETROFDK: COMPARAMETROMKD { $$ = $1; }
              | type igual caracter { $$ = new Node("type",$3); }
              | del igual fast { $$ = new Node("delete", "fast"); }
              | del igual full { $$ = new Node("delete", "full"); }
              | name igual identificador { $$ = new Node("name", $3); }
              | name igual cadena { $$ = new Node("name", $3); }
              | add igual num { $$ = new Node("add", $3); };

COMMOUNT: COMMOUNT COMPARAMETRO_M {
                           $$ = $1;
                           $$->agregar(*$2);
                         }
       | COMPARAMETRO_M {
                        $$ = new Node("parametros","");
                        $$->agregar(*$1);
                      };

COMPARAMETRO_M: path igual cadena { $$ = new Node("path",$3); }
             | path igual ruta { $$ = new Node("path", $3); }
             | name igual identificador { $$ = new Node("name", $3); }
             | name igual cadena { $$ = new Node("name",$3); };

COMUMOUNT: COMumount id igual idmount {
                                          $$ = new Node("COMumount", "");
                                          Node *n = new Node("id",$4);
                                          $$->agregar(*n);
                                        };


COMMKFS: COMMKFS COMPARAM_MKFS {
                        $$ = $1;
                        $$->agregar(*$2);
                      }
      | COMPARAM_MKFS {
                      $$ = new Node("parametros", "");
                      $$->agregar(*$1);
                   };

COMPARAM_MKFS: id igual idmount{ $$ = new Node("ident",$3); }
            | type igual fast { $$ = new Node("type", "fast"); }
            | type igual full { $$ = new Node("type", "full"); }
            | fs igual fs2 { $$ = new Node("fs", "2fs"); }
            | fs igual fs3 { $$ = new Node("fs", "3fs"); };

COMEXEC: exec path igual cadena {
              $$ = new Node("exec","");
              Node *path = new Node("path", $4);
              $$->agregar(*path);
              }
        | exec path igual ruta {
              $$ = new Node("exec","");
              Node *n = new Node("path", $4);
              $$->agregar(*n);
              };
              
COMREP: COMREP COMPARAMETRO_REP{
                     $$ = $1;
                     $$->agregar(*$2);
                    }
     | COMPARAMETRO_REP
    {
                    $$ = new Node("parametros", "");
                    $$->agregar(*$1);
                  };

COMPARAMETRO_REP: name igual mbr { $$ = new Node("name","mbr"); }
              | name igual bm_inode { $$ = new Node("name", "bm_inode"); }
             | name igual bm_block { $$ = new Node("name", "bm_block"); }
             | name igual sb { $$ = new Node("name", "superbloque"); }
             | name igual fileRep { $$ = new Node("name", "file"); }
             | name igual disk { $$ = new Node("name","disk"); }
             | path igual cadena{ $$ = new Node("path", $3); }
             | path igual ruta { $$ = new Node("path",$3); }
             | id igual idmount { $$ = new Node("ident", $3); }
             | name igual inode { $$ = new Node("name", "inode"); }
             | name igual journaling { $$ = new Node("name", "journal"); }
             | name igual block { $$ = new Node("name", "block"); }
             | name igual ls { $$ = new Node("name", "ls"); }
             | rutaR igual ruta { $$ = new Node("ruta", $3); }
             | rutaR igual cadena { $$ = new Node("ruta", $3); };
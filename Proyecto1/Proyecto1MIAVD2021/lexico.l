%{
    #include <iostream>
    #include "parser.h"
    #include <QList>
    char coment[400];
    int yylex();
    void yyerror(const char *s);
    int linea = 0;
    int columna = 1;
    int anterior = 0;
    QList<Node> *listNodos;
%}

%option case-insensitive
%option noyywrap

letra [a-zA-Z]
entero ("-")?[0-9]+
caracter ({letra}|{entero})
idmount "vd"({letra}{entero})
id {letra}({letra}|{entero}|"_")*

password ({letra}|{entero}|[!$@+*])+
cadena "\""[^"\""]*"\""
extension "."{id}
diagonal "/"
ruta ({diagonal}{id})*({diagonal}{id}{extension})
directorio ({diagonal}{id})+

%x COM

%%
INITIAL
{
    "#"         { BEGIN COM; }

    "mkdisk"    { return mkdisk; }
    "rmdisk"    { return rmdisk; }
    "fdisk"     { return fdisk; }
    "mount"     { return mount; }
    "unmount"   { return umount; }
    "exec"      { return exec; }
    "rep"      { return rep; }

    "-ruta"      { return rutaR; }
    "-size"     { return size; }
    "-unit"     { return unit; }
    "-path"     { return path; }
    "-fit"      { return fit; }
    "-name"     { return name; }
    "-type"     { return type; }
    "-delete"   { return del; }
    "-add"      { return add; }
    "-id"       { return id; }
    "bf"        { return bf; }
    "ff"        { return ff; }
    "wf"        { return wf; }
    "Fast"      { return fast; }
    "Full"      { return full; }
    "mbr"       { return mbr; }
    "disk"      { return disk; }

    "inode"      { return inode; }
    "journaling" { return journaling; }
    "block"      { return block; }
    "bm_inode"   { return bm_inode; }
    "bm_block"   { return bm_block; }
    "tree"       { return tree; }
    "sb"         { return sb; }
    "file"       { return fileRep; }
    "ls"         { return ls; }
    
    "mkfs"       { return mkfs; }
    "login"      { return login; }
    "logout"     { return logout; }
    "mkgrp"      { return mkgrp; }
    "rmgrp"      { return rmgrp; }
    "mkusr"      { return mkusr; }
    "rmusr"      { return rmusr; }
    "chmod"      { return Rchmod; }
    "mkfile"     { return mkfile; }
    "pause"     { return pausa; }

    "-fs"        { return fs; }
    "2fs"        { return fs2; }
    "3fs"        { return fs3; }
    "-r"         { return r; }
    "-p"         { return p; }

    "~:~"         { return igual; }

    {entero}    {strcpy(yylval.text, yytext); return num;}
    {caracter}  {strcpy(yylval.text, yytext); return caracter;}
    {cadena}    {strcpy(yylval.text, yytext); return cadena;}
    {idmount}    {strcpy(yylval.text, yytext); return idmount;}
    {id}        {strcpy(yylval.text, yytext); return identificador;}
    {ruta}      {strcpy(yylval.text, yytext); return ruta;}
    {extension} {strcpy(yylval.text, yytext); return extension;}
    {directorio} {strcpy(yylval.text, yytext); return directorio;}

    [[:blank:]] {}

    .            {std::cout <<yytext <<" Error Lexico" << std::endl;}
}

<COM>"\n"        { BEGIN INITIAL; /*strcpy(yylval.text, coment); memset(coment,0,400); return comentario;*/ };
<COM>.           { /*strcat(coment, yytext);*/ };

%%
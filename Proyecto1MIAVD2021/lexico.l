%{
    #include <iostream>
    #include "parser.h"
    char coment[400];
    int yylex();
    void yyerror(const char *s);
    int linea = 0;
    int columna = 1;
    int anterior = 0;
%}

%option case-insensitive
%option noyywrap

letra [a-zA-Z]
entero ("-")?[0-9]+
caracter ({letra}|{entero})
id {letra}({letra}|{entero}|"_")*
idmount {entero}{letra}

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
    "umount"   { return umount; }
    "exec"      { return exec; }
    "rep"      { return rep; }

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



    "~:~"         { return igual; }

    {entero}    {strcpy(yylval.text, yytext); return num;}
    {caracter}  {strcpy(yylval.text, yytext); return caracter;}
    {cadena}    {strcpy(yylval.text, yytext); return cadena;}
    {id}        {strcpy(yylval.text, yytext); return identificador;}
    {idmount}        {strcpy(yylval.text, yytext); return idmount;}
    {ruta}      {strcpy(yylval.text, yytext); return ruta;}
    {extension} {strcpy(yylval.text, yytext); return extension;}
    {directorio} {strcpy(yylval.text, yytext); return directorio;}

    [[:blank:]] {}

    .            {std::cout <<yytext <<" Error Lexico" << std::endl;}
}

<COM>"\n"        { BEGIN INITIAL; /*strcpy(yylval.text, coment); memset(coment,0,400); return comentario;*/ };
<COM>.           { /*strcat(coment, yytext);*/ };

%%

#include <QCoreApplication>
#include <iostream>
#include <QString>
#include <stdio.h>
#include <string.h>
#include <QTextStream>

using namespace std;

bool comExit(char exit[399]){
    if(exit[0]=='e' && exit[1]=='x' && exit[2]=='i' && exit[3]=='t' ){
        return true;
    }
    return false;

}

int main()
{
    char encabezado[] = "------------------------------SISTEMA DE ARCHIVOS-----------------------------\n"
             "By: Bryan Gerardo Paez Morales_______________________________________201700945\n";
    printf("%s",encabezado);
    char lcomando[399];
    string exit = "exit";
    QTextStream qtin(stdin);
    while(true){
        printf("~");
        fgets(lcomando,sizeof (lcomando),stdin);
        if (comExit(lcomando)){
            break;
        }
    }

    return 0;
}

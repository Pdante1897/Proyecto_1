#include "mkfs.h"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

using namespace std;
extern QList<ParticionMount> partMontadas;
extern QList<QString> pathsMontados;
enum choice{
    SIZE=2,
    FIT=3,
    UNIT=4,
    PATH=5,
    ADD=8,
    TYPE=9,
    DELETE=10,
    NAME=11,
    ID=18,
    FS=19

};

void validarMKFS(Node *Raiz){
    bool banderaID = true;
    bool banderaType = true;
    bool banderaFs = true;
    bool bandera = true;
    QString id = "";
    QString type = "";
    int fileSystem = 2;//si no viene un valor por defecto usa el ext2
    for(int i = 0; i < Raiz->hijitos.count(); i++)
    {
        Node nodito = Raiz->hijitos.at(i);
        nodito.asignarTipo();
        switch (nodito.tipo) {
        case ID:
        {
            if(banderaID){
                banderaID = false;
                id = nodito.valor;
            }else{
                printf(ANSI_COLOR_CYAN"Error! Ya fue definido el parametro -ID \n");
                bandera = false;
                break;
            }

        }
            break;
        case TYPE:
        {
            if(banderaType){
                banderaType = false;
                type = nodito.valor;
            }else{
                printf(ANSI_COLOR_CYAN"Error! Ya fue definido el parametro -TYPE \n");
                bandera = false;
                break;
            }

        }
            break;
        case FS:
        {
            if(banderaFs){

            }else{
                printf(ANSI_COLOR_CYAN"Error! Ya fue definido el parametro -FS \n");
                bandera = false;
                break;
            }
            banderaFs = false;
            if(nodito.valor == "3fs"){fileSystem = 3;}
            else{fileSystem = 2;}

        }
            break;
        }
    }
    if(bandera){
        if(!banderaID){
            ParticionMount aux = getPart(id);
            ParticionMount aux2 = *new ParticionMount("","",' ',0,"");

            QString name="";
            QString path="";
            for(int i=0; i<partMontadas.length();i++){
                if(partMontadas.at(i).id==id){
                    name = partMontadas.at(i).name;
                    path = partMontadas.at(i).dir;
                }
            }
            if(aux.id!="nulo"){
                FILE *archivo = fopen(path.toStdString().c_str(), "rb+");
                MBR mbr;
                fseek(archivo,0,SEEK_SET);
                fread(&mbr,sizeof (MBR),1,archivo);

                int index=-1;
                bool part=false;
                bool banderaExtend=false;
                for(int i = 0; i < 4; i++){
                    if((strcmp(mbr.mbr_partitions[i].part_name, name.toStdString().c_str()) == 0)){
                        index = i;
                        part=true;
                        if(mbr.mbr_partitions[i].part_type == 'E')
                            banderaExtend = true;
                        break;
                    }
                }

                if(part and !banderaExtend){
                    MBR mbr;
                    archivo = fopen(aux.dir.toStdString().c_str(),"rb+");
                    fread(&mbr,sizeof(MBR),1,archivo);
                    int inicio = mbr.mbr_partitions[index].part_start;
                    int tamanio = mbr.mbr_partitions[index].part_size;
                    QString dir;
                    if(fileSystem == 3){
                        MKFSExt3(inicio, tamanio, aux.dir);
                    }
                    else{
                            MKFSExt2(inicio, tamanio, aux.dir);
                    }
                    fclose(archivo);
                }else if(!banderaExtend){
                    MBR mbr;
                    EBR ebr;
                    int inicio;
                    int tamanio;
                    if((archivo = fopen(path.toStdString().c_str(),"rb+"))){
                        int ext = -1;
                        fseek(archivo,0,SEEK_SET);
                        fread(&mbr,sizeof(MBR),1,archivo);
                        for(int i = 0; i < 4; i++){
                            if(mbr.mbr_partitions[i].part_type == 'E'){
                                ext = i;
                                break;
                            }
                        }
                        if(ext != -1){
                            fseek(archivo, mbr.mbr_partitions[ext].part_start,SEEK_SET);
                            while(fread(&ebr,sizeof(EBR),1,archivo)!=0 && (ftell(archivo) < mbr.mbr_partitions[ext].part_start + mbr.mbr_partitions[ext].part_size)){
                                if(strcmp(ebr.part_name, name.toStdString().c_str()) == 0){
                                    inicio = ebr.part_start;
                                    tamanio = ebr.part_size;
                                    if(fileSystem == 3){
                                        MKFSExt3(inicio, tamanio, aux.dir);
                                    }
                                    else{
                                            MKFSExt2(inicio, tamanio, aux.dir);
                                    }
                                }
                            }
                        }
                    }
                    fclose(archivo);

                }
            }else
                printf(ANSI_COLOR_CYAN"ERROR: No se ha encontrado ninguna particion montada con el id ingresado \n");
        }else
            printf(ANSI_COLOR_CYAN"ERROR: El parametro ID no fue definido \n");
    }
}

ParticionMount getPart(QString id){
    ParticionMount part = *new ParticionMount("","",' ',0,"");
    part.id="nulo";
    for(int i=0; i<partMontadas.length();i++){
        if(partMontadas.at(i).id==id){
            part=partMontadas.at(i);
            return part;
        }
    }
    return part;
}

void MKFSExt2(int inicio, int tamanio, QString dir){
        double n = (tamanio - static_cast<int>(sizeof(SuperBloque)))/(4 + static_cast<int>(sizeof(TablaInodos)) +3*static_cast<int>(sizeof(BloqueDeArchivos)));
        int num_estructuras = static_cast<int>(floor(n));//Numero de inodos
        int num_bloques = 3*num_estructuras;
        SuperBloque superB;
        FILE *archivote = fopen(dir.toStdString().c_str(),"rb+");
        BloqueDeArchivos archivo;//Bloque para users.txt
        superB.s_filesystem_type = 2;
        superB.s_inodes_count = num_estructuras;
        superB.s_blocks_count = num_bloques;
        superB.s_free_blocks_count = num_bloques -2;
        superB.s_free_inodes_count = num_estructuras -2;
        superB.s_mtime = time(nullptr);
        superB.s_umtime = 0;
        superB.s_mnt_count = 0;
        superB.s_magic = 0xEF53;
        superB.s_inode_size = sizeof(TablaInodos);
        superB.s_block_size = sizeof(BloqueDeArchivos);
        superB.s_first_ino = 2;
        superB.s_first_blo = 2;
        superB.s_bm_inode_start = inicio + static_cast<int>(sizeof(SuperBloque));
        superB.s_bm_block_start = inicio + static_cast<int>(sizeof(SuperBloque)) + num_estructuras;
        superB.s_inode_start = inicio + static_cast<int>(sizeof (SuperBloque)) + num_estructuras + num_bloques;
        superB.s_block_start = inicio + static_cast<int>(sizeof(SuperBloque)) + num_estructuras + num_bloques + (static_cast<int>(sizeof(TablaInodos))*num_estructuras);

        TablaInodos inodoT;
        BloqueCarpeta bloqueC;
        char buffer = '0';
        char buffer2 = '1';
        char buffer3 = '2';

        fseek(archivote,inicio,SEEK_SET);//superbloque
        fwrite(&superB,sizeof(SuperBloque),1,archivote);

        for(int i = 0; i < num_estructuras; i++){//bitmap de inodos
            fseek(archivote,superB.s_bm_inode_start + i,SEEK_SET);
            fwrite(&buffer,sizeof(char),1,archivote);
        }
        fseek(archivote,superB.s_bm_inode_start,SEEK_SET);//bit para / y users.txt en BM
        fwrite(&buffer2,sizeof(char),1,archivote);
        fwrite(&buffer2,sizeof(char),1,archivote);
        for(int i = 0; i < num_bloques; i++){//bitmap de bloques
            fseek(archivote,superB.s_bm_block_start + i,SEEK_SET);
            fwrite(&buffer,sizeof(char),1,archivote);
        }
        fseek(archivote,superB.s_bm_block_start,SEEK_SET);//bit para / y users.txt en BM
        fwrite(&buffer2,sizeof(char),1,archivote);
        fwrite(&buffer3,sizeof(char),1,archivote);
        inodoT.i_uid = 1;//inodo para carpeta root
        inodoT.i_gid = 1;
        inodoT.i_size = 0;
        inodoT.i_atime = time(nullptr);
        inodoT.i_ctime = time(nullptr);
        inodoT.i_mtime = time(nullptr);
        inodoT.i_block[0] = 0;
        for(int i = 1; i < 15;i++){
            inodoT.i_block[i] = -1;
        }
        inodoT.i_type = '0';
        inodoT.i_perm = 664;
        fseek(archivote,superB.s_inode_start,SEEK_SET);
        fwrite(&inodoT,sizeof(TablaInodos),1,archivote);

        //-bloque para carpeta root
        strcpy(bloqueC.b_content[0].b_name,".");//Actual (el mismo)
        bloqueC.b_content[0].b_inodo=0;

        strcpy(bloqueC.b_content[1].b_name,"..");//Padre
        bloqueC.b_content[1].b_inodo=0;

        strcpy(bloqueC.b_content[2].b_name,"users.txt");
        bloqueC.b_content[2].b_inodo=1;

        strcpy(bloqueC.b_content[3].b_name,".");
        bloqueC.b_content[3].b_inodo=-1;
        fseek(archivote,superB.s_block_start,SEEK_SET);
        fwrite(&bloqueC,sizeof(BloqueCarpeta),1,archivote);

        inodoT.i_uid = 1;//inodo para users.txt
        inodoT.i_gid = 1;
        inodoT.i_size = 27;
        inodoT.i_atime = time(nullptr);
        inodoT.i_ctime = time(nullptr);
        inodoT.i_mtime = time(nullptr);
        inodoT.i_block[0] = 1;
        for(int i = 1; i < 15;i++){
            inodoT.i_block[i] = -1;
        }
        inodoT.i_type = '1';
        inodoT.i_perm = 755;
        fseek(archivote,superB.s_inode_start + static_cast<int>(sizeof(TablaInodos)),SEEK_SET);
        fwrite(&inodoT,sizeof(TablaInodos),1,archivote);

        memset(archivo.b_content,0,sizeof(archivo.b_content));
        strcpy(archivo.b_content,"1,G,root\n1,U,root,root,123\n");
        fseek(archivote,superB.s_block_start + static_cast<int>(sizeof(BloqueCarpeta)),SEEK_SET);
        fwrite(&archivo,sizeof(BloqueDeArchivos),1,archivote);
        printf(ANSI_COLOR_RED"~~~>Formato Ext2 \n");
        printf(ANSI_COLOR_RED"~~~>Disco formateado con exito! \n");
        fclose(archivote);
}

void MKFSExt3(int inicio, int tamanio, QString direccion){
    double n = (tamanio - static_cast<int>(sizeof(SuperBloque)))/(4 + static_cast<int>(sizeof(TablaInodos)) +3*static_cast<int>(sizeof(BloqueDeArchivos)));
    int num_estructuras = static_cast<int>(floor(n));//Bitmap de indos
    int num_bloques = 3*num_estructuras;//Bitmap de bloques
    int super_size = static_cast<int>(sizeof(SuperBloque));
    int journal_size = static_cast<int>(sizeof(Journal))*num_estructuras;
    SuperBloque superB;
    TablaInodos inodoT;
    BloqueCarpeta bloqueC;
    BloqueDeArchivos archivo;    //Bloque para users.txt
    FILE *archivote = fopen(direccion.toStdString().c_str(),"rb+");
    superB.s_filesystem_type = 3;
    superB.s_inodes_count = num_estructuras;
    superB.s_blocks_count = num_bloques;
    superB.s_free_blocks_count = num_bloques - 2;
    superB.s_free_inodes_count = num_estructuras - 2;
    superB.s_mtime = time(nullptr);
    superB.s_umtime = 0;
    superB.s_mnt_count = 0;
    superB.s_magic = 0xEF53;
    superB.s_inode_size = sizeof(TablaInodos);
    superB.s_block_size = sizeof(BloqueDeArchivos);
    superB.s_first_ino = 2;
    superB.s_first_blo = 2;
    superB.s_bm_inode_start = inicio + super_size + journal_size;
    superB.s_bm_block_start = inicio + super_size + journal_size + num_estructuras;
    superB.s_inode_start = inicio + super_size + journal_size + num_estructuras + num_bloques;
    superB.s_block_start = inicio + super_size + journal_size + num_estructuras + num_bloques + static_cast<int>(sizeof(TablaInodos))*num_estructuras;
    char bf1 = '0';
    char bf2 = '1';
    char bf3 = '2';

    fseek(archivote,inicio,SEEK_SET);//superbloque
    fwrite(&superB,sizeof(SuperBloque),1,archivote);
    for(int i = 0; i < num_estructuras; i++){//BITMAP DE INODOS
        fseek(archivote,superB.s_bm_inode_start + i,SEEK_SET);
        fwrite(&bf1,sizeof(char),1,archivote);
    }

    fseek(archivote,superB.s_bm_inode_start,SEEK_SET);//bit para / y users.txt en BM
    fwrite(&bf2,sizeof(char),1,archivote);
    fwrite(&bf2,sizeof(char),1,archivote);

    for(int i = 0; i < num_bloques; i++){//BITMAP DE BLOQUES
        fseek(archivote,superB.s_bm_block_start + i,SEEK_SET);
        fwrite(&bf1,sizeof(char),1,archivote);
    }

    fseek(archivote,superB.s_bm_block_start,SEEK_SET);//bit para / y users.txt en BM
    fwrite(&bf2,sizeof(char),1,archivote);
    fwrite(&bf3,sizeof(char),1,archivote);

    inodoT.i_uid = 1;//inodo para carpeta root
    inodoT.i_gid = 1;
    inodoT.i_size = 0;
    inodoT.i_atime = time(nullptr);
    inodoT.i_ctime = time(nullptr);
    inodoT.i_mtime = time(nullptr);
    inodoT.i_block[0] = 0;
    for(int i = 1; i < 15;i++){
        inodoT.i_block[i] = -1;
    }
    inodoT.i_type = '0';
    inodoT.i_perm = 664;
    fseek(archivote,superB.s_inode_start,SEEK_SET);
    fwrite(&inodoT,sizeof(TablaInodos),1,archivote);
    //-Bloque para carpeta root
    strcpy(bloqueC.b_content[0].b_name,".");//Actual
    bloqueC.b_content[0].b_inodo=0;

    strcpy(bloqueC.b_content[1].b_name,"..");//Padre
    bloqueC.b_content[1].b_inodo=0;

    strcpy(bloqueC.b_content[2].b_name,"users.txt");
    bloqueC.b_content[2].b_inodo=1;

    strcpy(bloqueC.b_content[3].b_name,".");
    bloqueC.b_content[3].b_inodo=-1;
    fseek(archivote,superB.s_block_start,SEEK_SET);
    fwrite(&bloqueC,sizeof(BloqueCarpeta),1,archivote);


    inodoT.i_uid = 1;//inodo para users.txt
    inodoT.i_gid = 1;
    inodoT.i_size = 27;
    inodoT.i_atime = time(nullptr);
    inodoT.i_ctime = time(nullptr);
    inodoT.i_mtime = time(nullptr);
    inodoT.i_block[0] = 1;
    for(int i = 1; i < 15;i++){
        inodoT.i_block[i] = -1;
    }
    inodoT.i_type = '1';
    inodoT.i_perm = 755;
    fseek(archivote,superB.s_inode_start + static_cast<int>(sizeof(TablaInodos)),SEEK_SET);
    fwrite(&inodoT,sizeof(TablaInodos),1,archivote);
    memset(archivo.b_content,0,sizeof(archivo.b_content));
    strcpy(archivo.b_content,"1,G,root\n1,U,root,root,123\n");
    fseek(archivote,superB.s_block_start + static_cast<int>(sizeof(BloqueCarpeta)),SEEK_SET);
    fwrite(&archivo,sizeof(BloqueDeArchivos),1,archivote);
    printf(ANSI_COLOR_RED"~~~>Formato Ext3 \n");
    printf(ANSI_COLOR_RED"~~~>Disco formateado con exito! \n");
    fclose(archivote);
}




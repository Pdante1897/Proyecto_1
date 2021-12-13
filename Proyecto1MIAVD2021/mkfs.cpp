#include "mkfs.h"


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
    FS=17,
    ID=18
};

void validarMKFS(Node *Raiz){
    bool banderaID = true;
    bool banderaType = true;
    bool banderaFs = true;
    bool bandera = true;
    QString id = "";
    QString type = "";
    int fileSystem = 2;//si no viene un valor por defecto usa el ext2
    for(int i = 0; i < Raiz->hijos.count(); i++)
    {
        Node nodito = Raiz->hijos.at(i);
        switch (nodito.tipo) {
        case ID:
        {
            if(banderaID){
                banderaID = true;
                id = nodito.valor;
            }else{
                printf("Error! Ya fue definido el parametro -ID \n");
                bandera = true;
                break;
            }

        }
            break;
        case TYPE:
        {
            if(banderaType){
                banderaType = true;
                type = nodito.valor;
            }else{
                printf("Error! Ya fue definido el parametro -TYPE \n");
                bandera = true;
                break;
            }

        }
            break;
        case FS:
        {
            if(banderaFs){

            }else{
                printf("Error! Ya fue definido el parametro -FS \n");
                bandera = true;
                break;
            }
            banderaFs = true;
            if(nodito.valor == "3fs"){fileSystem = 3;}
            else{fileSystem = 2;}

        }
            break;
        }
    }
    if(!bandera){
        if(banderaID){
            ParticionMount *aux = getPart(id);
            if(aux!=nullptr){
                int index;
                //= particion.buscarPartP_E(aux->dir,aux->name);
                if(index != -1){
                    MBR mbr;
                    FILE *filep = fopen(aux->dir.toStdString().c_str(),"rb+");
                    fread(&mbr,sizeof(MBR),1,filep);
                    int inicio = mbr.mbr_partitions[index].part_start;
                    int tamanio = mbr.mbr_partitions[index].part_size;
                    QString dir;
                    if(fileSystem == 3){
                        MKFSExt3(inicio, tamanio, aux->dir);
                    }
                    else{
                            MKFSExt2(inicio, tamanio, aux->dir);
                    }
                    fclose(filep);
                }else{
                    //index = particion.buscarPart_L(aux->dir,aux->name);
                }
            }else
                cout << "ERROR: No se ha encontrado ninguna particion montada con el id ingresado" << endl;
        }else
            cout << "ERROR: El parametro ID no fue definido" << endl;
    }
}

ParticionMount* getPart(QString id){
    ParticionMount *part;
    for(int i=0; i<partMontadas.length();i++){
        if(partMontadas.at(i).id==id){
            *part=partMontadas.at(i);
            return part;
        }
    }
    return nullptr;
}

void MKFSExt2(int inicio, int tamanio, QString dir){
        double n = (tamanio - static_cast<int>(sizeof(SuperBloque)))/(4 + static_cast<int>(sizeof(TablaInodos)) +3*static_cast<int>(sizeof(BloqueDeArchivos)));
        int num_estructuras = static_cast<int>(floor(n));//Numero de inodos
        int num_bloques = 3*num_estructuras;
        SuperBloque superB;
        FILE *archivote = fopen(dir.toStdString().c_str(),"rb+");

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

        BloqueDeArchivos archivo;//Bloque para users.txt
        memset(archivo.b_content,0,sizeof(archivo.b_content));
        strcpy(archivo.b_content,"1,G,root\n1,U,root,root,123\n");
        fseek(archivote,superB.s_block_start + static_cast<int>(sizeof(BloqueCarpeta)),SEEK_SET);
        fwrite(&archivo,sizeof(BloqueDeArchivos),1,archivote);
        cout << "Formato Ext2" << endl;
        cout << "..." << endl;
        cout << "Disco formateado con exito!" << endl;
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
    char buffer1 = '0';
    char buffer2 = '1';
    char buffer3 = '2';

    fseek(archivote,inicio,SEEK_SET);//superbloque
    fwrite(&superB,sizeof(SuperBloque),1,archivote);
    for(int i = 0; i < num_estructuras; i++){//BITMAP DE INODOS
        fseek(archivote,superB.s_bm_inode_start + i,SEEK_SET);
        fwrite(&buffer1,sizeof(char),1,archivote);
    }

    fseek(archivote,superB.s_bm_inode_start,SEEK_SET);//bit para / y users.txt en BM
    fwrite(&buffer2,sizeof(char),1,archivote);
    fwrite(&buffer2,sizeof(char),1,archivote);

    for(int i = 0; i < num_bloques; i++){//BITMAP DE BLOQUES
        fseek(archivote,superB.s_bm_block_start + i,SEEK_SET);
        fwrite(&buffer1,sizeof(char),1,archivote);
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
    printf("~~~>Formato Ext3 \n");
    printf("~~~>Disco formateado con exito! \n");
    fclose(archivote);
}




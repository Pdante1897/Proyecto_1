#Crea 2 discos

#CREANDO DISCOS ----------------------------------------------------------------------------------------
mkdisk -size~:~75 -unit~:~M -path~:~/home/bryan/Escritorio/diciembre/Disco1.disk -fit~:~FF
pause
mkdisk -unit~:~K -size~:~51200 -path~:~/home/bryan/Escritorio/diciembre/Disco2.disk -fit~:~FF
pause
#CREANDO PARTICIONES ----------------------------------------------------------------------------------
#Crear particiones Disco1.disk
pause
fdisk -size~:~20 -type~:~P -unit~:~M -name~:~Part1  -path~:~/home/bryan/Escritorio/diciembre/Disco1.disk -fit~:~BF
pause
fdisk -size~:~20 -type~:~P -unit~:~M -name~:~Part1  -path~:~/home/bryan/Escritorio/vacas/Disco1.disk -fit~:~BF
pause
fdisk -size~:~20 -type~:~E -unit~:~M -name~:~Part2  -path~:~/home/bryan/Escritorio/diciembre/Disco1.disk -fit~:~BF
pause
fdisk -size~:~5 -type~:~L -unit~:~M -name~:~Part4L  -path~:~/home/bryan/Escritorio/diciembre/Disco1.disk -fit~:~BF
pause
fdisk -size~:~5 -type~:~L -unit~:~M -name~:~Part5L  -path~:~/home/bryan/Escritorio/diciembre/Disco1.disk -fit~:~BF
pause
fdisk -type~:~P -unit~:~M -name~:~Part2 -size~:~15 -path~:~/home/bryan/Escritorio/fase1/Disco1.disk -fit~:~BF
pause
fdisk -type~:~P -unit~:~M -name~:~Part3 -size~:~20 -path~:~/home/bryan/Escritorio/diciembre/Disco1.disk -fit~:~BF
#pause
#fdisk -delete~:~fast -name~:~Part2 -path~:~/home/bryan/Escritorio/diciembre/Disco1.disk 
#pause
#fdisk -delete~:~full -name~:~Part2 -path~:~/home/bryan/Escritorio/diciembre/Disco1.disk 
#pause
#Crear particiones Disco2.disk
fdisk -type~:~P -unit~:~M -name~:~Part1_D2 -size~:~20 -path~:~/home/bryan/Escritorio/fase1/Disco2.disk -fit~:~BF
pause
fdisk -type~:~P -unit~:~M -name~:~Part2_D2 -size~:~10 -path~:~/home/bryan/Escritorio/fase1/Disco2.disk -fit~:~BF
pause
fdisk -type~:~P -unit~:~M -name~:~Part3_D2 -size~:~10 -path~:~/home/bryan/Escritorio/fase1/Disco2.disk -fit~:~BF
pause

fdisk -add~:~25 -unit~:~m -name~:~Part2  -path~:~/home/bryan/Escritorio/diciembre/Disco1.disk 
#Monta las particiones 
pause


mount -path~:~/home/bryan/Escritorio/diciembre/Disco1.disk -name~:~Part1 
mount -path~:~/home/bryan/Escritorio/vacas/Disco1.disk -name~:~Part1 
pause

mount -path~:~/home/bryan/Escritorio/diciembre/Disco1.disk -name~:~Part2 
mount -path~:~/home/bryan/Escritorio/diciembre/Disco1.disk -name~:~Part3 
pause
mount -path~:~/home/bryan/Escritorio/diciembre/Disco1.disk -name~:~Part4L 

mount -path~:~/home/bryan/Escritorio/diciembre/Disco2.disk -name~:~Part1_D2 
pause
mount -path~:~/home/bryan/Escritorio/diciembre/Disco2.disk -name~:~Part2_D2 
mount -path~:~/home/bryan/Escritorio/diciembre/Disco2.disk -name~:~Part3_D2 
pause


unmount -id~:~vda1 

#Crea el sistema de bryan/Escritorio en todas las particiones primarias
#fast reescribe el superbloque 
#full poner /0 y luego reescribir el superbloque

mkfs -type~:~full -id~:~vda1
pause
mkfs -type~:~fast -id~:~vda1 -fs~:~3fs
pause
mkfs -type~:~full -id~:~vdc1
pause
mkfs -type~:~fast -id~:~vda2 -fs~:~3fs
pause

rep -id~:~vda2 -path~:~/home/bryan/Escritorio/diciembre/REPORTEMbr.png -name~:~MBR
rep -id~:~vda1 -path~:~/home/bryan/Escritorio/diciembre/REPORTEDisk.png -name~:~disk
rep -id~:~vda1 -path~:~/home/bryan/Escritorio/diciembre/REPORTESuperBloque.png -name~:~sb
#reporte journal no jala :,c 
rep -id~:~vda1 -path~:~/home/bryan/Escritorio/diciembre/REPORTEJournal.png -name~:~journaling
rep -id~:~vda1 -path~:~/home/bryan/Escritorio/diciembre/REPORTESuperBloqueInodo.png -name~:~inode
rep -id~:~vda1 -path~:~/home/bryan/Escritorio/diciembre/REPORTEBloque.png -name~:~block
rep -id~:~vda1 -path~:~/home/bryan/Escritorio/diciembre/REPORTESuperBloqueInodo.txt -name~:~bm_inode
rep -id~:~vda1 -path~:~/home/bryan/Escritorio/diciembre/REPORTEBitMapBloque.txt -name~:~bm_block



exec -path~:~/home/bryan/Escritorio/Proyecto_1/Proyecto1/Proyecto1MIAVD2021/archivo.sh

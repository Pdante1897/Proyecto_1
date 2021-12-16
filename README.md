# Proyecto_1
Proyecto 1 MIA Vacaciones Diciembre 2021 

Developer: Bryan Gerardo Paez Morales 
Carnet: 201700945

# Sistema De Archivos EXT2 y EXT3

La solucion simula un sistema de archivos, esta recibira comandos escritos en consola, recibiendo un comando a la vez sin distinguir entre mayusculas y minusculas, y es capaz de ejecutar un script contenido en un archivo con extension .sh

## Ejecucion

Al haber descargado el repositorio, debera abrir una terminal, luego debera acceder a la direccion de la carpeta llamada "build-Proyecto1-Desktop-Debug" (sin comillas) desde la terminal, escribir "./Proyecto1" (sin comillas) y pulsar enter.

## Comandos
- MKDISK: Crea un archivo binario, que simula un disco duro.
- RMKDISK: Borrara el archivo binario especificado.
- FDISK: Administra las operaciones que se realizara en una particion como crear, eliminar o quitar espacio.
- MOUNT: Monta una particion
- UNMOUNT: Desmonta una particion
- REP: Genera un reporte tipo imagen
- MKFS: Formatea la particion especificada en formato EXT2 o EXT3
- EXEC: Ejecuta un archivo con comandos ya escritos. 

## Demostracion

La aplicacion inicia mostrando la siguiente informacion, se requiere escribir algun comando de los ya mencionados.

![No se encuentra la imagen](https://github.com/Pdante1897/Proyecto_1/blob/main/imagenes/1.png)

De la siguiente manera se debera escribir un comando para crear un archivo binario que simulara un disco duro.

![No se encuentra la imagen](https://github.com/Pdante1897/Proyecto_1/blob/main/imagenes/2.png)

La aplicacion exige ingresar la contrasenia del administrador del equipo.
Una vez ingresada se creara el disco en la ubicacion especificada.


Se utiliza el comando Fdisk para gestionar las particiones del disco deseado. Ingresando los parametros para la creacion del mismo.

![No se encuentra la imagen](https://github.com/Pdante1897/Proyecto_1/blob/main/imagenes/3.png)

La particion ha sido creada con exito.


Para cerrar la aplicacion utilice el comando "exit" (sin comillas).

## Herramientas
- Lenguaje de programacion: C/C++ 
- IDE: Qt Creator
- SO: Linux Ubuntu
- Herramientas de analisis lexico y sintactico: Flex / Bison
- Herramienta para graficar reportes: Graphviz

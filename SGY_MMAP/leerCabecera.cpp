#include <iostream>         //entrada estandar
#include <sys/mman.h>       //para mapear el archivo
#include <sys/stat.h>       //para informacion del archivo
#include <fcntl.h>          //para abrir y cerrar archivos, así como para establecer sus permisos y atributos
#include <unistd.h>         //para trabajar con descriptores de archivo, como lseek() para obtener el tamaño del archivo

#include <string>           //cadenas de c++
#include <vector>           //arreglo
#include <fstream>          //manejo de archivos

#include <cstring>          //para copiar la informacion de un bloque de memoria a otro
#include <cstdio>           
#include <stdlib.h>         //librercia para c
#include <arpa/inet.h>

//librerias para trabajar archivos sgy
#include "convertir.h"
#include "funciones.h"
#include "cabeceraBinaria.h"
#include "trazaCabecera.h"                                                                       

void CabeceraPrincipalSGY(char *puntero);
void Trazas(char *puntero , BinaryHeader &datosTrazas);
using namespace std;

int main()
{
    /*---------------------------------------------------Variables a utilizar----------------------------------------------------*/

    int descriptorArchivo;                          //descriptor del archivo
    struct stat datosArchivo;                       //extraccion de los datos del archivo
    BinaryHeader datosTrazas;                       //objeto que contiene datos binarios sobre las trazas
    ofstream archivoTXT("datoBinArchivo.txt");      //informacion de la cabecera binaria , es salida    
    char *puntero;                                  //puntero que manejara las direcciones del mapa
    char cabeceraBinaria[400];                      //arreglo para copiar los 400bytes con la informacion para trabajar las trazas
    /*-----------------------------------------------------------------------------------------------------------------------------*/

    descriptorArchivo=open("archivos/DataSet3.sgy",O_RDONLY);        //se asigna el descriptor del archivo abierto a una variable
    fstat(descriptorArchivo,&datosArchivo);                         //usando el descript , obtiene los datos del archivo
    cout << "archivo: "<<descriptorArchivo<<endl;
    cout << "tamaño de archivo: "<<datosArchivo.st_size/1024<<"KBytes"<<endl;
    while (getchar()!='\n');

    /*-----------------------------------------------------------------------------------------------------------------------------*/

    puntero= (char*) mmap(NULL, datosArchivo.st_size, PROT_READ, MAP_PRIVATE, descriptorArchivo, 0);
    //el puntero devuelto por la funcion mmap apunta a void , asi que debe especificarse al puntero receptor que se trabajara con caracteres
    CabeceraPrincipalSGY(puntero);                          //se pasa la direccion del puntero como argumento de la funcion
    while (getchar()!='\n');

    /*-----------------------------------------------------------------------------------------------------------------------------*/

    memcpy(cabeceraBinaria,puntero+3200,400);               //se copia en el arreglo cabeceraBinaria los 400 bytes del la cabecera binaria del sgy
    datosTrazas.store(cabeceraBinaria);                     //se carga en el objeto los datos de la cabecera
    datosTrazas.write(archivoTXT);                          //funcion que escribe un archivo txt con la informacion de las trazas
    archivoTXT.close();
    cout<<"creado el archivo con los datos de la cabecera binaria de 400bytes con informacion para trabajar las trazas\n";
    while (getchar()!='\n');
    /*-----------------------------------------------------------------------------------------------------------------------------*/

    Trazas(puntero,datosTrazas);        //se pasa como parametro el puntero del archivo sgy y un objeto con la cabecera con informacion de las trazas
}

void CabeceraPrincipalSGY(char *puntero)
{
    ofstream archivoTXT("datosArchivo.txt");    //informacion de cabecera, es salida
    char cabecera[3200];                        //se destinan 3200 posiciones de tipo caracter

    memcpy(cabecera,puntero,3200);  /*copia la informacion de un espacio de memoria a otro , en este caso desde la posicion 0 a 3199 creado
    por el mmap en la direccion 0 a 3199 del vector de caracteres*/

    convertASCII(cabecera,3200);                //funcion que traduce de EBCDIC a ASCII
    write_text_header(archivoTXT,cabecera);     //funcion que imprime en un archivo txt la informacion de la cadena

    archivoTXT.close();
    cout << "creado el archivo con los datos de la cabecera principal de 3200bytes\n";
}

void Trazas(char *puntero, BinaryHeader &datosTrazas)
{
    int muestras=datosTrazas.get_num_of_samples();          //se extrae el numero de muestras por traza
    int trazasTotal= datosTrazas.get_num_of_trace();        //se extrae la cantidad de trazas
    TraceHeader traza;      //se delara una instancia de la cabecera de la traza
    int base= 3600;         //3200bytes de cabecera principal + 400 bytes de cabecera binaria
    float muestra;

    cout << "cantidad de trazas: " << trazasTotal<<endl;   //imprime el indice de la
    cout << "cantidad de muestras:" << muestras<<endl;
    for (size_t i = 0; i < trazasTotal; i++)
    {
        char *info= new char[240];         //arreglo donde se copiara la informacion de una traza
        memcpy(info,puntero+base,240);

        traza.store(info);
        cout<<"\nDatos de la cabecera de la traza "<<i+1<<"\n";
        cout << traza;
        
        memcpy(&muestra,puntero+base+248,4);
        cout << "muestra: "<<muestra<<endl;

        base=base+muestras*4+240;       //siguiente cabecera de traza
        delete[] info;
        while (getchar()!='\n');
        cout << "---------------------------------------------------\n";
    }
    
}

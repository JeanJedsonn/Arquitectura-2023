//Jeanmarco Alarcon CI 27117926
//Alejandro Cerpa 30334870
//Arquiectura del computador 07/11/2023
//Universidad de Carabobo, Facyt , Computacion , Naguanagua
//Version 2: 20/11/2023

#include <iostream>
#include <fstream>
#include <vector>
#include <cwchar>
#include <cmath>
#include "listaSimple.h"
using namespace std;


void Cache(int cacheKB,int tamanoBloq,int tamanoConjunto, string cadenaDireccion);  //funcion que maneja las conversiones necesarias
void CacheDirecta(int bloqCache, int tamanoBloq, string cadenaDireccion);                           //cache de correspondencia directa
void CacheConjuntos(int bloqCache,int tamanoBloq, int tamanoConjunto ,string cadenaDireccion);                         //cache asociativa por conjuntos
void CacheAsoCompleta(int bloqCache,int tamanoBloq, string cadenaDireccion);                       //cache completamente asociativa

const int PALABRA=4;    //valores de unidades
const int KB=1024;


int main()
{
    int tamCache,tamBloque,tamConjunto;
    string d;
    ifstream e;
    bool byte=false;

    system("clear");
    cout << "Este programa toma la conversion de datos de KB basandose en el libro, donde KB=1024bytes segun la norma IEC 80000-13\n";
    cout << "ademas, el programa toma en cuenta ciertas concideraciones para calcular la cache, revise el readme para mas informacion\n";
    cout << "presione enter para continuar\n";
    while(getchar() != '\n'); //Limpiar la cache de lectura.
    getchar(); //Pausar el programa antes de salir.
    system("clear");

    cout << "Introduzca la direccion del archivo con las direcciones de memoria: ";
    getline(cin, d); //Guardar la direccion del archivo.
    system("clear");

    e.open(d.c_str());
    if (e.fail()) //Comprobar si el archivo existe, termina la ejecucion del programa sino es así.
    {
        cout<<"No se pudo abrir el archivo."<<endl;
        cout<<"Cerrando programa."<<endl;
        getchar(); //Pausa antes de salir.
        exit(EXIT_FAILURE);
    }
    e.close();
    
    cout << "ingrese 1 para utilizar Bytes , 0 para utilizar KB\n";
    cout << "modo: ";
    cin >> byte;
    cout << "\nintroduzca el tama\xA4o de la cache\n";
    cin>>tamCache;
    cout << "introduzca el tama\xA4o de los bloques\nPalabras: ";
    cin>>tamBloque;
    cout << "introduzca el tama\xA4o de los conjuntos\nBloques: ";
    cin>>tamConjunto;

    system("clear");
    if (byte)
    {
        Cache(tamCache,tamBloque,tamConjunto,d);
    }
    else
    {
        Cache(tamCache*KB,tamBloque,tamConjunto,d);
    }
    
    while(getchar() != '\n'); //Limpiar la cache de lectura.
    getchar(); //Pausar el programa antes de salir.
    return 0;
}

void Cache(int cacheKB,int tamanoBloq,int tamanoConjunto, string cadenaDireccion)
{
    int palabrasCache;
    int bloqCache;
    float aux;

    aux=log2(tamanoBloq);   //retorna los bits necesarios
    aux=ceil(aux);          //redondea al mayor mas cercano
    tamanoBloq=pow(2,aux);  //se le ajusta al tamano de bloque la cantidad mayor mas cercana

    palabrasCache=cacheKB/PALABRA;
    bloqCache=palabrasCache/tamanoBloq;
    cout << palabrasCache << " Palabras de cache\n";
    cout << bloqCache << " Bloques de cache\n";

    CacheDirecta(bloqCache, tamanoBloq, cadenaDireccion);
    CacheAsoCompleta(bloqCache, tamanoBloq,cadenaDireccion);
    CacheConjuntos(bloqCache,tamanoBloq,tamanoConjunto, cadenaDireccion);
}

void CacheDirecta(int bloqCache,int tamanoBloq,string cadenaDireccion)
{
    vector<int>cache(bloqCache);
    int despBloque=log2(tamanoBloq);    //el numero retornado la cantidad de bits necesitados para el desplazamiento
    int indice;
    int etiqueta;
    int i;
    int instruccion;
    int error=0;

    ifstream soli;                      //variable donde se cargara el archivo de solititudes
    ofstream log;                       //variable del archivo donde se registrara el log de ejecucion
    soli.open(cadenaDireccion.c_str());
    log.open("logDirecta.txt");

    //funcion que elimina todos los datos de los bloques
    for (i = 0; i < bloqCache; i++)
    {
        cache[i]=-1;
    }
    
    while (soli>>instruccion)
    {
        etiqueta=instruccion>>despBloque;   //se le quita a la instruccion los bits que no
        indice=etiqueta%bloqCache;
        log << "direccion: "<< instruccion << " etiqueta: " << etiqueta << " indice: " << indice;
        if (cache[indice]==etiqueta)
        {
            log << " | acierto\n";
        }
        else
        {
            log << " | error\n";
            cache[indice]=etiqueta;
            error++;
        }
    }
    cout << "\nCache directa\nnumero de fallos: "<< error << endl;
    cout << "Numero de posiciones: "<< bloqCache << endl;
    log.close();
    soli.close();
}

void CacheConjuntos(int bloqCache,int tamanoBloq, int tamanoConjunto ,string cadenaDireccion)
{
    vector< Lista<int> > vecConjunto(bloqCache/tamanoConjunto);     //se crea un vector de conjuntos , bloques/tamano del conjunto | 64/3=21 conjuntos  
    int conjuntos=bloqCache/tamanoConjunto;
    int despBloque=log2(tamanoBloq);    //el numero retornado la cantidad de bits necesitados para el desplazamiento

    int etiqueta;
    int indice;
    int indiceConjunto;
    int instruccion;
    int error=0;

    ifstream soli;
    ofstream log;
    soli.open(cadenaDireccion.c_str());
    log.open("logConjunto.txt");


    while (soli>>instruccion)
    {
        etiqueta=instruccion>>despBloque;
        indiceConjunto=etiqueta%conjuntos;                          //conjunto correspondiente
        indice=vecConjunto[indiceConjunto].buscar(etiqueta);     //indice de la instruccion dentro del conjunto

        log << "direccion: "<< instruccion << " etiqueta: " << etiqueta << " conjunto: " << indiceConjunto;
        if (indice!=-1)     //-1 indica que no existe la instruccion buscada en el bloque
        {
            log << " acierto \n";
            vecConjunto[indiceConjunto].eliminar(indice);           //se elimina en la posicion donde se encontro
            vecConjunto[indiceConjunto].insertar(1,etiqueta);    //se inserta al inicio del conjunto
        }
        else if(vecConjunto[indiceConjunto].getLongitud()<tamanoConjunto) //-1 es que no esta , se valida el espacio libre
        {
            log << " error con espacio\n";
            vecConjunto[indiceConjunto].insertar(vecConjunto[indiceConjunto].getLongitud()+1,etiqueta);
            //se inserta al final de el conjunto actual la instruccion leida
            error++;
        }
        else
        {
            //cout << "lectura incorrecta,la instruccion "<< instruccion << "no esta en cache y no hay memoria libre,eliminando al final e insertando al inicio\n";
            log << " error sin espacio\n";
            vecConjunto[indiceConjunto].eliminar(vecConjunto[indiceConjunto].getLongitud());    //se elimina la ultima posicion del conjunto actual
            vecConjunto[indiceConjunto].insertar(1,etiqueta);                                //se inserta en la primera posicion del conjuto actual la nueva instruccion
            error++;
        }
        
    }
    cout << "\nCache por Conjuntos\nnumero de fallos: "<< error << endl;
    cout << "total de conjuntos: "<<conjuntos<<endl;
    soli.close();
    log.close();

}

void CacheAsoCompleta(int bloqCache,int tamanoBloq, string cadenaDireccion)
{
    //la cache completamente asociativa no tiene un indice como tal, busca si la instruccion esta guardada y de no estarlo , la guarda
    int indice;                 //posicion donde se encontro la instruccion
    int instruccion;            //instruccion leida del archivo 

    int despBloque=log2(tamanoBloq);    //el numero retornado la cantidad de bits necesitados para el desplazamiento
    int etiqueta;
    int error=0;


    ifstream soli;
    ofstream log;
    soli.open(cadenaDireccion.c_str());
    log.open("logTotal.txt");
    Lista<int> cola;

    while (soli>>instruccion)
    {
        etiqueta=instruccion>>despBloque;
        indice=cola.buscar(etiqueta);
        log << "direccion: "<< instruccion << " etiqueta: " << etiqueta;
        if (indice!=-1)     //si no se encontro la instruccion en la cache , retornara -1 indicando el fallo
        {
            log << " acierto \n";
            cola.eliminar(indice);          //se elimino de la posicion donde se encontro
            cola.insertar(1,etiqueta);   //se inserto al inicio de la cache
        }
        else if(cola.getLongitud()<bloqCache) //-1 es que no esta , se valida el espacio libre
        {
            log << " error con espacio \n";
            cola.insertar(cola.getLongitud()+1,etiqueta);    //como hay espacio en cache , se inserta al final de la ultima instruccion
            error++;
        }
        else
        {
            log << " error sin espacio \n";
            cola.eliminar(cola.getLongitud());  //se elimina la ultima instruccion en cache
            cola.insertar(1,etiqueta);       //se inserta la nueva instruccion al principio
            error++;
        }
        
    }
    cout << "\nCache completamente Asociativa\nnumero de fallos: "<< error << endl;
    cout << "tamano final usado " << cola.getLongitud() << endl;
    soli.close();
    log.close();
}
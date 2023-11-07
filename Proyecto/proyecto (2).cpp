//Jeanmarco Alarcon CI 27117926
//Alejandro Cerpa 30334870
//Arquiectura del computador 07/11/2023
//Universidad de Carabobo, Facyt , Computacion , Naguanagua

#include <iostream>
#include <random>
#include <fstream>
#include <vector>
#include <cwchar>
#include "listaSimple.h"

using namespace std;
void Cache(int memKB,int cacheKB,int tamanoBloq, string cadenaDireccion);   //funcion que maneja las conversiones necesarias
void CacheDirecta(int bloqCache, string cadenaDireccion);                   //cache de correspondencia directa
void CacheConjuntos(int bloqCache, string cadenaDireccion);                 //cache asociativa por conjuntos
void CacheAsoCompleta(int bloqCache, string cadenaDireccion);               //cache completamente asociativa

const int PALABRA=4;    //valores de unidades
const int KB=1024;

int main()
{
    int a,b,c;
    string d;
    ifstream e;

    cout << "este programa toma la conversion de datos de KB basandose en el libro, donde KB=1024bytes segun la norma IEC 80000-13\n";
    
    cout << "Introduzca la direccion del archivo: ";
    getline(cin, d); //Guardar la direccion del archivo.
    e.open(d.c_str());
    if (e.fail()) //Comprobar si el archivo existe, termina la ejecucion del programa sino es así.
    {
        cout<<"No se pudo abrir el archivo."<<endl;
        cout<<"Cerrando programa."<<endl;
        getchar(); //Pausa antes de salir.
        exit(EXIT_FAILURE);
    }
    e.close();
    
    cout << "introduzca el tama\xA4o de la memoria\nKB: ";
    cin >>a;
    cout << "introduzca el tama\xA4o de la cache\nKB: ";
    cin>>b;
    cout << "introduzca el tama\xA4o de los bloques\nPalabras: ";
    cin>>c;

    

    Cache(a,b,c, d);
    
    while(getchar() != '\n'); //Limpiar la cache de lectura.
    getchar(); //Pausar el programa antes de salir.
    return 0;
}

void Cache(int memKB,int cacheKB,int tamanoBloq, string cadenaDireccion)
{
    int palabrasMem=(memKB*KB)/PALABRA;         //tama\xA4o total de la memoria en palabras
    int palabrasCache=(cacheKB*KB)/PALABRA;     //tama\xA4o total de la cache en palabras
    int bloqMem=palabrasMem/tamanoBloq;         //numero total de bloques de memoria
    int bloqCache=palabrasCache/tamanoBloq;     //numero total de bloques de cache


    cout << palabrasMem <<"Palabras de Memoria\n";
    cout << palabrasCache << "Palabras de cache\n";
    cout << bloqMem <<"Bloques de Memoria\n";
    cout << bloqCache << "Bloques de cache\n";

    // random_device motor;                                //generador de numeros aleatorios, hay que quitarlo en
    // default_random_engine generador(motor());           //caso de que se quiera ingresar un archivo preconfigurado
    // uniform_int_distribution<int> rango(0,bloqMem-1);   //bloqMem el el numero total de 1 a n, necesitamos que sea de 0 a n-1

    // i solicitudes;
    // solicitudes.open("soli.txt");                       //archivo donde se contendran los numeros generador
    // for (size_t i = 0; i < bloqMem; i++)
    // {
    //     solicitudes << rango(generador)<<"\n";
    // }
    // solicitudes.close();

    CacheDirecta(bloqCache, cadenaDireccion);
    CacheAsoCompleta(bloqCache, cadenaDireccion);
    CacheConjuntos(bloqCache, cadenaDireccion);
}

void CacheDirecta(int bloqCache, string cadenaDireccion)
{
    vector<int> bloques(bloqCache);     //los bloques se representan con un vector al ser fijos
    int instruccion;                    //instruccion leida del archivo
    int indice;                         //indice en cache resultante de aplicar mod
    int error=0;                        //veces que fallo la lectura, incluidos bloques vacios
    ifstream soli;                      //variable donde se cargara el archivo de solititudes
    ofstream log;                       //variable del archivo donde se registrara el log de ejecucion
    soli.open(cadenaDireccion.c_str());
    log.open("logDirecta.txt");

    while (soli>>instruccion)           //se ejecuta mientras se lean correctamente las instrucciones
    {
        indice=instruccion%bloqCache;
        if (bloques[indice]==instruccion)       //lectura correcta
        {

            log << "lectura correcta de la instruccion "<<instruccion<<"en el indice "<<indice <<"\n"; 
        }
        else
        {
            log << "lectura incorrecta de la instruccion "<<instruccion<<"en el indice "<<indice <<"\n"; 
            bloques[indice]=instruccion;        //fallo la lectura ,se asigna la nueva instruccion en el indice 
            error++;
        }
    }
    cout << "\nCache directa\nnumero de fallos: "<< error << endl;
    log.close();
    soli.close();
}

void CacheConjuntos(int bloqCache, string cadenaDireccion)
{
    int tamConjunto;                    //cantidad de bloques que conforman cada conjunto
    int conjuntos;                      //numero total de conjuntos calculado con el total de bloques entre el tama\xA4o de los conjuntos
    int indice;                         //indice del conjunto , calculado con el modulo de la instruccion y el numero de conjuntos
    int instruccion;                    //instruccion leida del archivo
    int error=0;                        //total de fallos al leer
    int indiceConjunto;                 //indice de la ubicacion de la instruccion dentro del conjunto

    cout << "\ntama\xA4o del conjunto: ";
    cin>>tamConjunto;
    conjuntos=bloqCache/tamConjunto;

    vector<Lista<int> > vecConjunto(conjuntos);     //se crea un vector de conjuntos

    ifstream soli;
    ofstream log;
    soli.open(cadenaDireccion.c_str());
    log.open("logConjunto.txt");

    while (soli>>instruccion)
    {
        indiceConjunto=instruccion%conjuntos;                       //conjunto correspondiente
        indice=vecConjunto[indiceConjunto].buscar(instruccion);     //indice de la instruccion dentro del conjunto
        
        if (indice!=-1)     //-1 indica que no existe la instruccion buscada en el bloque
        {
            log << "lectura correcta de la instruccion "<<instruccion<< "en el indice "<<indice<<"del conjunto "<<indiceConjunto<<"se insertara al principio\n";
            vecConjunto[indiceConjunto].eliminar(indice);           //se elimina en la posicion donde se encontro
            vecConjunto[indiceConjunto].insertar(1,instruccion);    //se inserta al inicio del conjunto
        }
        else if(vecConjunto[indiceConjunto].getLongitud()<tamConjunto) //-1 es que no esta , se valida el espacio libre
        {
            log << "lectura incorrecta de la instruccion "<<instruccion<< "en el conjunto "<< indiceConjunto << " ,la cache tiene bloques libres , insertando al final\n";
            vecConjunto[indiceConjunto].insertar(vecConjunto[indiceConjunto].getLongitud()+1,instruccion);
            //se inserta al final de el conjunto actual la instruccion leida
            error++;
        }
        else
        {
            //cout << "lectura incorrecta,la instruccion "<< instruccion << "no esta en cache y no hay memoria libre,eliminando al final e insertando al inicio\n";
            log << "lectura incorrecta de la instruccion "<<instruccion<< "en el conjunto "<<indiceConjunto<<" ,la cache esta llena , eliminando la ultima posicion e insertando al principio\n";
            vecConjunto[indiceConjunto].eliminar(vecConjunto[indiceConjunto].getLongitud());    //se elimina la ultima posicion del conjunto actual
            vecConjunto[indiceConjunto].insertar(1,instruccion);                                //se inserta en la primera posicion del conjuto actual la nueva instruccion
            error++;
        }
        
    }
    cout << "\nCache por Conjuntos\nnumero de fallos: "<< error << endl;
    cout << "total de conjuntos: "<<conjuntos<<endl;
    soli.close();
    log.close();
}



void CacheAsoCompleta(int bloqCache, string cadenaDireccion)
{
    //la cache completamente asociativa no tiene un indice como tal, busca si la instruccion esta guardada y de no estarlo , la guarda
    int indice;                 //posicion donde se encontro la instruccion
    int instruccion;            //instruccion leida del archivo 
    int error=0;

    ifstream soli;
    ofstream log;
    soli.open(cadenaDireccion.c_str());
    log.open("logTotal.txt");
    Lista<int> cola;

    while (soli>>instruccion)
    {
        
        indice=cola.buscar(instruccion);
        
        if (indice!=-1)     //si no se encontro la instruccion en la cache , retornara -1 indicando el fallo
        {
            log << "lectura correcta de la instruccion "<<instruccion<< "en el indice "<<indice<<",se insertara al principio\n";
            cola.eliminar(indice);          //se elimino de la posicion donde se encontro
            cola.insertar(1,instruccion);   //se inserto al inicio de la cache
        }
        else if(cola.getLongitud()<bloqCache) //-1 es que no esta , se valida el espacio libre
        {
            //cout << "lectura incorrecta,la instruccion "<< instruccion << " no esta en cache y hay espacio, insertando al final\n";
            log << "lectura incorrecta de la instruccion "<<instruccion<< "la cache tiene bloques libres , insertando al final\n";
            cola.insertar(cola.getLongitud()+1,instruccion);    //como hay espacio en cache , se inserta al final de la ultima instruccion
            error++;
        }
        else
        {
            //cout << "lectura incorrecta,la instruccion "<< instruccion << "no esta en cache y no hay memoria libre,eliminando al final e insertando al inicio\n";
            log << "lectura incorrecta de la instruccion "<<instruccion<< "la cache esta llena , eliminando la ultima posicion e insertando al principio\n";
            cola.eliminar(cola.getLongitud());  //se elimina la ultima instruccion en cache
            cola.insertar(1,instruccion);       //se inserta la nueva instruccion al principio
            error++;
        }
        
    }
    cout << "\nCache completamente Asociativa\nnumero de fallos: "<< error << endl;
    cout << "tamano" << cola.getLongitud() << endl;
    soli.close();
    log.close();
}
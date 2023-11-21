//Jeanmarco Alarcon CI 27117926
//Programacion 2 07/11/2023 , Naguanagua

#ifndef LISTA
#define LISTA

//---------------------------------------------------------------------------------------------------------------//
//                                               Clase Nodo                                                      //
//---------------------------------------------------------------------------------------------------------------//

template <class Tipo1>      //todas las asignaciones hechas en esta plantilla se vuelven del tipo de dato mas conveniente automaticamente
class Nodo{
    public:                 //esta publico por que la libreria lista necesitara acceder directamente y que esta no hereda de nodo,
        Tipo1 valor;        //gracias a eso es posible trabajar con objetos complejos como otra lista
        Nodo* nodoSig;      
        
        //constructores:
            Nodo();                                 //constructor por defecto
            Nodo(Tipo1 &a);                         //constructor con dato
            Nodo(Nodo<Tipo1>* in ,Tipo1 &a);        //constructor con nodo como copia

        //se implementaron consultores y modificadores pese a ser publico para facilitar la lectura del codigo
        //modificadores:
            Nodo<Tipo1>* getSig();                  
            Tipo1 getValor();

        //consultores:
            void setSig(Nodo<Tipo1>* sig);          
            void setValor(Tipo1 &a);
};

//---------------------------------------------------------------------------------------------------------------//
//                                               Clase Lista                                                     //
//---------------------------------------------------------------------------------------------------------------//

template <class Tipo1>
class Lista{
    private:
        int longitud;                               //la longitud solo puede ser un entero
        Nodo<Tipo1>* nodoAncla;                     //los nodos siempre son nodos , aun que contengan otras cosas
        Nodo<Tipo1>* nodoFinal;

        //metodo privado
        Nodo<Tipo1>* iterar(int pos);
    public:

        //constructores:
            Lista();
            Lista(Lista<Tipo1> &in);                        //recibe la direccion de la lista

        //consultores:            
            bool esVacia();
            int getLongitud();
            Tipo1& consultar(int pos);                      //si se fuese una lista de listas , debe retornarse la referencia al objeto
            int buscar(Tipo1 objetivo);
           
        //modificadores:
            void invertir();
            void copiar(Lista<Tipo1> &in);                  //recibe la direccion de la lista
            void modificar(int pos,Tipo1 in);
            void insertar(int pos,Tipo1 dato);              //inserta valores constantes , como '1'
            void insertarR(int pos,Tipo1 &dato);            //inserta valores por referencia , para evitar llamadas al constructor
            void intercambiar(int pos1,int pos2);

        //destructores:
            ~Lista();
            void eliminar(int pos);
            void vaciar();

        //Operadores:
            Tipo1 &operator [] (int pos);                   //retorna la referencia de la posicion indicada
            void  operator = (Lista<Tipo1> &listIn);          // recibe el dato por referencia en caso de ser una clase u otra lista
            bool  operator == (Lista<Tipo1> &listIn);
};

//---------------------------------------------------------------------------------------------------------------//
//                                               Metodos Nodo                                                    //
//---------------------------------------------------------------------------------------------------------------//

template <class Tipo1>
Nodo<Tipo1>::Nodo()
{                                     //constructor por defecto
    this->nodoSig=NULL;
}      

template <class Tipo1>
Nodo<Tipo1>::Nodo(Tipo1 &a)
{                               //constructor por defecto
    this->valor=a;
    this->nodoSig=NULL;
}

template <class Tipo1>
Nodo<Tipo1>::Nodo(Nodo<Tipo1>* in ,Tipo1 &a) //constructor para crear una copia del nodo ()     
{                   
    this->valor=a;
    this->nodoSig=NULL;
}

//-------------------------------------------------//
//                  Modificadores                  //
//-------------------------------------------------//

template <class Tipo1>
void Nodo<Tipo1>::setSig(Nodo<Tipo1>* sig)          //se recibe la direccion en memoria del objeto
{  
    this->nodoSig=sig;                              //nodoDer se vuelve un puntero al recibir la direccion de der
}

template <class Tipo1>
void Nodo<Tipo1>::setValor(Tipo1 &a)
{
    this->valor=a;
}

//-------------------------------------------------//
//                   Consultores                   //
//-------------------------------------------------//

template <class Tipo1>
Nodo<Tipo1>* Nodo<Tipo1>::getSig()          //asigna que el retorno de la funcion sera un puntero del objeto
{           
    return this->nodoSig;                   //el objeto retorna su propio parametro
}

template <class Tipo1>
Tipo1 Nodo<Tipo1>::getValor()
{
    return this->valor;
}

//---------------------------------------------------------------------------------------------------------------//
//                                               Metodos Lista                                                   //
//---------------------------------------------------------------------------------------------------------------//

//-------------------------------------------------//
//                  Constructores                  //
//-------------------------------------------------//

template <class Tipo1>  //hace falta volver a indicar el template
Lista<Tipo1>::Lista()   //se debe de poner el tipo por delante de la biblioteca en uso
{
    this->longitud=0;
    this->nodoAncla=NULL;
    this->nodoFinal=NULL;
}

template <class Tipo1>
Lista<Tipo1>::Lista(Lista<Tipo1> &in)       //se reutilizo el codio de "copiar" para evitar llamadas en bucle al copiar en una funcion
{
    int i;
    int tope=in.getLongitud();              //asigna como maximo de iteracion la longitud de la lista entrante
    Tipo1 aux;

    if (longitud>0)                         //si la lista no esta vacia , la vacia
    {
        this->vaciar();
    }
    for (i = 1; i < tope+1; i++)            
    {
        
        aux=in.consultar(i);
        insertar(i,aux);       //inserta en la posicion i el valor en la posicion i de la lista entrnte
    }
}

//-------------------------------------------------//
//                   Consultores                   //
//-------------------------------------------------//

template<class Tipo1>
bool Lista<Tipo1>::esVacia()
{
    return this->longitud==0;
}

template <class Tipo1>
int Lista<Tipo1>::getLongitud()
{
    return this->longitud;
}

template <class Tipo1>
Tipo1& Lista<Tipo1>::consultar(int pos)
{
    Nodo<Tipo1>* aux;                   //aux recorre los nodos, por defecto vale 0

    if (pos>0 && pos<=this->longitud)   //se verifica que la consulta este en rango
    {
        if (pos==1)
        {
            aux=this->nodoAncla;
        }
        else if(pos==longitud)
        {
            aux=this->nodoFinal;
        }
        else
        {
            aux=iterar(pos);
        }
    }
    return aux->valor;                  //se retorna una referencia al valor contenido en el nodo
}

template <class Tipo1>
int Lista<Tipo1>::buscar(Tipo1 objetivo)
{
    int i=0;
    int encontrado=1;
    Nodo<Tipo1>* aux;
    aux=this->nodoAncla;

    while (encontrado && aux!=NULL)     //recordar que si aux=NULL , se salio del rango de la lista
    {
        i++;                            //se incrementa al inicio , pues
        if (aux->getValor()==objetivo)  //se consola si el nodo actual tiene el valor que se esta buscando
        {
            encontrado=0;               //se asigna 0 pero negar la bandera
        }
        else
        {
            aux=aux->getSig();          //si no se encontro , se avanza al prox nodo , si es NULL entonces i no iterara
        }
    }
    if (encontrado)                     //si encontrado==1 , no se encontro nada
    {   
        i=-1;                           //rango invalido
    }
    return i;
}
//-------------------------------------------------//
//                 Modificadores                   //
//-------------------------------------------------//
template<class Tipo1>
void Lista<Tipo1>::invertir()
{
    Nodo<Tipo1>* aux;
    Nodo<Tipo1>* nodo1;
    Nodo<Tipo1>* nodo2;
    aux=NULL;
    nodo1=this->nodoAncla;

    while (nodo1!=NULL)
    {
        nodo2=nodo1->getSig();
        nodo1->setSig(aux);
        aux=nodo1;
        nodo1=nodo2;
    }
    aux=nodoFinal;
    nodoFinal=nodoAncla;
    nodoAncla=aux;
}

template<class Tipo1>
void Lista<Tipo1>::copiar(Lista<Tipo1> &in)
{
    int i;
    int tope=in.getLongitud();             //asigna como maximo de iteracion la longitud de la lista entrante
    Tipo1 aux;

    if (longitud>0)                         //si la lista no esta vacia , la vacia
    {
        this->vaciar();
    }
    for (i = 1; i < tope+1; i++)            
    {
        
        aux=in.consultar(i);
        insertar(i,aux);       //inserta en la posicion i el valor en la posicion i de la lista entrnte
    }
}

template <class Tipo1>
void Lista<Tipo1>::modificar(int pos,Tipo1 in)
{
    Nodo<Tipo1>* aux;
    aux=iterar(pos);
    aux->setValor(in);
}

template <class Tipo1>
void Lista<Tipo1>::insertarR(int pos,Tipo1 &dato)
{
    int tope=this->longitud+1;
    if(pos>0 && pos<=this->longitud+1)
    {
        int tope=this->longitud+1;
        Nodo<Tipo1>* aux;       //para recibir la direccion del nodo entre 2 y n
        Nodo<Tipo1>* nuevo=new Nodo<Tipo1>;
        nuevo->setValor(dato);

        if (pos==1)
        {
            nuevo->setSig(nodoAncla);   //se asigna al nodo nuevo la direccion del primer nodo
            nodoAncla=nuevo;            //se asigna al apuntador del nodo ancla la direccion del nuevo nodo
            if (this->longitud==0)            //si la longitud es 0 , ambos punteros estaban nulos
            {
                nodoFinal=nuevo;
            }
        }
        else if(pos==this->longitud+1)    
        {
            nodoFinal->setSig(nuevo);     //se asigna al ultimo nodo el nuevo nodo
            nodoFinal=nuevo;              //se asigna al puntero la direccion del nuevo nodo
        }
        else
        {
            aux=iterar(pos-1);              //puntero anterior a la pos que se esta buscando
            nuevo->setSig(aux->getSig());    //se asigno al nodo insertado la direccion del nodo que esta en la posicion indicada
            aux->setSig(nuevo);              //al nodo que esta en pos-1 se le asigno la direccion del nodo insertado
        }
        this->longitud=tope;
    }
}

template <class Tipo1>
void Lista<Tipo1>::insertar(int pos,Tipo1 dato)
{
    int tope=this->longitud+1;
    if(pos>0 && pos<=this->longitud+1)
    {
        int tope=this->longitud+1;
        Nodo<Tipo1>* aux;       //para recibir la direccion del nodo entre 2 y n
        Nodo<Tipo1>* nuevo=new Nodo<Tipo1>;
        nuevo->setValor(dato);

        if (pos==1)
        {
            nuevo->setSig(nodoAncla);   //se asigna al nodo nuevo la direccion del primer nodo
            nodoAncla=nuevo;            //se asigna al apuntador del nodo ancla la direccion del nuevo nodo
            if (this->longitud==0)            //si la longitud es 0 , ambos punteros estaban nulos
            {
                nodoFinal=nuevo;
            }
        }
        else if(pos==this->longitud+1)    
        {
            nodoFinal->setSig(nuevo);     //se asigna al ultimo nodo el nuevo nodo
            nodoFinal=nuevo;              //se asigna al puntero la direccion del nuevo nodo
        }
        else
        {
            aux=iterar(pos-1);              //puntero anterior a la pos que se esta buscando
            nuevo->setSig(aux->getSig());    //se asigno al nodo insertado la direccion del nodo que esta en la posicion indicada
            aux->setSig(nuevo);              //al nodo que esta en pos-1 se le asigno la direccion del nodo insertado
        }
        this->longitud=tope;
    }
}

template<class Tipo1>
void Lista<Tipo1>::intercambiar(int pos1,int pos2)    //no se usa a iterar() para no recorrer nodos ya visiados
{
    //FIX: CAMBIAR LOS NODOS ; NO REASIGNES EL VALOR
    Nodo<Tipo1>* nodo1;                 //direccion del nodo en la pos1
    Nodo<Tipo1>* nodo2;                 //direccion del nodo en la pos2
    Nodo<Tipo1>* aux1;                  //puntero para almacenar la direccion en el intercambio
    Nodo<Tipo1>* aux2;                  //puntero para almacenar la direccion en el intercambio
    int i=0;
    nodo2=nodoAncla;                    //primer nodo
    nodo1=nodoAncla;                    //primer nodo (se dejo listo en caso de necesiarse)
    

    if ( pos1<=this-> longitud && pos2<=this-> longitud && pos1!=pos2)
    {
        if(pos1>pos2)   //se intercambian las posiciones
        {
            i=pos2;
            pos2=pos1;
            pos1=i;
        }

        if (pos1==1 && pos2 == this->longitud)      //caso 1: extremos
        {
            nodo1=this->nodoAncla->getSig();              //2do nodo
            for (i = 2; i < this->longitud; i++)    // i<lontigud es el anterior
            {
                nodo2=nodo2->getSig();
            }

            nodoAncla->setSig(NULL);                //el primer nodo no apunta a nada 
            nodo2->setSig(NULL);                    //el penultimo nodo no apunta a nada

            aux1=nodoFinal;                          //se copia el ultimo nodo
            nodoFinal=this->nodoAncla;                    //el ultimo nodo es el primer nodo, que no apunta a nada
            nodo2->setSig(this->nodoFinal);               //el penultimo nodo apunta al nodo final
            
            nodoAncla=aux1;                          //el primer nodo es la copia del nodo final, que no apunta a nada
            aux1->setSig(nodo1);                     //el primer nodo apunta al segundo
        }
        else if(pos1==1)                            //caso 2: extremo izquierdo 
        {
            for (i = 2; i < pos2; i++)              //nodo anterior al buscado
            {
                nodo2=nodo2->getSig();
            }

            aux1=nodo2;                 //nodo n-1
            nodo2=nodo2->getSig();      //nodo n
            aux2=nodo2->getSig();       //nodo n+1

            nodo2->setSig( nodo1->getSig() );       //el nodo n apunta al 2do nodo
            this->nodoAncla=nodo2;                  //el primer nodo es el nodo n

            aux1->setSig(nodo1);                    //el nodo n-1 apunta al 1er nodo
            nodo1->setSig(aux2);                    //el 1er nodo apunta al nodo n+1
            if(pos2==2)
            {
                nodo2->setSig(nodo1);               //cuando pos2==2 , el nodo2 queda apuntandose a si mismo y no a al 1er nodo
            }

        }
        else if(pos2==this->longitud)               //caso 3: extremo derecho
        {
            for (i = 2; i < pos1; i++)              //nodo anterior al buscado
            {
                nodo1=nodo1->getSig();
            }
            nodo2=nodo1;                            //al nodo2 se le asigna la ultima posicion del nodo1
            for ( i = i; i < this->longitud ; i++)
            {
                nodo2=nodo2->getSig();              //se itera hasta el nodo anterior al ultimo
            }

            aux1=nodo1;                 //nodo n-1
            nodo1=nodo1->getSig();      //nodo n
            aux2=nodo1->getSig();       //nodo n+1

            nodo2->setSig(nodo1);       //se inserta como nodo final el nodo n
            nodo1->setSig(NULL);        //el ultimo nodo no apunta a nada

            if(pos1==this->longitud-1)
            {
                //intercambiar n y n-1 causa un error
                this->nodoFinal->setSig(aux1->getSig());
                aux1->setSig(nodoFinal);
            }
            else
            {
                //intercambio entre n  y un nodo del 2 al n-2
                aux1->setSig(nodoFinal);    //lo que era el ultimo nodo es apuntado por n-1 
                this->nodoFinal->setSig(aux2);    //lo que era el ultimo nodo ahora apunta al nodo n+1
            }

            this->nodoFinal=nodo1;            //el puntero al final de la lista se asigna al nodo n
        }
        else
        {
            for (i = 2; i < pos1; i++)              //nodo anterior al buscado
            {
                nodo1=nodo1->getSig();
            }
            nodo2=nodo1;                            //al nodo2 se le asigna la ultima posicion del nodo1
            for ( i = i; i < pos2; i++)
            {
                nodo2=nodo2->getSig();              //se itera hasta el nodo anterior al ultimo
            }
            std::cout << "entre\n";
            aux1=nodo1;                             //nodo anterior a n
            nodo1=nodo1->getSig();                        //nodo m
            aux2=nodo2;                             //nodo anterior a n
            nodo2=nodo2->getSig();                        //nodo n

            aux1->setSig(nodo1->getSig());
            nodo1->setSig(nodo2->getSig());
            if (pos1==pos2-1)                           //cuando los nodos estan juntos , solo falta un paso
            {
                nodo2->setSig(nodo1);                   //el nodo n apunta al nodo m
            }
            else
            {
                aux2->setSig(nodo1);                    //el nodo n-1 apunta al nodo n
                nodo2->setSig(aux1->getSig());          //el nodo n apunta al nodo m+1
                aux1->setSig(nodo2);                    //el nodo m-1 al nodo n
            }
        }
    }
}

//-------------------------------------------------//
//                  destructores                   //
//-------------------------------------------------//
template <class Tipo1>
void Lista<Tipo1>::vaciar()
{
    Nodo<Tipo1>* borrar;        //el nodo que sera borrado en la iteracion actual
    Nodo<Tipo1>* aux;           //punto con el que se recorre la cadena de nodos
    aux=nodoAncla;              //se apunta al primer nodo
    this->nodoFinal=NULL;             //no se usara durante la iteracion , se asigna null primero que todo

    while (aux!=NULL)           //una vez que aux sea null , se habra recorrido toda la lista
    {
        borrar=aux;             //el nodo que borrare sera al que avance en la iteracion anterior   
        aux=aux->getSig();      //avanzo al nodo siguiente antes de borrar el actual
        borrar->setSig(NULL);   //asigno al nodo una direccion vacoa
        delete borrar;          //boro la direccion a la que apunta el puntero
    }
    this->longitud=0;
    this->nodoAncla=NULL;             //asigno null por seguridad     
}

template <class Tipo1>
Lista<Tipo1>::~Lista()
{
    this->vaciar();
}

template <class Tipo1>
void Lista<Tipo1>::eliminar(int pos)  
{
    if (pos>0 && pos <= longitud)
    {
        Nodo<Tipo1>* aux1;
        Nodo<Tipo1>* aux2;
        if (pos==1)
        {
            if (longitud==1)    //si la longitud es 1 , nodoFinal no apuntara a nada
            {
                nodoFinal=NULL;
            }
            
            aux1=nodoAncla->getSig();       //segundo nodo de la lista
            this->nodoAncla->setSig(NULL);        //apunto a una direccion vacia
            delete this->nodoAncla;               //borro la direccion a la que apunta el puntero al primer nodo
            this->nodoAncla=aux1;                 //asigno al puntero la direccion del 2do nodo
        }
        else if(pos==longitud)
        {
            aux1=iterar(pos-1);               //retorna el penultimo nodo
            this->nodoFinal=aux1;                         //asigno la direccion del penultimo nodo
            aux1=aux1->getSig();                    //avanza al ultimo nodo,el cual ya apunta a null
            delete aux1;                            //borro el ultimo nodo
            this->nodoFinal->setSig(NULL);                //se asigna null al nuevo nodo final
        }
        else
        {
            aux1=iterar(pos-1);             //retorna el nodo anterior
            aux2=aux1->getSig();            //retorna el nodo que se desea eliminar
            aux1->setSig(aux2->getSig() );  //se asigna al nodo anterior el nodo que le sigue al que borrare
            aux2->setSig(NULL);             //al nodo le asigno una direccion nula
            delete aux2;                    //borro la direccion a la que apunta el puntero
        }
        this->longitud--;
    }

}

//-------------------------------------------------//
//                   Utilidades                    //
//-------------------------------------------------//

template<class Tipo1>
Nodo<Tipo1>* Lista<Tipo1>::iterar(int pos)
{
    int i;
    Nodo<Tipo1>* aux;                           //puntero para retornar la posicion
    aux=nodoAncla;                              //se apunta al primer nodo en espera de que hacer
    if(this->longitud>0 && pos<=this->longitud)
    {
        for (i = 1; i < pos; i++)               //nunca se alcanza a pos , pues el iterador de los nodos va en la posicion i+1 del ciclo actual
        {
            aux=aux->getSig();                  //cuando i sea justo el penultimo , retornara la direccion que se esta buscando
        }
    }
    return aux;
}

//-------------------------------------------------//
//                   Operadores                    //
//-------------------------------------------------//
template<class Tipo1>
Tipo1 &Lista<Tipo1>::operator [] (int pos)  //
{
    Nodo<Tipo1>* aux;
    aux=this->iterar(pos);         
    return aux->valor;
}

template<class Tipo1>
void Lista<Tipo1>::operator =(Lista<Tipo1> &listIn)
{
    this->copiar(listIn);
}

template<class Tipo1>
bool Lista<Tipo1>::operator ==(Lista<Tipo1> &listIn)
{
    Nodo<Tipo1>* aux1;
    Nodo<Tipo1>* aux2;
    bool sol=true;

    aux1=this->nodoAncla;
    aux2=listIn.nodoAncla;

    if(this->longitud==listIn.longitud)
    {
        while ( aux1!=NULL && aux2!=NULL && sol==true)
        {
            if (aux1->valor!=aux2->valor)
            {
                sol=false;
            }
            aux1=aux1->getSig();
            aux2=aux2->getSig();
        }
    }
    else
    {
        sol=false;
    }
    return sol;
}

#endif
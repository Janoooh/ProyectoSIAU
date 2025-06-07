#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_IMPUTADOS 50

/*===================================================================*/
/*===============DEFINICION DE STRUCTS PARA EL PROGRAMA==============*/
/*===================================================================*/

struct SIAU;
struct NodoCausa;
struct Causa;
struct Carpeta;
struct NodoRegistro;
struct Registro;

struct SIAU {
    struct NodoCausa *causas;
    int cantCausas;
};

struct NodoCausa {
    struct Causa *datosCausa;
    struct NodoCausa *izq, *der;
};

struct Causa {
    char *ruc;
    struct Registro *denunciaInicial;
    struct Carpeta *investigacion;
    int estado;
};

struct Carpeta {
    struct NodoRegistro *registros[5];
    char **imputados;
    int cantImputados;
};

struct NodoRegistro {
    struct Registro *dataRegistro;
    struct NodoRegistro *sig;
};

struct Registro {
    int id;
    char *involucrado;
    char *fechaRegistro;
    char *detalle;
    int tipo;
};

/*===================================================================*/

/*===================================================================*/
/*=============DEFINICION DE FUNCIONES PARA EL PROGRAMA==============*/
/*===================================================================*/

void leerOpcion(int *opcion, int limInf, int limSup);
void limpiarConsola();

void agregarDatos(struct SIAU * siau);
void listarDatos(struct SIAU * siau);
void buscarDatos(struct SIAU * siau);
void modificarDatos(struct SIAU * siau);
void borrarDatos(struct SIAU * siau);
void otrasOpciones(struct SIAU * siau);

char * leerCadena(char *mensaje);

int agregarCausa(struct NodoCausa **raiz, struct Causa *nuevaCausa);
struct Causa * crearCausa();
struct Registro * crearRegistro(int tipoRegistro);
int agregarRegistro(struct SIAU *siau, struct Registro *nuevo);
void codigoAgregarRegistroMenu(struct SIAU *siau, int tipoRegistro);
int agregarImputado(struct SIAU *siau);


void imprimirImputado(char *imputado);
void imprimirRegistro(struct Registro *registro);
void imprimirCausa(struct Causa *causa);
void recorrerImputados(char **imputados, int cantidad); /*Cambiar por recorrerImprimirImputados en vez de recorrer?*/
void recorrerRegistrosParaImprimirlos(struct NodoRegistro *listaRegistros);
void recorrerCausasParaImprimirlas(struct NodoCausa *actual);
void listarCarpetas(struct Carpeta *carpeta, int parametro);
void  mostrarCausasPorEstado(struct NodoCausa * raiz, int estado);/*Func Otras Opc.*/
void mostrarResolucionesJudicialesDeImputado(struct Carpeta * carpeta, const char * imputadoBuscado);/*Func Otras Opc.*/

struct Causa *buscarCausaPorRuc(struct NodoCausa *causas, char *ruc);
struct Registro *buscarRegistroPorId(struct Carpeta * carpeta, int tipo, int idRegistro);
char* buscarImputadoEnCarpeta(struct Carpeta * carpeta, const char * rutBuscado);

void modificarCausa(struct NodoCausa * causas);
int modificarEstadoCausa();
void validarModificacionRegistros(struct SIAU * siau, int tipo);
void modificarTipoRegistro(struct Registro *registro, struct Carpeta *carpeta);
void modificarRegistro(struct Registro *registro, struct Carpeta *carpeta);


void eliminarRegistro(struct NodoRegistro **head, int id, char *nombreTipoRegistro);
void eliminarRegistroEnArreglo(struct SIAU *siau, int tipoRegistro, char *nombreTipoRegistro);
struct NodoCausa* encontrarReemplazo(struct NodoCausa* nodo);
struct NodoCausa *eliminarCausa(struct NodoCausa *raiz, char *rucCausa, int *eliminacionExitosa);
void eliminarCausaEnArbol(struct SIAU *siau);
void eliminarImputado(char **imputados, int *cantImputados, char *rutImputado);
void eliminarImputadoDeCausa(struct SIAU *siau);



/*----------------------------------------------------------------------------------------------------------*/
/*-------------------------- FUNCIONES RELACIONADA CON AGREGAR Y CREAR DATOS -------------------------------*/
/*----------------------------------------------------------------------------------------------------------*/

/*Funcion leerCadena: Encargada de leer una cadena de texto ingresada por el usuario. Lo leido
se almacena en un buffer temporal, para despues traspasarlo a una nueva cadena dinamica, reservandole
a esta una cantidad exacta de memoria. Recibe por parametro un mensaje que indica al usuario que debe ingresar,
y retorna un puntero a una cadena dinamica.*/
char * leerCadena(char *mensaje){
    char buffer[101];
    char *cadRetorno = NULL;
    int nCaracteres;

    /*Mostramos el mensaje indicatorio de que debe ingresar el usuario.*/
    printf("%s",mensaje);
    scanf(" %[^\n]",buffer);/*Leemos la cadena de texto.*/
    printf("\n");

    nCaracteres = (int)strlen(buffer);/*Obtenemos la longitud de la cadena leida*/
    /*Reservamos memoria para la nueva cadena, incluyendo espacio para el caracter nulo*/
    cadRetorno = (char *) malloc((nCaracteres + 1) * sizeof(char));

    strcpy(cadRetorno , buffer);/*Copiamos la cadena del buffer a la nueva cadena dinamica.*/

    return cadRetorno;/*Retornamos un puntero a la cadena dinamica que se creo y lleno.*/
}
/*Funcion crearCausa: Se encarga de crear una nueva causa, implicando en ello la lectura
 y asignacion default de todos los datos que contiene el struct Causa. Retorna un puntero
 a la Causa creada, rellena con los datos ingresados por el usuario.*/
struct Causa * crearCausa(){
    struct Causa *nueva = NULL;
    int x;

    /*Primero reservaremos memoria, y colocaremos los valores default para ciertas
    variables.*/

    /*Reservamos memoria para la causa.*/
    nueva = (struct Causa *) malloc(sizeof(struct Causa));

    /*Reservamos memoria para el registro de la denuncia inicial de la causa.*/
    nueva->denunciaInicial = (struct Registro *) malloc(sizeof(struct Registro));

    /*La denuncia inicial de una causa siempre tendra reservada la id 0*/
    nueva->denunciaInicial->id = 0;

    /*Como se sabe que es una denuncia, se le coloca tipo 0 al registro*/
    nueva->denunciaInicial->tipo = 0;

    /*Reservamos memoria para la carpeta investigativa de la Causa.*/
    nueva->investigacion = (struct Carpeta *) malloc(sizeof(struct Carpeta));

    /*Apuntamos los punteros del arreglo de listas a NULL, para que quede limpio.*/
    for (x = 0; x < 5; x++) nueva->investigacion->registros[x] = NULL;

    /*Reservamos memoria para el arreglo de imputados de la carpeta.*/
    nueva->investigacion->imputados = (char **) malloc(MAX_IMPUTADOS * sizeof(char *));

    /*Como la carpeta parte vacia, se inicializa en 0 la cantidad de imputados.*/
    nueva->investigacion->cantImputados = 0;

    /*Iniciamos la causa en estado de investigacion.*/
    nueva->estado = 0;

    /*Ahora procederemos a leer los datos del usuario y a guardarlos
    en la nueva causa que creamos*/

    /*Leemos el RUC ingresado por el usuario, y lo guardamos en la causa.*/
    nueva->ruc = leerCadena("Ingrese el RUC de la causa(Max 14 caracteres):");

    /*Leemos el RUT del denunciante, y lo guardamos en la denuncia inicial de la causa*/
    nueva->denunciaInicial->involucrado = leerCadena("Ingrese el RUT del denunciante(Max 13 caracteres):");

    /*Leemos la descripcion de la denuncia. y la guardamos en la denuncia inicial de la causa.*/
    nueva->denunciaInicial->detalle = leerCadena("Ingrese la descripcion de la denuncia(Max 100 caracteres):");


    /*Leemos la fecha de la denuncia, y la guardamos en la denuncia inicial de la causa.*/
    nueva->denunciaInicial->fechaRegistro = leerCadena("Ingrese la fecha de la denuncia(Formato DD/MM/AAAA):");

    /*Retornamos la causa creada con los datos completados.*/
    return nueva;
}

/*Funcion agregarCausa: Se encarga de agregar una nueva causa a un arbol de causas. Recibe
 por parametro el nodo raiz del arbol, y la Causa nueva, retornando un 1 si se pudo agregar
 correctamente, o un 0 si ocurrio algun error.*/
int agregarCausa(struct NodoCausa **raiz, struct Causa *nuevaCausa){
    struct NodoCausa *nuevo, *rec = NULL;
    int comparacion;

    /*Reservamos memoria para el nuevo nodo, y completamos sus campos correspondientemente.*/
    nuevo = (struct NodoCausa *)malloc(sizeof(struct NodoCausa));
    nuevo->datosCausa = nuevaCausa;
    nuevo->izq = NULL;
    nuevo->der = NULL;

    /*Si la raiz es NULL, significa que el arbol estaba vacio, sin nodos.*/
    if(*(raiz) == NULL){
        *(raiz) = nuevo;
        return 1;
    }

    rec = *(raiz);

    while(rec != NULL){
        /*Comparamos el ruc de la nueva causa con la causa almacenada en rec.
        Si la nueva causa tiene un ruc lexicograficamente mayor, se almacena valor > 0
        Si la nueva causa tiene un ruc lexicograficamente menor, se almacena valor < 0
        Si la nueva causa tiene un ruc igual, se almacena valor = 0*/
        comparacion = strcmp(nuevaCausa->ruc , rec->datosCausa->ruc );

        /*Si el ruc es igual, significa que la causa ya existe.*/
        if(comparacion == 0)return 0;

        /*Si el ruc es mayor, ir a la derecha, de lo contrario, a la izquierda*/
        if(comparacion > 0){
            /*Si a la derecha hay un NULL, agregar ahi el nuevo nodo*/
            if(rec->der == NULL){
                rec->der = nuevo;
                return 1;
            }

            rec = rec->der;

        }else{
            /*Si a la izquierda hay un NULL, agregar ahi el nuevo nodo*/
            if(rec->izq == NULL){
                rec->izq = nuevo;
                return 1;
            }

            rec = rec->izq;
        }
    }

    return 1;
}

/*Funcion crearRegistro: Encargada de crear un registro, incluyendo la lectura
de datos correspondiente para que se llenen los campos. Recibe por parametro el
tipo de registro que se desea crear. Retorna el nuevo registro creado.*/
struct Registro * crearRegistro(int tipoRegistro){
    struct Registro *nuevo = NULL;
    /*Cadenas para indicar correctamente al usuario que ingresar, segun el contexto del tipo de registro*/
    char *tiposInvolucrado[5] = {"denunciante(Max 13 caracteres):","declarante(Max 13 caracteres):","investigador(Max 13 caracteres):","involucrado(Max 13 caracteres):","destinatario de la resolucion(Max 13 caracteres):"};
    char *tiposDetalle[5] = {"la descripcion de la denuncia(Max 100 caracteres):","el detalle de la declaracion(Max 100 caracteres):","la informacion de la prueba(Max 100 caracteres):","el detalle de la diligencia(Max 100 caracteres):","el tipo de resolucion judicial(Max 100 caracteres):"};
    char *tiposFecha[5] = {"denuncia(Formato DD/MM/AAAA):","declaracion(Formato DD/MM/AAAA):","creacion del registro(Formato DD/MM/AAAA):","diligencia(Formato DD/MM/AAAA):","emision de la resolucion judicial(Formato DD/MM/AAAA):"};
    char bufferInvolucrado[61] = "Ingrese el RUT del ";
    char bufferDetalle[61] = "Ingrese ";
    char bufferFecha[61] = "Ingrese la fecha de la ";

    /*Reserva de memoria para el nuevo registro*/
    nuevo = (struct Registro *) malloc(sizeof(struct Registro));

    printf("Ingrese el id del registro:");
    scanf("%d",&(nuevo->id));

    nuevo->involucrado = leerCadena(strcat(bufferInvolucrado,tiposInvolucrado[tipoRegistro]));

    nuevo->detalle = leerCadena(strcat(bufferDetalle,tiposDetalle[tipoRegistro]));

    nuevo->fechaRegistro = leerCadena(strcat(bufferFecha,tiposFecha[tipoRegistro]));

    nuevo->tipo = tipoRegistro;

    return nuevo;

}




void agregarRegistroAlista(struct NodoRegistro **lista, struct Registro *registro) {
    struct NodoRegistro *nuevo;
    nuevo = (struct NodoRegistro *) malloc(sizeof(struct NodoRegistro));
    nuevo->dataRegistro = registro;
    nuevo->sig = NULL;

    nuevo->sig = *lista;
    *lista = nuevo;
}

/*Funcion agregarRegistro: Encargada de agregar un registro a una lista enlazada. Recibe por
parametros la estructura siau, y el registro que se quiere agregar. Primero preguntara al usuario
en que causa quiere agregar el registro, buscandola en el arbol de causas. Si la encuentra, accedera
al arreglo estatico de listas de registros, donde segun el tipo que sea el nuevo registro, se insertara
un nuevo nodo con el registro creado. Retorna un -1 si la causa no existe, un 0 si el registro ya existia,
y un 1 si se agrego correctamente.*/
int agregarRegistro(struct SIAU *siau, struct Registro *nuevo){
    struct Causa *causaBuscada = NULL;
    struct Registro *registroBuscado = NULL;
    struct NodoRegistro *nuevoNodo = NULL;
    char *rucBuscado = NULL;
    int tipoRegistro;

    rucBuscado = leerCadena("Ingrese el RUC de la causa a la que quiere acceder(Max 14 caracteres):");
    causaBuscada = buscarCausaPorRuc(siau->causas,rucBuscado);

    if(causaBuscada == NULL)
        return -1;/*La causa no existe.*/

    registroBuscado = buscarRegistroPorId(causaBuscada->investigacion , nuevo->tipo, nuevo->id);

    if(registroBuscado != NULL)
        return 0;/*El registro nuevo ya existe.*/

    nuevoNodo = (struct NodoRegistro *) malloc(sizeof(struct NodoRegistro));
    nuevoNodo->dataRegistro = nuevo;
    nuevoNodo->sig = NULL;

    tipoRegistro = nuevo->tipo;

    nuevoNodo->sig = causaBuscada->investigacion->registros[tipoRegistro];
    causaBuscada->investigacion->registros[tipoRegistro] = nuevoNodo;

    return 1;/*Se agrego el nuevo registro correctamente al inicio de la lista.*/
}
/*Funcion codigoAgregarRegistroMenu: Funcion para agregar un registro en el menu, funciona
 como interfaz de mensajes que entrega dependiendo del resultado de la accion que se realizo.
 Recibe por parametro el siau y el tipo de registro que se quiere agregar.
 */
void codigoAgregarRegistroMenu(struct SIAU *siau, int tipoRegistro) {
    struct Registro *tempRegistro = NULL;
    int resultadoInstruccion;

    tempRegistro = crearRegistro(tipoRegistro);
    resultadoInstruccion = agregarRegistro(siau, tempRegistro);
    if (resultadoInstruccion == 1)
        printf("El registro se agrego correctamente.");
    else if (resultadoInstruccion == 0)
        printf("El registro ya existia en la causa (Mismo id y mismo tipo).");
    else if (resultadoInstruccion == -1)
        printf("No se encontro la causa a la cual agregar el registro.");
}

/*Funcion agregarImputado: Funcion encargada de agregar una cadena que contendra un RUT
leido, en el arreglo de cadenas de alguna carpeta. Primero busca la causa a la cual se le agregara
el imputado, si la encuentra, buscara el rut nuevo en el arreglo, para evitar duplicados. Recibe
por parametro la estructura siau, y retorna -2 si la causa no existia, -1 si el imputado ya existia
en el arreglo, 0 si no queda espacio para agregar, y 1 si se agrego correctamente.*/
int agregarImputado(struct SIAU *siau){
    struct Causa *causaBuscada = NULL;
    char *nuevoImputado = NULL, *rucBuscado = NULL, *imputadoBuscado = NULL;
    int pLibre;

    rucBuscado = leerCadena("Ingrese el RUC de la causa a la que quiere acceder(Max 100 caracteres):");
    causaBuscada = buscarCausaPorRuc(siau->causas, rucBuscado);

    if(causaBuscada == NULL)
        return -2;/*No existe la causa*/

    nuevoImputado = leerCadena("Ingrese el RUT del nuevo imputado(Max 100 caracteres):");

    imputadoBuscado = buscarImputadoEnCarpeta(causaBuscada->investigacion, nuevoImputado);

    if(imputadoBuscado != NULL)
        return -1;/*El imputado ya existe en el arreglo*/

    pLibre = causaBuscada->investigacion->cantImputados;
    if(pLibre < MAX_IMPUTADOS){
        causaBuscada->investigacion->imputados[pLibre] = nuevoImputado;
        causaBuscada->investigacion->cantImputados += 1;
        return 1;/*imputado agregado correctamente.*/
    }

    return 0;/*No hay mas espacio para imputados*/

}

/*----------------------------------------------------------------------------------------------------------*/
/*---------------------------- FUNCIONES RELACIONADA CON LISTADO E IMPRESION -------------------------------*/
/*----------------------------------------------------------------------------------------------------------*/

/*Funcion que imprime el rut de un imputado, toma con parametro al imputado*/
void imprimirImputado(char *imputado) {
    printf("Rut del imputado : %s\n", imputado);
}

/*Funcion que imprime un registro con un formato, recibe como parametro un registro*/
void imprimirRegistro(struct Registro *registro) {
    printf("===========================================================\n");
    printf(" ID : %i\n", registro->id);
    switch (registro->tipo) {
        default:
            printf(" Tipo : Denuncia\n");
            break;
        case 1:
            printf(" Tipo : Declaracion\n");
            break;
        case 2:
            printf(" Tipo : Prueba\n");
            break;
        case 3:
            printf(" Tipo : Diligencia\n");
            break;
        case 4:
            printf(" Tipo : Resolucion Judicial\n");
            break;
    }
    printf(" Fecha: %s", registro->fechaRegistro);
    printf(" ----------------------------------------------------------\n");
    printf(" Involucrado : \n");
    if (registro->involucrado != NULL) {
        printf("  %s\n", registro->involucrado);
    }
    printf(" ----------------------------------------------------------\n");
    printf(" Detalle:\n");
    printf("   %s\n", registro->detalle);
    printf("===========================================================\n");
}

/*Funcion que recibe una causa e imprime su ruc, estado y la denuncia inicial (llamando a la funcion para imprimir registro)*/
void imprimirCausa(struct Causa *causa) {
    printf(" --------------------------------------------------------- \n");
    printf("| Ruc : %s       | Estado : ", causa->ruc);
    switch (causa->estado) {
        case 0:
            printf("Abierta           |\n");
            break;
        case 1:
            printf("Cerrada           |\n");
            break;
        case 2:
            printf("Archivada         |\n");
            break;
        case 3:
            printf("En juicio         |\n");
            break;
        default:
            printf("Error.");
    }
    printf(" --------------------------------------------------------- \n");
    imprimirRegistro(causa->denunciaInicial);
    printf("\n");
}

/*Funcion que recorre un arreglo de imputados y llama a la funcion para imprimirlos*/
void recorrerImputados(char **imputados, int cantidad) {
    int i;
    for (i = 0 ; i < cantidad ; i ++) {
        imprimirImputado(imputados[i]);
    }
}

/*Funcion que recorre una lista de registros y llama a la funcion para imprimir cada uno de ellos*/
void recorrerRegistrosParaImprimirlos(struct NodoRegistro *listaRegistros) {
    if (listaRegistros == NULL)
        printf("No hay registros\n");
    while (listaRegistros != NULL) {
        /*llamada a la funcion para imprimir el registro*/
        imprimirRegistro(listaRegistros->dataRegistro);
        listaRegistros = listaRegistros->sig;
    }
}

/*Funcion recursiva que recorre el arbol de causas en orden y que llama a otra funcion para imprimir el contenido*/
void recorrerCausasParaImprimirlas(struct NodoCausa *actual) {
    if (actual != NULL) {
        recorrerCausasParaImprimirlas(actual->izq);
        /*llamada a la funcion para imprimir los datos*/
        imprimirCausa(actual->datosCausa);
        recorrerCausasParaImprimirlas(actual->der);
    }
    return;
}

/*Funcion que recibe una carpeta y un parametro que es el indice del arreglo de nodos*/
void listarCarpetas(struct Carpeta *carpeta, int parametro) {
    if (carpeta != NULL) {/*Necesaria la condicion??*/
        /*Pregunta si el parametro es un indice del arreglo de nodos*/
        if (parametro >= 0 && parametro <= 4) {
            /* Si es un indice, se lo pasa a la funcion para recorrer la lista correspondiente a ese indice*/
            recorrerRegistrosParaImprimirlos(carpeta->registros[parametro]);
        }
        /* Si no es un indice, entonces significa que se quiere imprimir los imputados*/
        else {
            /* Si en la carpeta los imputados son mayores que cero se llama a la funcion para recorrer el arreglo*/
            if (carpeta->cantImputados > 0)
                recorrerImputados(carpeta->imputados, carpeta->cantImputados);
            else
                printf("No hay imputados para mostrar\n"); /*En caso de los imputados sean cero*/
        }
    }
    else
        printf("La carpeta no existe\n"); /*En caso de la carpeta no exista*/
}

/* aquí estan las 2 pincipales funciones extras que son las que se complementan con el codigo de
 imprimir Causas e imprimir Registros
*/

// Mostrar Causas por estado
void  mostrarCausasPorEstado(struct NodoCausa * raiz, int estado)// 1,2,3 o 4
{
    if (raiz == NULL) return;
    //1.- Se recorre el subArbol izquierdo
    mostrarCausasPorEstado(raiz->izq, estado);

    // si el estado es igual, imprimimos la causa usando la función de imprimir causa
    if (raiz->datosCausa != NULL && raiz->datosCausa->estado == estado)
    {
        // falta implementar la función
        imprimirCausa(raiz->datosCausa);
    }

    // Recorre a la derecha
    mostrarCausasPorEstado(raiz->der, estado);
}

//Mostrar resoluciones judiciales de imputado

void mostrarResolucionesJudicialesDeImputado(struct Carpeta * carpeta, const char * imputadoBuscado)
{
    struct NodoRegistro *actual = carpeta->registros[4];

    if (carpeta == NULL || imputadoBuscado == NULL) return;

    while (actual != NULL)
    {
        // compara si existe el imputado y si lo está lo imprime
        if (actual->dataRegistro->involucrado != NULL && strcmp(actual->dataRegistro->involucrado, imputadoBuscado) == 0)
        {
            // falta implementar la función
            imprimirRegistro(actual->dataRegistro);
        }
        actual = actual->sig;
    }

}



/*----------------------------------------------------------------------------------------------------------*/
/*----------------------- FUNCIONES RELACIONADAS CON BUSCAR UN DATO ESPECIFICO -----------------------------*/
/*----------------------------------------------------------------------------------------------------------*/


/*Función para  buscar una causa dado o entregado un ruc en especifico*/
struct Causa *buscarCausaPorRuc(struct NodoCausa *causas, char *ruc)
{
    if (causas == NULL)return NULL;

    if (strcmp(ruc, causas->datosCausa->ruc) == 0)
    {
        return causas->datosCausa;
    }
    else if (strcmp(ruc,causas->datosCausa->ruc ) < 0 )
    {
        return buscarCausaPorRuc(causas->izq, ruc);
    }
    else
    {
        return buscarCausaPorRuc(causas->der, ruc);
    }

}

/*Función para buscarRegistro en la carpeta dado un ID, de diferente tipo, ya sea 1, 2 ,3 ó 4 */
struct Registro *buscarRegistroPorId(struct Carpeta * carpeta, int tipo, int idRegistro)
{
    struct NodoRegistro * actual;

    if (carpeta == NULL || tipo < 0 || tipo > 4)
        return NULL;
    
    actual = carpeta->registros[tipo];
    while (actual != NULL)
    {
        if (actual->dataRegistro != NULL && actual->dataRegistro->id == idRegistro)
            return actual->dataRegistro;

        actual = actual->sig;
    }
    return NULL;
}
/*Función que busca un imputado en base al rut  entregado*/
char* buscarImputadoEnCarpeta(struct Carpeta * carpeta, const char * rutBuscado)
{
    int i;
    if (rutBuscado == NULL)
        return NULL;

    for (i = 0; i < carpeta->cantImputados;i++)
    {
        if (carpeta->imputados[i] != NULL && strcmp(rutBuscado, carpeta->imputados[i]) == 0)
        {
            return carpeta->imputados[i];
        }
    }
    return NULL;
}
//-----------------------------------------------------------------------------------------------

/*----------------------------------------------------------------------------------------------------------*/
/*------------------------ FUNCIONES RELACIONADAS CON MODIFICAR LOS DATOS ----------------------------------*/
/*----------------------------------------------------------------------------------------------------------*/

/*Funcion modificarCausa: Permite cambiar los datos guardados en cada campo de la causa, ya sean
los que estan dentro de la denuncia inicial, o el estado de la causa. Recibe por parametro
la raiz del arbol de causas.*/
void modificarCausa(struct NodoCausa * causas) {
    struct Causa *causaBuscada = NULL;
    int nuevoEstado, opcion = 0;

    causaBuscada = buscarCausaPorRuc(causas, leerCadena("Ingrese el RUC de la causa a modificar : "));

    if (causaBuscada == NULL) {
        printf("La RUC ingresado no se encontro en ninguna causa.\n");
        return;
    }

    while(opcion != 5){
        limpiarConsola();
        printf("                 -------------------                      \n");
        printf("                 | Modificar Causa |                     \n");
        printf("                 -------------------                      \n");
        printf("1) Modificar denunciante.\n");
        printf("2) Modificar descripcion de la denuncia.\n");
        printf("3) Modificar fecha de la denuncia.\n");
        printf("4) Modificar estado de la causa.\n");
        printf("5) Volver atras.\n");
        printf("Ingrese una opcion:");
        leerOpcion(&opcion,1,5);
        switch(opcion) {
            case 1:
                causaBuscada->denunciaInicial->involucrado = leerCadena("Ingrese el nuevo RUT del denunciante(Max 13 caracteres): ");
            printf("Modificacion realizada correctamente.");
            break;

            case 2:
                causaBuscada->denunciaInicial->fechaRegistro = leerCadena("Ingrese la nueva descripcion de la denuncia(Max 100 caracteres): ");
            printf("Modificacion realizada correctamente.");
            break;

            case 3:
                causaBuscada->denunciaInicial->detalle = leerCadena("Ingrese la nueva fecha(Formato DD/MM/AAAA): ");
            printf("Modificacion realizada correctamente.");
            break;

            case 4:
                nuevoEstado = modificarEstadoCausa();
                if(nuevoEstado != -1){
                    causaBuscada->estado = nuevoEstado;
                    printf("Modificacion realizada correctamente.");
                }
            break;

            case 5:
                return;

            default:
                printf("Error.");
        }
    }
    return;

}

/*Funcion modificarEstadoCausa: Encargada de realizar de manera mas amigable con el usuario el cambio de estado
de la causa, representa a traves de un menu de prints las distintas opciones para realizar la modificacion.
Retorna un numero correspondiente al nuevo estado de la causa, o un -1 si se decide volver al menu anterior.*/
int modificarEstadoCausa(){
    int opcion = 0;

    while(opcion != 5){
        limpiarConsola();
        printf("Seleccione el nuevo estado de la causa\n");
        printf("-------------------------------------\n");
        printf("[1] En investigacion.\n");
        printf("[2] Cerrada.\n");
        printf("[3] En juicio.\n");
        printf("[4] Archivada.\n");
        printf("[5] Volver atras.\n");
        printf("Ingrese una opcion:");
        leerOpcion(&opcion,1,5);
        switch(opcion){
            case 1:
                return 0;

            case 2:
                return 1;

            case 3:
                return 2;

            case 4:
                return 3;

            case 5:
                return -1;

            default:
                printf("Error.");
                return -1;
        }
    }
}

/* Funcion que valida la modificacion de un registro, si todo esta bien llama a la funcion de modificar*/
void validarModificacionRegistros(struct SIAU * siau, int tipo) {
    struct Causa *causaBuscada;
    struct Registro *registroBuscado;
    int idBuscado;

    causaBuscada = buscarCausaPorRuc(siau->causas, leerCadena("Ingrese el RUC de la causa a modificar : "));
    printf("Ingrese el ID del registro buscado : ");
    scanf("%d", &idBuscado);
    if (causaBuscada != NULL && causaBuscada->investigacion != NULL) {
        registroBuscado = buscarRegistroPorId(causaBuscada->investigacion, tipo, idBuscado);
        if (registroBuscado != NULL)
            modificarRegistro(registroBuscado, causaBuscada->investigacion);
        else
            printf("No existe ese registro.\n");
    }
    else
        printf("No existe.\n");
}

/*Funcion modificarRegistro: Menu donde se encuentran las opciones para modificar datos
de un registro. Recibe por parametro una estructura Registro y la carpeta.
Esta ultima es necesaria por si se quiere cambiar el tipo del registro*/
void modificarRegistro(struct Registro *registro, struct Carpeta *carpeta) {
    int opcion = 0;
    while(opcion != 5){
        limpiarConsola();
        printf("                 --------------------                      \n");
        printf("                | Modificar Registro |                     \n");
        printf("                 --------------------                      \n");
        printf("1) Modificar persona involucrada.\n");
        printf("2) Modificar fecha del registro.\n");
        printf("3) Modificar descripcion del registro.\n");
        printf("4) Modificar el tipo del registro.\n");
        printf("5) Volver atras.\n");
        printf("Ingrese una opcion:");
        leerOpcion(&opcion,1,5);
        switch(opcion){
            case 1:
                registro->involucrado = leerCadena("Ingrese el nuevo RUT de la persona involucrada: ");
            break;

            case 2:
                registro->fechaRegistro = leerCadena("Ingrese la nueva fecha: ");
            break;

            case 3:
                registro->detalle = leerCadena("Ingrese la nueva descripcion del registro: ");
            break;

            case 4:
                modificarTipoRegistro(registro, carpeta);
            break;

            case 5:
                return;

            default:
                printf("Error.");
        }
    }
    return;
}

/*Funcion modificarTipoRegistro: Menu donde se encuentran las opciones para modificar el
 tipo de un registro. Recibe por parametro una estructura Registro y la carpeta que lo contiene.*/
void modificarTipoRegistro(struct Registro *registro, struct Carpeta *carpeta) {
    int opcion = 0;
    while(opcion != 6){
        limpiarConsola();
        printf("Seleccione el nuevo tipo del registro\n");
        printf("-------------------------------------\n");
        printf("[1] Denuncia.\n");
        printf("[2] Declaracion.\n");
        printf("[3] Prueba.\n");
        printf("[4] Diligencia.\n");
        printf("[5] Resolucion Judicial.\n");
        printf("[6] Volver atras.\n");
        printf("Ingrese una opcion:");
        leerOpcion(&opcion,1,6);
        switch(opcion){
            case 1:
                if (registro->tipo != 0) {
                    if (buscarRegistroPorId(carpeta, 0, registro->id) == NULL){
                        eliminarRegistro(&carpeta->registros[registro->tipo], registro->id, "Denuncia");
                        registro->tipo = 0;
                        agregarRegistroAlista(&carpeta->registros[registro->tipo], registro);
                    }
                    else
                        printf("Ese id ya esta tomado.\n");
                }
                else
                    printf("El registro ya tiene ese tipo.\n");
                break;

            case 2:
                if (registro->tipo != 1) {
                    if(buscarRegistroPorId(carpeta, 1, registro->id) == NULL){
                        eliminarRegistro(&carpeta->registros[registro->tipo], registro->id, "Declaracion");
                        registro->tipo = 1;
                        agregarRegistroAlista(&carpeta->registros[registro->tipo], registro);
                    }
                    else
                        printf("Ese id ya esta tomado.\n");
                }
                else
                    printf("El registro ya tiene ese tipo.\n");
                break;

            case 3:
                if (registro->tipo != 2) {
                    if(buscarRegistroPorId(carpeta, 2, registro->id) == NULL){
                        eliminarRegistro(&carpeta->registros[registro->tipo], registro->id, "Prueba");
                        registro->tipo = 2;
                        agregarRegistroAlista(&carpeta->registros[registro->tipo], registro);
                    }
                    else
                        printf("Ese id ya esta tomado.\n");
                }
                else
                    printf("El registro ya tiene ese tipo.\n");
                break;

            case 4:
                if (registro->tipo != 3) {
                    if(buscarRegistroPorId(carpeta, 3, registro->id) == NULL){
                        eliminarRegistro(&carpeta->registros[registro->tipo], registro->id, "Diligencia");
                        registro->tipo = 3;
                        agregarRegistroAlista(&carpeta->registros[registro->tipo], registro);
                    }
                    else
                        printf("Ese id ya esta tomado.\n");
                }
                else
                    printf("El registro ya tiene ese tipo.\n");
                break;

            case 5:
                if (registro->tipo != 4) {
                    if (buscarRegistroPorId(carpeta, 4, registro->id) == NULL){
                        eliminarRegistro(&carpeta->registros[registro->tipo], registro->id, "Resolucion Judicial");
                        registro->tipo = 4;
                        agregarRegistroAlista(&carpeta->registros[registro->tipo], registro);
                    }
                    else
                        printf("Ese id ya esta tomado.\n");
                }
                else
                    printf("El registro ya tiene ese tipo.\n");
                break;

            case 6:
                return;

            default:
                printf("Error.");
        }
    }
    return;
}



/*----------------------------------------------------------------------------------------------------------*/
/*------------------------ FUNCIONES RELACIONADAS CON ELIMINAR DATOS ESPECIFICOS----------------------------*/
/*----------------------------------------------------------------------------------------------------------*/


/*FUNCION: Eliminar un registro en lista simplemente enlazada*/
void eliminarRegistro(struct NodoRegistro **head, int id, char *nombreTipoRegistro) {
    /*Declaracion de variables*/
    struct NodoRegistro *rec = (*head), *ant = NULL;

    /*Caso: La lista esta vacia*/
    if (rec == NULL) {
        printf("La lista esta vacia.\n");
        return;
    }
    while (rec != NULL) {
        if (rec->dataRegistro->id == id) {
            /*Caso 1: Es el head*/
            if (ant == NULL) {
                (*head) = rec->sig;
            }
            /*Caso 2: Está entre medio o al final de la lista*/
            else {
                ant->sig = rec->sig;
            }
            printf("La %s (ID %d) fue eliminada exitosamente. \n", nombreTipoRegistro, id);
            return;
        }
        ant = rec;
        rec = rec->sig;
    }
    printf("La %s (ID %d) no ha sido encontrada.\n", nombreTipoRegistro, id);
}

/*FUNCION PRINCIPAL DE ELIMINACION: Elimina un registro dado, del arreglo "registros"
Recibe el número y el titulo, dependiendo de que desea eliminar:
"0": "Denuncia", "1": "Declaración", "2": "Prueba", "3": "Diligencia", "4": "Resolución judicial"
Pide datos: RUC e ID del registro*/
void eliminarRegistroEnArreglo(struct SIAU *siau, int tipoRegistro, char *nombreTipoRegistro) {
    struct Causa *causa = NULL;
    char rucCausa[15];
    int idRegistro;

    /*Consultar sobre los datos*/
    printf("Ingrese el RUC de la causa a la que desea eliminar la %s(Max 14 caracteres):", nombreTipoRegistro);
    scanf("%s", rucCausa);
    printf("Ingrese el id de la %s a eliminar: ", nombreTipoRegistro);
    scanf("%d", &idRegistro);

    /*Se busca la Causa para verificar de que exista*/
    causa = buscarCausaPorRuc(siau->causas, rucCausa);
    if (causa != NULL) {
        /*Se valida de que la carpeta investigativa y el arreglo de registros existan*/
        if (causa->investigacion != NULL && causa->investigacion->registros != NULL) {
            eliminarRegistro(&(causa->investigacion->registros[tipoRegistro]), idRegistro, nombreTipoRegistro);
        }
    }
    else{
        printf("No se encontro la Causa con RUC %s.\n", rucCausa);
    }
}

/*FUNCION: Encuentra el reemplazo para la eliminacion de NodoCausa (ABB) del Caso 3: Nodo con dos hijos
Busca el nodo mas pequeno del sub arbol derecho*/
struct NodoCausa* encontrarReemplazo(struct NodoCausa* nodo) {
    while (nodo && nodo->izq != NULL)
        nodo = nodo->izq;
    return nodo;
}

/*FUNCION: Elimina un nodo del ABB*/
struct NodoCausa *eliminarCausa(struct NodoCausa *raiz, char *rucCausa, int *eliminacionExitosa) {
    struct Causa *datosAEliminar = NULL;
    struct NodoCausa *aux = NULL;
    int ruc;

    /*Caso: El arbol esta vacio */
    if (raiz == NULL) {
        printf("No se encontro una Causa con ese RUC.\n");
        *eliminacionExitosa = 0;
        return raiz;
    }

    ruc = strcmp(rucCausa, raiz->datosCausa->ruc);
    /*Buscar el NodoCausa*/
    if (ruc < 0) {
        raiz->izq = eliminarCausa(raiz->izq, rucCausa, eliminacionExitosa);
    } else if (ruc > 0) {
        raiz->der = eliminarCausa(raiz->der, rucCausa, eliminacionExitosa);
    /*El NodoCausa fue encontrado*/
    } else {
        *eliminacionExitosa = 1;
        /*Caso 1: No tiene hijos o solo tiene derecho*/
        if (raiz->izq == NULL) {
            printf("Se elimino la Causa con RUC %s.\n", rucCausa);
            /*Se salta el nodo, conectando su hijo derecho con el arbol padre*/
            return raiz->der;

        /*Caso 2: Solo tiene el hijo izquierdo*/
        } else if (raiz->der == NULL) {
            printf("Se elimino la Causa con RUC %s.\n", rucCausa);
            /*Se salta el nodo, conectando su hijo izquierdo con el arbol padre*/
            return raiz->izq;
        }
    /*Caso 3: Tiene los dos hijos, izquierda y derecha*/
        /*Buscar el reemplazo*/
        aux = encontrarReemplazo(raiz->der);
        /*Cambio de valores entre el nodo actual y el reemplazo*/
        datosAEliminar = raiz->datosCausa;
        raiz->datosCausa = aux->datosCausa;
        aux->datosCausa = datosAEliminar;
        /*Ahora busca el nodo con los datos a eliminar que quedaron en el de reemplazo, como es el
        menor del subarbol derecho, su hijo izquierdo es NULL, cae en caso 1 y es eliminado*/
        raiz->der = eliminarCausa(raiz->der, aux->datosCausa->ruc, eliminacionExitosa);
    }
    /*Al finalizar la eliminacion del nodo, se retorna el arbol actualizado*/
    return raiz;
}

/*FUNCION PRINCIPAL DE ELIMINACION: Elimina una Causa
Pide datos: RUC, lo busca y lo elimina*/
void eliminarCausaEnArbol(struct SIAU *siau) {
    /*Declaracion de variables*/
    char rucCausa[16];
    int eliminacionExitosa = 0;

    /*Consultar sobre los datos*/
    printf("Ingrese el RUC de la Causa que desea eliminar.\n");
    scanf("%s", rucCausa);
    siau->causas = eliminarCausa(siau->causas, rucCausa, &eliminacionExitosa);
    if (eliminacionExitosa) {
        siau->cantCausas--;
    }
}

void compactarArreglo(int pos, char **imputados, int pLibre) {
    int i;
    for(i = pos ; i < pLibre - 1 ; i++) {
        imputados[i] = imputados[i + 1];
    }
    imputados[pLibre - 1] = NULL;
}

/*FUNCION: Elimina el RUT de un imputado en el arreglo "Imputados"*/
void eliminarImputado(char **imputados, int *cantImputados, char *rutImputado) {
    int i;
    /*Se recorre el arreglo*/
    for(i = 0 ; i < (*cantImputados) ; i++) {
        if (strcmp(imputados[i], rutImputado) == 0) {
            /*Se compacta el arreglo y se decrementa el pLibre*/
            compactarArreglo(i, imputados, (*cantImputados));
            (*cantImputados)--;
            printf("Se elimino el Imputado de RUT %s.\n", rutImputado);
            return;
        }
    }
    printf("No se encontro Imputado con RUT %s.\n", rutImputado);
}

/*FUNCION PRINCIPAL DE ELIMINACION: Elimina un imputado de una Causa. Pide datos: RUC y RUT*/
void eliminarImputadoDeCausa(struct SIAU *siau) {
    struct Causa *causa = NULL;
    char rutImputado[14], rucCausa[16];

    /*Consulta sobre los datos*/
    printf("Ingrese el RUC de la Causa a la que desea eliminar el imputado: ");
    scanf("%s", rucCausa);
    printf("Ingrese el RUT del imputado a eliminar: ");
    scanf("%s", rutImputado);

    /*Se verifica si existe la causa*/
    causa = buscarCausaPorRuc(siau->causas, rucCausa);
    if (causa != NULL) {
        if (causa->investigacion != NULL && causa->investigacion->imputados != NULL) {
            /*Si existe la Causa y el arreglo de Imputados, se elimina al acusado con el rut*/
            eliminarImputado(causa->investigacion->imputados, &causa->investigacion->cantImputados, rutImputado);
        }
    }
    else {
        printf("No se encontro la Causa con RUC %s.\n", rucCausa);
    }
}

// ----------------------------------------------------------------------------------


/*Funcion leerOpcion: Encargada de leer una opcion para un menu con cierta cantidad de opciones.
 Recibe por parametros una variable donde se leera la opcion, y dos limites(uno inferior y
 otro superior), los cuales permiten validar que el usuario ingrese una opcion dentro del
 rango permitido.*/
void leerOpcion(int *opcion, int limInf, int limSup){
    scanf("%d",opcion);
    while((*(opcion) < limInf) || (*(opcion) > limSup)){
        printf("Opcion no valida, ingresar nuevamente:");
        scanf("%d",opcion);
    }
    return;
}

/*Funcion limpiarConsola: Encargada de imprimir multiples saltos de linea para hacer
 mas atractivo a la vista el programa en consola.*/
void limpiarConsola() {
    /*printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");*/
    printf("\n\n");

}

/*Funcion agregarDatos: Menu donde se encuentran las opciones para agregar datos
a un SIAU. Recibe por parametro una estructura SIAU.*/
void agregarDatos(struct SIAU *siau){
    struct Causa *tempCausa;
    int opcion = 0;
    while(opcion != 8){
        /*limpiarConsola();*/
        printf("===========================================================\n");
        printf("=======================Agregar Datos=======================\n");
        printf("===========================================================\n");
        printf("1- Agregar Causa.\n");
        printf("2- Agregar denuncia en carpeta.\n");
        printf("3- Agregar declaracion en carpeta.\n");
        printf("4- Agregar prueba en carpeta.\n");
        printf("5- Agregar diligencia en carpeta.\n");
        printf("6- Agregar resolucion judicial en carpeta.\n");
        printf("7- Agregar imputado en carpeta.\n");
        printf("8- Volver atras.\n");
        printf("Ingrese una opcion:");
        leerOpcion(&opcion,1,8);
        printf("\n");
        switch(opcion){

            case 1:
                /*Agregar causa en carpeta.*/
                tempCausa = crearCausa();
                if (agregarCausa(&(siau->causas), tempCausa)) {
                    printf("La causa fue agregada correctamente.\n");
                    siau->cantCausas++;
                    printf("Causas existentes actualmente: %d\n\n",siau->cantCausas);
                }else {
                    printf("La causa no se pudo agregar debido a que ya existia.\n\n");
                }
                break;

            case 2:
                /*Agregar denuncia a carpeta*/
                codigoAgregarRegistroMenu(siau,0);

                break;

            case 3:
                /*Agregar declaracion a carpeta*/
                codigoAgregarRegistroMenu(siau,1);
                break;

            case 4:
                /*Agregar prueba a carpeta*/
                codigoAgregarRegistroMenu(siau,2);
                break;

            case 5:
                /*Agregar diligencia a carpeta*/
                codigoAgregarRegistroMenu(siau,3);
                break;

            case 6:
                /*Agregar resolucion judicial a carpeta*/
                codigoAgregarRegistroMenu(siau,4);
                break;

            case 7:
                /*Agregar imputado a carpeta*/
                break;

            case 8:
                /*Volver atras*/
                return;


            default:
                printf("Error.");
        }
    }
}

/*-------------------------------------------------------------------------------------------------*/
/*Funcion listarDatos y listarDatosCarpeta: Menus donde se encuentran las opciones para listar datos
de un SIAU. */
void listarDatosCarpeta(struct Carpeta *carpeta) {
    int opcion = 0;
    while(opcion != 7){
        limpiarConsola();
        printf("-----------------------------------------------------------\n");
        printf("                  Listar Datos en Carpeta                  \n");
        printf("-----------------------------------------------------------\n");
        printf("1- Listar denuncias en carpeta.\n");
        printf("2- Listar declaraciones en carpeta.\n");
        printf("3- Listar pruebas en carpeta.\n");
        printf("4- Listar diligencias en carpeta.\n");
        printf("5- Listar resoluciones judiciales en carpeta.\n");
        printf("6- Listar imputados en carpeta.\n");
        printf("7- Volver atras.\n");
        printf("Ingrese una opcion:");
        leerOpcion(&opcion,1,7);
        switch(opcion){
            case 1:
                /*llamada a la funcion para imprimir los datos de la carpeta*/
                listarCarpetas(carpeta, 0);
                break;

            case 2:
                listarCarpetas(carpeta, 1);
                break;

            case 3:
                listarCarpetas(carpeta, 2);
                break;

            case 4:
                listarCarpetas(carpeta, 3);
                break;

            case 5:
                listarCarpetas(carpeta, 4);
                break;

            case 6:
                listarCarpetas(carpeta, 5);
                break;

            case 7:

                return;

            default:
                printf("Error.\n");
        }
    }
    return;
}

void listarDatos(struct SIAU *siau) {
    int opcion = 0;
    struct Causa * causaBuscada;
    while(opcion != 3) {
        limpiarConsola();
        printf("===========================================================\n");
        printf("=======================Listar Datos========================\n");
        printf("===========================================================\n");
        printf("1- Listar Causas.\n");
        printf("2- Listar datos en carpeta.\n");
        printf("3- Volver atras.\n");
        printf("Ingrese una opcion:");
        leerOpcion(&opcion,1,3);
        switch(opcion) {
            case 1:
                    /*Caso que quiera listar todas las causas del sistema*/
                    /*Pregunta si hay causas en el sistema, si las hay llama a la funcion para recorrer */
                    if (siau->cantCausas > 0)
                        recorrerCausasParaImprimirlas(siau->causas);
                    else
                        printf("No hay nada para mostrar\n");
                    break;

            case 2:
                    /* Caso de que se quiera listar los datos de la carpeta*/
                    /* Para ello se busca la causa que tiene la carpeta*/
                    causaBuscada = buscarCausaPorRuc(siau->causas, leerCadena("Ingrese RUC buscado : "));
                    /*Si la causa existe se pasa al siguiente menu*/
                    if (causaBuscada != NULL)
                        listarDatosCarpeta(causaBuscada->investigacion);
                    else
                        printf("No existe\n");
                    break;

            case 3:
                return;

            default:
                printf("Error.\n");
        }
    }
    return;
}
/*----------------------------------------------------------------------------------------------*/

/*Funcion buscarDatos: Menu donde se encuentran las opciones para buscar datos
en un SIAU. Recibe por parametro una estructura SIAU.*/
void buscarDatos(struct SIAU * siau/*, int tipo, int id, char * rucBuscado, char * rutBuscado*/) {
    int opcion = 0;
    while(opcion != 8){
        limpiarConsola();
        printf("===========================================================\n");
        printf("=======================Buscar Datos========================\n");
        printf("===========================================================\n");
        printf("1- Buscar causa por RUC.\n");
        printf("2- Buscar denuncia en carpeta por id.\n");
        printf("3- Buscar declaracion en carpeta por id.\n");
        printf("4- Buscar prueba en carpeta por id.\n");
        printf("5- Buscar diligencia en carpeta por id.\n");
        printf("6- Buscar resolucion judicial en carpeta por id.\n");
        printf("7- Buscar imputado en carpeta por RUT\n");
        printf("8- Volver atras.\n");
        printf("Ingrese una opcion:");
        leerOpcion(&opcion,1,9);
        printf("\n");
        switch(opcion){
            // falta mandar como parametro el rucBuscado
            case 1:
                /*Buscar causa por RUC*/
                    /*buscarCausaPorRuc(siau->causas, rucBuscado);*/
                break;

            case 2:
                /*Buscar denuncia en carpeta por id*/
                    // faltaria el número del arreglo y el id del registro para cada caso correspondiente
                    /*buscarRegistroPorId(siau->causas->datosCausa->investigacion, tipo, id);*/

                break;

            case 3:
                /*Buscar declaracion en carpeta por id*/
                    /*buscarRegistroPorId(siau->causas->datosCausa->investigacion, tipo, id);*/

                break;

            case 4:
                /*Buscar prueba en carpeta por id*/
                    /*buscarRegistroPorId(siau->causas->datosCausa->investigacion, tipo, id);*/

                break;

            case 5:
                /*Buscar diligencia en carpeta por id*/
                    /*buscarRegistroPorId(siau->causas->datosCausa->investigacion, tipo, id);*/
                break;

            case 6:
                /*Buscar resolucion judicial en carpeta por id*/
                    /*buscarRegistroPorId(siau->causas->datosCausa->investigacion, tipo, id);*/
                break;
            case 7:
                /*Buscar imputado por RUT*/
                    /*buscarImputadoEnCarpeta(siau->causas->datosCausa->investigacion,rutBuscado);*/
                break;

            case 8:
                /*Volver atras*/
                return;


            default:
                printf("Error.");
        }
    }
    return;
}

/*Funcion modificarDatos: Menu donde se encuentran las opciones para modificar datos
de un SIAU. Recibe por parametro una estructura SIAU.*/
void modificarDatos(struct SIAU * siau) {
    int opcion = 0;
    while(opcion != 8){
        limpiarConsola();
        printf("===========================================================\n");
        printf("======================Modificar Datos======================\n");
        printf("===========================================================\n");
        printf("1- Modificar Causa.\n");
        printf("2- Modificar denuncia en carpeta.\n");
        printf("3- Modificar declaracion en carpeta.\n");
        printf("4- Modificar prueba en carpeta.\n");
        printf("5- Modificar diligencia en carpeta.\n");
        printf("6- Modificar resolucion judicial en carpeta.\n");
        printf("7- Modificar imputado en carpeta.\n");
        printf("8- Volver atras.\n");
        printf("Ingrese una opcion:");
        leerOpcion(&opcion,1,8);
        printf("\n");
        switch(opcion){

            case 1:
                /*  Modificar causa*/
                    modificarCausa(siau->causas);
                    break;

            case 2:
                    validarModificacionRegistros(siau, 0);
                    break;

            case 3:
                    validarModificacionRegistros(siau, 1);
                    break;

            case 4:
                    validarModificacionRegistros(siau, 2);
                    break;

            case 5:
                    validarModificacionRegistros(siau, 3);
                    break;

            case 6:
                    validarModificacionRegistros(siau, 4);
                    break;
            case 7:
                /*Modificar imputado en carpeta*/
                    break;
            case 8:
                /*Volver atras*/
                    return;


            default:
                printf("Error.");
        }
    }
    return;
}
/*Funcion borrarDatos: Menu donde se encuentran las opciones para borrar datos
de un SIAU. Recibe por parametro una estructura SIAU.*/
void borrarDatos(struct SIAU * siau) {
    int opcion = 0;
    while(opcion != 8){
        limpiarConsola();
        printf("===========================================================\n");
        printf("=======================Borrar Datos========================\n");
        printf("===========================================================\n");
        printf("1- Borrar Causa.\n");
        printf("2- Borrar denuncia en carpeta.\n");
        printf("3- Borrar declaracion en carpeta.\n");
        printf("4- Borrar prueba en carpeta.\n");
        printf("5- Borrar diligencia en carpeta.\n");
        printf("6- Borrar resolucion judicial en carpeta.\n");
        printf("7- Borrar imputado en carpeta.\n");
        printf("8- Volver atras.\n");
        printf("Ingrese una opcion:");
        leerOpcion(&opcion,1,8);
        printf("\n");
        switch(opcion){

            case 1:
                /*  Borrar causa*/
                    eliminarCausaEnArbol(siau);
                break;

            case 2:
                /*Borrar denuncia en carpeta*/
                    eliminarRegistroEnArreglo(siau, 0, "Denuncia");
                break;

            case 3:
                /*Borrar declaracion en carpeta*/
                    eliminarRegistroEnArreglo(siau, 1, "Declaracion");
                break;

            case 4:
                /*Borrar prueba en carpeta*/
                    eliminarRegistroEnArreglo(siau, 2, "Prueba");
                break;

            case 5:
                /*Borrar diligencia en carpeta*/
                    eliminarRegistroEnArreglo(siau, 3, "Diligencia");
                break;

            case 6:
                /*Borrar resolucion judicial en carpeta*/
                    eliminarRegistroEnArreglo(siau, 4, "Resolucion judicial");
                break;
            case 7:
                /*Borrar imputado en carpeta*/
                    eliminarImputadoDeCausa(siau);
                break;
            case 8:
                /*Volver atras*/
                return;


            default:
                printf("Error.");
        }
    }
    return;
}

/*Funcion otrasOpciones: Menu donde se encuentran opciones varias.
Recibe por parametro una estructura SIAU.*/
void otrasOpciones(struct SIAU * siau/*, int estado*/) {
    int opcion = 0;
    while(opcion != 7){
        limpiarConsola();
        printf("===========================================================\n");
        printf("=====================Otras opciones========================\n");
        printf("===========================================================\n");
        printf("1- Mostrar Causas por estado.\n");
        printf("2- Mostrar resoluciones judiciales de imputado.\n");
        printf("3- Mostrar resoluciones judiciales por tipo de resolucion.\n");
        printf("4- Generar reporte estadistico.\n");
        printf("5- Eliminar causas con estado cerrado.\n");
        printf("6- Funcion extra 2.\n");
        printf("7- Volver atras.\n");
        printf("Ingrese una opcion:");
        leerOpcion(&opcion,1,7);
        printf("\n");
        switch(opcion){

            case 1:

                /*  Mostrar causas*/
                    //faltaria pasar el estado según el estado
                    /*mostrarCausasPorEstado(siau->causas,estado);*/
                break;

            case 2:
                /*Mostrar resoluciones judiciales de imputado*/
                break;

            case 3:
                /*Mostrar resoluciones judiciales por tipo de resolucion*/
                break;

            case 4:
                /*Generar reporte estadistico*/
                break;

            case 5:
                /*Eliminar causas con estado cerrado.*/
                break;

            case 6:
                /*Funcion extra 2*/
                break;

            case 7:
                /*Volver atras*/
                return;


            default:
                printf("Error.");
        }
    }
    return;
}

/*===================================================================*/


/*===================================================================*/
/*===============DEFINICION DEL MAIN PARA EL PROGRAMA================*/
/*===================================================================*/
int main(void) {
    struct SIAU *siau = NULL;
    int opcion = 0;

    siau = (struct SIAU*)malloc(sizeof(struct SIAU));
    siau->causas = NULL;
    siau->cantCausas = 0;

    while(opcion != 7){
        limpiarConsola();
        printf("===========================================================\n");
        printf("========Sistema de Informacion y Atencion a Usuarios=======\n");
        printf("===========================================================\n");
        printf("1- Agregar Datos.\n");
        printf("2- Listar Datos.\n");
        printf("3- Buscar Datos.\n");
        printf("4- Modificar Datos.\n");
        printf("5- Borrar Datos.\n");
        printf("6- Otras opciones.\n");
        printf("7- Cerrar Programa.\n");
        printf("Ingrese una opcion:");
        leerOpcion(&opcion,1,7);
        switch(opcion){
            case 1:
                /*Agregar*/
                agregarDatos(siau);
                break;

            case 2:
                /*Listar*/
                listarDatos(siau);
                break;


            case 3:
                /*Buscar*/
                buscarDatos(siau);
                break;

            case 4:
                modificarDatos(siau);
                break;

            case 5:
                borrarDatos(siau);
                break;

            case 6:
                /*Otras opciones*/
                otrasOpciones(siau);
                break;

            case 7:
                /*Cerrar*/
                return 0;

            default:
                printf("Error.");
                break;

        }

    }

    return 0;
}

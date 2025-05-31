#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_IMPUTADOS 60

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

/*Funcion leerCadena: Encargada de leer una cadena de texto ingresada por el usuario. Lo leido
se almacena en un buffer temporal, para despues traspasarlo a una nueva cadena dinamica, reservandole
a esta una cantidad exacta de memoria. Recibe por parametro un mensaje que indica al usuario que debe ingresar,
y retorna un puntero a una cadena dinamica.*/
char * leerCadena(char *mensaje){
    char buffer[101];
    char limpiador;
    char *cadRetorno = NULL;
    int nCaracteres;

    /*Mostramos el mensaje indicatorio de que debe ingresar el usuario.*/
    printf("%s",mensaje);
    scanf(" %100[^\n]",buffer);/*Leemos hasta 100 caracteres, o hasta un salto de linea*/
    printf("\n");
    while( (limpiador = getchar()) != '\n' && limpiador != EOF);/*Limpiamos el archivo de entrada estandar*/

    nCaracteres = strlen(buffer);/*Obtenemos la longitud de la cadena leida*/
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

    /*Reservamos memoria para el arreglo de imputados de la carpeta.*/
    nueva->investigacion->imputados = (char **) malloc(MAX_IMPUTADOS * sizeof(char *));

    /*Como la carpeta parte vacia, se inicializa en 0 la cantidad de imputados.*/
    nueva->investigacion->cantImputados = 0;

    /*Iniciamos la causa en estado de investigacion.*/
    nueva->estado = 0;

    /*Ahora procederemos a leer los datos del usuario y a guardarlos
    en la nueva causa que creamos*/

    /*Leemos el RUC ingresado por el usuario, y lo guardamos en la causa.*/
    nueva->ruc = leerCadena("Ingrese el RUC de la causa:");

    /*Leemos el RUT del denunciante, y lo guardamos en la denuncia inicial de la causa*/
    nueva->denunciaInicial->involucrado = leerCadena("Ingrese el RUT del denunciante:");

    /*Leemos la descripcion de la denuncia. y la guardamos en la denuncia inicial de la causa.*/
    nueva->denunciaInicial->detalle = leerCadena("Ingrese la descripcion de la denuncia:");


    /*Leemos la fecha de la denuncia, y la guardamos en la denuncia inicial de la causa.*/
    nueva->denunciaInicial->fechaRegistro = leerCadena("Ingrese la fecha de la denuncia:");

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
}


/*----------------------------------- FUNCIONES DE LIBERACION DE MEMORIA -----------------------------------*/
/*FUNCION: Libera la memoria de un struct registro*/
void liberarMemoriaRegistro(struct Registro *registro) {
    if (registro != NULL) {
        free(registro->detalle);
        free(registro->fechaRegistro);
        free(registro->involucrado);
        free(registro);
    }
}

/*FUNCION: Libera la memoria de una lista simplemente enlazada (Para el Arreglo de registros)*/
void liberarMemoriaNodoRegistro(struct NodoRegistro *head) {
    struct NodoRegistro *aux;
    while (head != NULL) {
        aux = head;
        liberarMemoriaRegistro(aux->dataRegistro);
        free(aux);
        head = head->sig;
    }
}

/*FUNCION: Libera la memoria total de la carpeta investigativa*/
void liberarMemoriaCarpeta(struct Carpeta *carpeta) {
    int i, k;
    if (carpeta != NULL) {
        /*Liberación de memoria para */
        for(i = 0 ; i < 5 ; i++) {
            liberarMemoriaNodoRegistro(carpeta->registros[i]);
        }

        if (carpeta->imputados != NULL) {
            for (k = 0 ; k < carpeta->cantImputados ; k++) {
                free(carpeta->imputados[k]);
            }
            free(carpeta->imputados);
        }
        free(carpeta);
    }
}

/*FUNCION: Libera la memoria total de una Causa*/
void liberarMemoriaCausa(struct Causa *datos) {
    if (datos != NULL) {
        free(datos->ruc);
        liberarMemoriaRegistro(datos->denunciaInicial);
        liberarMemoriaCarpeta(datos->investigacion);
        free(datos);
    }
}
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
            /*Caso: Es el head*/
            if (ant == NULL) {
                (*head) = rec->sig;
            }
            /*Caso: Está entre medio o al final de la lista*/
            else {
                ant->sig = rec->sig;
            }
            /*Liberacion de memoria para el registro*/
            liberarMemoriaRegistro(rec->dataRegistro);
            printf("La %s (ID %d) fue eliminada exitosamente. \n", nombreTipoRegistro, id);
            return;
        }
        ant = rec;
        rec =  rec->sig;
    }
    printf("La %s (ID %d) no ha sido encontrada.\n", nombreTipoRegistro, id);
}

/*FUNCION PRINCIPAL DE ELIMINACION: Elimina un registro dado, del arreglo registros
Recibe el número y el titulo dependiendo de que desea eliminar:
"0": "Denuncia"
"1": "Declaracion"
"2": "Prueba"
"3": "Diligencia"
"4": "Resolucion judicial" Pide datos: RUC e ID del registro*/
void eliminarRegistroEnArreglo(struct SIAU *siau, int tipoRegistro, char *nombreTipoRegistro) {
    struct NodoCausa *causa = NULL;
    char rucCausa[15];
    int idRegistro;

    /*FALTA FUNCION: Comprobar RUT, RUC, si no es valido que vuelta a intentar ingresar RUT, RUC
      FALTA FUNCION: Comprobar RUT, RUC, si no es valido que vuelta a intentar ingresar RUT, RUC
      FALTA FUNCION: Comprobar RUT, RUC, si no es valido que vuelta a intentar ingresar RUT, RUC*/
    printf("Ingrese el RUC de la causa a la que desea eliminar la %s:", nombreTipoRegistro);
    scanf("%s", rucCausa);
    printf("Ingrese el ID de la %s a eliminar: ", nombreTipoRegistro);
    scanf("%d", &idRegistro);

    /*FALTA FUNCION: Buscar Causa, retorna la Causa si la encuentra, y si no NULL
      FALTA FUNCION: Buscar Causa, retorna la Causa si la encuentra, y si no NULL
      FALTA FUNCION: Buscar Causa, retorna la Causa si la encuentra, y si no NULL*/
    if (causa != NULL) {
        if (causa->datosCausa->investigacion != NULL && causa->datosCausa->investigacion->registros != NULL) {
            eliminarRegistro(&(causa->datosCausa->investigacion->registros[tipoRegistro]), idRegistro, nombreTipoRegistro);
        }
    }
    else{
        printf("No se encontro la Causa con RUC %s.\n", rucCausa);
    }
}

/*FUNCION: Encuentra el reemplazo para la eliminacion de NodoCausa (ABB) del Caso 3: Nodo con dos hijos*/
struct NodoCausa* encontrarMinimo(struct NodoCausa* nodo) {
    while (nodo && nodo->izq != NULL)
        nodo = nodo->izq;
    return nodo;
}

/*FUNCION: Elimina un nodo del ABB*/
struct NodoCausa *eliminarCausa(struct NodoCausa *raiz, char *rucCausa) {
    struct Causa *datosAEliminar = NULL;
    struct NodoCausa *aux = NULL;
    char *rucTemporal;
    int ruc;

    /*Caso: El arbol esta vacio */
    if (raiz == NULL) {
        printf("No se encontro una Causa con ese RUC.\n");
        return raiz;
    }

    ruc = strcmp(rucCausa, raiz->datosCausa->ruc);

    /*Buscar el NodoCausa*/
    if (ruc < 0) {
        raiz->izq = eliminarCausa(raiz->izq, rucCausa);
    } else if (ruc > 0) {
        raiz->der = eliminarCausa(raiz->der, rucCausa);
    /*El NodoCausa fue encontrado*/
    } else {

        /*Caso 1: No tiene hijos o solo tiene derecho*/
        if (raiz->izq == NULL) {
            aux = raiz->der;
            printf("Se elimino la Causa con RUC %s.\n", rucCausa);
            liberarMemoriaCausa(raiz->datosCausa);
            free(raiz);
            return aux;
        /*Caso 2: Solo tiene el hijo izquierdo*/
        } else if (raiz->der == NULL) {
            aux = raiz->izq;
            printf("Se elimino la Causa con RUC %s.\n", rucCausa);
            liberarMemoriaCausa(raiz->datosCausa);
            free(raiz);
            return aux;
        }
    /*Caso 3: Tiene los dos hijos, izquierda y derecha*/
        aux = encontrarMinimo(raiz->der);

        datosAEliminar = raiz->datosCausa;
        raiz->datosCausa = aux->datosCausa;
        aux->datosCausa = datosAEliminar;

        raiz->der = eliminarCausa(raiz->der, aux->datosCausa->ruc);
    }
    return raiz;
}

/*FUNCION PRINCIPAL DE ELIMINACION: Elimina una Causa, pide datos: RUC, lo busca y lo elimina*/
void eliminarCausaEnArbol(struct SIAU *siau) {
    /*Declaracion de variables*/
    char rucCausa[15];

    /*FALTA FUNCION: Comprobar RUC, si no es valido que vuelta a intentar ingresar RUT, RUC
      FALTA FUNCION: Comprobar RUC, si no es valido que vuelta a intentar ingresar RUT, RUC
      FALTA FUNCION: Comprobar RUT, RUC, si no es valido que vuelta a intentar ingresar RUT, RUC*/
    printf("Ingrese el RUC de la Causa que desea eliminar.\n");
    scanf("%s", rucCausa);
    siau->causas = eliminarCausa(siau->causas, rucCausa);
}

void compactarArreglo(int pos, char **imputados, int pLibre) {
    int i;
    for(i = pos ; i < pLibre - 1 ; i++) {
        imputados[i] = imputados[i + 1];
    }
    imputados[pLibre - 1] = NULL;
}

/*FUNCION: Elimina el RUT de un imputado en el arreglo Imputados*/
void eliminarImputado(char **imputados, int *cantImputados, char *rutImputado) {
    int i;
    /*Se recorre el arreglo*/
    for(i = 0 ; i < (*cantImputados) ; i++) {
        if (strcmp(imputados[i], rutImputado) == 0) {
            free(imputados[i]);
            /*Se compacta el arreglo y se decrementa el pLibre*/
            compactarArreglo(i, imputados, (*cantImputados));
            (*cantImputados)--;
            printf("Se elimino el Imputado de RUT %s.\n", rutImputado);
            return;
        }
    }
    printf("No se encontro Imputado con RUT %s.\n", rutImputado);
    return;
}

/*FUNCION PRINCIPAL DE ELIMINACION: Elimina un imputado de una Causa. Pide datos: RUC y RUT*/
void eliminarImputadoDeCausa(struct SIAU *siau) {
    struct Causa *causa = NULL;
    char rutImputado[14], rucCausa[16];
    int i;

    /*FALTA FUNCION: Comprobar RUT, RUC, si no es valido que vuelta a intentar ingresar RUT, RUC
      FALTA FUNCION: Comprobar RUT, RUC, si no es valido que vuelta a intentar ingresar RUT, RUC
      FALTA FUNCION: Comprobar RUT, RUC, si no es valido que vuelta a intentar ingresar RUT, RUC*/
    printf("Ingrese el RUC de la causa a la que desea eliminar el imputado: ");
    scanf("%s", rucCausa);
    printf("Ingrese el RUT del imputado a eliminar: ");
    scanf("%s", rutImputado);

    /*FALTA FUNCION: Buscar Causa, retorna la Causa si la encuentra, y si no NULL
      FALTA FUNCION: Buscar Causa, retorna la Causa si la encuentra, y si no NULL
      FALTA FUNCION: Buscar Causa, retorna la Causa si la encuentra, y si no NULL*/
    if (causa != NULL) {
        if (causa->investigacion->imputados != NULL) {
            eliminarImputado(causa->investigacion->imputados, &causa->investigacion->cantImputados, rutImputado);
        }
    }
    else {
        printf("No se encontro la Causa con RUC %s.\n", rucCausa);
    }
}
/*-------------------------------------------------------------------------
/*Función para  buscar una causa dado o entregado un ruc en especifico*/
struct Causa *buscarCausaPorRuc(struct NodoCausa *causas, char *ruc)
{
    struct Causa * causaBuscada;
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
    return NULL;
}

/*Función para buscarRegistro en la carpeta dado un ID, de diferente tipo, ya sea 1, 2 ,3 ó 4 */
struct Registro *buscarRegistroPorId(struct Carpeta * carpeta, int tipo, int id)
{
    if (carpeta == NULL || tipo < 0 || tipo > 4)
        return NULL;
    struct NodoRegistro * actual = carpeta->registros[tipo];

    while (actual != NULL)
    {
        if (actual->dataRegistro != NULL && actual->dataRegistro->id == id)
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

/*Funcion crearRegistro: Encargada de crear un registro, incluyendo la lectura
de datos correspondiente para que se llenen los campos. Recibe por parametro el
tipo de registro que se desea crear. Retorna el nuevo registro creado.*/
struct Registro * crearRegistro(int tipoRegistro){
    struct Registro *nuevo = NULL;
    /*Cadenas para indicar correctamente al usuario que ingresar, segun el contexto del tipo de registro*/
    char *tiposInvolucrado[5] = {"denunciante:","declarante:","investigador:","involucrado:","destinatario de la resolucion:"};
    char *tiposDetalle[5] = {"la descripcion de la denuncia:","el detalle de la declaracion:","la informacion de la prueba:","el detalle de la diligencia:","el tipo de resolucion judicial:"};
    char *tiposFecha[5] = {"denuncia:","declaracion:","creacion del registro:","diligencia:","emision de la resolucion judicial:"};
    char bufferInvolucrado[51] = "Ingrese el RUT del ";
    char bufferDetalle[51] = "Ingrese ";
    char bufferFecha[51] = "Ingrese la fecha de la ";

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
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

}

/*Funcion agregarDatos: Menu donde se encuentran las opciones para agregar datos
a un SIAU. Recibe por parametro una estructura SIAU.*/
void agregarDatos(struct SIAU *siau){
    struct Causa *tempCausa;
    struct Registro *tempRegistro;
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
                tempRegistro = crearRegistro(4);
                if (tempRegistro)printf("Registro creado bien.");
                else printf("Registro creado mal :(");
                break;

            case 3:
                /*Agregar declaracion a carpeta*/
                break;

            case 4:
                /*Agregar prueba a carpeta*/
                break;

            case 5:
                /*Agregar diligencia a carpeta*/
                break;

            case 6:
                /*Agregar resolucion judicial a carpeta*/
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

/*Funcion listarDatos: Menu donde se encuentran las opciones para listar datos
de un SIAU. Recibe por parametro una estructura SIAU.*/
void listarDatos(struct SIAU * siau) {
    int opcion = 0;
    while(opcion != 8){
        /*limpiarConsola();*/
        printf("===========================================================\n");
        printf("=======================Listar Datos========================\n");
        printf("===========================================================\n");
        printf("1- Listar Causas.\n");
        printf("2- Listar denuncias en carpeta.\n");
        printf("3- Listar declaraciones en carpeta.\n");
        printf("4- Listar pruebas en carpeta.\n");
        printf("5- Listar diligencias en carpeta.\n");
        printf("6- Listar resoluciones judiciales en carpeta.\n");
        printf("7- Listar imputados en carpeta.\n");
        printf("8- Volver atras.\n");
        printf("Ingrese una opcion:");
        leerOpcion(&opcion,1,8);
        printf("\n");
        switch(opcion){

            case 1:
                /*  Listar causas*/
                break;

            case 2:
                /*Listar denuncias en carpeta*/
                break;

            case 3:
                /*Listar declaraciones en carpeta*/
                break;

            case 4:
                /*Listar pruebas en carpeta*/
                break;

            case 5:
                /*Listar diligencias en carpeta*/
                break;

            case 6:
                /*Listar resoluciones judiciales en carpeta*/
                break;
            case 7:
                /*Listar imputados en carpeta*/
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

/*Funcion buscarDatos: Menu donde se encuentran las opciones para buscar datos
en un SIAU. Recibe por parametro una estructura SIAU.*/
void buscarDatos(struct SIAU * siau) {
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

            case 1:
                /*Buscar causa por RUC*/
                    buscarCausaPorRuc(siau->causas, rucBuscado);
                break;

            case 2:
                /*Buscar denuncia en carpeta por id*/
                    buscarRegistroPorId(siau->causas->datosCausa->investigacion, tipo, id);

                break;

            case 3:
                /*Buscar declaracion en carpeta por id*/
                    buscarRegistroPorId(siau->causas->datosCausa->investigacion, tipo, id);

                break;

            case 4:
                /*Buscar prueba en carpeta por id*/
                    buscarRegistroPorId(siau->causas->datosCausa->investigacion, tipo, id);

                break;

            case 5:
                /*Buscar diligencia en carpeta por id*/
                    buscarRegistroPorId(siau->causas->datosCausa->investigacion, tipo, id);
                break;

            case 6:
                /*Buscar resolucion judicial en carpeta por id*/
                    buscarRegistroPorId(siau->causas->datosCausa->investigacion, tipo, id);
                break;
            case 7:
                /*Buscar imputado por RUT*/
                    buscarImputadoEnCarpeta(siau->causas->datosCausa->investigacion,rutBuscado);
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
                break;

            case 2:
                /*Modificar denuncia en carpeta*/
                break;

            case 3:
                /*Modificar declaracion en carpeta*/
                break;

            case 4:
                /*Modificar prueba en carpeta*/
                break;

            case 5:
                /*Modificar diligencia en carpeta*/
                break;

            case 6:
                /*Modificar resolucion judicial en carpeta*/
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
void otrasOpciones(struct SIAU * siau) {
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
                break;

            default:
                printf("Error.");
                break;

        }

    }
    return 0;
}

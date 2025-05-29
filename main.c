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
                break;

            case 2:
                /*Buscar denuncia en carpeta por id*/
                break;

            case 3:
                /*Buscar declaracion en carpeta por id*/
                break;

            case 4:
                /*Buscar prueba en carpeta por id*/
                break;

            case 5:
                /*Buscar diligencia en carpeta por id*/
                break;

            case 6:
                /*Buscar resolucion judicial en carpeta por id*/
                break;
            case 7:
                /*Buscar imputado por RUT*/
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
                break;

            case 2:
                /*Borrar denuncia en carpeta*/
                break;

            case 3:
                /*Borrar declaracion en carpeta*/
                break;

            case 4:
                /*Borrar prueba en carpeta*/
                break;

            case 5:
                /*Borrar diligencia en carpeta*/
                break;

            case 6:
                /*Borrar resolucion judicial en carpeta*/
                break;
            case 7:
                /*Borrar imputado en carpeta*/
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
        printf("5- Funcion extra 1.\n");
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
                /*Funcion extra 1*/
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

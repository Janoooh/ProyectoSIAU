#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_IMPUTADOS 60;

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
void leerOpcion(int *opcion, int limInf, int limSup){
    scanf("%d",opcion);
    while((*(opcion) < limInf) || (*(opcion) > limSup)){
        printf("Opcion no valida, ingresar nuevamente:");
        scanf("%d",opcion);
    }
    return;
}

void limpiarConsola() {
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

}

void agregarDatos(){
    int opcion = 0;
    while(opcion != 8){
        limpiarConsola();
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
        switch(opcion){

            case 1:
                /*Agregar causa*/
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

void mostrarDatos() {
    int opcion = 0;
    while(opcion != 8){
        limpiarConsola();
        printf("===========================================================\n");
        printf("=======================Mostrar Datos=======================\n");
        printf("===========================================================\n");
        printf("1- Mostrar Causas.\n");
        printf("2- Mostrar denuncias en carpeta.\n");
        printf("3- Mostrar declaraciones en carpeta.\n");
        printf("4- Mostrar pruebas en carpeta.\n");
        printf("5- Mostrar diligencias en carpeta.\n");
        printf("6- Mostrar resoluciones judiciales en carpeta.\n");
        printf("7- Mostrar imputados en carpeta.\n");
        printf("8- Volver atras.\n");
        printf("Ingrese una opcion:");
        leerOpcion(&opcion,1,8);
        switch(opcion){

            case 1:
                /*  Mostrar causas*/
                break;

            case 2:
                /*Mostrar denuncias en carpeta*/
                break;

            case 3:
                /*Mostrar declaraciones en carpeta*/
                break;

            case 4:
                /*Mostrar pruebas en carpeta*/
                break;

            case 5:
                /*Mostrar diligencias en carpeta*/
                break;

            case 6:
                /*Mostrar resoluciones judiciales en carpeta*/
                break;
            case 7:
                /*Mostrar imputados en carpeta*/
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

void buscarDatos() {
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

void modificarDatos() {
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
void borrarDatos() {
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

void otrasOpciones() {
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
    siau = (struct SIAU*)malloc(sizeof(struct SIAU));
    int opcion = 0;
    while(opcion != 7){
        limpiarConsola();
        printf("===========================================================\n");
        printf("========Sistema de Informacion y Atencion a Usuarios=======\n");
        printf("===========================================================\n");
        printf("1- Agregar Datos.\n");
        printf("2- Mostrar Datos.\n");
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
                agregarDatos();
                break;

            case 2:
                /*Mostrar*/
                mostrarDatos();
                break;


            case 3:
                /*Buscar*/
                buscarDatos();
                break;

            case 4:
                modificarDatos();
                break;

            case 5:
                borrarDatos();
                break;

            case 6:
                /*Otras opciones*/
                otrasOpciones();
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

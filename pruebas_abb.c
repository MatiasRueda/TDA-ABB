#include "abb.h"
#include "pila.h"
#include "testing.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define LARGO_ARR_VOLUMEN 10000
#define LARGO_CLAVE 10

/* *****************************************************************
 *                     FUNCIONES AUXILIARES                        *
 * *****************************************************************/

static ssize_t buscar(const char* clave, char* claves[], size_t largo)
{
    for (size_t i = 0; i < largo; i++) {
        if (strcmp(clave, claves[i]) == 0) return (ssize_t) i;
    }
    return -1;
}

/// Cambios ligeros al codigo tomado de la siguiente pagina: https://programacion.net/foros/c-c-plus/desordenar_un_vector_63012
void desorden_de_cadenas(char* array[], size_t largo){
    size_t estalibre[largo];
    char* desordenado[largo];
    size_t i,pos;
    time_t t;

    //semilla para el rand
    srand((unsigned) time(&t));

    //inicializacion de los arrays
    for(i=0;i<largo;i++) {
        estalibre[i]=1;//1 -> libre, 0 -> ocupado
    }

    for(i=0;i<largo;i++) {
        pos=rand()%largo;
    //este bucle evita que dos elementos sean guardados en la misma posicion
        while (estalibre[pos]==0)pos=rand()%largo;
            desordenado[pos]=array[i];
            estalibre[pos]=0;
    }

    for (size_t j = 0; j < largo; j++) {
        array[j] = desordenado[j];
    }
}

bool duplicar_dos(const char* clave, void* dato, void* extra) {
    if ( *(int*)extra > 1 ) {
        return false;
    }
    *(int*)dato *= 2;
    *(int*)extra+=1;
    return true;
}

bool guardar_en_pila(const char* clave, void* dato, void* extra) {
    pila_t* pila = (pila_t*)extra;
    pila_apilar(pila, dato);
    return true;
}

void prueba_agregar_al_abb(abb_t* abb_volumen, size_t largo, char* claves[], size_t* valores[], bool debug) {

    bool ok = true;
    for (size_t i = 0; i < largo; i++) {
        claves[i] = malloc(sizeof(char) * LARGO_CLAVE);
        valores[i] = malloc(sizeof(size_t));
        sprintf(claves[i], "%08zu", i);
        *valores[i] = i;
    }

    desorden_de_cadenas(claves, largo);

    for (size_t i = 0; i < largo; i++) {
        ok = abb_guardar(abb_volumen, claves[i], valores[i]);
        if (!ok) break;
    }

    if (debug) print_test("Almaceno muchos elementos", ok);
    if (debug) print_test("La cantidad de elementos es correcta", abb_cantidad(abb_volumen) == largo);

    // Verifica que devuelva los valores correctos
    for (size_t i = 0; i < largo; i++) {
        //ok = abb_pertenece(abb, claves[i]);
        //if (!ok) break;
        //ok = abb_obtener(abb, claves[i]) == valores[i];
        ok = abb_pertenece(abb_volumen, claves[i]) && (abb_obtener(abb_volumen, claves[i]) == valores[i]);
        if (!ok) break;
    }

    if (debug) print_test("el abb pertenece y obtener muchos elementos", ok);
    if (debug) print_test("La cantidad de elementos es correcta", abb_cantidad(abb_volumen) == largo);
}
/* *****************************************************************
 *                        PRUEBAS UNITARIAS                        *  
 * *****************************************************************/

static void prueba_crear_abb_vacio() {
    printf("INICIO PRUEBA: ABB CREAR\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    print_test("Crear abb vacio", abb);
    print_test("La cantidad de elementos es 0", abb_cantidad(abb) == 0);
    print_test("Obtener clave A, es NULL, no existe", !abb_obtener(abb, "A"));
    print_test("Pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
    print_test("Borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));

    abb_destruir(abb);
}

static void prueba_abb_guardar() {
    printf("INICIO PRUEBA: ABB GUARDAR\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    /* Inserta 1 valor y luego lo borra */
    print_test("Guardo la clave1", abb_guardar(abb, clave1, valor1));
    print_test("La cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Obtengo clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Obtengo clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Me fijo de que pertenece clave1, es true", abb_pertenece(abb, clave1));
    print_test("Borro clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    /* Inserta otros 2 valores y no los borra (se destruyen con el abb) */
    print_test("Guardo la clave2", abb_guardar(abb, clave2, valor2));
    print_test("La cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Obtengo clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Obtengo clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Me fijo de que pertenece clave2, es true", abb_pertenece(abb, clave2));

    print_test("Guardo la clave3", abb_guardar(abb, clave3, valor3));
    print_test("La cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Obtengo clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("obtengo clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Me fijo de que pertenece clave3, es true", abb_pertenece(abb, clave3));

    abb_destruir(abb);
}

static void prueba_abb_borrar() {
    printf("INICIO PRUEBA: ABB BORRAR\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    /* Inserta 3 valores y luego los borra */
    print_test("Guardo clave1", abb_guardar(abb, clave1, valor1));
    print_test("Guardo clave2", abb_guardar(abb, clave2, valor2));
    print_test("Guardo clave3", abb_guardar(abb, clave3, valor3));

    /* Al borrar cada elemento comprueba que ya no está pero los otros sí. */
    print_test("Me fijo que pertenece clave3, es verdadero", abb_pertenece(abb, clave3));
    print_test("Borrar clave3, es valor3", abb_borrar(abb, clave3) == valor3);
    print_test("Borrar clave3, es NULL", !abb_borrar(abb, clave3));
    print_test("Me fijo que pertenece clave3, es falso", !abb_pertenece(abb, clave3));
    print_test("Obtener clave3, es NULL", !abb_obtener(abb, clave3));
    print_test("La cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Me fijo que pertenece clave1, es verdadero", abb_pertenece(abb, clave1));
    print_test("Borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Borrar clave1, es NULL", !abb_borrar(abb, clave1));
    print_test("Me fijo que pertenece clave1, es falso", !abb_pertenece(abb, clave1));
    print_test("Obtener clave1, es NULL", !abb_obtener(abb, clave1));
    print_test("La cantidad de elementos es 1", abb_cantidad(abb) == 1);

    print_test("Me fijo que pertenece clave2, es verdadero", abb_pertenece(abb, clave2));
    print_test("Borrar clave2, es valor2", abb_borrar(abb, clave2) == valor2);
    print_test("Borrar clave2, es NULL", !abb_borrar(abb, clave2));
    print_test("Me fijo que pertenece clave2, es falso", !abb_pertenece(abb, clave2));
    print_test("Obtener clave2, es NULL", !abb_obtener(abb, clave2));
    print_test("La cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_borrar_raiz() {
    printf("INICIO PRUEBA: ABB BORRAR RAIZ\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    print_test("Guardo clave1", abb_guardar(abb, clave1, valor1));
    print_test("Guardo clave2", abb_guardar(abb, clave2, valor2));
    print_test("Guardo clave3", abb_guardar(abb, clave3, valor3));

    print_test("Pertenece la clave1", abb_pertenece(abb, clave1));
    print_test("La clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Borrar clave1 es NULL", !abb_borrar(abb, clave1));
    print_test("No pertenece la clave1", !abb_pertenece(abb, clave1));
    print_test("Obtener clave1 es NULL", !abb_obtener(abb, clave1));

    print_test("Pertenece la clave2", abb_pertenece(abb, clave2));
    print_test("La clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Pertenece la clave3", abb_pertenece(abb, clave3));
    print_test("La clave3 es valor3", abb_obtener(abb, clave3) == valor3);

    abb_destruir(abb);

}

static void prueba_abb_clave_vacia() {
    printf("INICIO PRUEBA: CLAVE VACIA\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave = "", *valor = "";

    print_test("Guardo clave vacia", abb_guardar(abb, clave, valor));
    print_test("La cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Verifico que obtener clave vacia es valor", abb_obtener(abb, clave) == valor);
    print_test("Verifico que  pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Borrar clave vacia, es valor", abb_borrar(abb, clave) == valor);
    print_test("La cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_valor_null() {
    printf("INICIO PRUEBA: VALOR NULL\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave = "", *valor = NULL;

    /* Inserta 1 valor y luego lo borra */
    print_test("Guardo la clave vacia valor NULL", abb_guardar(abb, clave, valor));
    print_test("La cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Verifico que obtener la clave vacia es valor NULL", abb_obtener(abb, clave) == valor);
    print_test("Verifico que pertenece la clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Borrar la clave vacia, es valor NULL", abb_borrar(abb, clave) == valor);
    print_test("La cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_reemplazar() {
    printf("INICIO PRUEBA: ABB REEMPLAZAR\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
    char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

    /* Inserta 2 valores y luego los reemplaza */
    print_test("Guardo la clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Obtengo la clave1 y es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Obtengo la clave1 y es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Guardo la clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Obtengo la clave2 y es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Obtengo la clave2 y es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("La cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Guardo la clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Obtengo la clave1 y es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Obtengo la clave1 y es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Guardo la clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Obtengo la clave2 y es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Obtengo la clave2 y es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("La cantidad de elementos es 2", abb_cantidad(abb) == 2);

    abb_destruir(abb);
}

static void prueba_abb_reemplazar_con_destruir() {
    printf("INICIO PRUEBA: ABB REEMPLAZAR UTILIZANDO LA FUNCION FREE\n");
    abb_t* abb = abb_crear(strcmp,free);

    char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;

    /* Pide memoria para 4 valores */
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
    print_test("Guardo la clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Obtengo la clave1 y es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Obtengo la clave1 y es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Guardo la clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Obtengo la clave2 y es valor2b", abb_obtener(abb, clave2) == valor2a);
    print_test("Obtengo la clave2 y es valor2b", abb_obtener(abb, clave2) == valor2a);
    print_test("La cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Guardo clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Obtengo la clave1 y es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Obtengo la clave1 y es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Guardo la clave 2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Obtengo la clave2 y es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Obtengo la clave2 y es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("La cantidad de elementos es 2", abb_cantidad(abb) == 2);

    /* Se destruye el abb (se debe liberar lo que quedó dentro) */
    abb_destruir(abb);
}


static void prueba_abb_volumen(size_t largo, bool debug) {
    printf("INICIO PRUEBA: ABB VOLUMEN\n");
    abb_t* abb_volumen = abb_crear(strcmp, NULL);

    char* claves[largo];
    size_t* valores[largo];

    // Inserta 'largo' parejas en el abb
    bool ok = true; 

    prueba_agregar_al_abb(abb_volumen, largo, claves, valores, debug);

    // Verifica que borre y devuelva los valores correctos
    for (size_t i = 0; i < largo; i++) {
        ok = abb_borrar(abb_volumen, claves[i]) == valores[i];
        if (!ok) break;
    }

    if (debug) print_test("Se pueden borrar muchos elementos", ok);
    if (debug) print_test("La cantidad de elementos es 0", abb_cantidad(abb_volumen) == 0);

    // Destruye el abb y crea uno nuevo que sí libera
    abb_destruir(abb_volumen);
    abb_volumen = abb_crear(strcmp,free);

    // Inserta 'largo' parejas en el abb
    ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = abb_guardar(abb_volumen, claves[i], valores[i]);
        free(claves[i]);
        if (!ok) break;
    }

    // Destruye el abb - debería liberar los enteros
    abb_destruir(abb_volumen);

}

static void prueba_abb_vacio_destruir_con_NULL() {
    printf("INICIO PRUEBA: DESTRUIR ABB VACIO USANDO NULL\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    print_test("Se crea el abb vacio correctamente", abb);
    abb_destruir(abb);
    print_test("Destruyo el abb vacio",true);
}


static void prueba_abb_no_vacio_destruir_con_NULL() {
    printf("INICIO PRUEBA: DESTRUIR ABB NO VACIO USANDO NULL\n");
    abb_t* abb = abb_crear(strcmp, NULL);     
    char* clave1 = "B";
    char* clave2 = "A";
    char* clave3 = "C";
    int valor;

    print_test("Se guarda la clave1",abb_guardar(abb, clave1, &valor));
    print_test("Se guarda la clave2",abb_guardar(abb, clave2, &valor));
    print_test("Se guarda la clave3",abb_guardar(abb, clave3, &valor));

    abb_destruir(abb);
    print_test("Destruyo el abb no vacio",true);
}

static void prueba_abb_vacio_destruir_con_funcion() {
    printf("INICIO PRUEBA: DESTRUIR ABB VACIO CON UNA FUNCION\n");
    abb_t* abb = abb_crear(strcmp, pila_destruir); 

    print_test("Se crea el abb vacio correctamente", abb);

    abb_destruir(abb);
    print_test("Destruyo el abb vacio",true);
}

static void prueba_abb_destruir_con_funcion() {
    printf("INICIO PRUEBA: DESTRUIR ABB VACIO CON UNA FUNCION\n");
    abb_t* abb = abb_crear(strcmp, pila_destruir); 
    char* clave1 = "B";
    char* clave2 = "A";
    char* clave3 = "C";

    pila_t* pila_prueba1 = pila_crear();
    pila_t* pila_prueba2 = pila_crear();
    pila_t* pila_prueba3 = pila_crear();

    print_test("Se guarda la clave1",abb_guardar(abb, clave1, pila_prueba1));
    print_test("Se guarda la clave2",abb_guardar(abb, clave2, pila_prueba2));
    print_test("Se guarda la clave3",abb_guardar(abb, clave3, pila_prueba3));

    abb_destruir(abb);

    print_test("Destruyo el abb no vacio", true);
}


/* *****************************************************************
 *                  PRUEBAS ITERADOR EXTERNO                       *  
 * *****************************************************************/

static void prueba_iterar_externo_abb_vacio() {
    printf("INICIO PRUEBA: ITERAR EN UN ABB VACIO\n");
    abb_t* abb = abb_crear(strcmp,NULL);
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Se puede crear el iterador externo del abb", iter);
    print_test("El iterador del abb esta al final", abb_iter_in_al_final(iter));
    print_test("No se puede avanzar en el iterador vacio", !abb_iter_in_avanzar(iter));
    print_test("El iter_ver_actual es NULL", !abb_iter_in_ver_actual(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}


static void prueba_abb_iterarador_externo() {
    printf("INICIO PRUEBA: ABB ITERADOR EXTERNO\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    char *claves[] = {"perro", "gato", "vaca"};
    char *valores[] = {"guau", "miau", "mu"};

    /* Guardo 3 valores */
    print_test("Guardo la clave1", abb_guardar(abb, claves[0], valores[0]));
    print_test("Guardo la clave2", abb_guardar(abb, claves[1], valores[1]));
    print_test("Guardo la clave3", abb_guardar(abb, claves[2], valores[2]));

    // Prueba de iteración sobre las claves almacenadas.
    abb_iter_t* iter = abb_iter_in_crear(abb);
    const char *clave;
    ssize_t indice;

    print_test("El abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Primer valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("El abb iterador ver actual, es una clave valida", indice != -1);
    print_test("El abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("El abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("El abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Segundo valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("El abb iterador ver actual, es una clave valida", indice != -1);
    print_test("El abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("El abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("El abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Tercer valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("El abb iterador ver actual, es una clave valida", indice != -1);
    print_test("El abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    abb_iter_in_avanzar(iter);
    print_test("El abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    /* Vuelve a tratar de avanzar, por las dudas */
    print_test("El abb iterador ver actual, es NULL", !abb_iter_in_ver_actual(iter));
    print_test("El abb iterador avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("El abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

static void prueba_abb_iterarador_externo_volumen(size_t largo, bool debug) {
    printf("INICIO PRUEBA: ABB ITERADOR EXTERNO VOLUMEN\n");
    abb_t* abb_iter_vol = abb_crear(strcmp, free);

    char* claves[largo];
    size_t* valores[largo];

    prueba_agregar_al_abb(abb_iter_vol, largo, claves, valores, debug);

    // Prueba de iteración sobre las claves almacenadas.
    abb_iter_t* iter = abb_iter_in_crear(abb_iter_vol);
    print_test("El abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    bool ok = true;
    size_t i;
    const char *clave;
    size_t *valor;

    for (i = 0; i < largo; i++) {
        if ( abb_iter_in_al_final(iter) ) {
            ok = false;
            break;
        }
        clave = abb_iter_in_ver_actual(iter);
        if ( clave == NULL ) {
            ok = false;
            break;
        }
        valor = abb_obtener(abb_iter_vol, clave);
        if ( valor == NULL ) {
            ok = false;
            break;
        }
        *valor = largo;
        abb_iter_in_avanzar(iter);
    }
    print_test("Se pudo iterar sobre el abb correctamente", ok);
    print_test("El abb iteración en volumen, recorrio todo el largo", i == largo);
    print_test("El abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    ok = true;
    for (i = 0; i < largo; i++) {
        if ( *valores[i] != largo ) {
            ok = false;
            break;
        }
        free(claves[i]);
    }
    print_test("se cambiaron todo los elementos", ok);

    abb_iter_in_destruir(iter);
    abb_destruir(abb_iter_vol);
}

void static prueba_iterador_in_order(void) {
    printf("INICIO PRUEBA: ITERADOR EXTERNO IN ORDER\n");

    abb_t* abb = abb_crear(strcmp, NULL);
    char* arreglo[] = {"3","6","1","5","4","2"};
    size_t largo_arreglo = 6;
    int valor;
    bool ok = true;

    for (size_t i = 0; i < largo_arreglo; i++){
        ok &= abb_guardar(abb, arreglo[i], &valor);
    }
    print_test("Se guardaron todas las claves correctamente", ok);

    ok = true;

    abb_iter_t* iter = abb_iter_in_crear(abb);

    ok &= (strcmp(abb_iter_in_ver_actual(iter),"1") == 0);
    print_test("Verifico que el actual sea el 1 y avanzo", ok);
    abb_iter_in_avanzar(iter);

    ok &= (strcmp(abb_iter_in_ver_actual(iter),"2") == 0);
    print_test("Verifico que el actual sea el 2 y avanzo", ok);
    abb_iter_in_avanzar(iter);

    ok &= (strcmp(abb_iter_in_ver_actual(iter),"3") == 0);
    print_test("Verifico que el actual sea el 3 y avanzo", ok);
    abb_iter_in_avanzar(iter);

    ok &= (strcmp(abb_iter_in_ver_actual(iter),"4") == 0);
    print_test("Verifico que el actual sea el 4 y avanzo", ok);
    abb_iter_in_avanzar(iter);

    ok &= (strcmp(abb_iter_in_ver_actual(iter),"5") == 0);
    print_test("Verifico que el actual sea el 5 y avanzo", ok);
    abb_iter_in_avanzar(iter);

    ok &= (strcmp(abb_iter_in_ver_actual(iter),"6") == 0);
    print_test("Verifico que el actual sea el 6 y avanzo", ok);
    abb_iter_in_avanzar(iter);

    print_test("Chequeo que estoy al en el final de la iteracion", abb_iter_in_al_final(iter));
    print_test("Las claves salieron todas en orden", ok);

    abb_iter_in_destruir(iter);
    abb_destruir(abb);

}


/*******************************************************************
 *                  PRUEBAS ITERADOR INTERNO                       *  
 *******************************************************************/


static void prueba_invariante_iter_interno() {
    printf("INICIO PRUEBA: INVARIANTE ITERADOR INTERNO\n");
    abb_t* abb = abb_crear(strcmp, NULL);
    char* claves[] = {"F", "D", "J", "E", "B"};
    int valores[] = {3, 1, 4, 2, 0};
    for (int i = 0; i < 5; i++) {
        abb_guardar(abb, claves[i], &valores[i]);
    }
    
    pila_t* pila = pila_crear();
    abb_in_order(abb, guardar_en_pila, pila);
    print_test("Desapilar 4", *(int*)pila_desapilar(pila) == 4);
    print_test("Desapilar 3", *(int*)pila_desapilar(pila) == 3);
    print_test("Desapilar 2", *(int*)pila_desapilar(pila) == 2);
    print_test("Desapilar 1", *(int*)pila_desapilar(pila) == 1);
    print_test("Desapilar 0", *(int*)pila_desapilar(pila) == 0);
    print_test("La pila esta vacia", pila_esta_vacia(pila));

    pila_destruir(pila);
    abb_destruir(abb);
}

static void prueba_iter_interno_corte() {
    printf("INICIO PRUEBA: ITERADOR INTERNO CON CORTE\n");
    abb_t* abb = abb_crear(strcmp, NULL);
    char* claves[] = {"F", "D", "J", "E", "B"};
    int valores[] = {3, 1, 4, 2, 0};
    for (int i = 0; i < 5; i++) {
        abb_guardar(abb, claves[i], &valores[i]);
    }
    int contador = 0;
    abb_in_order(abb, duplicar_dos, &contador);

    print_test("F no se duplico", *(int*)abb_obtener(abb, claves[0]) == 3);
    print_test("D se duplico", *(int*)abb_obtener(abb, claves[1]) == 2);
    print_test("J no se duplico", *(int*)abb_obtener(abb, claves[2]) == 4);
    print_test("E no se duplico", *(int*)abb_obtener(abb, claves[3]) == 2);
    print_test("B se duplico", *(int*)abb_obtener(abb, claves[4]) == 0);

    abb_destruir(abb);
}

static void prueba_iter_interno_abb_vacio() {
    printf("INICIO PRUEBA: ITERAR UN ABB VACIO\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    print_test("Se crea el abb vacio", abb);
    print_test("El abb esta vacio", abb_cantidad(abb) == 0);
    abb_in_order(abb, duplicar_dos, NULL);
    print_test("Iterar el abb vacio no suelta ningun error", true);
    print_test("El abb sigue estando vacio", abb_cantidad(abb) == 0);

    abb_destruir(abb);
    print_test("Destruyo el abb vacio",true);
}

void pruebas_abb_estudiante() {
	prueba_crear_abb_vacio();
	prueba_abb_guardar();
    prueba_abb_borrar();
    prueba_abb_borrar_raiz();
    prueba_abb_clave_vacia();
    prueba_abb_valor_null();
    prueba_abb_reemplazar();
    prueba_abb_reemplazar_con_destruir();
    prueba_abb_volumen(LARGO_ARR_VOLUMEN, true);
    prueba_abb_vacio_destruir_con_NULL();
    prueba_abb_no_vacio_destruir_con_NULL();
    prueba_abb_vacio_destruir_con_funcion();
    prueba_abb_destruir_con_funcion();

    prueba_iterar_externo_abb_vacio();
    prueba_abb_iterarador_externo();
    prueba_abb_iterarador_externo_volumen(LARGO_ARR_VOLUMEN, true);
    prueba_iterador_in_order();

    prueba_invariante_iter_interno();
    prueba_iter_interno_corte();
    prueba_iter_interno_abb_vacio();
}

#ifndef CORRECTOR

int main(void) {
    pruebas_abb_estudiante();

    return 0;
}

#endif
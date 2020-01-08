#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "list.h"
#include "Map.h"

typedef linea Linea;
typedef parada Parada;

struct linea
{
    char* hora_inicio;
    int numero;
    int frecuencia;
    List* paradas;
};

struct parada
{
    char* nombre;
    List* lineas;
};

void printmenu()
{
    printf("---------------------MENU-----------------\n");
    printf("------------------------------------------\n");
    /*
        Planificar viaje:
        El usuario ingresa un origen y un destino, informacion con
        la cual la app calculara las combinaciones mas eficientes para
        llegar al destino.
    */
    printf("1. Planificar un viaje.\n");
    /*
        Mostrar lineas de buses por parada:
        Se el numero de las lineas que paran en aquella parada.

    */
    printf("2. Mostrar lineas de buses por parada.\n");
    /*
        Mostrar paradas de una linea:
        Se muestran los nombres de cada parada que tiene cada linea.
        Nombres que deberian describir la ubicacion de esta.
    */
    printf("3. Mostrar paradas de una linea de buses.\n");
    printf("4. Mostrar horarios de una linea de buses.\n");
    printf("5. Mostrar horarios de una parada.\n");
    printf("6. Salir.\n");
    printf("------------------------------------------\n");
    printf("Ingrese una opcion:\n");
}

char nextchar()
{
    char temp;
    scanf("%c", &temp);
    while(iscntrl(temp))
    {
        scanf("%c", &temp);
    }
    return temp;
}

long long stringHash(const void * key) {
    long long hash = 5381;

    const char * ptr;

    for (ptr = key; *ptr != '\0'; ptr++) {
        hash = ((hash << 5) + hash) + tolower(*ptr); /* hash * 33 + c */
    }

    return hash;
}

int stringEqual(const void * key1, const void * key2) {
    const char * A = key1;
    const char * B = key2;

    return strcmp(A, B) == 0;
}

int main()
{
    char op;

    Map* horarios_x_linea = createMap(stringHash, stringEqual);
    Map* horarios_x_parada = createMap(stringHash, stringEqual);

    printf("Bienvenido a Busapp!\n");
    printmenu();
    op = nextchar();

    while(op != '6')
    {
        switch(op)
        {
            case '1':
                break;
            case '2':
                break;
            case '3':
                break;
            case '4':
                break;
            case '5':
                break;
            default:
                printf("Opcion Incorrecta.\n");
        }
        printmenu();
        op = nextchar();
    }
    return 0;
}

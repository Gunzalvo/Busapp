#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "list.h"
#include "Map.h"

const int MAX_CARACTERES = 1024;
typedef struct linea Linea;
typedef struct parada Parada;

struct linea
{
    char* hora_inicio;
    int numero;
    list* paradas;
};

struct parada
{
    char* nombre;
    int proxima;
    list* lineas;
};

char * _strdup (const char *s) {
    size_t len = strlen (s) + 1;
    void *new = malloc (len);

    if (new == NULL)
        return NULL;

    return (char *) memcpy (new, s, len);
}

const char *get_csv_field (char * tmp, int i) {
    //se crea una copia del string tmp
    char * line = _strdup (tmp);
    const char * tok;
    for (tok = strtok (line, ","); tok && *tok; tok = strtok (NULL, ",\n")) {
        if (!--i) {
            return tok;
        }
    }
    return NULL;
}

void printmenu()
{
    printf("---------------------MENU-----------------\n");
    printf("------------------------------------------\n");
    printf("1. Planificar un viaje.\n");
    printf("2. Mostrar lineas de buses por parada.\n");
    printf("3. Mostrar paradas de una linea de buses.\n");
    printf("4. Mostrar horarios de una linea de buses.\n");
    printf("5. Mostrar horarios de una parada.\n");
    printf("6. Salir.\n");
    printf("------------------------------------------\n");
    printf("Ingrese una opcion:\n");
}

/* Utilidad para obtener el proximo caracter de la consola */
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

int obtenerLineas(Map* lineas)
{
    FILE* archivo_lineas = fopen("lineas.csv", "r");
    char* linea[MAX_CARACTERES];

    if(archivo_lineas != NULL)
    {
        while( fgets(linea, MAX_CARACTERES, archivo_lineas) )
        {
            char* numero = _strdup(get_csv_field(linea, 1));
            char* inicio = _strdup(get_csv_field(linea, 2));

            Linea* existe = (Linea*) searchMap(lineas, numero);
            if( existe == NULL )
            {
                Linea* nueva = (Linea*) malloc( sizeof(Linea) );
                nueva->hora_inicio = inicio;
                nueva->numero = atoi(numero);
                nueva->paradas = list_create_empty();

                insertMap(lineas, numero, nueva);
            }
        }
        return 1;
    }
    return 0;
}

int obtenerParadas(Map* paradas, Map* lineas)
{
    FILE* archivo_paradas = fopen("paradas.csv", "r");
    char* linea[MAX_CARACTERES];

    if(archivo_paradas != NULL)
    {
        while( fgets(linea, MAX_CARACTERES, archivo_paradas) )
        {
            char* numero = _strdup(get_csv_field(linea, 1));
            char* nombre = _strdup(get_csv_field(linea, 2));
            char* proxima = _strdup(get_csv_field(linea, 3));

            Linea* existe_linea = (Linea*) searchMap(lineas, numero);
            Parada* existe_parada = (Parada*) searchMap(paradas, nombre);

            if( (existe_linea != NULL) && (existe_parada == NULL) )
            {
                Parada* nueva = (Parada*) malloc( sizeof(Parada) );
                nueva->nombre = nombre;
                nueva->proxima = atoi(proxima);
                nueva->lineas = list_create_empty();

                list_push_back(existe_linea->paradas, nueva);
                list_push_back(nueva->lineas, existe_linea);
                insertMap(paradas, nombre, nueva);
            }
        }
        return 1;
    }
    return 0;
}

int main()
{
    char op;
    Map* lineas = createMap(stringHash, stringEqual);
    Map* paradas = createMap(stringHash, stringEqual);

    printf("Cargando base de datos de lineas...\n");
    if( obtenerLineas(lineas) )
    {
        printf("Lineas cargadas con exito.\n");
    }
    else
    {
        printf("No se pudo cargar las lineas (Archivo lineas.csv no existe).\n");
        return 1;
    }


    printf("Cargando base de datos de paradas...\n");
    if( obtenerParadas(paradas, lineas) )
    {
        printf("Paradas cargadas con exito.\n");
    }
    else
    {
        printf("No se pudo cargar las paradas (Archivo paradas.csv no existe).\n");
        return 1;
    }

    printf("Bienvenido a Busapp!\n");
    printmenu();
    op = nextchar();

    while(op != '6')
    {
        char* input[MAX_CARACTERES];
        Linea* linea;
        switch(op)
        {
            case '1':
                break;
            case '2':
                break;
            case '3':
                scanf("%c", &op);
                printf("Ingrese el numero de la linea:\n");
                fgets(input, MAX_CARACTERES, stdin);

                linea = (Linea*) searchMap(lineas, input);
                if( linea != NULL )
                {
                    Parada* parada = list_first(linea->paradas);
                    while( parada )
                    {
                        printf("%s\n", parada->nombre );
                        parada = (Parada*) list_next(linea->paradas);
                    }
                }
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

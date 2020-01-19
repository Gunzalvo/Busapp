#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "list.h"
#include "Map.h"

const int MAX_CARACTERES = 1024;
typedef struct linea Linea;
typedef struct parada Parada;

struct linea
{
    int hora_inicio;
    int numero;
    list* paradas;
};

struct parada
{
    char* nombre;
    int proxima;
    list* lineas;
};

int obtenerLineas(Map* lineas);
int obtenerParadas(Map* paradas, Map* lineas);
void planificar(char* origen, char* destino, Map* lineas, Map* paradas);
void mostrarLineas(char* parada, Map* paradas);
void mostrarParadas(char* linea, Map* lineas);
void mostrarHorariosLinea(char* linea, Map* lineas);
void mostrarHorariosParada(char* parada, Map* paradas);

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

int menuPrincipal(){
    int opcion = 0;
    do {
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
        scanf("%d", &opcion);
        getchar();
    } while (opcion < 1 || opcion > 6);
    return opcion;
}

int main()
{
    /* Se definen las variables globales */
    int op;
    Map* lineas = createMap(stringHash, stringEqual);
    Map* paradas = createMap(stringHash, stringEqual);

    /* Se ingresan los datos de las linea y paradas desde los archivos .csv */
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

    /* Se muestra el menu principal */
    printf("Bienvenido a Busapp!\n");
    /* Ciclo principal de la aplicacion */
    while(1){
        char* input_1[MAX_CARACTERES];
        char* input_2[MAX_CARACTERES];
        Linea* linea;

        /* Se ejecuta la opcion ingresada */
        switch(menuPrincipal()){
            case 1:
                printf("Ingrese lugar de partida:\n");
                scanf("%[^\n]s", input_1);
                getchar();

                printf("Ingrese lugar de destino:\n");
                scanf("%[^\n]s", input_2);
                getchar();

                planificar(input_1, input_2, lineas, paradas);
                break;
            case 2:
                scanf("%c", &op);
                printf("Ingrese nombre de la parada:\n");
                scanf("%s", &input_1);

                mostrarLineas(input_1, paradas);
                break;
            case 3:
                scanf("%c", &op);
                printf("Ingrese el numero de la linea:\n");
                scanf("%s", &input_1);
                mostrarParadas(input_1, lineas);
                break;
            case 4:
                scanf("%c", &op);
                printf("Ingrese el numero de la linea:\n");
                scanf("%s", &input_1);

                mostrarHorariosLinea(input_1, lineas);
                break;
            case 5:
                scanf("%c", &op);
                printf("Ingrese nombre de la parada:\n");
                scanf("%s", &input_1);

                mostrarHorariosParada(input_1, paradas);
                break;
            case 6:
                return;
                break;
            case 7:
                linea = (Linea*) firstMap(lineas);
                while(linea)
                {
                    printf("%d\n", linea->numero);
                    linea = (Linea*) nextMap(lineas);
                }
                break;
            default:
                printf("Opcion Incorrecta.\n");
        }
    }
    return 0;
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
                nueva->hora_inicio = atoi(inicio);
                nueva->numero = atoi(numero);
                nueva->paradas = list_create_empty();

                insertMap(lineas, numero, nueva);
            }
        }
        fclose(archivo_lineas);
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
            else if( existe_linea != NULL )
            {
                list_push_back(existe_linea->paradas, existe_parada);
                list_push_back(existe_parada->lineas, existe_linea);
                insertMap(paradas, nombre, existe_parada);
            }
        }
        fclose(archivo_paradas);
        return 1;
    }
    return 0;
}

void printTiempo(float unixT)
{
    float fraccion_hora = unixT / 3600;
    int hora = unixT / 3600;
    int minutos = round( (fraccion_hora - hora) * 60 );

    printf("%02d:%02d", hora, minutos);
}

void planificar(char* origen, char* destino, Map* lineas, Map* paradas)
{
    Parada *ori , *dest;
    if(searchMap(paradas, origen) != NULL)
    {
        if(searchMap(paradas, destino) != NULL)
        {
            ori = searchMap(paradas , origen);
            dest = searchMap(paradas, destino);
            list *aux = list_create_empty();

            list_first(ori->lineas);
            while(list_next(ori->lineas) != NULL )
            {
                list_first(dest->lineas);
                while(list_next(dest->lineas) != NULL)
                {
                    if(ori->lineas == dest->lineas)
                    {
                        list_push_back(aux, ori->lineas);
                        printf("%p", aux);
                        break;
                    }
                }
            }
        }else{
            printf("Parada destino no encontrada\n");
            return;
        }

    }else{
        printf("Parada origen no encontrada\n");
        return;
    }
}

void mostrarLineas(char* inputParada, Map* paradas)
{
    Parada* parada = (Parada*) searchMap(paradas, inputParada);

    printf("\n######## Lineas que se detienen en %s ########\n", parada->nombre);
    if( parada != NULL )
    {
        Linea* linea = (Linea*) list_first(parada->lineas);
        while( linea )
        {
            printf("- %d\n", linea->numero);
            linea =(Linea*) list_next(parada->lineas);
        }
        printf("\n");
    }
    else
    {
        printf("No se encontró la parada.\n");
    }
}

void mostrarParadas(char* inputLinea, Map* lineas)
{
    Linea* linea = (Linea*) searchMap(lineas, inputLinea);

    printf("\n######## Paradas Linea %d ########\n", linea->numero);
    if( linea != NULL )
    {
        Parada* parada = list_first(linea->paradas);
        int i = 1;
        while( parada )
        {
            printf("%d. %s\n", i, parada->nombre );
            parada = (Parada*) list_next(linea->paradas);
            i++;
        }
        printf("\n");
    }
    else
    {
        printf("No se encontró el número de línea especificado.\n");
    }
}

void mostrarHorariosLinea(char* inputLinea, Map* lineas)
{
    Linea* linea = (Linea*) searchMap(lineas, inputLinea);
    if( linea != NULL )
    {
        Parada* parada = (Parada*) list_first(linea->paradas);
        float actual = (float) linea->hora_inicio;

        printf("\n######## Horarios Linea %d ########\n", linea->numero);
        while( parada )
        {
            printTiempo(actual);
            printf(" - %s\n", parada->nombre);
            actual += parada->proxima * 60;
            parada = (Parada*) list_next(linea->paradas);
        }
        printf("\n");
    }
    else
    {
        printf("No se encontró el número de línea especificado.\n");
    }
}

void mostrarHorariosParada(char* inputParada, Map* paradas)
{
    Parada* parada = (Parada*) searchMap(paradas, inputParada);
    if(parada != NULL)
    {
        Linea* linea = (Linea*) list_first(parada->lineas);
        printf("\n######## Horarios de Parada %s ########\n", parada->nombre);
        while( linea )
        {
            float tiempo = (float) linea->hora_inicio;

            Parada* linea_p = (Parada*) list_first(linea->paradas);
            while( linea_p )
            {
                if( strcmp(linea_p->nombre, inputParada) == 0 )
                {
                    printTiempo(tiempo);
                    printf(" - %d\n", linea->numero);
                    break;
                }
                tiempo += linea_p->proxima * 60;
                linea_p = (Parada*) list_next(linea->paradas);
            }
            linea = (Linea*) list_next(parada->lineas);
        }
        printf("\n");
    }
    else
    {
        printf("No se encontro la parada.\n");
    }
}

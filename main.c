#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "list.h"
#include "Map.h"

const int MAX_CARACTERES = 1024;
typedef struct linea Linea;
typedef struct parada Parada;

struct linea
{
    long hora_inicio;
    int numero;
    list* intersecciones;
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

const char *get_csv_field (char * tmp, int i)
{
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

long long stringHash(const void * key)
{
    long long hash = 5381;

    const char * ptr;

    for (ptr = key; *ptr != '\0'; ptr++)
    {
        hash = ((hash << 5) + hash) + tolower(*ptr); /* hash * 33 + c */
    }

    return hash;
}

int stringEqual(const void * key1, const void * key2)
{
    const char * A = key1;
    const char * B = key2;

    return strcmp(A, B) == 0;
}

int menuPrincipal()
{
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
    } while (opcion < 1 || opcion > 7);
    return opcion;
}

int main()
{
    /* Se definen las variables globales */
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
    while(1)
    {
        char* input_1[MAX_CARACTERES];
        char* input_2[MAX_CARACTERES];

        /* Se ejecuta la opcion ingresada */
        switch( menuPrincipal() )
        {
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
                printf("Ingrese nombre de la parada:\n");
                scanf("%[^\n]s", input_1);
                getchar();

                mostrarLineas(input_1, paradas);
                break;
            case 3:
                printf("Ingrese el numero de la linea:\n");
                scanf("%s", &input_1);

                mostrarParadas(input_1, lineas);
                break;
            case 4:
                printf("Ingrese el numero de la linea:\n");
                scanf("%s", &input_1);

                mostrarHorariosLinea(input_1, lineas);
                break;
            case 5:
                printf("Ingrese nombre de la parada:\n");
                scanf("%[^\n]s", input_1);
                getchar();

                mostrarHorariosParada(input_1, paradas);
                break;
            case 6:
                return 0;
                break;
            default:
                printf("Opcion Incorrecta.\n");
        }
    }
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
                nueva->intersecciones = list_create_empty();

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

            Parada* nueva = (Parada*) malloc( sizeof(Parada) );
            nueva->nombre = nombre;
            nueva->proxima = atoi(proxima);
            nueva->lineas = list_create_empty();

            /* Si la linea de buses existe en el csv de lineas */
            if( existe_linea != NULL )
            {
                list_push_back(existe_linea->paradas, nueva);

                if( existe_parada != NULL )
                {
                    // si existe la parada, entonces agregamos la linea
                    list_push_back(existe_parada->lineas, existe_linea);

                    /*
                        Buscamos las intersecciones que se generan al insertar esta
                        nueva linea a la parada existente.
                    */
                    Linea* linea = (Linea*) list_first(existe_parada->lineas);
                    while( linea )
                    {
                        if( linea->numero != existe_linea->numero )
                        {
                            Linea* intersectante = (Linea*) list_first(linea->intersecciones);
                            while( intersectante )
                            {
                                if( intersectante->numero == existe_linea->numero )
                                    break;
                                intersectante = (Linea*) list_next(linea->intersecciones);
                            }

                            if( intersectante == NULL )
                            {
                                list_push_back(linea->intersecciones, existe_linea);
                                list_push_back(existe_linea->intersecciones, linea);
                            }
                        }
                        linea = (Linea*) list_next(existe_parada->lineas);
                    }
                }
                /*
                    Si no existe la parada agregamos una referencia de esta
                    al mapa (por eso la próxima se pone a 0, nunca se va a utilizar
                    ese valor ya que si se busca esta parada en el mapa es para
                    obtener las líneas de esta parada).
                */
                else
                {
                    Parada* referencia = (Parada*) malloc( sizeof(Parada) );
                    referencia->nombre = nombre;
                    referencia->proxima = 0;
                    referencia->lineas = list_create_empty();
                    list_push_back(referencia->lineas, existe_linea);
                    insertMap(paradas, nombre, referencia);
                }
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

void planificar(char* nombre_origen, char* nombre_destino, Map* lineas, Map* paradas)
{
    Parada *origen = searchMap(paradas, nombre_origen);
    Parada *destino = searchMap(paradas, nombre_destino);

    if( origen != NULL )
    {
        if( destino != NULL )
        {
            Parada* interseccion;
            int tiempo_minimo = 0;
            int linea_inicio = 0;
            int linea = 0;

            // Recorremos las lineas de la parada de origen (que son siempre pocas).
            Linea* linea_partida = (Linea*) list_first(origen->lineas);
            while( linea_partida )
            {
                /*
                    Por cada linea de origen, recorremos las lineas de la parada de destino
                    para comprobar si se puede llegar a destino en un solo bus.
                */
                Linea* linea_destino = (Linea*) list_first(destino->lineas);
                while( linea_destino )
                {
                    /*
                        Si la linea de origen está en la parada de destino entonces se
                        puede llegar a destino en un solo bus y se calcula su tiempo.
                    */
                    if( linea_destino->numero == linea_partida->numero )
                    {
                        Parada* parada = (Parada*) list_first(linea_partida->paradas);
                        int tiempo = 0;
                        int direccion;
                        char* objetivo;

                        // Buscamos el sentido del bus (y a lo mejor de la vida uwu).
                        while( parada )
                        {
                            if( strcmp(parada->nombre, origen->nombre) == 0)
                            {
                                objetivo = destino->nombre;
                                break;
                            }

                            if( strcmp(parada->nombre, destino->nombre) == 0)
                            {
                                objetivo = origen->nombre;
                                break;
                            }
                            parada = (Parada*) list_next(linea_partida->paradas);
                        }

                        // Calculamos el tiempo hasta destino.
                        while( strcmp(parada->nombre, objetivo) != 0 )
                        {
                            tiempo += parada->proxima * 60;
                            parada = (Parada*) list_next(linea_partida->paradas);
                        }

                        /*
                            Si encontramos un tiempo menor o todavia no se ha
                            determinado un tiempo minimo lo establecemos.
                        */
                        if( (tiempo < tiempo_minimo) || (tiempo_minimo == 0) )
                        {
                            tiempo_minimo = tiempo;
                            linea = linea_destino->numero;
                        }
                        break;
                    }
                    linea_destino = (Linea*) list_next(destino->lineas);
                }
                linea_partida = (Linea*) list_next(origen->lineas);
            }

            // Si el tiempo minimo tiene un valor mayor a cero, se encontró una linea directa.
            if( tiempo_minimo != 0 && linea != 0 )
            {
                printf("\n========== Planificacion del Viaje ============\n");
                printf("Linea: %d, Tiempo Estimado: ", linea);
                printTiempo(tiempo_minimo);
                printf(":00\n\n");
                return;
            }

            /*
                Si no se encontro una linea, hay que hacer una combinación, asi que
                comenzamos por la primera línea que pasa por el origen.
            */
            linea_partida = (Linea*) list_first(origen->lineas);
            while( linea_partida )
            {
                Linea* inter_origen = (Linea*) list_first(linea_partida->intersecciones);
                while( inter_origen )
                {
                    Linea* linea_destino = (Linea*) list_first(destino->lineas);
                    while( linea_destino )
                    {
                        if( inter_origen->numero == linea_destino->numero )
                        {
                            long tiempo1 = 0;
                            long tiempo2 = 0;

                            Parada* inter = (Parada*) list_first( linea_partida->paradas );
                            Parada* parada = (Parada*) searchMap(paradas, inter->nombre);
                            while( parada )
                            {
                                Linea* linea_parada = (Linea*) list_first(parada->lineas);
                                while( linea_parada != NULL )
                                {
                                    if( linea_parada->numero == linea_destino->numero )
                                        break;
                                    linea_parada = (Linea*) list_next(parada->lineas);
                                }
                                if (linea_parada != NULL) break;
                                inter = (Parada*) list_next( linea_partida->paradas );
                                parada = (Parada*) searchMap(paradas, inter->nombre);
                            }

                            char* objetivo;
                            parada = (Parada*) list_first( linea_partida->paradas );
                            while( parada )
                            {
                                if( strcmp(parada->nombre, inter->nombre) == 0 )
                                {
                                    objetivo = origen->nombre;
                                    break;
                                }
                                if( strcmp(parada->nombre, origen->nombre) == 0)
                                {
                                    objetivo = inter->nombre;
                                    break;
                                }
                                parada = (Parada*) list_next( linea_partida->paradas );
                            }

                            while( strcmp(parada->nombre, objetivo) != 0 )
                            {
                                tiempo1 += parada->proxima * 60;
                                parada = (Parada*) list_next( linea_partida->paradas );
                            }

                            parada = (Parada*) list_first( linea_destino->paradas );
                            while( parada )
                            {
                                if( strcmp(parada->nombre, inter->nombre) == 0 )
                                {
                                    objetivo = destino->nombre;
                                    break;
                                }
                                if( strcmp(parada->nombre, destino->nombre) == 0)
                                {
                                    objetivo = inter->nombre;
                                    break;
                                }
                                parada = (Parada*) list_next( linea_destino->paradas );
                            }

                            while( strcmp(parada->nombre, objetivo) != 0 )
                            {
                                tiempo2 += parada->proxima * 60;
                                parada = (Parada*) list_next( linea_destino->paradas );
                            }

                            if( (tiempo_minimo > (tiempo1+tiempo2)) || (tiempo_minimo == 0))
                            {
                                interseccion = inter;
                                tiempo_minimo = tiempo1+tiempo2;
                                linea_inicio = linea_partida->numero;
                                linea = linea_destino->numero;
                            }
                        }
                        linea_destino = (Linea*) list_next(destino->lineas);
                    }
                    inter_origen = (Linea*) list_next(linea_partida->intersecciones);
                }
                linea_partida = (Linea*) list_next(origen->lineas);
            }

            printf("\n========== Planificacion del Viaje ============\n");
            printf("1 - Tomar %d en '%s' hasta '%s'.\n", linea_inicio, origen->nombre, interseccion->nombre);
            printf("2 - Luego combinar con %d hasta %s.\nTiempo Total Estimado: ", linea, destino->nombre);
            printTiempo(tiempo_minimo);
            printf(":00.\nSuerte!.\n===============================================\n");
        }
        else
        {
            printf("Parada destino no encontrada.\n");
            return;
        }
    }
    else
    {
        printf("Parada origen no encontrada.\n");
        return;
    }
}

void mostrarLineas(char* inputParada, Map* paradas)
{
    Parada* parada = (Parada*) searchMap(paradas, inputParada);

    if( parada != NULL )
    {
        printf("\n######## Lineas que se detienen en %s ########\n", parada->nombre);
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
        printf("No se encontro la parada.\n");
    }
}

void mostrarParadas(char* inputLinea, Map* lineas)
{
    Linea* linea = (Linea*) searchMap(lineas, inputLinea);

    if( linea != NULL )
    {
        printf("\n######## Paradas Linea %d ########\n", linea->numero);
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
        printf("No se encontro el numero de linea especificado.\n");
    }
}

void mostrarHorariosLinea(char* inputLinea, Map* lineas)
{
    Linea* linea = (Linea*) searchMap(lineas, inputLinea);
    if( linea != NULL )
    {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        long tiempo = linea->hora_inicio;
        long actual = tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec;

        /*
            Para contextualizar los horarios respecto a la hora en la que
            el usuario está preguntando, suponemos que el usuario está
            a un poco menos de la mitad del recorrido.
        */
        long t_hasta_mitad = 0;
        int mitad_viaje = list_size(linea->paradas) * 0.2;

        Parada* parada = (Parada*) list_first(linea->paradas);
        for (int i = 0; i < mitad_viaje; i++)
        {
            t_hasta_mitad += parada->proxima * 60;
            parada = (Parada*) list_next(linea->paradas);
        }

        /*
           Si el usuario pregunta por los horarios antes del inicio
           del servicio del bus, solo mostramos los horarios desde cuando parten.
        */
        if( (actual - t_hasta_mitad) > tiempo )
        {
            /*
               Desfase entre la salida del bus y el tiempo de partida ideal
               (tiempo en el que el bus llegaria justo a la hora actual).
               La frecuencia de salida de los buses es 15 min, osea 900 seg.
            */
            long desfase = actual - t_hasta_mitad - tiempo;
            while( desfase <= -900 || desfase > 0)
            {
                tiempo += 900;
                desfase = actual - t_hasta_mitad - tiempo;
            }
        }

        parada = (Parada*) list_first(linea->paradas);
        printf("\n######## Horarios Linea %d ########\n", linea->numero);
        while( parada )
        {
            printTiempo(tiempo);
            printf(" - %s\n", parada->nombre);
            tiempo += parada->proxima * 60;
            if( tiempo >= 86400 )
                tiempo -= 86400;
            parada = (Parada*) list_next(linea->paradas);
        }
        printf("\n");
    }
    else
    {
        printf("No se encontro el numero de linea especificado.\n");
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
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);

            long tiempo_inicio = linea->hora_inicio;
            long actual = tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec;

            long tiempo_hasta_parada = 0;

            /*
                Calculamos el tiempo que se demora el bus en llegar hasta
                la parada desde el terminal.
            */
            Parada* parada_linea = (Parada*) list_first(linea->paradas);
            while( parada_linea )
            {
                if( strcmp(parada_linea->nombre, inputParada) == 0 )
                    break;

                tiempo_hasta_parada += parada_linea->proxima * 60;
                parada_linea = (Parada*) list_next(linea->paradas);
            }

            if( (actual - tiempo_hasta_parada) > tiempo_inicio )
            {
                /*
                    Un desfase negativo significa que el bus llegara en
                    el futuro (no podemos retroceder el tiempo para tomar
                    el bus anterior xd).
                */
                long desfase = actual - tiempo_hasta_parada - tiempo_inicio;
                while( desfase <= -900 || desfase > 0)
                {
                    // Movemos el tiempo de inicio del bus
                    tiempo_inicio += 900;
                    desfase = actual - tiempo_hasta_parada - tiempo_inicio;
                }
            }

            printTiempo((float) (tiempo_inicio + tiempo_hasta_parada));
            printf(" - %d\n", linea->numero);

            linea = (Linea*) list_next(parada->lineas);
        }
        printf("\n");
    }
    else
    {
        printf("No se encontro la parada.\n");
    }
}

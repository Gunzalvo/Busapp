/**
 MIT License

 Copyright (c) 2018 Matias Barrientos.

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#ifndef Map_h
#define Map_h

typedef struct Map Map;

/**
 @typedef
 Prototipo de la funcion hash.

 @param key Llave para poder obtener el valor del hash.
 @return El valor hash de la llave.
 */
typedef long long (* MapHashCallBack)(const void * key);

/**
 @typedef
 Prototipo de funcion Equal, la cual comparar dos llaves para saber si son iguales.

 @param key1 Primera llave.
 @param key2 Segunda llave.
 @return Retorna 1 si las dos llaves son iguales; 0 lo contrario.
 */
typedef int (* MapEqualCallBack)(const void * key1, const void * key2);

/**
 Crea un nuevo puntero de tipo Map.

 @param hash Funcion hash.
 @param equal Funcion equal.
 @return Puntero al nuevo Map creado.
 */
Map * createMap(MapHashCallBack hash, MapEqualCallBack equal);

/**
 Inserta un nuevo elemento en el Map.

 Esta función tiene como complejidad:

 - Mejor caso: O(1)

 - Peor caso: O(n)

 @param map Puntero al Map.
 @param key Puntero a la llave del elemento que se va a insertar.
 @param value Puntero al valor que se va a insertar.
 */
void insertMap(Map * map, const void * key, const void * value);

/**
 Retorna la cantidad de elementos no nulos del Map.

 Complejidad: O(1)

 @param map Puntero al Map.
 @return La cantidad de elemntos del Map.
 */
long mapCount(Map * map);

/**
 Prueba si el Map está vacio.

 Complejidad: O(1)

 @param map Puntero al Map.
 @return 1 (true) si y solo si el Map no contiene elementos; 0 (false) lo contrario.
 */
int emptyMap(Map * map);

/**
 Elimina un elemento con la llave del Map, pero no lo libera de la memoria.

 Esta función tiene como complejidad:

 - Mejor caso: O(1)

 - Peor caso: O(n)

 @param map Puntero al Map.
 @param key Llave del elemento a eliminar.
 @return Puntero al dato eliminado del Map.
 */
void * eraseKeyMap(Map * map, const void * key);

/**
 Busca un elemento en el Map con la llave.

 Esta función tiene como complejidad:

 - Mejor caso: O(1)

 - Peor caso: O(n)

 @param map Puntero al Map.
 @param key Llave del elemento a buscar.
 @return Puntero al dato encontrado del Map.
 */
void * searchMap(Map * map, const void * key);

/**
 Retorna el primero elemento no nulo del Map.

 Complejidad: O(n)

 @param map Puntero al Map.
 @return Puntero al primero elemento encontrado del Map.
 */
void * firstMap(Map * map);

/**
 Retorna el siguiente elemento no nulo del Map.

 Complejidad: O(n)

 @param map Puntero al Map.
 @return Puntero al siguiente elemento encontrado del Map.
 */
void * nextMap(Map * map);

/**
 Elimina todos los elementos del Map, pero sin liberar la memoria de los datos contentidos.

 Complejidad: O(n)

 @param map Puntero al Map.
 */
void removeAllMap(Map * map);

#endif /* Map_h */


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

#ifndef list_h
#define list_h

typedef struct list list;

/**
 @typedef
 Prototipo de funcion para eliminar de memoria el objeto que se guarda dentro de la lista.

 @param object Puntero al objecto guardado.
 */
typedef void (*list_release_object_callback)(void *object);

/**
 Crea un nuevo puntero de tipo List.

 @return Puntero a la nueva List.
 */
extern list *list_create (list_release_object_callback release);

/**
 Crea un nuevo puntero de tipo List.

 @return Puntero a la nueva List.
 */
extern list *list_create_empty (void);

/**
 Devuele el primer dato de la List.

 Complejidad: O(1)

 @param list Puntero a la List.
 @return Puntero al dato contenido en la cabeza.
 */
extern void *list_first (list *list);

/**
 Devuele el siguiente dato de la List.

 Complejidad: O(1)

 @param list Puntero a la List.
 @return Puntero al dato.

 Ejemplo:

 @code
 void *data = list_first(list);

 while (data != NULL) {
 // Aqui puede manipular el dato o imprimirlo.
 data = list_next(list);
 }
 */
extern void *list_next (list *list);

/**
 Devuelve el ultimo dato de la List.

 Complejidad: O(1)

 @param list Puntero a la List.
 @return Puntero al dato.
 */
extern void *list_last (list *list);

/**
 Devuele el anterior dato de la List.

 Complejidad: O(1)

 @param list Puntero a la List.
 @return Puntero al dato.

 @code
 void *data = list_last(list);

 while (data != NULL) {
 // Aqui puede manipular el dato o imprimirlo.
 data = list_prev(list);
 }
 */
extern void *list_prev (list *list);

/**
 Cantidad de elementos en la List.

 Complejidad: O(1)

 @param list Puntero a la List.
 @return Cantidad de elementos en la List.
 */
extern long list_size (list *list);

/**
 Prueba si la List está vacía.

 Complejidad: O(1)

 @param list Puntero a la List.
 @return 1 (true) si y solo si la List no contiene elementos; 0 (false) lo contrario.
 */
extern int list_empty (list *list);

/**
 Inserta un elemento en al inicio de la List.

 Complejidad: O(1)

 @param list Puntero a la List.
 @param data Puntero al dato que se quiere guardar.
 */
extern void list_push_front (list *list, const void *data);

/**
 Inserta un elemento al final de la lista.

 Complejidad: O(1)

 @param list Puntero a la List.
 @param data Puntero al dato que se quiere guardar.
 */
extern void list_push_back (list *list, const void *data);

/**
 Inserta un elemento despues del current. El current se actualiza cuando se reccore la lista con las funciones firstList, etc.

 Complejidad: O(1)

 @param list Puntero a la List.
 @param data Puntero al dato que se quiere guardar.
 */
extern void list_push_current (list *list, const void *data);

/**
 Elimina el nodo del inicio de la lista,
 si esta activada la funcion release, esta eliminara
 de la memoria el puntero que apunta hacia el objeto.

 Complejidad: O(1)

 @param list Puntero a la List.
 @return Puntero del dato eliminado.
 */
extern void *list_pop_front (list *list);

/**
 Elimina el nodo final de la lista,
 si esta activada la funcion release, esta eliminara
 de la memoria el puntero que apunta hacia el objeto.

 Complejidad: O(1)

 @param list Puntero a la List.
 @return Puntero del dato eliminado.
 */
extern void *list_pop_back (list *list);

/**
 Elimina el nodo en donde se encuentre el current,
 actualiza el current al siguiente de este.
 Si esta activada la funcion release, esta eliminara
 de la memoria el puntero que apunta hacia el objeto.

 Complejidad: O(1)

 @param list Puntero a la List.
 @return Puntero del dato eliminado.
 */
extern void *list_pop_current (list *list);

/**
 Elimina todos los nodos de la lista,
 si esta activada la funcion release, esta eliminara
 de la memoria el puntero que apunta hacia el objeto.

 Complejidad: O(n)

 @param list Puntero a la List.
 */
extern void list_release (list **list);

#endif /* list_h */

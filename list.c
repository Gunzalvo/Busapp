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

#include "list.h"
#include <stdlib.h>
#include <assert.h>

typedef struct node node;

struct node {
    /*! Puntero al dato */
    const void *data;

    /*! Puntero al siguiente nodo */
    node *next;

    /*! Puntero al anterior nodo */
    node *prev;
};

struct list {
    /*! Puntero al incio (cabeza) de la lista */
    node *head;

    /*! Puntero al final (cola) de la lista */
    node *tail;

    /*! Punteor para poder recorrer la lista */
    node *current;

    /*! Cantidad de elemento en la lista */
    long count;

    list_release_object_callback release;
};

static node *
node_init (const void *data) {
    node *new = (node *) malloc (sizeof (node));

    assert (new != NULL); // No hay memoria para reservar el Nodo.

    new->data = data;
    new->prev = NULL;
    new->next = NULL;

    return new;
}

list *
list_create (list_release_object_callback release) {
    list *new = (list * )malloc (sizeof (list));

    assert (new != NULL); // No hay memoria para reservar la Lista.

    new->head = new->tail = new->current = NULL;
    new->count = 0;
    new->release = release;

    return new;
}

list *
list_create_empty () {
    return list_create (NULL);
}

long
list_size (list *list) {
    assert (list != NULL); // list no puede ser NULL.

    if (list->head == NULL)
        return 0;

    return list->count;
}

int
list_empty (list *list) {
    assert (list != NULL); // list no puede ser NULL.

    return list->count == 0;
}

void *
list_first (list *list) {
    assert (list != NULL); // list no puede ser NULL.

    if (list->head == NULL)
        return NULL;

    list->current = list->head;

    return (void *) list->current->data;
}

void *
list_next (list *list) {
    assert (list != NULL); // list no puede ser NULL.

    if (list->head == NULL || list->current == NULL || list->current->next == NULL)
        return NULL;

    list->current = list->current->next;

    return (void *) list->current->data;
}

void *
list_last (list *list) {
    assert (list != NULL); // list no puede ser NULL.

    if (list->head == NULL)
        return NULL;

    list->current = list->tail;

    return (void *) list->current->data;
}

void *
list_prev (list *list) {
    assert (list != NULL); // list no puede ser NULL.

    if (list->head == NULL || list->current == NULL || list->current->prev == NULL)
        return NULL;

    list->current = list->current->prev;

    return (void *) list->current->data;
}

void
list_push_front (list *list, const void *data) {
    assert (list != NULL); // list no puede ser NULL.

    node *new = node_init (data);

    if (list->head == NULL) {
        list->tail = new;
    } else {
        new->next = list->head;
        list->head->prev = new;
    }

    list->head = new;
    list->count += 1;
}

void
list_push_back (list *list, const void *data) {
    assert (list != NULL); // list no puede ser NULL.

    node *new = node_init (data);

    if (list->head == NULL) {
        list->head = new;
    } else {
        list->tail->next = new;
        new->prev = list->tail;
    }

    list->tail = new;
    list->count += 1;
}

void
list_push_current (list *list, const void *data) {
    assert (list != NULL); // list no puede ser NULL.

    if (list->current == NULL)
        return;

    node *new = node_init (data);

    new->next = list->current->next;
    new->prev = list->current;

    if (list->current->next != NULL) {
        list->current->next->prev = new;
    }

    list->current->next = new;

    if (list->current == list->tail) {
        list->tail = new;
    }

    list->count += 1;
}

void *
list_pop_front (list *list) {
    assert (list != NULL); // list no puede ser NULL.

    if (list->head == NULL)
        return NULL;

    node *aux = list->head;

    void *data = (void *) aux->data;

    if (list->release != NULL) {
        list->release (data);
        data = NULL;
    }

    if (list->head == list->tail) {
        list->tail = list->head = NULL;
    } else {
        list->head = list->head->next;
        list->head->prev = NULL;
    }

    if (list->release != NULL) {
        list->release (data);
        data = NULL;
    }

    free (aux);

    list->count -= 1;

    return data;
}

void *
list_pop_back (list *list) {
    assert (list != NULL); // list no puede ser NULL.

    if (list->head == NULL)
        return NULL;

    node *aux = list->tail;

    void *data = (void *) aux->data;

    if (list->release != NULL) {
        list->release (data);
        data = NULL;
    }

    if (list->tail == list->head) {
        list->tail = list->head = NULL;
    } else {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }

    if (list->release != NULL) {
        list->release (data);
        data = NULL;
    }

    free (aux);

    list->count -= 1;

    return data;
}

void *
list_pop_current (list *list) {
    assert(list != NULL); // list no puede ser NULL.

    if (list->head == NULL || list->current == NULL)
        return NULL;

    node *aux = list->current;

    if (list->current == list->head) {
        return list_pop_front (list);
    } else if (list->current == list->tail) {
        return list_pop_back (list);
    } else {
        if (aux->next != NULL) {
            aux->next->prev = aux->prev;
        }

        if (aux->prev != NULL) {
            aux->prev->next = aux->next;
        }
    }

    void *data = (void *) aux->data;

    list->current = aux->next;

    if (list->release != NULL) {
        list->release (data);
        data = NULL;
    }

    free (aux);

    list->count -= 1;

    return data;
}

void
list_release (list **list) {
    assert(list != NULL); // list no puede ser NULL.
    assert((*list) != NULL);

    while ((*list)->head != NULL) {
        list_pop_front (*list);
    }

    free(*list);
    *list = NULL;
}


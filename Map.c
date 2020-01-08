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

#include "Map.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

typedef struct pair Pair;

/*
 http://planetmath.org/goodhashtableprimes
 */
static const long primes[] = {
        3, 7, 13, 23, 41, 71, 127, 191, 251, 383, 631, 1087, 1723,
        2803, 4523, 7351, 11959, 19447, 31231, 50683, 81919, 132607,
        214519, 346607, 561109, 907759, 1468927, 2376191, 3845119,
        6221311, 10066421, 16287743, 26354171, 42641881, 68996069,
        111638519, 180634607, 292272623, 472907251
};

static const long primeLength = sizeof(primes) / sizeof(primes[0]);

static const double maxLoadFactor = 0.77;

struct pair {
    const void * key;
    const void * value;
};

struct Map {
    long size; // tamaño del arreglo
    long count; // cantidad actual
    long current;
    int primeIndex: 6; // indice del primo en cual esta.
    long loadFactor; // el factor de carga que tiene el mapa.

    Pair ** buckets;

    MapHashCallBack hash;
    MapEqualCallBack equal;
};

static void enlarge(Map * map) {
    if (map->primeIndex == (primeLength - 1)) return; // se verifica si super el ultimo indice.

    long newSize = primes[++(map->primeIndex)]; // se obtiene el nuevo size y se aumento el primeIndex.
    long oldSize = primes[(map->primeIndex - 1)];

    Pair ** oldBucket = map->buckets;

    map->buckets = (Pair **)malloc(sizeof(Pair *) * newSize);
    memset(map->buckets, 0, newSize * sizeof(Pair *));
    map->count = 0;
    map->size = newSize;
    map->loadFactor = (long) ceil(newSize * maxLoadFactor);

    long i;

    if (oldBucket == NULL) return;

    for (i = 0; i < oldSize; i++) {
        if (oldBucket[i] != NULL) {
            if (oldBucket[i]->value != NULL) {
                insertMap(map, oldBucket[i]->key, oldBucket[i]->value);
            } else {
                free(oldBucket[i]);
            }
        }
    }

    free(oldBucket);
}

static long quadraticProbing(Map * map, const void * key) {
    assert(map->hash != NULL && map->equal != NULL);
    long long hash = llabs(map->hash(key));
    long idx = hash % map->size;
    long i = 0;
    while (map->buckets[idx] != NULL && map->equal(map->buckets[idx]->key , key) == 0) {
        //idx = (idx + i * i) % map->size;
        i += 1;
        idx = (idx + 1) % map->size;
    }
    return idx;
}

static Pair * createPair(const void * key, const void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));

    assert(new != NULL);

    new->key = key;
    new->value = value;
    return new;
}

Map * createMap(MapHashCallBack hash, MapEqualCallBack equal) {
    Map * new = (Map *)malloc(sizeof(Map));

    if (new == NULL) return NULL;

    new->primeIndex = 0; // se guarda el indice del actual numero primo.
    new->count = 0;
    new->current = 0;
    new->loadFactor = (long) ceil(primes[0] * maxLoadFactor); // se calcula el factor de carga.
    new->size = primes[0];
    new->buckets = (Pair **)malloc(sizeof(Pair *) * primes[0]);
    memset(new->buckets, 0, primes[0] * sizeof(Pair *)); // se inicializa en null.
    new->hash = hash;
    new->equal = equal;

    return new;
}

long mapCount(Map * map) {
    assert(map != NULL);
    if (map->buckets == NULL) return 0;
    return map->count;
}

int emptyMap(Map * map) {
    assert(map != NULL);
    return map->count == 0;
}

void insertMap(Map * map, const void * key, const void * value) {
    assert(map != NULL); // El mapa no puede ser NULL.

    long idx = quadraticProbing(map, key);

    if (map->buckets[idx] == NULL) {
        map->buckets[idx] = createPair(key, value);
        map->count += 1;
    } else {
        if (map->buckets[idx]->value == NULL) {
            map->buckets[idx]->value = value;
            map->count += 1;
        }
    }

    if ((map->count) >= map->loadFactor) enlarge(map); // si la cantidad supera al factor de carga se hace el enlarge del mapa.
}

void * eraseKeyMap(Map * map, const void * key) {
    assert(map != NULL); // El mapa no puede ser NULL.

    long idx = quadraticProbing(map, key);

    if (map->buckets[idx] == NULL || map->buckets[idx]->value == NULL) return NULL;

    void * aux = (void *)map->buckets[idx]->value;

    free(map->buckets[idx]);

    map->buckets[idx] = NULL;

    map->count -= 1;

    return aux;
}

void * searchMap(Map * map, const void * key) {
    assert(map != NULL); // El mapa no puede ser NULL.

    long idx = quadraticProbing(map, key);

    if (map->buckets[idx] == NULL || map->buckets[idx]->value == NULL) return NULL;

    map->current = idx;

    return (void *)map->buckets[idx]->value;
}

void * firstMap(Map * map) {
    assert(map != NULL); // El mapa no puede ser NULL.

    if (map->buckets == NULL) return NULL;

    long i;

    for (i = 0; i < map->size; i++) {
        if (map->buckets[i] != NULL && map->buckets[i]->value != NULL) {
            map->current = i;
            return (void *)map->buckets[i]->value;
        }
    }

    return NULL;
}

void * nextMap(Map * map) {
    assert(map != NULL); // El mapa no puede ser NULL.

    if (map->buckets == NULL) return NULL;

    long i;

    for (i = (map->current + 1); i < map->size; i++) {
        if (map->buckets[i] != NULL && map->buckets[i]->value != NULL) {
            map->current = i;
            return (void *)map->buckets[i]->value;
        }
    }

    return NULL;
}

void removeAllMap(Map * map) {
    assert(map != NULL); // El mapa no puede ser NULL.

    long i;

    for (i = 0; i < map->size; i++) {
        if (map->buckets[i] != NULL) {
            free(map->buckets[i]);
        }
    }

    free(map->buckets);
    map->primeIndex = 0; // se guarda el indice del actual numero primo.
    map->count = 0;
    map->current = 0;
    map->loadFactor = (long) ceil(primes[0] * maxLoadFactor); // se calcula el factor de carga.
    map->size = primes[0];
    map->buckets = (Pair **)malloc(sizeof(Pair *) * primes[0]);
    memset(map->buckets, 0, primes[0] * sizeof(Pair *)); // se inicializa en null.


}


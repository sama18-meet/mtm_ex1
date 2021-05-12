#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "map.h"


#define MAP_FOREACH_INTERNAL(type, iterator, map) \
    for (type iterator = (type) map->firstPair; \
        iterator ;\
        iterator = iterator->next)


typedef struct Pair {
    MapKeyElement key;
    MapDataElement data;
    struct Pair* next;
} *Pair;

struct Map_t {
    copyMapDataElements copyDataElement;
    copyMapKeyElements copyKeyElement;
    freeMapDataElements freeDataElement;
    freeMapKeyElements freeKeyElement;
    compareMapKeyElements compareKeyElements;
    Pair firstPair;
    Pair currentPair;
};


Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements) {
    if (copyDataElement==NULL ||
        copyKeyElement==NULL  ||
        freeDataElement==NULL ||
        freeKeyElement==NULL  ||
        compareKeyElements==NULL) { return NULL; }
    Map map = malloc(sizeof(*map));
    if (map==NULL) { return NULL; }
    map->copyDataElement = copyDataElement;
    map->copyKeyElement = copyKeyElement;
    map->freeDataElement = freeDataElement;
    map->freeKeyElement = freeKeyElement;
    map->compareKeyElements = compareKeyElements;
    map->firstPair = NULL;
    map->currentPair = NULL;
    return map;
}


Pair map_get_pair(Map map, MapKeyElement key) {
    for (Pair p = map->firstPair; p != NULL ; p = p->next) {
        if (map->compareMapKeyElements(p->key, key) == 0) {
            return p;
        }
    }
    return NULL;
}


Pair get_prev_pair(Map map, Pair pair) {
    assert(mapContains(map, pair->key));
    Pair prev_pair = NULL;
    for (Pair p = map->firstPair; p != NULL ; p = p->next) {
        if (p == pair) { return prev_pair; }
        prev_pair = p;
    }
    assert(false);
    return NULL;
}

MapResult mapRemove(Map map, MapKeyElement keyElement) {
    if (map == NULL || keyElement == NULL) { return MAP_NULL_ARGUMENT; }
    Pair pair = map_get_pair(map, keyElement);
    if (pair == NULL) { return  MAP_ITEM_DOES_NOT_EXIST; }
    prev_pair = get_prev_pair(map, pair);
    next_pair = pair->next;
    map->freeKeyElement(pair->key);
    map->freeDataElement(pair->data);
    free(pair);
    if (prev_pair == NULL) {
        map->firstPair = next_pair;
    }
    else {
        prev_pair->next = next_pair;
    }
    map->currentPair = NULL;
    return MAP_SUCCESS;
}


MapResult mapClear(Map map) {
    if (map==NULL) { return MAP_NULL_ARGUMENT; }
    Pair ptr = map->firstPair;
    while (ptr != NULL) {
        Pair toDelete = ptr;
        ptr = ptr->next;
        mapRemove(toDelete);
    }
    map->firstPair = NULL;
    return MAP_SUCCESS;
}

void mapDestroy(Map map) {
    mapClear(map);
    free(map);
}


int mapGetSize(Map map) {
    if (map == NULL) { return -1; }
    int num = 0;
    for (Pair p = map->firstPair; p != NULL ; p = p->next) {
        num++;
    }
    return num;
}

MapDataElement mapGet(Map map, MapKeyElement keyElement) {
    if (map == NULL || keyElement == NULL) { return NULL; }
    Pair pair = map_get_pair(map, keyElement);
    if (pair == NULL) { return NULL; }
    return pair->data;
}

bool mapContains(Map map, MapKeyElement element) {
    return (mapGet(map, element) != NULL);
}


MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement) {
    if (map == NULL || keyElement == NULL || dataElement == NULL) { return MAP_NULL_ARGUMENT; }
    Pair prev_pair = NULL;
    for (Pair p = map->firstPair; p != NULL ; p = p->next) {
        if (map->compareKeyElements(p->key, keyElement) == 0) {
            p->data = dataElement;
            return MAP_SUCCESS;
        }
        if (map->compareKeyElements(p->key, keyElement) < 0) {
            prev_pair = p;
        }
        else if (map->compareKeyElements(p->key, keyElement) > 0) {
            Pair pair = malloc(sizeof(*pair);
            if (pair == NULL)
                return MAP_OUT_OF_MEMORY;
            pair->key = keyElement;
            pair->data = dataElement;
            pair->next = p;
            if (p == map->firstPair)
                map->firstPair = pair;
            else
                prev_pair->next = pair;
            return MAP_SUCCESS;
        }
    }
    Pair pair = malloc(sizeof(*pair);
    if (pair == NULL)
        return MAP_OUT_OF_MEMORY;
    pair->key = keyElement;
    pair->data = dataElement;
    pair->next = NULL;
    if (prev_pair == NULL)
        map->firstPair = pair;
    else
        prev_pair->next = pair;
    return MAP_SUCCESS;
}



MapKeyElement mapGetFirst(Map map) {
    if (map == NULL || map->firstPair == NULL)
        return NULL;
    MapKeyElement keyCopy = malloc(sizeof(*keyCopy));
    if (keyCopy == NULL)
        return NULL;
    *keyCopy = *(map->firstPair->key);
    map->currentPair = map->firstPair;
    return keyCopy;
}



MapKeyElement mapGetNext(Map map) {
    if (map == NULL || map->currentPair == NULL)
        return NULL;
    if (currentPair->next == NULL)
        return NULL;
    MapKeyElement keyCopy = malloc(sizeof(*keyCopy));
    if (keyCopy == NULL)
        return NULL;
    map->currentPair = map->currentPair->next;
    *keyCopy = *(map->currentPair->key);
    return keyCopy;
}

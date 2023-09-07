#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {

  Pair* newPair = (Pair*)malloc(sizeof(Pair));
  newPair->key = strdup(key); 
  newPair->value = value;

  long i=hash(key,map->capacity); 
  long copia=i;
  while(1){
    Pair* aux =map->buckets[i];
    if(aux==NULL || aux->key==NULL){
      map->buckets[i] = newPair;
      map->size++; 
      return;
    }
    i=(i+1)% map->capacity;
    if (i==copia){
      return;
    }
  }
}

void enlarge(HashMap* map) {
  enlarge_called = 1;
  if(map == NULL){
    return;
  }
  long newCapacity = map->capacity * 2;
  Pair** newBuckets = (Pair**)malloc(sizeof(Pair*) * newCapacity);
  if(newBuckets == NULL){
    return;
  }
  for(long i = 0; i < map->capacity; i++){
    Pair* currentPair = map->buckets[i];
    if (currentPair != NULL && currentPair->key != NULL){
      long newPos = hash(currentPair->key, newCapacity);
      while (newBuckets[newPos] != NULL){
        newPos = (newPos + 1) % newCapacity;
      }
      newBuckets[newPos] = currentPair;
    }
  }
  free(map->buckets);
  map->buckets = newBuckets;
  map->capacity =newCapacity;
}


HashMap * createMap(long capacity) {
  HashMap* map=(HashMap*)malloc(sizeof(HashMap));
    
  map->size=0;
  map->capacity=capacity;
  map->current=-1;

  map->buckets = (Pair**)malloc(sizeof(Pair*) * capacity);
  for(long i=0; i<capacity; i++){
    map->buckets[i]=NULL;
  }
  return map;
}

void eraseMap(HashMap* map, char* key) {
  long i = hash(key, map->capacity);
  long copia = i;

  while(1){
    Pair* aux = map->buckets[i];
    if(aux!=NULL && strcmp(aux->key, key) == 0){
      map->size--;
      map->buckets[i]->key = NULL; 
      return;
    }
    i = (i + 1) % map->capacity;
    if(i == copia){
      return;
    }
  }
}


Pair* searchMap(HashMap* map, char* key) {
  long i = hash(key,map->capacity);
  map->current = i;
  Pair* current = map->buckets[i];
  while(current != NULL){
    if(strcmp(current->key, key) == 0){
      map->current = i;
      return current; 
    }
    i = (i + 1) % map->capacity;
    current = map->buckets[i];
    if(i == map->current){
      break;
    }
  }
  return NULL;
}


Pair* firstMap(HashMap* map) {
  if (map == NULL || map->buckets == NULL || map->capacity <= 0) {
    return NULL; // Mapa Null
  }
  map->current = -1;
  return nextMap(map);
}

Pair * nextMap(HashMap * map) {
  int a = map->capacity;
  for(int i = map->current + 1 ; i < a ; i++){
    if(map->buckets[i] != NULL && map->buckets[i]->key != NULL){
      map->current = i;
      return map->buckets[i];
    }
  }
  return NULL;
}


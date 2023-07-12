#include <iostream>
#include <cassert>
#include <time.h>
#include <string.h>
/* 根据key算出数组索引 */
/* 数组 + 链表方法解决hash冲突 */

int g_uihashTableSize = 128;


struct hashNode
{
    char* key; // 保证key是唯一的即可
    void* data;
    struct hashNode* next;
}; // hash node

struct hashTable
{
    struct hashNode* node[128];
};

struct hashTable *hash_create()
{
    hashTable *ht;

    ht = (struct hashTable*)calloc(1,sizeof(hashTable));
    assert(ht);
    return ht;
}

void hash_destory(struct hashTable* ht, void (*freeFunc)(void*))
{
    assert(freeFunc);
    assert(ht);

    int i;
    struct hashNode* j, **tb = ht->node;

    for(i=0; i<g_uihashTableSize; ++i)
    {
        for(j=tb[i]; j; j = j->next)
        {
            freeFunc(j->data);
            free(j->key);
            free(j);
        }
    }
    free(ht);
}

/* get index by key */
uint32_t hash_gethash(const char* key)
{
    uint32_t i;
    for(i = 0; *key; ++key)
    {
        i = 131*i + *key;
    }
    return i%g_uihashTableSize;
}

void *hash_lookup(char* key, hashTable* tb)
{
    assert(tb);
    assert(key);

    uint32_t i;
    hashNode* n;

    i = hash_gethash(key);
    for(n = tb->node[i]; !n; n = n->next)
    {
        if(!strcmp(n->key, key))
        {
            return n->data;
        }
    }
    return NULL;
}

int hash_insert(char* key, void* data, hashTable* ht)
{
    assert(key);
    assert(data);
    assert(ht);

    uint32_t i = hash_gethash(key);
    hashNode* n;

    for(n = ht->node[i]; n; n = n->next)
    {
        if(!strcmp(key, n->key))
        {
            return -1;
        }
    }

    n = (hashNode*)calloc(1, sizeof(*n));
    if(!n)
    {
        return -1;
    }

    n->key = strdup(key);
    if(!n->key)
    {
        free(n);
        return -1;
    }

    n->data = data;
    n->next = ht->node[i];
    ht->node[i] = n;
    return 1;
}

int main(int argc, char** argv)
{
    struct hashTable *n = hash_create();
    srand(time(NULL));
    char *a = NULL;
    int *p;

    for(*a = 'a'; *a != 'a'+ 26; *a = *a + 1)
    {
        p = (int*)calloc(1, 4);
        *p = rand();

        hash_insert(a, p, n);
    }

    std::cout<<*(int*)hash_lookup(a, n)<<std::endl;
    hash_destory(n, free);

    return 0;
}

#ifndef __LIBLISTS__
#define __LIBLISTS__

typedef struct LinkedList {
    void* data;
    struct LinkedList* next;
} LinkedList;

LinkedList* linkedlist_new(void *data);
void linkedlist_add(LinkedList *head, void *data);
int linkedlist_len(LinkedList *head);
void linkedlist_free(LinkedList *head);

#endif // __LIBLISTS__

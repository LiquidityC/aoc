#include <stdlib.h>
#include "libcollections.h"
#include "util.h"

LinkedList* linkedlist_new(void *data)
{
    LinkedList *node = ec_malloc(sizeof(LinkedList));
    node->data = data;
    node->next = NULL;
    return node;
}

void linkedlist_add(LinkedList *head, void *data)
{
    LinkedList *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = linkedlist_new(data);
}

int linkedlist_len(LinkedList *head)
{
    LinkedList *current = head;
    int len = 1;
    while (current->next != NULL) {
        current = current->next;
        len++;
    }
    return len;
}

void linkedlist_free(LinkedList *head)
{
    LinkedList *next = head->next;
    free(head->data);
    free(head);
    while (next != NULL) {
        head = next;
        next = head->next;
        free(head->data);
        free(head);
    }
}

#include <stdlib.h>
#include <assert.h>
#include <libcollections.h>

static LinkedList* create_list()
{
    int *data = malloc(sizeof(int));
    *data = 0;
    LinkedList *list = linkedlist_new(data);
    for (int i = 0; i < 10; i++) {
        data = malloc(sizeof(int));
        *data = i+1;
        linkedlist_add(list, data);
    }
    return list;
}

static void simple_test()
{
    int *data = malloc(sizeof(int));
    *data = 5;
    LinkedList *list = linkedlist_new(data);
    assert(*((int*) list->data) == 5);
    linkedlist_free(list);
}

static void len_test()
{
    int *data = malloc(sizeof(int));
    *data = 0;
    LinkedList *list = linkedlist_new(data);
    assert(linkedlist_len(list) == 1);
    for (int i = 0; i < 10; i++) {
        data = malloc(sizeof(int));
        *data = i+1;
        linkedlist_add(list, data);
    }
    assert(linkedlist_len(list) == 11);
    linkedlist_free(list);
}

static void confirm_content()
{
    LinkedList *list = create_list();
    LinkedList *it = list;
    int index = 0;
    do {
        assert(*((int*) it->data) == index);
        it = it->next;
        index++;
    } while (it);
    linkedlist_free(list);
}

int main(int argc, char *argv[])
{
    simple_test();
    len_test();
    confirm_content();
    return 0;
}

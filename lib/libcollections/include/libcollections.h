#ifndef __LIBCOLLECTIONS__
#define __LIBCOLLECTIONS__

typedef struct LinkedList {
    void* data;
    struct LinkedList* next;
} LinkedList;

LinkedList* linkedlist_new(void *data);
void linkedlist_add(LinkedList *head, void *data);
int linkedlist_len(LinkedList *head);
void linkedlist_free(LinkedList *head);

typedef struct Matrix {
    int **data;
    unsigned int width;
    unsigned int height;
} Matrix;

Matrix* matrix_new(unsigned int width, unsigned int height);
void matrix_copy(Matrix* src, Matrix* dest);
int matrix_sum(Matrix* m);
int matrix_neighbors8(Matrix *matrix, int *buf, unsigned int x, unsigned int y);
void matrix_print(Matrix* m);
void matrix_free(Matrix* matrix);

#endif // __LIBCOLLECTIONS__

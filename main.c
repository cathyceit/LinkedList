#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define TRACK_ALLOCATIONS 0

typedef struct node {
    int value;
    struct node *next;
} node;

typedef struct linked_list {
    node *first;
    int count;
} linked_list;

int allocations_count = 0;

void *my_malloc(size_t size) {
    allocations_count += 1;

    if (TRACK_ALLOCATIONS) {
        printf("Allocation for size=%li, allocation_count=%i\n", size, allocations_count);
    }

    return malloc(size);
}

void my_free(void *ptr) {
    allocations_count -= 1;

    if (TRACK_ALLOCATIONS) {
        printf("Deallocation, allocation_count=%i\n", allocations_count);
    }

    free(ptr);
}


node *node_create(int value) {
    node *node = my_malloc(sizeof(node));
    node->value = value;
    node->next = NULL;
    return node;
}

linked_list *linked_list_create() {
    linked_list *linked_list = my_malloc(sizeof(linked_list));
    linked_list->first = NULL;
    linked_list->count = 0;
    return linked_list;
}

void linked_list_destroy(linked_list **self_ptr) {
    linked_list *self = *self_ptr;

    node *current = self->first;

    while (current) {
        node *next = current->next;
        my_free(current);
        current = next;
    }

    my_free(*self_ptr);
    *self_ptr = NULL;
}

void linked_list_push_front(linked_list *self, node *new_node) {
    if (self->first == NULL) {
        self->first = new_node;
    } else {
        node *temp = self->first;
        new_node->next = temp;
        self->first = new_node;
    }
    self->count++;
}

void linked_list_print(linked_list *self) {
    node *current = self->first;
    while (current) {
        printf("%i\n", current->value);
        current = current->next;
    }
}

void linked_list_reverse(linked_list *self) {
    // array of node*
    node **array = my_malloc(sizeof(node *) * self->count);
    node *current = self->first;
    int index = 0;
    while (current) {
        array[index] = current;
        index++;
        current = current->next;
    }
    self->first->next = NULL;
    self->first = NULL;
    int count = self->count;
    self->count = 0;

    for (int i = 0; i < count; i++) {
        linked_list_push_front(self, array[i]);
    }
    my_free(array);
}

typedef struct {
    int *first;
    int length;
} array;

array *linked_list_to_array(linked_list *self) {
    array *my_array = my_malloc(sizeof(array));
    my_array->first = my_malloc(sizeof(int) * self->count);
    my_array->length = self->count;

    node *current = self->first;
    int index = 0;

    while (current) {
        my_array->first[index] = current->value;
        index++;
        current = current->next;
    }

    return my_array;
}

void array_print(array *self) {
    for (int i = 0; i < self->length; i++) {
        printf("%i\n", self->first[i]);
    }
}

void array_destroy(array **array) {
    my_free((**array).first);
    my_free(*array);
    *array = NULL;
}


int main(void) {
    linked_list *linked_list = linked_list_create();
    for (int i = 0; i < 100; i++) {
        node *node = node_create(i);
        linked_list_push_front(linked_list, node);
    }
    linked_list_reverse(linked_list);
    //linked_list_print(linked_list);

    array *my_array = linked_list_to_array(linked_list);
    array_print(my_array);

    array_destroy(&my_array);
    linked_list_destroy(&linked_list);

    printf("allocations left: %i\n", allocations_count);

    return 0;
}

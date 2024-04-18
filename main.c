#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct node {
    int value;
    struct node* next;
} node;

typedef struct linked_list {
    node *first;
    int count;
} linked_list;

node* node_create(int value)
{
    node* node = malloc(sizeof(node));
    node->value = value;
    node->next = NULL;
    return node;
}

linked_list* linked_list_create()
{
    linked_list* linked_list = malloc(sizeof(linked_list));
    linked_list->first = NULL;
    linked_list->count = 0;
    return linked_list;
}

void linked_list_append(linked_list* self, node* new_node)
{
    if (self->first == NULL)
    {
        self->first = new_node;
    }
    else
    {
        node *temp = self->first;
        new_node->next = temp;
        self->first = new_node;
    }
    self->count++;
}

void linked_list_print(linked_list* self)
{
    node* current = self->first;
    while (current)
    {
        printf("%i\n", current->value);
        current = current->next;
    }
}

void linked_list_reverse(linked_list* self)
{
    // array of node*
    node** array = malloc(sizeof(node*)*self->count);
    node* current = self->first;
    int index = 0;
    while (current)
    {
        array[index] = current;
        index++;
        current = current->next;
    }
    self->first->next = NULL;
    self->first = NULL;
    int count = self->count;
    self->count = 0;

    for (int i = 0; i < count; i++)
    {
        linked_list_append(self, array[i]);
    }
    free(array);
}

int main(void) {
    linked_list* linked_list = linked_list_create();
    for (int i = 0; i < 100; i++)
    {
        node* node = node_create(i);
        linked_list_append(linked_list, node);
    }
    linked_list_reverse(linked_list);
    linked_list_print(linked_list);

    return 0;
}

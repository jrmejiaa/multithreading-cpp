#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct list_node list_node;

struct list_node
{
    list_node* next;
    list_node* prev;
    int data;
};

typedef struct
{
    list_node* head;
    list_node* tail;
} list;

void list_append (list* list, int data) {
    list_node* new_node = malloc( sizeof(list_node) );
    new_node->data = data;
    new_node->next = new_node->prev = NULL;
    if (list->tail == NULL) {
        list->head = list->tail = new_node;
    } else {
        list->tail->next = new_node;
        new_node->prev = list->tail;
        list->tail = new_node;
    }
}

void show_items (list* list) {
    list_node* tmp = list->head;
    int i = 0;
    while (tmp != NULL) {
        printf("item %d = %d\n", i, tmp->data);
        tmp = tmp->next;
        i++;
    }
}   

int main()
{
    list* list = malloc (sizeof(list));
    list_append(list, 20);
    list_append(list, 15);
    list_append(list, 5);
    list_append(list, 10);
    list_append(list, 1);
    show_items(list);
}
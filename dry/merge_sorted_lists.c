#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// **************************************************************** //
typedef struct node_t {
    int x;
    struct node_t *next;
} *Node;

typedef enum {
    SUCCESS=0,
    MEMORY_ERROR,
    UNSORTED_LIST,
    NULL_ARGUMENT,
} ErrorCode;

int getListLength(Node list);
bool isListSorted(Node list);
Node mergeSortedLists(Node list1, Node list2, ErrorCode* error_code);


int getListLength(Node list) {
    int len = 0;
    while (list != NULL) {
        len++;
        list = list->next;
    }
    return len;
}

bool isListSorted(Node list) {
    int len = getListLength(list);
    for (int i=0; i<len-1; ++i) {
        if (list->x > list->next->x) {
            return false;
        }
        list = list->next;
    }
    return true;
}

// **************************************************************** //

Node mergeSortedLists(Node list1, Node list2, ErrorCode* error_code) {
    int length1 = getListLength(list1);
    int length2 = getListLength(list2);
    if (length1 == 0  ||  length2 == 0) {
        *error_code = NULL_ARGUMENT;
        return NULL;
    }
    if (!isListSorted(list1) || !isListSorted(list2)) {
        *error_code = UNSORTED_LIST;
        return NULL;
    }
    int merged_list_length = length1 + length2;
    Node merged_list = malloc(sizeof(*merged_list)); 
    if (merged_list == NULL) {
        *error_code =  MEMORY_ERROR;
        return NULL;
    }
    Node merged_list_iterator = merged_list;
    for (int i=0; i<merged_list_length; i++) {
        if (/*list2 == NULL || should be removed*/ list1->x >= list2->x) {
            merged_list_iterator->x = list2->x; //changed every 1 into 2 
            list2 = list2->next;
        }
        else {
            assert(list1 == NULL || list2->x > list1->x); //deleted = in ">="
            merged_list_iterator->x = list1->x;//changed every 2 into 1
            list1 = list1->next;
        }
        if (i == merged_list_length-1) {
            *error_code = SUCCESS;
            return merged_list;
        }
        Node merged_list_next = malloc(sizeof(*merged_list));
        if (merged_list_next == NULL) {
            *error_code =  MEMORY_ERROR;
            return NULL;
        }
        merged_list_iterator->next = merged_list_next;
        merged_list_iterator = merged_list_next;
    }
    assert(1==0); // we will never get here
    return NULL;
    
}
// **************************************************************** //

Node getNode(int size) {
    if (size <= 0) {return NULL;}
    Node prev_n = NULL;
    Node first_n = NULL;
    int num;
    for (int i=0; i<size; ++i) {
        scanf("%d", &num);
        Node n = malloc(sizeof(n));    
        if (n==NULL) {return NULL;}
        n->x = num;
        n->next = NULL;
        if (i==0) { first_n = n; }
        else { prev_n->next = n; }
        prev_n = n;
    }
    return first_n;
}

void print_list(Node list) {
    while (list!=NULL) {
        printf("%d", list->x);
        list = list->next;
    }
    printf("\n");
}

int main() {
    int size1, size2;
    scanf("%d", &size1);
    scanf("%d", &size2);
    Node node1 = getNode(size1);
    print_list(node1);
    Node node2 = getNode(size2);
    print_list(node2);
    if (node1 ==NULL ||node2 ==NULL) {return 1;} 
    ErrorCode* error_code = NULL;
    Node sorted_list = mergeSortedLists(node1, node2, error_code);
    print_list(sorted_list);
}

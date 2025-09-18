// list/list.c
//
// Implementation for linked list.
//
// <Author>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

static void free_nodes(node_t *n);

/* Allocate an empty list on the heap */
list_t *list_alloc() {
  list_t *mylist = (list_t *) malloc(sizeof(list_t));
  if (!mylist) return NULL;
  mylist->head = NULL;
  return mylist;
}

/* Free all nodes, then the list struct itself */
void list_free(list_t *l) {
  if (!l) return;
  free_nodes(l->head);
  l->head = NULL;
  free(l);
}

static void free_nodes(node_t *n) {
  while (n) {
    node_t *next = n->next;
    free(n);
    n = next;
  }
}

/* Print the list as: v1->v2->...->NULL\n */
void list_print(list_t *l) {
  if (!l) {
    printf("NULL\n");
    return;
  }
  node_t *curr = l->head;
  while (curr) {
    printf("%d->", curr->value);
    curr = curr->next;
  }
  printf("NULL\n");
}

/* returns string of List (caller owns/free) */
char * listToString(list_t *l) {
  char *buf = (char *) malloc(sizeof(char) * 10024);
  if (!buf) return NULL;
  buf[0] = '\0';  /* initialize as empty C-string */

  char tbuf[32];
  node_t *curr = l ? l->head : NULL;
  while (curr != NULL) {
    snprintf(tbuf, sizeof(tbuf), "%d->", curr->value);
    strcat(buf, tbuf);
    curr = curr->next;
  }
  strcat(buf, "NULL");
  return buf;
}

/* Return number of nodes */
int list_length(list_t *l) {
  if (!l) return 0;
  int count = 0;
  node_t *c;
  for (c = l->head; c; c = c->next) {
    count++;
  }
  return count;
}

/* Helper: make a node on the heap */
node_t * getNode(elem value) {
  node_t *mynode = (node_t *) malloc(sizeof(node_t));
  if (!mynode) return NULL;
  mynode->value = value;
  mynode->next = NULL;
  return mynode;
}

/* Push to back */
void list_add_to_back(list_t *l, elem value) {
  if (!l) return;
  node_t *n = getNode(value);
  if (!n) return;

  if (!l->head) {
    l->head = n;
    return;
  }
  node_t *c = l->head;
  while (c->next) c = c->next;
  c->next = n;
}

/* Push to front */
void list_add_to_front(list_t *l, elem value) {
  if (!l) return;
  node_t *cur_node = (node_t *) getNode(value);
  if (!cur_node) return;

  cur_node->next = l->head;
  l->head = cur_node;
}

/* Insert at 1-based index */
void list_add_at_index(list_t *l, elem value, int index) {
  if (!l || index < 1) return;

  if (index == 1) {
    list_add_to_front(l, value);
    return;
  }

  int pos = 1;
  node_t *prev = l->head;
  while (prev && pos < index - 1) {
    prev = prev->next;
    pos++;
  }

  if (!prev) return;

  node_t *n = getNode(value);
  if (!n) return;

  n->next = prev->next;
  prev->next = n;
}

/* Pop from back; return removed value or -1 if empty */
elem list_remove_from_back(list_t *l) {
  if (!l || !l->head) return -1;

  node_t *curr = l->head;
  node_t *prev = NULL;
  while (curr->next) {
    prev = curr;
    curr = curr->next;
  }

  elem val = curr->value;
  if (prev) {
    prev->next = NULL;
  } else {
    l->head = NULL;
  }
  free(curr);
  return val;
}

/* Pop from front; return removed value or -1 if empty */
elem list_remove_from_front(list_t *l) {
  if (!l || !l->head) return -1;
  node_t *old = l->head;
  elem val = old->value;
  l->head = old->next;
  free(old);
  return val;
}

/* Remove at 1-based index; return removed value or -1 if invalid */
elem list_remove_at_index(list_t *l, int index) {
  if (!l || index < 1 || !l->head) return -1;

  if (index == 1) {
    return list_remove_from_front(l);
  }

  int pos = 1;
  node_t *prev = l->head;
  while (prev && pos < index - 1) {
    prev = prev->next;
    pos++;
  }

  if (!prev || !prev->next) {
    return -1;
  }

  node_t *target = prev->next;
  elem val = target->value;
  prev->next = target->next;
  free(target);
  return val;
}

/* True if value exists */
bool list_is_in(list_t *l, elem value) {
  if (!l) return false;
  node_t *c;
  for (c = l->head; c; c = c->next) {
    if (c->value == value) return true;
  }
  return false;
}

/* 1-based index. Return value at index or -1 if invalid */
elem list_get_elem_at(list_t *l, int index) {
  if (!l || index < 1) return -1;
  int pos = 1;
  node_t *c = l->head;
  while (c && pos < index) {
    c = c->next;
    pos++;
  }
  if (!c) return -1;
  return c->value;
}

/* Return first 1-based index of value, or -1 if not found */
int list_get_index_of(list_t *l, elem value) {
  if (!l) return -1;
  int pos = 1;
  node_t *c;
  for (c = l->head; c; c = c->next, pos++) {
    if (c->value == value) return pos;
  }
  return -1;
}

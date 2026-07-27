#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { NODE_OPERAND, NODE_OPERATOR } NodeType;

typedef struct Node {
  NodeType type;
  int val;
  int priority;
  struct Node *left;
  struct Node *right;
} Node;

typedef struct {
  Node **items;
  int top;
  int capacity;
} NodeStack;

typedef struct {
  int top;
  int capacity;
  char *items;
} CharStack;

NodeStack *make_stack(int cap) {
  NodeStack *s = malloc(sizeof(NodeStack));
  s->items = malloc(sizeof(Node *) * cap);
  s->top = -1;
  s->capacity = cap;
  return s;
}

void push_node(NodeStack *s, Node *n) {
  if (s->top >= s->capacity - 1)
    return;
  s->items[++s->top] = n;
}

Node *pop_node(NodeStack *s) {
  if (s->top < 0)
    return NULL;
  return s->items[s->top--];
}

Node *peek_node(NodeStack *s) {
  if (s->top < 0)
    return NULL;
  return s->items[s->top];
}

CharStack *make_cstack(int cap) {
  CharStack *s = malloc(sizeof(CharStack));
  s->items = malloc(cap);
  s->top = -1;
  s->capacity = cap;
  return s;
}

void push_char(CharStack *s, char c) {
  if (s->top >= s->capacity - 1)
    return;
  s->items[++s->top] = c;
}

char pop_char(CharStack *s) {
  if (s->top < 0)
    return 0;
  return s->items[s->top--];
}

char peek_char(CharStack *s) {
  if (s->top < 0)
    return 0;
  return s->items[s->top];
}

int prio(char op) {
  if (op == '+' || op == '-')
    return 1;
  if (op == '*' || op == '/')
    return 2;
  return 0;
}

Node *make_operand(int v) {
  Node *n = malloc(sizeof(Node));
  n->type = NODE_OPERAND;
  n->val = v;
  n->priority = 0;
  n->left = n->right = NULL;
  return n;
}

Node *make_op(char op, int priority) {
  Node *n = malloc(sizeof(Node));
  n->type = NODE_OPERATOR;
  n->val = op;
  n->priority = priority;
  n->left = n->right = NULL;
  return n;
}

void combine(NodeStack *vals, CharStack *ops) {
  char c = pop_char(ops);
  Node *r = pop_node(vals), *l = pop_node(vals);
  Node *op = make_op(c, prio(c));
  op->left = l;
  op->right = r;
  push_node(vals, op);
}

void feed(NodeStack *vals, CharStack *ops, Node *n) {
  // TOOK ALOT OF HELP FROM AI FOR THIS.
  if (n->type == NODE_OPERAND) {
    push_node(vals, n);
    return;
  }
  if (n->val == '(') {
    push_char(ops, '(');
    return;
  }
  if (n->val == ')') {
    while (ops->top >= 0 && peek_char(ops) != '(')
      combine(vals, ops);
    pop_char(ops);
    return;
  }
  while (ops->top >= 0 && peek_char(ops) != '(' &&
         prio(peek_char(ops)) >= n->priority)
    combine(vals, ops);
  push_char(ops, n->val);
}

Node *done(NodeStack *vals, CharStack *ops) {
  while (ops->top >= 0)
    combine(vals, ops);
  return pop_node(vals);
}

void print_tree_(Node *n, const char *pref, int last) {
  if (!n)
    return;
  if (n->type == NODE_OPERAND)
    printf("%s%s%d\n", pref, last ? "└── " : "├── ", n->val);
  else
    printf("%s%s%c\n", pref, last ? "└── " : "├── ", n->val);
  char next[256];
  snprintf(next, sizeof next, "%s%s", pref, last ? "    " : "│   ");
  if (n->left)
    print_tree_(n->left, next, n->right == NULL);
  if (n->right)
    print_tree_(n->right, next, 1);
}

void print_tree(Node *n) { print_tree_(n, "", 1); }

int dfs(Node *n) {
  if (n->type == NODE_OPERAND)
    return n->val;
  int l = dfs(n->left), r = dfs(n->right);
  switch (n->val) {
  case '+':
    return l + r;
  case '-':
    return l - r;
  case '*':
    return l * r;
  case '/':
    return l / r;
  default:
    return 0;
  }
}

int main(void) {
  printf("Expression: ((4 + 5) * (2 + 6))\n\n");

  NodeStack *vals = make_stack(64);
  CharStack *ops = make_cstack(64);

  feed(vals, ops, make_op('(', 0)); // outer (
  feed(vals, ops, make_op('(', 0)); // inner (
  feed(vals, ops, make_operand(4));
  feed(vals, ops, make_op('+', 1));
  feed(vals, ops, make_operand(5));
  feed(vals, ops, make_op(')', 0)); // resolves inner
  feed(vals, ops, make_op('*', 2));
  feed(vals, ops, make_op('(', 0)); // (
  feed(vals, ops, make_operand(2));
  feed(vals, ops, make_op('+', 1));
  feed(vals, ops, make_operand(6));
  feed(vals, ops, make_op(')', 0)); // resolves (2+6)
  feed(vals, ops, make_op(')', 0)); // resolves outer

  Node *root = done(vals, ops);

  printf("--- Abstract Syntax Tree ---\n");
  print_tree(root);
  printf("\nResult: %d\n", dfs(root));

  free(root);
  free(vals->items);
  free(vals);
  free(ops->items);
  free(ops);
  return 0;
}

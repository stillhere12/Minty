#include <stdio.h>
#include <stdlib.h>

typedef enum {
  NODE_OPERAND,
  NODE_OPERATOR,
} NodeType;

typedef struct Node {
  NodeType type;
  char val;
  int priority;
  struct Node *left;
  struct Node *right;
} Node;

Node *create_operand(char val) {
  Node *node = malloc(sizeof(Node));
  node->type = NODE_OPERAND;
  node->val = val;
  node->priority = 0;
  node->left = node->right = NULL;
  return node;
}

Node *create_operator(char op, int priority) {
  Node *node = malloc(sizeof(Node));
  node->type = NODE_OPERATOR;
  node->val = op;
  node->priority = priority;
  node->left = node->right = NULL;
  return node;
}

Node *insert_operator(Node *root, Node *op_node, Node *right_operand) {
  op_node->right = right_operand;
  if (root->type == NODE_OPERAND || root->priority >= op_node->priority) {
    op_node->left = root;
    return op_node;
  }
  Node *curr = root;
  while (curr->right && curr->right->type == NODE_OPERATOR &&
         curr->right->priority < op_node->priority)
    curr = curr->right;
  op_node->left = curr->right;
  curr->right = op_node;
  return root;
}
// ponytail: fixed 256-char prefix buffer, safe up to depth ~63
void print_tree_(Node *node, const char *prefix, int is_last) {
  if (!node)
    return;
  printf("%s%s%c\n", prefix, is_last ? "└── " : "├── ", node->val);
  char next[256];
  snprintf(next, sizeof next, "%s%s", prefix, is_last ? "    " : "│   ");
  if (node->left)
    print_tree_(node->left, next, node->right == NULL);
  if (node->right)
    print_tree_(node->right, next, 1);
}
void print_tree(Node *node) { print_tree_(node, "", 1); }

void free_tree(Node *node) {
  if (!node)
    return;
  free_tree(node->left);
  free_tree(node->right);
  free(node);
}

int main(void) {
  printf("Parsing expression: A + B * C / D - E\n\n");

  Node *root = create_operand('A');
  root = insert_operator(root, create_operator('+', 1), create_operand('B'));
  root = insert_operator(root, create_operator('*', 2), create_operand('C'));
  root = insert_operator(root, create_operator('/', 2), create_operand('D'));
  root = insert_operator(root, create_operator('-', 1), create_operand('E'));

  printf("--- Abstract Syntax Tree ---\n");
  print_tree(root);

  free_tree(root);
  return 0;
}

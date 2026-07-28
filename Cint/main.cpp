#include <cctype>
#include <stack>
#include <stdio.h>
#include <string>
#include <vector>

int prio(char op) {
  if (op == '+' || op == '-')
    return 1;
  if (op == '*' || op == '/')
    return 2;
  return 0;
}

void apply(std::stack<int> &v, std::stack<char> &o) {
  int r = v.top();
  v.pop();
  int l = v.top();
  v.pop();
  char op = o.top();
  o.pop();
  switch (op) {
  case '+':
    v.push(l + r);
    break;
  case '-':
    v.push(l - r);
    break;
  case '*':
    v.push(l * r);
    break;
  case '/':
    v.push(l / r);
    break;
  }
}

int eval_expr(const std::string &expr) {
  int es = (int)expr.size(), A = 0, i = 0;
  std::vector<int> D(es);
  for (int i = 0; i < es; ++i) {
    if (expr[i] == '(')
      A++;
    else if (expr[i] == ')')
      A--;
    D[i] = A;
  }
  std::vector<std::stack<int>> vals(1);
  std::vector<std::stack<char>> ops(1);
  while (i < es) {
    if (expr[i] == '(') {
      if (D[i] >= (int)vals.size())
        vals.resize(D[i] + 1), ops.resize(D[i] + 1);
      i++;
    } else if (expr[i] == ')') {
      int d = D[i] + 1; // ponytail: D[i] is depth after ')', add 1 for inside depth
      while (!ops[d].empty())
        apply(vals[d], ops[d]);
      int r = vals[d].top();
      vals[d].pop(), vals[d - 1].push(r);
      i++;
    } else if (std::isdigit(expr[i])) {
      int n = 0, d = D[i]; // ponytail: capture depth before i advances
      while (i < (int)expr.size() && std::isdigit(expr[i]))
        n = n * 10 + (expr[i++] - '0');
      vals[d].push(n);
    } else {
      // Reason is to first apply more powerful expression then others
      while (!ops[D[i]].empty() && prio(ops[D[i]].top()) >= prio(expr[i]))
        apply(vals[D[i]], ops[D[i]]);
      ops[D[i]].push(expr[i]);
      i++;
    }
  }
  while (!ops[0].empty())
    apply(vals[0], ops[0]);
  return vals[0].top();
}

int main(void) {
  const char *s = "(1+(4+5+2)-3)+(6+8)";
  printf("Expression: %s\n", s);
  printf("Result: %d\n", eval_expr(s));
  return 0;
}

#include <cctype>
#include <stack>
#include <stdio.h>
#include <string>
#include <vector>
/*Added support for prefix - operator */
int prio(char op) {
  if (op == '+' || op == '-')
    return 1;
  if (op == '*' || op == '/')
    return 2;
  if (op == '~')
    return 3;
  return 0;
}

void apply(std::stack<int> &v, std::stack<char> &o) {
  char op = o.top();
  o.pop();
  if (op == '~') {
    v.top() = -v.top();
    return;
  }
  int r = v.top();
  v.pop();
  int l = v.top();
  v.pop();
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
  std::string e;
  // Skip whitespaces
  for (char c : expr)
    if (c != ' ')
      e += c;
  int es = (int)e.size(), A = 0, i = 0;
  std::vector<int> D(es);
  for (int i = 0; i < es; ++i) {
    if (e[i] == '(')
      A++;
    else if (e[i] == ')')
      A--;
    D[i] = A;
  }
  std::vector<std::stack<int>> vals(1);
  std::vector<std::stack<char>> ops(1);
  while (i < es) {
    if (e[i] == '(') {
      if (D[i] >= (int)vals.size())
        vals.resize(D[i] + 1), ops.resize(D[i] + 1);
      i++;
    } else if (e[i] == ')') {
      int d = D[i] + 1; // we want calculation of previous depth
      while (!ops[d].empty())
        apply(vals[d], ops[d]);
      int r = vals[d].top();
      vals[d].pop(), vals[d - 1].push(r);
      i++;
    } else if (std::isdigit(e[i])) {
      int n = 0, d = D[i]; // ponytail: capture depth before i advances
      while (i < es && std::isdigit(e[i]))
        n = n * 10 + (e[i++] - '0');
      vals[d].push(n);
    } else {
      char c = e[i];
      if (c == '-' && (i == 0 || e[i - 1] == '(' || prio(e[i - 1]) > 0))
        c = '~';
      if (c != '~')
        while (!ops[D[i]].empty() && prio(ops[D[i]].top()) >= prio(c))
          apply(vals[D[i]], ops[D[i]]);
      ops[D[i]].push(c);
      i++;
    }
  }
  while (!ops[0].empty())
    apply(vals[0], ops[0]);
  return vals[0].top();
}

int main(void) {
  struct {
    const char *e;
    int r;
  } tests[] = {
      {"-5", -5},
      {"-5+3", -2},
      {"-(1+2)", -3},
      {"-(1+2)-3", -6},
      {"5*-3", -15},
      {"--5", 5},
      {"(1+(4+5+2)-3)+(6+8)", 23},
      {"-1", -1},
      {"1-(     -2)", 3}, // ponytail: spaces + unary
      {"  42  ", 42},
      {"1 - -2", 3},
      {"  ( 1 + 2 )  ", 3},
  };
  for (auto &t : tests) {
    int got = eval_expr(t.e);
    printf("%s = %d %s\n", t.e, got, got == t.r ? "OK" : "FAIL");
  }
  return 0;
}

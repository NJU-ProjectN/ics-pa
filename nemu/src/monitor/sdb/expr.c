#include "common.h"
#include <assert.h>
#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NUM,
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {
  {" +", TK_NOTYPE},       // spaces
  {"\\+", '+'},            // plus
  {"-", '-'},              // subtract
  {"\\*", '*'},            // multiply
  {"/", '/'},              // divide
  {"\\(", '('},            // left_bracket
  {"\\)", ')'},            // right_bracket
  {"==", TK_EQ},           // equal
  {"[0-9]+", TK_NUM},      // int
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        Token tk = {};
        switch (rules[i].token_type) {
          case TK_NUM:
              assert(substr_len < 32);
              strncpy(tk.str, substr_start, substr_len);
              tk.str[substr_len] = '\0';
          default: tk.type = rules[i].token_type;
        }

        tokens[nr_token++] = tk;
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


bool check_parentheses(int p, int q) {
  if ((tokens[p].type != '(') && (tokens[q].type != ')')) {
    return false;
  }
  int r = 0;
  for (int i = p; i < q; i ++) {
    if (tokens[i].type == '(') {
      r++;
    } else if (tokens[i].type == ')') {
      r--;
      if (r < 0) {
        return false;
      }
    }
  }

  if (r != 0) {
    return false;
  }
  return true;
}

bool is_op_type(int type) {
  switch (type) {
    case '+':
    case '-':
    case '*':
    case '/': return true;
    default: return false;
  }
}

int op_prio(int type) {
  switch (type) {
    case '+':
    case '-': return 1;
    case '*':
    case '/': return 2;
    default: assert(0);
  }
}

int find_master_op(int p, int q) {
  int op_index = -1;
  int in_parentheses_flag = 0;
  for (int i = p; i < q; i ++) {
    if (tokens[i].type == '(') {
        in_parentheses_flag++;
    }
    if (tokens[i].type == ')') {
        in_parentheses_flag--;
    }

    // 跳过非符号
    if (is_op_type(tokens[i].type) == false) {
        continue;
    }

    if (in_parentheses_flag <= 0 && op_index < 0) {
        op_index = i;
        continue;
    }

    if (op_prio(tokens[i].type) <= op_prio(tokens[op_index].type) && in_parentheses_flag <= 0) {
        op_index = i;
    }
  }
  assert(op_index > 0);
  return op_index;
}

word_t eval(int p, int q) {
  assert(p >= q);
  if (p == q) {
    assert(tokens[p].type == TK_NUM);
    return atoi(tokens[p].str);
  } else if (check_parentheses(p, q) == true) {
    return eval(p + 1, q - 1);
  } else {
    int op_index = find_master_op(p, q);
    word_t val1 = eval(p, op_index - 1);
    word_t val2 = eval(op_index + 1, q);

    switch (tokens[op_index].type) {
      case '+': return val1 + val2;
      case '-': return val1 - val2;
      case '*': return val1 * val2;
      case '/': return val1 / val2;
      default: assert(0);
    }
  }
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  word_t ret = eval(0, nr_token - 1);
  *success = true;
  return ret;
}


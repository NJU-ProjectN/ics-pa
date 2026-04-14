#!/usr/bin/python

import random, tempfile, subprocess

def execute(commands):
  p = subprocess.Popen(commands, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  (out, err) = p.communicate()
  if p.returncode != 0:
    raise Exception("Execute {0} fail".format(' '.join(commands)))
  return out

def gen(n, arr_len, var):
  VARS = [ "a[{0}]".format(i) for i in range(0, arr_len) ] + var

  program = []
  program.append( '''
u32 a[{0}];
u32 {1};
  '''.format(arr_len, ', '.join(var)) )

  program.append( "void f() {" )

  labels = [i for i in range(0, n)]
  random.shuffle(labels)
  nxt = [-1] * n

  for i in range(0, n - 1):
    nxt[labels[i]] = labels[i+1]

  program.append( "goto Label{0};".format(labels[0]) )


  def gen_rval(x):
    if x == 0: return "{0}u".format(random.randint(0, (1<<32)-1))
    if x == 1: return random.choice(VARS)
    if x >= 2:
      l = random.randint(0, x - 1)
      p1 = lambda: "a[({0}) % {1}u]".format(gen_rval(x - 1), arr_len)
      p2 = lambda: "({0}) {1} (({2})|1)".format(
        gen_rval(l),
        random.choice("/%"),
        gen_rval(x - l - 1),
      )
      p3 = lambda: "({0}({1}))".format(
        random.choice("~!"),
        gen_rval(x - 1),
      )
      p4 = lambda: "({0}) {1} ({2})".format(
        gen_rval(l),
        random.choice("+-*^|&"),
        gen_rval(x - l - 1),
      )
      ty = random.randint(0, 100)
      if ty < 10: return p1()
      if ty < 20: return p2()
      if ty < 30: return p3()
      return p4()

  def gen_lval(x):
    assert(x > 0)
    if x == 1: return random.choice(VARS)
    return "a[({0}) % {1}u]".format( gen_rval(x - 1), arr_len )

  def L(lim = 3):
    return gen_lval(random.randint(1, lim))
  def R(lim = 6):
    return gen_rval(random.randint(0, lim))

  def gen_block():
    for x in range(0, random.randint(1, 5)):
      program.append("  {0} = {1};".format(L(), R()) )
  def gen_cond():
    program.append( "  if (({0}) {1} ({2}))".format(R(1), random.choice(["<", ">", "!=", "=="]), R(1)), )

  for i in range(0, n):
    program.append( "Label{0}:".format(i) )
    program.append( "  S ++;")

    now = i
    for j in range(0, random.randint(0, 3)):
      now = nxt[now]
      if now == -1: break
      gen_cond()
      program.append( "    goto Label{0};".format(now) )

    gen_block()
    if nxt[i] == -1:
      program.append( "  return;" )
    else:
      program.append( "  goto Label{0};".format(nxt[i]) )
    

  program.append( '}\n' )

  def gen_print():
    return '\n'.join( [ '  printf("0x%08x\\n", {0});'.format(v) for v in VARS ] + 
                      [ '  printf("%d\\n", S);' ])
      
  code_pr = '''
#include <stdio.h>
typedef unsigned int u32;
#pragma GCC diagnostic ignored "-Wunused-label"

u32 S = 0;
''' + '\n'.join(program) + '''
int main() {
  f();
''' + gen_print() + "\n  return 0;\n}\n"

  fp = tempfile.NamedTemporaryFile(suffix = ".c", delete = False)
  fp.write(code_pr)
  fp.close()

  cfile = fp.name
  execute(["gcc", "-m32", cfile, "-o", cfile + ".exe"])
  ans = execute([cfile + ".exe"])

  def gen_assert(ans):
    return '\n'.join( [ '  nemu_assert({0} == {1});'.format(v, a) for (v, a) in zip(VARS + ['S'], ans) ] )


  return '''
#include <am.h>
#include "trap.h"
#pragma GCC diagnostic ignored "-Wunused-label"

u32 S = 0;
''' + '\n'.join(program) + '''
int main() {
  f();
''' + gen_assert(ans.strip().split('\n')) + '''
  HIT_GOOD_TRAP;
  return 0;
}
'''

print gen(16, 3, ["x", "y", "z", "u", "v", "w"])

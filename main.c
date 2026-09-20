
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>
#define exit(_) do { fflush(stdout); assert(0); } while(0)

typedef union {
  int sort;
  const char *var;
  struct {
    const char *param;
    int type, body, special;
  };
  struct { 
    int func, arg;
  };
  struct {
    const char *lname;
    int ltype; int lvalue; int lbody;
  };
} term;

enum { TYPE, VAR, LAM, PI, APP, LET };

#define NTERMS 1024*1024
term *ts;
static int *tags;
static int ts_count = 1;

const char *input;
static int pos = 0;

void print_term(int id) {
  term t = ts[id];

  switch (tags[id]) {

  case TYPE:
    printf("*%d", t.sort);
    break;

  case VAR:
    printf("%s", t.var);
    break;

  case LAM:
    printf("\\%s : ", t.param);
    print_term(t.type);
    printf(". ");
    print_term(t.body);
    break;

  case PI:
    printf("&%s : ", t.param);
    print_term(t.type);
    printf(". ");
    print_term(t.body);
    break;

  case APP:;
    int pf = tags[t.func] == LAM || tags[t.func] == PI;
    if (pf) printf("(");
    print_term(t.func);
    if (pf) printf(")");
    int pa = tags[t.arg] == LAM || tags[t.arg] == APP || tags[t.arg] == PI;
    printf(" ");
    if (pa) printf("(");
    print_term(t.arg);
    if (pa) printf(")");
    break;

  default:
    assert(0);
  }
}

static inline int alloc(int id, int tag, term t) {
  int n = ts_count++;
  ts[n] = t;
  tags[n] = tag;
  return n;
}

void skip_ws(void) {
  while (isspace(input[pos])) ++pos;
}

static inline int parse_str(const char *s) {
  int l = strlen(s);
  skip_ws();
  if (strncmp(input + pos, s, l)) return 0;
  pos += l;
  skip_ws();
  return 1;
}

const char *parse_name(void) {
  skip_ws();
  int b = pos;
  while (isalnum(input[pos]) || input[pos] == '_') ++pos;
  if (b == pos) return 0;
  return strndup(input + b, pos - b);
}

static inline int parse_term(void);

static inline int parse_var(void) {
  skip_ws();
  int b = pos;
  const char *n = parse_name();
  if (!n) return 0;
  term t;
  t.var = n;
  int id = alloc(0, VAR, t);
  return id;
}
int parse_include(void) {
    skip_ws();
    if (input[pos] != '"') return 0;
    ++pos;
    int old = pos;
    while (input[pos] && input[pos] != '"') ++pos;
    if (input[pos] != '"') return 0;
    ++pos;
    char buf[512];
    strncpy(buf, input + old, pos - old - 1);
    buf[pos - old - 1]=0;
    if (pos - old - 1 < 4 || strncmp(buf + pos - old - 5, ".coc", 4)) {
        strcat(buf + pos - old - 1, ".coc");
    }
    FILE *f = fopen(buf, "rb");
    char *dr = dirname(buf);
    int o = open(".", O_RDONLY | O_DIRECTORY);
    chdir(dr);
    char fbuf[16*1024];
    fbuf[fread(fbuf, 1, sizeof(fbuf), f)] = 0;
    const char *i = input;
    int p = pos;
    input = fbuf;
    pos = 0;
    int id = parse_term();
    fchdir(o);
    input = i;
    pos = p;
    return id;
}
static inline int parse_atom(void) {
  int b = pos;
  if (parse_str(",")) return parse_term();
  if (parse_str("*")) {
    int b = pos;
    while (isdigit(input[pos])) ++pos;
    int n = atoi(strndup(input + b, pos - b));
    term t;
    t.sort = n;
    int id = alloc(0, TYPE, t);
    return id;
  }
  int id = parse_var();
  if (id) return id;
  pos = b;
  id = parse_include();
  if (id) return id;
  pos = b;
  if (!parse_str("(")) return 0;
  id = parse_term();
  if (!parse_str(")")) return 0;
  return id;
}

static inline int subst(const char *j, int value, int id);

static inline int parse_let(void) {
  term t={0};
  const char *param = parse_name();
  t.lname = param;
  if (!t.lname) return 0;
  if (!parse_str(":")) return 0;
  t.ltype = parse_term();
  if (!t.ltype) return 0;
  if (!parse_str(";")) return 0;
  if (!parse_str(t.lname)) return 0;
  if (!parse_str("=")) return 0;
  t.lvalue = parse_term();
  if (!t.lvalue) return 0;
  if (!parse_str(";")) return 0;
  t.lbody = parse_term();
  if (!t.lbody) return 0;
  return alloc(0, LET, t);
}

static inline int parse_apps(void);
static inline int parse_anon(void) {
  term t={0};
  int b = pos;
  t.param = "_";
  t.type = parse_apps();
  if (!t.type) return 0;
  if (!parse_str("->")) return t.type;
  t.body = parse_term();
  if (!t.body) return 0;
  int id = alloc(0, PI, t);
  return id;
}

static inline int parse_lam(void) {
  term t={0};
  int b = pos;
  int pi = parse_str("&");
  if (!pi && !parse_str("\\")) return 0;
  const char *param = parse_name();
  t.param = param;
  if (!param) return 0;
  if (!parse_str(":")) return 0;
  t.type = parse_term();
  if (!t.type) return 0;
  if (!parse_str(".")) return 0;
  t.body = parse_term();
  if (!t.body) return 0;
  int id = alloc(0, pi ? PI : LAM, t);
  return id;
}

static inline int parse_decl(void) {
  term t={0};
  int b = pos;
  const char *param = parse_name();
  t.param = param;
  if (!param) return 0;
  if (!parse_str(":")) return 0;
  t.type = parse_term();
  if (!t.type) return 0;
  if (!parse_str(";")) return 0;
  t.body = parse_term();
  if (!t.body) return 0;
  t.special = 1;
  int id = alloc(0, LAM, t);
  return id;
}

static inline int parse_apps(void) {
  term t;
  int b = pos;
  t.func = parse_atom();
  if (!t.func) return 0;
  while (1) {
    t.arg = parse_atom();
    if (!t.arg) break;
    t.func = alloc(0, APP, t);
  }
  return t.func;
}

static inline int parse_term(void) {
  int b = pos;
  int id = parse_lam();
  if (id) return id;
  pos = b;
  id = parse_let();
  if (id) return id;
  pos = b;
  id = parse_decl();
  if (id) return id;
  pos = b;
  return parse_anon();
}

static inline int isfree(const char *j, int id) {
  term t = ts[id];
  if (tags[id] == TYPE) return 1;
  else if (tags[id] == VAR) return strcmp(j, t.var);
  else if (tags[id] == LAM || tags[id] == PI) {
    if (!isfree(j, t.type)) return 0;
    if (!strcmp(j, t.param)) return 1;
    return isfree(j, t.body);
  } else if (tags[id] == APP) {
    return isfree(j, t.func) && isfree(j, t.arg);
  } else {
    if (!isfree(j, t.ltype)) return 0;
    if (!isfree(j, t.lvalue)) return 0;
    if (!strcmp(j, t.lname)) return 1;
    return isfree(j, t.lbody);
  }
}

int unique=0;
static inline int subst(const char *j, int value, int id) {
  term t = ts[id];
  int source = 0;
  int old = id;
  if (tags[id] == TYPE) id = alloc(id, tags[id], t);
  else if (tags[id] == VAR) id = !strcmp(j, t.var) ? value : id;
  else if (tags[id] == LAM || tags[id] == PI) {
    t.type = subst(j, value, t.type);
    if (!strcmp(j, t.param)) id = alloc(id, tags[id], t);
    else {
        if (!t.special && !isfree(t.param, value)) {
          char b[32];
          sprintf(b, "_%d", unique++);
          term v = {.var = strdup(b)};
          int v2 = alloc(0, VAR, v);
          t.body = subst(t.param, v2, t.body);
          t.param = v.var;
        }
        t.body = subst(j, value, t.body);
        id = alloc(id, tags[id], t);
    }
  } else if (tags[id] == APP) {
    t.func = subst(j, value, t.func);
    t.arg = subst(j, value, t.arg);
    id = alloc(id, tags[id], t);
  } else {
    t.ltype = subst(j, value, t.ltype);
    t.lvalue = subst(j, value, t.lvalue);
    if (!isfree(t.lname, value)) {
      char b[32];
      sprintf(b, "_%d", unique++);
      term v = {.var = strdup(b)};
      int v2 = alloc(id, VAR, v);
      t.lbody = subst(t.lname, v2, t.lbody);
      t.lname = v.var;
    }
    t.lbody = subst(j, value, t.lbody);
    id = alloc(id, LET, t);
  }

  return id;
}

static inline int evaluate(int id) {
  term t = ts[id];
  if (tags[id] == TYPE || tags[id] == VAR) return id;
  else if (tags[id] == LAM || tags[id] == PI) {
    t.type = evaluate(t.type);
    t.body = evaluate(t.body);
    if (t.special) return t.body;
    return alloc(id, tags[id], t);
  } else if (tags[id] == APP) {
    t.func = evaluate(t.func);
    t.arg = evaluate(t.arg);
    if (tags[t.func] != LAM) {
      return alloc(id, APP, t);
    }
    term f = ts[t.func];
    return evaluate(subst(f.param, t.arg, f.body));
  } else {
    t.lvalue = evaluate(t.lvalue);
    term name;
    name.var = t.lname;
    return evaluate(subst(t.lname, t.lvalue, t.lbody));
  }
}

static inline int eq(int a, int b) {
  term x=ts[a], y=ts[b];
  if (tags[a] != tags[b]) return 0;
  else if (tags[a] == TYPE) return x.sort == y.sort;
  else if (tags[a] == VAR) return !strcmp(x.var, y.var);
  else if (tags[a] == LAM || tags[a] == PI) {
    if (!eq(x.type, y.type)) return 0;
    term p;
    p.var = x.param;
    int p2 = alloc(0, VAR, p);
    y.body = subst(y.param, p2, y.body);
    return eq(x.body, y.body);
  } else {
    return eq(x.func, y.func) && eq(x.arg, y.arg);
  }
}

static const char *ctx_names[NTERMS];
static int ctx_types[NTERMS];
static int ctx_count=0;

static inline int infer(int id) {
  term t = ts[id];
  if (tags[id] == TYPE) {
    t.sort++;
    return alloc(0, TYPE, t);
  }
  else if (tags[id] == VAR) {
    for (int i=ctx_count-1; i >= 0; --i) {
      if (!strcmp(ctx_names[i], t.var)) {
        return ctx_types[i];
      }
    }
    printf("Unbound identifier: %s\n", t.var);
    exit(1);
  } else if (tags[id] == LAM || tags[id] == PI) {
    int tyty = infer(t.type);
    t.type = evaluate(t.type);
    ctx_names[ctx_count] = t.param;
    ctx_types[ctx_count++] = t.type;
    t.body = infer(t.body);
    if (t.special) return t.body;
    --ctx_count;
    if (tags[id] == PI) {
      assert(tags[tyty] == TYPE);
      assert(tags[t.body] == TYPE);
      return t.body;
    }
    return alloc(0, PI, t);
  } else if (tags[id] == APP) {
    int ty = infer(t.func);
    int aty = infer(t.arg);
    term f = ts[ty];
    if (tags[ty] != PI) {
      printf("Trying to call ");
      print_term(t.func);
      printf(", which is not a lambda\n");
      exit(1);
    };
    if (!eq(aty, f.type)) {
      print_term(aty);
      printf("\n");
      print_term(f.type);
      printf("\n");
      print_term(id);
      exit(1);
    }
    t.arg = evaluate(t.arg);
    f.body = evaluate(subst(f.param, t.arg, f.body));
    return f.body;
  } else {
    infer(t.ltype);
    t.ltype = evaluate(t.ltype);
    int aty = infer(t.lvalue);
    if (!eq(aty, t.ltype)) {
      print_term(aty);
      printf("\n");
      print_term(t.ltype);
      printf("\n");
      exit(1);
    }
    t.lvalue = evaluate(t.lvalue);
    term name;
    name.var = t.lname;
    return infer(subst(t.lname, t.lvalue, t.lbody));
  }
}

FILE *out;
void compile(int id) {
  term t = ts[id];
  if (tags[id] == VAR) fprintf(out, "%s", t.var);
  else if (tags[id] == LAM) {
    fprintf(out, "[=](auto %s) { return ", t.param);
    compile(t.body);
    fprintf(out, "; }");
  } else if (tags[id] == APP ){
    compile(t.func);
    fprintf(out, "(");
    compile(t.arg);
    fprintf(out, ")");
  } else {
    fprintf(out, "0");
  }
}

int main(int argc, char **argv) {
  assert(argc >= 2);
  char *buf = malloc(16*4096);
  memset(buf, 0, 16*4096);

  ts = malloc(sizeof(*ts)*NTERMS);
  tags = malloc(sizeof(*tags)*NTERMS);

  int comp = 0;
  if (!strcmp(argv[1], "compile")) {
      comp = 1;
      argc--;
      argv++;
      assert(argc >= 2);
  }

  char *olddr = realpath(".", NULL);
  for (int i=1; i < argc; ++i) {
    const char *file_name = argv[i];
    ts_count = 1;
    ctx_count = 0;
    pos = 0;
    unique = 0;

    FILE *f = fopen(argv[i], "rb");
    int n = fread(buf, 1, 16*4096, f);
    fclose(f);
    buf[n] = 0;
    input = buf;

    char *dr = realpath(dirname(argv[i]), NULL);
    chdir(dr);

    char msg[256];
    sprintf(msg, "checking %s...", file_name);
    printf("%-40s", msg);
    fflush(stdout);

    int v = parse_term();
    if (!v) exit(1);
    skip_ws();
    if (input[pos]) {
        printf("%s\n", input + pos);
        return 1;
    }
    infer(v);
    
    if (comp) {
        out = fopen("out.cpp", "w+");
        fprintf(out, "int main() {\n");
        fprintf(out, "    ");
        compile(evaluate(v));
        fprintf(out, ";\n");
        fprintf(out, "}\n");
        fclose(out);
        system("clang out.cpp -include lib.cpp -O3 -std=c++26 -o out");
        system("clang out.cpp -include lib.cpp -O3 -std=c++26 -S");
    } else {
        printf("%s\n", "ok");
        fflush(stdout);
        chdir(olddr);
    }
  }
}

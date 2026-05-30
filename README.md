# C-Like Language Interpreter

A tree-walking interpreter for a simplified C-style language, written from
scratch in C++. It takes source code as text and runs it end to end — through
lexical analysis, parsing, scope resolution, and evaluation — with no external
parser or interpreter libraries. Every stage of the language pipeline is
hand-built.

The project exists to understand, concretely, what happens between typing a
program and seeing it run: how raw characters become tokens, how tokens become
a tree, how scope and types are tracked, and how that tree is walked to produce
behavior.

## The language

It interprets a C-like language with functions, procedures, typed variables,
arithmetic and boolean expressions, and control flow. A representative program:

```c
function int sum_of_first_n_squares (int n)
{
  int sum;

  sum = 0;
  if (n >= 1)
  {
    sum = n * (n + 1) * (2 * n + 1) / 6;
  }
  return sum;
}

procedure main (void)
{
  int n;
  int sum;

  n = 100;
  sum = sum_of_first_n_squares (n);
  printf ("sum of the squares of the first %d numbers = %d\n", n, sum);
}
```

Supported features:

- Functions (with return values) and procedures
- Typed variable declarations with lexical scoping
- Arithmetic, relational, and boolean expressions with correct precedence
- `if` and `while` control flow
- Nested block scopes
- `printf`-style formatted output
- Comment stripping and basic error detection/reporting

## How it works

Source code flows through a five-stage pipeline, each stage in its own module:

```
source text
   │
   ▼
remove_comments      Strips comments before tokenizing
   │
   ▼
Tokenizer            A 28-state DFA that scans characters into typed Tokens
   │                 (identifiers, literals, operators, keywords, punctuation)
   ▼
RecursiveDescentParser   Consumes tokens and builds the parse structure,
   │                     enforcing the grammar via mutually recursive routines
   ▼
AbstractSyntaxTree   The program represented as a tree of nodes that captures
   │                 structure and precedence independent of surface syntax
   ▼
SymbolTable          Records declarations, types, and scope so identifiers can
   │                 be resolved correctly across nested blocks
   ▼
Interpreter          Walks the AST, evaluating expressions and executing
                     statements — starting from `main` and following calls
```

Notable implementation choices:

- **The lexer is an explicit deterministic finite automaton** (28 states)
  rather than ad-hoc character matching, so token boundaries (e.g.
  distinguishing `=` from `==`, handling string literals) are handled by
  well-defined state transitions.
- **Recursive descent for parsing** — one routine per grammar rule, with the
  call stack mirroring the grammar's structure. This makes the parser readable
  and the operator-precedence handling explicit.
- **A separate symbol table pass** keeps scope resolution distinct from
  evaluation, so the interpreter can look up an identifier's declaration and
  scope without re-deriving it during execution.

## Building

Requires a C++ compiler supporting C++14 (g++ or clang++).

### With g++ (primary)

From the project root:

```bash
g++ -std=c++14 -I. -o interpreter \
  main.cpp remove_comments.cpp Tokenizer.cpp Token.cpp \
  RecursiveDescentParser.cpp SymbolTable.cpp \
  AbstractSyntaxTree.cpp Interpreter.cpp
```

On Windows (PowerShell), keep it on a single line and name the output `.exe`:

```powershell
g++ -std=c++14 -I. -o interpreter.exe main.cpp remove_comments.cpp Tokenizer.cpp Token.cpp RecursiveDescentParser.cpp SymbolTable.cpp AbstractSyntaxTree.cpp Interpreter.cpp
```

### With CMake (alternative)

A `CMakeLists.txt` is included for IDE/CMake users:

```bash
cmake -B build -S .
cmake --build build
```

## Running

The interpreter prompts for two file paths on startup: an input source file and
an output file. It first strips comments from the input into the output file,
then tokenizes, parses, builds the symbol table and AST, and executes the
program — printing the program's output at the end.

```bash
# Linux / macOS
./interpreter

# Windows (PowerShell) — note the leading .\
.\interpreter.exe
```

When prompted, give it one of the sample programs, e.g.:

```
Enter the input file name: test-1.c
Enter the output file name: out.txt
```

Expected output for that program:

```
sum of the squares of the first 100 numbers = 338350
```

## Project structure

```
main.cpp                  Entry point
Token.*                   Token types and the Token class
Tokenizer.*               DFA-based lexical analyzer
remove_comments.*         Comment stripping pass
RecursiveDescentParser.*  Grammar-driven parser
AbstractSyntaxTree.*      AST node definitions and construction
SymbolTable.*             Scope and declaration tracking
Interpreter.*             Tree-walking evaluator
CMakeLists.txt            CMake build configuration
*TestFiles/               Sample programs exercising each stage
```


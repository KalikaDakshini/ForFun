## Lexer

- [ ] Lexer() takes an input stream
- [ ] next() function returns token
- [ ] END token at the end of file

Tokens to match

```regexp
[1-9][0-9]*				INT
[1-9][0-9]*(.[0-9]*)	FLOAT
+						ADD
*						MUL
[[:space]]				skip
.						discard
```

## Parser

Recursive descent parser to parser a right-recursive, left-factored grammar
with backtracking

- [ ] Add rules
- [ ] Implement backtracking

```bnf
E : T E'

E' : + E | * E | null

T : num | ( E )
```

## Main Language

- [ ] Lexer and Parser as seperate modules
- [ ] Call Lexer on input stream and call Parser on Lexer

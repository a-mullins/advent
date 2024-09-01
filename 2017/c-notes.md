- `const` is not used in this code at the moment. These solutions are small
  and there is only one author. Also, where do you use it? It is pointless on
  params that are passed by value, so....
- TODO: implement const
- Generally, errors from *alloc()s are not checked, nor are the implicit
  allocs of getline, &c. This is bad practice in production, but acceptable
  here.
- Is there a way to get the compiler to warn about unknown-param funct
  declarations? In other words, it should flag `void foo();` but not `void
  foo(void);`
- In function param, prefer passing pointers instead of arrays. That is,
  `foo(int *matrix)` vs `foo(int matrix[])`.
- Avoid VLAs, they make stack frame sizes unpredictable and potentially
  bloated.
- sizeof (int) not sizeof(int), to emphasize that sizeof is a statement not a
  function.
- Clever trick for trimming a string: `buf[strcspn(buf, "\r\n")] = '\0';`

> Compared to later standards, ANSI C (aka C89 or C90) is
> interesting. It strongly encourages the programmer to lay out their
> functions in a manner corresponding to their runtime stack
> frames. For example, all vars declared at the start of the block, no
> variable-length arrays (because frame size will be known at compile
> time), and no mixing code and declarations. That is, `short *digits
> = strlen(buf)` is not allowed since, when the frame is created,
> space is made for data _and then_ the code is executed.
>
> Anyway, if, like me, you have wondered if there's a concrete reason
> why some style guides encourage putting all declarations at the top
> of a block... well, here is some historical context.
>
> Vernor Vinge was thinking ahead when he made some of his characters
> Software Archeologists. 🤓

- day07 is getting at the idea of self-balancing trees, like AVL or
  Red-Black trees.

# 2017: C #

## General C Notes ##
In these solutions, errors from alloc()s are not checked for failure, neither
are the implicit allocs of getline. This is bad practice in production, but
acceptable here.

Is there a way to get the compiler to warn about unknown-param funct
declarations? In other words, it should flag `void foo();` but not `void
foo(void);`

In function param, prefer passing pointers instead of arrays. That is,
`foo(int *matrix)` vs `foo(int matrix[])`. (why?) The latter is a pointer
anyway, so perhaps using the former is more honest.

The latter declares a variable length array. VLAs should be avoided since they
make stack frame sizes unpredictable and potentially large. This may seem
related to the previous point, but they are two seperate issues.

`sizeof (int)` not `sizeof(int)`, to emphasize that sizeof is a statement not a
function.

Clever trick for trimming a string: `buf[strcspn(buf, "\r\n")] = '\0';`

## ANSI C v later standards ##

If, like me, you have wondered if there's a concrete reason why some C style
guides encourage putting all declarations at the top of a block, here is some
historical context that I found.

Compared to later standards, ANSI C (aka C89 or C90) is interesting. It
strongly encourages the programmer to lay out their functions in a manner
corresponding to their runtime stack frames. For example, all vars declared at
the start of the block, no variable-length arrays (because frame size will be
known at compile time), and no mixing code and declarations. That is, `short
*digits = strlen(buf)` is not allowed since, when the frame is created, space
is made for data _and then_ the code is executed.

Vernor Vinge was thinking ahead when he made some of his characters Software
Archeologists. 🤓

## Day 03 ##
```
17  16  15  14  13
18   5   4   3  12
19   6   1   2  11
20   7   8   9  10
21  22  23---> ...
```

Note that the bottom left corners of the squares form a sequence
of squares of odds.
eg: 1^2 = 1, 3^2 = 9, 5^2 = 25....

So if we have an index `i`, we can find which odd-square is above it by
`f(i) = if ceil(sqrt(i)) then ceil(sqrt(i)); else ceil(sqrt(i))+1`.
eg:
```
  i : 22 17 15 10 8 3 1 ...
f(i):  5  5  5  5 3 3 1 ...
```

also note that:
```
   j:  0  1  2  3 ...
2j+1:  1  3  5  7 ...
```

So if we know the odd-square above `i`, we can find that odd square's
coordinates by rearranging `j = 2k+i  ⟺  k = (j-1)/2.`

Then we can count backwards, turning at square border, which is
square units away from the last turn.

## Day 07 ##
Day 07 is getting at the idea of self-balancing trees, like AVL or
Red-Black trees, a little bit.

# Advent of Code
My personal Advent of Code solutions.

## Notes on JS
- Almost no JS host supports tail call elimination. Prefer iteration to
  recursion.

- === & == are only useful for primatives. For objects they compare pointer
  values. Many Array methods rely on === for searching, so only use those for
  Arrays of primatives. Array.some() is higher-order & allows a function as a
  predicate, so will be more useful for arrays of objects.

- instanceof has similar problems when applied to complex objects, becaus any
  object is just. 'object'. However, classes often implement a method to
  recognize their own, eg Array.isArray().
# Advent of Code
My personal Advent of Code solutions.

### Notes on js
- Few js hosts support tail call elimination. Prefer iteration to recursion.

- js does not support operator overloading. So === & == are only useful for
  primatives. For objects they compare pointer values. That means objects are
  only equal if they are pointers to the exact same chunk of memory--equality
  of data stored in the structures isn't consedired. Many Array methods rely on
  === for search, so only use those methods for Arrays of
  primatives. Array.some(), .find(), etc, are higher-order & allow a function
  as a predicate, so will be more useful for arrays of objects.

- typeof has similar problems when applied to complex objects because again,
  for any object, (typeof obj) === 'object', not its class. That makes typeof
  of only marginal value when trying to restrict a function's input types,
  etc. 'instanceof' is useful here.

- Importing modules in the js ecosystem is a complete mess. That is all.

- Minor changes in behavior between puzzle parts can be consolidated into a
  single file by using the filename in argv.

- day11: if a-star is necessary and we're tracking weights in a seperate array,
  why not actually apply the weights to a DiGraph and use a dfs which chooses
  the path with the lowest weight? I've already implemented dfs & graph nodes.

- day11: critical than h(n) be admissable!

- day13: multi-dimensional array footgun:
  
  `new Array(this.maxY).fill(new Array(this.maxX).fill("_"));`

  will create an array where every row is a shallow refence to the same object.
  This must be done instead:

  `Array.from(Array(this.maxY), () => new Array(this.maxX));`
  
- This does not behave like it should. Can you spot the error?
  (hint, look at hash's arg and see if it's used in the function body.
   no error is thrown since i is in scope.
   ...why is i in scope?)

  function hasTriple(s) {return s.match(/(.)\1{2}/) ? true : false;}
  function hasQuint(s)  {return s.match(/(.)\1{4}/) ? true : false;}
  function hash(idx) {
      return crypto.createHash("md5").update(salt + String(i)).digest("hex");
  }

  //const salt = fs.readFileSync(0, "ascii").trim("\n");
  const salt = "abc";
  const hashes = [];
  const keys = [];

  let i = 0;
  while(i < 18) {
      let isKey = false;
      // iterate forward until triple.
      if(!hashes[i]) {
          hashes[i] = hash(i);
      }
      if(hasTriple(hashes[i])) {
          console.log(`found trip @ i${i} , looking for quint`);
          // if there is a quintuple in the next 1000 hashes, hash[i] is a key.
          for(let j = i+1; j < (i+1000); j++) {
              if(!hashes[j]) {hashes[j] = hash(j);}
              console.log(`j = ${j}; checking hashes[j] = ${hashes[j]}`);
              if(hasQuint(hashes[j])) {
                  isKey = true;
                  break;
              }
          }
          console.log("No quint.");
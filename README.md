# Non Deterministic Turing Machine Simulator

Project for "Algorithm and Principles of Computer Science" course of Politecnico di Milano.

**The main focus of the entire project is on complex data structures**

## Specific

- **Standard C language** implementation (with **libc only**) of an interpreter
of **non-deterministic Turing machines**, in the **single-ribbon variant** and
**acceptors only**.
- **Input file** structure: transition function is provided first,
then the acceptance states and a maximum limit on the number of steps to
perform for a single computation (to banally avoid the
problem of non-terminating machines), finally a series of strings
to be read by the machine.
- On **exit**, a file is expected containing 0 for unaccepted strings and 1
for those accepted; as there is also a limit on the number of steps, the
result can also be U if acceptance has not been reached.
- For simplicity, the ribbon symbols are chars, while the states are
int. The "_" character indicates the "blank" symbol.
- The machine always starts from state 0 and from the first character of the
input string.
- It is assumed, as usual, that the tape is unlimited on both left and a
right and that contains the character "_" in each position.
- The characters "L", "R", "S" are used, as usual, for moving the
head.
- The input file is provided via the standard input, while
the output is on standard output.

## Input file
The input file is divided into 4 parts:
- The first part, starting with "tr", contains the transitions, one per line 
each character can be separated from the others by spaces.
For example: 0 to c R 1 means that you go from state 0 to state 1, reading a and
writing c; the head is moved to the right (R).
- The next part, starting with "acc", lists the acceptance states, one
per line.
- To avoid problems of infinite computations, the next section, that
starts with "max", it indicates the maximum number of moves that can be made to accept a string.
- The final part, starting with "run", is a list of strings to be supplied to the
machine, one per line.

### Input file example

es. (MT non-det. per WW^(R) )

tr

0 a a R 0

0 b b R 0

0 a c R 1

0 b c R 2

1 a c L 3

2 b c L 3

3 c c L 3

3 a c R 4

3 b c R 5

4 c c R 4

4 a c L 3

5 c c R 5

5 b c L 3

3 _ _ R 6

6 c c R 6

6 _ _ S 7

acc

7

max

800

run

aababbabaa

aababbabaaaababbabaa

aababbabaaaababbabaab

aababbabaaaababbabaabbaababbabaaaababbabaa

aababbabbaaababbabaabbaababbabaaaababbabaa


===>Standard output:

1

1

0

U

0

### More details on the Transition function

Each line of the function is coded following the expression: (the character [] indicates a space character "")

**departure_state [] read_char [] written_char [] movement [] arrival_status**

- Characters are encoded as char
- The states are encoded as int (the initial state is by convention the state 0)
- The "_" character indicates the blank
- Head movement is encoded using characters
   - “L” Left
   - "R" Right
   - “S” Stop
- The transfer function may not be sorted by starting state
- Assume that if the state N exists then the states N-1, N-2,…, 0 also exist

## Output

The output must be provided on stdout.
For each input string (one value per line):
- 1 if the string is accepted
- 0 if the string is not accepted
- U if the computation does not finish within the specified maximum number of steps

# Non-Deterministic-Turing-Machin-Simulator

Project for "Algorithm and Principles of Computer Science" course of Politecnico di Milano.

**The main focus of the entire project is on complex data structures**

## Specific

- **Standard C language** implementation (with **libc only**) of an interpreter
of non-deterministic Turing machines, in the single-ribbon variant and
acceptors only.
- **Input file** structure: transition function is provided first,
then the acceptance states and a maximum limit on the number of steps to
perform for a single computation (to banally avoid the
problem of non-terminating machines), finally a series of strings
to be read by the machine.
- On exit, a file is expected containing 0 for unaccepted strings and 1
for those accepted; as there is also a limit on the number of steps, the
result can also be U if acceptance has not been reached.
- For simplicity, the ribbon symbols are chars, while the states are
int. The "_" character indicates the "blank" symbol.
- The machine always starts from state 0 and from the first character of the
input string.
- It is assumed, as usual, that the tape is unlimited on both left and a
right and that contains the character "_" in each position.
- The characters "L", "R", "S" are used, as usual, for the movement
head.
- The input file is provided via the standard input, while
the output is on standard output.

Il file di ingresso è diviso in 4 parti:
- La prima parte, che inizia con "tr", contiene le transizioni, una per linea -
ogni carattere può essere separato dagli altri da spazi.
Per es. 0 a c R 1 significa che si va dallo stato 0 allo stato 1, leggendo a e
scrivendo c; la testina viene spostata a destra (R).
- La parte successiva, che inizia con "acc", elenca gli stati di accettazione, uno
per linea.
- Per evitare problemi di computazioni infinite, nella sezione successiva, che
inizia con "max", viene indicato il numero di mosse massimo che si possono
fare per accettare una stringa.
- La parte finale, che inizia con "run", è un elenco di stringhe da fornire alla
macchina, una per linea.

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

## Input
Il formato dell'input è il seguente
● tr
● funzione di transizione
● acc
● stati finali (di accettazione)
● max
● numero massimo di passi
● run
● stringhe di input
Funzione di transizione
Ogni riga della funzione è codificata seguendo l’espressione:
(il carattere [ ] indica un carattere spazio “ “)
stato_partenza[ ]carattere_letto[ ]carattere_scritto[ ]movimento[ ]stato_arrivo
● I caratteri sono codificati come char
● Gli stati sono codificati come int
○ Lo stato iniziale è convenzionalmente lo stato 0
● Il carattere ”_” indica il blank
● Il movimento della testina è codificato usando i caratteri
○ “L” Left
○ “R” Right
○ “S” Stop
● La funzione di trasferimento può non essere ordinata per stato di partenza
● Si assuma che se esiste lo stato N allora esistono anche gli stati N-1, N-2, …, 0
Stati finali
Gli stati finali (se presenti) sono elencati uno per riga.
Si assuma che non esistano archi uscenti dagli stati finali.
Numero massimo di passi
In caso di macchina non-deterministica questo parametro si riferisce al singolo percorso
non-deterministico.
Stringhe di input
Non ci sono vincoli né sul numero delle stringhe di input né sulla loro lunghezza.
Ogni stringa è posta su una riga.
Configurazione iniziale
Macchina nello stato 0 con la testina posizionata sul primo carattere della stringa di
ingresso, ovviamente inserita nel consueto nastro singolo sbiancato bi-infinito.
Output
L’output deve venire fornito su stdout.
Per ogni stringa di ingresso (un valore per riga):
● 1 se la stringa viene accettata
● 0 se la stringa non viene accettata
● U se la computazione non termina entro il numero massimo di passi specificato

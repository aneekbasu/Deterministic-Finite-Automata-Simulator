# Deterministic Finite Automata Simulator


- It reads in a specific machine definition (5 tuple definition) in json format and process input strings against this DFA; output ACCEPT or NOT ACCEPT for each string.
- Support json definition for machine; read machine defined in json; also output json for machine
definition in verbose mode.


# Sample Commands

- Compile the program
``` g++ -o DFA -g DFA.cpp ```
- Header
``` ./DFA -h ```
- Run
``` ./DFA -d <DFA Definition file i.e. json file> ```
- Run in verbose mode
``` ./DFA -d <DFA Definition file in json format> -v ```
- Run with i flag
```./DFA -d <DFA Definition file i.e. json file> -i <File containning input strings>```
- Run with iinput and output flags
```./DFA -d <DFA Definition file i.e. json file> -i <File containning input strings> -o <File that wil contain the output>```
- Run with input, ooutput and verbose flags
```./DFA -d <DFA Definition file i.e. json file> -i <File containning input strings> -o <File that wil contain the output> -v```

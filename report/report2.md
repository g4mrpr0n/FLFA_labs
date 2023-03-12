# Implementation of a formal language grammar and the corresponding finite automaton.

### Course: Formal Languages & Finite Automata
### Author: Globa Georgeana

----

## Theory
An alphabet is a collection of characters that can form strings, or words, that belong in an alphabet. 
A grammar is a set of 4 components - terminal characters, nonterminal characters, productions and first character.
The first character is a nonterminal symbol used at the beginning of each word. Terminal characters are symbols which cannot transition to others defined in the productions.
Nonterminal characters, on the other hand, can have a transition to other combination of terminal and nonterminal characters.
The production is the set of rules that defines which combinations can form words using the set Vn and Vt.
An automaton is a model of a finite-state machine. It can check whether or not a word belongs to any given alphabet.
It is formed of a set of 5 things - a set of states (Q), an alphabet (sigma), a transition function (delta), starting state (q0) and final states (F).

## Objectives:

* Provide a function in your grammar type/class that could classify the grammar based on Chomsky hierarchy.
* Implement conversion of a finite automaton to a regular grammar.
* Determine whether your FA is deterministic or non-deterministic.
* Implement some functionality that would convert an NDFA to a DFA.

## Implementation description

##### In order to avoid OOP issues, I have switched the inheritance the other way around. (Class Grammar now inherits class FiniteAutomaton and previous tasks don't work.)

* Class Grammar has been changed, initialization of all private attributes happens only in constructors, and the default one initializes it with the variant from the previous lab.
```cpp
class Grammar : public FiniteAutomaton
{
	
private:
	string P[7];
	char Vn[3] , Vt[4] , S;
	string word;
public:
	friend class FiniteAutomaton;
	Grammar() :word(""), P{ "S->aS", "S->bB", "B->cB", "B->aD", "B->d", "D->aB", "D->b" }, Vn{ 'S','B','D' }, Vt{'a','b','c','d'}, S('S') {}
}
```
* Parametrized constructor added, for conversion from FA to Grammar.
It follows the same logic as the one from the FA class.
```cpp
	Grammar(FiniteAutomaton &f, int prod, int sig, int q, char last)
	{
		S = f.getQ0();
		string arrow = "->";
		for (int i = 0; i < prod; i++)
		{
			string combo = "";
			char leftside = f.getdel()[i][0];
			char label = f.getdel()[i][2];
			char rightside = f.getdel()[i][4];
			if (rightside == last)
			{
				rightside = ' ';
			}
			combo += leftside + arrow + label + rightside;
			P[i] = combo;
			if (i < q)
			{
				this->Vn[i] = f.getQ()[i];
			}
			if (i < sig)
			{
				this->Vt[i] = f.getsigma()[i];
			}
		}
		P[prod] = last + arrow + ' ';
	}
```


* Method called `flagNonTerminal(string,string&)` was created to add the indices of all nonterminals (on the right side of a production) in a string passed by reference.
```cpp
	void flagNonTerminal(string s, string &flags) {
		for (int i = 0; i < s.length(); i++)
		{
			if (s[i] <= 90)
			{
				flags.append(to_string(i));
			}
		}
	}
```

* Method called `classification()` labels given Grammar according to Chomsky classification.

TLDR: It iterates through each production, updating the value of `type` integer variable according to new assumptions.
First, the production is split to left side and right side strings.
Then, it's being checked on following rules:
	*If left side is bigger than one character and the right side is empty, assumed type is 0.
	*Otherwise, assumed type is 1, and if it's lower than the current type, it's updated.
	*If there are no nonterminals on the right side, assumed type is 3.
	*If there's more than one nonterminal on the right side, assumed type is 2.
	*If there's only one nonterminal, and it's on the very left side of the right side, with no other characters following, assumed type is 2.
	*Otherwise, it checks whether it's a left side grammar or right side grammar.
	*If there will be contradictions in the grammar(one productions are left side and others are right side), assumed type is 2.
	*Otherwise, type will be 3. 
```cpp
void classification()
	{
		int type;
		for (int i = 0; i < 7; i++)
		{
			int j = 0, assumedType = 0, side = 0;
			type = 0;
			string  leftside="", rightside="", flags="";
			bool b = false;
			string cProduction = P[i];
			while (j < cProduction.length())
			{
				if (cProduction[j] != '-' && !b)
				{
					leftside+=cProduction[j];
					j++;
				}
				else if (cProduction[j] =='-') {
					b = true;
					j += 2;
				}
				else {
					rightside += cProduction[j];
					j++;
				}
			}

			if (leftside.length() > 1)
			{
				if (rightside[0]== '\0')
				{
					assumedType = 0;
				}
				else {
					assumedType = 1;
					if (assumedType > type)
					{
						type = assumedType;
					}
				}
			}

			else {
				flagNonTerminal(rightside, flags);
				if (flags.length() == 0)
				{
					assumedType = 3;
					if (assumedType > type)
					{
						type = assumedType;
					}
				}
				else if (flags.length() > 1)
				{
					assumedType = 2;
					if (assumedType > type)
					{
						type = assumedType;
					}
				}

				else {
					int index = flags[0] - 48;
					if (index == 0)
					{
						if (rightside[++index] == '\0')
						{
							assumedType = 2;
							if (assumedType > type)
							{
								type = assumedType;
							}
						}
						else {
							if (side == 0)
							{
								assumedType = 3;
								side = 1;
								if (assumedType > type)
								{
									type = assumedType;
								}
							}
							if (side == 2)
							{
								type = 2;
								side = 0;
							}
						}
					}

					else {
						if (rightside[++index] == '\0' && side==0)
						{
							assumedType = 3;
							if (assumedType > type)
							{
								type = assumedType;
							}
							side = 2;
						}
					}
				}
			}
		}
		cout << type;
	}
```

* Method `checkNFA()`.
It iterates through each production, and checks whether there are transitions with the same state and label more than once.
```cpp
bool checkNFA()
    {
        for (int i = 0; i < 7; i++)
        {
            char leftside = delta[i][0];
            char label = delta[i][2];
            for (int j = i+1; j < 7; j++)
            {
                if (delta[j][0] == leftside && delta[j][2] == label)
                {
                    return true;
                }
            }
        }
        return false;
    }
```

* Getters were added in `FiniteAutomaton` class
```cpp
char getQ0()
    {
        return q0;
    }
    char* getQ()
    {
        return Q;
    }
    char* getsigma()
    {
        return sigma;
    }
    string* getdel()
    {
        return delta;
    }
```

* `delt()` method has been changed.
Now, we have an index which will be updated according to last finding (this will be useful in conversion function).
The first loop is in case we want the loop to start at 0 always.
```cpp
    char delt(char q, char sigm, int &index)
    {
        int in;
        if (index == -1)
        {
            int in = 0;
            for (in; in < 7; in++)
            {
                if (delta[in][0] == q)
                {
                    if (delta[in][2] == sigm)
                    {
                        return delta[in][4];
                    }
                }
            }
        }
        else {
            for (index; index < 7; index++)
            {
                if (delta[index][0] == q)
                {
                    if (delta[index][2] == sigm)
                    {
                        return delta[index][4];
                    }
                }
            }
        }
        return 'n';
    }
```

* Method `NFADFA()` converts given automaton from NFA to DFA.
It's a lot to explain, so here's a TLDR:
It builds the table for the new DFA, while keeping track of the new states in another array.
It iterates through the known states first, identifying new states and filling in the `table[][]` variable at any iteration.
During each iteration, the `delta` array is being checked every time, from start to finish (very unomptimized).
After the default states are checked, it goes to the new states and performs pretty much the same checks.
This time, however, if it encounters a composite state, checks whether that state has already been identified or not.
```cpp
void NFADFA()
    {
        string table[30][30], states = "", newstates[20];
        bool converted = false;
        int j = 0, index = 0, newstate = 3;
        for (j = 0; j < 3; j++)
        {
            char state = j + 48;
            for (int i = 0; i < 3; i++)
            {
                char m = sigma[i];
                while (index < 7)
                {
                    char x = delt(state, m, index);
                    int y = x;
                    y = y - 48;
                    if (x != 'n' && x != '\0')
                    {
                        states.append(to_string(y));
                    }
                    index++;
                }
                if (states.length() == 1)
                {
                    table[j][i] = states;
                }
                else if (states.length() > 1)
                {
                    newstates[newstate - 3] = states;
                    table[j][i] = states;
                    newstate++;
                }
                else {
                    table[j][i] = 'n';
                }
                states = "";
                index = 0;
            }
        }
        string otherstates = "";
        while (j < newstate)
        {
            states = newstates[j-3];
            
            for (int i = 0; i < 3; i++)
            {
                char state = sigma[i];
                for (int m = 0; m < states.length(); m++)
                {
                    char f = states[m];
                    index = 0;
                    while (index < 7)
                    {
                        char x = delt(f, state, index);
                        int y = x;
                        y = y - 48;
                        if (x != 'n' && x != '\0')
                        {   
                            otherstates.append(to_string(y));
                        }
                        index++;
                    }     
                }
                if (otherstates.length() == 1)
                {
                    table[j][i] = otherstates;
                }
                else if (otherstates.length() > 1)
                {
                    for (int o = newstate - 3; o >= 0; o--)
                    {
                        if (otherstates == newstates[o])
                        {
                            table[j][i] = otherstates;
                            goto label;
                        }
                    }
                    newstates[newstate - 3] = otherstates;
                    table[j][i] = otherstates;
                    newstate++;
                }
                else {
                    table[j][i] = 'n';
                }
            label:
                otherstates = "";
            }
            j++;
        }
    
        

        for (int i = 0; i < newstate; i++)
        {
            for (int k = 0; k < j; k++)
            {
                cout << table[i][k] << " ";
            }
            cout << "\n";
        }
    }
```
## Conclusions / Screenshots / Results
In conclusion, this implementation took more time than expected but I'm really happy that it's done. I haven't checked
how compatible it is with other types, but there is a little bit of hardcoding done. I still tried to make it as generic
as possible but some things were just too much to do and to implement. I'm also happy to say that the code is done in 
(almost) pure C, and without external libraries (only `string.h` for `s.length()` function).

## References
none were used.

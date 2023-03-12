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

* Implement 2 classes - Grammar and Finite Automaton;
* Add a function that would generate 5 valid strings from the language expressed by the grammar 
* Implement functionality that could convert and object of type Grammar to one of type Finite Automaton
* Add a method that checks if an input string can be obtained via the state transition from it.

## Implementation description

* Class Grammar has been added, it has one string array (for the productions) and 2 char arrays (Vn, Vt), the starting character S and the word as private attributes;
```cpp
	class Grammar
	{
	private:
		string P[7] = { "S->aS", "S->bB", "B->cB", "B->aD", "B->d","D->aB", "D->b" };
		char Vn[3] = { 'S','B','D' }, Vt[4] = { 'a','b','c','d' }, S='S';
		string word;
	}
```
* Default constructor and parametrized constructor added - they initialize the word attribute with empty/given string.
```cpp
	Grammar() :word(""){}
	Grammar(const string w) {
		this->word = w;
	}
```

* Of course, we have getters for all the private attributes:
```cpp
	string getWord()
	{
		return word;
	}
	char* getVn()
	{
		return Vn;
	}
	char* getVt()
	{
		return Vt;
	}
	string* getP()
	{
		return P;
	}
```

* Method called `findGoodcombibation()` was created for the purpose of  iterating through the productions and find all indexes that match the  last non-terminal character in the current word, returning it as a string of integers.
It assumes that the current version of the word has only one non-terminal character at the end
of the string, and it assumes that every production has only one non-terminal character on the left side.
```cpp
	string findGoodcombination()
	{
		string x = "";
		for (int i = 0; i < 7; i++)
		{
			if (P[i][0] == word[word.length()-1]) {
				x.append(to_string(i));
			}
		}
		return x;
	}
```

* Method called `transition(int n)` was created to apply a production to the word.
The n variable is the index of the chosen production.
It will iterate through the production string at index n until it finds the arrow and adds everything afterwards to a temporary string that is being returned at the end.
The condition is that i should be at any but the first iteration, and the previous character in production should be the arrow.
Not sure why I put `x!=""`... oh well.
At the end it pops the last non-terminal character and returns our new string.
```cpp
string transition(int n)
	{
		string x = "";
		for (int i = 0; i < P[n].length(); i++)
		{
			if (((i > 0) && (P[n][i-1] == '>')) || x!="")
			{
				x = x + P[n][i];
			}
		}
		word.pop_back();
		return x;
	}
```

* The actual method that generates strings `generateString()`
This method is pretty straight-forward. If there already is an existing word inside the object.word attribute,
it resets it. It then adds the first character in any string S. The rng variable is for determining how many
transitions will happen. Then, a simple for loop that will make those **rng** transitions.
x is the string made of good possible combos for the current non-terminal character. 
Firstly are covered the 2 edge cases - arriving at the last transition and needing to make the last character terminal;
Arriving at the second to last transition, and the non-terminal character being one that does not have a direct terminal transition
(having to transition to another nonterminal and then terminal);
and otherwise it's normal transitions.
The first edge case abuses the fact that the productions are sorted in such a way that the combo string's last index will be the one
with the terminal character, and sets as the index choice will be that and perform the transition.
The second case abuses the fact that there's only one nonterminal character with no direct terminal choice and it has 2 transitions to perform
in order to finish the word, so i hardcoded it to go the easy way and just add the missing characters instead of actually going through more functions.
If it's a normal transition, the index choice will be made by the `rand()` funcion mod the choice string's length and add the value of the first 
index so it starts from there (if the sequence is "234" it will add 2 so it starts from the right place).
Afterwards, we check whether the choice is not the terminal one (in which case there will be fewer transitions than planned)
and if the last nonterminal character is not S (there is a stupid loophole so the second condition is necessary), and if these two
conditions are met, it goes to the previous choice. Then it transitions.
```cpp
string generateString()
	{
		if (word.length() > 0)
			word= "";
		word += S;
		int rng;
		rng = rand() % 6 + 3;
		for (int i = 0; i <= rng; i++)
		{
			string x = findGoodcombination();
			if (i == rng - 1)
			{
				int choice = x[x.length() - 1] - 48;
				word = word + transition(choice);
				return word;
			}
			else if (i == rng - 2 && word[word.length() - 1] == 'S')
			{
				word.pop_back();
				word = word + "bd";
				return word;
			}
			else {
				int choice = rand() % x.length() + (x[0] - 48);
				if (choice == (x[x.length() - 1] - 48) && (word[word.length()-1 != 'S']))
					choice--;
				word = word + transition(choice);
			}
		}
		return word;
	}
```

* Now it's time for the Finite Automaton class, containing all elements of a finite automaton. It inherits the Grammar class.
```cpp
class FiniteAutomaton : public Grammar
{
private:
    char Q[5], sigma[4], q0, F;
    string delta[7];
}
```

* There is the default constructor and parametrized one.
The parametrized constructor takes as a parameter an object of type Grammar, in order to perform the conversion
to an object of type Finite Automaton.
It defines q0 as 'S' and F as 'X' (the terminal state). 
String combo, comma and dash will be used to formulate the delta transition function. 
Char state, label and endstate will be in the delta definition as well, according to each production in the Grammar.
It is hardcoded to get specific characters using the indexes 0, 3 and 4 of each production (since they all have the same structure)
because my grammar is of right linear type 3.
If instead of a nonterminal there is null character on index 4 of any production, the endstate will be replaced with F.
Then it creates the combo and adds it to the delta string array.
The 2 if's are for making all the conversion happen in one loop instead of however many the Vt and Vn arrays would have.
```cpp
FiniteAutomaton() = default;
    FiniteAutomaton(Grammar g)
    {
        q0 = 'S';
        F = 'X';
        for (int i = 0; i < 7; i++)
        {
            string combo = "";
            string comma = ",";
            string dash = "-";
            char state = g.getP()[i][0];
            char label = g.getP()[i][3];
            char endstate = (g.getP()[i][4] == '\0') ? F : g.getP()[i][4];
            combo += state + comma + label + dash + endstate;

            delta[i] = combo;
            
            if (i < 4)
            {
                this->Q[i] = g.getVn()[i];
            }
            if (i < 5)
            {
                this->sigma[i] = g.getVt()[i];
            } 
        }
        Q[4] = F;
    }
```
* The next method, `delt()` returns the new state after a transition in the delta function.
It will iterate through the entire array of state transitions and once it will find the q and sigma provided
as parameters, it will return the new state (hardcodedly).
```cpp
char delt(char q, char sigm)
    {
        for (int i = 0; i < 7; i++)
        {
            if (delta[i][0] == q)
            {
                if (delta[i][2] == sigm)
                {
                    return delta[i][4];
                }
            }
        }
    }
```

* Last method, `stringBelongToLanguage(string inputString)` checks whether any input string belongs to the language.
It defines a nonterminal char equal to the starting character q0, takes each char in the string iteratively and checks
whether the delt function does not return X and it's not the last iteration, substituting the nonterminal with the new 
state returned by delt. Otherwise, it will finish and return 1.
```cpp 
bool stringBelongToLanguage (string inputString)
    {
        char nonterminal = q0;
        for (int i = 0; i < inputString.length(); i++)
        {
            char label = inputString[i];
            
            if (delt(nonterminal, label) != 'X' && i != inputString.length() - 1)
            {
                nonterminal = delt(nonterminal, label);
            }
            else if (delt(nonterminal, label) == 'X' && i == inputString.length() - 1)
            {
                return 1;
            }
        }
        return 0;
    }
```
## Conclusions / Screenshots / Results
In conclusion, implementing such an alphabet isn't specifically difficult, but I think my implementation could be
better if I made it compatible with any type of grammar, not ony right linear type 3. I'm happy I managed to do
it without external help and I'm ready to answer any questions. 

## References
none were used.

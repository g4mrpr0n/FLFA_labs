#pragma once
#include <string>
#include <iostream>
#include <time.h>
#include "FiniteAutomaton.h"
using namespace std;

class Grammar : public FiniteAutomaton
{
	
private:
	string P[7];
	char Vn[3] , Vt[4] , S;
	string word;
public:
	friend class FiniteAutomaton;
	Grammar() :word(""), P{ "S->aS", "S->bB", "B->cB", "B->aD", "B->d", "D->aB", "D->b" }, Vn{ 'S','B','D' }, Vt{'a','b','c','d'}, S('S') {}
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

	string generateString()
	{
		if (word.length() > 0)
			word = "";
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
				if (choice == (x[x.length() - 1] - 48) && (word[word.length() - 1 != 'S']))
					choice--;
				word = word + transition(choice);
			}
		}
		return word;
	}

	string transition(int n)
	{
		string x = "";
		for (int i = 0; i < P[n].length(); i++)
		{
			if (((i > 0) && (P[n][i - 1] == '>')) || x != "")
			{
				x = x + P[n][i];
			}
		}
		word.pop_back();
		return x;
	}

	string findGoodcombination()
	{
		string x = "";
		for (int i = 0; i < 7; i++)
		{
			if (P[i][0] == word[word.length() - 1]) {
				x.append(to_string(i));
			}
		}
		return x;
	}

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

	void flagNonTerminal(string s, string &flags) {
		for (int i = 0; i < s.length(); i++)
		{
			if (s[i] <= 90)
			{
				flags.append(to_string(i));
			}
		}
	}

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
};

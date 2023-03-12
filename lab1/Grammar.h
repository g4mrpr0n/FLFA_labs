#pragma once
#include <string>
#include <iostream>
#include <time.h>
using namespace std;

class Grammar
{
private:
	string P[7] = { "S->aS", "S->bB", "B->cB", "B->aD", "B->d","D->aB", "D->b" };
	char Vn[3] = { 'S','B','D' }, Vt[4] = { 'a','b','c','d' }, S='S';
	string word;
public:
	Grammar() :word(""){}
	Grammar(const string w) {
		this->word = w;
	}

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
};

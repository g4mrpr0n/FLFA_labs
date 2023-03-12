#pragma once
#include "Grammar.h"


class FiniteAutomaton : public Grammar
{
private:
    char Q[5], sigma[4], q0, F;
    string delta[7];
public:
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

    bool stringBelongToLanguage (string inputString)
    {
        char nonterminal = q0;
        for (int i = 0; i < inputString.length(); i++)
        {
            char label = inputString[i];
            
            if (delt(nonterminal, label) != F && i != inputString.length() - 1)
            {
                nonterminal = delt(nonterminal, label);
            }
            else if (delt(nonterminal, label) == F && i == inputString.length() - 1)
            {
                return 1;
            }
        }
        return 0;
    }
};

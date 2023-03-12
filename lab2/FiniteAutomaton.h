#pragma once
//#include "Grammar.h"
#include <string>
#include <iostream>
#include <time.h>
using namespace std;

class FiniteAutomaton //:public Grammar
{
private:
    char Q[3], sigma[3], q0, F;
    string delta[7];

public:
    FiniteAutomaton() :Q{ '0', '1', '2' }, sigma{ 'a', 'b', 'c' }, q0('0'), F('2'), delta{ "0,a-0", "0,b-1", "1,c-1", "1,c-2", "2,a-0", "1,a-1", "2->"}
    {}
    //FiniteAutomaton(Grammar &g)
    //{
    //    q0 = 'S';
    //    F = 'X';
    //    for (int i = 0; i < 7; i++)
    //    {
    //        string combo = "";
    //        string comma = ",";
    //        string dash = "-";
    //        char state = g.getP()[i][0];
    //        char label = g.getP()[i][3];
    //        char endstate = (g.getP()[i][4] == '\0') ? F : g.getP()[i][4];
    //        combo += state + comma + label + dash + endstate;
    //
    //        delta[i] = combo;
    //
    //        if (i < 3)
    //        {
    //            this->Q[i] = g.getVn()[i];
    //        }
    //        if (i < 4)
    //        {
    //            this->sigma[i] = g.getVt()[i];
    //        }
    //    }
    //    Q[3] = F;
    //}

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

    bool stringBelongToLanguage(string inputString)
    {
        char nonterminal = q0;
        for (int i = 0; i < inputString.length(); i++)
        {
            char label = inputString[i];
            int index = -1;
            if (delt(nonterminal, label, index) != F && i != inputString.length() - 1)
            {
                nonterminal = delt(nonterminal, label, index);
            }
            else if (delt(nonterminal, label, index) == F && i == inputString.length() - 1)
            {
                return true;
            }
        }
        return false;
    }
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
};

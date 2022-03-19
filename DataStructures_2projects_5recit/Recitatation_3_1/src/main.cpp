#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stack>
#include <string>
#include <iomanip>

using namespace std;

void infix_to_postfix(string, string &);
void infix_to_prefix(string, string &);
bool higher_priority(char, char);
double eval_postfix(string);
double eval_prefix(string);
double evaluate(double, double, char);

int main()
{
    /*string one = "hello";
    char x;
    for (int i = one.length() - 1; i >= 0; i--)
    {
        x = one[i];
    }*/

    string infix;
    string postfix;
    string prefix;
    bool is_exit = false;

    while (!is_exit)
    {
        cout << "Input the expression in infix: ";
        cin >> infix;

        if (infix == "E" || infix == "e")
            is_exit = true;

        else
        {
            infix_to_postfix(infix, postfix);
            cout << postfix << endl;
            cout << setprecision(2) << fixed << eval_postfix(postfix) << endl;
            infix_to_prefix(infix, prefix);
            cout << prefix << endl;
            cout << setprecision(2) << fixed << eval_prefix(prefix) << endl;
        }

        cin.clear();
        cin.ignore();
        infix.clear();
        postfix.clear();
    }
    return EXIT_SUCCESS;
}

void infix_to_prefix(string infix, string &prefix)
{
    string new_prefix;
    prefix = {};
    for (int i = infix.length() - 1; i >= 0; i--)
    {
        new_prefix += infix[i];
    }

    stack<char> s;

    for (char c : new_prefix)
    {
        if (c >= 48 && c <= 57)
        {
            prefix += c;
        }
        else
        {
            if (!s.empty())
            {
                if (higher_priority(c, s.top()) >= higher_priority(s.top(), c))
                {
                    s.push(c);
                }
                else
                {
                    while (!s.empty() && higher_priority(c, s.top()) < higher_priority(s.top(), c))
                    {
                        prefix += s.top();
                        s.pop();
                    }
                    s.push(c);
                }
            }
            else
            {
                s.push(c);
            }
        }
    }
    while (!s.empty())
    {
        prefix += s.top();
        s.pop();
    }

    new_prefix = {};
    for (int i = prefix.length() - 1; i >= 0; i--)
    {
        new_prefix += prefix[i];
    }

    prefix = new_prefix;
}

void infix_to_postfix(string infix, string &postfix)
{
    stack<char> s;
    // *********************Fill this method

    for (char c : infix)
    {
        if (c >= 48 && c <= 57)
        {
            postfix += c;
        }
        else
        {
            if (!s.empty())
            {
                if (higher_priority(c, s.top()) > higher_priority(s.top(), c))
                {
                    s.push(c);
                }
                else
                {
                    while (!s.empty() && higher_priority(c, s.top()) <= higher_priority(s.top(), c))
                    {
                        postfix += s.top();
                        s.pop();
                    }
                    s.push(c);
                }
            }
            else
            {
                s.push(c);
            }
        }
    }
    while (!s.empty())
    {
        postfix += s.top();
        s.pop();
    }
}

bool higher_priority(char first, char second)
{
    if (first == '*' || first == '/')
        return true;
    else if (second == '+' || second == '-')
        return true;
    else
        return false;
}

double eval_postfix(string expr)
{
    stack<double> s;
    double first, second;
    // *********************Fill this method
    for (char c : expr)
    {
        if (c >= 48 && c <= 57)
        {
            s.push(c - 48);
        }
        else
        {
            second = s.top();
            s.pop();
            first = s.top();
            s.pop();
            s.push(evaluate(first, second, c));
        }

    } //2+3/7+5*2

    return s.top();
}

double eval_prefix(string expr)
{
    string reversed;
    for (int i = expr.length() - 1; i >= 0; i--)
    {
        reversed += expr[i];
    }

    stack<double> s;
    double first, second;
    // *********************Fill this method
    for (char c : reversed)
    {
        if (c >= 48 && c <= 57)
        {
            s.push(c - 48);
        }
        else
        {
            second = s.top();
            s.pop();
            first = s.top();
            s.pop();
            s.push(evaluate(second, first, c));
        }

    }
    return s.top();
}

double evaluate(double first, double second, char c)
{
    switch (c)
    {
    case '*':
        return first * second;
    case '+':
        return first + second;
    case '-':
        return first - second;
    case '/':
        return first / second;
    default:
        return 0;
    }
}

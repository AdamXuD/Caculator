#include <iostream>
#include <string>
#include <cctype>
#include <cstring>
#include <cmath>
#include <stack>

using namespace std;

class caculator
{
public:
    void start()
    {
        while (1)
        {
            empty();
            cout << "ÇëÊäÈëËãÊ½>" << endl;
            getline(cin, this->formula, ';');
            if (!isLegal())
            {
                continue;
            }
            toRPN();
        }
    }

    void empty()
    {
        formula.empty();
        RPN.empty();
        op.empty();
        num.empty();
    }

    int getPriority(char ch)
    {
        switch (ch)
        {
        case '#':
            return 0;
        case '(':
            return 1;
        case '+':
        case '-':
            return 2;
        case '*':
        case '/':
            return 3;
        case ')':
            return 4;
        default:
            return -1;
        }
    }

    bool isLegal()
    {
        return true;
    }

    double getFigure(int &i)
    {
        double num = 0, k = 1;
        bool point = false;
        for (; isdigit(this->formula[i]) || this->formula[i] == '.'; i++)
        {
            if (!point)
            {
                if (isdigit(this->formula[i]))
                {
                    num = num * 10 + this->formula[i] - '0';
                }
                else if (this->formula[i] == '.')
                {
                    point = true;
                }
            }
            else
            {
                if (isdigit(this->formula[i]))
                {
                    num = num + (this->formula[i] - '0') / pow(10, k);
                    k++;
                }
            }
        }
        i -= 1;
        return num;
    }

    void toRPN()
    {
        op.push('#');
        char num[1024] = {0};
        for (int i = 0; i < this->formula.length(); i++)
        {
            if (isdigit(this->formula[i]) || this->formula[i] == '.')
            {
                sprintf(num, "%d ", (int)getFigure(i));
                this->RPN.append(num);
            }
            else if (getPriority(this->formula[i]) > 0)
            {
                if (this->formula[i] == '(')
                {
                    op.push(this->formula[i]);
                }
                else if (this->formula[i] == ')')
                {
                    while (op.top() != '(')
                    {
                        sprintf(num, "%c ", op.top());
                        this->RPN.append(num);
                        op.pop();
                    }
                    op.pop();
                }
                else if (getPriority(this->formula[i]) > getPriority(op.top()))
                {
                    op.push(this->formula[i]);
                }
                else if (getPriority(this->formula[i]) <= getPriority(op.top()))
                {
                    while (getPriority(this->formula[i]) <= getPriority(op.top()))
                    {
                        sprintf(num, "%c ", op.top());
                        this->RPN.append(num);
                        op.pop();
                    }
                    op.push(this->formula[i]);
                }
            }
        }
        while (op.top() != '#')
        {
            sprintf(num, "%c ", op.top());
            this->RPN.append(num);
            op.pop();
        }
        // cout << this->RPN << endl;
    }


private:
    string formula;
    string RPN;
    stack<char> op;
    stack<double> num;

protected:
};
int main()
{
    caculator cac;
    cac.start();
}

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
    void main()
    {
        while (1)
        {
            empty();
            cout << "Input your formula here (put a ';' at the end of formula)>" << endl;
            getline(cin, this->formula, ';');
            if (!isLegal())
            {
                continue;
            }
            caculate();
            printf("Result is %lf\n", this->result);
        }
    }

    void empty()
    {
        formula.empty();
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

    double getFigure(string formula, int &i)
    {
        double num = 0, k = 1;
        bool point = false;
        for (; isdigit(formula[i]) || formula[i] == '.'; i++)
        {
            if (!point)
            {
                if (isdigit(formula[i]))
                {
                    num = num * 10 + formula[i] - '0';
                }
                else if (formula[i] == '.')
                {
                    point = true;
                }
            }
            else
            {
                if (isdigit(formula[i]))
                {
                    num = num + (formula[i] - '0') / pow(10, k);
                    k++;
                }
            }
        }
        i -= 1;
        return num;
    }

    void dealWithflag(char flag)
    {
        double num1, num2;
        num2 = num.top();
        num.pop();
        num1 = num.top();
        num.pop();
        switch (flag)
        {
        case '+':
        {
            num.push(num1 + num2);
            break;
        }
        case '-':
        {
            num.push(num1 - num2);
            break;
        }
        case '*':
        {
            num.push(num1 * num2);
            break;
        }
        case '/':
        {
            num.push(num1 / num2);
            break;
        }
        }
    }

    void caculate()
    {
        op.push('#');
        for (int i = 0; i < this->formula.length(); i++)
        {
            if (isdigit(this->formula[i]) || this->formula[i] == '.')
            {
                num.push((int)getFigure(this->formula, i));
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
                        dealWithflag(op.top());
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
                        dealWithflag(op.top());
                        op.pop();
                    }
                    op.push(this->formula[i]);
                }
            }
        }
        while (op.top() != '#')
        {
            dealWithflag(op.top());
            op.pop();
        }
        result = num.top();
        num.pop();
    }

private:
    string formula;
    //string RPN;
    stack<char> op;
    stack<double> num;
    double result;

protected:
};

int main()
{
    caculator cac;
    cac.main();
}

#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <cctype>
#include <cstring>
#include <cmath>
#include <typeinfo>
#include <stack>
#include <list>

using namespace std;

class integral : private caculator
{
public:
    integral(string str, double low, double high)
    {
        this->formula = str;
        this->lo = low;
        this->hi = hi;
        caculate();
    }

    double dealWithAlpha(int &i, double x)
    {
        switch (formula[i])
        {
        case 's':
        {
            int end = formula.find(')', i);
            int head = formula.find('(', i) + 1;
            i = end;
            return sin(getFigure(formula, head));
        }
        case 'c':
        {
            int end = formula.find(')', i);
            int head = formula.find('(', i) + 1;
            i = end;
            return cos(getFigure(formula, head));
        }
        case 't':
        {
            int end = formula.find(')', i);
            int head = formula.find('(', i) + 1;
            i = end;
            return tan(getFigure(formula, head));
        }
        case 'i':
        {
            list<char> integral;
            string integral_formula;

            return 0;
        }
        case 'l':
        {
            if (formula.find("log") != string::npos)
            {
                int end = formula.find(')', i);
                int head = formula.find('(', i) + 1;
                i = end;
                double down = getFigure(formula, head);
                do
                {
                    head++;
                } while (isdigit(formula[head]) == 0);
                double up = getFigure(formula, head);
                return log(up) / log(down);
            }
            else if (formula.find("ln") != string::npos)
            {
                int end = formula.find(')', i);
                int head = formula.find('(', i) + 1;
                i = end;
                return log(getFigure(formula, head));
            }
        }
        case 'p':
        {
            i += 2;
            return M_PI;
        }
        case 'e':
        {
            i++;
            return M_E;
        }
        }
        return 0;
    }

    void caculate()
    {
        op.push('#');
        for (int i = 0; i < this->formula.length(); i++)
        {
            if (this->formula[i] = 'x')
            {
                num.push(formula[i]);
            }
            else if (isalpha(this->formula[i]))
            {
                num.push(dealWithAlpha(i));
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
    }

    double Func(list<char> integral, double x)
    {
    }

    double caculatorintegral(double lo, double hi, double (*Func)(double), int n = 1000)
    { //lo:下限；hi:上限；Func:函数；n:等分数
        double x;
        double step = (hi - lo) / n;

        double result = 0.0;
        x = lo;
        for (int i = 1; i < n; i++)
        {
            x += step;
            result += Func(x);
        }
        result += (Func(lo) + Func(hi)) / 2;
        result *= step;

        return result;
    }

private:
    double lo;
    double hi;
    stack<char> op;
    stack<double> num;
    string formula;

protected:
};

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
        case '^':
            return 4;
        case ')':
            return 5;
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
        for (; isdigit(formula[i]) || formula[i] == '.' || formula[i] == ' '; i++)
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
        case '^':
        {
            num.push(pow(num1, num2));
            break;
        }
        }
    }

    double dealWithAlpha(int &i)
    {
        switch (formula[i])
        {
        case 's':
        {
            int end = formula.find(')', i);
            int head = formula.find('(', i) + 1;
            i = end;
            return sin(getFigure(formula, head));
        }
        case 'c':
        {
            int end = formula.find(')', i);
            int head = formula.find('(', i) + 1;
            i = end;
            return cos(getFigure(formula, head));
        }
        case 't':
        {
            int end = formula.find(')', i);
            int head = formula.find('(', i) + 1;
            i = end;
            return tan(getFigure(formula, head));
        }
        case 'i':
        {
            list<char> integral;
            string integral_formula;

            return 0;
        }
        case 'l':
        {
            if (formula.find("log") != string::npos)
            {
                int end = formula.find(')', i);
                int head = formula.find('(', i) + 1;
                i = end;
                double down = getFigure(formula, head);
                do
                {
                    head++;
                } while (isdigit(formula[head]) == 0);
                double up = getFigure(formula, head);
                return log(up) / log(down);
            }
            else if (formula.find("ln") != string::npos)
            {
                int end = formula.find(')', i);
                int head = formula.find('(', i) + 1;
                i = end;
                return log(getFigure(formula, head));
            }
        }
        case 'p':
        {
            i += 2;
            return M_PI;
        }
        case 'e':
        {
            i++;
            return M_E;
        }
        }
        return 0;
    }

    void caculate()
    {
        op.push('#');
        for (int i = 0; i < this->formula.length(); i++)
        {
            if (isdigit(this->formula[i]) || this->formula[i] == '.')
            {
                num.push(getFigure(this->formula, i));
            }
            else if (isalpha(this->formula[i]))
            {
                num.push(dealWithAlpha(i));
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

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

class RPN
{
public:
    string formula;
    RPN(string str)
    {
        this->formula = str;
    }

    RPN() {}

    void empty()
    {
        formula.empty();
        op.empty();
        num.empty();
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

    template<typename _T>
    double dealWithAlpha(int &i)
    {
        switch (formula[i])
        {
        case 's':
        {
            int end = formula.find(')', i);
            int head = formula.find('(', i);
            i = end;
            _T inner(formula.substr(head, end - head - 1));
            return sin(inner.caculate());
        }
        case 'c':
        {
            int end = formula.find(')', i);
            int head = formula.find('(', i);
            i = end;
            _T inner(formula.substr(head, end - head - 1));
            return cos(inner.caculate());
        }
        case 't':
        {
            int end = formula.find(')', i);
            int head = formula.find('(', i);
            i = end;
            _T inner(formula.substr(head, end - head - 1));
            return tan(inner.caculate());
        }
        case 'i':
        {
            /*积分部分*/
            int end = formula.find(')', i);
            int head = formula.find('(', i);
            i = end;
            _T inner(formula.substr(head, end - head - 1));
        }
        case 'l':
        {
            if (formula.find("log") != string::npos)
            {
                int end = formula.find(')', i);
                int head = formula.find(',', i);
                i = end;
                _T inner_down(formula.substr(head, end - head - 1));
                double down = inner_down.caculate();
                _T inner_up(formula.substr(head, end - head - 1));
                double up = inner_up.caculate();
                return log(up) / log(down);
            }
            else if (formula.find("ln") != string::npos)
            {
                int end = formula.find(')', i);
                int head = formula.find('(', i);
                i = end;
                _T inner(formula.substr(head, end - head - 1));
                return log(inner.caculate());
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
    }

    double caculate()
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
                num.push(dealWithAlpha<RPN>(i));
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
        double result = num.top();
        num.pop();
        return result;
    }

private:
    stack<char> op;
    stack<double> num;

protected:
};

class equation : private RPN
{
public:
    equation(string str, double low, double high)
    {
        this->formula = str;
        this->lo = low;
        this->hi = hi;
    }

    double caculate(double x)
    {
        op.push('#');
        for (int i = 0; i < this->formula.length(); i++)
        {
            if (isdigit(this->formula[i]) || this->formula[i] == '.')
            {
                num.push(getFigure(this->formula, i));
            }
            else if (this->formula[i] == 'x')
            {
                num.push(x);
            }
            else if (isalpha(this->formula[i]))
            {
                num.push(dealWithAlpha<equation>(i));
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
        double result = num.top();
        num.pop();
        return result;
    }

    double caculateintegral(double lo, double hi, int n = 1000)
    { //lo:���ޣ�hi:���ޣ�Func:������n:�ȷ���
        double x;
        double step = (hi - lo) / n;

        double result = 0.0;
        x = lo;
        for (int i = 1; i < n; i++)
        {
            x += step;
            result += caculate(x);
        }
        result += (caculate(lo) + caculate(hi)) / 2;
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
            getline(cin, this->cac.formula, ';');
            if (!isLegal())
            {
                continue;
            }
            this->result = cac.caculate();
            printf("Result is %lf\n", this->result);
        }
    }

    void empty()
    {
        cac.empty();
    }

    bool isLegal()
    {
        return true;
    }

private:
    RPN cac;
    double result;

protected:
};

int main()
{
    caculator cac;
    cac.main();
}

#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <cctype>
#include <cstring>
#include <cmath>
#include <typeinfo>
#include <stack>

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
        if (flag == '!')
        {
            double number = num.top(), sum = 1;
            num.pop();
            while(number != 0)
            {
                sum *= number--;
            }
            num.push(sum);
            return;
        }
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
        return;
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
        case '!':
            return 4;
        case ')':
            return 5;
        default:
            return -1;
        }
    }

    double substitute(string equation, double x)
    {
        RPN tmp;
        char num[50] = {0};
        sprintf(num, "%.10f", x);
        tmp.formula = equation;
        for (int i = 0; tmp.formula[i] != '\0'; i++)
        {
            if (tmp.formula[i] == 'x')
            {
                tmp.formula.replace(i, 1, num);
            }
        }
        return tmp.caculate();
    }

    double caculateintegral(double lo, double hi, string equation, int n = 1000)
    {
        double x;
        double step = (hi - lo) / n;

        double result = 0.0;
        x = lo;
        for (int i = 1; i < n; i++)
        {
            x += step;
            result += substitute(equation, x);
        }

        result += (substitute(equation, lo) + substitute(equation, hi)) / 2;
        result *= step;

        return result;
    }

    double dealWithAlpha(int &i)
    {
        switch (formula[i])
        {
        case 's':
        {
            int head = formula.find('(', i);
            stack<char> flag;
            for (i = head; formula[i] != '\0'; i++)
            {
                if (formula[i] == '(')
                {
                    flag.push(formula[i]);
                }
                else if (formula[i] == ')')
                {
                    flag.pop();
                }
                if (flag.size() == 0)
                {
                    break;
                }
            }
            int end = i;
            RPN inner(formula.substr(head + 1, end - head - 1));
            return sin(inner.caculate());
        }
        case 'c':
        {
            int head = formula.find('(', i);
            stack<char> flag;
            for (i = head; formula[i] != '\0'; i++)
            {
                if (formula[i] == '(')
                {
                    flag.push(formula[i]);
                }
                else if (formula[i] == ')')
                {
                    flag.pop();
                }
                if (flag.size() == 0)
                {
                    break;
                }
            }
            int end = i;
            RPN inner(formula.substr(head + 1, end - head - 1));
            return cos(inner.caculate());
        }
        case 't':
        {
            int head = formula.find('(', i);
            stack<char> flag;
            for (i = head; formula[i] != '\0'; i++)
            {
                if (formula[i] == '(')
                {
                    flag.push(formula[i]);
                }
                else if (formula[i] == ')')
                {
                    flag.pop();
                }
                if (flag.size() == 0)
                {
                    break;
                }
            }
            int end = i;
            RPN inner(formula.substr(head + 1, end - head - 1));
            return tan(inner.caculate());
        }
        case 'i':
        {
            int comma_1 = formula.find(',', i);
            int head = formula.find('(', i);
            RPN first(formula.substr(head + 1, comma_1 - head - 1));
            double lo = first.caculate();
            int comma_2 = formula.find(',', comma_1 + 1);
            RPN second(formula.substr(comma_1 + 1, comma_2 - comma_1 - 1));
            double hi = second.caculate();
            stack<char> flag;
            for (i = head; formula[i] != '\0'; i++)
            {
                if (formula[i] == '(')
                {
                    flag.push(formula[i]);
                }
                else if (formula[i] == ')')
                {
                    flag.pop();
                }
                if (flag.size() == 0)
                {
                    break;
                }
            }
            int end = i;
            string equation = formula.substr(comma_2 + 1, end - comma_2 - 1);
            return caculateintegral(lo, hi, equation);
        }
        case 'l':
        {
            if (formula.find("log") != string::npos)
            {
                int comma = formula.find(',', i);
                int head = formula.find('(', i);
                stack<char> flag;
                for (i = head; formula[i] != '\0'; i++)
                {
                    if (formula[i] == '(')
                    {
                        flag.push(formula[i]);
                    }
                    else if (formula[i] == ')')
                    {
                        flag.pop();
                    }
                    if (flag.size() == 0)
                    {
                        break;
                    }
                }
                int end = i;
                RPN inner_down(formula.substr(head + 1, comma - head - 1));
                double down = inner_down.caculate();
                RPN inner_up(formula.substr(comma + 1, end - comma - 1));
                double up = inner_up.caculate();
                return log(up) / log(down);
            }
            else if (formula.find("ln") != string::npos)
            {
                int head = formula.find('(', i);
                stack<char> flag;
                for (i = head; formula[i] != '\0'; i++)
                {
                    if (formula[i] == '(')
                    {
                        flag.push(formula[i]);
                    }
                    else if (formula[i] == ')')
                    {
                        flag.pop();
                    }
                    if (flag.size() == 0)
                    {
                        break;
                    }
                }
                int end = i;
                RPN inner(formula.substr(head + 1, end - head - 1));
                return log(inner.caculate());
            }
        }
        case 'p':
        {
            i++;
            return M_PI;
        }
        case 'e':
        {
            return M_E;
        }
        }
        return 0;
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
            char flag = op.top();
            dealWithflag(flag);
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
            getchar();
            this->result = cac.caculate();
            printf("Result is %lf\n", this->result);
        }
    }

    void empty()
    {
        cac.empty();
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

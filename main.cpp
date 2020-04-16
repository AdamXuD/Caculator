#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <stack>

#ifdef _MSC_VER
#define < conio.h>
#elif defined __GNUC__
#include <termios.h>
#endif

using namespace std;

#ifdef __GNUC__
int getch()
{
    int cr;
    struct termios nts, ots;

    if (tcgetattr(0, &ots) < 0) // 得到当前终端(0表示标准输入)的设置
        return EOF;

    nts = ots;
    cfmakeraw(&nts);                     // 设置终端为Raw原始模式，该模式下所有的输入数据以字节为单位被处理
    if (tcsetattr(0, TCSANOW, &nts) < 0) // 设置上更改之后的设置
        return EOF;

    cr = getchar();
    if (tcsetattr(0, TCSANOW, &ots) < 0) // 设置还原成老的模式
        return EOF;

    return cr;
}
#endif

void clear()
{
#ifdef __GNUC__
    system("clear");
#elif defined _MSC_VER
    system("cls");
#endif
}

int menu(string list[], int size) //这个
{
    int select = 1;
    while (1)
    {
        clear();
        for (int i = 0; i < size; i++)
        {
            if (i == select - 1)
            {
                cout << "\033[7m"
                     << ">" << list[i] << "\033[0m" << endl;
            }
            else
            {
                cout << ">" << list[i] << endl;
            }
        }
        //上下键相当于同时按下224和（72或者80）键
        //而回车键是13
        int ch1 = getch(); //获取第一个键
        if (ch1 == 13)
        {
            return select; //如果是回车 返回菜单号
        }
        int ch2 = getch(); //如果不是回车 获取第二个键值
        int ch3 = getch();
        if (ch1 == 27 && ch2 == 91 && ch3 == 66)
        {
            select++;
        }
        else if (ch1 == 27 && ch2 == 91 && ch3 == 65) //判断按下了哪个键
        {
            select--;
        }

        if (select > size) //判断越界
        {
            select = 1;
        }
        else if (select < 1)
        {
            select = size;
        }
    }
}

class RPN
{
public:
    string formula;
    RPN(string str)
    {
        this->formula = str;
    }

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
            while (number != 0)
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
        char num[50] = {0};
        sprintf(num, "%.10f", x);
        RPN tmp(num);
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

    double handleBracket(int &i)
    {
        int head = i;
        stack<char> flag;
        for (; formula[i] != '\0'; i++)
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
        return inner.caculate();
    }

    double dealWithAlpha(int &i)
    {
        switch (formula[i])
        {
        case 's':
        {
            i = formula.find('(', i);
            return sin(handleBracket(i));
        }
        case 'c':
        {
            i = formula.find('(', i);
            return cos(handleBracket(i));
        }
        case 't':
        {
            i = formula.find('(', i);
            return tan(handleBracket(i));
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
                i = formula.find('(', i);
                return log(handleBracket(i));
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
                    num.push(handleBracket(i));
                }
                else if (this->formula[i] == ')')
                {
                    continue;
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
            cout << "Input your formula here (put a ';' at the end of formula)>" << endl;
            getline(cin, this->formula, ';');
            getchar();
            RPN cac(formula);
            this->result = cac.caculate();
            printf("Result is %lf\n", this->result);
        }
    }

private:
    string formula;
    double result;

protected:
};

class determinant
{
public:
    determinant()
    {
        memset(num, 0, sizeof(num));
        row = 0;
        col = 0;
        result = 0;
    }

    void main()
    {
        while (1)
        {
            cout << "Please input the determinant(Input '/' at the end of line and put another '/' at the end of determinant>)" << endl;
            for (col = 0;; col++)
            {
                for (row = 0;; row++)
                {
                    cin >> data[col][row];
                    if (*(data[col][row].end() - 1) == '/')
                    {
                        data[col][row].erase(data[col][row].end() - 1);
                        break;
                    }
                }
                if (*(data[col][row].end() - 1) == '/')
                {
                    data[col][row].erase(data[col][row].end() - 1);
                    break;
                }
            }
            for (int i = 0; i <= col; i++)
            {
                for (int j = 0; j <= row; j++)
                {
                    RPN tmp(data[i][j]);
                    num[i][j] = tmp.caculate();
                }
            }
            caculate();
        }
    }

    void caculate()
    {
        double master = 0, sub = 0;
        for (int t_col = 0; t_col <= col; t_col++)
        {
            double branch = 1;
            for (int i = 0, j = t_col; i <= row; i++, j++)
            {
                if (j > col)
                {
                    j -= (col + 1);
                }
                cout << branch;
                branch *= num[i][j];
                cout << " * " << num[i][j] << " = " << branch << endl;
            }
            cout << master;
            master += branch;
            cout << " + " << branch << " = " << master << endl;
            cout << endl;
        }
        for (int t_col = col; t_col >= 0; t_col--)
        {
            double branch = 1;
            for (int i = 0, j = t_col; i <= row; i++, j--)
            {
                if (j < 0)
                {
                    j += (col + 1);
                }
                cout << branch;
                branch *= num[i][j];
                cout << " * " << num[i][j] << " = " << branch << endl;
            }
            cout << sub;
            sub += branch;
            cout << " + " << branch << " = " << sub << endl;
            cout << endl;
        }
        result = master - sub;
        cout << "Result is> " << result << endl;
    }

private:
    string data[100][100];
    double num[100][100];
    int row;
    int col;
    double result;

protected:
};

int main()
{
    string menu_list[] = {"科学计算器", "行列式计算器"};
    switch(menu(menu_list, 2))
    {
        case 1:{ caculator cac; cac.main(); break; }
        case 2:{ determinant det; det.main(); break; }
    }
}

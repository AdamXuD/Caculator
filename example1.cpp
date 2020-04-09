#include <iostream>
#include <string.h>
#include <string>
using namespace std;

//定义栈
#define max_size 200 //栈的最大容量
typedef char datatype;
typedef struct
{
    datatype zhan[max_size];
    int top; //栈顶
} stack;

//栈的初始化
void initial(stack &st)
{
    st.top = 0;
}

//类型为datatype的x入栈
void push(stack &st, datatype x)
{
    //当栈顶和max_size相等时，栈满
    if (st.top == max_size)
    {
        // cout<<"This stack has already full!";
        cout << "no";
        exit(0);
    }
    else
    {
        st.zhan[st.top] = x;
        st.top++;
    }
}

//出栈
char pop(stack &st)
{
    if (st.top == 0)
    {
        // cout<<"This stack is empty!";
        cout << "no";
        exit(0);
    }
    else
    {
        st.top--;
        return st.zhan[st.top];
    }
}

int main()
{

    stack s;
    initial(s);

    /*输入字符串，并将字符串放到字符数组中，
 实现能够逐个扫描字符串中的字符，并且不跳过空格符 
 */
    string str;
    getline(cin, str);
    char ch[200] = {'\0'};
    strcpy(ch, str.c_str());

    //flag标志状态 1为括号匹配，0为不匹配
    int flag = 1;
    int i;
    for (i = 0; ch[i] != '\0'; i++)
    {
        //元素若为{，(，[则入栈
        if ((ch[i] == '{') || (ch[i] == '[') || (ch[i] == '('))
        {
            push(s, ch[i]);
        } //元素若为}，)，]则出栈 赋值给a
        else if ((ch[i] == '}') || (ch[i] == ']') || (ch[i] == ')'))
        {
            char a;
            a = pop(s);
            //若a与ch[i]匹配，进行下一个字符扫描
            if ((a == '{' && ch[i] == '}') || (a == '(' && ch[i] == ')') || (a == '[' && ch[i] == ']'))
            {
                continue;
            }
            else
                flag = 0;
        }
    }

    if (s.top != 0)
    { //当左括号多出没有与右括号匹配的时候（如：" {() "）
        flag = 0;
    }

    if (flag == 0)
    {
        cout << "no";
    }
    else
        cout << "yes";
    return 0;
}
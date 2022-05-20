#include <iostream>
#include <stack>
#include <unordered_map>
#include <string>


using namespace std;

int main()
{
    setlocale(LC_ALL, "RUS");

    stack <int> vir;
    char ch;
    int g;

    string a;

    cout << "input: ";
    while ((ch = getchar()) != '\n') {

        if (ch == ' ') {
            continue;
        }

        // если число то добавляем в стек
        if (isdigit(ch) && ch != '\n') {
            a = "";
            while (isdigit(ch) && ch != '\n') {
                if (isdigit(ch)) {
                    a += ch;
                }
                ch = getchar();
            }
            vir.push(atoi(a.c_str()));
        }
        // если операция то извлекаем из стека 2 числа 
        // выполняем операцию
        // результат добавляем в стек
        if (!isdigit(ch)) {
            switch (ch)
            {
            case '+':
                g = vir.top();
                vir.pop();
                vir.top() += g;
                break;
            case '-':
                g = vir.top();
                vir.pop();
                vir.top() -= g;
                break;
            case '/':
                g = vir.top();
                vir.pop();
                vir.top() /= g;
                break;
            case '*':
                g = vir.top();
                vir.pop();
                vir.top() *= g;
                break;
            }

        }
    }

    // в стеке остается одно значение
    // которое является результатом
    cout << endl << vir.top();
    vir.pop();



}


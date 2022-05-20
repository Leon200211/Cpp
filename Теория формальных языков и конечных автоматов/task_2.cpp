#include <iostream>
#include <stack>
#include <unordered_map>



using namespace std;

int main()
{
    setlocale(LC_ALL, "RUS");

    stack <int> vir;
    char ch;
    int g;

    cout << "input: ";
    while ((ch = getchar()) != '\n') {
        // если число то добавляем в стек
        if (isdigit(ch)) {
            vir.push(int(ch) - '0');
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


#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>

using namespace std;

// Типы лексем
enum lex_type
{
    LEX_NULL,       // 0
    // КЛЮЧЕВЫЕ СЛОВА
    LEX_AND,        // 1
    LEX_END,        // 1
    LEX_BOOL,       // 2
    LEX_REAL,       // 2
    LEX_ELSE,       // 3
    LEX_END_ELSE,       // 4
    LEX_IF,         // 5
    LEX_FALSE,      // 6
    LEX_INT,        // 7
    LEX_NOT,        // 8
    LEX_OR,         // 9
    LEX_INPUT,       // 10
    LEX_THEN,       // 11
    LEX_TRUE,       // 12
    LEX_DO,      // 13
    LEX_WHILE,      // 13
    LEX_LOOP,      // 14
    LEX_OUTPUT,      // 15
    LEX_LET,      // 15
    LEX_FOR,      // 16
    // МАРКЕР КОНЦА ТЕКСТА ПРОГРАММЫ
    LEX_FIN,        // 17
    // ОПЕРАТОРЫ И РАЗДЕЛИТЕЛИ
    LEX_SEMICOLON,  // 18
    LEX_COMMA,      // 19
    LEX_COLON,      // 22
    LEX_ASSIGN,     // 23
    LEX_LPAREN,     // 24
    LEX_RPAREN,     // 25
    LEX_EQ,         // 26
    LEX_LSS,        // 27
    LEX_GTR,        // 28
    LEX_PLUS,       // 29
    LEX_MINUS,      // 30
    LEX_TIMES,      // 31
    LEX_SLASH,      // 32
    LEX_LEQ,        // 33
    LEX_NEQ,        // 34
    LEX_GEQ,        // 35
    LEX_SCOM,        // 35
    LEX_ECOM,        // 35
    // ЧИСЛОВАЯ КОНСТАНТА
    LEX_NUM,        // 36
    // ИДЕНТИФИКАТОР
    LEX_ID          // 37
};



// Класс "Лексема"
class Lex
{
    lex_type type;
    int value;

public:
    Lex(lex_type t = LEX_NULL, int v = 0)
    {
        type = t;
        value = v;
    }
    lex_type getType()
    {
        return type;
    }
    int getValue()
    {
        return value;
    }
    friend ostream& operator << (ostream& s, Lex l)
    {
        s << "(" << l.type << ',' << l.value << ");";
        return s;
    }
};

// Класс "Идентификатор"
class Id
{
    char* name;
    bool declared;
    bool assigned;
    lex_type type;
    int value;

public:
    Id()
    {
        declared = false;
        assigned = false;
    }
    char* getName()
    {
        return name;
    }
    void setName(const char* name)
    {
        this->name = new char[strlen(name) - 1];
        strcpy(this->name, name);
    }
    lex_type getType()
    {
        return type;
    }
    void setType(lex_type type)
    {
        this->type = type;
    }
    bool isDeclared()
    {
        return declared;
    }
    void setDeclared()
    {
        declared = true;
    }
    bool isAssigned()
    {
        return assigned;
    }
    void setAssigned()
    {
        assigned = true;
    }
    int getValue()
    {
        return value;
    }
    void setValue(int value)
    {
        this->value = value;
    }
};

// Класс "Таблица идентификаторов"
class TableId
{
    Id* p;
    int size;
    int top;

public:
    TableId(int max_size)
    {
        size = max_size;
        p = new Id[size];
        top = 1;
    }
    ~TableId()
    {
        delete[] p;
    }
    Id& operator [] (int k)
    {
        return p[k];
    }
    int put(const char* name);
};

int TableId::put(const char* name)
{
    for (int i = 1; i < top; ++i) {
        if (!strcmp(name, p[i].getName())) {
            return i;
        }
    }
    p[top].setName(name);
    top++;
    return top - 1;
}


////////////////////////////
// ЛЕКСИЧЕСКИЙ АНАЛИЗАТОР //
////////////////////////////

class Lexer
{
    int lencom = 0;
    enum state {
        H,       // новая строка
        ID,      // буква или цифра
        NUM,     // цифра
        COM,     // коментарий
        ALE,     // сравнение
        NEQ,     // не равно
        DELIM    // символы
    };
    state CS;
    static const char* TW[];
    static const char* TD[];
    static lex_type words[];
    static lex_type dlms[];
    FILE* fp;
    char c;
    char buf[80];
    int buf_top;

    void clear()
    {
        buf_top = 0;
        for (int i = 0; i < 80; ++i) {
            buf[i] = '\0';
        }
    }

    void add()
    {
        buf[buf_top++] = c;
    }

    int look(const char* buf, const char** list)
    {
        int i = 0;
        while (list[i]) {
            if (!strcmp(buf, list[i])) {
                return i;
            }
            i++;
        }
        return 0;
    }

    void gc()
    {
        c = (char)fgetc(fp);
    }

public:
    Lexer(const char* filename)
    {
        fp = fopen(filename, "r");
        CS = H;
        clear();
        gc();
    }

    Lex getLex();
};


// Таблица ключевых слов
const char* Lexer::TW[] = {
    "",
    "and",
    "$",
    "do",
    "while",
    "loop",
    "for",
    "else",
    "end_else",
    "end",
    "if",
    "false",
    "%",
    "!",
    "~",
    "or",
    "input",
    "then",
    "true",
    "output",
    "let",
    NULL
};

// Таблица разделителей
const char* Lexer::TD[] = {
    "",
    ";",
    ",",
    ":",
    "=",
    "(",
    ")",
    "EQ",
    "LT",
    "GT",
    "plus",
    "min",
    "mult",
    "div",
    "LE",
    "NE",
    "GE",
    "(*",
    "*)",
    NULL
};

// Таблица идентификаторов
TableId TID(100);

// Таблица типов ключевых слов
lex_type Lexer::words[] = {
    LEX_NULL,
    LEX_AND,
    LEX_BOOL,
    LEX_DO,
    LEX_WHILE,
    LEX_LOOP,
    LEX_FOR,
    LEX_ELSE,
    LEX_END_ELSE,
    LEX_END,
    LEX_IF,
    LEX_FALSE,
    LEX_INT,
    LEX_REAL,
    LEX_NOT,
    LEX_OR,
    LEX_INPUT,
    LEX_THEN,
    LEX_TRUE,
    LEX_OUTPUT,
    LEX_LET,
    LEX_NULL
};

// Таблица типов разделителей
lex_type Lexer::dlms[] = {
    LEX_NULL,
    LEX_SEMICOLON,
    LEX_COMMA,
    LEX_COLON,
    LEX_ASSIGN,
    LEX_LPAREN,
    LEX_RPAREN,
    LEX_EQ,
    LEX_LSS,
    LEX_GTR,
    LEX_PLUS,
    LEX_MINUS,
    LEX_TIMES,
    LEX_SLASH,
    LEX_LEQ,
    LEX_NEQ,
    LEX_GEQ,
    LEX_SCOM,
    LEX_ECOM,
    LEX_NULL
};



// Основная функция лексического разбора
Lex Lexer::getLex()
{
    int d, j;
    CS = H;
    do {
        switch (CS) {
        case H:
            if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
                gc();
            }


            /*
            else if (c == 'Q' && buf[strlen(buf) - 2] == 'E') {
                add();
                gc();
                CS = ALE;
            }
            else if (c == 'T' && buf[strlen(buf) - 2] == 'L') {
                add();
                gc();
                CS = ALE;
            }
            else if (c == 'T' && buf[strlen(buf) - 2] == 'G') {
                add();
                gc();
                CS = ALE;
            }
            else if (c == 'E' && buf[strlen(buf) - 2] == 'L') {
                add();
                gc();
                CS = ALE;
            }
            else if (c == 'E' && buf[strlen(buf) - 2] == 'N') {
                add();
                gc();
                CS = ALE;
            }
            else if (c == 'E' && buf[strlen(buf) - 2] == 'G') {
                add();
                gc();
                CS = ALE;
            }
            */


            else if (isalpha(c)) {
                clear();
                add();
                gc();
                CS = ID;
            }
            else if (isdigit(c)) {
                d = c - '0';
                gc();
                CS = NUM;
            }

            else if (c == '~') {
                clear();
                add();
                gc();
                CS = NEQ;
            }

            else {
                CS = DELIM;
            }

            break;


        case ID:
            if (isalpha(c) || isdigit(c) || c == '_') {
                add();
                gc();
            }
            else {

                j = look(buf, TD);
                if (j == 0) {
                    j = look(buf, TW);
                    if (j) {
                        return Lex(words[j], j);
                    }
                    else {
                        j = TID.put(buf);
                        return Lex(LEX_ID, j);
                    }
                }
                else if (j != 0) {
                    return Lex(dlms[j], j);
                }

                /*
                j = look(buf, TW);
                if (j) {
                    return Lex(words[j], j);
                }
                */
                else {
                    j = TID.put(buf);
                    return Lex(LEX_ID, j);
                }
            }
            break;


        case NUM:
            if (isdigit(c)) {
                d = d * 10 + (c - '0');
                gc();
            }
            else {
                return Lex(LEX_NUM, d);
            }
            break;


        case COM:
            gc();
            lencom += 1;
            if (lencom > 50) {
                lencom = 0;
                return Lex(LEX_NULL, 0);
            }
            if (c == '*' && strlen(buf) == 0) {
                add();
            }
            if (c == ')' && strlen(buf) == 1) {
                add();
            }
            if (c == ')' && buf[strlen(buf) - 2] == '*' && strlen(buf) == 2) {
                gc();
                CS = H;
            }
            break;

        case ALE:
            j = look(buf, TD);
            return Lex(dlms[j], j);
            break;

        
        case NEQ:

            if (c == '=') {
                add();
                gc();
                j = look(buf, TD);
                return Lex(LEX_NEQ, j);
            }
            else {
                throw '!';
            }
            /*
            j = look(buf, TW);
            return Lex(LEX_NEQ, j);
                        */
            break;
            

        case DELIM:
            clear();
            add();

            /*
            if (c == '(') {
                gc();
                add();
                if (c == '*' && buf[strlen(buf) - 2] == '(') {
                    CS = COM;
                    clear();
                    break;
                }
            }
            */

            j = look(buf, TD);
            if (j == 0) {
                j = look(buf, TW);
                gc();
                return Lex(words[j], j);
            }
            if (j) {
                gc();
                return Lex(dlms[j], j);
            }
            else {
                throw c;
            }
            break;


        }
    } while (true);
}

///////////////////////////////
// СИНТАКСИЧЕСКИЙ АНАЛИЗАТОР //
///////////////////////////////
class Parser
{
    Lex curr_lex;       // текущая лексема
    lex_type c_type;    // её тип
    int c_val;          // её значение
    Lexer lexer;

    int stack[100];     // стек переменных для контроля повторного объявления
    int top = 0;




    // рекурсивные функции
    void P();
    void D1();
    void D();
    void B();
    void S();
    void E();
    void E1();
    void T();
    void F();

    // Получить очередную лексему
    void gl()
    {
        curr_lex = lexer.getLex();
        c_type = curr_lex.getType();
        c_val = curr_lex.getValue();
    }

    void reset()
    {
        top = 0;
    }

    void push(int i)
    {
        stack[top] = i;
        ++top;
    }

    int pop()
    {
        --top;
        return stack[top];
    }

    void dec(lex_type type)
    {
        int i;
        while (top) {
            i = pop();
            if (TID[i].isDeclared()) {
                throw "decalred twice";
            }
            TID[i].setDeclared();
            TID[i].setType(type);
        }
    }

public:

    // Провести синтаксический разбор
    void analyze()
    {
        gl();
        P();
    }

    // Конструктор
    Parser(const char* filename) : lexer(filename), top(0) {}
};


// P->  S|D1 (: | \n) {S|D1}
void Parser::P()
{
    while (true) {

        if (c_type == LEX_ID) {

            /*
            gl();
            while (c_type == LEX_COMMA) {
                gl();
                D1();
            }
            */
            D1();
        }
     
        else if(c_type == LEX_END){
            break;
        }


        else if (c_type == LEX_NULL) {
            throw curr_lex;
        }

        S();

        if (c_type == LEX_END) {
            break;
        }

        gl();
    }

}


void Parser::S()
{

    if (c_type == LEX_IF) {
        gl();
        E();
        if (c_type == LEX_THEN) {
            gl();
            S();
            gl();
            if (c_type == LEX_ELSE) {
                gl();
                S();
                gl();
            }
            if (c_type != LEX_END_ELSE) {
                throw curr_lex;
            }
        }
        else {
            throw curr_lex;
        }
    }

    else if (c_type == LEX_DO) {
        gl();
        if (c_type != LEX_WHILE) {
            throw curr_lex;
        }
        while(c_type != LEX_LOOP) {
            gl();
            S();
        }
    }

    else if (c_type == LEX_FOR) {
        gl();
        if (c_type != LEX_LPAREN) {
            throw curr_lex;
        }
        gl();
        while (c_type != LEX_SEMICOLON)
        {
            gl();
        } 
        gl();
        while  (c_type != LEX_SEMICOLON) {
            gl();
        }
        gl();
        while (c_type != LEX_RPAREN) {
            gl();
        }
        if (c_type != LEX_RPAREN) {
            throw curr_lex;
        }
    }

    else if (c_type == LEX_INPUT) {
        gl();
        if (c_type != LEX_LPAREN) {
            throw curr_lex;
        }
        gl();
        if (c_type != LEX_ID) {
            throw curr_lex;
        }
        gl();
        if (c_type != LEX_RPAREN) {
            throw curr_lex;
        }
    }
    else if (c_type == LEX_OUTPUT) {
        gl();
        if (c_type != LEX_LPAREN) {
            throw curr_lex;
        }
        gl();
        E();
        if (c_type != LEX_RPAREN) {
            throw curr_lex;
        }
    }
    else if (c_type == LEX_ID) {
        gl();
        if (c_type != LEX_ASSIGN) {
            throw curr_lex;
        }
        gl();
        E();
    }
}


// D1→ 𝐼 {,𝐼} : (%|!|$);
void Parser::D1()
{
    reset();
    if (c_type != LEX_ID) {
        throw curr_lex;
    }
    push(c_val);
    gl();
    while (c_type == LEX_COMMA) {
        gl();
        if (c_type != LEX_ID) {
            throw curr_lex;
        }
        else {
            push(c_val);
            gl();
        }
    }
    if (c_type != LEX_COLON) {
        throw curr_lex;
    }
    gl();


    if (c_type == LEX_INT) {
        this->dec(LEX_INT);
        gl();
        if (c_type != LEX_SEMICOLON) {
            throw curr_lex;
        }
    }
    else if (c_type == LEX_BOOL) {
        this->dec(LEX_BOOL);
        gl();
        if (c_type != LEX_SEMICOLON) {
            throw curr_lex;
        }
    }
    else if (c_type == LEX_REAL) {
        this->dec(LEX_REAL);
        gl();
        if (c_type != LEX_SEMICOLON) {
            throw curr_lex;
        }
    }
    else {
        throw curr_lex;
    }
}




// E→E1{[NE | EQ | LT | LE | GT | GE]E1}
void Parser::E()
{
    E1();
    if (c_type == LEX_EQ ||
        c_type == LEX_LSS ||
        c_type == LEX_GTR ||
        c_type == LEX_LEQ ||
        c_type == LEX_GEQ ||
        c_type == LEX_NEQ)
    {
        gl();
        E1();
    }
}

// E1→T{[ plus | min | or ] T}
void Parser::E1()
{
    T();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR) {
        gl();
        T();
    }
}

// T→F{[ mult  | div | and ] F}
void Parser::T()
{
    F();
    while (c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND) {
        gl();
        F();
    }
}

// F→I | N | L | ~ F | (E)
void Parser::F()
{
    if (c_type == LEX_ID) {
        gl();
    }
    else if (c_type == LEX_NUM) {
        gl();
    }
    else if (c_type == LEX_TRUE || c_type == LEX_FALSE) {
        gl();
    }
    else if (c_type == LEX_NOT) {
        gl();
        F();
    }
    else if (c_type == LEX_LPAREN) {
        gl();
        E();
        if (c_type != LEX_RPAREN) {
            throw curr_lex;
        }
        gl();
    }
    else {
        throw curr_lex;
    }
}









int main()
{
    cout << "declared twice" << endl;
    /*
    Parser parser("example.txt");
    try {
        parser.analyze();
        cout << "Ok" << endl;
    }
    catch (Lex lex) {
        cout << "Error => " << lex << endl;
    }
    */
}




/*
leon : !;
leon = 423
(* комментарий *)
if i ~= true then input(leon) end_else
if 5 EQ 5 and true then output(2) else input(leon) end_else
do while true input(leon) loop
for(i;g;2) output(test)
end
*/
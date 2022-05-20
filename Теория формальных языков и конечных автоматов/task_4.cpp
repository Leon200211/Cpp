#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Lexer {
private:
    string path = "";
    enum states {H, ASGH, ID, ERR, NM, DLM};
    enum tokenType {KWORD, IDENT, NUM, OPER, DELIM};
    string kWords[2] = {"for", "do"};
    struct tokenNode {
        string value;
        tokenType type;
    };
    struct lexList {
        tokenNode* token;
        lexList* next;
    };
    lexList* list = nullptr;
public:
    string getType(tokenType t) {
        switch (t)
        {
        case Lexer::KWORD:
                return "KWORD";
            break;
        case Lexer::IDENT:
                return "IDENT";
            break;
        case Lexer::NUM:
                return "NUM";
            break;
        case Lexer::OPER:
                return "OPER";
            break;
        case Lexer::DELIM:
                return "DELIM";
            break;
        default:
                return "NOT TOKEN";
            break;
        }

    }
    void outList() {
        lexList* target = list;
        while (target != nullptr) {
            cout << target->token->value << "\t:  " << getType(target->token->type) << endl;
            target = target->next;
        }
        cout << endl;
    }
    void addToken(string token, tokenType type) {
        tokenNode* node = new tokenNode();
        node->value = token;
        node->type = type;
        lexList* temp = new lexList();
        temp->token = node;
        lexList* target = list;
        if (list == nullptr)
            list = temp;
        else {
            while (target->next != nullptr)
                target = target->next;
            target->next = temp;
        }
    }
    bool isKeyWord(string word) {
        for (string i : kWords)
            if (i == word) return true;
        return false;
    }
    bool isNumber(string word) {
        enum numStates {S, M, D, E, ERR, P};
        numStates state = S;
        for (int i = 0; i < word.length();) {
            if (word == "++" || word == "--") return true;
            char symbol = word[i];
            switch (state)
            {
            case S:
                if (symbol == '-' || symbol == '+' || symbol == '.' || isdigit(symbol)) {
                    state = M;
                    i++;
                }
                else 
                    state = ERR;
                break;
            case M:
                if (isdigit(symbol)) {
                    i++;
                }
                else if (symbol == 'e' || symbol == 'E') {
                    state = E;
                    i++;
                }
                else if (symbol == '.') {
                    state = D;
                    i++;
                }
                else {
                    state = ERR;
                }
                break;
            case D:
                if (isdigit(symbol)) {
                    i++;
                }
                else if (symbol == 'e' || symbol == 'E') {
                    state = E;
                    i++;
                }
                else {
                    state = ERR;
                }
                break;
            case E:
                if (symbol == '-' || symbol == '+') {
                    i++;
                    state = P;
                }
                else {
                    state = ERR;
                }
                break;
            case ERR:
                return false;
                break;
            case P:
                if (isdigit(symbol)) {
                    i++;
                }
                else {
                    state = ERR;
                }
                break;
            default:
                break;
            }
        }
        return true;
    }
    Lexer(string path) {
        this->path = path;
        lex();
        outList();
    }
    int lex() {
        states state;
        state = H;
        ifstream file(path);
        char symbol = ' ';
        string word = "";
        while (!file.eof())
        {
            switch (state)
            {
            case Lexer::H:
                if (symbol == ' ' || symbol == '\n' || symbol == '\t') symbol = file.get();
                else if (symbol == ':') state = ASGH;
                else if (symbol >= 'A' && symbol <= 'Z' || symbol >= 'a' && symbol <= 'z' || symbol == '_') {
                    state = ID;
                }
                else if(isdigit(symbol) || symbol == '.' || symbol == '+' || symbol == '-'){
                    state = NM;
                }
                else {
                    state = DLM;
                }
                break;
            case Lexer::ASGH:
                symbol = file.get();
                if (symbol == '=') {
                    addToken(":=", OPER);
                    symbol = file.get();
                    state = H;
                }
                else {
                    state = ERR;
                }
                break;
            case Lexer::ID:
                if (symbol >= 'A' && symbol <= 'Z' || symbol >= 'a' && symbol <= 'z' || symbol == '_' || isdigit(symbol)) {
                    word += symbol;
                    symbol = file.get();
                }
                else {
                    if (isKeyWord(word)) 
                        addToken(word, KWORD);
                    else 
                        addToken(word, IDENT);
                    word = "";
                    state = H;
                }
                break;
            case Lexer::ERR:
                cout << "Lex error (" << symbol << ")" << endl;
                state = H;
                break;
            case Lexer::NM:
                if (symbol == 'e' || symbol == 'E' || symbol == '-' || symbol == '+' || symbol == '.' || isdigit(symbol)) {
                    word += symbol;
                    symbol = file.get();
                }
                else {
                    if (isNumber(word)) {
                        addToken(word, NUM);
                        state = H;
                    }
                    else {
                        state = ERR;
                    }
                    word = "";
                }
                break;
            case Lexer::DLM:
                if (symbol == '(' || symbol == ')' || symbol == ';' || symbol == '<' || symbol == '>' || symbol == '=') {
                    addToken(word + symbol, DELIM);
                    symbol = file.get();
                    state = H;
                }
                else {
                    state = ERR;
                }
                break;
            default:
                break;
            }
        }
        return 0;
    }
};

int main()
{
    new Lexer("test.txt");
}

/* test.txt
for(i:=0; i < 5; i++) do
print(f)
*/
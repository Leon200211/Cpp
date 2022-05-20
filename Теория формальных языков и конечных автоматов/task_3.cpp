#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Lexer {
private:
    string path = "";
    enum states {H, BO, EO, BA, ERR, EA, CA, CN, BS, L, S, NM};
    enum tokenType { BEGIN_OBJECT, END_OBJECT, BEGIN_ARRAY, END_ARRAY, COMMA, COLON, LITERAL, STRING, NUMBER };
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
        case Lexer::BEGIN_OBJECT:
            return "BEGIN_OBJECT";
            break;
        case Lexer::END_OBJECT:
            return "END_OBJECT";
            break;
        case Lexer::BEGIN_ARRAY:
            return "BEGIN_ARRAY";
            break;
        case Lexer::END_ARRAY:
            return "END_ARRAY";
            break;
        case Lexer::COMMA:
            return "COMMA";
            break;
        case Lexer::COLON:
            return "COLON";
            break;
        case Lexer::LITERAL:
            return "LITERAL";
            break;
        case Lexer::STRING:
            return "STRING";
            break;
        case Lexer::NUMBER:
            return "NUMBER";
            break;
        default:
            return "NOT TOKEN";
            break;
        }
    }
    void outList() {
        lexList* target = list;
        while (target != nullptr) {
            cout << "(" << getType(target->token->type) << ", " << "\'" << target->token->value << "\')" << endl;
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
    bool isNumber(string word) {
        enum numStates {S, M, D, E, ERR, P};
        numStates state = S;
        for (int i = 0; i < word.length();) {
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
        cout << "Token list" << endl;
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
                word = "";
                if (symbol == ' ' || symbol == '\n' || symbol == '\t') symbol = file.get();
                else if (symbol == ':') state = CN;
                else if (symbol == '{') state = BO;
                else if (symbol == '}') state = EO;
                else if (symbol == '[') state = BA;
                else if (symbol == ']') state = EA;
                else if (symbol == ',') state = CA;
                else if (symbol == 't' || symbol == 'f' || symbol == 'n') state = L;
                else if(isdigit(symbol) || symbol == '.' || symbol == '+' || symbol == '-'){
                    state = NM;
                }
                else if (symbol == '"') state = S;
                else {
                    state = ERR;
                }
                break;
            case Lexer::CN:
                word += symbol;
                addToken(word, COLON);
                symbol = file.get();
                state = H;
                break;
            case Lexer::BO:
                word += symbol;
                addToken(word, BEGIN_OBJECT);
                symbol = file.get();
                state = H;
                break;
            case Lexer::EO:
                word += symbol;
                addToken(word, END_OBJECT);
                symbol = file.get();
                state = H;
                break;
            case Lexer::BA:
                word += symbol;
                addToken(word, BEGIN_ARRAY);
                symbol = file.get();
                state = H;
                break;
            case Lexer::EA:
                word += symbol;
                addToken(word, END_ARRAY);
                symbol = file.get();
                state = H;
                break;
            case Lexer::CA:
                word += symbol;
                addToken(word, COMMA);
                symbol = file.get();
                state = H;
                break;
            case Lexer::ERR:
                cout << "Lex error (" << symbol << ")" << endl;
                symbol = file.get();
                state = H;
                break;
            case Lexer::L:
                if (symbol > 'a' && symbol < 'z') {
                    word += symbol;
                    symbol = file.get();
                }
                else {
                    if (word == "false" || word == "true" || word == "null") {
                        addToken(word, LITERAL);
                        state = H;
                    }
                    else
                        state = ERR;
                }
                break;
            case Lexer::S:
                if (word == "")
                {
                    symbol = file.get();
                }
                if (symbol == '"') {
                    symbol = file.get();
                    addToken(word, STRING);
                    state = H;
                }
                else if (symbol == '\\') {
                    word += symbol;
                    symbol = file.get();
                    word += symbol;
                    symbol = file.get();
                }
                else {
                    word += symbol;
                    symbol = file.get();
                }
                break;
            case Lexer::NM:
                if (symbol == 'e' || symbol == 'E' || symbol == '-' || symbol == '+' || symbol == '.' || isdigit(symbol)) {
                    word += symbol;
                    symbol = file.get();
                }
                else {
                    if (isNumber(word)) {
                        addToken(word, NUMBER);
                        state = H;
                    }
                    else {
                        state = ERR;
                    }
                    word = "";
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
{
  "squadName": "Super hero squad",
  "homeTown": "Metro City",
  "formed": 2016,
  "secretBase": "Super tower",
  "active": true,
  "members": [
    {
      "name": "Molecule Man",
      "age": 29,
      "secretIdentity": "Dan Jukes",
      "powers": [
        "Radiation resistance",
        "Turning tiny",
        "Radiation blast"
      ]
    },
    {
      "name": "Madame Uppercut",
      "age": 39,
      "secretIdentity": "Jane Wilson",
      "powers": [
        "Million tonne punch",
        "Damage resistance",
        "Superhuman reflexes"
      ]
    },
    {
      "name": "Eternal Flame",
      "age": 1000000,
      "secretIdentity": "Unknown",
      "powers": [
        "Immortality",
        "Heat Immunity",
        "Inferno",
        "Teleportation",
        "Interdimensional travel"
      ]
    }
  ]
}
*/
#include <iostream>
#include <vector>
#include <queue>
#include <string>

using namespace std;

struct connection {
	string from;
	char symbol;
	string to;
};
class Machine {
private:
	vector<string> states;
	vector<char> alphabet;
	vector<connection*> connections;
	string first;
	vector<string> finalStates;
public:
	bool isState(string state) {
		for (int i = 0; i < states.size(); i++)
			if (state == states.at(i))
				return true;
		return false;
	}
	bool isFinal(string state) {
		for (int i = 0; i < finalStates.size(); i++)
			if (state == finalStates.at(i))
				return true;
		return false;
	}
	void addSymbolToAlphabet(char symbol) {
		alphabet.push_back(symbol);
	}
	vector<char>* getAlphabet() {
		return &alphabet;
	}
	void addFirstState(string state) {
		first = state;
	}
	void addFinalState(string state) {
		finalStates.push_back(state);
	}
	bool addState(string state) {
		for (int i = 0; i < states.size(); i++)
			if (state == states.at(i))
				return false;
		states.push_back(state);
		return true;
	}
	void addConnecton(string from, char symbol, string to) {
		connection* temp = new connection();
		temp->from = from;
		temp->symbol = symbol;
		temp->to = to;
		connections.push_back(temp);
	}
	vector<connection*>* getConnectionsOfState(string state) {
		vector<connection*>* temp = new vector<connection*>();
		for (int i = 0; i < connections.size(); i++)
			if (connections.at(i)->from == state)
				temp->push_back(connections.at(i));
		return temp;
	}
	vector<connection*>* getConnectionsOfState(string state, char symbol) {
		vector<connection*>* temp = new vector<connection*>();
		for (int i = 0; i < connections.size(); i++)
			if (connections.at(i)->from == state && connections.at(i)->symbol == symbol)
				temp->push_back(connections.at(i));
		return temp;
	}
	vector<string>* getStates() {
		return &states;
	}
	void printMachine() {
		for (int i = 0; i < states.size(); i++)
		{
			if (first == states.at(i))
				cout << "Initial ";
			for (int j = 0; j < finalStates.size(); j++)
				if (finalStates.at(j) == states.at(i))
					cout << "Final ";
			cout << states.at(i) << endl;
			vector<connection*>* temp = getConnectionsOfState(states.at(i));
			for (int j = 0; j < temp->size(); j++)
				cout << "\t" << temp->at(j)->from << "-" << temp->at(j)->symbol << "->" << temp->at(j)->to << endl;
		}
		cout << endl;
	}
	void printMachineClassic() {
		for (int i = 0; i < states.size(); i++)
		{
			if (first == states.at(i))
				cout << "Initial ";
			for (int j = 0; j < finalStates.size(); j++)
				if (finalStates.at(j) == states.at(i))
					cout << "Final ";
			cout << states.at(i) << endl;
			vector<connection*>* temp = getConnectionsOfState(states.at(i));
			for (int j = 0; j < temp->size(); j++)
				cout << "\t" << temp->at(j)->from << "-" << temp->at(j)->symbol << "->" << temp->at(j)->to << endl;
		}
		cout << endl;
	}
	Machine* getDFM() {
		queue<string>* temp = new queue<string>();
		Machine* result = new Machine();
		result->addState(first);
		result->addFirstState(first);
		temp->push(first);
		while (temp->size() > 0) {
			string state = temp->front();
			for (int i = 0; i < alphabet.size(); i++)
			{
				string newState = "";
				bool thisFinal = false;
				for (int k = 0; k < states.size(); k++)
				{
					if (state.find(states.at(k)) != state.npos) {
						vector<connection*>* connectionsBySymbol = getConnectionsOfState(states.at(k), alphabet.at(i));
						for (int j = 0; j < connectionsBySymbol->size(); j++) {
							if (newState.find(connectionsBySymbol->at(j)->to) == newState.npos) {
								newState += connectionsBySymbol->at(j)->to;
								if (isFinal(connectionsBySymbol->at(j)->to))
									thisFinal = true;
							}
						}
					}
				}
				if (newState != "") {
					if (result->isState(newState)) {
						result->addConnecton(state, alphabet.at(i), newState);
					}else if (result->addState(newState)) {
						result->addConnecton(state, alphabet.at(i), newState);
						if (thisFinal)
							result->addFinalState(newState);
						temp->push(newState);
					}
				}
			}
			temp->pop();
		}
		return result;
	}
};

int main()
{
	Machine* test = new Machine();
	//(1,a,1) (1,a,2) (1,b,3) (2,a,2) (2,b,1) (2,b,3) (3,a,3) (3,b,3)
	
	cout << "Enter set of states: " << endl;
	string temp;
	getline(cin, temp);
	string word = "";
	for (char i : temp)
	{
		if (i != ' ') {
			word += i;
		}
		else {
			test->addState(word);
			word = "";
		}
	}
	if(word != "")
		test->addState(word);
	cout << "Enter the input alphabet: " << endl;
	getline(cin, temp);
	for (char i : temp)
		if (i != ' ')
			test->addSymbolToAlphabet(i);
	cout << "Enter state-transitions function (current state, input character, next state): " << endl;
	getline(cin, temp);
	word = ""; temp += " ";
	for (char i : temp)
	{
		if (i != ' ') {
			word += i;
		}
		else {
			string from = "";
			char symbol;
			string to = "";
			int j = 0;
			for (j; word[j] != ','; j++)
				if (word[j] != '(')
					from += word[j];
			j++;
			symbol = word[j];
			j += 2;
			for (j; word[j] != ')'; j++)
				if (word[j] != '(')
					to += word[j];
			test->addConnecton(from, symbol, to);
			word = "";
		}
	}
	cout << "Enter a set of initial states: " << endl;
	getline(cin, temp);
	word = "";
	for (char i : temp)
	{
		if (i != ' ') {
			word += i;
		}
		else {
			test->addFirstState(word);
			word = "";
		}
	}
	if (word != "")
		test->addFirstState(word);
	cout << "Enter a set of final states: " << endl;
	getline(cin, temp);
	word = "";
	for (char i : temp)
	{
		if (i != ' ') {
			word += i;
		}
		else {
			test->addFinalState(word);
			word = "";
		}
	}
	if (word != "")
		test->addFinalState(word);

	test->printMachine();
	test->getDFM()->printMachine();
	/*
	test->addState("1");
	test->addState("2");
	test->addState("3");
	test->addSymbolToAlphabet('a');
	test->addSymbolToAlphabet('b');
	test->addConnecton("1", 'a', "1");
	test->addConnecton("1", 'a', "2");
	test->addConnecton("1", 'b', "3");
	test->addConnecton("2", 'a', "2");
	test->addConnecton("2", 'b', "1");
	test->addConnecton("2", 'b', "3");
	test->addConnecton("3", 'a', "3");
	test->addConnecton("3", 'b', "3");
	test->addFinalState("3");
	test->addFirstState("1");
	test->getDFM()->printMachine();*/
	/*
	test->addState("1");
	test->addState("2");
	test->addSymbolToAlphabet('a');
	test->addSymbolToAlphabet('b');
	test->addConnecton("1", 'a', "1");
	test->addConnecton("1", 'a', "2");
	test->addConnecton("1", 'b', "1");
	test->addConnecton("2", 'b', "1");
	test->addConnecton("2", 'b', "2");
	test->addFinalState("2");
	test->addFirstState("1");
	test->getDFM()->printMachine();
	*/
}

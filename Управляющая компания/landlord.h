// landlord.h
// заголовочный файл landlord.cpp - содержит объ€влени€
// классов и т.п.
#pragma warning (disable:4786) // дл€ множеств (только
 // компил€торы microsoft)
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm> // дл€ sort()
#include <numeric> // дл€ accumulate()
using namespace std;
////////////////////глобальные методы//////////////////////
void getaLine(string& inStr); // получение строки текста
char getaChar(); // получение символа
////////////////////класс tenant (жильцы)//////////////////
class tenant
{
private:
	string name; // им€ жильца
	int aptNumber; // номер комнаты жильца
	// здесь может быть проча€ информаци€ о жильце
	// (телефон и т.п.)
public:
	tenant(string n, int aNo);
	~tenant();
	int getAptNumber();
	// нужно дл€ использовани€ во множестве
	friend bool operator<(const tenant&, const tenant&);
	friend bool operator==(const tenant&, const tenant&);
	// дл€ операций ввода-вывода
	friend ostream& operator<<(ostream&, const tenant&);
}; // конец объ€влени€ класса tenant
////////////////////класс compareTenants///////////////////
class compareTenants // функциональный объект дл€
 // сравнени€ имен жильцов
{
public:
	bool operator()(tenant*, tenant*) const;
};
///////////////////класс tenantList////////////////////////
class tenantList
{
private:
	// установить указатели на жильцов
	set<tenant*, compareTenants> setPtrsTens;
	set<tenant*, compareTenants>::iterator iter;
public:
	~tenantList(); // деструктор
	// (удаление жильцов)
	void insertTenant(tenant*); // внесение жильца в список
	int getAptNo(string); // возвращает номер комнаты
	void display(); // вывод списка жильцов
};
// конец объ€влени€ класса tenantList
////////////////класс tenantInputScreen////////////////////
class tenantInputScreen
{
private:
	tenantList* ptrTenantList;
	string tName;
	int aptNo;
public:
	tenantInputScreen(tenantList* ptrTL) : ptrTenantList(ptrTL)
	{ /* тут пусто */
	}
	void getTenant();
}; // конец класса tenantInputScreen
////////////////////класс rentRow//////////////////////////
// одна строка таблицы прибыли: адрес и 12 мес€чных оплат
class rentRow
{
private:
	int aptNo;
	float rent[12];
public:
	rentRow(int); // конструктор с одним
	// параметром
	void setRent(int, float); // запись платы за мес€ц
	float getSumOfRow(); // сумма платежей
	// из одной строки
	// нужно дл€ сохранени€ во множестве
	friend bool operator<(const rentRow&, const rentRow&);
	friend bool operator==(const rentRow&, const rentRow&);
		// дл€ вывода
	friend ostream & operator<<(ostream&, const rentRow&);
}; // конец класса rentRow
///////////////////////////////////////////////////////////
class compareRows // функциональный объект сравнени€
 // объектов rentRows
{
public:
	bool operator()(rentRow*, rentRow*) const;
};
////////////////////класс rentRecord///////////////////////
class rentRecord
{
private:
	// множество указателей на объекты rentRow (по одному на
	// жильца)
	set<rentRow*, compareRows> setPtrsRR;
	set<rentRow*, compareRows>::iterator iter;
public:
	~rentRecord();
	void insertRent(int, int, float);
	void display();
	float getSumOfRents(); // сумма всех платежей
}; // конец класса rentRecord
////////////////////класс rentInputScreen//////////////////
class rentInputScreen
{
private:
	tenantList* ptrTenantList;
	rentRecord* ptrRentRecord;
	string renterName;
	float rentPaid;
	int month;
	int aptNo;
public:
	rentInputScreen(tenantList* ptrTL, rentRecord* ptrRR) :
		ptrTenantList(ptrTL), ptrRentRecord(ptrRR)
	{ /*тут пусто*/
	}
	void getRent(); // арендна€ плата одного
	// жильца за один мес€ц
}; // конец класса rentInputScreen
///////////////////////класс expense///////////////////////
class expense
{
public:
	int month, day;
	string category, payee;
	float amount;
	expense()
	{ }
	expense(int m, int d, string c, string p, float a) :
		month(m), day(d), category(c), payee(p), amount(a)
	{ /* тут пусто! */}
	// нужно дл€ хранени€ во множестве
	friend bool operator<(const expense&, const expense&);
	friend bool operator==(const expense&, const expense&);
	// нужно дл€ вывода
	friend ostream& operator<<(ostream&, const expense&);
}; // конец класса expense
///////////////////////////////////////////////////////////
class compareDates // функциональный объект сравнени€ затрат
{
public:
	bool operator()(expense*, expense*) const;
};
///////////////////////////////////////////////////////////
class compareCategories // функциональный объект сравнени€ затрат
{
public:
	bool operator()(expense*, expense*) const;
};
///////////////////класс expenseRecord/////////////////////
class expenseRecord
{
private:
	// вектор указателей на расходы
	vector<expense*> vectPtrsExpenses;
	vector<expense*>::iterator iter;
public:
	~expenseRecord();
	void insertExp(expense*);
	void display();
	float displaySummary(); // нужно дл€ годового отчета
}; // конец класса expenseRecord
////////////////класс expenseInputScreen///////////////////
class expenseInputScreen
{
private:
	expenseRecord* ptrExpenseRecord;
public:
	expenseInputScreen(expenseRecord*);
	void getExpense();
}; // конец класса expenseInputScreen
//////////////////класс annualReport///////////////////////
class annualReport
{
private:
	rentRecord * ptrRR;
	expenseRecord* ptrER;
	float expenses, rents;
public:
	annualReport(rentRecord*, expenseRecord*);
	void display();
}; // конец класса annualReport
//////////////////конец userInterface//////////////////////
class userInterface
{
private:
	tenantList* ptrTenantList;
	tenantInputScreen* ptrTenantInputScreen;
	rentRecord* ptrRentRecord;
	rentInputScreen* ptrRentInputScreen;
	expenseRecord* ptrExpenseRecord;
	expenseInputScreen* ptrExpenseInputScreen;
	annualReport* ptrAnnualReport;
	char ch;
public:
	userInterface();
	~userInterface();
	void interact();
}; // конец класса userInterfac
//////////////////конец файла landlord.h///////////////////
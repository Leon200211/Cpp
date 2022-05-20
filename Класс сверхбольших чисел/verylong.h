#pragma once

// без этой хуитени компилятор ебет нам мозг из за С4996
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE

#include<iostream>
#include<string>
#include<stdlib.h>

using namespace std;

const int SZ = 100000;



class verylong
{
private:
	char vlstr[SZ]; //число как строка
	int vlen; //длина строки verylong
	verylong multdigit(const int) const; //прототипы
	verylong mult10(const verylong) const;// скрытых функций
public:
	verylong() : vlen(0) //конструктор без аргументов
	{
		vlstr[0] = '\0';
	}
	verylong(const char s[SZ]) //конструктор (1 аргумент)
	{
		strcpy(vlstr, s); vlen = strlen(s);
	} //для строки
	verylong(const unsigned long n) //конструктор (1 арг.)
	{
		ltoa(n, vlstr, 10); //перевести в строку
		strrev(vlstr); //перевернуть ее
		vlen = strlen(vlstr); //найти длину
	}
	void putvl() const; //вывести число
	void getvl(); //получить число от пользователя
	verylong operator + (const verylong); //сложить числа
	verylong operator * (const verylong); //умножить
};
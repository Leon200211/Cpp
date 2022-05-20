// вычисляет факториалы больших чисел
#include "verylong.h" //заголовочный файл verylong
int main()
{
	setlocale(LC_ALL, "Russian");

	unsigned long numb, j;
	verylong fact = 1; //инициализировать verylong
	cout << "\n\nВведите число: ";
	cin >> numb; //ввод числа типа long int
	for (j = numb; j > 0; j--) //факториал — это numb *
		fact = fact * j; // numb-1 * numb-2 *
	cout << "Факториал= "; // numb-3 и т. д.
	fact.putvl(); //вывести значение факториала
	cout << endl;
	return 0;
}
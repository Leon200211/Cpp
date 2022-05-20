// verylong.cpp
// реализация обработки данных типа verylong
#include "verylong.h" //заголовочный файл для verylong
//---------------------------------------------------------
void verylong::putvl() const //вывод на экран verylong
{
	char temp[SZ];
	strcpy(temp, vlstr); //создать копию
	cout << strrev(temp); //перевернуть копию
} //и вывести ее
//---------------------------------------------------------
void verylong::getvl() //получить сверхбольшое число от
 //пользователя
{
	cin >> vlstr; //получить строку от пользователя
	vlen = strlen(vlstr); //найти ее длину
	strrev(vlstr); //перевернуть ее
}//---------------------------------------------------------
verylong verylong::operator + (const verylong v)
//сложение
{
	char temp[SZ];
	int j;
	//найти самое длинное число
	int maxlen = (vlen > v.vlen) ? vlen : v.vlen;
	int carry = 0; //установить в 1, если сумма >= 10
	for (j = 0; j < maxlen; j++) //и так для каждой позиции
	{
		int d1 = (j > vlen - 1) ? 0 : vlstr[j] - '0'; //получить
		//разряд
		int d2 = (j > v.vlen - 1) ? 0 : v.vlstr[j] - '0'; //и еще
		int digitsum = d1 + d2 + carry; //сложить разряды
		if (digitsum >= 10) //если перенос, то
		{
			digitsum -= 10; carry = 1;
		} //увеличить сумму на 10
		else //установить перенос в 1
			carry = 0; //иначе перенос = 0
		temp[j] = digitsum + '0'; //вставить символ в строку
	}
	if (carry == 1) //если перенос в конце,
		temp[j++] = '1'; //последняя цифра = 1
	temp[j] = '\0'; //поставить ограничитель строки
	return verylong(temp); //вернуть временный verylong
}
//---------------------------------------------------------
verylong verylong::operator * (const verylong v)//умножение сверхбольших чисел
{
	verylong pprod; //произведение одного разряда
	verylong tempsum; //текущая сумма
	for (int j = 0; j < v.vlen; j++)//для каждого разряда аргумента
	{
		int digit = v.vlstr[j] - '0'; //получить разряд
		pprod = multdigit(digit); //умножить текущий на него
		for (int k = 0; k < j; k++) //умножить результат на
			pprod = mult10(pprod); // степень 10-ти
		tempsum = tempsum + pprod; //прибавить произведение к текущей сумме
	}
	return tempsum; //вернуть полученную текущую сумму
}
//---------------------------------------------------------
verylong verylong::mult10(const verylong v) const //умножение аргумента на 10
{
	char temp[SZ];
	for (int j = v.vlen - 1; j >= 0; j--)//сдвинуться на один разряд
		temp[j + 1] = v.vlstr[j]; //выше
	temp[0] = '0'; //обнулить самый младший разряд
	temp[v.vlen + 1] = '\0'; //поставить ограничитель строки
	return verylong(temp); //вернуть результат
}
//---------------------------------------------------------
verylong verylong::multdigit(const int d2) const
{ //умножение числа на
	char temp[SZ]; //аргумент (цифру)
	int j, carry = 0;
	for (j = 0; j < vlen; j++) //для каждого разряда
	{ // в этом сверхбольшом
		int d1 = vlstr[j] - '0'; //получить значение разряда
		int digitprod = d1 * d2; //умножить на цифру
		digitprod += carry; //добавить старый перенос
		if (digitprod >= 10) //если возник новый перенос,
		{
			carry = digitprod / 10; //переносу присвоить //значение старшего разряда
			digitprod -= carry * 10; //результату - младшего
		}
		else
			carry = 0; //иначе перенос = 0
		temp[j] = digitprod + '0'; //вставить символ в строку
	}
	if (carry != 0) //если на конце перенос,
		temp[j++] = carry + '0'; //это последний разряд
	temp[j] = '\0'; //поставить ограничитель
	return verylong(temp); //вернуть сверхбольшое число
}
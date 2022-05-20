#include "elev_app.h" //поставляется клиентом
//(застройщиком)
#include "msoftcon.h" //для консольной графики
#include <iostream>
#include <iomanip> //для setw()
#include <conio.h> //для вывода на экран
#include <stdlib.h> //для itoa()
#include <process.h> //для exit()
using namespace std;
enum direction { UP, DN, STOP };
const int LOAD_TIME = 3; //время посадки/высадки
 //(В tick'ах)
const int SPACING = 7; //расстояние между
 //изображениями кабинок
const int BUF_LENGTH = 80; //длина буфера рабочей строки
class building; //будущее объявление
///////////////////////////////////////////////////////////
class elevator
{
private:
	building* ptrBuilding; //указатель на building
	const int car_number; //номер лифта (от 0 до nc-1)
	int current_floor; //где мы? (от 0 до nf-1)
	int old_floor; //куда едем? (от 0 до nf-1)
	direction current_dir; //в каком направлении?
	bool destination[NUM_FLOORS]; //выбирается пассажирами
	int loading_timer; //ненулевое во время посадки
	int unloading_timer; //ненулевое во время высадки
public:
	elevator(building*, int); //конструктор
	void car_tick1(); //метроном-1 для каждой кабинки
	void car_tick2(); //метроном-2 для каждой кабинки
	void car_display(); //рисование лифта
	void dests_display() const;//вывод запросов
	void decide(); //принятие решения
	void move(); //движение лифта
	void get_destinations(); //получение номеров конечных этажей
	int get_floor() const; //получение текущего этажа
	direction get_direction() const; //получение текущего направления
};
///////////////////////////////////////////////////////////
class building
{
private:
	elevator* car_list[NUM_CARS]; //указатель на кабинки
	int num_cars; //уже созданные лифты
	//массив кнопок «вверх», «вниз»
	bool floor_request[2][NUM_FLOORS];//false=вверх, true=вниз
public:
	building(); //конструктор
	~building(); //деструктор
	void master_tick(); //рассылка временных меток всем
	//лифтам
	int get_cars_floor(const int) const; //поиск лифта
	//выяснение направления движения
	direction get_cars_dir(const int) const;
	//проверка запроса с этажа
	bool get_floor_req(const int, const int) const;
	//установка запроса с этажа
	void set_floor_req(const int, const int, const bool);
	void record_floor_reqs(); //получение запроса с этажа
	void show_floor_reqs() const; //вывод запроса
};
///////////////////////////////////////////////////////////
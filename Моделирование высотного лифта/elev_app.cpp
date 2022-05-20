// elev_app.cpp
// клиентский файл
#include "elev.h" //для объявлений классов
int main()
{
	setlocale(LC_ALL, "Russian");

	building theBuilding;
	while (true)
	{
		theBuilding.master_tick(); //послать временные метки
		//всем лифтам
		wait(1000); //пауза
		//получить запросы этажей от пользователей
		theBuilding.record_floor_reqs();
	}
	return 0;
}
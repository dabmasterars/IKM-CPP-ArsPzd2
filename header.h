#include <iostream>
using namespace std;
class Dominode {//структура для домино; двойной список с двумя значениями и булевым тегом для удобной работы в списке
    public:
    //int first, second;
    int pattern[2];
    Dominode* prev;//пришлось написать prev и next по отдельности, иначе вылазит ошибка
    Dominode* next;
    Dominode(int f, int s) : pattern{f,s}, prev(nullptr), next(nullptr) {}
    bool used=0;//если 1, это значит, что домино было поставлено в спискок и выведено
};
bool isNumber(const string& s);//проверка, является ли строка положительным номером
bool dominoCheck(string s);//проверка, является ли строка числом от 0 до 6
void addDomino(Dominode*& head, int first, int second);//добавление кости в список
bool arrangeDominoes(Dominode* head, unsigned long long int n);//проверка, можно ли сделать ряд из домино и вывод этого ряда
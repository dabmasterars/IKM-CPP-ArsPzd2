#include "header.h"
// Функция для добавления кости в двухсвязный список
bool isNumber(const string& s) {//проверка, является ли строка положительным номером
    if (s.empty())//если строка пустая, возвращаем 0
        return 0;
    for (char c : s)//проверяем каждый знак, является ли он числом
        if (!isdigit(c))
            return 0;
    return 1;
}
bool dominoCheck(string s){//проверка, является ли строка числом от 0 до 6
    if(s=="0"||s=="1"||s=="2"||s=="3"||s=="4"||s=="5"||s=="6")
        return 1;
    else
        return 0;
}
void addDomino(Dominode*& head, int first, int second){
    Dominode* newDomino = new Dominode(first, second);//новый нод
    if (!head) //если список пустой, делаем нод началом списка
        head = newDomino;
    else{
        Dominode* temp = head;
        while (temp->next)//двигаемся в конец списка
            temp = temp->next;
        temp->next = newDomino;//добавляем нод в список
        newDomino->prev = temp;
    }
}
void checkIdenticalDominoes(Dominode* head){//проверка одинаковых костей
    Dominode* target=head;
    while(target){//если две кости одинаковы или симметричны, то программа закрывается
        if((target->pattern[0]==head->pattern[0]&&target->pattern[1]==head->pattern[1])
        ||(target->pattern[0]==head->pattern[1]&&target->pattern[1]==head->pattern[0])){
            cout<<"Нарушение правил домино: несколько одинаковых костей";
            exit(0);
        }
        target=target->next;//если нет, идём дальше по списку
    }
}
bool arrangeDominoes(Dominode* head, unsigned long long int n) {//проверка, можно ли сделать ряд из домино и вывод этого ряда
    if (!head) return 0;//если список пустой (такого не должно быть, но на всякий случай)
    int degree[7] = {0};//массив для подчёта числа паттернов (a[i]=x - паттерн с i точками встречается x раз)
    Dominode* temp = head;
    while (temp){//идём по списку и заносим точки в массив
        degree[temp->pattern[0]]++;
        degree[temp->pattern[1]]++;
        temp=temp->next;
    }
    
    Dominode* tempIdentical = head;
    while (tempIdentical->next){//проверяем, не повторяется ли домино (это против правил)
        checkIdenticalDominoes(tempIdentical);
        tempIdentical=tempIdentical->next;//делаем операцию с каждым домино
    }
    //Паттерны точек у примыкающих половинок разных домино в ряду должны совпадать. Из этого следует,
    //то что количество половинок домино с одинаковым паттерном точек должно быть чётное,
    //за возможным исключением двух половинок, которые могут встречаться в нечётном количестве: они стоят в начале и в конце ряда.
    
    short int nBuffer, nBuffer2;//запоминаем паттерны, которые встречаются нечётное количество раз...
    int patbuffer;//...а также паттерн второй половины домино (когда будем составлять ряд)
    bool bufCheck=1;//для переключения между буферами в цикле (сначала nBuffer, потом nBuffer2)
    int oddCount = 0;//проверяем число паттернов, которые встречаются нечётное количество раз
    for (int i = 0; i < 7; i++) {
        if (degree[i] % 2 != 0) {
            oddCount++;
            if(bufCheck){nBuffer=i; bufCheck=0;}
            else nBuffer2=i;
        }
    }
    if (oddCount>2){//проверяем условие
        cout << "Невозможно расставить кости в ряд" << endl;
        exit(0);
    }
    
    if (oddCount==0){//если крайние паттерны могут быть любыми, берём любые числа
        nBuffer=head->pattern[0]; nBuffer=head->next->pattern[0];
    }
    
    //cout<<"Крайние паттерны: "<<nBuffer<<" "<<nBuffer2<<endl;//дебуг
    
    Dominode* headbuffer=head;//сохраняем начало списка в буфер для сброса позиции в списке
    
    while (head) {//идём по списку и ищем нод с крайним паттерном; выводим его и помечаем, как прочитанный
        if (head->pattern[0]==nBuffer||head->pattern[1]==nBuffer){
            if (head->pattern[0]==nBuffer2||head->pattern[1]==nBuffer2){//если оба крайних паттерна в одном и том же домино, разложить нельзя
                cout << "Невозможно расставить кости в ряд" << endl;
                exit(0);
            }
            else{
                if (head->pattern[1]==nBuffer){//переворачиваем домино, если искомый паттерн стоит справа
                    int temp=head->pattern[1];
                    head->pattern[1]=head->pattern[0];
                    head->pattern[0]=temp;
                }
                cout<<"Можно уложить кости так: "<<head->pattern[0]<<head->pattern[1];
                patbuffer=head->pattern[1];
                head->used=1;//помечаем кость домино как использованную
                break;
            }
        }
        head = head->next;
    }
    n--;
    while(n>1){//ищем и выводим домино, которые должны стоять между двумя домино с крайними паттернами
        //идём по списку и проверяем, чтобы домино не было использовано и чтобы в нём не было второго крайнего паттерна
        //продолжаем, пока количество костей домино не будет равно 1 (последняя кость содержит второй крайний паттерн)
        if ((head->pattern[0]==patbuffer||head->pattern[1]==patbuffer)&&(!head->used)&&(head->pattern[0]!=nBuffer2&&head->pattern[1]!=nBuffer2)){
            if (head->pattern[1]==patbuffer){//переворачиваем домино, если искомый паттерн стоит справа
                int temp=head->pattern[1];
                head->pattern[1]=head->pattern[0];
                head->pattern[0]=temp;
            }
            cout<<", "<<head->pattern[0]<<head->pattern[1];
            patbuffer=head->pattern[1];
            head->used=1;
            n--;
        }
        if (!head->next){//если достигнут конец списка, двигаемся в начало
            while (head->prev) head=head->prev;
        }
        else head = head->next;//иначе двигаемся вперёд
    }
    while (head) {//идём по списку и ищем нод со вторым крайним паттерном
        if (head->pattern[0]==nBuffer2||head->pattern[1]==nBuffer2){
            if (head->pattern[0]==nBuffer2){//переворачиваем домино, если искомый паттерн стоит справа
                int temp=head->pattern[1];
                head->pattern[1]=head->pattern[0];
                head->pattern[0]=temp;
            }
            cout<<", "<<head->pattern[0]<<head->pattern[1];
            break;
        }
        if (!head->next){
            while (head->prev) head=head->prev;
        }
        else head = head->next;
    }
    return 1;
}
bool arrangeTwoDominoes(Dominode* head) {//проверка, можно ли сделать ряд из домино и вывод этого ряда
    if (!head) return 0;//если список пустой (такого не должно быть, но на всякий случай)
    int degree[7] = {0};//массив для подчёта числа паттернов (a[i]=x - паттерн с i точками встречается x раз)
    Dominode* temp = head;
    while (temp){//идём по списку и заносим точки в массив
        degree[temp->pattern[0]]++;
        degree[temp->pattern[1]]++;
        temp=temp->next;
    }
    
    checkIdenticalDominoes(head);//проверяем, не повторяется ли домино (это против правил)
    
    short int nBuffer, nBuffer2;//запоминаем паттерны, которые встречаются нечётное количество раз...
    int patbuffer;//...а также паттерн второй половины домино (когда будем составлять ряд)
    bool bufCheck=1;//для переключения между буферами в цикле (сначала nBuffer, потом nBuffer2)
    int oddCount = 0;//проверяем число паттернов, которые встречаются нечётное количество раз
    for (int i = 0; i < 7; i++) {
        if (degree[i] % 2 != 0) {
            oddCount++;
            if(bufCheck){nBuffer=i; bufCheck=0;}
            else nBuffer2=i;
        }
    }
    if (oddCount>0){//проверяем условие
        cout << "Невозможно расставить кости в ряд" << endl;
        exit(0);
    }
    cout<<"Можно уложить кости так: "<<head->pattern[0]<<head->pattern[1];
    head=head->next;
    if(head->prev->pattern[1]==head->pattern[1]){
        int temp=head->pattern[1];
        head->pattern[1]=head->pattern[0];
        head->pattern[0]=temp;
    }
    cout<<", "<<head->pattern[0]<<head->pattern[1];
    return 1;
}

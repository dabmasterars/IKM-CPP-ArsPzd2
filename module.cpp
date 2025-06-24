#include "header.h"
// Функция для добавления кости в двухсвязный список
bool isNumber(const string& s) {//проверка, является ли строка положительным номером
    if (s.empty())//если строка пустая, возвращаем 0
        return 0;
    for (char c : s)//проверяем каждый знак, является ли он числом
        if (!isdigit(c))
            return 0;
    return 1; // If all characters are digits, return true
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
bool arrangeDominoes(Dominode* head, unsigned long long int n) {//проверка, можно ли сделать ряд из домино и вывод этого ряда
    if (!head) return 0;//если список пустой (такого не должно быть, но на всякий случай)
    int degree[7] = {0};//массив для подчёта числа паттернов (a[i]=x - паттерн с i точками встречается x раз)
    Dominode* temp = head;
    while (temp){//идём по списку и заносим точки в массив
        degree[temp->first]++;
        degree[temp->second]++;
        temp=temp->next;
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
        nBuffer=head->first; nBuffer=head->next->first;
    }
    
    //cout<<"Крайние паттерны: "<<nBuffer<<" "<<nBuffer2<<endl;//дебуг
    
    Dominode* headbuffer=head;//сохраняем начало списка в буфер для сброса позиции в списке
    
    while (head) {//идём по списку и ищем нод с крайним паттерном; выводим его и помечаем, как прочитанный
        if (head->first==nBuffer||head->second==nBuffer){
            if (head->first==nBuffer2||head->second==nBuffer2){//если оба крайних паттерна в одном и том же домино, разложить нельзя
                cout << "Невозможно расставить кости в ряд" << endl;
                exit(0);
            }
            else{
                if (head->second==nBuffer){//переворачиваем домино, если искомый паттерн стоит справа
                    int temp=head->second;
                    head->second=head->first;
                    head->first=temp;
                }
                cout<<"Можно уложить кости так: "<<head->first<<head->second;
                patbuffer=head->second;
                head->used=1;//помечаем кость домино как использованную
                // Dominode* temp = head;
                // head->next=headbuffer;
                // headbuffer->prev=temp;//домино с крайним паттерном переносим в начало
                // head->prev=NULL;
                break;
            }
        }
        head = head->next;
    }
    n--;
    while(n>1){//ищем и выводим домино, которые должны стоять между двумя домино с крайними паттернами
        //идём по списку и проверяем, чтобы домино не было использовано и чтобы в нём не было второго крайнего паттерна
        //продолжаем, пока количество костей домино не будет равно 1 (последняя кость содержит второй крайний паттерн)
        if ((head->first==patbuffer||head->second==patbuffer)&&(!head->used)&&(head->first!=nBuffer2&&head->second!=nBuffer2)){
            if (head->second==patbuffer){//переворачиваем домино, если искомый паттерн стоит справа
                int temp=head->second;
                head->second=head->first;
                head->first=temp;
            }
            cout<<", "<<head->first<<head->second;
            patbuffer=head->second;
            head->used=1;
            n--;
        }
        if (!head->next){//если достигнут конец списка, двигаемся в начало
            while (head->prev) head=head->prev;
        }
        else head = head->next;//иначе двигаемся вперёд
    }
    while (head) {//идём по списку и ищем нод со вторым крайним паттерном
        if (head->first==nBuffer2||head->second==nBuffer2){
            if (head->first==nBuffer2){//переворачиваем домино, если искомый паттерн стоит справа
                int temp=head->second;
                head->second=head->first;
                head->first=temp;
            }
            cout<<", "<<head->first<<head->second;
            break;
        }
        if (!head->next){
            while (head->prev) head=head->prev;
        }
        else head = head->next;
    }
    //head=headbuffer->prev;
    // temp = head;
    // headbuffer->prev=temp;
    // head->next=headbuffer;
    // head->prev=NULL;
    
    // cout<<"\n\n";
    // while (head!=NULL){
    //     cout<<head->first<<head->second<<" ";
    //     head=head->next;
    // }
    // cout<<"\n\n";
    
    // Dominode* tail=head;//конец списка
    // //Dominode* tailbuffer=tail;
    // while(tail->next!=NULL){
    //     tail=tail->next;
    // }
    // while (head) {//идём по списку и ищем ноды с крайними паттернами
    //     Dominode* temp = head;
    //     if (head->first==nBuffer2||head->second==nBuffer2){
    //         tail->next=temp;//домино с крайним паттерном переносим в начало
            
    //         tail=temp;
    //         break;
    //     }
    //     head = head->next;
    // }
    // head=headbuffer->prev;
    // if (head->second==nBuffer){//если крайний паттерн стоит справа, переворачиваем домино
    //     int temp=head->second;
    //     head->second=head->first;
    //     head->first=temp;
    // }
    // headbuffer=head;//обновляем буфер начала списка
    // int patbuffer;//буфер для второй половины домино
    // for (int i=0; i<n; i++){
    //     Dominode* temp=head;
    //     cout<<head->first<<head->second<<" ";
    //     patbuffer=head->second;
    //     delete head;
    //     while((temp->second==head->next->first||temp->second==head->next->second)&&(head->next->first!=nBuffer2||head->next->second!=nBuffer2)){
    //         head=head->next;
    //     }
    //     head=head->next;
    //     if (temp->second==head->first){
    //         cout<<head->first<<head->second<<" ";
    //     }
    //     if (temp->second==head->second){//переворачиваем домино если надо
    //         int temp=head->second;
    //         head->second=head->first;
    //         head->first=temp;
    //         cout<<head->first<<head->second<<" ";
    //     }
    // }
    return 1;
}
// void freeDominoList(Dominode* head) {
//     while (head) {//идём по списку и удаляем ноды по очереди
//         Dominode* temp = head;
//         head = head->next;
//         delete temp;
//     }
// }
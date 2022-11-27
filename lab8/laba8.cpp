#include <iostream>

using namespace std;

/* Шаблон структуры элемента */
template <typename T>
struct Element
{
    T element; // элемент
    Element<T>* next; // следующий от элемента
    Element<T>* prev; // предыдущий от элемента

    // Оператор вывода
    template <typename V>
    friend ostream& operator<<(ostream& out, Element<V> el);
};

/* Шаблон класса списка */
template <typename T>
class List
{
public:
    Element <T>* start_element; // Первый элемент списка
    Element <T>* end_element; // Последний элемент списка
    int count; // Количество элементов в списке

    List() {
        start_element = end_element = NULL; // пустой список
        count = 0;
        checkList(); // проверка на пустоту
    }

    // Итератор
    class Iterator {
    private:
        List<T>* container; // Контейнер
        int index; // Индекс
    public:
        Iterator(int index, List<T>* container) :
            container(container),
            index(index > container->count ? -1 : index) {
        }
        // оператор ++
        Iterator& operator++() {
            if (index != container->count)
                index++;
            else
                index = -1;
            return *this;
        }
        // оператор указателя на элемент
        T operator*() {                            
            return container->getElement(index);
        }
        // оператор !=
        bool operator != (Iterator& it) {
            return it.index != index;
        }
    };

    // Добавление элемента в конец списка
    void addEnd(int el)
    {
        Element <T>* temp = new Element<T>; // Создание элемента
        temp->next = 0; // Следующий
        temp->element = el; // Сам элемент
        temp->prev = end_element; // Предыдущий
        // Существуют ли элементы
        if (end_element != 0)
            end_element->next = temp;
        // Если элементов нет
        if (count == 0)
            start_element = end_element = temp;
        // Добавление элемента в конец
        else
            end_element = temp;
        count++; // Увеличение списка
    }

    // Добавление элемента в начало списка
    void addStart(int el)
    {
        Element<T>* temp = new Element<T>; // Создание элемента
        temp->prev = 0; // Предыдущий
        temp->element = el; // Сам элемент
        temp->next = start_element; // Следующий
        // Существуют ли элементы
        if (start_element != 0)
            start_element->prev = temp;
        // Если элементов нет
        if (count == 0)
            start_element = end_element = temp;
        // Добавление элемента в конец
        else
            start_element = temp;
        count++; // Увеличение списка
    }

    // Вывод списка
    void print()
    {
        // Если элементы существуют
        if (count != 0)
        {
            Element<T>* temp = start_element; // Начальный элемент
            cout << "List: ( ";
            while (temp->next != 0) // Пока есть элементы
            {
                cout << temp->element << ", ";
                temp = temp->next; // Следующий
            }
            cout << temp->element << " )\n"; // Последний
        }
    }

    // Получение длины списка
    int getCount()
    {
        return count;
    }

    // Проверка на пустоту списка
    void checkList() {
        if (getCount() == 0) cout << "\nСписок пуст\n";
        else cout << "\nСписок не пуст\n";
    }

    // Получение начального элемента
    T getStartElement() {
        Element<T>* temp;
        temp = start_element;
        return temp->element;
    }

    // Получение последнего элемента
    T getEndElement() {
        Element<T>* temp;
        temp = end_element;
        return temp->element;
    }
    
    // Удаление начального элемента
    void deleteStart() {
        Element<T>* deleteElement = start_element; 
        Element<T>* afterDeleteElement = deleteElement->next;
        start_element = afterDeleteElement; // новый первый элемент
        delete deleteElement;
        count--;
    }

    // Удаление последнего элемента
    void deleteEnd() {
        Element<T>* Del = end_element;
        Element<T>* PrevDel = Del->prev;
        Element<T>* AfterDel = Del->next;
        PrevDel->next = AfterDel;
        end_element = PrevDel; // новый последний элемент
        delete Del;
        count--;
    }
    
    // Получение элемента
    T getElement(int pos)
    {
        Element<T>* temp;
        // Если ближе идти от начала
        if (pos <= count / 2)
        {
            temp = start_element;
            int i = 1;
            while (i < pos)
            {
                temp = temp->next;
                i++;
            }
        }
        // Если ближе идти от конца
        else
        {
            temp = end_element;
            int i = 1;
            while (i <= count - pos)
            {
                temp = temp->prev;
                i++;
            }
        }
        return temp->element;
    }

    // Начало итератора
    Iterator& begin() {
        return *(new Iterator(1, this));
    }

    // Конец итератора
    Iterator& end() {
        return *(new Iterator(-1, this));
    }

};

// Оператор <<
template <typename T>
ostream& operator<<(ostream& out, Element<T> el) {
    out << el.element;
    return out;
}

int randomCharacteristic(int a, int b); // Случайное число

int main()
{
    srand(time(0));
    setlocale(LC_ALL, "Russian"); // Руссификатор
    cout << "Создан пустой список\n";
    List <int> list; // Инициализация списка

    const int N = 10; // Количество элементов
    int array[N];
    int new_element;

    for (int i = 0; i < N; i++) {
        array[i] = randomCharacteristic(0, 10);
        list.addEnd(array[i]); // Добавление элемента в список
    }

    list.print(); // Вывод списка
    list.checkList(); // Проверка на пустоту

    cout << "Длина нового списка: " << list.getCount() << endl; // Длина списка

    cout << "\nВведите число для вставки в начало: ";
    cin >> new_element;
    list.addStart(new_element); // Добавление элемента в начало
    list.print(); // Вывод списка

    cout << "Длина нового списка: " << list.getCount() << endl; // Длина списка

    cout << "\nВведите число для вставки в конец: ";
    cin >> new_element;
    list.addEnd(new_element); // Добавление элемента в конец
    list.print(); // Вывод списка

    cout << "Длина нового списка: " << list.getCount() << endl; // Длина списка

    cout << "\nЭлемент в начале списка: " << list.getStartElement(); // Начальный элемент
    cout << "\nЭлемент в конце списка: " << list.getEndElement() << "\n\n"; // Последний элемент

    list.deleteStart(); // Удаление начального элемента
    list.print(); // Вывод списка
    cout << "Длина нового списка: " << list.getCount() << "\n\n"; // Длина списка

    list.deleteEnd(); // Удаление последнего элемента
    list.print(); // Вывод списка
    cout << "Длина нового списка: " << list.getCount() << endl; // Длина списка

    // Ввывод списка по итератору
    cout << "\nВывод списка через итератор\n";
    for (auto el : list) {
        cout << el << " ";
    }

    return 0;
}

/* Случайное число */
int randomCharacteristic(int a, int b) {
    return a + rand() % (b - a + 1);
}

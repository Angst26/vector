// ConsoleApplication3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
using namespace std;
struct no_default {
    int data;
    no_default(int n) { data = n; }
};

template<typename T, typename A = allocator<T>> class vector
{
    A alloc;
    size_t sz;
    T* elem;
    size_t space;

    void reserve(int newalloc);
  
    
public: 
    // элементы инициализации
    vector() : sz{ 0 }, elem(nullptr), space{0} {} 
   explicit vector (size_t s) 
        :sz{ s }, space (sz),
        elem{ new T[sz] }
    {
        for (size_t i = 0; i < sz; i++)
        {
            elem[i] = 0; 
        }
    }                      
   vector(initializer_list<T> lst)
       :sz{ lst.size() }, elem{ new T[sz] }
   {
       copy(lst.begin(), lst.end(), elem);
   }
 

   //копирующие конструктор и присваивание
   vector(const vector&);
   vector& operator=(const vector&);

   //перемещающиме констр. и присваивание
   vector(vector&&);
   vector& operator=( vector&&);

   // дуструктор 
   ~vector()  {delete[] elem;}

   //доступ
   T& operator[](int index);
   T operator[](int index) const;
 
   int const size() { return sz; }
   size_t capacity() const { return space; };

   //увеличение
    void resize(int newsize, T def = T());
    void display();
    void push_back(T);
};





template<typename T, typename A>
void vector<T, A>::reserve(int newalloc)
{
    if (newalloc <= space) return;
    T* p = alloc.allocate(newalloc);
    // копирование:
    for (int i = 0; i < sz; i++) alloc.construct( &p[i],elem[i]);
    //уничтожение: 
    for (int i = 0; i < sz; ++i) alloc.destroy(&elem[i]);
    alloc.deallocate(elem, space);
    elem = p;
    space = newalloc;
}



template<typename T, typename A>
vector<T, A>::vector(const vector& arg)
    :sz{arg.sz}, elem { new T[arg.sz]}
{
   copy(arg.elem, arg.elem + arg.sz, elem);
}

template<typename T, typename A>
T& vector<T, A>::operator[](int index)
{
    return elem[index];
}

template<typename T, typename A>
T vector<T, A>::operator[](int index) const
{
    return elem[index];
}


////////////////////////////////
template<typename T, typename A>
vector<T, A>& vector<T, A>::operator=(const vector& a)
{
    if (this == &a) return *this;
    if (a.sz <= space) {
        for (size_t i = 0; i < a.sz; i++)
            elem[i] = a.elem[i];
        sz = a.sz;
        return *this;
    }
    T* p = new T[a.sz];
    for (size_t i = 0; i < a.sz; i++)
         p[i] = a.elem[i];
    delete[] elem;
    space = sz = a.sz;
    elem = p;
    return *this;
}

template<typename T, typename A>
vector<T, A>::vector( vector&& a)
    :sz(a.sz), elem{a.elem}
{
    a.sz = 0;
    a.elem = nullptr;
}
template<typename T, typename A>
vector<T, A>& vector<T, A>::operator=( vector&& a)
{
    delete[] elem;
    elem = a.elem;
    sz = a.sz;
    a.elem = nullptr;
    a.sz = 0;
    return *this;
}





template<typename T, typename A>
void vector<T, A>::resize(int newsize, T def)
{
   
    reserve(newsize);
    for (size_t i = sz; i < newsize; i++) alloc.construct(&elem[i], def);
    for (size_t i = newsize; i < sz; i++) alloc.destroy(&elem[i]);
    sz = newsize;
}


template<typename T, typename A>
void vector<T, A>::display()
{
    cout << "\nThe vector is: ";
    for (size_t i = 0; i < sz; i++)
    {
        cout << elem[i];
        if (i < sz -1)
            cout << " - ";
    }
}

template<typename T, typename A>
void vector<T, A>::push_back(T val)
{
    if (space == 0)
        reserve(8);
    else if (sz == space)
        reserve(2 * space);
    alloc.construct(&elem[sz], val);
    ++sz;
}

/////////////////////////////////////////////////////////////////////////   ////////


int main()
{
    vector<float> v{1, 2, 43};
    v.push_back(1);
    cout << v.capacity();
    v.display();
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.



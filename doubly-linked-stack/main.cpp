//14. Контейнер: двунаправленный стек.
//Итераторы: вывода и двунаправленный.
//Сортировка: быстрая сортировка.

#include <iostream>
#include <stdexcept>
#include <string>

template<typename T>
class Stack {
private:
    struct Node {
        T data;
        Node *up = nullptr;
        Node *down = nullptr;
        
        explicit Node(T data): data{std::move(data)} { }
        Node(T data, Node *bottom): data{std::move(data)}, down{bottom} { }
        
        ~Node() {
            if (down != nullptr) {
                delete down;
            }
        }
    };
    
    struct Iter {
    private:
        Node * cur = nullptr;
        int index = 0;
        
    public:
        explicit Iter(Node *n, int index): cur{n}, index{index} { }
        Iter(): cur{nullptr}, index{0} { }
        
        operator T() {
            return *(*this);
        }
        
        operator bool() {
            return cur != nullptr;
        }
        
        T& operator*() {
            if (cur == nullptr) {
                throw std::logic_error("cannot get data from iterator that holding no data");
            }
            return cur->data;
        }
        
        Iter & operator++() {
            if (cur == nullptr) {
                throw std::logic_error("cannot move iterator that holding no data");
            }
            cur = cur->up;
            return *this;
        }
        
        Iter & operator--() {
            if (cur == nullptr) {
                throw std::logic_error("cannot move iterator that holding no data");
            }
            cur = cur->down;
            return *this;
        }
        
        Iter & operator--(int) {
            return --(*this);
        }
        
        Iter & operator++(int) {
            return ++(*this);
        }
        
        Iter operator+(int idx) {
            Iter iter(cur, index);
            for (int i = 0; i < idx; i++) {
                if (!iter.cur)
                    break;
                ++iter;
            }
            return iter;
        }
        
        Iter operator-(int idx) {
            Iter iter(cur, index);
            for (int i = 0; i < idx; i++){
                if (!iter.cur)
                    break;
                --iter;
            }
            return iter;
        }
        
        bool operator==(Iter iter) {
            return cur != nullptr && iter.cur == cur;
        }
        
        bool operator!=(Iter iter) {
            return !(iter == *this);
        }
        
        bool operator==(T t) {
            return cur != nullptr && cur->data == t;
        }
        
        bool operator!=(T t) {
            return !(t == *this);
        }
        
        bool operator<(Iter iter) {
            return cur != nullptr && iter.index < index;
        }
        
        bool operator>(Iter iter) {
            return cur != nullptr && iter.index > index;
        }
        
        friend std::ostream & operator<<(std::ostream & stream, Iter iter) {
            if (iter) {
                stream << *iter;
            } else {
                stream << "<null>";
            }
            return stream;
        }
    };
    
    Node * top = nullptr;
    Node * bottom = nullptr;
    
    int _size = 0;
    
public:
    typedef Iter iterator;
    
    Stack() { }
    ~Stack() {
        if (_size > 0 && top != nullptr) {
            delete top;
        }
    }
    
    void push(T data) {
        if (_size == 0) {
            bottom = top = new Node(data);
        } else {
            Node * tmp = new Node(data, top);
            top->up = tmp;
            top = tmp;
        }
        _size++;
    }
    
    T pop() {
        if (_size == 0 || top == nullptr) {
            throw std::logic_error("stack is empty. pop failed");
        }
        Node * tmp = top;
        top = top->down;
        top->up = nullptr;
        _size--;
        
        if (_size == 0) {
            top = bottom = nullptr;
        }
        
        T data = tmp->data;
        delete tmp;
        return data;
    }
    
    T peek() {
        if (_size == 0 || top == nullptr) {
            throw std::logic_error("stack is empty. peek failed");
        }
        return top->data;
    }
    
    iterator find(T element) {
        if (_size == 0 || top == nullptr) {
            return iterator();
        }
        
        for (iterator i = begin(); i; --i)
            if (i == element)
                return i;
        return iterator();
    }
    
    void sort() {
        quicksort(end(), 0, _size - 1);
    }
    
    iterator begin() {
        return Iter(top, _size - 1);
    }
    
    iterator end() {
        return Iter(bottom, 0);
    }
    
    int size() const {
        return size;
    }
    
private:
    void swap(iterator i, iterator j) {
        T tmp = *j;
        *j = *i;
        *i = tmp;
    }
    
    int parition(iterator base, int low, int high) {
        int i = low - 1;
        T pivot = base + high;
        for (int j = low; j <= high; j++) {
            if (*(base + j)< pivot) {
                i++;
                swap(base + i, base + j);
            }
        }
        swap(base + (i + 1), base + high);
        return i + 1;
    }
    
    void quicksort(iterator base, int low, int high) {
        if (low < high) {
            int pi = parition(base, low, high);
            quicksort(base, low, pi - 1);
            quicksort(base, pi + 1, high);
        }
    }
};

int read_int() {
    while (true) {
        std::string buffer;
        std::getline(std::cin, buffer);
        try {
            return std::stoi(buffer);
        } catch (...) {
            std::cout << "not a number!" << std::endl;
            continue;
        }
    }
}

float read_float() {
    while (true) {
        std::string buffer;
        std::getline(std::cin, buffer);
        try {
            return std::stof(buffer);
        } catch(...) {
            std::cout << "not a float number" << std::endl;
            continue;
        }
    }
}

std::string read_string() {
    while (true) {
        std::string buffer;
        std::getline(std::cin, buffer);
        if (buffer.size() > 0) {
            return buffer;
        }
    }
}

void my_terminate() {
    std::cout << "terminate!" << std::endl;
    exit(1);
}

void my_unexpected() {
    std::cout << "unexpected!" << std::endl;
    std::terminate();
}

int main(int argc, const char * argv[]) {
    std::set_terminate(my_terminate);
    std::set_unexpected(my_unexpected);
    
    std::cout << "enter count for first stack: ";
    int first_count = read_int();
    if (first_count < 0) {
        std::cout << "count cannot be < 0" << std::endl;
        return 1;
    }
    
    std::cout << "enter count for second stack: ";
    int second_count = read_int();
    if (second_count < 0) {
        std::cout << "count cannot be < 0" << std::endl;
    }
    
    Stack<int> first;
    Stack<float> second;
    
    std::cout << "items for first stack:" << std::endl;
    for (int i = 0; i < first_count; i++) {
        std::cout << (i + 1) << "/" << first_count << ":";
        int number = read_int();
        first.push(number);
    }
    
    
    std::cout << "items for second stack:" << std::endl;
    for (int i = 0; i < second_count; i++) {
        std::cout << (i + 1) << "/" << second_count << ":";
        float number = read_float();
        second.push(number);
    }
    
    std::cout << "first stack: " << std::endl;
    for (Stack<int>::iterator i = first.begin(); i; i--) {
        std::cout << " " << *i << std::endl;
    }
    
    std::cout << "second stack: " << std::endl;
    for (Stack<float>::iterator i = second.begin(); i; i--) {
        std::cout << " " << *i << std::endl;
    }
    
    std::cout << "sorting second stack" << std::endl;
    second.sort();
    std::cout << "second stack: " << std::endl;
    for (Stack<float>::iterator i = second.begin(); i; i--) {
        std::cout << " " << *i << std::endl;
    }
    
    std::cout << "enter index to get item from the first stack: ";
    int index = read_int();
    std::cout << "use exception handler? ('y' to use): ";
    std::string choice = read_string();
    if (choice[0] == 'y') {
        try {
            int item = *(first.begin() - index);
            std::cout << index << " item is " << item << std::endl;
        } catch (std::logic_error error) {
            std::cout << "error getting " << index << " item from first stack\n";
            std::cout << error.what() << std::endl;
        }
    } else {
        int item = *(first.begin() - index);
        std::cout << index << " item is " << item << std::endl;
    }
    
    return 0;
}

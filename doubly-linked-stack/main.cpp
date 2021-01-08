//14. Контейнер: двунаправленный стек.
//Итераторы: вывода и двунаправленный.
//Сортировка: быстрая сортировка.

#include <iostream>
#include <stdexcept>
#include <string>

class Show {
    int id;
    std::string name;
    std::string review;
public:
    Show(int id, std::string name) : id{ id }, name{ name }, review{""} {}
    Show(int id, std::string name, std::string review): id{ id }, name{ name }, review{ review } { }
    ~Show() = default;
    
    
    friend std::ostream & operator<<(std::ostream & stream, Show show) {
        stream << "show " << show.name;
        if (!show.review.empty())
            stream << " review: " << show.review << std::endl;
        else stream << std::endl;
        return stream;
    }
    
    bool operator==(Show show) {
        return show.name == name && show.review == review;
    }
    
    bool operator!=(Show show) {
        return !(*this == show);
    }
    
    bool operator>(Show show) {
        return strcmp(show.name.c_str(), name.c_str()) > 0;
    }
    
    bool operator<(Show show) {
        return strcmp(show.name.c_str(), name.c_str()) < 0;
    }
};

template<typename T>
class Stack {
private:
    struct Node {
        T data;
        Node *up = nullptr;
        Node *down = nullptr;
        
        explicit Node(T data): data{std::move(data)} { }
        Node(T data, Node *bottom): data{std::move(data)}, down{bottom} { }
        
        ~Node() = default;
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
    bool use_dublicates = true;
    
public:
    typedef Iter iterator;
    
    Stack() { }
    Stack(bool use_dublicates): use_dublicates{ use_dublicates } { }
    ~Stack() {
        for (int i = 0; i < _size && top != nullptr; i++)
            pop();
    }
    
    void push(T data) {
        if (!use_dublicates) {
            iterator dup = find(data);
            if (dup) return;
        }
        
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
        
        if (top != nullptr) {
            top->up = nullptr;
        }
        
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

std::string read_string() {
    while (true) {
        std::string buffer;
        std::getline(std::cin, buffer);
        if (buffer.size() > 0) {
            return buffer;
        }
    }
}

Show read_show(int id) {
    std::cout << "enter show name: ";
    std::string name = read_string();
    std::cout << "enter show review (empty if none): ";
    std::string review;
    std::getline(std::cin, review);
    return Show(id, name, review);
}

template<typename T>
void print_stack(Stack<T> &stack) {
    std::cout << "stack items: " << std::endl;
    for (Stack<Show>::iterator i = stack.begin(); i; i--) {
        std::cout << " " << i;
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
    
    std::cout << "use dublicates in stack? ('y' to use): ";
    std::string choice = read_string();
    
    Stack<Show> stack(choice[0] == 'y');
    
    std::cout << "enter count for the stack: ";
    int count = 0;
    while (true) {
        count = read_int();
        if (count < 0) {
            std::cout << "count cannot be < 0" << std::endl;
        } else break;
    }
    
    std::cout << "items for the stack:" << std::endl;
    for (int i = 0; i < count; i++) {
        std::cout << "item " << (i + 1) << std::endl;
        Show show = read_show(i);
        stack.push(show);
        std::cout << std::endl;
    }
    
    print_stack(stack);
    
    std::cout << "sorting the stack" << std::endl;
    stack.sort();
    print_stack(stack);
    
    std::cout << "enter index to get item from the stack: ";
    int index = read_int();
    
    std::cout << "use exception handler? ('y' to use): ";
    choice = read_string();
    
    if (choice[0] == 'y') {
        try {
            Show item = *(stack.begin() - index);
            std::cout << index << " item is " << item << std::endl;
        } catch (std::logic_error error) {
            std::cout << "error getting " << index << " item from the stack\n";
            std::cout << error.what() << std::endl;
        }
    } else {
        Show item = *(stack.begin() - index);
        std::cout << index << " item is " << item << std::endl;
    }
    
    std::cout << "enter number of items to remove from the top of the stack: ";
    index = read_int();
    
    if (index >= 0) {
        for (int i = 0; i < index; i++) {
            try {
                stack.pop();
            } catch(std::logic_error) {
                std::cout << "bottom of the stack has been reached" << std::endl;
                break;
            }
        }
        std::cout << "Removed " << index << " items" << std::endl;
    } else {
        std::cout << index << " is invalid number of items" << std::endl;
    }
    
    print_stack(stack);
    
    return 0;
}

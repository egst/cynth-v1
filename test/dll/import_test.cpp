#include <iostream>

namespace A {
    class Bar {
    protected:
        int x;
    public:
        Bar(int);
    };

    class __declspec(dllimport) Foo: public Bar {
    public:
        Foo(int);
        friend int operator+(Foo, Foo);
    };
}

int main() {
    A::Foo foo(2);
    A::Foo bar(1);
    std::cout << bar + foo << '\n';
}
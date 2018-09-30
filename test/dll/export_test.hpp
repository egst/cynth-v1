namespace A {

    class Bar {
    protected:
        int x;
    public:
        Bar(int);
    };

    class __declspec(dllexport) Foo: public Bar {
    public:
        Foo(int);
        friend int operator+(Foo, Foo);
    };

    __declspec(dllexport) int operator+(Foo, Foo);
    
}
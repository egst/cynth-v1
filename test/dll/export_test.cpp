#include "export_test.hpp"

namespace A {

    Bar::Bar(int x): x{x} {}

    Foo::Foo(int x): Bar{x} {}

    int operator+(Foo a, Foo b) { return a.x + b.x; }

}
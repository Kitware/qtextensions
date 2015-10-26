class Foo
{
public:
    Foo() : a{0} {}
    Foo(int a) : a{a} {}
    operator int() { return a; }

protected:
    int a;
};

int foo(Foo a) { return a; }

Foo bar(int a) { return {a}; }

int main()
{
    return foo({0});
}

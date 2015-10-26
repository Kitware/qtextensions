template <typename T> struct Outer
{
    static int value() { return T::value(); }
};

template <int Value> struct Inner
{
    static int value() { return Value; }
};

int main()
{
    return Outer<Inner<0>>::value();
}

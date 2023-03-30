# TEST {#mainpage}

## TOC

1. [other](./actions.md)


```cpp
auto test = mousetrap::Action("test");
test.set_function([](mousetrap::FileDescriptor& test){
    std::cout << "out" << std::endl;
})
```
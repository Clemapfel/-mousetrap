# TEST {#mainpage}

## TOC

1. [other](./actions.md)


```cpp
// test comment
auto test = mousetrap::Action("test");
test.set_function([](mousetrap::FileDescriptor& test){
    std::cout << "out" << std::endl;
})
```

# Manual: Test
# Manual: Actions
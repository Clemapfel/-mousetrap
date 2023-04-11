# Chapter 2: Actions

This chapter we will learn:
+ How to encapsulate functionality using the Command pattern
+ How to define `mousetrap::Action`s behavior
+ The difference between a stateless and stateful action
+ How to trigger actions using `MenuBar` and `Button`

## Introduction: Command Pattern

## Specifying Action Behavior

The most trivial use of an action is as that of a functor, a callable object. We initiaize an action by giving it a 
name, then use `set_function` to set its behavior. In C++, `set_function` can take both a C-function or a C++ lambda:


\julia_code_begin
```julia
function() begin end
```
\julia_code_end

test

\cpp_code_begin
```cpp
template<typename T>
void test();
```
\cpp_code_ned

## Application

`Action`s are stored globally on an application level. `mousetrap::Application` acts as an action map, an object 
that keeps track of actions and allows accessing them.

## Action

### Stateless Actions

### Stateful Actions

## Menus

## Keybindings
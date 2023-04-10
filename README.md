# mousetrap

# refacotr:
+ FileMonitor
+ shape, texture, rendertexture, render_area, render_task, shader
+ music, sound, soundbuffer
> > This repo is under rapid active development and should not be interacted with by the public. Licensed CC-BY-SA-NC

GTK4 is well optimized, incredibly powerful and utterly inscrutable. It has hundreds of thousands of lines of code and with functionality split between GTK, GDK, Glib and more,
learning the library tsake weeks, meaning many user will feel overwhelmed and never try at all. Mousetrap aims to address this, by
wrapping GTK4 in a way that is much easier to understand and use, allowing beginners to start developing applications within minutes instead of days.

## Advantages of mousetrap over GTK4
+ automatic memory management, `new` is unnecessary to be called
+ complex widgets like `ColumnView` or `TreeView` can be initialized with exactly one line
+ Adds Image- and Sound-processing utilities, including natively playing sounds at runtime
+ Adds high-level, fully abstracted OpenGL functionality, such as native 2d shapes, textures, render textures, shaders, blend-modes, transforms and more
+ Input of funtions are sanity checked and verbose warnings are printed before something breaks, this speeds up 
  debugging and prevents potential bugs
+ Documentation is in one single place, tutorial introduces readers gently step-by-step

## Disadvantages over GTK4
+ many of the gio and glib functionality is not yet exposed, file I/O and anything related to images is present in 
  mousetrap, however
+ GTK Builder is ommitted completely, all objects are defined and initializes in a C++ file
+ a hand full of widgets are not yet exposed, or where intentionally ommitted, the latter category includes `FlowBox`,
  `ScaleButton`, and any widget marked for deprecation since GTK4.10

The main disadvantage of mousetrap is that not every GTK4 functionality is imported, making mousetrap somewhat less flexible. However, **every non-deprecated GTK4-widget is available**, 
meaning there is no limiation put upon the actual graphical user interface.

# TODO
+ Search Bar
+ FontDialog
+ Allow multiple tick callbacks for widget
+ Make ListView, GridView, ColumnView filterable
+ Add DropDown search bar
+ Expose TextView widget insertion
+ Refactor ColumnView to be a tree
+ Scale: pango layout interface to allow markdown during text format function
+ Expose GTK_DEBUG interface
+ File: file system, monitor, measure disk size, asynchronous read/write 
+ Action: allow changing shortcut triggers during runtime
+ Grid: Baseline Positions
+ check button groups
+ clipboard: allow arbitrary datatypes
+ Sound: Mutate SoundBuffer, allow Music stream from memory
+ Expose GResource interface
+ Log: allow stream to file
+ FileSystem: allow async operations

# Design Goals

## Widgets can only contain Widgets

If a widget is a container, only other widgets can be added to it. For example, we cannot add a string to a `ListView`, 
instead, we have to first construct a label for that string, then add that label instead:

```cpp
auto text = "text";
auto label = Label(text);

auto list = ListView(Orientation::HORIZONTAL);
list.push_back(&label);
```

This fully abstracts away the [factory-pattern](https://docs.gtk.org/gtk4/class.ListItemFactory.html) and 
[model-view-pattern](https://docs.gtk.org/gtk4/method.ListView.set_model.html) used by many of the more complex GTK 
widgets such as `ListView`, `GridView` and `ColumnView`, all of which can be initialized in exactly one line in mousetrap.

While vastly simplfying the interface, it allows for the same flexiblity, as, similar to a `Box`, any type of object can 
be added to the container. Adding an `ImageDisplay` to a `GridView` is the same 1-line process as adding another `GridView` 
to a `GridView`.

Mutating widgets that area already inserted is also made easier this way. If we create a `ListView` containing
10 `Label`s, we will first have to create the 10 labels ourself. By keeping a reference to them, if we want to change 
the text displayed by the `ListView`, we can simply call `set_text` on each label. Because the `ListView` does not 
construct the string-displaying widget for us, we keep full control over them.

## Lambdas instead of static C functions

One of C++s most convenient feature are lambdas. In pure GTK4, we are unable to for example connect a runtime-created
lambda to a signal directly. Through employing clever wrapping, in mousetrap connecting to a signal handler is possible
using both lambdas and regular C functions:

```cpp
auto window = Window();
window.connect_signal_close_request([]() -> bool {
    std::cout << "window closed" << std::endl;
    return false; // do not prevent window from closing
});
```

Through code generation, most of GTK4s macros were wrapped with a more expressive, explicitly defined function. Compare:

```cpp
window.connect_signal("close-request", // ...
window.connect_signal_close_request( // ...
```

The latter has the advantage of a) not allowing user to use an incorrect or misspelled signal id and b) asserts that 
the function connected to the signal handler is of the correct signature. In GTK4, due to the way C function pointers work,
a function of any signature can be connected to any signal, which allows for data corruption and possible side-effects,
both of which were eliminated with the explicit signal notation above.

## OpenGL instead of Cairo

Gtk4 now supports native OpenGL, removing the performance penatly it used to incur by having to move the OpenGL 
framebuffer to display it using cairo. mousetrap offers `RenderArea` which abstracts away any manual OpenGL tasks such
as initializing the state or keeping track of GPU-side memory. Similar to a library like [SFML](https://www.sfml-dev.org/),
mousetrap offers easy to initialize and manipulate objects for shapes, shaders, transforms and blendmodes. Rendering a 
triangle in mousetrap only takes a few lines:

```cpp

auto triangle = Shape();
shape.as_triangle({0.25, 0.25}, {0.75, 0.75})

auto render_area = RenderArea();
TODO
```

## There is only one library

While extremely powerful, the way GTK4 is split between GTK, GDK and Gio / Glib makes it hard to form a cohesive picture
of the library as sometimes, a single task such as constructing a list displaying images requires calls from all three
different libraries who are documented on three different internet domains. mousetrap aims to unify both documentation 
and code usage, keeping everything in one place by abstracting away any gdk or glib functionality.

# Hello World

```cpp
#include <include/mousetrap.hpp>

using namespace mousetrap;

// global state, holds persistent widgets for the duration of runtime
inline struct State
{
    Window window;
    Label hello_world_label = Label("hello world");
    
}* state;

int main()
{
    auto app = Application("app.hello_world");

    app.connect_signal_activate([&](Application*)
    {
        // initialize
        state = new State{Window(app)};
        state->window.set_child(&state->hello_world_label);
        state->window.present();
    });

    app.connect_signal_shutdown([](Application*)
    {
        // shut down
        delete state;
    });

    return app.run(); // main render loop
}
```

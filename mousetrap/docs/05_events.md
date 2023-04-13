# Chapter 5: Event Handling

In this chapter, we will learn:
+ How to handle user interaction, such as cursor movement, keyboard presses, or even touchpad stylus movements
+ What an event controller is
+ How to connect a controller to any widget

## Event Model

So far, we were able to react to a user interacting with the GUI through widgets. For example, if the user pressed the left mouse button while hovering over a `mousetrap::Button`, that button will emit the signal `clicked`. While powerful, this is also failry limiting. Widgets have a built-in way of handling user interactions, and we rely on a widget to forward this to our custom signal handlers. What if we want to trigger a button if the user hovers over it as opposed to clicking it? So far, this was impossible, however mousetrap offers a solution for this: event controllers.

### What is an Event?

When the user interacts with a computer in the physical world, they will control some kind of device, for example a mouse, keyboard, touchpad, or maybe even a webcam. This device will send data to the operating system, which processes the data into what is called an event. Pressing a keyboard key is an event, releasing the key is a new event. Moving the cursor by one unit is an event, pressing a stylus against a touchpad is an event, etc. Mousetrap is based on GTK4, which has a very powerful and versatile event abstraction. We don't have to deal with OS-specific events directly, instead it will automatically transform and distribute events for us.

To receive such an event, we need a `mousetrap::EventController`. That specific class is an interface, we cannot instantiate it directly, instead we have to instantiate any of its children. Each child class handles one or more types of events. Each type of event emits it's own signal, for example the event controller handling keyboard events will emit signal `key_pressed` if the user pressed a keyboard key, `key_released` if the user releases the keyboard key, etc. By connecting to these signals, we can trigger any kind of behavior we want, which empowers us to make our own custom widgets.

## Input Focus

Important to avoiding confusion is to understand the concept of [input focus](https://en.wikipedia.org/wiki/Focus_(computing)). In mousetrap and GUIs in general, each widget has a hidden property that indicates whether the widget currently has *focus*. Only exactly one widget can hold focus at a time. If a widget has focus, all it's children hold focus as well. For example, if the focused widget is a box, all widgets inside that focus hold focus. If the focus is just a button, no other widget except that button can hold focus.

Only a widget holding focus can receive input events. Which widget gets focus next is controlled by a somewhat complex heuristic. We do have direct control over this, however:

### Preventing Focus

Only `focusable` widgets can hold focus. We can make a widget focusable by calling `mousetrap::set_is_focusable`. Not all widgets are focusable by default, however any widget that has an intuitive way of interacting with it (such as a `Button`, `Entry`, `Scale`, `SpinButton` etc.) will be focusable by default. 

Widgets that cannot have focusable children and do not usually require user interaction (such as `Label` and `ImageDisplay`) will be unable to grab focus by default.

By setting `mousetrap::Widget::set_can_respond_to_focus` to false, a widget and all of its children are unable to receive focus under any circumstances. Most graphical backends indicate this to the user by making the widget slightly opaqued, it will appear "greyed-out". This should be reserved to temporarily disabling a widget, otherwise `set_is_focusable` is recommended.

### Requesting Focus

`mousetrap::Widget::grab_focus` will make a widget attempt to receive the current focus. If this is impossible, for example because the widget is greyed-out, nothing wil happen. We can check if a widget currently has focus by calling `mousetrap::Widget::get_has_focus`.

Many widgets will automatically grab focus if interacted with, for example if the user places the text cursor inside an `Entry`, that entry will grab focus. If a button is clicked, it will usually grab focus. We can make any widget, even those that do not usually require interaction, grab focus when clicked by setting `mousetrap::Widget::set_focus_on_click` to `true`.

## Connecting a Controller

We will now use our newly gained knowledge about input focus to create our very first event controller: `FocusEventController`. This controller reacts to a widget gainined or loosing input focus. This makes it easy to monitor, as we do not need to ask each widget manually via `get_has_focus`.

After instantiating it, it will not react to any events yet. We need to *connect* it to a widget. For this chapter, we will assume this widget is the toplevel window `state->window`. Because all widgets are contained in a window, this will make the controller connected to it react to all events targeted at any widget (that can have focus). 

We create and connect a `FocusEventController` like so:

\cpp_code_begin
```cpp
auto focus_controller = FocusEventController();
state->window.add_controller(focus_controller);
```
\cpp_code_end

\julia_code_begin
```julia
# TODO
```
\julia_code_end

## Gaining / Loosing Focus

While the controller is now able to receive events, nothing will happen as a result of them. This is because we haven't connected to the controllers signals yet.

`FocusEventController` has two signals:
+ `focus_gained`, signature `(FocusEventController*) -> void`
+ `focus_lost`, signaure `(FocusEventController*) -> void`
+ 
## Action Shortcuts
## Keyboard Key Presses

## Cursor Motion
## Mouse Button Presses
## Long-Presses
## Click-Dragging
## Scrolling

## Touchscreen: Panning
## Touchscreen: Pinch-Zoom
## Touchscreen: 2-Finger Rotate
## Touchscreen: Swipe

## Touchpad Stylus



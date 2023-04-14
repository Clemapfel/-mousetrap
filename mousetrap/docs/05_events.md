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

## Gaining / Loosing Focus: FocusEventController

While the controller is now able to receive events, nothing will happen as a result of them. This is because we haven't connected to the controllers signals yet.

### Signals

`FocusEventController` has two signals:

| id             | signature                                   | emitted when...                                         |
|----------------|---------------------------------------------|---------------------------------------------------------|
| `focus_gained` | `(FocusEventController*, (Data_t)) -> void` | widget which currently does not have focus, gains focus |
| `focus_lost`   | `(FocusEventController*, (Data_t)) -> void` | widget which currently does have focus, looses focus    |

After connecting to these signals:

\cpp_code_begin
```cpp
auto focus_controller = FocusEventController();

focus_controller.connect_signal_focus_gained([](FocusEventController*){
    std::cout << "focus gained" << std::endl;
});

focus_controller.connect_signal_focus_lost([](FocusEventController*) {
    std::cout << "focus lost" << std::endl;
});

state->window.add_controller(focus_controller);
```
\cpp_code_end

\julia_code_begin
```julia
# TODO
```
\julia_code_end

we are able to easily monitor when a widget gains/looses input focus.

---

## Keyboard Keys: KeyEventController

To react to keyboard pressed, we use `mousetrap::KeyEventController`. This is one of the most commonly used controllers, so it is pertinent to talk about keys first.


### Modifiers

Mousetrap is based on GTK4, which splites keys into two groups: regular keys and *modifiers*.

A modifier is one of the following:
+ `Shift`
+ `Control`
+ `Alt`

These keys are not tracked directly, rather they are modifiers to the state of a key. For example, the key `C` can be pressed, not pressed, pressed while only shift is held, pressed while shift and alt are held, etc. To express all these options, mousetrap provides `mousetrap::ModifierState`, which holds information about which modifier keys are pressed.

To query the modifier state, we use static function of `KeyEventController`. A static function is a function that can be invoked without instantiating the class, meaning we do not need a `KeyEventController` to call them. The functions

+ \link mousetrap::KeyEventController::shift_pressed shift_pressed <br>
+ \link mousetrap::KeyEventController::control_pressed control_pressed <br>
+ \link mousetrap::KeyEventController::alt_pressed alt_pressed <br>

query whether the corresponding modifier key is currently down.

### Key Identification

Mousetrap uses two kinds of identifiers for keys, `KeyValue`s and `KeyCode`s. For most purposes, only the first is relevant for most users. `KeyCode` is a 32-bit int
designating the internal identifier of the key, as the users OS designates it. `KeyValue` identifies the key in an platform-independent manner, which is why testing against it should be preferred.

GTK4 provides key identifiers for almost every keyboard layout, a list of identifiers can be found [here](https://gitlab.gnome.org/GNOME/gtk/-/blob/main/gdk/gdkkeysyms.h#L38).

### Signals

Now that we know how to identify a code, we can look at the 3 signals of `KeyEventController:

| id                                                                                                                                               | signature                                                                    | emitted when...                                      |
|--------------------------------------------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------|------------------------------------------------------|
| `key_pressed`                                                                                   `(T*, KeyValue, KeyCode, ModifierState) -> bool` | user presses a key that is currently up                                      |                                                      |
| `key_released`                                                                                                                                   | `(T*, KeyValue, KeyCode, ModifierState) -> void`                             | user releases a key that is currently down           | 
 | `modifiers_changed`                                                                                                                              | `(T*, KeyValue key_value, KeyCode key_code, ModifierState modifier) -> bool` | a modifier key is pressed or released\cpp_code_begin |
               
Pressing and releasing a regular key are separate events, they can be used to keep track of whether a key is currently down. For each signal, we get access to the keyvalue, the less relevant key code and the modifier state, which we can query using the `KeyEventController` instance. For example, to test whether the user presses the space key while `state->window` holds input focus:
       
\cpp_code_begin
```cpp
auto key_controller = KeyEventController();
key_controller.connect_signal_key_pressed([](KeyEventController* instance, int32_t key_value, int32_t _, ModifierState state){
    if (key_value == GDK_KEY_space)
        std::cout << "space pressed" << std::endl;
});
state->window.add_controller(key_controller);
```
\cpp_code_end

\julia_code_begin
```julia
# TODO
```
\julia_code_end
                   
Where we choose `_` as the variable name for the key code to signifiy that it will go unused.

Not that `KeyEventController` should not be used to detect whether the user pressed a common key binding, for example `<Control>c`, mousetrap provides a special 
event controller called `ShortcutEventController` for this, which we will learn about shortly. 
`KeyEventController` is intended to monitor both key presses and key releases of individual keys, not to trigger actions based on common keybinding shortcuts.

---

## Cursor Motion: MotionEventController
                                                                 
Now that we know how to handle keyboard events, we will turn our attention to mouse-based events. There are two types 
of events a mouse can emit, *cursor motion*  and *mouse button presses*. These are handled in different controllers, we 
will deal with cursor motion first.

The corresponding event controller `MotionEventController` has 3 signals:

| id             | signature                                                             | emitted when...                                              |
|----------------|-----------------------------------------------------------------------|--------------------------------------------------------------|
| `motion_enter` | `(MotionEventController*, double x, double y, (Data_t) -> void`       | Cursor enters the allocated space of the controlled widget   |
 | `motion` | `T*, double x, double y) -> void`                                     | Cursor moves inside allocated space of the controlled widget |
| `motion_leave` | `(MotionEventController*, (Data_t)) -> void`                          | Cursor leaves allocated space ot he controlled widget        |

`motion_enter` and `motion` supply the signal handler with 2 addition arguments, `x` and `y`. These are the absolute position of the cursor, in pixels.
For example, if the widget is a `Box` and it has allocated 500x200 pixels on screen, when the cursor is exactly at the center of the box, `x` will have a value of `250`, `y` will have a value of `100`.

Note that `motion` is only emitted when the cursor already entered the controlled widget. We cannot track a cursor that is outside the widget. To track all cursor movement, we would have to connect the controll to a window and make that window cover the entire desktop screen.

To track cursor position as the cursor moves over a window, we can do the following:

\cpp_code_begin
```cpp
auto motion_controller = MotionEventController();
motion_controller.connect_signal_motion([](MotionEventController*, double x, double y){
   std::cout << "Cursor Position: " << x << " " << y << std::endl;  
});
window.add_controller(motion_controller);
```
\cpp_code_end

\julia_code_begin
```julia
# TODO
```
\julia_code_end

---

## Mouse Button Presses: ClickEventController

With cursor movement taken care of, we now turn our attention to handling the other type of mouse event: button presses.

### Signals

The corresponding event controller is called `ClickEventController` which has 3 signals:

| id             | signature | emitted when...                                     |
|----------------|-----------|-----------------------------------------------------|
 | `click_pressed` | `(ClickEventController*, int32_t n_presses, double x, double y, (Data_t)) -> void` | any mouse button that is current up is pressed      |
| `click_released` | `(ClickEventController*, int32_t n_presses, double x, double y, (Data_t)) -> void` | any mouse button that is currently down is released |
| `click_stopped` | `(ClickEventController*, (Data_t)) -> void` | a sequence of multiple clicks stops                 |

Much like with `MotionEventController`, the signals provide the handler with `x` and `y`, the absolute position of the cursor when they click happend, in local widget-space. The first argument for two of the signals, `click_pressed` and `click_released`, `n_presses` is the number of clicks in the current sequence. For example `n_presses = 2` in a `click_pressed` means that this is the second time the mouse button was pressed in sequence. 

`click_stopped` signals that a sequence of clicks has stopped. It may be helpful to consider an example:

Let's say the user clicks the mouse button 2 times total, then stops clicking. This will emit the following events in this order:
1) `click_pressed` (n = 1)
2) `click_released` (n = 1)
3) `click_pressed` (n = 2)
4) `click_released` (n = 2)
5) `click_stopped`

This allows us to easily handle double-clicks without any external function keeping track of them. The delay after which a click sequence stops is system-dependent and usually decided by the window manager, not mousetrap.

### Differentiating Mouse Buttons

`ClickEventController` is one of a few event controllers that inherit from `mousetrap::SingleClickGesture`. This interface provides functionality that lets us distinguish between 
different mouse buttons. mousetrap supports up to 9 different mouse buttons, identified by `mousetrap::ButtonID`:

+ ButtonID::BUTTON_01 is usually the left mouse button or a touchpad click
+ ButtonID::BUTTON_02 is usually the right mouse button
+ ButtonID::ANY is used as a catch-all for all possible mouse buttons
+ ButtonID::BUTTON_03 - BUTTON_09 are subsequent hardware-specific buttons
+ ButtonID::NONE represents none of the above

To check which mouse button was pressed when a signal of `ClickEventController` was emitted, we use `get_current_button`, which returns an id above.
If we only want signals to emitted for certain buttons, we can use `set_only_list_to_button` to restrict the choice of button to one specific button,
or we can call `set_touch_only` to only listen to touch-event

For example, if we want to activate an action when a widget `some_widget` (that is usually not clickable), is clicked twice with the left mouse button, we can do the following:

\cpp_code_begin
```cpp
auto click_controller = ClickEventController();
click_controller.connect_signal_click_pressed([](ClickEventController* controller, int32_t n_presses, double x, doubel y){
    if (n_pressed == 2 and controller->get_current_button() == ButtonID::BUTTON_01)
        std::cout << "double click registered at: " << x << " " << y << std::endl;
});
some_widget.add_controller(click_controller);
```
\cpp_code_end

\julia_code_begin
```julia
# TODO
```
\julia_code_end

`ClickEventController` gives us full control over one or multiple mouse- / touchscreen clicks. There is a more specialized
controller dealing with mouse input, thought.

---

## Long-Presses: LongPressEventController

`LongPressEventController` reacts to a specific used input of pressing a mouse button or touchpad, keeping it at a spefici position and not releasing the
button for a long time. This is called a long press, and it is sometimes used to trigger additional behavior not related
to individual "short" clicks. Not that it is necessary for the cursor to stay in the same position for the entire durtion, if it moves, it is registered as a "pan" gesture, which we will talk about later.

`LongPressEventController` has 2 signals:

| id             | signature                                                          | emitted when...                                                                |
|----------------|--------------------------------------------------------------------|--------------------------------------------------------------------------------|
| `pressed` | `(LongPressEventController*, double x, double y, (Data_t) -> void` | emitted when button is held for long enough duration to register as long press |
| `press_cancelled` | `(LongPressEventController*, (Data_t)) -> void`  | emitted when button is released or moved too much before `pressed` was emitted |                   

Similar to `clicked`, `LongPressEventController` provides use with the location of the cursor.

We can modify how long the user has to hold the button for to register as a long-press by 
multiplying the default amount of time with a factor, which is the argument to `mousetrap::LongPressEventController::set_delay_factor`. Being a factor, a value of `1` means no change, a value of `2` means it will take twice as long and a value of `0.5` means it will take half as long.

`LongPressEventController`, like `ClickEventController`, inherits from `SingleClickGesture`, which allows us to differentiate between different mouse button or touchscreens just like before.

---

## Mousewheel-Scrolling: ScrollEventController

We know how to handle mouse buttons, so we now turn our attention to the mouses scroll wheel. This is usually a designated button on the physical mouse, but maybe also be triggered by the OS through a touchpad or touchscreen gesture, such as the "two finger scroll" common for most tablets.

### Signals

No matter how the scroll was inputted, `ScrollEventController` allows us to react to it with its 3+1 signals:

| id             | signature                                  | emitted when...                                         |
|----------------|--------------------------------------------|---------------------------------------------------------|
| `scroll_begin` | `(ScrollEventController*, (Data_t)) -> void` | scroll starts |
| `scroll` | `(ScrollEventController*, double delta_x, double delta_y, (Data_t)) -> void` | once each frame while the scroll is happening |
| `scroll_end` | `(ScrollEventController*, (Data_t)) -> void` | scroll end |

These three signals are fairly straighforward, when the user starts scrolling `scroll_begin` is emitted. Then, as the user keeps scrolling, `scroll` is emitted every tick to update us on the position of the scroll wheel. Signal `scroll` provides the signal handler with `delta_x` and `delta_y` which is the difference between the current position of the scroll wheel and the position at the start of the scroll (when `scroll_begin` was emitted). This difference is in 2D space, as some mice and most touchscreen allow scrolling in two dimensions. Once the user stops scrolling `scroll_end` is emitted once.

### Kinetic Scrolling

`ScrollEventController` has a fourth signal which reacts to *kinetic scrolling*. Kinetic scrolling is a feature of touchscreens
where the user can quickly scroll on the screen to make the scrolled widget behave as if it had inertia. The scroll distance of the widget is more than the distance the users actual fingers moved. Not all devices support this kind of scrolling, but if they do most of the event sequence will be the same as with regular scroll: `scroll_begin` once, then `scroll` many times, then `scroll_end` once the users finger leaves the touchscreen. Afterwards, while the widget is still scrolling because of intertia, the following signal is emitted:

| id                          | signature                                                                          | emitted when...                                              |
|-----------------------------|------------------------------------------------------------------------------------|--------------------------------------------------------------|
| `kinetic_scroll_decelerate` | `(ScrollEventController*, double x_velocity, double y_velocity, (Data_t)) -> void` | scroll ends but inertia should continue scrolling the widget |

`x_velocity` and `y_velocity` are the read-only current conceptual speed of the scroll. They automatically decay and the "friction" (the speed at which it decelerates) is device-dependent. Either way, for devices/widgets who support kinetic scrolling, we should connect to this signal to monitor the speed of scrolling as it slows down.

`mousetrap::ScrolledWindow`, from the previous chapter, has a default handler that uses kinetic scrolling for a smoother user
experience, for a widget like this connecting to `kinetic_scroll_decelerate` is highly adviced, regardless of the users hardware.

---

## Click-Dragging: DragEventController

| id             | signature | emitted when...                                         |
|----------------|-----------|---------------------------------------------------------|

\cpp_code_begin
```cpp
```
\cpp_code_end

\julia_code_begin
```julia
# TODO
```
\julia_code_end

---

## Action Shortcuts: ShortcutController


| id             | signature | emitted when...                                         |
|----------------|-----------|---------------------------------------------------------|

\cpp_code_begin
```cpp
```
\cpp_code_end

\julia_code_begin
```julia
# TODO
```
\julia_code_end

---

## Touchscreen Panning: PanEventController


| id             | signature | emitted when...                                         |
|----------------|-----------|---------------------------------------------------------|

\cpp_code_begin
```cpp
```
\cpp_code_end

\julia_code_begin
```julia
# TODO
```
\julia_code_end

---

## Touchscreen Pinch-Zoom: PinchZoomEventController


| id             | signature | emitted when...                                         |
|----------------|-----------|---------------------------------------------------------|

\cpp_code_begin
```cpp
```
\cpp_code_end

\julia_code_begin
```julia
# TODO
```
\julia_code_end

---

## Touchscreen 2-Finger Rotate: RotateEventController


| id             | signature | emitted when...                                         |
|----------------|-----------|---------------------------------------------------------|

\cpp_code_begin
```cpp
```
\cpp_code_end

\julia_code_begin
```julia
# TODO
```
\julia_code_end

---

## Touchscreen Swipe: SwipeEventController


| id             | signature | emitted when...                                         |
|----------------|-----------|---------------------------------------------------------|

\cpp_code_begin
```cpp
```
\cpp_code_end

\julia_code_begin
```julia
# TODO
```
\julia_code_end

---

## Touchpad Stylus: StylusEventController


| id             | signature | emitted when...                                         |
|----------------|-----------|---------------------------------------------------------|

\cpp_code_begin
```cpp
```
\cpp_code_end

\julia_code_begin
```julia
# TODO
```
\julia_code_end

---


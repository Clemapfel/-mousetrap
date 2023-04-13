# Chapter 3: Signals

In this chapter we will learn:
@todo

## Introduction: Signal Architecture

Central to mousetrap, GTK and many GUI libraries like QT is what is called signal architecture or [signal programming](https://en.wikipedia.org/wiki/Signal_programming).

A signal has 3 components:
+ an **id**, which uniquely identifies it. Ids may not be shared between signals
+ an **emitter**, which is a non-signal object
+ a **callback**, which is a function called when an emitter emits a signal

It may be easiest to consider an example: In the previous chapter on actions, we have learned of `mousetrap::Button` which triggers an action when clicked. However, this is not the only way of connecting behavior to the click of a button. Instead, we can connect to a signal emitted when the button is clicked.

Looking at `mousetrap::Button`s documentation, we see that it inherits from a class called `mousetrap::has_signal_clicked`. like the name suggest, inheriting from this class makes a signal named `clicked` available to the child, `Button` in this case. An object can have any number of signals, as long as they do not share an id. Button has more than just the `clicked` signal, however we will reserve these for later chapters.

Inheriting from `has_signal_clicked` makes the following functions available to a `Button` instance:

+ `Button::connect_signal_clicked`
+ `Button::disconnect_signal_clicked`
+ `Button::emit_signal_clicked`
+ `Button::set_signal_clicked_blocked`
+ `Button::get_signal_clicked_blocked`

We will go through these one-by-one, as every signal will have exactly these functions, with `clicked` replaced by the corresponding signals name, of course.



## Connecting a Signal Callback


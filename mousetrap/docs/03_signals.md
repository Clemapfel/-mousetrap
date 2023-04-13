# Chapter 3: Signals

In this chapter we will learn:
@todo

## Introduction: Signal Architecture

Central to mousetrap, GTK and many GUI libraries like QT is what is called signal architecture or [signal programming](https://en.wikipedia.org/wiki/Signal_programming).

A signal has 3 components:
+ an id, which uniquely identifies it. Ids may not be shared between signals
+ an emitter, which is a non-signal object
+ a callback, which is called when an emitter emits a signal

It may be easiest to consider an example. In the previous chapter on actions, we have learned of `mousetrap::Button` which triggers an action when clicked. This is not the only way for the click of a button to trigger behavior.

\todo link

Looking at `Button`s documentation, we see that it inherits from a class called `has_signal_clicked`. Much like the name suggest, this means Button has a signal with id `clicked`. `has_signal_clicked` is an interface of type `SignalComponent`. Inheriting from it injects functionality into its children, it cannot be instatiated by itself.
An object may have any number of signal components, as long as the do not share a name, including no signal component at all. 

`Button` inherits the following functions from mousetrap::has_signal_clicked:

+



## Signal Emission & Handling

### Connecting to Signals

### Blocking Signal Emission


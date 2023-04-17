# Chapter 4: Widgets

Widgets are central to any GUI application. Usually, a widget is 

## mousetrap::Widget

All widgets inherit from `mousetrap::Widget`. When creating a custom widget, 

#### Signals

| id          | signature               | emitted when...                                               |
|-------------|-------------------------|---------------------------------------------------------------|
| `realize`   | `(T*, (Data_t) -> void` | widget itself and all its parents are realized                |               
| `unrealize` | `(T*, (Data_t) -> void` | the display surface associated with the widget is deallocated |
| `hide`      | `(T*, (Data_t) -> void` | widget that is currently visible is                           |
| `show`      | `(T*, (Data_t) -> void` | widget that is currently hidden is shown                      |                    
| `map`       | `(T*, (Data_t) -> void` | once the widget iself and all its parents become visible      |
| `unmap`     | `(T*, (Data_t) -> void` | widget itself or any of its parents are hidden                |  
| `destroy`   | `(T*, (Data_t) -> void` | all references to a widget are released                       |

Where `T` is the fully specialized widget class, not `Widget*`. For example, for `Separator`, the signature of `realize` would be `(Separator*, (Data_t)) -> void`.

### Widget Properties

#### Margin

#### Size Request

#### Expansion

#### Alignment

#### Visibility

#### Cursor

#### Tooltip

---

## Common Widget Containers

### Window

#### Signals

| id                        | signature                                         | emitted when...                                                                             |
|---------------------------|---------------------------------------------------|---------------------------------------------------------------------------------------------|
 | `close_request`           | `(Window*, (Data_t)) -> WindowCloseRequestResult` | window manager requests the window to close, for example by the user pressing the "x" button |
| `activate_default_widget` | `(Window*, (Data_t)) -> void`                     | default widget is activate (see below)                                                      |
| `activate_focused_widget` | `(Window*, (Data_t)) -> void`                      | `activate` emitted on currently focused widget                                              |

\todo default widget

### Box

### Overlay

### Frame

### AspectFrame

### ScrolledWindow

#### Signals

| id             | signature                                                                  | emitted when...                                       |
|----------------|----------------------------------------------------------------------------|-------------------------------------------------------|
| `scroll_child` | `(ScrolledWindow*, ScrollType type, bool is_horizontal, (Data_t)) -> void` | user requests scroll action through with a keybinding |

### Paned

---

## Display Widgets

### Separator

### Label

### ImageDisplay

### LevelBar

### Progressbar & Spinner 

## Interactive Widgets

### Button


#### Signals

| id        | signature                     | emitted when...               |
|-----------|-------------------------------|-------------------------------|
| `activate` | `(Button*, (Data_t)) -> void` | widget is activated, for example by clicking the button, pressing enter while it holds focus, or by calling `activate()` |
| `clicked` | `(Button*, (Data_t)) -> void` | button is clicked by the user |

### ToggleButton

#### Signals

| id        | signature                           | emitted when...               |
|-----------|-------------------------------------|-------------------------------|
| `activate` | `(ToggleButton*, (Data_t)) -> void` | widget is activated, for example by clicking the button, pressing enter while it holds focus, or by calling `activate()` |
| `clicked` | `(ToggleButton*, (Data_t)) -> void` | button is clicked by the user |
| `toggled` | `(ToggleButton*, (Data_t)) -> void` | buttons state changes |                                   
### CheckButton

#### Signals

| id        | signature                          | emitted when...               |
|-----------|------------------------------------|-------------------------------|
| `activate` | `(CheckButton*, (Data_t)) -> void` | widget is activated, for example by clicking the button, pressing enter while it holds focus, or by calling `activate()` |
| `toggled` | `(CheckButton*, (Data_t)) -> void` | buttons state changes | 

### Switch

#### Signals

| id        | signature                     | emitted when...               |
|-----------|-------------------------------|-------------------------------|
| `activate` | `(Switch*, (Data_t)) -> void` | widget is activated, for example by clicking the button, pressing enter while it holds focus, or by calling `activate()` |

### Adjustments

#### Signals

| id        | signature                     | emitted when...               |
|-----------|-------------------------------|-------------------------------|
| `value_changed` | `(Adjustment*, (Data_t)) -> void` | `value` property changes |
| `properties_changed` | `(Adjustment*, (Data_t)) -> void` | property that isn't `value` changes |

### SpinButton

#### Signals

| id        | signature                         | emitted when...                                            |
|-----------|-----------------------------------|------------------------------------------------------------|
| `value_changed` | `(SpinButton*, (Data_t)) -> void` | `value` property changes                                   |
| `wrapped` | `(SpinButton*, (Data_t)) -> void` | `set_should_wrap` is set to true and value under/overflows |

### Scale

#### Signals

| id        | signature                    | emitted when...                                            |
|-----------|------------------------------|------------------------------------------------------------|
| `value_changed` | `(Scale*, (Data_t)) -> void` | `value` property changes          |              

### ScrollBar

#### Signals

`ScrollBar` does not have any signals, but we can connect to the signals of the  `Adjustment` controlling the scrollbar.

### Entry

#### Signals

| id         | signature                                                        | emitted when...                                                               |
|------------|------------------------------------------------------------------|-------------------------------------------------------------------------------|
| `activate` | `(Entry*, (Data_t)) -> void`                                     | widget is activated, for example by pressing enter while it holds input focus |
| `text_changed` | `(Entry*, (Data_t)) -> void`                                     | text buffer changes                 |

### TextView

#### Signals

| id         | signature                       | emitted when...                                                                          |
|------------|---------------------------------|------------------------------------------------------------------------------------------|
| `text_changed` | `(TextView*, (Data_t)) -> void` | text buffer changes                                                                      |
| `undo` | `(TextView*, (Data_t)) -> void` | undo keybinding is pressed, this signal can be emitted directly to trigger this behavior |                 
| `undo` | `(TextView*, (Data_t)) -> void` | redo keybinding is pressed, this signal can be emitted directly to trigger this behavior |  

### Revealer

### PopoverMenuButton & Popovers

#### Signals: Popover

| id       | signature                      | emitted when...      |
|----------|--------------------------------|----------------------|
| `closed` | `(Popover*, (Data_t)) -> void` | popover is closed    |  

#### Signals: PopoverMenu

| id       | signature                          | emitted when...      |
|----------|------------------------------------|----------------------|
| `closed` | `(PopoverMenu*, (Data_t)) -> void` | popover is closed    |

#### Signals: PopoverMenuButton

| id         | signature                                | emitted when...                                                                          |
|------------|------------------------------------------|------------------------------------------------------------------------------------------|
| `activate` | `(PopoverMenuButton*, (Data_t)) -> void` | button is clicked or otherwise activated |            

---

## Selectable Widget Containers

### SelectionModel

#### Signals

| id                 | signature                                | emitted when...                                      |
|--------------------|------------------------------------------|------------------------------------------------------|
| `selection_changed` |  (SelectionModel* , int32_t position, int32_t n_items, (Data_t)) -> void` | selection of widget controlled by this model changes |

### ListView

#### Signals

| id         | signature                                                       | emitted when...                                                              |
|------------|-----------------------------------------------------------------|------------------------------------------------------------------------------|
| `activate` | (ListView*, (Data_t)) -> void`                                  | user pressed the enter key or otherwise activates view or a widget inside it |

### GridView

#### Signals

| id         | signature                      | emitted when...                                                                  |
|------------|--------------------------------|----------------------------------------------------------------------------------|
| `activate` | (GridView*, (Data_t)) -> void` | user pressed the enter key or otherwise activates the view or a widget inside it |

### Column View

#### Signals

| id         | signature                        | emitted when...                                                                  |
|------------|----------------------------------|----------------------------------------------------------------------------------|
| `activate` | (ColumnView*, (Data_t)) -> void` | user pressed the enter key or otherwise activates the view or a widget inside it |

### Stack

#### Signals

`Stack` does not directly emit signals, but we can connect so its `SelectionModel` to track changes in the stacks current page

#### StackSwitcher

#### StackSidebar

### Notebook

---

## Additional Widget Container

### Expander

### Revealer

### Grid

### CenterBox

### Fixed




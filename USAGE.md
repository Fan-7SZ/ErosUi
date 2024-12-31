# ErosUi Document

## Overview
This document provides an overview of the APIs and classes implemented in the Frameless Window component, including usage examples and customization options.

## Class Overview
| Class Name            | Description                                 | DerivedClasses                                          | Inherited from|
|-----------------------|---------------------------------------------|---------------------------------------------------------|---------------|
| [`FramelessWindow`](#framelesswindow)     | Main class for creating a frameless window |                                                               |`QWidget`|
| [`Titlebarbase`](#titlebarbase)        | Base class of titlebar                      | [`TitlebarWinstyle`](#titlebarwinstyle)                                            |`QWidget`|
| [`WindowEffect`](#windoweffect)        | Implementation of window effect.            | [`WinStyleEffect`](#winstyleeffect) `Defaulteffect` `Acryliceffect` [`Micaeffect`](#micaeffect) |`QObject`|
| [`titlebarbutton`](#titlebarbutton)     | Implementation of buttons in built-in titlebar|   `closebutton` `minimizebutton` `maximizebutton`           |`QAbstractButton`|
---




## FramelessWindow

>The main class for creating a frameless window. Since it's inherited from `QWidget`, using `setWindowTitle(QString)` to set Window title.

### Properties

`Titlebarbase *t` **[Public]**: This pointer represents the base class for implementing customizable title bars.

`WindowEffect *effect` **[Private]**: This pointer represents the base class for implementing effect of window.

<h3 id='methods_1'>Methods</h3>

`explicit Framelesswindow(QWidget *parent = nullptr)`**[Public]**: Constructor,  which the input parameter is pointer to the parent component, as it is window component here, usually set as `nullptr`.

`void setTitle(const QString &title)`**[Public]**: Sets the window title.

`void setTitlebarBgColor(QColor c)`**[Public]**: Sets title bar background color.

`void setEffect(EWinStyle E)`**[Public]**: Applies a specific window style (See EWinStyle enum class definition [here](#ewinstyle)).

`void setEffect(WindowEffect *custom)`**[Public]**: Applies a custom window effect. (See how to create a window effect [here](#inheritance-template-1))

`void setTitlebar(Titlebarbase *c)`**[Public]**: Sets custom title bar. (See how to create a title bar [here](#inheritance-template))

`WindowEffect* getEffect()`**[Public]**: Returns a pointer to the `WindowEffect` class.

### Signals

- `focus()`: This signal is emitted when the window is focused.

- `unfocus()`: This signal is emitted when the window is unfocused.

- `modechange()`: This signal is emitted when system settings of light or dark mode change.

### EWinStyle

| Constant | Definition            |
|----------|-----------------------|
|EWinStyle::Defaulteffect|    Default window effect: The window has a system-style outline and shadow.                      |
|EWinStyle::Acryliceffect|    Acrylic Effect: The window applies a system-style acrylic material effect on top of the default effect. It is not recommended for resizable windows, as resizing may cause stuttering.                     |
|EWinStyle::Micaeffect|       The window applies a system-style mica material effect on top of the default effect. It can synchronize with the system's light and dark mode settings and provides an API to enforce a specific light or dark mode.          |
---

## TitleBarBase

>Base class for implementing window title bar. It contains null pointers for three buttons and installs an event filter in the constructor to enable mouse drag and resize functionality (see the source code of the `FramelessWindow` class for the definition).

### Derived Classes

- `TitlebarWinstyle`: The specific implementation of the built-in title bar.

### Properties

`titlebarbutton *closebtn`**[Public]**

`titlebarbutton *maximizebtn`**[Public]**

`titlebarbutton *minimizebtn`**[Public]**


<h3 id='methods_2'>Methods</h3>

`explicit Titlebarbase(QWidget *parent = nullptr)`**[Public]**: Constructor, which the input parameter is pointer to parent window component.

`void setbgColor(QColor)`**[Public]**: Sets title bar background color.

`QColor getbgColor()`**[Public]**: Returns title bar background color.

`static qreal getrate(QWidget *w)`**[Public]**: Returns ratio of DPI Scaling.

`void mouseDoubleClickEvent(QMouseEvent*)`**[Protected]** **[Override]**: This method is overridden to enable the window to be maximized and restored when the user double-clicks the title bar.

`void mousePressEvent(QMouseEvent* event)`**[Protected]** **[Override]**: This method is overridden to enable the window to be moved by dragging the title bar which emits a signal `doubleclick()`.

### Signals

- `doubleclick()`: This signal is emitted when title bar area double clicked by user.

### Inheritance Template

``` cpp
class Custom_titlebar : public Titlebarbase{
    Q_OBJECT
public:
    explicit Custom_titlebar(QWidget *parent=nullptr);
    // Other declarations of properties or methods

};

Custom_titlebar::Custom_titlebar(QWidget *parent):
Titlebarbase(parent)
{

    // Instantiation button object, add layout and connect Signals and Slots

}

// Other this class definitions of functions 

```

## TitlebarWinstyle

> The Implementation of the Built-in Title Bar.

### Methods

`explicit TitlebarWinstyle(QWidget *parent=nullptr)` **[Public]**: Constructor, which the input parameter is pointer to parent window component.

`void setTitleColorSet(QVector<QColor> c)`**[Public]**: Sets the value of private property `titleColorSet`, which is a length 2 `QVector<QColor>`. Index 0 represents the color of the title font when the window is focused, and index 1 represents the color of the title font when the window is unfocused.

`QVector<QColor> getTitleColorSet()`**[Public]**: Returns private property `titleColorSet`.

`void setTitleColor(QColor c)`**[Protected]**: Sets the color of window title font.

## titlebarbutton

> Base class for title bar button, including the implementation of button image rendering and the overriding of some mouse event methods.

The button's state is maintained using an enumeration ButtonState, which includes the following values:

- `COMMON`: Default state.
- `HOVER`: When the mouse is hovering over the button.
- `PRESSED`: When the button is pressed.

Button behavior is defined by a virtual slot function named `behavior`. This function is overridden in the derived classes (`CloseButton`, `MinimizeButton`, `MaximizeButton`) to implement specific actions. The overridden slot functions are dynamically bound and invoked when appropriate, enabling polymorphic behavior.

### Derived Classes

- `closebutton` : The button for closing window.

- `minimizebutton` : The button for minimizing window.

- `maximizebutton` : The button for maximizing window.

<h3 id='methods_3'>Methods</h3>

`explicit titlebarbutton(QWidget *)`**[Public]**: Constructor, which the input parameter is pointer to parent window component.

`void setCommonBgColor(QColor)` **[Public]**: Sets button background color when State is common.

`void setHoverBgColor(QColor)` **[Public]**:  Sets button background color when State is hover.

`void setPressedBgColor(QColor)` **[Public]**:  Sets button background color when State is pressed.

`QColor getColor()` **[Protected]**: Returns button background color which responds to current button state.

`void setSvg(QString path)` **[Public]**: Sets buttion icon path.

`QColor getIconColor()` **[Public]**: Returns button color which responds to current button state.

`void setIconColor(QVector<QColor> color)` **[Public]**: Sets the candidate set of background colors for the button. The input is a  length 6 `QVector<QColor>`. Index 0 corresponds to the background color in the `COMMON` state, index 1 to the `HOVER` state, and index 2 to the `PRESSED` state.

`void setColorSet(QVector<QColor> color)` **[Public]**: Sets the complete candidate set of button colors. The input is a  length 6 `QVector<QColor>`. Indices 0-2 correspond to the colors for the three states (`COMMON`, `HOVER`, `PRESSED`) when the window is focused, indices 3-5 correspond to the colors for the three states when the window is unfocused.

`QVector<QColor> getColorSet()` **[Public]**: Returns a `QVector <QColor>` of the complete candidate set of button colors.

`virtual void behavior()` **[Protected]**: This function can be overridden to implement the button's behavior, such as maximizing, minimizing, or closing the window. 

`void paintEvent(QPaintEvent *)` **[Protected]** **[Override]**: This function overrides the paintEvent of `QAbstractButton` and defines how an SVG file is rendered as the button's icon. The SVG image is drawn in the center of the button area, with a size equivalent to a 10px square. (Note: The button area is set as a rectangle with dimensions 46px × 32px.)

## WindowEffect

>Base class for implementing visual effects on windows, providing interface for enabling window effect.

### Derived Classes

- `WinStyleEffect`: Utility class that defines methods for invoking WinAPI to add effects to Qt application windows on the Windows platform.


<h3 id='methods_3'>Methods</h3>

`WindowEffect(QWidget *w)` **[Public]** : Constructor, where the input parameter is QWidget pointer to the window where the effect will be applied.

`virtual void enableWindowEffect()=0` **[Public]**: Public interface for enabling window effect.

### Inheritance Template

```cpp
class Custom_effect : public WindowEffect
{
    Q_OBJECT
public:
    Custom_effect(QWidget *w);
    void enableWindowEffect() override;
    // Other declarations of properties or methods 
};


Custom_effect:: Custom_effect (QWidget *w): //Constructor
    WindowEffect(w)
{
    // put code for preprocessing at here.
}

Custom_effect::enableWindowEffect() // implementation of public interface
{
    QWidget *w=this->w; //utilize w property to apply effect on target window

    // Other specific code for implementing Window effect

}
```

## WinStyleEffect 

> Utility class that defines methods for invoking WinAPI to add effects to Qt application windows on the Windows platform.

### Derived Classes

- `Defaulteffect`: Implementation of default effect.

- `Acryliceffect`: Implementation of Acrylic effect.

- `Micaeffect`: Implementation of Mica effect.

<h3 id='methods_4'>Methods</h3>

`void setShadow(bool)`**[Protected]**: Applies shadow effect to window.

`void setAnimationEffect()`**[Protected]**: Applies animation effect to window.

`void setAcrylicEffect()`**[Protected]**: Applies Acrylic effect to window.

`void enableBlurBehind()`**[Protected]**: Applies window outline blur effect.

`void setMicaEffect()`**[Protected]**: Applies Mica effect to window.

## Micaeffect

>This class implements the Mica effect, supports synchronization with the system's light and dark mode settings, and provides public interfaces to override the system settings. Use the `getEffect` function to obtain a pointer to the effect, allowing access to these public interfaces.

<h3 id='methods_5'>Methods</h3>

`void setForceDarkMode (bool enable)`**[Public]**: If the input parameter is true, the Mica effect of the window is forced into dark mode.

`void setForceLightMode (bool enable)`**[Public]**: If the input parameter is true, the Mica effect of the window is forced into light mode.

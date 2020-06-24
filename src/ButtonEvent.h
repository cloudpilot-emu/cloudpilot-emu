#ifndef _BUTTON_EVENT_H_
#define _BUTTON_EVENT_H_

class ButtonEvent {
   public:
    enum class Button {
        app1,
        app2,
        app3,
        app4,
        rockerUp,
        rockerDown,
        power,
        cradle,
        contrast,
        antenna,
        invalid
    };

    enum class Type { press, release };

   public:
    ButtonEvent(Button button, Type type) : button(button), type(type) {}

    Button GetButton() const { return button; }

    Type GetType() const { return type; }

   private:
    Button button{Button::invalid};

    Type type{Type::press};
};

#endif  // _BUTTON_EVENT_H_

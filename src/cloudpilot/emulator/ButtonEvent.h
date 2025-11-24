#ifndef _BUTTON_EVENT_H_
#define _BUTTON_EVENT_H_

class ButtonEvent {
   public:
    enum class Button {
        app1 = 0,
        app2 = 1,
        app3 = 2,
        app4 = 3,
        rockerUp = 4,
        rockerDown = 5,
        power = 6,
        cradle = 7,
        contrast = 8,
        antenna = 9,
        invalid = 10,
        wheelUp = 11,
        wheelDown = 12,
        wheelPush = 13,
        rockerLeft = 14,
        rockerRight = 15,
        rockerEnter = 16
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

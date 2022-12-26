#ifndef _COMMON_UTIL_H_
#define _COMMON_UTIL_H_

template <typename E>
class EnumWrapper {
   public:
    EnumWrapper(E wrapped) : wrapped(wrapped) {}

    operator E() const { return wrapped; }

    operator long() const { return static_cast<long>(wrapped); }

   private:
    E wrapped;

   private:
    EnumWrapper() = delete;
};

#endif  // _COMMON_UTIL_H_

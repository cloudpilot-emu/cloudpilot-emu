#ifndef _EM_EVENT_H_
#define _EM_EVENT_H_

#include <functional>
#include <vector>

#include "EmCommon.h"

template <class... Ts>
class EmEvent {
   public:
    using HandlerT = function<void(Ts...)>;
    using HandleT = int;

    HandleT AddHandler(HandlerT handler);

    void RemoveHandler(HandleT handle);

    void Dispatch(Ts...);

   private:
    struct Entry {
        int handle;
        HandlerT handler;
    };

    vector<Entry> entries;

    int nextHandle{1};
};

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <class... Ts>
int EmEvent<Ts...>::AddHandler(HandlerT handler) {
    HandleT handle = nextHandle++;

    entries.push_back({.handle = handle, .handler = handler});

    return handle;
}

template <class... Ts>
void EmEvent<Ts...>::RemoveHandler(int handle) {
    typename vector<Entry>::size_type j = 0;

    for (typename vector<Entry>::size_type i = 0; i < entries.size(); i++)
        if (entries[i].handle != handle) {
            if (j != i) entries[j] = entries[i];
            j++;
        }

    if (j < entries.size()) entries.resize(j);
}

template <class... Ts>
void EmEvent<Ts...>::Dispatch(Ts... args) {
    for (Entry& entry : entries) entry.handler(args...);
}

#endif  // _EVENT_H_

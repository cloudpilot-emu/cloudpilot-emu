/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 2000-2001 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#include "EmThreadSafeQueue.h"

#include "ButtonEvent.h"
#include "EmCommon.h"
#include "KeyboardEvent.h"
#include "PenEvent.h"

#ifdef EM_THREADS
    #define LOCK() unique_lock<mutex> lock(fMutex);
#else
    #define LOCK()
#endif

// ---------------------------------------------------------------------------
//		� EmThreadSafeQueue
// ---------------------------------------------------------------------------

template <class T>
EmThreadSafeQueue<T>::EmThreadSafeQueue(int maxSize) : fContainer(), fMaxSize(maxSize) {}

// ---------------------------------------------------------------------------
//		� EmThreadSafeQueue
// ---------------------------------------------------------------------------

template <class T>
EmThreadSafeQueue<T>::~EmThreadSafeQueue(void) {}

// ---------------------------------------------------------------------------
//		� EmThreadSafeQueue::Put
// ---------------------------------------------------------------------------

template <class T>
void EmThreadSafeQueue<T>::Put(const T& value) {
    LOCK();

    EmAssert(fMaxSize == 0 || (long)fContainer.size() < fMaxSize);

    fContainer.push_back(value);
}

// ---------------------------------------------------------------------------
//		� EmThreadSafeQueue::Get
// ---------------------------------------------------------------------------

template <class T>
T EmThreadSafeQueue<T>::Get(void) {
    LOCK();

    // Make sure there's something in the queue (this shouldn't happen,
    // because the caller should always call GetUsed before Get).

    EmAssert(fContainer.size() > 0);

    T result = fContainer[0];
    fContainer.pop_front();

    return result;
}

// ---------------------------------------------------------------------------
//		� EmThreadSafeQueue::Peek
// ---------------------------------------------------------------------------

template <class T>
T EmThreadSafeQueue<T>::Peek(void) {
    LOCK();

    // Make sure there's something in the queue (this shouldn't happen,
    // because the caller should always call GetUsed before Get).

    EmAssert(fContainer.size() > 0);

    T result = fContainer[0];

    return result;
}

// ---------------------------------------------------------------------------
//		� EmThreadSafeQueue::GetFree
// ---------------------------------------------------------------------------

template <class T>
int EmThreadSafeQueue<T>::GetFree(void) {
    LOCK();

    return fMaxSize - fContainer.size();
}

// ---------------------------------------------------------------------------
//		� EmThreadSafeQueue::GetUsed
// ---------------------------------------------------------------------------

template <class T>
int EmThreadSafeQueue<T>::GetUsed(void) {
    LOCK();

    return fContainer.size();
}

// ---------------------------------------------------------------------------
//		� EmThreadSafeQueue::Clear
// ---------------------------------------------------------------------------

template <class T>
void EmThreadSafeQueue<T>::Clear(void) {
    LOCK();

    fContainer.clear();
}

// ---------------------------------------------------------------------------
//		� EmThreadSafeQueue::GetMaxSize
// ---------------------------------------------------------------------------

template <class T>
int EmThreadSafeQueue<T>::GetMaxSize(void) {
    return fMaxSize;
}

// Instantiate the ones we want.

template class EmThreadSafeQueue<uint8>;
template class EmThreadSafeQueue<PenEvent>;
template class EmThreadSafeQueue<KeyboardEvent>;
template class EmThreadSafeQueue<ButtonEvent>;

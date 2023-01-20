#include "CallbackManager.h"

#include <map>

#include "EmBankMapped.h"
#include "EmCPU68K.h"
#include "EmCommon.h"
#include "MetaMemory.h"

namespace {
    struct RegisteredCallback {
        RegisteredCallback(CallbackManager::CallbackT callback, uint16* stub)
            : callback(callback), stub(stub) {}

        CallbackManager::CallbackT callback;
        uint16* stub;
    };

    map<emuptr, RegisteredCallback> registeredCallbacks;
}  // namespace

void CallbackManager::Clear() {
    for (auto& elt : registeredCallbacks) ReleaseCallback(elt.first);
}

emuptr CallbackManager::RegisterCallback(CallbackT callback) {
    uint16* stub = static_cast<uint16*>(malloc(4));
    EmBankMapped::MapPhysicalMemory(stub, 4);
    emuptr callbackPtr = EmBankMapped::GetEmulatedAddress(stub);

    EmMemPut16(callbackPtr, 20085);  // RTS

    MetaMemory::MarkInstructionBreak(callbackPtr);

    registeredCallbacks.emplace(pair(callbackPtr, RegisteredCallback(callback, stub)));

    return callbackPtr;
}

void CallbackManager::ReleaseCallback(emuptr callbackPtr) {
    auto entry = registeredCallbacks.find(callbackPtr);

    if (entry == registeredCallbacks.end()) return;

    RegisteredCallback& registeredCallback = entry->second;

    EmBankMapped::UnmapPhysicalMemory(registeredCallback.stub);
    MetaMemory::UnmarkInstructionBreak(callbackPtr);

    free(registeredCallback.stub);

    registeredCallbacks.erase(callbackPtr);
}

void CallbackManager::HandleBreakpoint() {
    auto entry = registeredCallbacks.find(gCPU->GetPC());

    if (entry != registeredCallbacks.end()) {
        entry->second.callback();
    }
}

CallbackWrapper::CallbackWrapper(CallbackManager::CallbackT callback) {
    callbackPtr = CallbackManager::RegisterCallback(callback);
}

CallbackWrapper::~CallbackWrapper() { CallbackManager::ReleaseCallback(callbackPtr); }

CallbackWrapper::operator emuptr() const { return callbackPtr; }

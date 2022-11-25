#ifndef _EM_SYSTEM_STATE_H_
#define _EM_SYSTEM_STATE_H_

#include "EmCommon.h"
#include "EmEvent.h"

class SavestateLoader;

class EmSystemState {
   public:
    EmSystemState() = default;

    void Initialize();
    void Reset();

    template <typename T>
    void Save(T& savestate);

    void Load(SavestateLoader& loader);

    void SetOSVersion(uint32 version);
    uint32 OSVersion(void) const;
    uint32 OSMajorMinorVersion(void) const;
    uint32 OSMajorVersion(void) const;
    uint32 OSMinorVersion(void) const;

    void SetUIInitialized();
    bool IsUIInitialized() const;

    void SetSetupComplete();
    bool IsSetupComplete() const;

    void SetHotsyncUserName(string hotsyncUserName);
    const string& GetHotsyncUserName() const;

    bool IsScreenDirty() const;
    bool ScreenRequiresFullRefresh() const;
    void MarkScreenClean();
    inline void MarkScreenDirty();
    inline void MarkScreenDirty(emuptr addressLo, emuptr addressHi);

    emuptr GetScreenHighWatermark() const;
    emuptr GetScreenLowWatermark() const;

   public:
    EmEvent<> onMarkScreenClean;

   private:
    template <typename T>
    void DoSaveLoad(T& helper, uint32 version);

   private:
    enum class ScreenState : uint8 { clean, dirty, needsFullRefresh };

    uint32 osVersion{0};

    bool uiInitialized{false};
    bool setupComplete{false};

    string hotsyncUserName;

    ScreenState screenState;
    emuptr screenHighWatermark;
    emuptr screenLowWatermark;
};

extern EmSystemState gSystemState;

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

inline void EmSystemState::MarkScreenDirty() { screenState = ScreenState::needsFullRefresh; }

inline void EmSystemState::MarkScreenDirty(emuptr addressLo, emuptr addressHi) {
    if (likely(screenState == ScreenState::dirty)) {
        if (addressLo < screenLowWatermark) screenLowWatermark = addressLo;
        if (addressHi > screenHighWatermark) screenHighWatermark = addressHi;
    } else if (screenState == ScreenState::clean) {
        screenLowWatermark = addressLo;
        screenHighWatermark = addressHi;
        screenState = ScreenState::dirty;
    }
}

#endif  // _EM_SYSTEM_STATE_H_

#include "EmSession.h"

Bool EmSession::IsNested() { return false; }

Bool EmSession::ExecuteSpecial(Bool checkForResetOnly) { return false; }

Bool EmSession::CheckForBreak(void) { return false; }

#include "SuspendContextClipboardCopy.h"

SuspendContextClipboardCopy::SuspendContextClipboardCopy(const string& clipboardContent)
    : clipboardContent(clipboardContent) {}

SuspendContext::Kind SuspendContextClipboardCopy::GetKind() const { return Kind::clipboardCopy; }

void SuspendContextClipboardCopy::Cancel() { Resume(); }

void SuspendContextClipboardCopy::Resume() { ResumeExecution(); }

const char* SuspendContextClipboardCopy::GetClipboardContent() { return clipboardContent.c_str(); }

bool SuspendContextClipboardCopy::RequiresStackAccess() { return false; }

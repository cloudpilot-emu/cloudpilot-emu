#include "SuspendContextClipboardCopy.h"

SuspendContextClipboardCopy::SuspendContextClipboardCopy(const string& clipboardContent)
    : clipboardContent(clipboardContent) {}

SuspendContext::Kind SuspendContextClipboardCopy::GetKind() const { return Kind::clipboardCopy; }

void SuspendContextClipboardCopy::Cancel() { Resume(); }

void SuspendContextClipboardCopy::Resume() { TriggerResume(); }

string SuspendContextClipboardCopy::GetClipboardContent() { return clipboardContent; }

#ifndef _FEATURE_H_
#define _FEATURE_H_

class Feature {
   public:
    static void SetClipboardIntegration(bool toggle);

    static bool GetClipboardIntegration();

   private:
    static bool clipboardIntegration;
};

#endif  // _FEATURE_H_

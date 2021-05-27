#ifndef _FEATURE_H_
#define _FEATURE_H_

class Feature {
   public:
    static void SetClipboardIntegration(bool toggle);

    static bool GetClipboardIntegration();

    static void SetNetworkRedirection(bool toggle);

    static bool GetNetworkRedirection();

   private:
    static bool clipboardIntegration;
    static bool networkRedirection;
};

#endif  // _FEATURE_H_

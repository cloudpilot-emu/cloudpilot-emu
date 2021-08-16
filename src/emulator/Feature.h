#ifndef _FEATURE_H_
#define _FEATURE_H_

class Feature {
   public:
    static void SetClipboardIntegration(bool toggle);

    static bool GetClipboardIntegration();

    static void SetNetworkRedirection(bool toggle);

    static bool GetNetworkRedirection();

    static void SetHotsyncNameManagement(bool toggle);

    static bool GetHotsyncNameManagement();

   private:
    static bool clipboardIntegration;
    static bool networkRedirection;
    static bool hotsyncNameManagement;
};

#endif  // _FEATURE_H_

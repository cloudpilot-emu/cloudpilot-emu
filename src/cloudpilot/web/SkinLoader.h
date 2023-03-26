#ifndef _SKIN_LOADER_H_
#define _SKIN_LOADER_H_

class SkinLoader {
   public:
    explicit SkinLoader(const char* name);

    ~SkinLoader();

    void* GetData() const;
    unsigned int GetSize() const;

   private:
    void* data;
    unsigned int size;

   private:
    SkinLoader(const SkinLoader&) = delete;
    SkinLoader(SkinLoader&&) = delete;
    SkinLoader& operator=(const SkinLoader&) = delete;
    SkinLoader& operator=(SkinLoader&&) = delete;
};

#endif  // _SKIN_LOADER_H_

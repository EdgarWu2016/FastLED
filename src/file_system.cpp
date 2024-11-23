#include "file_system.h"


#ifdef __EMSCRIPTEN__
#include "platforms/wasm/fs_wasm.h"
#elif __has_include(<SD.h>)
// work in progress.
//#include "platforms/fs_sdcard_arduino.hpp"
#endif

#include "namespace.h"


FASTLED_NAMESPACE_BEGIN

// WEAK SYMBOL
// Override this if you want to supply a file system for your platform.
__attribute__((weak)) FsImplPtr make_sdcard_filesystem(int cs_pin) {
    return FsImplPtr::Null();
}

bool FileSystem::beginSd(int cs_pin) {
    mFs = make_sdcard_filesystem(cs_pin);
    if (!mFs) {
        return false;
    }
    mFs->begin();
    return true;
}

bool FileSystem::begin(FsImplPtr platform_filesystem) {
    mFs = platform_filesystem;
    if (!mFs) {
        return false;
    }
    mFs->begin();
    return true;
}

size_t FileHandle::bytesLeft() const { return size() - pos(); }

FileSystem::FileSystem() : mFs() {}


void FileSystem::end() {
    if (mFs) {
        mFs->end();
    }
}

void FileSystem::close(FileHandlePtr file) { mFs->close(file); }

FileHandlePtr FileSystem::openRead(const char *path) { return mFs->openRead(path); }
Video FileSystem::openVideo(const char *path, size_t pixelsPerFrame, float fps, size_t nFrameHistory) {
    Video video;
    FileHandlePtr file = openRead(path);
    if (!file) {
        video.setError(fl::Str("Could not open file: ") << path);
        return video;
    }
    video.begin(file, pixelsPerFrame, fps, nFrameHistory);
    return video;
}

bool FileSystem::readText(const char *path, fl::Str* out) {
    FileHandlePtr file = openRead(path);
    if (!file) {
        return false;
    }
    size_t size = file->size();
    out->reserve(size + out->size());
    bool wrote = false;
    while (file->available()) {
        uint8_t buf[64];
        size_t n = file->read(buf, sizeof(buf));
        // out->append(buf, n);
        out->append((const char*)buf, n);
        wrote = true;
    }
    file->close();
    return wrote;
}

FASTLED_NAMESPACE_END

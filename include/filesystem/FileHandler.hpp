#pragma once

#include <nn/fs.h>
#include <string>
#include <stdlib.h>

namespace filesystem::FileHandler {
    extern bool isMountedSD;
    inline bool MountSD() {
        if (isMountedSD) {
            return true;
        }
        if (!nn::fs::MountSdCardForDebug("sd")) {
            isMountedSD = true;
            return true;
        }
        return false;
    }
    inline bool FileExists(const char *path) {
        nn::fs::DirectoryEntryType type;
        if (nn::fs::GetEntryType(&type, path).IsFailure()) {
            return false;
        }
        return type == nn::fs::DirectoryEntryType_File;
    }
    inline bool ReadFile(const char *path, std::string &out) {
        nn::fs::FileHandle handle{};
        if (nn::fs::OpenFile(&handle, path, nn::fs::OpenMode_Read).IsFailure()) {
            return false;
        }
        s64 size;
        if (nn::fs::GetFileSize(&size, handle).IsFailure()) {
            nn::fs::CloseFile(handle);
            return false;
        };
        char* buffer = (char*)malloc(size);
        nn::Result res = nn::fs::ReadFile(handle, 0, buffer, size);
        if (res.IsFailure()) {
            free(buffer);
            return false;
        }
        nn::fs::CloseFile(handle);
        out = std::string(buffer, size);
        free(buffer);
        return true;
    }
    inline bool WriteFile(const char *path, std::string text) {
        nn::fs::FileHandle handle{};
        if (FileExists(path)) {
            nn::fs::DeleteFile(path);
        }
        if (nn::fs::CreateFile(path, text.length()).IsFailure()) {
            return false;
        }
        if (nn::fs::OpenFile(&handle, path, nn::fs::OpenMode_Write).IsFailure()) {
            return false;
        }
        if (nn::fs::WriteFile(handle, 0, text.c_str(), text.length(), nn::fs::WriteOption::CreateOption(nn::fs::WriteOptionFlag_Flush)).IsFailure()) {
            nn::fs::CloseFile(handle);
            return false;
        }
        nn::fs::CloseFile(handle);
        return true;
    }
}
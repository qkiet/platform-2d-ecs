#include "internal_utils.h"

std::filesystem::path GetRootPath() {
    static bool isInit = false;
    static std::filesystem::path rootPath;
    if (!isInit) {
        isInit = true;
        rootPath = std::filesystem::current_path().parent_path();
    }
    return rootPath;
}

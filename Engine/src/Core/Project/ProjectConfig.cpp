#include "spch.h"
#include "ProjectConfig.h"
#include "Project.h"
#include "../AppContext.h"

namespace Six {
    // ★ 全是薄转发: 真正的实现在 SProjSetting / SProject 里
    //   (json 相关的查找逻辑关在 ProjectSetting.cpp, 这里只碰头文件)

    bool HasProjectConfig() {
        return Six::SGetProject()->IsLoaded();
    }

    std::string GetConfigStr(const std::string& key, const std::string& fallback) {
        return Six::SGetProject()->GetConfigStr(key, fallback);
    }

    int GetConfigInt(const std::string& key, int fallback) {
        return Six::SGetProject()->GetConfigInt(key, fallback);
    }

    bool GetConfigBool(const std::string& key, bool fallback) {
        return Six::SGetProject()->GetConfigBool(key, fallback);
    }

    bool SaveProjectConfig() {
        return Six::SGetProject()->Save();
    }
}

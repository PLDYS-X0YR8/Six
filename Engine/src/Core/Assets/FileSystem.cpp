#include "spch.h"
#include "FileSystem.h"

namespace Six {
#if defined(_WIN32)
    // 内部辅助函数，不对外暴露
    static HMODULE GetCurrentModuleImpl() {
        HMODULE hModule = nullptr;
        GetModuleHandleExW(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCWSTR)&GetCurrentModuleImpl,
            &hModule);
        return hModule;
    }
#endif

    // ★ 内部: 取 exe 的完整路径。
    //   必须先拿到完整路径才能取 exe 名(stem)——
    //   只拿目录的话 filename() 得到的是目录名("Debug"), 不是 exe 名("Sandbox")。
    static std::filesystem::path RawExecutablePath() {
#if defined(_WIN32)
        wchar_t buffer[MAX_PATH] = { 0 };
        DWORD len = GetModuleFileNameW(nullptr, buffer, MAX_PATH);
        if (len == 0 || len >= MAX_PATH) return {};   // 取不到就当没有, 后面会退回 exe 目录
        return std::filesystem::path(buffer);
#elif defined(__linux__)
        char buffer[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (len != -1) { buffer[len] = '\0'; return std::filesystem::path(buffer); }
        return {};
#elif defined(__APPLE__)
        char buffer[1024];
        uint32_t size = sizeof(buffer);
        if (_NSGetExecutablePath(buffer, &size) == 0)
            return std::filesystem::path(buffer);
        return {};
#else
        return {};
#endif
    }

    // ★ 内部: 取原始目录。
    //   必须走这一层,不能调 SGetExecutableDir().ToPath() —— 那样会
    //   toAbs() -> SResolvePath() -> SGetExecutableDir().ToPath() -> 无限递归。
    static std::filesystem::path RawExecutableDir() {
        return RawExecutablePath().parent_path();
    }

    static std::filesystem::path RawEngineDllDir() {
#if defined(_WIN32)
        HMODULE hModule = GetCurrentModuleImpl();
        wchar_t buffer[MAX_PATH];
        GetModuleFileNameW(hModule, buffer, MAX_PATH);
        return std::filesystem::path(buffer).parent_path();
#else
        return {};
#endif
    }

    // ★ 项目根发现: 从 exe 目录逐级向上找 *.sixproj.json
    //   1) 整条链上优先找 <exe名>.sixproj.json
    //      (Windows 大小写不敏感: Sandbox.exe -> sandbox.sixproj.json)
    //   2) 链上一个同名都没有, 才取遇到的第一个任意 *.sixproj.json
    //   最多向上找 8 层
    static std::filesystem::path DiscoverProjectRoot() {
        const auto exePath = RawExecutablePath();
        const auto exeDir = exePath.parent_path();
        if (exeDir.empty()) return {};
        // exe 名(不含扩展名): "Sandbox.exe" -> "Sandbox"
        const std::string exeStem = exePath.stem().string();

        // 第 1 轮: 同名优先(必须扫完整条链, 否则 exe 输出目录里
        //   一个无关的 *.sixproj.json 就会把绑定抢走)
        if (!exeStem.empty()) {
            std::filesystem::path dir = exeDir;
            for (int i = 0; i < 8; ++i) {
                std::error_code ec;
                if (std::filesystem::exists(dir / (exeStem + ".sixproj.json"), ec)) return dir;
                if (dir == dir.parent_path()) break;
                dir = dir.parent_path();
            }
        }

        // 第 2 轮: 任意一个 *.sixproj.json
        {
            std::filesystem::path dir = exeDir;
            for (int i = 0; i < 8; ++i) {
                std::error_code ec;
                for (std::filesystem::directory_iterator it(dir, ec);
                     !ec && it != std::filesystem::directory_iterator();
                     it.increment(ec)) {
                    if (it->path().filename().string().ends_with(".sixproj.json")) return dir;
                }
                if (dir == dir.parent_path()) break;
                dir = dir.parent_path();
            }
        }
        return {};
    }

    SIXAPI std::filesystem::path SGetProjectRoot() {
        // 只发现一次(函数内 static,线程安全初始化)
        static const std::filesystem::path root = DiscoverProjectRoot();
        return root;
    }

    SIXAPI std::filesystem::path SResolvePath(const std::string& raw) {
        if (raw.empty()) return {};
        const std::filesystem::path p(raw);

        // 内置资源前缀 #xxx
        if (raw[0] == '#') {
            return RawEngineDllDir() / "assets" / "builtin" / raw.substr(1);
        }

        const auto projectRoot = SGetProjectRoot();
        const auto fallback = RawExecutableDir();
        const auto base = projectRoot.empty() ? fallback : projectRoot;

        // 项目虚拟路径前缀 //assets/xxx
        if (raw.rfind("//", 0) == 0) {
            return base / raw.substr(2);
        }

        // 绝对路径原样返回
        if (p.is_absolute()) {
            return p;
        }

        // 相对路径 -> 相对项目根;找不到项目根才退回 exe 目录
        // weakly_canonical 可以把 a/../b 这种冗余路径规范化,且不要求文件一定存在
        return std::filesystem::weakly_canonical(base / p);
    }

    SIXAPI SPath SGetExecutableDir() {
        return SPath(RawExecutableDir());
    }

    SIXAPI SPath SGetEngineDllDir() {
        return SPath(RawEngineDllDir());
    }

    SIXAPI SPath SGetProjectDir() {
        const auto root = SGetProjectRoot();
        if (!root.empty()) return SPath(root);
        // 退化: 找不到项目根就退回 exe 目录本身(assets 放在 exe 旁边)
        //   ★ 以前是"exe 目录的上一级", 那是旧 out\x64\Debug 布局的猜测残留,
        //     现在会算出 Editor\out\x64\assets 这种鬼路径
        return SPath(RawExecutableDir());
    }

    // 项目配置文件: <项目根>/<exe名>.sixproj.json
    //   找不到同名就取项目根下第一个 *.sixproj.json;都没有则返回空 SPath
    SIXAPI SPath SGetProjectFile() {
        const auto root = SGetProjectRoot();
        if (root.empty()) return SPath();

        std::error_code ec;
        // 1) 同名优先
        const auto exeStem = RawExecutablePath().stem().string();
        if (!exeStem.empty()) {
            const auto p = root / (exeStem + ".sixproj.json");
            if (std::filesystem::exists(p, ec)) return SPath(p);
        }
        // 2) 退化: 项目根下第一个 *.sixproj.json
        for (std::filesystem::directory_iterator it(root, ec);
             !ec && it != std::filesystem::directory_iterator();
             it.increment(ec)) {
            if (it->path().filename().string().ends_with(".sixproj.json")) return SPath(it->path());
        }
        return SPath();
    }

    SIXAPI SPath SGetAssetDir() {
        return SGetProjectDir() / "assets";
    }
}

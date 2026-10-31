#pragma once
#include "spch.h"

namespace Six{
	// 项目根目录: 从 exe 目录逐级向上找 *.sixproj.json 找到的那一层。
	//   优先 <exe名>.sixproj.json(Windows 文件系统大小写不敏感:
	//   Sandbox.exe -> sandbox.sixproj.json),否则取遇到的第一个 *.sixproj.json。
	//   找不到返回空路径(此时相对路径退回以 exe 目录为基准)。
	SIXAPI std::filesystem::path SGetProjectRoot();

	// 把原始路径串解析成绝对路径(实现见 FileSystem.cpp):
	//   #xxx    -> 引擎内置资源 <EngineDllDir>/assets/builtin/xxx
	//   //xxx   -> <项目根>/xxx          (项目虚拟路径前缀)
	//   相对路径 -> <项目根>/xxx
	//   绝对路径 -> 原样
	// 放在 .cpp 是因为它要用到 SGetProjectRoot / SGetEngineDllDir,
	// 而 SPath 是 dllimport 类型,不能在头文件里直接调这些导出函数。
	SIXAPI std::filesystem::path SResolvePath(const std::string& raw);

    struct SIXAPI SPath {
        std::string path_str;

        // 默认构造
        SPath() = default;

        // 方便隐式转换（比如直接把 string 传进来）
        SPath(const std::string& path) : path_str(path) {}
        SPath(const char* path) : path_str(path) {}
        SPath(const std::filesystem::path& p) : path_str(p.string()) {}
        operator std::filesystem::path() const {
            return toAbs();
        }

        // 1. 判断是否为绝对路径（由 filesystem 动态计算，无需 is_abs 变量）
        bool IsAbsolute() const {
            // 如果是你引擎定义的特殊前缀，也算绝对/特殊路径
            if (!path_str.empty() && (path_str[0] == '#' || path_str.rfind("//", 0) == 0)) {
                return true;
            }
            return std::filesystem::path(path_str).is_absolute();
        }

        // 2. 获取真实的绝对路径（核心）—— 解析逻辑在 FileSystem.cpp 的 SResolvePath
        std::filesystem::path toAbs() const {
            return SResolvePath(path_str);
        }

        // 3. 便捷转换
        std::filesystem::path ToPath() const { return toAbs(); }
        // 与 std::filesystem::path 同名的接口: 规范化后的绝对路径字符串
        std::string string() const { return toAbs().string(); }
        std::string ToString() const { return string(); }
        const char* c_str() const { return path_str.c_str(); }

        // 4. 路径拼接: SPath / "assets" / "DefaultTexture.png"
        //    SPath 是路径类型,拼接是基本操作;缺了它 SGetEngineDllDir()/"assets" 这类写法编不过
        SPath operator/(const std::filesystem::path& rhs) const {
            return SPath(toAbs() / rhs);
        }

        // 5. 取父目录
        SPath parent_path() const {
            return SPath(toAbs().parent_path());
        }

        // 6. 检查文件是否存在
        bool Exists() const {
            std::error_code ec;
            return std::filesystem::exists(toAbs(), ec);
        }

        // 重载 == 方便比较
        bool operator==(const SPath& other) const { return path_str == other.path_str; }
    };

    // 项目根: 优先 SGetProjectRoot() 找到的目录,找不到才退回 exe 目录的上一级
	SIXAPI SPath SGetProjectDir();
	// exe 所在目录
	// 项目配置文件: <项目根>/<exe名>.sixproj.json
	//   找不到同名就取项目根下第一个 *.sixproj.json;都没有则返回空 SPath
	SIXAPI SPath SGetProjectFile();
	SIXAPI SPath SGetExecutableDir();
	// <项目根>/assets
	SIXAPI SPath SGetAssetDir();
	// Engine.dll 所在目录
	SIXAPI SPath SGetEngineDllDir();
}

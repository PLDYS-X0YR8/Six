#pragma once
#include "FileSystem.h"
#include "spch.h"

namespace Six {
	enum class SAssetsType {
		Texture,
		Mesh,
		Material,
		Shader,
		SceneTree,
		Config,
		Json,
		Unknown
	};
	enum class SAssetsLoadState {
		Unloaded,
		Loading,
		Loaded,
		Error,
		Unloading
	};
	// ★ 前向声明:SAssetsHandle::Get() 里要用 SAssetsManager
	class SIXAPI SAssetsManager;

	// ★ 必须定义在 SAssetsHandle 之前 —— 它的 Get() 里要用 SAsset*
	class SIXAPI SAsset {
	public:
		virtual ~SAsset() = default;

		// 每个资产自带类型和状态
		SAssetsType Type = SAssetsType::Unknown;
		SAssetsLoadState LoadState = SAssetsLoadState::Unloaded;

		virtual void OnLoadComplete() {} // 异步加载完成时的回调
	};

	struct SIXAPI SAssetsHandle {
		uint64_t uuid = 0;
		bool operator==(const SAssetsHandle& other) const {
			return uuid == other.uuid;
		}

		// 2. 判断句柄是否有效
		bool IsValid() const { return uuid != 0; }
		template<typename T>
		T* Get() {
			if (!IsValid()) return nullptr;

			// 调用 Manager 获取基类指针
			SAsset* asset = SAssetsManager::GetInstance().GetAsset(uuid);

			// 安全向下转型（如果你关了 RTTI，可以改成 static_cast<T*>）
			return dynamic_cast<T*>(asset);
		}
	};
}

namespace std {
	template<>
	struct hash<Six::SAssetsHandle> {
		size_t operator()(const Six::SAssetsHandle& handle) const {
			return hash<uint64_t>()(handle.uuid);
		}
	};
}

namespace Six{
	class SIXAPI SAssetsManager {
	public:
		static SAssetsManager& GetInstance() {
			static SAssetsManager m;
			return m;
		}
		SAsset* GetAsset(uint64_t uuid) {
			auto it = m_UuidToAsset.find(uuid);
			if (it != m_UuidToAsset.end()) {
				return it->second.get();
			}
			return nullptr;
		}

		// 统一的注册接口（由 LoadXxx 内部调用）
		SAssetsHandle RegisterAsset(const std::string& path, std::shared_ptr<SAsset> asset) {
			// 如果已经加载过，直接返回旧句柄（防重复加载）
			if (m_PathToUuid.find(path) != m_PathToUuid.end()) {
				return { m_PathToUuid[path] };
			}

			SAssetsHandle handle;
			handle.uuid = GenerateUUID();

			m_UuidToAsset[handle.uuid] = asset;
			m_PathToUuid[path] = handle.uuid;

			return handle;
		}

		// 获取当前加载的资产总数（方便 Debug）
		size_t GetAssetCount() const { return m_UuidToAsset.size(); }
		// 统一交给 SPath 解析(项目根 / 引擎内置 / 绝对路径)。
		// ★ 不要再单独用 current_path() —— 那是进程 CWD,
		//   VS 里启动和命令行启动的结果不一样,资源路径会飘。
		static std::string ResolvePath(const std::string& virtualPath) {
			if (virtualPath.empty()) return "";
			return SPath(virtualPath).ToString();
		}
	private:
		uint64_t GenerateUUID() {
			static uint64_t nextId = 1;
			return nextId++;
		}

		// 核心存储池
		std::unordered_map<uint64_t, std::shared_ptr<SAsset>> m_UuidToAsset;
		std::unordered_map<std::string, uint64_t> m_PathToUuid;
	};
}
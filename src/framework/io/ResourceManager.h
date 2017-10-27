#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#pragma once

#include <map>
#include <string>

namespace Framework
{
	enum class ResourceLifespan : unsigned
	{
		SCENE,
		MENU,
		GLOBAL
	};

	struct GenericResource 
	{
		ResourceLifespan lifespan;
		GenericResource(ResourceLifespan lspan) : lifespan(lspan) {}
		virtual ~GenericResource() {}
	};
	template <typename DataType>
	struct Resource final : GenericResource
	{
		DataType data;
		Resource(DataType data, ResourceLifespan lspan = ResourceLifespan::GLOBAL) 
			: data(data), GenericResource(lspan) {}
		Resource(const Resource& r) = delete;
		Resource& operator=(const Resource& r) = delete;
	};
	template <typename DataType>
	struct Resource<DataType*> final : GenericResource
	{
		DataType* data;
		Resource(DataType* data, ResourceLifespan lspan = ResourceLifespan::GLOBAL)
			: data(data), GenericResource(lspan) {}
		Resource(const Resource& r) = delete;
		Resource& operator=(const Resource& r) = delete;
		~Resource()
		{
			delete data;
		}
	};
	
	class ResourceManager final
	{
	private:
		ResourceManager(const ResourceManager& r) = delete;
		ResourceManager& operator=(const ResourceManager& r) = delete;

		static std::map<std::string, GenericResource*> _resources;
	public:
		static void ClearLifespan(ResourceLifespan span)
		{
			for (auto it = _resources.end(); it != _resources.begin(); it++)
			{
				if (it->second->lifespan == span)
				{
					GenericResource* rp = it->second;
					_resources.erase(it);
					delete rp;
				}
			}
		}

		template <typename Data>
		static Data* Load(const char* path, ResourceLifespan span = ResourceLifespan::GLOBAL)
		{
			LOG(LogLevel::WARN, "Unsure how to load: %s", path);
			return nullptr;
		}
	};

	std::map<std::string, GenericResource*> ResourceManager::_resources;

	template<>
	Shader* ResourceManager::Load(const char* path, ResourceLifespan span)
	{
		LOG(LogLevel::INFO, "Loading shader!");
		
		Shader shader;
		shader.load("basic", path);
		Resource<Shader>* res = new Resource<Shader>(shader, span);
		ResourceManager::_resources[std::string(path)] = res;
	}
}

#endif // RESOURCE_MANAGER_H
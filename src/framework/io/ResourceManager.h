#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#pragma once

#include <map>
#include <string>
#include "framework/graphics/Shader.h"
#include "framework/util/Logger.h"
#include "framework/util/Convenience.h"
#include "framework/graphics/Mesh.h"
#include "OBJLoad.h"
#include "framework/graphics/Texture2D.h"
#include "framework/graphics/Cubemap.h"

namespace Framework
{
#define CHECK_CACHE(path, Type) do { Type *t; if(CheckCache(path, t)) return t->data; } while(0)
	
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
		typedef std::map<std::string, GenericResource*>::iterator ResourcesInterator;
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
		static bool CheckCache(const char* path, Data& res)
		{
			ResourcesInterator it = _resources.find(path);
			if (it != _resources.end())
			{
				// resource exists in cache
				res = static_cast<Data>(it->second);
				return true;
			}
			res = nullptr;
			return false;
		}

		template <typename Data>
		static Data* Load(const char* path, ResourceLifespan span = ResourceLifespan::GLOBAL)
		{
			LOG(LogLevel::WARN, "Unsure how to load: %s", path);
			return nullptr;
		}
	};

	template<>
	inline Shader* ResourceManager::Load(const char* path, ResourceLifespan span)
	{
		CHECK_CACHE(path, Resource<Shader*>);
		
		const std::string shaderName = removeExtension(basename(path));
		
		Resource<Shader*>* res = new Resource<Shader*>(new Shader(shaderName, path), span);
		ResourceManager::_resources[std::string(path)] = res;

		return res->data;
	}

	template<>
	inline Mesh* ResourceManager::Load(const char* path, ResourceLifespan span)
	{
		CHECK_CACHE(path, Resource<Mesh*>);

		Mesh *m = OBJLoad::Load(path);
		Resource<Mesh*>* res = new Resource<Mesh*>(m, span);
		_resources[std::string(path)] = res;

		return res->data;
	}

	template<>
	inline Texture2D* ResourceManager::Load(const char* path, ResourceLifespan span)
	{
		CHECK_CACHE(path, Resource<Texture2D*>);

		Texture2D *tex = new Texture2D(path);
		Resource<Texture2D*>* res = new Resource<Texture2D*>(tex, span);
		_resources[std::string(path)] = res;

		return res->data;
	}

	template<>
	inline Cubemap* ResourceManager::Load(const char* path, ResourceLifespan span)
	{
		CHECK_CACHE(path, Resource<Cubemap*>);

		Cubemap *cubemap = new Cubemap(path);
		Resource<Cubemap*>* res = new Resource<Cubemap*>(cubemap, span);
		_resources[std::string(path)] = res;

		return res->data;
	}
}

#endif // RESOURCE_MANAGER_H
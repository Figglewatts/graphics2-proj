#ifndef OBJLOAD_H
#define OBJLOAD_H
#pragma once

#include <string>

namespace Framework
{
	class Mesh;

	class OBJLoad
	{
	private:
		OBJLoad() = delete;
		OBJLoad(const OBJLoad& objload) = delete;
	public:
		static Mesh* Load(const std::string& path);
	};
}

#endif // OBJLOAD_H
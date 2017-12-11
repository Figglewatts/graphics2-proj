#ifndef CONVENIENCE_H
#define CONVENIENCE_H
#pragma once

#include <string>
#include <glm/gtc/quaternion.hpp>

/*
 *	SOURCES:
 *	https://stackoverflow.com/a/8520871
 *
 */

struct MatchPathSeparator
{
	bool operator()(char ch) const
	{
		return ch == '\\' || ch == '/';
	}
};

// get the name of a file from a path
std::string basename(std::string const& pathname);

// remove the extension from a file name
std::string removeExtension(std::string const& filename);

glm::quat rotationBetween(glm::vec3 start, glm::vec3 dest);

#endif // CONVENIENCE_H
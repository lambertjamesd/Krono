#pragma once

#include <map>
#include <string>
#include <vector>

class GLSLIDRename
{
public:
	GLSLIDRename(void);
	~GLSLIDRename(void);

	const std::string& RenameID(const std::string& input) const;
	void AddRename(const std::string& oldName, const std::string& newName);
private:
	void PopulateNames();

	std::map<std::string, std::string> mRenameMap;
};


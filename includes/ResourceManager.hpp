#ifndef RESOURCE_MANAGER_HPP
# define RESOURCE_MANAGER_HPP

# include <string>
# include <SDL.h>

class ResourceManager
{
public:
	~ResourceManager(void);

	static ResourceManager & getInstance(void);
	std::string				getAbsolutePathname(std::string filename);

private:

	std::string				_basePath;

	ResourceManager(void);
	ResourceManager(const ResourceManager & src);
	ResourceManager & operator=(const ResourceManager & rhs);

};

#endif

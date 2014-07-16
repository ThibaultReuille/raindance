#pragma once

#include "Core/Resources/Resource.hh"

#include "Core/Headers.hh"
#include "Core/Resources/Texture.hh"
#include "Core/Resources/Shader.hh"

class ResourceManager
{
public:
	static ResourceManager& getInstance()
	{
		static ResourceManager instance;
		return instance;
	}

private:
	ResourceManager() {};
	ResourceManager(ResourceManager const&);
	void operator=(ResourceManager const&);

	struct ResourceItem
	{
		union
		{
			IResource* m_Resource;
			Texture* m_Texture;
			Shader::Program* m_Shader;
		};
		unsigned int m_RefCount;
	};

	ResourceItem* findResource(const char* name)
	{
		std::string sname(name);

		std::vector<ResourceItem>::iterator it;
		for (it = m_Resources.begin(); it != m_Resources.end(); ++it)
		{
			if (it->m_Resource->name() == sname)
			{
				return &(*it);
			}
		}
		return NULL;
	}
	std::vector<ResourceItem>::iterator findResource(IResource* resource)
	{
		std::vector<ResourceItem>::iterator it;
		for (it = m_Resources.begin(); it != m_Resources.end(); ++it)
		{
			if (it->m_Resource == resource)
			{
				return it;
			}
		}
		return it;
	}
	ResourceItem* addResource(IResource* resource)
	{
		ResourceItem item;
		item.m_RefCount = 1;
		item.m_Resource = resource;
		m_Resources.push_back(item);
		return &m_Resources.back();
	}

public:
	Texture* loadTexture(const char* name, unsigned char* data, unsigned int size)
	{
		ResourceItem* item = findResource(name);
		if (item == NULL)
		{
			Texture* texture = new Texture(name, data, size);
			item = addResource(texture);
			LOG("[RESOURCE] loadTexture('%s', %p, %i)\n", name, data, size);
		}
		else
		    item->m_RefCount++;

		return item->m_Texture;
	}

	Texture* loadTexture(const char* name, unsigned char* data, int width, int height, int components)
	{
		ResourceItem* item = findResource(name);
		if (item == NULL)
		{
			Texture* texture = new Texture(name, data, width, height, components);
			item = addResource(texture);
			LOG("[RESOURCE] loadTexture('%s', %p, %i, %i, %i)\n", name, data, width, height, components);
		}
		else
		    item->m_RefCount++;

		return item->m_Texture;
	}

	Texture* loadTexture(const char* name, int width, int height, int components)
	{
		ResourceItem* item = findResource(name);
		if (item == NULL)
		{
			Texture* texture = new Texture(name, width, height, components);
			item = addResource(texture);
			LOG("[RESOURCE] loadTexture('%s', %i, %i, %i)\n", name, width, height, components);
		}
		else
		    item->m_RefCount++;

		return item->m_Texture;
	}

	Shader::Program* loadShader(const char* name, const unsigned char* vertex, unsigned long vertexSize, const unsigned char* fragment, unsigned long fragmentSize)
	{
		ResourceItem* item = findResource(name);
		if (item == NULL)
		{
			Shader::Program* shader = new Shader::Program();
			shader->load(name, vertex, vertexSize, fragment, fragmentSize);
			item = addResource(shader);
			LOG("[RESOURCE] loadShader('%s', %p, %lu, %p, %lu)\n", name, vertex, vertexSize, fragment, fragmentSize);
		}
		else
		    item->m_RefCount++;

		return item->m_Shader;
	}

	Shader::Program* loadShader(const char* name, const std::string& vertex, const std::string& fragment)
	{
		return loadShader(name, reinterpret_cast<const unsigned char*>(vertex.data()), vertex.size(),
								reinterpret_cast<const unsigned char*>(fragment.data()), fragment.size());
	}

	bool unload(IResource* resource)
	{
		std::vector<ResourceItem>::iterator item = findResource(resource);
		if (item == m_Resources.end())
		{
			LOG("[RESOURCE] Resource pointer is not found !\n");
			return false;
		}

		item->m_RefCount --;
		if (item->m_RefCount > 0)
			LOG("[RESOURCE] Resource '%s' (RefCount : %u)\n", item->m_Resource->name().c_str(), item->m_RefCount);
		else
		{
			LOG("[RESOURCE] Destroying resource '%s' ...\n", item->m_Resource->name().c_str());
			delete item->m_Resource;
			m_Resources.erase(item);
		}

		return true;
	}

	void dump()
	{
		std::cout << "[RESOURCE]" << std::endl;

		unsigned int totalMemory = 0;

		std::vector<ResourceItem>::iterator it;
		for (it = m_Resources.begin(); it != m_Resources.end(); ++it)
		{
			std::cout
				<< "    . " << it->m_Resource->name() << " : "
				<< it->m_Resource->memory() << " bytes, "
				<< it->m_RefCount << (it->m_RefCount <= 1 ? " reference" : " references")
				<< "." << std::endl;
			totalMemory += it->m_Resource->memory();
		}

		std::cout << "    Total : " << m_Resources.size() << " resources, " << totalMemory << " bytes." << std::endl;
	}

private:
	std::vector<ResourceItem> m_Resources;
};


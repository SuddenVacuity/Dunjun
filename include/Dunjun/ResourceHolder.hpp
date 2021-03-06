
#ifndef DUNJUN_RESOURCE_HOLDER_HPP
#define DUNJUN_RESOURCE_HOLDER_HPP

#include <Dunjun/Graphics/Material.hpp>

//#include <unordered_map>

namespace Dunjun
{
	template <typename Resource, typename Identifier>
	class ResourceHolder
	{
	public:
		using IdentifierType = Identifier;
		using ResourceType = Resource;
		using ResourceUPtr = std::unique_ptr<Resource>;


		void insert(Identifier id, ResourceUPtr resource)
		{
			auto inserted = m_resources.insert(std::make_pair(id, std::move(resource)));
			assert(inserted.second && "ResourceHolder resource not inserted.");
		}

		std::unique_ptr<Resource> erase(const Resource& resource)
		{
			auto found = std::find_if(std::begin(m_resources),
									  std::end(m_resources),
								 	  [&resource](ResourceUPtr& res)
			{
				return res.get();
			});

			if (found != std::end(m_resources))
			{
				auto result = std::move(*found);

				m_resources.erase(found);

				return result;
			}
			// resource not found
			return nullptr;
		}

		std::unique_ptr<Resource> erase(Identifier id)
		{
			auto found = m_resources.find(id);

			if (found != std::end(m_resources))
			{
				auto result = std::move(*found);

				m_resources.erase(found);

				return result;
			}
			// resource not found
			return nullptr;
		}

		bool exists(Identifier id)
		{
			auto found = m_resources.find(id);

			if(found != m_resources.end())
				return true;

			return false;
		}

		Resource& get(Identifier id)
		{
			auto found = m_resources.find(id);
			assert(found != std::end(m_resources) && "ResourceHolder::get resource not found.");

			return *found->second;
		}

		const Resource& get(Identifier id) const
		{
			auto found = m_resources.find(id);
			assert(found != std::end(m_resources) && "ResourceHolder::get resource not found.");

			return *found->second;
		}

	protected:
		std::unordered_map<Identifier, ResourceUPtr> m_resources;

	};
} // end Dunjun

#endif

#ifndef DUNJUN_LEVEL_HPP
#define DUNJUN_LEVEL_HPP

#include <Dunjun/ModelAsset.hpp>

namespace Dunjun
{
	class Level
	{
	public:
		enum class TileSurfaceFace : u32
		{
			// directions tiles can face
			Right = 0,
			Left = 1,
			Up = 2,
			Down = 3,
			Forward = 4,
			Backward = 5,
		};

		// coordinates of texture in texture map
		struct TileId
		{
			u32 x;
			u32 y;
		};

		using RandomTileSet = std::vector<TileId>;

		Level();
		virtual ~Level();

		void addTileSurface(const Vector3& position, TileSurfaceFace face, const TileId& texPos);
		void addTileSurface(const Vector3& position, TileSurfaceFace face, const RandomTileSet& randomTileSet);

		void generate();

		const Material* material;
		Mesh* mesh;
		Transform transform;
	private:
		Mesh::Data m_meshData;
	};
} // end Dunjun

#endif

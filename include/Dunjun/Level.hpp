#ifndef DUNJUN_LEVEL_HPP
#define DUNJUN_LEVEL_HPP

#include <Dunjun/Scene.hpp>

namespace Dunjun
{
	class Level
	{
	public:
		enum class TileSurfaceFace : u32
		{
			// directions tiles can face
			Left = 0,
			Right = 1,
			Up = 2,
			Down = 3,
			Backward = 4,
			Forward = 5,
		};

		// coordinates of texture in texture map
		struct TileId
		{
			u32 x = 0;
			u32 y = 0;

			TileId()
			{
			}

			TileId(u32 x, u32 y)
				: x(x)
				, y(y)
			{
			}

			TileId(u32 xy[2])
				:x(xy[0])
				,y(xy[1])
			{
			}

			inline bool operator==(const TileId& id)
			{
				return (x == id.x) && (y == id.y);
			}

			inline bool operator!=(const TileId& id)
			{
				return !operator==(id);
			}
		};

		using RandomTileSet = std::vector<TileId>;

		Level();
		virtual ~Level();

		void addTileSurface(const Vector3& position, TileSurfaceFace face, const TileId& texPos);
		void addTileSurface(const Vector3& position, TileSurfaceFace face, const RandomTileSet& randomTileSet);

		void generate();

		std::vector<std::vector<TileId>> mapGrid;
		ReadOnly<u32, Level> sizeX = 128;
		ReadOnly<u32, Level> sizeZ = 128;
		ReadOnly<u32, Level> sizeY = 4;
		ReadOnly<u32, Level> wallHeight = 1;

		const Material* material;
		Mesh* mesh;
		Transform transform;
	private:
		void placeRooms();

		std::vector<Rectangle> m_rooms;

		Random m_random;
		Mesh::Data m_meshData;
	};
} // end Dunjun

#endif

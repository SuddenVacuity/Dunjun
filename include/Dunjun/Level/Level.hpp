

#include <Dunjun/Level/Room.hpp>
#ifndef DUNJUN_LEVEL_HPP
#define DUNJUN_LEVEL_HPP

namespace Dunjun
{
	class Level : public SceneNode
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
		//virtual ~Level();

		void addTileSurface(const Vector3& position, TileSurfaceFace face, const TileId& texPos);
		void addTileSurface(const Vector3& position, TileSurfaceFace face, const RandomTileSet& randomTileSet);

		void generate();

		std::vector<std::vector<TileId>> mapGrid;
		u32 sizeX = 128;
		u32 sizeZ = 128;
		u32 sizeY = 1;
		u32 wallHeight = 1;

		//Mesh* mesh;
		Transform transform;
		Material* material;

		u32 roomsRendered = 0;
		std::vector<Room*> rooms;
	private:
		void placeRooms();

		u32 roomCount;

		Random m_random;
		//Mesh::Data m_meshData;
	};
} // end Dunjun

#endif

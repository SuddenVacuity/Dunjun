#ifndef DUNJUN_lEVEL_ROOM_HPP
#define DUNJUN_lEVEL_ROOM_HPP


#include <Dunjun/Scene.hpp>


namespace Dunjun
{
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
			, y(xy[1])
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

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					.
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	class Room : public SceneNode
	{
	public:
		using u_ptr = std::unique_ptr<Room>;

		struct Size
		{
			Size(u32 x, u32 y, u32 z)
				: x(x)
				, y(y)
				, z(z)
			{
			}

			u32 x = 0;
			u32 y = 0;
			u32 z = 0;
		};

		enum class Walls
		{
			North,
			West,
			South,
			East, 
		};

		enum class Door
		{
			North,
			West,
			South,
			East,
		};

		explicit Room(Random& random, const Room::Size& size);

		virtual ~Room() override;

		void generate(bool showWalls);
		void generate(bool northWall, bool westWall, bool southWall, bool eastWall);

		const Room::Size size;
		const Material* material;

		bool northDoor = false;
		bool westDoor = false;
		bool southDoor = false;
		bool eastDoor = false;
	protected:

		void addTileSurface(const Vector3& position, TileSurfaceFace face, const TileId& texPos);
		void addTileSurface(const Vector3& position, TileSurfaceFace face, const RandomTileSet& randomTilePosSet);

		Mesh* m_mesh;

		Random& m_random;
		bool m_generated;
		Mesh::Data m_meshData;
	};
} // end Dunjun

#endif

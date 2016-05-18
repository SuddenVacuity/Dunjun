
#include <Dunjun/Level.hpp>


namespace Dunjun
{

	Level::Level()
		: m_random()
		, m_rooms()
	{
	}

	//Level::~Level()
	//{
	//	delete mesh;
	//}

	void Level::generate()
	{
	//	if (!mesh)
	//		mesh = new Mesh();
	//
	//	u32 roomSizeY = 4;
	//
	//	// initialize mapGrid
	//	mapGrid = std::vector<std::vector<TileId>>(sizeX, std::vector<TileId>(sizeZ));
	//
	//	m_random.setSeed(1);
	//
	//	static const TileId emptyTile = { 0xFFFFFFFF, 0xFFFFFFFF };
	//
	//	// location of texture in image map
	//	TileId lightWoodTile = { 0, 11 };
	//	TileId meshTile = { 0, 13 };
	//	TileId lightLogsTile = { 1, 11 };
	//	TileId lightLogEndTile = { 2, 11 };
	//	TileId stoneTile = { 2, 15 };
	//	TileId darkBrownDirtTile = { 3, 14 };
	//	
	//	Level::RandomTileSet mossyStoneTiles;
	//	for (u32 i = 1; i <= 2; i++)
	//		mossyStoneTiles.emplace_back(Level::TileId{ i, 15 });
	//
	//	// initialize mapGrid to build room from
	//	for (int i = 0; i < sizeX; i++)
	//	{
	//		for (int j = 0; j < sizeZ; j++)
	//				mapGrid[i][j] = emptyTile;
	//	}

		placeRooms();

	//	// generate rooms
	//	for(const auto& room : m_rooms)
	//	{
	//		for (int i = room.x; i < room.x + room.width; i++)
	//		{
	//			for (int j = room.y; j < room.y + room.height; j++)
	//			{
	//				mapGrid[i][j] = lightWoodTile;
	//			}
	//		}
	//	}
	//
	//	// generate mesh
	//	for (int i = 0; i < sizeX; i++)
	//		for (int j = 0; j < sizeZ; j++)
	//		{ 
	//			// generate edge walls
	//			if (i == 0)
	//			{ // left edge
	//				for (int k = 0; k < sizeY; k++)
	//					if(mapGrid[i][j] != emptyTile || k >= 0) // only add a wall if there's a floor or if is higher than obstacle
	//						addTileSurface(Vector3(i, k, j), TileSurfaceFace::Right, meshTile);
	//			}
	//			else if (i == sizeX - 1)
	//			{ // right edge
	//				for (int k = 0; k < sizeY; k++)
	//					if (mapGrid[i][j] != emptyTile || k >= 0)
	//						addTileSurface(Vector3(i + 1, k, j), TileSurfaceFace::Left, meshTile);
	//			}
	//			if (j == 0)
	//			{ // back edge
	//				for (int k = 0; k < sizeY; k++)
	//					if (mapGrid[i][j] != emptyTile || k >= 0)
	//						addTileSurface(Vector3(i, k, j), TileSurfaceFace::Backward, meshTile);
	//			}
	//			else if (j == sizeZ - 1)
	//			{ // front edge
	//				for (int k = 0; k < sizeY; k++)
	//					if (mapGrid[i][j] != emptyTile || k >= 0)
	//						addTileSurface(Vector3(i, k, j + 1), TileSurfaceFace::Forward, meshTile);
	//			}
	//			
	//
	//			// generate floor and internal walls
	//			if(0)
	//			if(mapGrid[i][j] != emptyTile)
	//			{ // generate floors
	//				addTileSurface(Vector3(i, 0, j), TileSurfaceFace::Up, darkBrownDirtTile);
	//			}
	//			//else
	//			//{ // generate internal walls
	//			//	for(int k = 0; k < wallHeight; k++)
	//			//	{ // loop for mapSizeY for height
	//			//		if (i > 0)
	//			//			if (mapGrid[i - 1][j] != emptyTile)
	//			//				addTileSurface(Vector3(i, k, j), TileSurfaceFace::Left, lightLogsTile);
	//			//			
	//			//		if (i < sizeX - 1)
	//			//			if (mapGrid[i + 1][j] != emptyTile)
	//			//				addTileSurface(Vector3(i + 1, k, j), TileSurfaceFace::Right, lightLogsTile);
	//			//
	//			//		if (j > 0)
	//			//			if (mapGrid[i][j - 1] != emptyTile)
	//			//				addTileSurface(Vector3(i, k, j), TileSurfaceFace::Forward, lightLogsTile);
	//			//		if (j < sizeZ - 1)
	//			//			if (mapGrid[i][j + 1] != emptyTile)
	//			//				addTileSurface(Vector3(i, k, j + 1), TileSurfaceFace::Backward, lightLogsTile);
	//			//	}
	//			//
	//			//	// cap tops of walls
	//			//	addTileSurface(Vector3(i, wallHeight, j), TileSurfaceFace::Up, lightLogEndTile);
	//			//} // end generate internal walls
	//
	//			// generate ceiling
	//			if(0)
	//			addTileSurface(Vector3(i, roomSizeY, j), TileSurfaceFace::Down, mossyStoneTiles);
	//
	//		} // end generate floors and internal walls
	//
	//	mesh->addData(m_meshData);
	}

	void Level::placeRooms()
	{
		if(material == nullptr)
		{
			std::cout << "Level::placeRoom() material = null ptr" << std::endl;
			throw std::runtime_error("Level::placeRoom() material = null ptr");
			return;
		}

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				auto room = make_unique<Room>(m_random, Room::Size(8, 4, 5));

				room->transform.position.x = 9 * i;
				room->transform.position.z = 6 * j;
				room->material = material;
				room->generate();

				attachChild(std::move(room));
			}
		}

		//int skips = 0;
		//
		//for(int n = 0; n < 10; n++)
		//{
		//	u32 roomSizeX = m_random.getInt(3, 16);
		//	u32 roomSizeZ = m_random.getInt(3, 16);
		//
		//	int x = m_random.getInt(0, sizeX - 1 - roomSizeX);
		//	int z = m_random.getInt(0, sizeZ - 1 - roomSizeZ);
		//
		//	Rectangle newRoom(roomSizeX, roomSizeZ, x, z);
		//
		//	// if room intersects, break
		//	bool failed = false;
		//	for(const Room& room : m_rooms)
		//	{
		//		Rectangle a(room.transform.position.x, room.transform.position.y, room.size.x, room.size.y);
		//		if(newRoom.intersects(a))
		//		{
		//			failed = true;
		//			break;
		//		}
		//	}
		//
		//	// if room does not intersect push room
		//	if(failed == false)
		//	{
		//		m_rooms.push_back(newRoom);
		//	}
		//	else
		//	{
		//		// limit number of attempts
		//		skips++;
		//
		//		if (skips < 10)
		//			n--;
		//
		//		continue;
		//	}
		//
		//
		//}
		//
	}


} // end Dunjun

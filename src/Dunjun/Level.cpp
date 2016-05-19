
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
		if (material == nullptr)
		{
			std::cout << "Level::placeRoom() material = null ptr" << std::endl;
			return;
		}

		m_random.setSeed(142);

		const u32 levelSizeX = 10;
		const u32 levelSizeY = 3;
		const u32 levelSizeZ = 10;

		bool grid[levelSizeX][levelSizeY][levelSizeZ] = { false };

		const Room::Size roomSize(5, 4, 5);

		// repeat process for multiple paths form start
		for (int r = 0; r < 3; r++)
			{
			// number of steps to take
			const u32 numSteps = 8;

			// starting point
			u32 x = levelSizeX / 2;
			u32 z = levelSizeZ / 2;

			// variables to track previous position
			u32 prevPosX = x;
			u32 prevPosZ = z;

			// variable to keep track of last direction
			u32 direction = m_random.getInt(0, 3);

			// TODO: make branch generation its own function
			// random walk (decide where rooms will be)
			// do for each floor
			for (int i = 0; i < levelSizeY; i++)
			{
				// set starting room as true
				grid[x][i][z] = true;

				u32 skips = 0; // used to limit attempts to generate a room
				u32 stepsTaken = 0; // used to keep direction change from happening after only 1 step

				for (int j = 0; j < numSteps; j++)
				{
					if(skips > 10)
						break;

					// chance to change direction
					if(m_random.getInt(0, 2) == 2 && stepsTaken > 0)
						direction = m_random.getInt(0, 3);

					switch (direction)
					{
					case 0: x++; break; // right
					case 1: z++; break;	// back
					case 2: x--; break;	// left
					case 3: z--; break;	// forward
					}

					// check if a room is already there
					if (grid[x][i][z] == true)
					{
						j--;
						skips++;
						x = prevPosX;
						z = prevPosZ;
						continue;
					}

					// check array limits
					if (x < 0 || z < 0)
					{
						j--;
						skips++;
						x = prevPosX;
						z = prevPosZ;
						continue;
					}
					else if (x > levelSizeX - 1 || z > levelSizeZ - 1)
					{
						j--;
						skips++;
						x = prevPosX;
						z = prevPosZ;
						continue;
					}

					prevPosX = x;
					prevPosZ = z;

					grid[x][i][z] = true;

					stepsTaken++;

					// try for branching path
					if(m_random.getInt(0, 4) == 4)
					{
						u32 branchX = x;
						u32 branchZ = z;

						u32 branchPrevPosX = prevPosX;
						u32 branchPrevPosZ = prevPosZ;

						u32 branchDirection = 0;

						u32 branchSkips = 0;
						u32 branchStepsTaken = 0;

						// generate branching path
						// static cast to avoid decimals
						for (int j = 0; j <static_cast<u32>(numSteps / 2); j++)
						{
							if (branchSkips > 10)
								break;

							// chance to change direction
							if (m_random.getInt(0, 2) == 2 && branchStepsTaken > 0)
								branchDirection = m_random.getInt(0, 3);

							switch (branchDirection)
							{
							case 0: branchX++; break; // right
							case 1: branchZ++; break; // back
							case 2: branchX--; break; // left
							case 3: branchZ--; break; // forward
							}

							// check if a room is already there
							if (grid[branchX][i][branchZ] == true)
							{
								j--;
								branchSkips++;
								branchX = branchPrevPosX;
								branchZ = branchPrevPosZ;
								continue;
							}

							// check array limits
							if (branchX < 0 || branchZ < 0)
							{
								j--;
								branchSkips++;
								branchX = branchPrevPosX;
								branchZ = branchPrevPosZ;
								continue;
							}
							else if (branchX > levelSizeX - 1 || branchZ > levelSizeZ - 1)
							{
								j--;
								branchSkips++;
								branchX = branchPrevPosX;
								branchZ = branchPrevPosZ;
								continue;
							}

							branchPrevPosX = branchX;
							branchPrevPosZ = branchZ;

							grid[branchX][i][branchZ] = true;
						} // end generate branching path
					} // end try for branching path
				}
			} // end do for each floor
		} // end repeat process for multiple paths

		// generate rooms
		for (int i = 0; i < levelSizeY; i++)
		{
			for (int j = 0; j < levelSizeZ; j++)
			{
				for (int k = 0; k < levelSizeX; k++)
				{
					if (grid[k][i][j] == false)
						continue;

					auto room = make_unique<Room>(m_random, roomSize);

					const u32 gap = 1;

					room->transform.position.y = (roomSize.y + gap) * i;
					room->transform.position.x = (roomSize.x + gap) * j;
					room->transform.position.z = (roomSize.z + gap) * k;

					room->material = material;
					room->generate();

					attachChild(std::move(room));
					roomCount++;
				}
			}
		} // end generate rooms

		std::cout << "Level::placeRooms() <<<< There are " << roomCount << " rooms." << std::endl;

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
	} //; end place rooms


} // end Dunjun

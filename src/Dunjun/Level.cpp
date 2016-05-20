
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
			std::cout << "Level::placeRoom() <<<< material = null ptr" << std::endl;
			return;
		}

		if(0)
			m_random.setSeed(142);

		// set room range
		// rooms can still end up small (30 - 50) with min rooms set
		u32 minRooms = 999999; // if min is not met it tries again but breaks if it fails too much
		u32 maxRooms = 999999; // hard cap to number of rooms

		const u32 levelSizeX = 30;
		const u32 levelSizeY = 5;
		const u32 levelSizeZ = 30;

		// starting point
		u32 startX = levelSizeX / 2;
		u32 startY = levelSizeY / 2;
		u32 startZ = levelSizeZ / 2;

		bool grid[levelSizeX][levelSizeY][levelSizeZ] = { false };

		const Room::Size roomSize(5, 4, 5);

		// number of steps taken since last turn
		const u32 numSteps = 6;

		// used to limit attempts to generate a room
		u32 breaks = 0;

		// TODO: make branch generation its own function
		// random walk (decide where rooms will be)
		for (int i = 0; i < 1; i++)
		{
			// check minimum room count has been met
			if (roomCount < minRooms)
			{
				i--; // run again
			}
			// escape if caught in endless loop
			if (breaks > 30 || roomCount >= maxRooms)
			{
				break;
			}

			// used to limit attempts to generate a room
			u32 skips = 0;

			u32 stepsTaken = 0; // used to keep direction change from happening after only 1 step

			// current location
			u32 stepX = startX;
			u32 stepY = startY;
			u32 stepZ = startZ;

			// variables to track previous position
			u32 prevPosX = stepX;
			u32 prevPosY = stepY;
			u32 prevPosZ = stepZ;

			// set starting room as true
			if(grid[stepX][stepY][stepZ] == false)
			{
				grid[stepX][stepY][stepZ] = true;
				roomCount++;
			}

			// variable to keep track of last direction
			u32 direction = m_random.getInt(0, 3);

			for (int j = 0; j < numSteps; j++)
			{
				if(skips > 10 || roomCount >= maxRooms)
				{
					breaks++;
					break;
				}

				u32 rand = m_random.getInt(0, 100);

				// chance to change direction
				if (rand <= 10 && stepsTaken > 1)
				{
					direction = m_random.getInt(4, 5); // up or down
					stepsTaken = 1; // set to one to make multiple y changes less common
					j -= 2; // add a few more steps for new floors
							// doesn't go below zero since you to take more than one step to get here
				}
				else if(rand <= 40 && stepsTaken > 0)
				{
					direction = m_random.getInt(0, 3); // left/right/back/forward
					stepsTaken = 0; // set to zero to prevent turn after 1 step
				}
				
				switch (direction)
				{
				case 0: stepX++; break; // right
				case 1: stepZ++; break;	// back
				case 2: stepX--; break;	// left
				case 3: stepZ--; break;	// forward
				case 4: stepY++; break;	// up
				case 5: stepY--; break;	// down
				}

				// check if a room is already there and undo step change if it is
				if (grid[stepX][stepY][stepZ] == true)
				{
					j--;
					skips++;
					stepX = prevPosX;
					stepY = prevPosY;
					stepZ = prevPosZ;
					continue;
				}

				// check array limits and undo step change if out of bounds
				if (stepX < 0 || stepY < 0 || stepZ < 0)
				{
					j--;
					skips++;
					stepX = prevPosX;
					stepY = prevPosY;
					stepZ = prevPosZ;
					continue;
				}
				else if (stepX > levelSizeX - 1 || stepY > levelSizeY - 1 || stepZ > levelSizeZ - 1)
				{
					j--;
					skips++;
					stepX = prevPosX;
					stepY = prevPosY;
					stepZ = prevPosZ;
					continue;
				}

				// chance to change start location
				if(rand < 90)
				{
					startX = stepX;
					startY = stepY;
					startZ = stepZ;
				}

				// set prevpos for next loop
				prevPosX = stepX;
				prevPosY = stepY;
				prevPosZ = stepZ;

				grid[stepX][stepY][stepZ] = true;

				roomCount++;
				stepsTaken++;
				skips = 0;

				// try for branching path
				if(m_random.getInt(0, 4) == 4)
				{
					// these act the same as the main path versions
					u32 branchNumSteps = 4;

					u32 branchX = stepX;
					u32 branchY = stepY;
					u32 branchZ = stepZ;

					u32 branchPrevPosX = prevPosX;
					u32 branchPrevPosY = prevPosY;
					u32 branchPrevPosZ = prevPosZ;

					u32 branchDirection = 0;

					u32 branchSkips = 0;
					u32 branchStepsTaken = 0;

					// generate branching path
					// static cast to avoid decimals
					for (int j = 0; j < branchNumSteps; j++)
					{
						if (branchSkips > 10 || roomCount >= maxRooms)
						{
							breaks++;
							break;
						}

						u32 branchRand = m_random.getInt(1, 100);

						// chance to change direction
						if (branchRand <= 10 && branchStepsTaken > 1)
						{
							branchDirection = m_random.getInt(4, 5); // up or down
							branchStepsTaken = 1;
							j -= 2;
						}
						else if (branchRand <= 40 && stepsTaken > 0)
						{
							branchDirection = m_random.getInt(0, 3); // left/right/back/forward
							branchStepsTaken = 0;
						}

						switch (branchDirection)
						{
						case 0: branchX++; break; // right
						case 1: branchZ++; break; // back
						case 2: branchX--; break; // left
						case 3: branchZ--; break; // forward
						case 4: branchY++; break; // up
						case 5: branchY--; break; // down
						}

						// check if a room is already there
						if (grid[branchX][branchY][branchZ] == true)
						{
							j--;
							branchSkips++;
							branchX = branchPrevPosX;
							branchY = branchPrevPosY;
							branchZ = branchPrevPosZ;
							continue;
						}

						// check array limits
						if (branchX < 0 || branchY < 0 || branchZ < 0)
						{
							j--;
							branchSkips++;
							branchX = branchPrevPosX;
							branchY = branchPrevPosY;
							branchZ = branchPrevPosZ;
							continue;
						}
						else if (branchX > levelSizeX - 1 || branchY > levelSizeY - 1 || branchZ > levelSizeZ - 1)
						{
							j--;
							branchSkips++;
							branchX = branchPrevPosX;
							branchY = branchPrevPosY;
							branchZ = branchPrevPosZ;
							continue;
						}

						branchPrevPosX = branchX;
						branchPrevPosY = branchY;
						branchPrevPosZ = branchZ;

						grid[branchX][branchY][branchZ] = true;

						roomCount++;
						branchNumSteps++;
						skips = 0;
					} // end generate branching path
				} // end try for branching path
			}

			std::cout << "Level::placeRooms() <<<< The number of skips this loop was  " << skips << "." << std::endl;
			std::cout << "Level::placeRooms() <<<< The number of breaks this loop was  " << breaks << "." << std::endl;

		} // end do for each floor

		// where to place the lowest point of the level
		f32 levelBase = -1;

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

					room->transform.position.y = (roomSize.y + gap) * i + levelBase * (roomSize.y + gap);
					room->transform.position.x = (roomSize.x + gap) * j;
					room->transform.position.z = (roomSize.z + gap) * k;

					room->material = material;

					room->generate();

					attachChild(std::move(room));
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


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
		placeRooms();
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

		const u32 levelSizeX = 60;
		const u32 levelSizeY = 5;
		const u32 levelSizeZ = 60;

		// set room count range
		// on average generates ~400 rooms in a 60*5*60 area with very high minRoom value
		u32 minRooms = 10; // min to range of number of rooms
		u32 maxRooms = 800; // max to range of number of rooms

		// calculate number of rooms possible
		// if min is not met it tries again but the loop will break if it fails too many times
		minRooms = m_random.getInt(minRooms, maxRooms);

		bool grid[levelSizeX][levelSizeY][levelSizeZ] = { false };

		const Room::Size roomSize(5, 4, 5);

		// number of steps take in each loop
		const u32 numSteps = 10;

		// starting point
		u32 startX = levelSizeX / 2;
		u32 startY = levelSizeY / 2;
		u32 startZ = levelSizeZ / 2;

		// track farthest room +/- on any given axis
		Vector3 mostPosX(startX, startY, startZ);
		Vector3 mostNegX(startX, startY, startZ);
		Vector3 mostPosY(startX, startY, startZ);
		Vector3 mostNegY(startX, startY, startZ);
		Vector3 mostPosZ(startX, startY, startZ);
		Vector3 mostNegZ(startX, startY, startZ);

		// cycle which of the far rooms to switch to if stuck
		u32 farRoomCycle = 0;

		// chance to turn to an axis, total 0~1 float value for chance to turn
		// cumulative and must be checked in the same order as added together
		f32 chanceTurnX = 0.15f;
		f32 chanceTurnY = 0.08f + chanceTurnX;
		f32 chanceTurnZ = 0.15f + chanceTurnY;

		// used to limit attempts to generate a room
		u32 breaks = 0;

		// TODO: make branch generation its own function
		// random walk (decide where rooms will be)
		while (roomCount < minRooms)
		{
			// escape if caught in endless loop
			if (breaks >= 32)
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

			// main path
			for (int j = 0; j < numSteps; j++)
			{
				if(roomCount >= minRooms)
					break;

				if(skips > 10)
				{
					// move start to a room as far from center as possible
					switch (farRoomCycle)
					{
						case 0: startX = mostPosX.x ; startY = mostPosX.y ; startZ = mostPosX.z ; farRoomCycle = 1; break; // Positive X
						case 1: startX = mostNegX.x ; startY = mostNegX.y ; startZ = mostNegX.z ; farRoomCycle = 2; break; // Negative X
						case 2: startX = mostPosZ.x ; startY = mostPosZ.y ; startZ = mostPosZ.z ; farRoomCycle = 3; break; // Positive Z
						case 3: startX = mostNegZ.x ; startY = mostNegZ.y ; startZ = mostNegZ.z ; farRoomCycle = 4; break; // Negative Z
						case 4: startX = mostPosY.x ; startY = mostPosY.y ; startZ = mostPosY.z ; farRoomCycle = 5; break; // Positive Y
						case 5: startX = mostNegY.x ; startY = mostNegY.y ; startZ = mostNegY.z ; farRoomCycle = 0; break; // Negative Y
					}

					// move current location to new start location
					stepX = startX;
					stepY = startY;
					stepZ = startZ;
					prevPosX = startX;
					prevPosY = startY;
					prevPosZ = startZ;

					breaks++;
					break;
				}

				f32 rand = m_random.getFloat(0, 1);

				// chance to change direction
				if (rand <= chanceTurnX && stepsTaken > 0)
				{
					direction = m_random.getInt(0, 1); // left/right/back/forward
					stepsTaken = 0; // set to zero to prevent turn after 1 step
				}
				else if (rand <= chanceTurnY && stepsTaken > 1)
				{
					direction = m_random.getInt(4, 5); // up or down
					stepsTaken = 0; // set to one to make multiple y changes less common
					j -= 2; // add an extra step for new floors
							// doesn't go below zero since you to take more than one step to get here
				} 
				else if (rand <= chanceTurnZ && stepsTaken > 0)
				{
					direction = m_random.getInt(2, 3); // left/right/back/forward
					stepsTaken = 0; // set to zero to prevent turn after 1 step
				}
				
				switch (direction)
				{
				case 0: stepX++; break; // right
				case 1: stepX--; break;	// left
				case 2: stepZ++; break;	// back
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
				if (rand <= 0.8f)
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

				// check if room is farthest in any direction
				Vector3 currentPos(stepX, stepY, stepZ);

				if		(stepX > mostPosX.x) { mostPosX = currentPos; }
				else if (stepX < mostNegX.x) { mostNegX = currentPos; }

				if		(stepY > mostPosY.y) { mostPosY = currentPos; }
				else if (stepY < mostNegY.y) { mostNegY = currentPos; }

				if		(stepZ > mostPosZ.z) { mostPosZ = currentPos; }
				else if (stepZ < mostNegZ.z) { mostNegZ = currentPos; }

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
						if (branchSkips > 10 || roomCount >= minRooms)
						{
							break;
						}

						f32 branchRand = m_random.getFloat(0, 1);

						// chance to change direction
						if (branchRand <= chanceTurnX && stepsTaken > 0)
						{
							branchDirection = m_random.getInt(0, 1); // left/right/back/forward
							branchStepsTaken = 0;
						}
						else if (branchRand <= chanceTurnY && branchStepsTaken > 1)
						{
							branchDirection = m_random.getInt(4, 5); // up or down
							branchStepsTaken = 0;
							j -= 2;
						}
						else if (branchRand <= chanceTurnZ && stepsTaken > 0)
						{
							branchDirection = m_random.getInt(2, 3); // left/right/back/forward
							branchStepsTaken = 0;
						}

						switch (branchDirection)
						{
						case 0: branchX++; break; // right
						case 1: branchX--; break; // left
						case 2: branchZ++; break; // back
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

						// check if room is farthest in any direction
						Vector3 currentPos(branchX, branchY, branchZ);

						if		(branchX > mostPosX.x) { mostPosX = currentPos; }
						else if (branchX < mostNegX.x) { mostNegX = currentPos; }

						if		(branchY > mostPosY.y) { mostPosY = currentPos; }
						else if (branchY < mostNegY.y) { mostNegY = currentPos; }

						if		(branchZ > mostPosZ.z) { mostPosZ = currentPos; }
						else if (branchZ < mostNegZ.z) { mostNegZ = currentPos; }

						roomCount++;
						branchNumSteps++;
						skips = 0;
					} // end generate branching path
				} // end try for branching path
			} // end main path

			//std::cout << "Level::placeRooms() <<<< The number of skips this loop was  " << skips << "." << std::endl;
			//std::cout << "Level::placeRooms() <<<< The number of breaks this loop was  " << breaks << "." << std::endl;

		} // end random walk (decide where rooms will be)

		// where to place the lowest point of the level in world
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

		std::cout << "Level::placeRooms() <<<< There are " << roomCount << " of " << minRooms << " attemped rooms." << std::endl;

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

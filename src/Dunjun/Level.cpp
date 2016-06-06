
#include <Dunjun/Level.hpp>

namespace Dunjun
{
	Level::Level()
		: m_random()
		, rooms()
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
			std::cout << "Level::placeRoom() <<<< material == nullptr" << std::endl;
			return;
		}
		else if (material->shaders == nullptr)
		{
			std::cout << "Level::placeRoom() <<<< shaders == nullptr" << std::endl;
			return;
		}

		if(0)
			m_random.setSeed(142);

#define DUNJUN_DEBUG_LEVEL // TODO: remove this for a preset test level
#ifdef DUNJUN_DEBUG_LEVEL
		// TODO: FIX: player doesn't line up with even/odd numbers correctly
		const u32 levelSizeX = 5;
		const u32 levelSizeY = 10;
		const u32 levelSizeZ = 5;
#else
		// TODO: FIX: player doesn't line up with even/odd numbers correctly
		const u32 levelSizeX = 50;
		const u32 levelSizeY = 8;
		const u32 levelSizeZ = 50;
#endif

		//this large an area causes overflow when regenerating rooms
		//const u32 levelSizeX = 305;
		//const u32 levelSizeY = 11;
		//const u32 levelSizeZ = 305;

		// center of the level : used to center level in the world
		f32 levelCenterOffsetX = levelSizeX / 2.0f;
		f32 levelCenterOffsetY = levelSizeY / 2.0f;
		f32 levelCenterOffsetZ = levelSizeZ / 2.0f;

		// set room count range
		// on average generates ~400 rooms in a 60*5*60 area with very high minRoom value
		u32 minRooms = 1; // min to range of number of rooms
		u32 maxRooms = 1000; // max to range of number of rooms

		// calculate number of rooms possible
		// if min is not met it tries again but the loop will break if it fails too many times
		minRooms = m_random.getInt(minRooms, maxRooms);

		bool grid[levelSizeX][levelSizeY][levelSizeZ] = {{{  }}};

		// bools for testing
		bool testEnableWalls = true;
		bool testEnableDoors = false;

// initialize grid as true for testing
bool testEnableRandomWalk = true;
#ifdef DUNJUN_DEBUG_LEVEL
		for (int i = 0; i < levelSizeX; i++)
			for (int j = 0; j < levelSizeY; j++)
				for (int k = 0; k < levelSizeZ; k++)
				{
					if(j > levelCenterOffsetY - 1)
						grid[i][j][k] = true; // levelsize = {5, 10, 5} for a 125 room cube
				}
		testEnableRandomWalk = false;
#endif

		const Room::Size roomSize(5, 4, 5);

		// number of steps take in each loop
		const u32 numSteps = 10;

		// starting point
		u32 startX = levelCenterOffsetX;
		u32 startY = levelCenterOffsetY;
		u32 startZ = levelCenterOffsetZ;

		// TODO: make Size class public data holder
		// track farthest room +/- on any given axis
		Room::Size mostPosX(startX, startY, startZ);
		Room::Size mostNegX(startX, startY, startZ);
		Room::Size mostPosY(startX, startY, startZ);
		Room::Size mostNegY(startX, startY, startZ);
		Room::Size mostPosZ(startX, startY, startZ);
		Room::Size mostNegZ(startX, startY, startZ);

		// cycle which of the far rooms to switch to if stuck
		u32 farRoomCycle = 0;

		// chance to turn to an axis, total 0~1 float value for chance to turn
		// cumulative and must be checked in the same order as added together
		f32 chanceTurnX = 0.15f;
		f32 chanceTurnY = 0.10f + chanceTurnX;
		f32 chanceTurnZ = 0.15f + chanceTurnY;

		// used to limit attempts to generate a room
		u32 breaks = 0;

		// TODO: make branch generation its own function
		// random walk (decide where rooms will be)
		// generate rooms until minRooms is met or break too many times
		if(testEnableRandomWalk)
		while (roomCount < minRooms)
		{
			// escape if caught in endless loop
			if (breaks >= 32)
				break;

			// used to limit attempts per room to generate a room
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
				roomCount++; // ++ is why if() is needed
			}

			// variable to keep track of last direction
			u32 direction = m_random.getInt(0, 3);

			// main path
			for (int j = 0; j < numSteps; j++)
			{
				if(roomCount >= minRooms)
					break;

				// change start point if stuck
				if(skips > 10)
				{
					// move start to a room as far from center as possible
					switch (farRoomCycle)
					{
						case 0: startX = mostPosX.x; startY = mostPosX.y; startZ = mostPosX.z; farRoomCycle = 1; break; // Positive X
						case 1: startX = mostNegX.x; startY = mostNegX.y; startZ = mostNegX.z; farRoomCycle = 2; break; // Negative X
						case 2: startX = mostPosZ.x; startY = mostPosZ.y; startZ = mostPosZ.z; farRoomCycle = 3; break; // Positive Z
						case 3: startX = mostNegZ.x; startY = mostNegZ.y; startZ = mostNegZ.z; farRoomCycle = 4; break; // Negative Z
						case 4: startX = mostPosY.x; startY = mostPosY.y; startZ = mostPosY.z; farRoomCycle = 5; break; // Positive Y
						case 5: startX = mostNegY.x; startY = mostNegY.y; startZ = mostNegY.z; farRoomCycle = 0; break; // Negative Y
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
					direction = m_random.getInt(0, 1); // left/right
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
					direction = m_random.getInt(2, 3); // back/forward
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

				// chance to change start location to current location
				if (rand >= 0.8f)
				{
					startX = stepX;
					startY = stepY;
					startZ = stepZ;
				}

				// set prevPos for next loop
				prevPosX = stepX;
				prevPosY = stepY;
				prevPosZ = stepZ;

				grid[stepX][stepY][stepZ] = true;

				// check if room is farthest in any direction
				Room::Size currentPos(stepX, stepY, stepZ);

				if (stepX > mostPosX.x) { mostPosX = currentPos; }
				else if (stepX < mostNegX.x) { mostNegX = currentPos; }
				
				if (stepY > mostPosY.y) { mostPosY = currentPos; }
				else if (stepY < mostNegY.y) { mostNegY = currentPos; }
				
				if (stepZ > mostPosZ.z) { mostPosZ = currentPos; }
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

						branchPrevPosX = branchX;
						branchPrevPosY = branchY;
						branchPrevPosZ = branchZ;

						grid[branchX][branchY][branchZ] = true;

						// check if room is farthest in any direction
						Room::Size currentPos(branchX, branchY, branchZ);

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

		} // end random walk (decide where rooms will be)


		// generate rooms
		for (int i = 0; i < levelSizeX; i++)
		{
			for (int j = 0; j < levelSizeY; j++)
			{
				for (int k = 0; k < levelSizeZ; k++)
				{
					if (grid[i][j][k] == false)
						continue;

					auto room = make_unique<Room>(m_random, roomSize);

					// debug variables
					const u32 gap = 0;

					// cast levelCenterY as u32 so player doesn't float
					room->transform.position.x = (roomSize.x + gap) * i - levelCenterOffsetX  * (roomSize.x + gap);
					room->transform.position.y = (roomSize.y + gap) * j - levelCenterOffsetY  * (roomSize.y + gap);
					room->transform.position.z = (roomSize.z + gap) * k - levelCenterOffsetZ  * (roomSize.z + gap);

					room->material = material;

					bool northWall = false;
					bool westWall = false;
					bool southWall = false;
					bool eastWall = false;

					bool floor = false;
					bool ceiling = false;

					bool northDoor = false;
					bool westDoor = false;
					bool southDoor = false;
					bool eastDoor = false;

					// check whether to add walls
					if(testEnableWalls)
					{
						// using switches to keep within array limits
						switch (i)
						{
						case 0:
							if (grid[i + 1][j][k] == 0) { eastWall = true; }
							westWall = true; break;
						case (levelSizeX - 1):
							if (grid[i - 1][j][k] == 0) { westWall = true; }
							eastWall = true; break;
						default: 
							if (grid[i - 1][j][k] == 0) { westWall = true; }
							if (grid[i + 1][j][k] == 0) { eastWall = true; }
							break;
						};

						// TODO: move this to grid generation so only up/down movements have openings
						// Y level
						switch (j)
						{
						case 0:
							if (grid[i][j + 1][k] == 0) { ceiling = true; }
							floor = true; break;
						case (levelSizeY - 1) :
							if (grid[i][j - 1][k] == 0) { floor = true; }
							ceiling = true; break;
						default:
							if (grid[i][j + 1][k] == 0) { ceiling = true; }
							if (grid[i][j - 1][k] == 0) { floor = true; }
							break;
						};

						switch (k)
						{
						case 0:
							if (grid[i][j][k + 1] == 0) { southWall = true; }
							northWall = true; break;
						case (levelSizeZ - 1):
							if (grid[i][j][k - 1] == 0) { northWall = true; }
							southWall = true; break;
						default:
							if (grid[i][j][k + 1] == 0) { southWall = true; }
							if (grid[i][j][k - 1] == 0) { northWall = true; }
							break;
						};
					} // end check wether to add walls


					// check wether to add door KEEP SEPERATE FROM CHECK WALLS
					if(testEnableDoors)
					{
						// using switches to keep within array limits
						switch (i)
						{
						case 0:
							if (grid[i + 1][j][k] != 0) { eastDoor = true; }
							westDoor = false; break;
						case (levelSizeX - 1):
							if (grid[i - 1][j][k] != 0) { westDoor = true; }
							eastDoor = false; break;
						default:
							if (grid[i - 1][j][k] != 0) { westDoor = true; }
							if (grid[i + 1][j][k] != 0) { eastDoor = true; }
							break;
						};

						switch (k)
						{
						case 0:
							if (grid[i][j][k + 1] != 0) { southDoor = true; }
							northDoor = false; break;
						case (levelSizeZ - 1):
							if (grid[i][j][k - 1] != 0) { northDoor = true; }
							southDoor = false; break;
						default:
							if (grid[i][j][k + 1] != 0) { southDoor = true; }
							if (grid[i][j][k - 1] != 0) { northDoor = true; }
							break;
						};
					} // end check wether to add door

					room->setDoors(northDoor, westDoor, southDoor, eastDoor);
					
					room->generate(northWall, westWall, southWall, eastWall, floor, ceiling);

					rooms.push_back(room.get());

					attachChild(std::move(room));
				}
			}
		} // end generate rooms



		std::cout << "Level::placeRooms() <<<< There are " << roomCount << " of " << minRooms << " attemped rooms." << std::endl;

	} // end place rooms




			//		// if version of wall switch in placeRooms() function
			//		// way too many comparisons
			//		if(i == 0 || j == 0 || k == 0)
			//		{
			//			if (i == 0)
			//			{
			//				if (grid[i + 1][j][k] == 0) { eastWall = true; }
			//				westWall = true;
			//			}
			//			if (j == 0)
			//			{
			//				if (grid[i][j + 1][k] == 0) { ceiling = true; }
			//				floor = true;
			//			}
			//			if (k == 0)
			//			{
			//				if (grid[i][j][k + 1] == 0) { southWall = true; }
			//				northWall = true;
			//			}
			//		}
			//		if (i == levelSizeX - 1 || j == levelSizeY - 1 || k == levelSizeZ - 1)
			//		{
			//			if (i == levelSizeX - 1)
			//			{
			//				if (grid[i - 1][j][k] == 0) { westWall = true; }
			//				eastWall = true;
			//			}
			//			if (j == levelSizeY - 1)
			//			{
			//				if (grid[i][j - 1][k] == 0) { floor = true; }
			//				ceiling = true;
			//			}
			//			if (k == levelSizeZ - 1)
			//			{
			//				if (grid[i][j][k - 1] == 0) { northWall = true; }
			//				southWall = true;
			//			}
			//		}
			//		if((i > 0 && i < levelSizeX - 1) || (j > 0 && j < levelSizeY - 1) || (k > 0 && k < levelSizeZ - 1))
			//		{
			//			if (grid[i - 1][j][k] == 0) { westWall = true; }
			//			if (grid[i + 1][j][k] == 0) { eastWall = true; }
			//			if (grid[i][j + 1][k] == 0) { ceiling = true; }
			//			if (grid[i][j - 1][k] == 0) { floor = true; }
			//			if (grid[i][j][k + 1] == 0) { southWall = true; }
			//			if (grid[i][j][k - 1] == 0) { northWall = true; }
			//		}


} // end Dunjun

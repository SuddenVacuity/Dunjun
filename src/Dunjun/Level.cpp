
#include <Dunjun/Level.hpp>


namespace Dunjun
{

	Level::Level()
	{
	}

	Level::~Level()
	{
		delete mesh;
	}

	void Level::generate()
	{
		if (!mesh)
			mesh = new Mesh();

		u32 roomSizeY = 4;

		// initialize mapGrid
		mapGrid = std::vector<std::vector<TileId>>(sizeX, std::vector<TileId>(sizeZ));

		m_random.setSeed(1);

		static const TileId emptyTile = { 0xFFFFFFFF, 0xFFFFFFFF };

		// location of texture in image map
		TileId lightWoodTile = { 0, 11 };
		TileId meshTile = { 0, 13 };
		TileId lightLogsTile = { 1, 11 };
		TileId lightLogEndTile = { 2, 11 };
		TileId stoneTile = { 2, 15 };
		TileId darkBrownDirtTile = { 3, 14 };
		
		Level::RandomTileSet mossyStoneTiles;
		for (u32 i = 1; i <= 2; i++)
			mossyStoneTiles.emplace_back(Level::TileId{ i, 15 });

		// initialize mapGrid to build room from
		for (int i = 0; i < sizeX; i++)
		{
			for (int j = 0; j < sizeZ; j++)
					mapGrid[i][j] = emptyTile;
		}

		placeRooms();

		// generate rooms
		for(const auto& room : m_rooms)
		{
			for (int i = room.x; i < room.x + room.width; i++)
			{
				for (int j = room.y; j < room.y + room.height; j++)
				{
					mapGrid[i][j] = lightWoodTile;
				}
			}
		}

		// generate mesh
		for (int i = 0; i < sizeX; i++)
			for (int j = 0; j < sizeZ; j++)
			{ 
				// generate edge walls
				if (i == 0)
				{ // left edge
					for (int k = 0; k < sizeY; k++)
						if(mapGrid[i][j] != emptyTile || k >= 0) // only add a wall if there's a floor or if is higher than obstacle
							addTileSurface(Vector3(i, k, j), TileSurfaceFace::Right, meshTile);
				}
				else if (i == sizeX - 1)
				{ // right edge
					for (int k = 0; k < sizeY; k++)
						if (mapGrid[i][j] != emptyTile || k >= 0)
							addTileSurface(Vector3(i + 1, k, j), TileSurfaceFace::Left, meshTile);
				}
				if (j == 0)
				{ // back edge
					for (int k = 0; k < sizeY; k++)
						if (mapGrid[i][j] != emptyTile || k >= 0)
							addTileSurface(Vector3(i, k, j), TileSurfaceFace::Backward, meshTile);
				}
				else if (j == sizeZ - 1)
				{ // front edge
					for (int k = 0; k < sizeY; k++)
						if (mapGrid[i][j] != emptyTile || k >= 0)
							addTileSurface(Vector3(i, k, j + 1), TileSurfaceFace::Forward, meshTile);
				}
				

				// generate floor and internal walls
				if(0)
				if(mapGrid[i][j] != emptyTile)
				{ // generate floors
					addTileSurface(Vector3(i, 0, j), TileSurfaceFace::Up, darkBrownDirtTile);
				}
				//else
				//{ // generate internal walls
				//	for(int k = 0; k < wallHeight; k++)
				//	{ // loop for mapSizeY for height
				//		if (i > 0)
				//			if (mapGrid[i - 1][j] != emptyTile)
				//				addTileSurface(Vector3(i, k, j), TileSurfaceFace::Left, lightLogsTile);
				//			
				//		if (i < sizeX - 1)
				//			if (mapGrid[i + 1][j] != emptyTile)
				//				addTileSurface(Vector3(i + 1, k, j), TileSurfaceFace::Right, lightLogsTile);
				//
				//		if (j > 0)
				//			if (mapGrid[i][j - 1] != emptyTile)
				//				addTileSurface(Vector3(i, k, j), TileSurfaceFace::Forward, lightLogsTile);
				//		if (j < sizeZ - 1)
				//			if (mapGrid[i][j + 1] != emptyTile)
				//				addTileSurface(Vector3(i, k, j + 1), TileSurfaceFace::Backward, lightLogsTile);
				//	}
				//
				//	// cap tops of walls
				//	addTileSurface(Vector3(i, wallHeight, j), TileSurfaceFace::Up, lightLogEndTile);
				//} // end generate internal walls

				// generate ceiling
				if(0)
				addTileSurface(Vector3(i, roomSizeY, j), TileSurfaceFace::Down, mossyStoneTiles);

			} // end generate floors and internal walls

		mesh->addData(m_meshData);
	}

	void Level::placeRooms()
	{
		int skips = 0;

		for(int n = 0; n < 10; n++)
		{
			u32 roomSizeX = m_random.getInt(3, 16);
			u32 roomSizeZ = m_random.getInt(3, 16);

			int x = m_random.getInt(0, sizeX - 1 - roomSizeX);
			int z = m_random.getInt(0, sizeZ - 1 - roomSizeZ);

			Rectangle newRoom(roomSizeX, roomSizeZ, x, z);

			// if room intersects, break
			bool failed = false;
			for(const Rectangle& room : m_rooms)
			{
				if(newRoom.intersects(room))
				{
					failed = true;
					break;
				}
			}

			// if room does not intersect push room
			if(failed == false)
			{
				m_rooms.push_back(newRoom);
			}
			else
			{
				// limit number of attempts
				skips++;

				if (skips < 10)
					n--;

				continue;
			}


		}
	}

	// create a 4 vertex tile in the at position in the direction of TileSurfaceFace with texPos being the position in texture map for the texture
	void Level::addTileSurface(const Vector3& position, TileSurfaceFace face, const TileId& texPos)
	{
		if (!mesh)
			mesh = new Mesh();

		// size of image map
		const f32 tileWidth = 1.0f / 16;
		const f32 tileHeight = 1.0f / 16;

		// used to offset indices
		size_t index = m_meshData.vertices.size();
		
		// predefine this to save space and allow value swapping
		Vector2 vertexTexCoord[4] = {{(texPos.x + 1)*tileWidth, (texPos.y + 1)*tileHeight },
									{ (texPos.x + 0)*tileWidth, (texPos.y + 1)*tileHeight },
									{ (texPos.x + 1)*tileWidth, (texPos.y + 0)*tileHeight },
									{ (texPos.x + 0)*tileWidth, (texPos.y + 0)*tileHeight }};

		// predefine this to save space and allow value swapping
		Color vertexColor[4] = {{ 0xFF, 0xFF, 0xFF, 0xFF },
								{ 0xFF, 0xFF, 0xFF, 0xFF },
								{ 0xFF, 0xFF, 0xFF, 0xFF },
								{ 0xFF, 0xFF, 0xFF, 0xFF }};

		// choose draw order based on direction
		if ((u32)face % 2 == 0)
			m_meshData.addFace(index, 0, 1, 2).addFace(index, 1, 3, 2); // hypotenuse goes down and right
		else // (u32)face % 2 == 1
		{
			// flip draw order
			m_meshData.addFace(index, 0, 2, 3).addFace(index, 0, 3, 1); // hypotenuse goes down and right

			// make temp variables and flip texture/colors to match draw order
			Vector2 v[4] = { vertexTexCoord[1], vertexTexCoord[0], vertexTexCoord[3], vertexTexCoord[2] };
			Color c[4] = { vertexColor[1], vertexColor[0], vertexColor[3], vertexColor[2] };

			// apply the flipped coords and colors
			for(int i = 0; i < 4; i++)
			{
				vertexTexCoord[i] = v[i];
				vertexColor[i] = c[i];
			}
		}

		// apply position portions based on direction
		// right/left vertices
		if ((u32)face < 2)
		{
			//vertex data				position (x, y, z}									texture coords		vertex colors
			 m_meshData.vertices.append({ position.x, position.y + 1.0f, position.z + 1.0f }, vertexTexCoord[0], vertexColor[0] );
			 m_meshData.vertices.append({ position.x, position.y + 1.0f, position.z + 0.0f }, vertexTexCoord[1], vertexColor[1] );
			 m_meshData.vertices.append({ position.x, position.y + 0.0f, position.z + 1.0f }, vertexTexCoord[2], vertexColor[2] );
			 m_meshData.vertices.append({ position.x, position.y + 0.0f, position.z + 0.0f }, vertexTexCoord[3], vertexColor[3] );
		}

		// up/down vertices
		else if ((u32)face < 4)
		{
			//vertex data				position (x, y, z}									texture coords		vertex colors
			 m_meshData.vertices.append({ position.x + 1.0f, position.y, position.z + 0.0f }, vertexTexCoord[0], vertexColor[0]);
			 m_meshData.vertices.append({ position.x + 0.0f, position.y, position.z + 0.0f }, vertexTexCoord[1], vertexColor[1]);
			 m_meshData.vertices.append({ position.x + 1.0f, position.y, position.z + 1.0f }, vertexTexCoord[2], vertexColor[2]);
			 m_meshData.vertices.append({ position.x + 0.0f, position.y, position.z + 1.0f }, vertexTexCoord[3], vertexColor[3]);
		}

		// forward/backward vertices
		else if ((u32)face< 6)
		{
			//vertex data				position (x, y, z}									texture coords		vertex colors
			 m_meshData.vertices.append({ position.x + 1.0f, position.y + 1.0f, position.z }, vertexTexCoord[0], vertexColor[0]);
			 m_meshData.vertices.append({ position.x + 0.0f, position.y + 1.0f, position.z }, vertexTexCoord[1], vertexColor[1]);
			 m_meshData.vertices.append({ position.x + 1.0f, position.y + 0.0f, position.z }, vertexTexCoord[2], vertexColor[2]);
			 m_meshData.vertices.append({ position.x + 0.0f, position.y + 0.0f, position.z }, vertexTexCoord[3], vertexColor[3]);
		}
		else
			std::runtime_error(std::string("Invalid tile direction in function Level::addTileSurface()"));

	}

	void Level::addTileSurface(const Vector3& position, TileSurfaceFace face, const RandomTileSet& randomTilePosSet)
	{
		size_t length = randomTilePosSet.size();
		TileId tilePos = randomTilePosSet[m_random.getInt(0, length - 1)];
		addTileSurface(position, face, tilePos);
	}


} // end Dunjun

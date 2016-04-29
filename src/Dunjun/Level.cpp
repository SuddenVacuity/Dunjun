
#include <Dunjun/Level.hpp>

#include <random>

namespace Dunjun
{

	Level::Level()
	{
	}

	void Level::generate()
	{
		if (!mesh)
			mesh = new Mesh();

		u32 roomSizeX = 10;
		u32 roomSizeZ = 10;
		u32 roomSizeY = 4;
		u32 wallHeight = 1;

		// initialize mapGrid
		mapGrid = std::vector<std::vector<TileId>>(roomSizeX, std::vector<TileId>(roomSizeZ));

		static const TileId emptyTile = {0xFFFFFFFF, 0xFFFFFFFF};

		std::random_device seed; // random seed
		std::mt19937 randNum(1); // random number generator
		std::uniform_int_distribution<int> randRange(0, 100); // range of random number

		// location of texture in image map
		Level::TileId darkBrownDirtTile = { 3, 14 };
		Level::TileId lightLogsTile = { 1, 11 };
		Level::TileId lightLogEndTile = { 2, 11 };
		Level::TileId stoneTile = { 2, 15 };
		
		Level::RandomTileSet mossyStoneTiles;
		for (u32 i = 1; i <= 2; i++)
			mossyStoneTiles.emplace_back(Level::TileId{ i, 15 });

		// set texture in mapGrid
		for (int i = 0; i < roomSizeX; i++)
			for (int j = 0; j < roomSizeZ; j++)
				if(randRange(randNum) > 10) // randomized generation
					mapGrid[i][j] = { 0, 11 };
				else
					mapGrid[i][j] = emptyTile;

		// generate mesh
		for (int i = 0; i < roomSizeX; i++)
			for (int j = 0; j < roomSizeZ; j++)
			{ 
				// generate edge walls
				if (i == 0)
				{ // left edge
					for (int k = 0; k < roomSizeY; k++)
						if(mapGrid[i][j] != emptyTile || k > wallHeight - 1) // only add a wall if there's a floor or if is higher than obstacle
							addTileSurface(Vector3(i, k, j), TileSurfaceFace::Right, mossyStoneTiles);
				}
				else if (i == roomSizeX - 1)
				{ // right edge
					for (int k = 0; k < roomSizeY; k++)
						if (mapGrid[i][j] != emptyTile || k > wallHeight - 1)
							addTileSurface(Vector3(i + 1, k, j), TileSurfaceFace::Left, mossyStoneTiles);
				}
				if (j == 0)
				{ // back edge
					for (int k = 0; k < roomSizeY; k++)
						if (mapGrid[i][j] != emptyTile || k > wallHeight - 1)
							addTileSurface(Vector3(i, k, j), TileSurfaceFace::Backward, mossyStoneTiles);
				}
				else if (j == roomSizeZ - 1)
				{ // front edge
					for (int k = 0; k < roomSizeY; k++)
						if (mapGrid[i][j] != emptyTile || k > wallHeight - 1)
							addTileSurface(Vector3(i, k, j + 1), TileSurfaceFace::Forward, mossyStoneTiles);
				}
				

				// generate floor and internal walls
				if(mapGrid[i][j] != emptyTile)
				{ // generate floors
					addTileSurface(Vector3(i, 0, j), TileSurfaceFace::Up, darkBrownDirtTile);
				}
				else
				{ // generate internal walls
					for(int k = 0; k < wallHeight; k++)
					{ // loop for mapSizeY for height
						if (i > 0)
							if (mapGrid[i - 1][j] != emptyTile)
								addTileSurface(Vector3(i, k, j), TileSurfaceFace::Left, lightLogsTile);
							
						if (i < roomSizeX - 1)
							if (mapGrid[i + 1][j] != emptyTile)
								addTileSurface(Vector3(i + 1, k, j), TileSurfaceFace::Right, lightLogsTile);

						if (j > 0)
							if (mapGrid[i][j - 1] != emptyTile)
								addTileSurface(Vector3(i, k, j), TileSurfaceFace::Forward, lightLogsTile);
						if (j < roomSizeZ - 1)
							if (mapGrid[i][j + 1] != emptyTile)
								addTileSurface(Vector3(i, k, j + 1), TileSurfaceFace::Backward, lightLogsTile);
					}

					// cap tops of walls
					addTileSurface(Vector3(i, wallHeight, j), TileSurfaceFace::Up, lightLogEndTile);
				} // end generate internal walls

				// generate ceiling
				addTileSurface(Vector3(i, roomSizeY, j), TileSurfaceFace::Down, mossyStoneTiles);

			} // end generate floors and internal walls

		mesh->addData(m_meshData);
	}

	Level::~Level()
	{
		delete mesh;
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
								{ 0xFF, 0x00, 0x00, 0xFF },
								{ 0x00, 0xFF, 0x00, 0xFF },
								{ 0x00, 0x00, 0xFF, 0xFF }};

		// choose draw order based on direction
		if ((u32)face % 2 == 0)
			m_meshData.addFace(index, 0, 1, 2).addFace(index, 1, 3, 2); // hypotenuse goes down and right
		else // (u32)face % 2 == 1
		{
			// flip draw order
			m_meshData.addFace(index, 0, 2, 3).addFace(index, 0, 3, 1); // hypotenuse goes down and right

			// flip texture coords and vertex colors to match draw order
			Vector2 v[4] = { vertexTexCoord[1], vertexTexCoord[0], vertexTexCoord[3], vertexTexCoord[2] };
			Color c[4] = { vertexColor[1], vertexColor[0], vertexColor[3], vertexColor[2] };

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
		TileId tilePos = randomTilePosSet[rand() % length];
		addTileSurface(position, face, tilePos);
	}


} // end Dunjun

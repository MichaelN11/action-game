#include "SDL.h"
#include "game.h"

//#include <iostream>
//#include "tmxparser.h"

int main(int argc, char **argv)
{
	// DEBUG
	//TMXData data = tmxparser::parseTileMap("content/maps/sample_fantasy.tmx");
	//std::cout << "Map width: " << data.mapWidth << ", Map Height: " << data.mapHeight << ", Tile Width: " << data.tileWidth << ", Tile Height: " << data.tileHeight << std::endl;
	//for (TMXTileSet tileSet : data.tileSets)
	//{
	//	std::cout << "First GID: " << tileSet.firstGID << ", Source: " << tileSet.source << ", Height: " << tileSet.height << ", Width: " << tileSet.width
	//		<< ", Tile Width: " << tileSet.tileWidth << ", Tile Height: " << tileSet.tileHeight << ", Spacing: " << tileSet.spacing << std::endl;
	//}
	//for (std::vector<std::vector<TMXTile>> vv : data.tileGridLayers)
	//{
	//	std::cout << "LAYER:" << std::endl;
	//	for (std::vector<TMXTile> v : vv)
	//	{
	//		for (TMXTile tile : v)
	//		{
	//			std::cout << tile.gid << ", ";
	//		}
	//		std::cout << std::endl;
	//	}
	//}

	Game game;

	return 0;
}
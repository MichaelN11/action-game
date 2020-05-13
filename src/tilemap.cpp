#include <algorithm>

#include "tilemap.h"
#include "graphics.h"
#include "tmxparser.h"
#include "rectangle.h"

TileMap::TileMap(Graphics &graphics, const std::string& tmxFileName)
{
	TMXData data = tmxparser::parseTileMap(tmxFileName);
	for (std::vector<std::vector<TMXTile>> tmxLayer : data.tileGridLayers)
	{
		std::vector<std::vector<Tile>> layer;
		
		for (std::vector<TMXTile> tmxRow : tmxLayer)
		{
			std::vector<Tile> row;

			for (TMXTile tmxTile : tmxRow)
			{
				Tile tile;
				tile.flippedDiagonally = tmxTile.flippedDiagonally;
				tile.flippedHorizontally = tmxTile.flippedHorizontally;
				tile.flippedVertically = tmxTile.flippedVertically;
				tile.id = tmxTile.gid;
				tile.tileSheetId = tmxTile.tileSetId;
				row.push_back(tile);
			}

			layer.push_back(row);
		}

		tileGridLayers.push_back(layer);
	}

	for (TMXTileSet tileSet : data.tileSets)
	{
		graphics.loadTilesheet(tileSet.source, tileSet.tileWidth, tileSet.tileHeight, tileSet.spacing);
		tileSheetPaths.push_back(tileSet.source);
	}

	mapWidth = data.mapWidth;
	mapHeight = data.mapHeight;
	tileWidth = data.tileWidth;
	tileHeight = data.tileHeight;

	EventManager::registerListener<DrawEvent>([this](DrawEvent& dEvent)
		{
			if (dEvent.graphics)
				draw(*(dEvent.graphics));
		});
}

void TileMap::draw(Graphics& graphics)
{
	// Portion of the tilemap being drawn
	Rectangle view = graphics.getView();

	int scaledTileWidth = (int)(tileWidth * graphics.getScale());
	int scaledTileHeight = (int)(tileHeight * graphics.getScale());

	// first and last tiles to draw
	int firstTileCol = std::max(view.getX() / scaledTileWidth, 0);
	int firstTileRow = std::max(view.getY() / scaledTileHeight, 0);
	int lastTileCol = std::min((view.getX2() / scaledTileWidth) + 1, mapWidth);
	int lastTileRow = std::min((view.getY2() / scaledTileHeight) + 1, mapHeight);
	// offset that is subtracted from the axis when starting drawing
	int drawOffsetX = 0, drawOffsetY = 0;
	if (view.getX() >= 0)
		drawOffsetX = view.getX() % scaledTileWidth;
	else
		drawOffsetX = view.getX();
	if (view.getY() >= 0)
		drawOffsetY = view.getY() % scaledTileHeight;
	else
		drawOffsetY = view.getY();

	for (std::vector<std::vector<Tile>> layer : tileGridLayers)
	{
		for (int rowNum = firstTileRow; rowNum < lastTileRow; rowNum++)
		{
			for (int colNum = firstTileCol; colNum < lastTileCol; colNum++)
			{
				Tile tile = layer.at(rowNum).at(colNum);
				if (tile.id >= 0 && tile.tileSheetId >= 0)
				{
					Rectangle destRect((colNum - firstTileCol) * scaledTileWidth - drawOffsetX,
						(rowNum - firstTileRow) * scaledTileHeight - drawOffsetY,
						scaledTileWidth,
						scaledTileHeight);

					std::string filePath = tileSheetPaths.at(tile.tileSheetId);
					// scaled is false because it's already scaled here
					graphics.drawImage(filePath, tile.id, destRect, tile.flippedDiagonally, tile.flippedHorizontally, tile.flippedVertically, false);
				}
			}
		}
	}
}
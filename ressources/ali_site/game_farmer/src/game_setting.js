//This file will content const values which the game need

const globalSize = 32;
const globalNaturalGeneration = 30;
const globalGrowthSpeed = 1;
const mapHeight = 75;
const mapWidth = 95;

const SOUND = {
	DEFAULT_SOUND: new Audio('assets/sound/default_sound.mp3')
}

const IMG = {
	GRASS: newImage("assets/image/ground/grass.png", "ground", 0, "ground"),
	GRASS_SIDE: newImage("assets/image/ground/grass_side.png", "ground", 0, "ground"),
	GRASS_CORNER: newImage("assets/image/ground/grass_corner.png", "ground", 0, "ground"),
	GRASS_FARM: newImage("assets/image/ground/grass_farm.png", "ground_farm", 0, "ground"),
	TREE0: newImage("assets/image/static/tree0.png", "static", 2),
	TREE1: newImage("assets/image/static/tree1.png", "static", 2),
	PLANT0: newImage("assets/image/static/plant0.png", "static", 1),
	ROCK0: newImage("assets/image/static/rock0.png", "static", 1),
	FLOWER0: newImage("assets/image/static/flower0.png", "static", 1),
	FLOWER1: newImage("assets/image/static/flower1.png", "static", 1),
	FLOWER2: newImage("assets/image/static/flower2.png", "static", 1),
	TRUNK0: newImage("assets/image/static/trunk0.png", "static", 1),
	MELON: newImages("assets/image/crops/melon.png", "crops", 1, 7),
	WHEAT: newImages("assets/image/crops/wheat.png", "crops", 1, 7),
	SUGARCANE: newImages("assets/image/crops/sugarcane.png", "crops", 1, 7),
	EGGPLANT: newImages("assets/image/crops/eggplant.png", "crops", 1, 7),
	CHILI: newImages("assets/image/crops/chili.png", "crops", 1, 7)
}

const IMG_ICON = {
	MELON: newImage("assets/image/icon/melon_icon.png", "icon", 0),
	WHEAT: newImage("assets/image/icon/wheat_icon.png", "icon", 0),
	SUGARCANE: newImage("assets/image/icon/sugarcane_icon.png", "icon", 0),
	EGGPLANT: newImage("assets/image/icon/eggplant_icon.png", "icon", 0),
	CHILI: newImage("assets/image/icon/chili_icon.png", "icon", 0)
}
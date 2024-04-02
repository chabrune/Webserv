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
	TREE0: newImage("assets/image/static/tree0.png", "tree0", 2),
	TREE1: newImage("assets/image/static/tree1.png", "tree1", 2),
	PLANT0: newImage("assets/image/static/plant0.png", "plant0", 1),
	ROCK0: newImage("assets/image/static/rock0.png", "rock0", 1),
	FLOWER0: newImage("assets/image/static/flower0.png", "flower0", 1),
	FLOWER1: newImage("assets/image/static/flower1.png", "flower1", 1),
	FLOWER2: newImage("assets/image/static/flower2.png", "flower2", 1),
	TRUNK0: newImage("assets/image/static/trunk0.png", "trunk0", 1),
	MELON: newImages("assets/image/crops/melon.png", "melon", 1, 7),
	WHEAT: newImages("assets/image/crops/wheat.png", "wheat", 1, 7),
	SUGARCANE: newImages("assets/image/crops/sugarcane.png", "sugarcane", 1, 7),
	EGGPLANT: newImages("assets/image/crops/eggplant.png", "eggplant", 1, 7),
	CHILI: newImages("assets/image/crops/chili.png", "chili", 1, 7)
}

const IMG_ICON = {
	SEED: newImage("assets/image/icon/seed_icon.png", "seed0", 0),
	MELON: newImage("assets/image/icon/melon_icon.png", "melon", 0),
	WHEAT: newImage("assets/image/icon/wheat_icon.png", "wheat", 0),
	SUGARCANE: newImage("assets/image/icon/sugarcane_icon.png", "sugarcane", 0),
	EGGPLANT: newImage("assets/image/icon/eggplant_icon.png", "eggplant", 0),
	CHILI: newImage("assets/image/icon/chili_icon.png", "chili", 0),
	WOOD: newImage("assets/image/icon/wood_icon.png", "wood", 0)

}

const RESOURCE = {
	SEED: new Resource("Seed", IMG_ICON.SEED),
	MELON: new Resource("Melon", IMG_ICON.MELON),
	WHEAT: new Resource("Wheat", IMG_ICON.WHEAT),
	SUGARCANE: new Resource("Sugarcane", IMG_ICON.SUGARCANE),
	EGGPLANT: new Resource("Eggplant", IMG_ICON.EGGPLANT),
	CHILI: new Resource("Chili", IMG_ICON.CHILI),
	WOOD: new Resource("Wood", IMG_ICON.WOOD)
}

const BLOCK = {
	GRASS: new ElementGround([IMG.GRASS]).setBlockAction(new BlockActionPlowe()),
	GRASS_SIDE: new ElementGround([IMG.GRASS_SIDE]),
	GRASS_CORNER: new ElementGround([IMG.GRASS_CORNER]),
	GRASS_FARM: new ElementGround([IMG.GRASS_FARM]),

	TREE0: new ElementStatic([IMG.TREE0]).setNaturalSpawnChance(20).setLootable(RESOURCE.SEED).setBlockAction(new BlockActionHarvest()),
	TREE1: new ElementStatic([IMG.TREE1]).setNaturalSpawnChance(20),
	PLANT0: new ElementStatic([IMG.PLANT0]).setNaturalSpawnChance(75),
	ROCK0: new ElementStatic([IMG.ROCK0]).setNaturalSpawnChance(30),
	FLOWER0: new ElementStatic([IMG.FLOWER0]).setNaturalSpawnChance(4),
	FLOWER1: new ElementStatic([IMG.FLOWER1]).setNaturalSpawnChance(4),
	FLOWER2: new ElementStatic([IMG.FLOWER2]).setNaturalSpawnChance(4),
	TRUNK0: new ElementStatic([IMG.TRUNK0]).setNaturalSpawnChance(10),
	MELON: new ElementCrop(IMG.MELON, IMG_ICON.MELON, "Melon", 5000),
	WHEAT: new ElementCrop(IMG.WHEAT, IMG_ICON.WHEAT, "Wheat", 15000),
	SUGARCANE: new ElementCrop(IMG.SUGARCANE, IMG_ICON.SUGARCANE, "Sugarcane", 13000),
	EGGPLANT: new ElementCrop(IMG.EGGPLANT, IMG_ICON.EGGPLANT, "Eggplant", 3000),
	CHILI: new ElementCrop(IMG.CHILI, IMG_ICON.CHILI, "Chili", 7000)


}
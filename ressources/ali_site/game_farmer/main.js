const blocks = [];
let map;
let handBlock = null;


document.getElementsByClassName('buttonPlay')[0].addEventListener('click', function() {
    document.getElementsByClassName('buttonPlay')[0].remove()
    initGame();
    SOUND.DEFAULT_SOUND.volume = 0.5;
    SOUND.DEFAULT_SOUND.loop = true;
    SOUND.DEFAULT_SOUND.play();
});

function initGame() {
    loadBlocks();
    map = new Map();
    loadGlobalListeners();
}

function loadBlocks() {
    blocks.push(new ElementGround(new Array(IMG.GRASS)).setBlockAction(new BlockActionPlowe()));
    blocks.push(new ElementGround(new Array(IMG.GRASS_SIDE)));
    blocks.push(new ElementGround(new Array(IMG.GRASS_CORNER)));
    blocks.push(new ElementGround(new Array(IMG.GRASS_FARM)));

    blocks.push(new ElementStatic(new Array(IMG.TREE0)).setNaturalSpawnChance(20).setLootable(RESOURCE.SEED).setBlockAction(new BlockActionHarvest()));
    blocks.push(new ElementStatic(new Array(IMG.TREE1)).setNaturalSpawnChance(20));
    blocks.push(new ElementStatic(new Array(IMG.PLANT0)).setNaturalSpawnChance(75));
    blocks.push(new ElementStatic(new Array(IMG.ROCK0)).setNaturalSpawnChance(30));
    blocks.push(new ElementStatic(new Array(IMG.FLOWER0)).setNaturalSpawnChance(4));
    blocks.push(new ElementStatic(new Array(IMG.FLOWER1)).setNaturalSpawnChance(4));
    blocks.push(new ElementStatic(new Array(IMG.FLOWER2)).setNaturalSpawnChance(4));
    blocks.push(new ElementStatic(new Array(IMG.TRUNK0)).setNaturalSpawnChance(10));
    blocks.push(new ElementCrop(IMG.MELON, IMG_ICON.MELON, "Melon", 5000));
    blocks.push(new ElementCrop(IMG.WHEAT, IMG_ICON.WHEAT, "Wheat", 15000));
    blocks.push(new ElementCrop(IMG.SUGARCANE, IMG_ICON.SUGARCANE, "Sugarcane", 13000));
    blocks.push(new ElementCrop(IMG.EGGPLANT, IMG_ICON.EGGPLANT, "Eggplant",3000));
    blocks.push(new ElementCrop(IMG.CHILI, IMG_ICON.CHILI, "Chili", 7000));
}

function loadGlobalListeners() {
    document.addEventListener('contextmenu', (event) => {
        event.preventDefault();
    })

    document.getElementById('toolbar').addEventListener('mousedown', mouseDownToolBar);
}

function getBlockFromId(id) {
    for (const block of blocks) {
        if (block .images[0].getAttribute("id").toUpperCase() === id.toUpperCase())
            return block;
    }
    return null;
}
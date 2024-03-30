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
    map = new Map('map');
    loadGlobalListeners();
}

function loadBlocks() {
    blocks.push(new Static(new Array(IMG.TREE0)).setNaturalSpawnChance(20).setLootable(RESOURCE.SEED).setBlockAction(new BlockActionHarvest()));
    blocks.push(new Static(new Array(IMG.TREE1)).setNaturalSpawnChance(20));
    blocks.push(new Static(new Array(IMG.PLANT0)).setNaturalSpawnChance(75));
    blocks.push(new Static(new Array(IMG.ROCK0)).setNaturalSpawnChance(30));
    blocks.push(new Static(new Array(IMG.FLOWER0)).setNaturalSpawnChance(4));
    blocks.push(new Static(new Array(IMG.FLOWER1)).setNaturalSpawnChance(4));
    blocks.push(new Static(new Array(IMG.FLOWER2)).setNaturalSpawnChance(4));
    blocks.push(new Static(new Array(IMG.TRUNK0)).setNaturalSpawnChance(10));
    blocks.push(new Crop(IMG.MELON, IMG_ICON.MELON, 5000));
    blocks.push(new Crop(IMG.WHEAT, IMG_ICON.WHEAT, 15000));
    blocks.push(new Crop(IMG.SUGARCANE, IMG_ICON.SUGARCANE, 13000));
    blocks.push(new Crop(IMG.EGGPLANT, IMG_ICON.EGGPLANT, 3000));
    blocks.push(new Crop(IMG.CHILI, IMG_ICON.CHILI, 7000));
}

function loadGlobalListeners() {
    document.addEventListener('contextmenu', (event) => {
        event.preventDefault();
    })

    document.getElementById('toolbar').addEventListener('mousedown', mouseDownToolBar);
}

function getBlockFromName(name) {
    for (const block of blocks) {
        if (block.name.toUpperCase() === name.toUpperCase())
            return block;
    }
    return null;
}

function mouseDownToolBar(event) {
    let block = getBlockFromName(event.target.textContent)
    if (block === null)
        return;
    handBlock = block;
}
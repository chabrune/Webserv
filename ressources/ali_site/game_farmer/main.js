const blocks = [];
const sounds = [];
let map;
let handBlock = null;
let grounds = [];

initGame();

function initGame() {
    loadSounds();
    loadGrounds();
    loadBlocks();
    map = new Map('map');
    loadGlobalListeners();
    initAllDefaultValues()
}

function loadSounds() {
    sounds.push(new Audio('assets/sound/default_sound.mp3'));
}

function loadBlocks() {
    blocks.push(new Static(new Array(loadImage("assets/image/static/tree0.png", "static", 2)), 20));
    blocks.push(new Static(new Array(loadImage("assets/image/static/tree1.png", "static", 2)), 20));
    blocks.push(new Static(new Array(loadImage("assets/image/static/plant0.png", "static", 1)), 75));
    blocks.push(new Static(new Array(loadImage("assets/image/static/rock0.png", "static", 1)), 30));
    blocks.push(new Static(new Array(loadImage("assets/image/static/flower0.png", "static", 1)), 4));
    blocks.push(new Static(new Array(loadImage("assets/image/static/flower1.png", "static", 1)), 4));
    blocks.push(new Static(new Array(loadImage("assets/image/static/flower2.png", "static", 1)), 4));
    blocks.push(new Static(new Array(loadImage("assets/image/static/trunk0.png", "static", 1)), 10));
    blocks.push(new Crop(loadImages("assets/image/crops/melon.png", "crops", 1, 7), 5000));
    blocks.push(new Crop(loadImages("assets/image/crops/wheat.png", "crops", 1, 7), 15000));
    blocks.push(new Crop(loadImages("assets/image/crops/sugarcane.png", "crops", 1, 7), 13000));
    blocks.push(new Crop(loadImages("assets/image/crops/eggplant.png", "crops", 1, 7), 3000));
    blocks.push(new Crop(loadImages("assets/image/crops/chili.png", "crops", 1, 7), 7000));
}

function loadGrounds() {
    grounds.push(loadImage("assets/image/ground/grass.png", "ground", 0));
    grounds.push(loadImage("assets/image/ground/grass_side.png", "ground", 0));
    grounds.push(loadImage("assets/image/ground/grass_corner.png", "ground", 0));
    grounds.push(loadImage("assets/image/ground/grass_farm.png", "ground", 0));
}

function loadGlobalListeners() {
    /*document.addEventListener('contextmenu', (event) => {
        event.preventDefault();
    })*/

    document.getElementById('toolbar').addEventListener('mousedown', mouseDownToolBar);
}

function initAllDefaultValues() {
    sounds[0].play();
}

function getBlockFromName(name) {
    for (const block of blocks) {
        if (block.name === name)
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
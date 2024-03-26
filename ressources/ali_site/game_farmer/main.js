const blocks = [];
const sounds = [];
let map;
let handBlock;
let grounds = [];

initGame();

function initGame() {
    map = new Map('map');
    loadSounds();
    loadGrounds();
    loadBlocks();
    loadGlobalListeners();
    initAllDefaultValues()
}

function loadSounds() {
    sounds.push(new Audio('../sound/default_sound.mp3'));
}

function loadBlocks() {
    blocks.push(new Static(loadImages("../image/static/tree.png", "static", 2, 1)));
    blocks.push(new Crop(loadImages("../image/crops/melon.png", "crops", 1, 7), 3000));
    blocks.push(new Crop(loadImages("../image/crops/wheat.png", "crops", 1, 7), 3000));
    blocks.push(new Crop(loadImages("../image/crops/sugarcane.png", "crops", 1, 7), 3000));
    blocks.push(new Crop(loadImages("../image/crops/eggplant.png", "crops", 1, 7), 3000));
    blocks.push(new Crop(loadImages("../image/crops/chili.png", "crops", 1, 7), 3000));
}

function loadGrounds() {
    grounds.push(loadImage("../image/ground/grass_farm.png", "ground", 0));
}

function loadGlobalListeners() {
    document.addEventListener('wheel', wheelListener);

    document.addEventListener('contextmenu', (event) => {
        event.preventDefault();
    })
}

function initAllDefaultValues() {
    handBlock = blocks[0];
    sounds[0].play();
}


function wheelListener(event) {
    if (event.deltaY < 0)
    {
        let i = blocks.indexOf(handBlock) - 1;
        console.log("up " + i)
        if (i < 0)
            i = blocks.length - 1;
        handBlock = blocks[i];
    }
    else if (event.deltaY > 0)
    {
        let i = blocks.indexOf(handBlock) + 1;
        console.log(i)

        if (i >= blocks.length)
            i = 0;
        handBlock = blocks[i];
    }
}
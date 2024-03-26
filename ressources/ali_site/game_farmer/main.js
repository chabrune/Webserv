const blocks = [];
const sounds = [];
let map;
let handBlock;

initGame();

function initGame() {
    map = new Map('map');
    loadSounds();
    loadBlocks();
    loadGlobalListeners();
    initAllDefaultValues()
}

function loadSounds() {
    sounds.push(new Audio('../sound/default_sound.mp3'));
}

function loadBlocks() {
    blocks.push(new Block(loadImages("../image/crops/melon.png", "crops", 1, 7), 3000));
    blocks.push(new Block(loadImages("../image/crops/wheat.png", "crops", 1, 7), 3000));
    blocks.push(new Block(loadImages("../image/crops/sugarcane.png", "crops", 1, 7), 3000));
    blocks.push(new Block(loadImages("../image/crops/eggplant.png", "crops", 1, 7), 3000));
    blocks.push(new Block(loadImages("../image/crops/chili.png", "crops", 1, 7), 3000));
}

function loadGlobalListeners() {
    document.addEventListener('wheel', wheelListener);
}

function initAllDefaultValues() {
    handBlock = blocks[0];
    sounds[0].play();
}


function wheelListener(event) {
    if (event.deltaY < 0)
    {
        let i = blocks.indexOf(handBlock) - 1;
        if (i <= 0)
            i = blocks.length - 1;
        handBlock = blocks[i];
    }
    else if (event.deltaY > 0)
    {
        let i = blocks.indexOf(handBlock) + 1;
        if (i >= blocks.length)
            i = 0;
        handBlock = blocks[i];
    }
}
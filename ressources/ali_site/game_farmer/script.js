// Calculate the number of squares horizontally and vertically based on the screen size

map = new Map('map');

let isDrawing = false;
let isErasing = false;

const character = new Character(500, 500, loadImage("../image/test.png"), 10);

document.addEventListener('keydown', character.moveListener);

const container = document.body;
for (let i = 0; i < map.numRows; i++) {
    for (let j = 0; j < map.squaresPerRow; j++) {
        const square = document.createElement('div');
        square.classList.add('square');
        square.appendChild(loadImage("../image/grass.png"));
        map.addSquare(square);
    }
}

function erase(square) {
    square.style.backgroundColor = 'lightgray';
}


function draw(square) {
    square.style.backgroundColor = 'blue';
}

function drawOnSquare() {
    // Calculate the index of the square in the linear grid
    let row = Math.floor(character.positionY / 64);
    let column = Math.floor(character.positionX / 64) - 1;
    const index = row * map.squaresPerRow + column;
    const square = document.querySelectorAll('.square')[index];

    square.removeChild(square.querySelector('img'));
    square.appendChild(loadImage("../image/blocks/farmland_dry.png"));
    setTimeout(function() {
        square.removeChild(square.querySelector('img'));
        square.appendChild(loadImage("../image/blocks/farmland_wet.png"));
    }, 5000);
    setTimeout(function() {
        square.removeChild(square.querySelector('img'));
        square.appendChild(loadImage("../image/blocks/dirt_podzol_top.png"));
    }, 10000);
}

function stopDrawing() {
    isDrawing = false;
    isErasing = false;
}
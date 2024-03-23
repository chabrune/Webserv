// Calculate the number of squares horizontally and vertically based on the screen size
map = document.getElementById('map');

result = getPercent(screen.width, 95);
map.style.width = result - (result % 64) + "px";
result = getPercent(screen.height, 80);
map.style.height = result - (result % 64) + "px";

const squaresPerRow = Math.floor(map.clientWidth / 64);
const numRows = Math.floor(map.clientHeight / 64);


let isDrawing = false;
let isErasing = false;

const character = new Character(500, 500, document.getElementById('characterimg'));


document.addEventListener('keydown', moveListener);
map.addEventListener('mousedown', mouseDownEvent);
map.addEventListener('mousemove', mouseMoveEvent);
map.addEventListener('mouseup', stopDrawing);

const container = document.body;
for (let i = 0; i < numRows; i++) {
    for (let j = 0; j < squaresPerRow; j++) {
        const square = document.createElement('div');
        square.classList.add('square');
        map.appendChild(square);
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
    let row = Math.floor((character.positionY - 45) / 64);
    let column = Math.floor((character.positionX - 70) / 64);
    const index = row * squaresPerRow + column;
    document.querySelectorAll('.square')[index].appendChild(loadImage("../image/4.png"))
}

function stopDrawing() {
    isDrawing = false;
    isErasing = false;
}
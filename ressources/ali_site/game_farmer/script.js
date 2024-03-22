// Calculate the number of squares horizontally and vertically based on the screen size
const screenWidth = window.innerWidth;
const screenHeight = window.innerHeight;
const squaresPerRow = Math.floor(screenWidth / 64) - 2;
const numRows = Math.floor(screenHeight / 64);

let isDrawing = false;
let isErasing = false;

const character = new Character(500, 500, document.getElementById('characterimg'));

document.addEventListener('keydown', moveListener);

// Create grid dynamically
const container = document.body;
for (let i = 0; i < numRows; i++) {
    for (let j = 0; j < squaresPerRow; j++) {
        const square = document.createElement('div');
        square.classList.add('square');
        square.style.backgroundColor = 'lightgray';
        square.addEventListener('mousedown', mouseDownEvent);
        square.addEventListener('mousemove', mouseMoveEvent);
        square.addEventListener('mouseup', stopDrawing);
        container.appendChild(square);
    }
    const clearfix = document.createElement('div');
    clearfix.classList.add('clearfix');
    container.appendChild(clearfix);
}

function erase(square) {
    square.style.backgroundColor = 'lightgray';
}


function draw(square) {
    square.style.backgroundColor = 'blue';
}

function drawOnSquare() {
    // Calculate the index of the square in the linear grid
    let row = Math.floor(character.positionY / 64)
    let column = Math.floor(character.positionX / 64)
    const index = row * squaresPerRow + column;
    //draw(document.querySelectorAll('.square')[index])
    console.log(index);
    document.querySelectorAll('.square')[index].appendChild(loadImage("../image/4.png"))
}

function stopDrawing() {
    isDrawing = false;
    isErasing = false;
}
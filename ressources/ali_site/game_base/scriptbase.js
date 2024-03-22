// Calculate the number of squares horizontally and vertically based on the screen size
const screenWidth = window.innerWidth;
const screenHeight = window.innerHeight;
const squaresPerRow = Math.floor(screenWidth / 64) - 2;
const numRows = Math.floor(screenHeight / 64);

let isDrawing = false;
let isErasing = false;

const character = document.getElementById('character');
let characterX = 500;
let characterY = 500;

document.addEventListener('keydown', moveListener);


function moveListener(event) {
    const stepSize = 5; // Adjust this value to change the step size of movement
    if (event.key === 'ArrowUp')
        characterY -= stepSize;
    if (event.key === 'ArrowDown')
        characterY += stepSize;
    if (event.key === 'ArrowLeft')
        characterX -= stepSize;
    if (event.key === 'ArrowRight')
        characterX += stepSize;
    if (event.key === ' ')
        drawOnSquare(Math.floor(characterY / 64), Math.floor(characterX / 64));
    moveCharacter(characterX, characterY);
}

function moveListener2(event) {
    if (event.key === ' ')
        drawOnSquare(Math.floor(characterY / 64), Math.floor(characterX / 64));
    moveCharacter(characterX, characterY);
}

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

function erase(event) {
    const square = event.target;
    square.style.backgroundColor = 'lightgray';
}

// Function to handle starting drawing or moving the character
function mouseDownEvent(event) {
    if (event.button === 0) {
        isDrawing = true;
        draw(event);
    } else {
        isErasing = true;
        erase(event);
    }
}

function mouseMoveEvent(event) {
    if (isDrawing === true && event.button === 0)
        draw(event);
    else if (isErasing === true)
        erase(event);
}

function draw(event) {
    const square = event.target;
    square.style.backgroundColor = 'blue';
}

function drawOnSquare(row, column) {
    // Calculate the index of the square in the linear grid
    const index = row * squaresPerRow + column;

    // Get the square element using its index
    const square = document.querySelectorAll('.square')[index];

    // Change the background color of the square
    square.style.backgroundColor = 'blue';
}

function moveCharacter(x, y) {
    character.style.left = x + 'px';
    character.style.top = y + 'px';
}

function stopDrawing() {
    isDrawing = false;
    isErasing = false;
}
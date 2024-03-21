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

// Check if the character element exists
if (!character) {
    console.error("Character element not found.");
}


document.addEventListener('keydown', function(event) {
    const stepSize = 5; // Adjust this value to change the step size of movement
    switch (event.key) {
        case 'ArrowUp':
            characterY -= stepSize;
            break;
        case 'ArrowDown':
            characterY += stepSize;
            break;
        case 'ArrowLeft':
            characterX -= stepSize;
            break;
        case 'ArrowRight':
            characterX += stepSize;
            break;
    }
    moveCharacter(characterX, characterY);
});

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

function moveCharacter(x, y) {
    character.style.left = x + 'px';
    character.style.top = y + 'px';
}

function stopDrawing() {
    isDrawing = false;
    isErasing = false;
}
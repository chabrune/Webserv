// Calculate the number of squares horizontally and vertically based on the screen size
const screenWidth = window.innerWidth;
const screenHeight = window.innerHeight;
const squaresPerRow = Math.floor(screenWidth / 64) - 2;
const numRows = Math.floor(screenHeight / 64);

let isDrawing = false;
let isErasing = false;

const character = new Character(500, 500, document.getElementById('characterimg'));

document.addEventListener('keydown', moveListener);

function moveListener(event) {
    const stepSize = 5;
    if (event.key === 'ArrowUp')
        character.positionY -= stepSize;
    if (event.key === 'ArrowDown')
        character.positionY += stepSize;
    if (event.key === 'ArrowLeft')
        character.positionX -= stepSize;
    if (event.key === 'ArrowRight')
        character.positionX += stepSize;
    if (event.key === ' ')
        drawOnSquare();
    character.moveCharacter();
}

function mouseDownEvent(event) {
    if (event.button === 0) {
        isDrawing = true;
        draw(event.target);
    } else {
        isErasing = true;
        erase(event.target);
    }
}

function mouseMoveEvent(event) {
    if (isDrawing === true && event.button === 0)
        draw(event.target);
    else if (isErasing === true)
        erase(event.target);
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

function erase(square) {
    square.style.backgroundColor = 'lightgray';
}


function draw(square) {
    square.style.backgroundColor = 'blue';
}

function loadImageIntoSquare(imageUrl, square) {
    // Create a new Image object
    const img = new Image();

    // Set the src attribute to the URL of the image
    img.src = imageUrl;

    // Add an event listener to execute code when the image has finished loading
    img.onload = function() {
        console.log(`Image loaded successfully into square`);
        // Append the image to the square
        square.appendChild(img);
    };

    // Add an event listener to handle errors if the image fails to load
    img.onerror = function() {
        console.error(`Failed to load image into square`);
    };
}

function drawOnSquare() {
    // Calculate the index of the square in the linear grid
    let row = Math.floor(character.positionY / 64)
    let column = Math.floor(character.positionX / 64)
    const index = row * squaresPerRow + column;
    //draw(document.querySelectorAll('.square')[index])
    console.log(index);
    loadImageIntoSquare("../image/4.png", document.querySelectorAll('.square')[index]);
}

function stopDrawing() {
    isDrawing = false;
    isErasing = false;
}
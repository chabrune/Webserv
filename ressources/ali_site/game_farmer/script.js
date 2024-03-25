const globalSize = 64;
const blocks = [];

map = new Map('map');

blocks.push(new Block(loadImages("../image/crops/wheat", "crops", 1, 4), 5000));

const character = new Character(500, 500, loadImage("../image/char.png", "character", 9), 10);
document.addEventListener('keydown', character.moveListener);
document.addEventListener('wheel', test);

function test(event) {
    if (event.deltaY < 0)
    {
        console.log('scrolling up');
    }
    else if (event.deltaY > 0)
    {
        console.log('scrolling down');
    }
}


function drawOnSquare(block) {
    let row = Math.floor(character.positionY / globalSize);
    let column = Math.floor(character.positionX / globalSize);
    if (map.isBorderOfMap(row, column))
        return;
    const index = row * map.squaresPerRow + column;
    const square = document.querySelectorAll('.square')[index];

    if (square.querySelector('#crops'))
        return;
    square.appendChild(block.images[0].cloneNode(true));
    for (let i= 1; i <= block.images.length - 1; i++) {
        setTimeout(function() {
            square.removeChild(square.querySelector('#crops'));
            square.appendChild(block.images[i].cloneNode(true));
        }, block.timeToGrowth * i);
    }
}
map = new Map('map');

blocks.push(new Block(loadImages("../image/crops/wheat", "crops", 1, 4), 5000));
blocks.push(new Block(loadImages("../image/crops/ad/wheat", "crops", 1, 2), 5000));

handBlock = blocks[0];

//const character = new Character(500, 500, loadImage("../image/char.png", "character", 9), 10);
//document.addEventListener('keydown', character.moveListener);
document.addEventListener('wheel', test);

function mouseDownEvent(event) {
    const mapRect = map.map.getBoundingClientRect();
    const x = event.clientX - mapRect.left;
    const y = event.clientY - mapRect.top;
    
    drawOnSquare(handBlock, x, y);
}

function mouseMoveEvent(event) {

}

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


function drawOnSquare(block, x, y) {
    let row = Math.floor(y / globalSize);
    let column = Math.floor(x / globalSize);
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
        }, block.timeToGrowth * i + (Math.random() * block.timeToGrowth));
    }
}
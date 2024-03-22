/*function moveListener(event) {
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
}*/
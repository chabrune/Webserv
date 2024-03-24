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
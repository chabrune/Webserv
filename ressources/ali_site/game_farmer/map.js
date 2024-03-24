class Map {
    constructor(elemId) {
        this.map = document.getElementById(elemId);
        let result = getPercent(screen.width, 60);
        this.map.style.width = result - (result % globalSize) + "px";
        result = getPercent(screen.height, 60);
        this.map.style.height = result - (result % globalSize) + "px";

        this.squaresPerRow = Math.floor(this.map.clientWidth / globalSize);
        this.numRows = Math.floor(this.map.clientHeight / globalSize);

        this.map.addEventListener('mousedown', mouseDownEvent);
        this.map.addEventListener('mousemove', mouseMoveEvent);
        this.map.addEventListener('mouseup', stopDrawing);
    }

    addSquare(square) {
        this.map.appendChild(square);
    }
}
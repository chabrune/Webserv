class Map {
    constructor(elemId) {
        this.map = document.getElementById(elemId);
        let result = getPercent(screen.width, 95);
        this.map.style.width = result - (result % 64) + "px";
        result = getPercent(screen.height, 80);
        this.map.style.height = result - (result % 64) + "px";

        this.squaresPerRow = Math.floor(this.map.clientWidth / 64);
        this.numRows = Math.floor(this.map.clientHeight / 64);

        this.map.addEventListener('mousedown', mouseDownEvent);
        this.map.addEventListener('mousemove', mouseMoveEvent);
        this.map.addEventListener('mouseup', stopDrawing);
    }

    addSquare(square) {
        this.map.appendChild(square);
    }
}
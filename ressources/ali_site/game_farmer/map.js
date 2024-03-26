class Map {
    constructor(elemId) {
        this.map = document.getElementById(elemId);
        let result = getPercent(screen.width, 60);
        this.map.style.width = result - (result % globalSize) + "px";
        result = getPercent(screen.height, 60);
        this.map.style.height = result - (result % globalSize) + "px";

        this.squaresPerRow = Math.floor(this.map.clientWidth / globalSize);
        this.numRows = Math.floor(this.map.clientHeight / globalSize);

        this.mapGenerator();

        this.map.addEventListener('mousedown', mouseDownEvent);
        this.map.addEventListener('mousemove', mouseMoveEvent);
    }

    addSquare(square) {
        this.map.appendChild(square);
    }

    mapGenerator() {
        for (let x = 0; x < this.numRows; x++) {
            for (let y = 0; y < this.squaresPerRow; y++) {
                const square = document.createElement('div');
                square.classList.add('square');
                if (this.#isCorner(x, y)) {
                    square.appendChild(loadImage("../image/ground/grass_corner.png", "corner", 0))
                }
                else if (this.isBorderOfMap(x, y)) {
                    square.appendChild(loadImage("../image/ground/grass_side.png", "side", 0))
                }
                else {
                    square.appendChild(loadImage("../image/ground/grass.png", "ground", 0));
                }
                const img = square.querySelector('img');
                img.style.transform = this.#rotateCalculation(x, y);
                this.addSquare(square);
            }
        }
    }

    #isCorner(x, y) {
        return x === 0 && y === 0 || x === 0 && y === (this.squaresPerRow - 1) || y === 0 && x === (this.numRows - 1) || x === (this.numRows - 1) && y === (this.squaresPerRow - 1)
    }

    isBorderOfMap(x, y) {
        return x === 0 || y === 0 || x === (this.numRows - 1) || y === (this.squaresPerRow - 1)
    }

    #rotateCalculation(x, y) {
        if (x === 0 && y > 0)
            return "rotate(90deg)";
        else if (y === (this.squaresPerRow - 1))
            return "rotate(180deg)";
        else if (x === (this.numRows - 1))
            return "rotate(270deg)";
        return "rotate(0deg)";
    }

    getSquareIndex(x, y) {
        /*let row = Math.floor(y / globalSize);
        let column = Math.floor(x / globalSize);
        if (map.isBorderOfMap(row, column))
            return;*/
        return y * map.squaresPerRow + x;
    }

    getSquare(x, y) {
        return document.querySelectorAll('.square')[this.getSquareIndex(x, y)];
    }
}

function mouseDownEvent(event) {
    const mapRect = map.map.getBoundingClientRect();
    const x = Math.floor((event.clientX - mapRect.left) / globalSize);
    const y = Math.floor((event.clientY - mapRect.top) / globalSize);
    if (map.isBorderOfMap(y, x))
        return;

    const square = map.getSquare(x, y);
    if (event.button === 0) {
        handBlock.build(square);
    }
    else {
        square.removeChild(square.querySelector('#ground'));
        square.appendChild(grounds[0].cloneNode(true));
    }
}

function mouseMoveEvent(event) {

}
class Map {
    constructor(elemId) {
        this.map = document.getElementById(elemId);
        let result = getPercent(screen.width, mapWidth);
        this.map.style.width = result - (result % globalSize) + "px";
        result = getPercent(screen.height, mapHeight);
        this.map.style.height = result - (result % globalSize) + "px";

        this.squaresPerRow = Math.floor(this.map.clientWidth / globalSize);
        this.numRows = Math.floor(this.map.clientHeight / globalSize);

        this.mapGenerator();

        this.map.addEventListener('mousedown', mouseDownMap);
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
                    square.appendChild(grounds[GROUND.GRASS_CORNER].cloneNode(true))
                }
                else if (this.isBorderOfMap(x, y)) {
                    square.appendChild(grounds[GROUND.GRASS_SIDE].cloneNode(true));
                }
                else {
                    square.appendChild(grounds[GROUND.GRASS].cloneNode(true));
                    if (Math.random() * 100 <= globalNaturalGeneration)
                        this.#generateElement(square);
                }
                const img = square.querySelector('img');
                img.style.transform = this.#rotateCalculation(x, y);
                this.addSquare(square);
            }
        }
    }

    #generateElement(square) {
        while (true) {
            let block = blocks[Math.floor(Math.random() * blocks.length)]
            if (block.constructor !== Static)
                continue;
            let randValue = Math.floor(Math.random() * 100);
            if (randValue <= block.naturalSpawnChance) {
                square.appendChild(block.images[0].cloneNode(true));
                break;
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
        return y * map.squaresPerRow + x;
    }

    getSquare(x, y) {
        return document.querySelectorAll('.square')[this.getSquareIndex(x, y)];
    }
}


//Map event Listener

function mouseDownMap(event) {
    const mapRect = map.map.getBoundingClientRect();
    const x = Math.floor((event.clientX - mapRect.left) / globalSize);
    const y = Math.floor((event.clientY - mapRect.top) / globalSize);
    if (map.isBorderOfMap(y, x))
        return;

    const square = map.getSquare(x, y);
    if (square.querySelectorAll("img").length > 1)
        return;

    if (event.button === 0 && handBlock != null) {
        handBlock.build(square);
    } else if (event.button === 2) {
        square.removeChild(square.querySelector('#ground'));
        square.appendChild(grounds[GROUND.GRASS_FARM].cloneNode(true));
    }
}

function mouseMoveEvent(event) {

}
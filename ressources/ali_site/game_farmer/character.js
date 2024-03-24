class Character {
    constructor(positionX, positionY, image, speed) {
        this.positionX = positionX;
        this.positionY = positionY;
        this.image = image;
        this.speed = speed;

        map.map.appendChild(image)
        this.image.style.position = 'absolute';
        this.image.style.left = positionX + 'px';
        this.image.style.top = positionY + 'px';
    }

     moveCharacter() {
        this.image.style.left = this.positionX + 'px';
        this.image.style.top = this.positionY + 'px';
        this.printCharacterPosition()
     }

     printCharacterPosition() {
        console.log("X: " + this.positionX, " Y: " + this.positionY)
     }

     moveListener(event) {
        if (event.key === 'ArrowUp')
            character.positionY -= character.speed;
        if (event.key === 'ArrowDown')
            character.positionY += character.speed;
        if (event.key === 'ArrowLeft')
            character.positionX -= character.speed;
        if (event.key === 'ArrowRight')
            character.positionX += character.speed;
        if (event.key === ' ')
            drawOnSquare();
        character.moveCharacter();
    }
}
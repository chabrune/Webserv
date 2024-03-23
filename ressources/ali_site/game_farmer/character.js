class Character {
    constructor(positionX, positionY, image) {
        this.positionX = positionX;
        this.positionY = positionY;
        this.image = image;
    }

     moveCharacter() {
        this.image.style.left = this.positionX + 'px';
        this.image.style.top = this.positionY + 'px';
        console.log("value" + this.positionX, this.positionY);
     }
}
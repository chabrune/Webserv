class Block {
    constructor(images, timeToGround) {
        this.images = images;
        this.timeToGrowth = timeToGround;
        console.log(this.images[0].src.split('/').pop());
        this.icon = images.pathname;
    }

}
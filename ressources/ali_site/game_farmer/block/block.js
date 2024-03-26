class Block {
    constructor(images) {
        if (this.constructor === Block)
            throw new Error("Abstract classes can't be instantiated.");
        this.images = images;
        this.icon = images.pathname;
    }

    build(square) {

    }
}
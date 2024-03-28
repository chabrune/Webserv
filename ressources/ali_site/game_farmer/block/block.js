class Block {
    constructor(images) {
        if (this.constructor === Block)
            throw new Error("Abstract classes can't be instantiated.");
        this.images = images;
        this.name = this.getNameFromImage()
        this.icon = loadImage("assets/image/icon/" + this.name.charAt(0).toLowerCase() + this.name.slice(1) + "_icon.png", "icon" ,0);
    }

    build(square) {

    }

    getNameFromImage() {
        let path = this.images[0].src.replace(/^.*[\\\/]/, '');

        path = path.charAt(0).toUpperCase() + path.slice(1);
        path = path.substring(0, path.indexOf('.'));
        if (isDigit(path[path.length - 1]))
            path = path.substring(0, path.length - 1);
        return path;
    }
}
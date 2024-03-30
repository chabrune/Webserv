class Block {
    constructor(images) {
        if (this.constructor === Block)
            throw new Error("Abstract classes can't be instantiated.");
        this.images = images;
        this.name = getNameFromImage(images[0]);
    }

    setIcon(icon) {
        this.icon = icon;
        return this;
    }

    setNaturalSpawnChance(naturalSpawnChance) {
        this.naturalSpawnChance = naturalSpawnChance;
        return this;
    }

    setBlockToSquare(square) {
        throw new Error("This function need to be implemented");
    }

}
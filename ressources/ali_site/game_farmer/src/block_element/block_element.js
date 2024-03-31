class BlockElement {
    constructor(images) {
        if (this.constructor === BlockElement)
            throw new Error("Abstract classes can't be instantiated.");
        this.images = images;
        this.name = this.images[0].getAttribute("id");
    }

    setIcon(icon) {
        this.icon = icon;
        return this;
    }

    setNaturalSpawnChance(naturalSpawnChance) {
        this.naturalSpawnChance = naturalSpawnChance;
        return this;
    }

    setBlockAction(blockAction) {
        this.blockAction = blockAction;
        return this;
    }

    setLootable(resource, resourceNumber = 1) {
        this.resource = resource;
        this.resourceNumber = resourceNumber;
        return this;
    }

    setBlockToSquare(square) {
        throw new Error("This function need to be implemented");
    }
}
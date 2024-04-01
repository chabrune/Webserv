class BlockElement {
    constructor(images) {
        if (this.constructor === BlockElement)
            throw new Error("Abstract classes can't be instantiated.");
        this.images = images;
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

    setDisplayName(displayName) {
        this.displayName = displayName;
        return this;
    }

    setElementToSquare(square) {
        throw new Error("This function need to be implemented");
    }
}
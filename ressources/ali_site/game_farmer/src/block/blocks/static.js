class Static extends Block {
	constructor(image, naturalSpawnChance) {
		super(image);
		this.naturalSpawnChance = naturalSpawnChance;
	}

	build(square) {
		square.appendChild(this.images[0].cloneNode(true));
	}
}
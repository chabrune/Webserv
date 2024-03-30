class Static extends Block {
	constructor(image) {
		super(image);
	}

	setBlockToSquare(square) {
		square.appendChild(this.images[0].cloneNode(true));
	}
}
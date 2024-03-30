class Static extends BlockElement {
	constructor(image) {
		super(image);
	}

	setBlockToSquare(square) {
		square.appendChild(this.images[0].cloneNode(true));
	}
}
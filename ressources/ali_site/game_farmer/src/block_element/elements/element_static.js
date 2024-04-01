class ElementStatic extends BlockElement {
	constructor(image) {
		super(image);
	}

	setElementToSquare(square) {
		square.appendChild(this.images[0].cloneNode(true));
	}
}
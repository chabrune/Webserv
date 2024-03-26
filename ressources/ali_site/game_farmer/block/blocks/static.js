class Static extends Block {
	constructor(image) {
		super(image);
	}

	build(square) {
		square.appendChild(this.images[0].cloneNode(true));
	}
}
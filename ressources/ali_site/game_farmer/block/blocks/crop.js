class Crop extends Block {
	constructor(images, timeToGrowth) {
		super(images);
		this.timeToGrowth = timeToGrowth;
	}

	build(square) {
		if (square.querySelector('#crops'))
			return;
		square.appendChild(this.images[0].cloneNode(true));
		for (let i= 1; i <= this.images.length - 1; i++) {
			setTimeout(() => { // Arrow function
				square.removeChild(square.querySelector('#crops'));
				square.appendChild(this.images[i].cloneNode(true));
			}, this.#cropGrowthCalculation(i));
		}
	}

	#cropGrowthCalculation(stage) {
		return (this.timeToGrowth * stage + (Math.random() * this.timeToGrowth)) * globalGrowthSpeed;
	}
}
class ElementCrop extends BlockElement {
	constructor(images, icon, displayName, timeToGrowth) {
		super(images);
		this.timeToGrowth = timeToGrowth;
		this.icon = icon;
		this.displayName = displayName;

		let htmlElement = document.getElementById('dropup-crop');

		let div = document.createElement('div');
		div.appendChild(this.icon);

		let spanHTML = `<span class="txt">${this.displayName}</span>`;

		div.insertAdjacentHTML('beforeend', spanHTML);
		htmlElement.appendChild(div);
	}

	setElementToSquare(square) {
		if (square.querySelector('#crops') || !square.querySelector('img#ground_farm'))
			return;
		square.appendChild(this.images[0].cloneNode(true));
		for (let i= 1; i <= this.images.length - 1; i++) {
			setTimeout(() => {
				square.removeChild(square.querySelectorAll('img')[1]);
				square.appendChild(this.images[i].cloneNode(true));
			}, this.#cropGrowthCalculation(i));
		}
	}

	#cropGrowthCalculation(stage) {
		return (this.timeToGrowth * stage + (Math.random() * this.timeToGrowth)) * globalGrowthSpeed;
	}
}
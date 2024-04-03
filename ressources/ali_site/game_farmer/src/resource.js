class Resource {
	constructor(displayName, icon) {
		this.displayName = displayName;
		this.icon = icon;

		const li = document.createElement("li")
		li.appendChild(this.icon.cloneNode(true));

		let spanHTML = `<span class="txt">0</span>`;

		li.insertAdjacentHTML('beforeend', spanHTML);
		resourceBarElement.appendChild(li)
	}

	getResourceFromId(id) {

	}

	addResourceToBar(number) {
		const img = resourceBarElement.querySelector(`li img[id="${this.icon.id}"]`);
		const li = img.closest("li").querySelector('span');
		console.log(li);
		li.textContent = (parseInt(li.textContent) + number).toString()
	}
}
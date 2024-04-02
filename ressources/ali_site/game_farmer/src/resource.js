class Resource {
	constructor(displayName, icon) {
		this.displayName = displayName;
		this.icon = icon;

		let htmlElement = document.getElementById('ressourcebar').getElementsByTagName('ul')[0];

		const li = document.createElement("li")
		li.appendChild(this.icon.cloneNode(true));

		let spanHTML = `<span class="txt">0</span>`;

		li.insertAdjacentHTML('beforeend', spanHTML);
		htmlElement.appendChild(li)
	}
}
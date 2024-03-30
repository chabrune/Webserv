class Resource {
	constructor(name, icon) {
		this.name = name;
		this.icon = icon;

		let htmlElement = document.getElementById('ressourcebar').getElementsByTagName('ul')[0];

		const li = document.createElement("li")
		li.appendChild(this.icon.cloneNode(true));

		let spanHTML = `<span class="txt">0</span>`;

		li.insertAdjacentHTML('beforeend', spanHTML);
		htmlElement.appendChild(li)
	}
}
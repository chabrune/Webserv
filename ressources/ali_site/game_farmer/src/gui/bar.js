class Bar {
    constructor(htmlElement) {
        this.htmlElement = htmlElement;
    }

    addChild(child) {
        this.htmlElement.appendChild(child);
    }

    getChildFromId(id) {
        const liList = this.htmlElement.getElementsByTagName("li");
        for (const liValue of liList ) {
            if (liValue.getElementsByTagName("img")[0].id === id)
             return liValue
        }
        return this.htmlElement.querySelector(`li img[id="${id}"]`);
    }
}
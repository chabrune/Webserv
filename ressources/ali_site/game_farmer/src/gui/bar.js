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

        }
        return this.htmlElement.getElementById(id);
    }
}
class BlockActionHarvest extends BlockAction {
	constructor() {
		super();
	}

	executor(square) {
		const block = getBlockFromId(square.querySelectorAll('img')[1].getAttribute("id"))
		block.getResource().addResourceToBar(block.getResourceNumber());
		square.removeChild(square.querySelectorAll('img')[1]);
		square.removeChild(square.querySelector('.ground'));
		square.appendChild(IMG.GRASS.cloneNode(true));
	}
}
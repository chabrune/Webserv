class BlockActionHarvest extends BlockAction {
	constructor() {
		super();
	}

	executor(square) {
		square.removeChild(square.querySelectorAll('img')[1]);
		//todo give resource
	}
}
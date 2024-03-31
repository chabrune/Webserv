function mouseDownMap(event) {
	const mapRect = map.map.getBoundingClientRect();
	const x = Math.floor((event.clientX - mapRect.left) / globalSize);
	const y = Math.floor((event.clientY - mapRect.top) / globalSize);
	if (map.isBorderOfMap(y, x))
		return;

	const square = map.getSquare(x, y);
	if (event.button === 0 && handBlock != null)
		handBlock.setBlockToSquare(square);
	else if (event.button === 2) {
		const block = map.getBlockFromSquare(square);
		if (block === null && !square.querySelector('#ground_farm')) {
			square.removeChild(square.querySelector('#ground'));
			square.appendChild(IMG.GRASS_FARM.cloneNode(true));
		}
		else if (block != null && block.blockAction !== undefined)
			block.blockAction.executor(square)
	}
}

function mouseMoveEvent(event) {
	const mapRect = map.map.getBoundingClientRect();
	const x = Math.floor((event.clientX - mapRect.left) / globalSize);
	const y = Math.floor((event.clientY - mapRect.top) / globalSize);
	const square = map.getSquare(x, y);
	if (map.isBorderOfMap(y, x))
		return;

	if (map.hoveredSquare === square)
		return;
	if (map.hoveredSquare != null)
		map.hoveredSquare.querySelector("img.ground").classList.remove("square-hover-effect");
	square.querySelector("img.ground").classList.add("square-hover-effect");
	map.hoveredSquare = square;

}
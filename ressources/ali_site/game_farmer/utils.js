    function loadImage(imagePath, square) {
    const img = new Image();
    img.src = imagePath;
    img.onerror = function() {
        console.error(`Failed to load image into square`);
    };
    return img;
}
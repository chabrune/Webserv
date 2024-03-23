function loadImage(imagePath) {
    const img = new Image();
    img.src = imagePath;
    img.onerror = function() {
        console.error(`Failed to load image into square`);
    };
    return img;
}

function getPercent(value, percent) {
    return (value * percent) / 100;
}

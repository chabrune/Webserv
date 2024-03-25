function loadImage(imagePath, id, z) {
    const img = new Image();
    img.src = imagePath;
    img.onerror = function() {
        console.error(`Failed to load image into square`);
    };
    img.setAttribute('id', id);
    img.style.zIndex = z;
    return img;
}

function loadImages(imagePath, id, z, number) {
    let imgs = [];

    for (let i = 0; i <= number; i++) {
        const img = new Image();
        img.src = imagePath + i + ".png";
        img.onerror = function() {
            console.error(`Failed to load image into square`);
        };
        img.setAttribute('id', id);
        img.style.zIndex = z;
        imgs.push(img);
    }
    return imgs;
}

function getPercent(value, percent) {
    return (value * percent) / 100;
}

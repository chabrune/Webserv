let mapWallPic1 = document.createElement('img');
let imagesToLoad = 0;

function loadImages() {

    let imageList = [
        {varName: mapWallPic1, fileName: 'wall.png'}
    ]

    imagesToLoad = imageList.length;

    for (let i=0; i < imageList.length; i++) {
        if(imageList[i].varName !== undefined) {
            beginLoadingImage(imageList[i].varName, imageList[i].fileName)
        }
    }
}

function beginLoadingImage(imgVar, fileName) {
    imgVar.src = fileName;
}

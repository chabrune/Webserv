let frameCount, frameRate, lastFrame, frameTime, deltaTime;

let frameTimes = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
let frameTimeIndex = 0;
let showFPS = true;

function updateFrameTimes() {
    frameCount++;
    frameTime = performance.now() - lastFrame;
    deltaTime = frameTime / UPDATE_INTERVAL;
    lastFrame = performance.now();
}

function drawFramerateCounter() {
    ctx.fillStyle = 'white';
    ctx.font = '20pt Arial';
    ctx.textAlign = 'left';
    ctx.fillText('FPS: '+frameRate, 10, 30);
}

function averageFramerate(newFrameTime) {
    let frameTimeTotal = 0;
    frameTimes[frameTimeIndex] = newFrameTime;
    for (let i=0; i<frameTimes.length; i++) {
        frameTimeTotal += frameTimes[i];
    }
    frameTimeIndex++;
    //Return average of frame times in frames per second
    return Math.round(1000 / (frameTimeTotal / frameTimes.length));
}

let canvas, ctx;
let width, height, midheight;
let player;
let scaleX = 32;
let scaleY = 32;
let keyHeldUp, keyHeldLeft, keyHeldDown, keyHeldRight, keyHeldTurnLeft, keyHeldTurnRight;

const FOV = 1.5708;
const RAYNUMBER = 640;
const SPEED = 5;
const ROTATESPEED = 5;
const playerSize = 3;
const UPDATE_INTERVAL = 1000 / 60;
// 0=Nothing 1=Wall 2=PlayerStart
const map = [
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1],
    [1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1],
    [1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
];

const ROTATE = 0.0174533 * ROTATESPEED;

class Player {
    x = 304;
    y = 240;
    angle = 0.0;
}

window.onload = function(){
    canvas = document.getElementById('gameCanvas');
    ctx = canvas.getContext('2d');
    width = canvas.width;
    height = canvas.height;
    midheight = Math.round(height / 2);
    player = new Player;

    document.addEventListener('keydown', handleKeyDown);
    document.addEventListener('keyup', handleKeyUp);

    run();
}

document.onvisibilitychange = function() {
    lastFrame = performance.now() - UPDATE_INTERVAL;
}

function drawMap() {
    for (let y = 0; y < map.length; y++) {
        for (let x = 0; x < map[y].length; x++) {
            if (map[y][x] === 1) {
                ctx.fillStyle = 'grey';
            } else {
                ctx.fillStyle = 'lightgray';
            }
            let drawX = x * scaleX;
            let drawY = y * scaleY;
            ctx.fillRect(drawX, drawY, scaleX, scaleY);
        }
    }
}

function drawPlayer() {
    ctx.beginPath();
    ctx.arc(player.x, player.y, playerSize, 0, 2 * Math.PI, false);
    ctx.lineWidth = 1;
    ctx.strokeStyle = '#FF0000';
    ctx.stroke();
}

function drawRay() {
    let angle = player.angle;
    let rayLength = width;
    let firstRay = player.angle - (FOV / 2);
    let rayStep = FOV / RAYNUMBER;
    for (let r = 0; r < RAYNUMBER; r++) {
        let currentX = player.x;
        let currentY = player.y;
        let stepX = Math.cos(firstRay + (rayStep * r));
        let stepY = Math.sin(firstRay + (rayStep * r));
        for (let i = 0; i < rayLength; i++) {
            currentX += stepX;
            currentY += stepY;
            let mapX = Math.floor(currentX / scaleX);
            let mapY = Math.floor(currentY / scaleY);
            if (map[mapY][mapX] === 1) {
                ctx.beginPath();
                ctx.moveTo(player.x, player.y);
                ctx.lineTo(currentX, currentY);
                ctx.lineWidth = 0.5
                ctx.strokeStyle = 'blue';
                ctx.stroke();
                break;
            }
        }
    }
}

function run() {
    move();
    ctx.clearRect(0, 0, width, height);
    drawMap();
    drawPlayer();
    drawRay();
    requestAnimationFrame(run);
}

function regAngle(a) {
    if (a > 1)
        return (1);
    else if (a < -1)
        return (-1);
    return (a);
}

function move() {
    if (keyHeldTurnRight === true) {
        player.angle += ROTATE;
    }
    if (keyHeldTurnLeft === true) {
        player.angle -= ROTATE;
    }
    if (keyHeldUp === true) {
        let step_x = Math.cos(player.angle) * SPEED;
        let step_y = Math.sin(player.angle) * SPEED;
        if (map[Math.floor(player.y / 32)][Math.floor((player.x + step_x) / 32)] !== 1)
            player.x += step_x;
        if (map[Math.floor((player.y + step_y) / 32)][Math.floor(player.x / 32)] !== 1)
            player.y += step_y;
    }
    if (keyHeldDown === true) {
        let step_x = Math.cos(player.angle) * SPEED;
        let step_y = Math.sin(player.angle) * SPEED;
        if (map[Math.floor(player.y / 32)][Math.floor((player.x - step_x) / 32)] !== 1)
            player.x -= step_x;
        if (map[Math.floor((player.y - step_y) / 32)][Math.floor(player.x / 32)] !== 1)
            player.y -= step_y;
    }
    if (keyHeldRight === true) {
        let dirx = Math.cos(player.angle);
        let diry = Math.sin(player.angle);
        let alpha;

        if (diry > 0)
            alpha = Math.acos(regAngle(dirx));
        else
            alpha = -Math.acos(regAngle(dirx));
        let step_y = Math.sin(alpha + (Math.PI / 2)) * (SPEED);
        let step_x = Math.cos(alpha + (Math.PI / 2)) * (SPEED);
        if (map[Math.floor(player.y / 32)][Math.floor((player.x + step_x) / 32)] !== 1)
            player.x += step_x;
        if (map[Math.floor((player.y + step_y) / 32)][Math.floor(player.x / 32)] !== 1)
            player.y += step_y;
    }
    if (keyHeldLeft === true) {
        let dirx = Math.cos(player.angle);
        let diry = Math.sin(player.angle);
        let alpha;

        if (diry > 0)
            alpha = Math.acos(regAngle(dirx));
        else
            alpha = -Math.acos(regAngle(dirx));
        let step_y = Math.sin(alpha + (Math.PI / 2)) * (SPEED);
        let step_x = Math.cos(alpha + (Math.PI / 2)) * (SPEED);
        if (map[Math.floor(player.y / 32)][Math.floor((player.x - step_x) / 32)] !== 1)
            player.x -= step_x;
        if (map[Math.floor((player.y - step_y) / 32)][Math.floor(player.x / 32)] !== 1)
            player.y -= step_y;
    }
}
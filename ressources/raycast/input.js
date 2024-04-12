const KEY_W = 87;
const KEY_A = 65;
const KEY_S = 83;
const KEY_D = 68;
const KEY_Q = 81;
const KEY_E = 69;
const KEY_ARROW_LEFT = 37;
const KEY_ARROW_UP = 38;
const KEY_ARROW_RIGHT = 39;
const KEY_ARROW_DOWN = 40;

function handleKeyDown(evt) {
    if (evt.keyCode === KEY_W || evt.keyCode === KEY_ARROW_UP) {
        keyHeldUp = true;
    }
    if (evt.keyCode === KEY_A) {
        keyHeldLeft = true;
    }
    if (evt.keyCode === KEY_S || evt.keyCode === KEY_ARROW_DOWN) {
        keyHeldDown = true;
    }
    if (evt.keyCode === KEY_D) {
        keyHeldRight = true;
    }
    if (evt.keyCode === KEY_E || evt.keyCode === KEY_ARROW_RIGHT) {
        keyHeldTurnRight = true;
    }
    if (evt.keyCode === KEY_Q || evt.keyCode === KEY_ARROW_LEFT) {
        keyHeldTurnLeft = true;
    }
}

function handleKeyUp(evt) {
    if (evt.keyCode === KEY_W || evt.keyCode === KEY_ARROW_UP) {
        keyHeldUp = false;
    }
    if (evt.keyCode === KEY_A) {
        keyHeldLeft = false;
    }
    if (evt.keyCode === KEY_S || evt.keyCode === KEY_ARROW_DOWN) {
        keyHeldDown = false;
    }
    if (evt.keyCode === KEY_D) {
        keyHeldRight = false;
    }
    if (evt.keyCode === KEY_E || evt.keyCode === KEY_ARROW_RIGHT) {
        keyHeldTurnRight = false;
    }
    if (evt.keyCode === KEY_Q || evt.keyCode === KEY_ARROW_LEFT) {
        keyHeldTurnLeft = false;
    }
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
        if (map[Math.floor(player.y / SCALE)][Math.floor((player.x + step_x) / SCALE)] !== 1)
            player.x += step_x;
        if (map[Math.floor((player.y + step_y) / SCALE)][Math.floor(player.x / SCALE)] !== 1)
            player.y += step_y;
    }
    if (keyHeldDown === true) {
        let step_x = Math.cos(player.angle) * SPEED;
        let step_y = Math.sin(player.angle) * SPEED;
        if (map[Math.floor(player.y / SCALE)][Math.floor((player.x - step_x) / SCALE)] !== 1)
            player.x -= step_x;
        if (map[Math.floor((player.y - step_y) / SCALE)][Math.floor(player.x / SCALE)] !== 1)
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
        if (map[Math.floor(player.y / SCALE)][Math.floor((player.x + step_x) / SCALE)] !== 1)
            player.x += step_x;
        if (map[Math.floor((player.y + step_y) / SCALE)][Math.floor(player.x / SCALE)] !== 1)
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
        if (map[Math.floor(player.y / SCALE)][Math.floor((player.x - step_x) / SCALE)] !== 1)
            player.x -= step_x;
        if (map[Math.floor((player.y - step_y) / SCALE)][Math.floor(player.x / SCALE)] !== 1)
            player.y -= step_y;
    }
}
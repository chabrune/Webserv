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
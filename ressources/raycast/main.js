
let canvas, ctx;
let width, height, midheight;
let player;
const mapSize = 100;
const mapZoomOut = 6;
const SCALE = 64;
let keyHeldUp, keyHeldLeft, keyHeldDown, keyHeldRight, keyHeldTurnLeft, keyHeldTurnRight;

const FOV = 1.5708;
const RAYNUMBER = 1280;
const SPEED = 5;
const ROTATESPEED = 5;
const playerSize = 3;
const UPDATE_INTERVAL = 1000 / 60;
const FOG_DISTANCE = 500;
// 0=Nothing 1=Wall 2=PlayerStart
const map = [
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
];

const ROTATE = 0.0174533 * ROTATESPEED;

class Player {
    x = 0;
    y = 0;
    angle = 0.0;
}

class Dda {
    ray;
    r_xy = [];
    v_xy = [];
    o_xy = [];
    cos;
    sin;
    dist_hv = [];
    current_angle;
    current_angle_fix;
    hit_hv;
    hit_direction = [];
    chunk_hv = [];
}

class s_col_drawing
{
    iterator;
    color;
    ceil_floor_line_h;
    wall_line_h;
    step;
    current_step;
    fog_strength;
    hit_block;
    floor_d;
}

function initPlayerPos() {
    for (let y = 0; y < map.length; y++) {
        for (let x = 0; x < map[y].length; x++) {
            if (map[y][x] === 2) {
                player.x = x * SCALE + (SCALE / 2);
                player.y = y * SCALE + (SCALE / 2);
                return;
            }
        }
    }
}

window.onload = function(){
    canvas = document.getElementById('gameCanvas');
    ctx = canvas.getContext('2d');
    width = canvas.width;
    height = canvas.height;
    midheight = Math.floor(height / 2);
    player = new Player;

    document.addEventListener('keydown', handleKeyDown);
    document.addEventListener('keyup', handleKeyUp);

    initPlayerPos();
    loadImages();
    run();
}

document.onvisibilitychange = function() {
    lastFrame = performance.now() - UPDATE_INTERVAL;
}

function drawMap() {
    let startX = ((player.x + map[0].length) / 4) - mapSize;
    let startY = ((player.y + map.length) / 4) - mapSize;

    for (let y = 0; y < mapSize * 2; y++) {
        let convY = Math.floor((y + startY) / 16);
        for (let x = 0; x < mapSize * 2; x++) {
            let convX = Math.floor((x + startX) / 16);
            let distance = Math.sqrt(Math.pow(x - mapSize, 2) + (Math.pow(y - mapSize, 2)));
            if (distance > mapSize)
                continue;
            if (convY < 0 || convX < 0 || convY > map.length - 1 || convX > map[0].length - 1
                || map[convY][convX] === 1) {
                ctx.fillStyle = 'black';
                ctx.fillRect(x, y, 1, 1);
            } else {
                ctx.fillStyle = 'grey';
                ctx.fillRect(x, y, 1, 1);
            }
        }
    }
    ctx.beginPath();
    ctx.arc(mapSize - 2.5, mapSize - 2.5, 2, 0, 2 * Math.PI, false);
    ctx.lineWidth = 1;
    ctx.strokeStyle = '#FF0000';
    ctx.stroke();
}

function jump_to_next(dda, val)
{
    let m_xy = [];

    while (1)
    {
        m_xy[0] = Math.floor(dda.r_xy[0] / SCALE);
        m_xy[1] = Math.floor(dda.r_xy[1] / SCALE);
        if (dda.r_xy[0] < 0 || dda.r_xy[1] < 0)
            break ;
        if (map[m_xy[1] % map.length][m_xy[0] % map[m_xy[1] % map.length].length] === 1)
        {
            dda.dist_hv[val] = dda.cos * (dda.r_xy[0] - player.x)
                - -dda.sin * (dda.r_xy[1] - player.y);
            break ;
        }
        dda.r_xy[0] += dda.o_xy[0];
        dda.r_xy[1] += dda.o_xy[1];
    }
}

function horizontal_cast(dda)
{
    let tan;

    tan = -1.0 / Math.tan(dda.current_angle);
    dda.r_xy[0] = player.x;
    dda.r_xy[1] = player.y;
    if (dda.sin > 0.001)
    {
        dda.r_xy[1] = (Math.floor(player.y / SCALE) * SCALE) + SCALE;
        dda.r_xy[0] = (player.y - dda.r_xy[1]) * tan + player.x;
        dda.o_xy[1] = SCALE;
        dda.o_xy[0] = -dda.o_xy[1] * tan;
        dda.hit_direction[1] = 2;
    }
    else if (dda.sin < -0.001)
    {
        dda.r_xy[1] = (Math.floor(player.y / SCALE) * SCALE) - 0.0002;
        dda.r_xy[0] = (player.y - dda.r_xy[1]) * tan + player.x;
        dda.o_xy[1] = -SCALE;
        dda.o_xy[0] = -dda.o_xy[1] * tan;
        dda.hit_direction[1] = 0;
    }
    if (dda.hit_direction[1] >= 0)
        jump_to_next(dda, 0);
}

function	vertical_cast(dda)
{
    let tan;

    tan = -Math.tan(dda.current_angle);
    dda.r_xy[0] = player.x;
    dda.r_xy[1] = player.y;
    if (dda.cos > 0.001)
    {
        dda.r_xy[0] = (Math.floor(player.x / SCALE) * SCALE) + SCALE;
        dda.r_xy[1] = (player.x - dda.r_xy[0]) * tan + player.y;
        dda.o_xy[0] = SCALE;
        dda.o_xy[1] = -dda.o_xy[0] * tan;
        dda.hit_direction[0] = 1;
    }
    else if (dda.cos < -0.001)
    {
        dda.r_xy[0] = (Math.floor(player.x / 64) * 64) - 0.0002;
        dda.r_xy[1] = (player.x - dda.r_xy[0]) * tan + player.y;
        dda.o_xy[0] = -64;
        dda.o_xy[1] = -dda.o_xy[0] * tan;
        dda.hit_direction[0] = 3;
    }
    if (dda.hit_direction[0] > 0)
        jump_to_next(dda, 1);
    dda.v_xy[0] = dda.r_xy[0];
    dda.v_xy[1] = dda.r_xy[1];
}

function	fisheyes_fixor(dda)
{
    let two_pi;

    dda.current_angle_fix = player.angle - dda.current_angle;
    two_pi = 2 * Math.PI;
    if (dda.current_angle_fix < 0)
        dda.current_angle_fix += two_pi;
    if (dda.current_angle_fix > two_pi)
        dda.current_angle_fix -= two_pi;
    if (dda.dist_hv[0] < 10000)
        dda.dist_hv[0] = dda.dist_hv[0] * Math.cos(dda.current_angle_fix);
}

function	setup_col_struct(tcd, dda)
{
    let convertor;
    let wall_height;

    tcd.wall_line_h = 0;
    tcd.iterator = 0;
    if (dda.dist_hv[0] >= 10000)
    {
        tcd.ceil_floor_line_h = height / 2;
        return ;
    }
    wall_height = (height * SCALE) / dda.dist_hv[0];
    tcd.floor_d = Math.cos(dda.current_angle - player.angle);
    tcd.ceil_floor_line_h = (height - wall_height) / 2;
    convertor = (SCALE * height) / dda.dist_hv[0];
    tcd.wall_line_h = (height + wall_height) / 2;
    tcd.step = SCALE / convertor;
    tcd.current_step = 0.0;
    tcd.fog_strength = dda.dist_hv[dda.hit_hv] / FOG_DISTANCE;
    tcd.hit_block = map[Math.floor((dda.r_xy[1] / SCALE) % map.length)][Math.floor(dda.r_xy[0] / SCALE) % map[Math.floor((dda.r_xy[1] / SCALE) % map.length)].length];
    if (tcd.wall_line_h > height)
    {
        tcd.current_step = (convertor - height) / 2.0;
        tcd.wall_line_h = height;
    }
    tcd.current_step *= tcd.step;
    if (dda.hit_hv === 1 && dda.hit_direction[0] === 3)
    dda.r_xy[1] = (SCALE - 1) - (dda.r_xy[1] % SCALE);
else if (dda.hit_hv === 0 && dda.hit_direction[1] === 2)
    dda.r_xy[0] = (SCALE - 1) - (dda.r_xy[0] % SCALE);
}

function	columns_drawing(dda)
{
    let tcd = new s_col_drawing();

    setup_col_struct(tcd, dda);
    ctx.beginPath();
    ctx.moveTo(dda.ray, 0);
    ctx.lineTo(dda.ray, tcd.ceil_floor_line_h);
    ctx.lineWidth = 1;
    ctx.strokeStyle = 'lightblue';
    ctx.stroke();

    ctx.beginPath();
    ctx.moveTo(dda.ray,tcd.ceil_floor_line_h);
    ctx.lineTo(dda.ray, tcd.wall_line_h);
    ctx.lineWidth = 1;
    ctx.strokeStyle = 'grey';
    ctx.stroke();

    ctx.beginPath();
    ctx.moveTo(dda.ray, tcd.wall_line_h);
    ctx.lineTo(dda.ray, height);
    ctx.lineWidth = 1;
    ctx.strokeStyle = 'darkgreen';
    ctx.stroke();
}

function	raycasting()
{
    let start_angle;
    //let dists = [];
    let dda = new Dda;

    dda.ray = -1;
    start_angle = player.angle - (FOV / 2);
    while (dda.ray++ < RAYNUMBER - 1)
    {
        dda.hit_direction[0] = -1;
        dda.hit_direction[1] = -1;
        dda.dist_hv[0] = 100000;
        dda.dist_hv[1] = 100000;
        dda.hit_hv = 0;
        dda.current_angle = start_angle + (dda.ray * (FOV / RAYNUMBER));
        if (dda.current_angle < 0)
            dda.current_angle += 6.28319;
        if (dda.current_angle > 6.28319)
        dda.current_angle -= 6.28319;
        dda.cos = Math.cos(dda.current_angle);
        dda.sin = Math.sin(dda.current_angle);
        vertical_cast(dda);
        horizontal_cast(dda);
        if (dda.dist_hv[1] < dda.dist_hv[0])
        {
            dda.r_xy[0] = dda.v_xy[0];
            dda.r_xy[1] = dda.v_xy[1];
            dda.dist_hv[0] = dda.dist_hv[1];
            dda.hit_hv = 1;
            dda.chunk_hv[0] = dda.chunk_hv[1];
        }
        //dists[dda.ray] = dda.dist_hv[dda.hit_hv];
        fisheyes_fixor(dda);
        columns_drawing(dda);
    }
    //draw_sprites(player, dists);
}

function run() {
    move();
    ctx.clearRect(0, 0, width, height);
    raycasting();
    drawMap();
    requestAnimationFrame(run);
}

function regAngle(a) {
    if (a > 1)
        return (1);
    else if (a < -1)
        return (-1);
    return (a);
}

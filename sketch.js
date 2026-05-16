// https://editor.p5js.org/dschoo7705/sketches/lgOSkNofb

let radius;
let depth = 4;
let sides;

let minRadius = 225;
let maxRadius = 1000;
let radiusRange = maxRadius - minRadius;

let minSides = 5;
let maxSides = 21;
let sidesRange = maxSides - minSides;

let radiusTheta;
let sidesTheta;
let thetaTheta;
let theta = 0;

let h = 0;

class Settings {
  constructor() {
    this.speed = 2.0;
    this.brightness = 0.3;
    this.show = true;
  }
}

let speed = 0.2;

let defaultW = 600;
let defaultH = defaultW;

let settings;
let gui;

function setup() {
  createCanvas(displayWidth, displayHeight);
  colorMode(HSL, 360, 100, 100);

  radiusTheta = -PI / 5;
  sidesTheta = PI / 7;

  settings = new Settings;

  gui = new dat.GUI({name: 'Settings'});
  gui.add(settings, "speed", 0.1, 100.0, 0.1);
  gui.add(settings, "brightness", 0.0, 1.0, 0.01)
}

function nGonFractal(x, y, sides, radius, depth) {
  if (depth == 0) {
    return;
  }

  beginShape();
  for (let i = 0; i < sides; i++) {
    let angle = ((2 * PI) / sides) * i;
    let vX = x + cos(angle) * radius;
    let vY = y + sin(angle) * radius;
    vertex(vX, vY);

    nGonFractal(vX, vY, sides, radius, depth - 1);
  }
  endShape(CLOSE);
}

function keyPressed() {
  if (key == "`") {
    if (settings.show) {
      gui.hide();
      settings.show = false;
    } else {
      gui.show();
      settings.show = true;
    }
  }
}

function draw() {
  background(0);
  translate(width / 2, height / 2);
  noFill();
  strokeWeight(settings.brightness);

  h += deltaTime * 0.0007 * settings.speed;
  h = h % 360;
  stroke(h, 100, 50);

  radiusTheta += deltaTime * 0.00003 * settings.speed;
  sidesTheta += deltaTime * 0.000015 * settings.speed;
  thetaTheta += deltaTime * 0.0000025 * settings.speed;
  theta = sin(thetaTheta) * 3 * PI;

  rotate(theta);

  let newRadius = minRadius + (sin(radiusTheta) + 1) * 0.5 * radiusRange;
  let newSides = minSides + (sin(sidesTheta) + 1) * 0.5 * sidesRange;
  radius = newRadius;
  sides  = newSides;

  nGonFractal(
    0,
    0,
    sides,
    radius,
    depth
  );
}

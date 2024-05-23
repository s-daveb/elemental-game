
let previousTime = Date.now();

function updateBall(Ball) {
    const currentTime = Date.now();
    const deltaTime = (currentTime - previousTime) / 1000; // Convert milliseconds to seconds

    previousTime= currentTime;

	let x = Ball.x;
	let y = Ball.y
	let velocityX = Ball.velocityX;
	let velocityY = Ball.velocityY;

    // Apply acceleration to velocity
    velocityX += Ball.acceleration * deltaTime;
    velocityY += Ball.acceleration * deltaTime;

    // Update ball position based on updated velocity
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;

    // Bounce off the walls
    let right_threshold = 720 - Ball.width;
    let bottom_threshold = 1280 - Ball.height;
    if (x < 0 || x > right_threshold) {
        velocityX = -velocityX;
        x = Math.max(0, Math.min(x, right_threshold));
    }
    if (y < 0 || y > bottom_threshold) {
        velocityY = -velocityY;
        y = Math.max(0, Math.min(y, bottom_threshold));
    }

	Ball.x = x;
	Ball.y = y;
	Ball.velocityX = velocityX;
	Ball.velocityY = velocityY;

    return Ball
}

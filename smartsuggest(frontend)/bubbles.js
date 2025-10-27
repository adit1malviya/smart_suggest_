const canvas = document.getElementById('bubbleCanvas');
const ctx = canvas.getContext('2d');

canvas.width = window.innerWidth;
canvas.height = window.innerHeight;

const bubbles = [];
const bubbleCount = 80; // number of bubbles

// Create random bubbles
for (let i = 0; i < bubbleCount; i++) {
  bubbles.push({
    x: Math.random() * canvas.width,
    y: Math.random() * canvas.height,
    radius: Math.random() * 20 + 5, // small to medium bubbles
    speed: Math.random() * 1 + 0.2,
    alpha: Math.random() * 0.3 + 0.1 // transparent
  });
}

// Animation loop
function animate() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);

  for (let i = 0; i < bubbles.length; i++) {
    let b = bubbles[i];
    ctx.beginPath();
    ctx.arc(b.x, b.y, b.radius, 0, Math.PI * 2);
    ctx.fillStyle = `rgba(255,255,255,${b.alpha})`; // transparent white
    ctx.fill();
    ctx.closePath();

    // move bubble up
    b.y -= b.speed;
    if (b.y + b.radius < 0) {
      b.y = canvas.height + b.radius; // wrap to bottom
      b.x = Math.random() * canvas.width;
      b.radius = Math.random() * 20 + 5;
      b.speed = Math.random() * 1 + 0.2;
      b.alpha = Math.random() * 0.3 + 0.1;
    }
  }

  requestAnimationFrame(animate);
}

animate();

// Handle resize
window.addEventListener('resize', () => {
  canvas.width = window.innerWidth;
  canvas.height = window.innerHeight;
});

/*
 * C3WiFi — Psychedelic Captive Portal  (multi-mode)
 * ESP32-C3 Super Mini · WiFi AP
 *
 * Modes: Mandala   /mandala
 *        Plasma    /plasma
 *        Fractal   /fractal
 *        Matrix    /matrix
 *        Cyber     /cyber
 *        Binary    /binary
 *        Starfield /starfield
 *        Particles /particles
 *        Tunnel    /tunnel
 *
 * LED: GPIO8 heartbeat blink (active LOW)
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

const char* AP_SSID   = "COSMIC-C3";
const byte  DNS_PORT  = 53;
const char* PORTAL_IP = "192.168.4.1";
#define LED_PIN 8

DNSServer dnsServer;
WebServer server(80);

// ─────────────────────────────────────────────────────────────────────────────
//  INDEX  —  Mode Selector  (9 modes)
// ─────────────────────────────────────────────────────────────────────────────
static const char INDEX_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:radial-gradient(ellipse at 50% 50%,#0d003d,#000010 70%);
min-height:100vh;display:flex;flex-direction:column;align-items:center;
font-family:'Courier New',monospace;color:#fff;padding:20px 12px 28px}
.glw{position:fixed;border-radius:50%;filter:blur(90px);z-index:-1;pointer-events:none}
.g1{width:350px;height:350px;top:-120px;left:-120px;background:rgba(131,56,236,.3)}
.g2{width:300px;height:300px;bottom:-100px;right:-100px;background:rgba(6,255,208,.18)}
h1{font-size:clamp(1.1rem,5vw,1.7rem);letter-spacing:7px;text-align:center;margin-bottom:3px;
background:linear-gradient(90deg,#ff006e,#ff6b00,#ffd700,#06ffd0,#3a86ff,#8338ec,#ff006e);
-webkit-background-clip:text;-webkit-text-fill-color:transparent;background-clip:text;
filter:drop-shadow(0 0 10px rgba(131,56,236,.7))}
.sub{font-size:.5rem;letter-spacing:8px;color:rgba(0,255,209,.45);margin-bottom:16px;text-align:center}
.cat{width:min(460px,92vw);font-size:.48rem;letter-spacing:5px;color:rgba(255,255,255,.3);
padding:10px 2px 4px;border-bottom:1px solid rgba(255,255,255,.06);margin-bottom:7px}
.grid{display:grid;grid-template-columns:repeat(auto-fill,minmax(185px,1fr));
gap:7px;width:min(460px,92vw);margin-bottom:4px}
.card{display:block;padding:10px 13px;border-radius:10px;text-decoration:none;
background:rgba(10,0,40,.6);border:1px solid;transition:transform .15s,box-shadow .15s}
.card:hover,.card:active{transform:scale(1.03)}
.c1{border-color:rgba(131,56,236,.5);box-shadow:0 0 10px rgba(131,56,236,.08)}.c1:hover{box-shadow:0 0 22px rgba(131,56,236,.38)}
.c2{border-color:rgba(255,107,0,.45);box-shadow:0 0 10px rgba(255,107,0,.06)}.c2:hover{box-shadow:0 0 22px rgba(255,107,0,.32)}
.c3{border-color:rgba(6,255,208,.38);box-shadow:0 0 10px rgba(6,255,208,.06)}.c3:hover{box-shadow:0 0 22px rgba(6,255,208,.28)}
.c4{border-color:rgba(0,255,65,.45);box-shadow:0 0 10px rgba(0,255,65,.06)}.c4:hover{box-shadow:0 0 22px rgba(0,255,65,.35)}
.c5{border-color:rgba(255,0,170,.45);box-shadow:0 0 10px rgba(255,0,170,.06)}.c5:hover{box-shadow:0 0 22px rgba(255,0,170,.38)}
.c6{border-color:rgba(0,136,255,.45);box-shadow:0 0 10px rgba(0,136,255,.06)}.c6:hover{box-shadow:0 0 22px rgba(0,136,255,.38)}
.c7{border-color:rgba(180,180,255,.38);box-shadow:0 0 10px rgba(180,180,255,.06)}.c7:hover{box-shadow:0 0 22px rgba(180,180,255,.3)}
.c8{border-color:rgba(255,215,0,.38);box-shadow:0 0 10px rgba(255,215,0,.06)}.c8:hover{box-shadow:0 0 22px rgba(255,215,0,.3)}
.c9{border-color:rgba(255,80,0,.45);box-shadow:0 0 10px rgba(255,80,0,.06)}.c9:hover{box-shadow:0 0 22px rgba(255,80,0,.38)}
.cA{border-color:rgba(255,60,60,.42);box-shadow:0 0 10px rgba(255,60,60,.06)}.cA:hover{box-shadow:0 0 22px rgba(255,60,60,.35)}
.cB{border-color:rgba(0,210,255,.38);box-shadow:0 0 10px rgba(0,210,255,.06)}.cB:hover{box-shadow:0 0 22px rgba(0,210,255,.32)}
.cC{border-color:rgba(255,195,0,.42);box-shadow:0 0 10px rgba(255,195,0,.06)}.cC:hover{box-shadow:0 0 22px rgba(255,195,0,.32)}
.cD{border-color:rgba(160,255,80,.38);box-shadow:0 0 10px rgba(160,255,80,.06)}.cD:hover{box-shadow:0 0 22px rgba(160,255,80,.3)}
.cE{border-color:rgba(255,140,190,.38);box-shadow:0 0 10px rgba(255,140,190,.06)}.cE:hover{box-shadow:0 0 22px rgba(255,140,190,.3)}
.cF{border-color:rgba(90,190,255,.38);box-shadow:0 0 10px rgba(90,190,255,.06)}.cF:hover{box-shadow:0 0 22px rgba(90,190,255,.3)}
.icon{font-size:1.25rem;margin-bottom:3px;display:block}
.name{font-size:.7rem;letter-spacing:3px;font-weight:bold;display:block;margin-bottom:2px}
.desc{font-size:.4rem;letter-spacing:.7px;opacity:.46;display:block}
.n1{color:#c77dff}.n2{color:#ff9500}.n3{color:#06ffd0}.n4{color:#00ff41}.n5{color:#ff00aa}
.n6{color:#44aaff}.n7{color:#b4b4ff}.n8{color:#ffd700}.n9{color:#ff6622}
.nA{color:#ff5555}.nB{color:#00ddff}.nC{color:#ffcc00}.nD{color:#aaff66}.nE{color:#ffaacc}.nF{color:#88ccff}
footer{margin-top:16px;font-size:.4rem;letter-spacing:4px;color:rgba(255,255,255,.07);text-align:center}
</style></head><body>
<div class="glw g1"></div><div class="glw g2"></div>
<h1>COSMIC-C3</h1><p class="sub">SELECT TRANSMISSION MODE</p>

<div class="cat">&#x25C6; MATRIX RAIN</div>
<div class="grid">
  <a class="card c4" href="/matrix">
    <span class="icon">&#x2328;</span>
    <span class="name n4">MATRIX</span>
    <span class="desc">CLASSIC &middot; GREEN PHOSPHOR &middot; KANA</span>
  </a>
  <a class="card c5" href="/cyber">
    <span class="icon">&#x26A1;</span>
    <span class="name n5">CYBER RAIN</span>
    <span class="desc">NEON &middot; MULTICOLOR &middot; GLOW</span>
  </a>
  <a class="card c6" href="/binary">
    <span class="icon">&#x2B1B;</span>
    <span class="name n6">BINARY</span>
    <span class="desc">ZERO ONE &middot; ELECTRIC BLUE</span>
  </a>
  <a class="card c4" href="/mfire">
    <span class="icon">&#x1F525;</span>
    <span class="name n4">FIRE RAIN</span>
    <span class="desc">YELLOW &#x2192; ORANGE &#x2192; RED</span>
  </a>
  <a class="card cB" href="/mice">
    <span class="icon">&#x2744;</span>
    <span class="name nB">ICE RAIN</span>
    <span class="desc">WHITE &#x2192; CYAN &#x2192; BLUE</span>
  </a>
  <a class="card c7" href="/mstorm">
    <span class="icon">&#x26A1;</span>
    <span class="name n7">STORM RAIN</span>
    <span class="desc">LIGHTNING &middot; WHITE &amp; PURPLE</span>
  </a>
  <a class="card cA" href="/mblood">
    <span class="icon">&#x25CF;</span>
    <span class="name nA">BLOOD RAIN</span>
    <span class="desc">CRIMSON &middot; VISCOUS &middot; DARK</span>
  </a>
  <a class="card cC" href="/mgold">
    <span class="icon">&#x2B50;</span>
    <span class="name nC">GOLD RAIN</span>
    <span class="desc">AMBER &middot; GOLDEN &middot; WARM</span>
  </a>
  <a class="card c7" href="/mvoid">
    <span class="icon">&#x25A1;</span>
    <span class="name n7">VOID RAIN</span>
    <span class="desc">INVERTED &middot; DARK ON LIGHT</span>
  </a>
  <a class="card c3" href="/mphantom">
    <span class="icon">&#x25CA;</span>
    <span class="name n3">PHANTOM</span>
    <span class="desc">GHOSTLY &middot; PALE CYAN &middot; FADES</span>
  </a>
  <a class="card cB" href="/mripple">
    <span class="icon">&#x223F;</span>
    <span class="name nB">RIPPLE RAIN</span>
    <span class="desc">SINE WAVE &middot; AQUA &middot; FLOWS</span>
  </a>
  <a class="card c4" href="/mglitch">
    <span class="icon">&#x26A0;</span>
    <span class="name n4">GLITCH RAIN</span>
    <span class="desc">CORRUPT &middot; SPIKES &middot; NEON</span>
  </a>
</div>

<div class="cat">&#x25C6; FRACTALS &amp; MATHEMATICS</div>
<div class="grid">
  <a class="card c3" href="/fractal">
    <span class="icon">&#x2726;</span>
    <span class="name n3">JULIA SET</span>
    <span class="desc">ESCAPE TIME &middot; C3 RENDERED</span>
  </a>
  <a class="card cE" href="/hopalong">
    <span class="icon">&#x2299;</span>
    <span class="name nE">HOPALONG</span>
    <span class="desc">ATTRACTOR &middot; CHAOS ORBIT</span>
  </a>
  <a class="card c1" href="/interference">
    <span class="icon">&#x223F;</span>
    <span class="name n1">INTERFERENCE</span>
    <span class="desc">WAVE PATTERNS &middot; DUAL SOURCE</span>
  </a>
  <a class="card cC" href="/voronoi">
    <span class="icon">&#x2B21;</span>
    <span class="name nC">VORONOI</span>
    <span class="desc">CELLS &middot; MOVING SEEDS</span>
  </a>
  <a class="card c3" href="/strange">
    <span class="icon">&#x221E;</span>
    <span class="name n3">STRANGE</span>
    <span class="desc">CLIFFORD ATTRACTOR &middot; CHAOS</span>
  </a>
  <a class="card c2" href="/lissajous">
    <span class="icon">&#x224B;</span>
    <span class="name n2">LISSAJOUS</span>
    <span class="desc">HARMONIC FIGURES &middot; DRIFT</span>
  </a>
  <a class="card cC" href="/sierpinski">
    <span class="icon">&#x25B2;</span>
    <span class="name nC">SIERPINSKI</span>
    <span class="desc">CHAOS GAME &middot; TRIANGLE</span>
  </a>
  <a class="card c1" href="/spirograph">
    <span class="icon">&#x25CB;</span>
    <span class="name n1">SPIROGRAPH</span>
    <span class="desc">HYPOTROCHOID &middot; GEARS</span>
  </a>
  <a class="card cD" href="/barnsley">
    <span class="icon">&#x2AEB;</span>
    <span class="name nD">BARNSLEY FERN</span>
    <span class="desc">IFS FRACTAL &middot; NATURE MATH</span>
  </a>
</div>

<div class="cat">&#x25C6; ABSTRACT</div>
<div class="grid">
  <a class="card c1" href="/mandala">
    <span class="icon">&#x2B21;</span>
    <span class="name n1">MANDALA</span>
    <span class="desc">SACRED GEOMETRY &middot; RINGS</span>
  </a>
  <a class="card c2" href="/plasma">
    <span class="icon">&#x25C9;</span>
    <span class="name n2">PLASMA</span>
    <span class="desc">LAVA BLOBS &middot; CSS BLEND</span>
  </a>
  <a class="card c7" href="/starfield">
    <span class="icon">&#x2605;</span>
    <span class="name n7">STARFIELD</span>
    <span class="desc">WARP SPEED &middot; 3D PROJECTION</span>
  </a>
  <a class="card c8" href="/particles">
    <span class="icon">&#x22C6;</span>
    <span class="name n8">PARTICLES</span>
    <span class="desc">CONSTELLATION &middot; MESH</span>
  </a>
  <a class="card c9" href="/tunnel">
    <span class="icon">&#x25CE;</span>
    <span class="name n9">TUNNEL</span>
    <span class="desc">VORTEX &middot; ROTATING RINGS</span>
  </a>
  <a class="card cE" href="/kaleidoscope">
    <span class="icon">&#x1F52E;</span>
    <span class="name nE">KALEIDOSCOPE</span>
    <span class="desc">MIRROR &middot; 8-FOLD SYMMETRY</span>
  </a>
  <a class="card cF" href="/noise">
    <span class="icon">&#x25A6;</span>
    <span class="name nF">NOISE FIELD</span>
    <span class="desc">SINE NOISE &middot; PSYCHEDELIC</span>
  </a>
  <a class="card c9" href="/lava2">
    <span class="icon">&#x25CF;</span>
    <span class="name n9">LAVA LAMP</span>
    <span class="desc">METABALLS &middot; FLUID BLOBS</span>
  </a>
</div>

<div class="cat">&#x25C6; SIMULATIONS</div>
<div class="grid">
  <a class="card c2" href="/campfire">
    <span class="icon">&#x1F525;</span>
    <span class="name n2">CAMPFIRE</span>
    <span class="desc">FLAME TONGUES &middot; WARM GLOW</span>
  </a>
  <a class="card cF" href="/raindrops">
    <span class="icon">&#x1F4A7;</span>
    <span class="name nF">RAINDROPS</span>
    <span class="desc">WATER RIPPLES &middot; EXPANDING</span>
  </a>
  <a class="card c3" href="/gameoflife">
    <span class="icon">&#x25A3;</span>
    <span class="name n3">GAME OF LIFE</span>
    <span class="desc">CONWAY &middot; CELLULAR AUTOMATON</span>
  </a>
  <a class="card cD" href="/aurora">
    <span class="icon">&#x1F30C;</span>
    <span class="name nD">AURORA</span>
    <span class="desc">BOREALIS &middot; SINE WAVES &middot; GLOW</span>
  </a>
  <a class="card cC" href="/dragon">
    <span class="icon">&#x1F409;</span>
    <span class="name nC">DRAGON CURVE</span>
    <span class="desc">L-SYSTEM &middot; ANIMATED BUILD</span>
  </a>
</div>

<div class="cat">&#x25C6; GAMES</div>
<div class="grid">
  <a class="card c1" href="/snake">
    <span class="icon">&#x1F40D;</span>
    <span class="name n1">SNAKE</span>
    <span class="desc">CLASSIC &middot; NEON &middot; TOUCH + KEYS</span>
  </a>
</div>

<div class="cat">&#x25C6; SPACE &amp; COSMOS</div>
<div class="grid">
  <a class="card cD" href="/deepstars">
    <span class="icon">&#x2B50;</span>
    <span class="name nD">DEEP STARS</span>
    <span class="desc">3D PARALLAX &middot; NEBULA &middot; WARP</span>
  </a>
  <a class="card c9" href="/nebula">
    <span class="icon">&#x1F300;</span>
    <span class="name n9">NEBULA</span>
    <span class="desc">GAS CLOUD &middot; STAR FIELD</span>
  </a>
  <a class="card cA" href="/plasmaglobe">
    <span class="icon">&#x26A1;</span>
    <span class="name nA">PLASMA GLOBE</span>
    <span class="desc">ELECTRIC TENDRILS &middot; GLOW</span>
  </a>
  <a class="card cF" href="/warpgrid">
    <span class="icon">&#x2395;</span>
    <span class="name nF">WARP GRID</span>
    <span class="desc">3D MESH &middot; WAVE DISTORT</span>
  </a>
</div>

<div class="cat">&#x25C6; PARTICLE SYSTEMS</div>
<div class="grid">
  <a class="card c2" href="/fireworks">
    <span class="icon">&#x1F386;</span>
    <span class="name n2">FIREWORKS</span>
    <span class="desc">LAUNCH &middot; BURST &middot; GRAVITY</span>
  </a>
  <a class="card cE" href="/bounceballs">
    <span class="icon">&#x25CF;</span>
    <span class="name nE">BOUNCE BALLS</span>
    <span class="desc">NEON GLOW &middot; PHYSICS</span>
  </a>
  <a class="card c5" href="/flowfield">
    <span class="icon">&#x27BF;</span>
    <span class="name n5">FLOW FIELD</span>
    <span class="desc">PERLIN FLOW &middot; PARTICLES</span>
  </a>
  <a class="card c1" href="/neonrain">
    <span class="icon">&#x2605;</span>
    <span class="name n1">NEON RAIN</span>
    <span class="desc">SYMBOL SHOWER &middot; GREEN &middot; BLUE</span>
  </a>
</div>

<div class="cat">&#x25C6; LIFE &amp; PHYSICS</div>
<div class="grid">
  <a class="card c3" href="/coral">
    <span class="icon">&#x1F420;</span>
    <span class="name n3">CORAL REEF</span>
    <span class="desc">CELLULAR GROWTH &middot; OCEAN</span>
  </a>
  <a class="card cB" href="/sandfall">
    <span class="icon">&#x23F3;</span>
    <span class="name nB">SAND FALL</span>
    <span class="desc">CELLULAR AUTOMATON &middot; PHYSICS</span>
  </a>
  <a class="card c8" href="/lightning">
    <span class="icon">&#x26A1;</span>
    <span class="name n8">LIGHTNING</span>
    <span class="desc">FRACTAL BRANCH &middot; ELECTRIC</span>
  </a>
  <a class="card c6" href="/crystal">
    <span class="icon">&#x1F48E;</span>
    <span class="name n6">CRYSTAL</span>
    <span class="desc">HEXAGONAL &middot; PRISMATIC GROW</span>
  </a>
</div>

<div class="cat">&#x25C6; PSYCHEDELIC GEOMETRY</div>
<div class="grid">
  <a class="card c7" href="/acidspiral">
    <span class="icon">&#x1F300;</span>
    <span class="name n7">ACID SPIRAL</span>
    <span class="desc">MULTI-ARM &middot; HSL CYCLING</span>
  </a>
  <a class="card cC" href="/goop">
    <span class="icon">&#x1FAA0;</span>
    <span class="name nC">GOOP</span>
    <span class="desc">ORGANIC BLOBS &middot; TENTACLES</span>
  </a>
  <a class="card c4" href="/metaballs">
    <span class="icon">&#x25D5;</span>
    <span class="name n4">METABALLS</span>
    <span class="desc">ISO-SURFACE &middot; MORPHING</span>
  </a>
  <a class="card c0" href="/wormhole">
    <span class="icon">&#x1F30C;</span>
    <span class="name n0">WORMHOLE</span>
    <span class="desc">RING VORTEX &middot; CONVERGING</span>
  </a>
  <a class="card cD" href="/cwaves">
    <span class="icon">&#x223F;</span>
    <span class="name nD">C-WAVES</span>
    <span class="desc">C-SHAPED SINE &middot; INTERFERENCE</span>
  </a>
  <a class="card cF" href="/dna">
    <span class="icon">&#x1F9EC;</span>
    <span class="name nF">DNA HELIX</span>
    <span class="desc">DOUBLE HELIX &middot; 3D ROTATE</span>
  </a>
</div>

<footer>esp32-c3 super mini &middot; wifi ap &middot; 192.168.4.1 &middot; 53 modes</footer>
</body></html>
)EOF";


// ─────────────────────────────────────────────────────────────────────────────
//  MANDALA  —  Sacred Geometry (original + nav)
// ─────────────────────────────────────────────────────────────────────────────
static const char MANDALA_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>MANDALA · COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:radial-gradient(ellipse at 35% 45%,#0d003d 0%,#000010 60%,#001a20 100%);
min-height:100vh;display:flex;flex-direction:column;align-items:center;
justify-content:center;font-family:'Courier New',monospace;color:#fff;
overflow-x:hidden;padding:56px 16px 24px}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,10,.75);backdrop-filter:blur(8px);
border-bottom:1px solid rgba(131,56,236,.3);z-index:99;display:flex;align-items:center;justify-content:space-between}
.nav a{color:#8338ec;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#c77dff}
.nav span{color:rgba(255,255,255,.18);font-size:.5rem;letter-spacing:3px}
.glw{position:fixed;border-radius:50%;filter:blur(80px);z-index:-1;pointer-events:none}
.g1{width:320px;height:320px;top:-100px;left:-100px;background:rgba(123,47,255,.35)}
.g2{width:260px;height:260px;top:-80px;right:-80px;background:rgba(0,255,209,.2)}
.g3{width:300px;height:300px;bottom:-100px;left:-80px;background:rgba(58,134,255,.28)}
.g4{width:240px;height:240px;bottom:-80px;right:-80px;background:rgba(255,0,110,.22)}
.stars{position:fixed;inset:0;z-index:-1}
.s{position:absolute;background:#fff;border-radius:50%}
h1{font-size:clamp(1.5rem,5vw,2.2rem);letter-spacing:6px;text-align:center;
background:linear-gradient(90deg,#ff006e,#ff6b00,#ffd700,#06ffd0,#3a86ff,#8338ec,#ff006e);
-webkit-background-clip:text;-webkit-text-fill-color:transparent;background-clip:text;
filter:drop-shadow(0 0 8px rgba(131,56,236,.8));margin-bottom:4px}
.sub{font-size:.65rem;letter-spacing:9px;color:rgba(0,255,209,.65);text-align:center;margin-bottom:20px}
.mandala{position:relative;width:min(260px,72vw);height:min(260px,72vw);margin:0 auto 22px}
.r,.d{position:absolute;border-radius:50%;top:50%;left:50%;transform:translate(-50%,-50%)}
.r0{width:100%;height:100%;background:conic-gradient(#ff006e,#ff6b00,#ffd700,#06ffd0,#3a86ff,#8338ec,#ff006e)}
.d0{width:91%;height:91%;background:#000010}
.r1{width:84%;height:84%;background:conic-gradient(#3a86ff,#8338ec,#ff006e,#ffd700,#06ffd0,#ff6b00,#3a86ff);transform:translate(-50%,-50%) rotate(26deg)}
.d1{width:76%;height:76%;background:#000010}
.r2{width:70%;height:70%;background:conic-gradient(#ffd700,#06ffd0,#ff006e,#8338ec,#ff6b00,#3a86ff,#ffd700);transform:translate(-50%,-50%) rotate(52deg)}
.d2{width:62%;height:62%;background:#000010}
.r3{width:56%;height:56%;background:conic-gradient(#06ffd0,#ff6b00,#8338ec,#ff006e,#3a86ff,#ffd700,#06ffd0);transform:translate(-50%,-50%) rotate(78deg)}
.d3{width:48%;height:48%;background:#000010}
.r4{width:43%;height:43%;background:conic-gradient(#8338ec,#3a86ff,#06ffd0,#ff6b00,#ff006e,#ffd700,#8338ec);transform:translate(-50%,-50%) rotate(104deg)}
.d4{width:35%;height:35%;background:#000010}
.r5{width:30%;height:30%;background:conic-gradient(#ff6b00,#ffd700,#3a86ff,#06ffd0,#8338ec,#ff006e,#ff6b00);transform:translate(-50%,-50%) rotate(130deg)}
.sp{position:absolute;width:1px;height:100%;
background:linear-gradient(transparent 0%,rgba(255,255,255,.2) 30%,rgba(255,255,255,.45) 50%,rgba(255,255,255,.2) 70%,transparent 100%);
top:0;left:calc(50% - .5px);transform-origin:center}
.hl{position:absolute;border-radius:50%;top:50%;left:50%;transform:translate(-50%,-50%);pointer-events:none;border:1px solid}
.h1{width:110%;height:110%;border-color:rgba(131,56,236,.5);box-shadow:0 0 15px rgba(131,56,236,.35),inset 0 0 15px rgba(131,56,236,.12)}
.h2{width:118%;height:118%;border-color:rgba(0,255,209,.25);box-shadow:0 0 10px rgba(0,255,209,.2)}
.core{width:17%;height:17%;background:radial-gradient(circle,#fff 0%,#e040fb 45%,rgba(131,56,236,0) 100%);box-shadow:0 0 20px #c77dff,0 0 40px rgba(131,56,236,.5)}
.card{background:rgba(10,0,45,.55);border:1px solid rgba(131,56,236,.4);border-radius:12px;padding:16px 26px;text-align:center;width:min(290px,88vw);box-shadow:0 0 24px rgba(131,56,236,.15),inset 0 1px 0 rgba(255,255,255,.04)}
.lbl{font-size:.58rem;letter-spacing:3px;color:rgba(0,255,209,.55);margin-bottom:3px;text-transform:uppercase}
.val{font-size:.95rem;font-weight:bold;margin-bottom:13px}
.pink{color:#ff006e;text-shadow:0 0 12px rgba(255,0,110,.6)}
.cyan{color:#06ffd0;text-shadow:0 0 12px rgba(6,255,208,.6)}
.gold{color:#ffd700;text-shadow:0 0 12px rgba(255,215,0,.5)}
.dot{display:inline-block;width:7px;height:7px;border-radius:50%;background:#06ffd0;box-shadow:0 0 8px #06ffd0;margin-right:5px;vertical-align:middle}
footer{margin-top:18px;font-size:.5rem;letter-spacing:4px;color:rgba(255,255,255,.12);text-align:center;text-transform:uppercase}
</style></head><body>
<div class="nav"><a href="/">⬡ MODES</a><span>COSMIC-C3</span></div>
<div class="glw g1"></div><div class="glw g2"></div><div class="glw g3"></div><div class="glw g4"></div>
<div class="stars">
<div class="s" style="width:2px;height:2px;top:7%;left:13%"></div>
<div class="s" style="width:1px;height:1px;top:11%;left:82%"></div>
<div class="s" style="width:2px;height:2px;top:18%;left:45%"></div>
<div class="s" style="width:1px;height:1px;top:29%;left:7%"></div>
<div class="s" style="width:3px;height:3px;top:36%;left:90%"></div>
<div class="s" style="width:1px;height:1px;top:43%;left:24%"></div>
<div class="s" style="width:2px;height:2px;top:51%;left:71%"></div>
<div class="s" style="width:1px;height:1px;top:60%;left:37%"></div>
<div class="s" style="width:2px;height:2px;top:67%;left:86%"></div>
<div class="s" style="width:1px;height:1px;top:73%;left:17%"></div>
<div class="s" style="width:2px;height:2px;top:81%;left:54%"></div>
<div class="s" style="width:1px;height:1px;top:88%;left:76%"></div>
<div class="s" style="width:3px;height:3px;top:4%;left:61%"></div>
<div class="s" style="width:1px;height:1px;top:25%;left:70%"></div>
<div class="s" style="width:2px;height:2px;top:55%;left:4%"></div>
<div class="s" style="width:1px;height:1px;top:92%;left:40%"></div>
<div class="s" style="width:2px;height:2px;top:16%;left:32%"></div>
<div class="s" style="width:1px;height:1px;top:46%;left:60%"></div>
<div class="s" style="width:2px;height:2px;top:77%;left:9%"></div>
<div class="s" style="width:1px;height:1px;top:34%;left:52%"></div>
</div>
<h1>COSMIC PORTAL</h1>
<p class="sub">ESP32 &middot; C3 &middot; SUPER MINI</p>
<div class="mandala">
  <div class="r r0"></div><div class="d d0"></div>
  <div class="r r1"></div><div class="d d1"></div>
  <div class="r r2"></div><div class="d d2"></div>
  <div class="r r3"></div><div class="d d3"></div>
  <div class="r r4"></div><div class="d d4"></div>
  <div class="r r5"></div>
  <div class="sp" style="transform:rotate(0deg)"></div>
  <div class="sp" style="transform:rotate(30deg)"></div>
  <div class="sp" style="transform:rotate(60deg)"></div>
  <div class="sp" style="transform:rotate(90deg)"></div>
  <div class="sp" style="transform:rotate(120deg)"></div>
  <div class="sp" style="transform:rotate(150deg)"></div>
  <div class="hl h1"></div><div class="hl h2"></div>
  <div class="r core"></div>
</div>
<div class="card">
  <div class="lbl">Network</div><div class="val pink">COSMIC-C3</div>
  <div class="lbl">Address</div><div class="val cyan">192.168.4.1</div>
  <div class="lbl">Status</div><div class="val gold"><span class="dot"></span>TRANSMITTING</div>
</div>
<footer>wifi &middot; ap mode &middot; esp32-c3 &middot; 2.4&nbsp;ghz</footer>
</body></html>
)EOF";

// ─────────────────────────────────────────────────────────────────────────────
//  PLASMA  —  CSS Animated Blob Plasma
// ─────────────────────────────────────────────────────────────────────────────
static const char PLASMA_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>PLASMA · COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#030008}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(3,0,8,.8);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(131,56,236,.25);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#8338ec;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#c77dff}
.nav span{color:rgba(255,255,255,.18);font-size:.5rem;letter-spacing:3px}
.blob{position:fixed;border-radius:50%;mix-blend-mode:screen}
.b1{width:90vmax;height:90vmax;
background:radial-gradient(circle,rgba(140,0,255,.9) 0%,rgba(90,0,210,.45) 30%,transparent 70%);
animation:f1 9s ease-in-out infinite}
.b2{width:80vmax;height:80vmax;
background:radial-gradient(circle,rgba(255,50,0,.85) 0%,rgba(200,20,0,.4) 30%,transparent 70%);
animation:f2 11s ease-in-out infinite}
.b3{width:85vmax;height:85vmax;
background:radial-gradient(circle,rgba(0,180,255,.8) 0%,rgba(0,90,200,.35) 30%,transparent 70%);
animation:f3 13s ease-in-out infinite}
.b4{width:75vmax;height:75vmax;
background:radial-gradient(circle,rgba(0,255,120,.75) 0%,rgba(0,170,70,.3) 30%,transparent 70%);
animation:f4 7s ease-in-out infinite}
.b5{width:70vmax;height:70vmax;
background:radial-gradient(circle,rgba(255,210,0,.8) 0%,rgba(210,100,0,.35) 30%,transparent 70%);
animation:f5 15s ease-in-out infinite}
.b6{width:65vmax;height:65vmax;
background:radial-gradient(circle,rgba(255,0,140,.8) 0%,rgba(180,0,80,.35) 30%,transparent 70%);
animation:f6 8s ease-in-out infinite}
@keyframes f1{0%,100%{transform:translate(-15%,-25%)}25%{transform:translate(45%,25%)}50%{transform:translate(55%,-35%)}75%{transform:translate(5%,55%)}}
@keyframes f2{0%,100%{transform:translate(55%,55%)}25%{transform:translate(-25%,15%)}50%{transform:translate(15%,-45%)}75%{transform:translate(65%,-15%)}}
@keyframes f3{0%,100%{transform:translate(15%,-45%)}33%{transform:translate(-35%,45%)}66%{transform:translate(65%,25%)}}
@keyframes f4{0%,100%{transform:translate(45%,15%)}25%{transform:translate(-15%,55%)}50%{transform:translate(25%,-35%)}75%{transform:translate(-25%,-15%)}}
@keyframes f5{0%,100%{transform:translate(-25%,35%)}33%{transform:translate(55%,-25%)}66%{transform:translate(5%,65%)}}
@keyframes f6{0%,100%{transform:translate(35%,-35%)}20%{transform:translate(-35%,5%)}60%{transform:translate(45%,45%)}80%{transform:translate(5%,-45%)}}
.lbl{position:fixed;bottom:18px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.52rem;letter-spacing:6px;
color:rgba(255,255,255,.25);text-transform:uppercase;z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">⬡ MODES</a><span>PLASMA</span></div>
<div class="blob b1"></div>
<div class="blob b2"></div>
<div class="blob b3"></div>
<div class="blob b4"></div>
<div class="blob b5"></div>
<div class="blob b6"></div>
<div class="lbl">PLASMA &middot; CSS ANIMATED &middot; COSMIC-C3</div>
</body></html>
)EOF";

// ─────────────────────────────────────────────────────────────────────────────
//  FRACTAL  —  Julia Set (canvas + JS, rendered in browser)
// ─────────────────────────────────────────────────────────────────────────────
static const char FRACTAL_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>FRACTAL · COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000}
canvas{display:block;width:100%;height:100%}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,0,.7);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(6,255,208,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#06ffd0;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#80ffe8}
.nav span{color:rgba(255,255,255,.18);font-size:.5rem;letter-spacing:3px}
#overlay{position:fixed;inset:0;display:flex;flex-direction:column;
align-items:center;justify-content:center;z-index:50;
font-family:'Courier New',monospace;pointer-events:none}
.ov-title{font-size:clamp(.8rem,3vw,1.1rem);letter-spacing:6px;
color:#06ffd0;text-shadow:0 0 15px rgba(6,255,208,.7);margin-bottom:12px}
.ov-sub{font-size:.55rem;letter-spacing:4px;color:rgba(6,255,208,.45)}
.lbl{position:fixed;bottom:18px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.52rem;letter-spacing:5px;
color:rgba(255,255,255,.22);text-transform:uppercase;z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">⬡ MODES</a><span>FRACTAL</span></div>
<canvas id="c"></canvas>
<div id="overlay">
  <div class="ov-title">COMPUTING</div>
  <div class="ov-sub">JULIA SET &middot; C = &minus;0.7269 + 0.1889i</div>
</div>
<div class="lbl">JULIA SET &middot; ESCAPE TIME &middot; COSMIC-C3</div>
<script>
(function(){
  var canvas = document.getElementById('c');
  var W = canvas.width  = window.innerWidth;
  var H = canvas.height = window.innerHeight;
  var ctx = canvas.getContext('2d');
  var img = ctx.createImageData(W, H);
  var d = img.data;
  var cr = -0.7269, ci = 0.1889;
  var MAX = 120;
  var scale = Math.min(W, H) / 3.0;

  function hsl2rgb(h, s, l) {
    s /= 100; l /= 100;
    var a = s * Math.min(l, 1 - l);
    function f(n) {
      var k = (n + h / 30) % 12;
      return l - a * Math.max(-1, Math.min(k - 3, Math.min(9 - k, 1)));
    }
    return [Math.round(f(0)*255), Math.round(f(8)*255), Math.round(f(4)*255)];
  }

  // Render in chunks to avoid blocking the browser
  var py = 0;
  function renderChunk() {
    var end = Math.min(py + 20, H);
    for (; py < end; py++) {
      for (var px = 0; px < W; px++) {
        var zx = (px - W * 0.5) / scale;
        var zy = (py - H * 0.5) / scale;
        var i = 0;
        while (zx*zx + zy*zy < 4 && i < MAX) {
          var tx = zx*zx - zy*zy + cr;
          zy = 2*zx*zy + ci;
          zx = tx;
          i++;
        }
        var idx = (py * W + px) * 4;
        if (i === MAX) {
          d[idx] = d[idx+1] = d[idx+2] = 0;
        } else {
          var t = i / MAX;
          var rgb = hsl2rgb((200 + t * 300) % 360, 100, 20 + t * 55);
          d[idx] = rgb[0]; d[idx+1] = rgb[1]; d[idx+2] = rgb[2];
        }
        d[idx+3] = 255;
      }
    }
    ctx.putImageData(img, 0, 0);
    if (py < H) {
      requestAnimationFrame(renderChunk);
    } else {
      document.getElementById('overlay').style.display = 'none';
    }
  }
  requestAnimationFrame(renderChunk);
})();
</script>
</body></html>
)EOF";

// ─────────────────────────────────────────────────────────────────────────────
//  MATRIX  —  Classic Green Digital Rain
// ─────────────────────────────────────────────────────────────────────────────
static const char MATRIX_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>MATRIX &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,5,0,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(0,255,65,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#00ff41;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#afffaf}
.nav span{color:rgba(0,255,65,.3);font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;font-family:'Courier New',monospace;
font-size:.5rem;letter-spacing:6px;color:rgba(0,255,65,.2);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>MATRIX</span></div>
<canvas id="c"></canvas>
<div class="lbl">DIGITAL RAIN &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var fs=14,cols=W/fs|0,drops=[];
for(var i=0;i<cols;i++)drops[i]=Math.random()*H/fs|0;
var ch='0123456789ABCDEFGHIJKLabcdefghijk@#$%&*<>{}|'
  +'&#xFF66;&#xFF67;&#xFF68;&#xFF69;&#xFF6A;&#xFF6B;&#xFF6C;&#xFF6D;&#xFF6E;&#xFF6F;&#xFF71;&#xFF72;&#xFF73;&#xFF74;&#xFF75;'
  +'&#xFF76;&#xFF77;&#xFF78;&#xFF79;&#xFF7A;&#xFF7B;&#xFF7C;&#xFF7D;&#xFF7E;&#xFF7F;&#xFF80;&#xFF81;&#xFF82;&#xFF83;&#xFF84;'
  +'&#xFF85;&#xFF86;&#xFF87;&#xFF88;&#xFF89;&#xFF8A;&#xFF8B;&#xFF8C;&#xFF8D;&#xFF8E;&#xFF8F;&#xFF90;&#xFF91;&#xFF92;&#xFF93;'
  +'&#xFF94;&#xFF95;&#xFF96;&#xFF97;&#xFF98;&#xFF99;&#xFF9A;&#xFF9B;&#xFF9C;&#xFF9D;';
function draw(){
  ctx.fillStyle='rgba(0,0,0,0.05)';
  ctx.fillRect(0,0,W,H);
  ctx.font=fs+'px monospace';
  for(var i=0;i<cols;i++){
    ctx.fillStyle='#afffaf';
    ctx.fillText(ch[Math.random()*ch.length|0],i*fs,drops[i]*fs);
    if(drops[i]*fs>H&&Math.random()>.975)drops[i]=0;
    drops[i]++;
  }
}
setInterval(draw,50);
})();
</script></body></html>
)EOF";

// ─────────────────────────────────────────────────────────────────────────────
//  CYBER  —  Neon Cyberpunk Matrix Rain
// ─────────────────────────────────────────────────────────────────────────────
static const char CYBER_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>CYBER RAIN &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(8,0,8,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(255,0,170,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#ff00aa;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#ff80d5}
.nav span{color:rgba(255,0,170,.3);font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;font-family:'Courier New',monospace;
font-size:.5rem;letter-spacing:6px;color:rgba(255,0,170,.2);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>CYBER RAIN</span></div>
<canvas id="c"></canvas>
<div class="lbl">NEON MATRIX &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var fs=14,cols=W/fs|0,drops=[],clr=[];
var pal=['#ff00aa','#aa00ff','#00ffbb','#ff6600','#00ccff','#ffff00','#ff0055','#00ff88'];
for(var i=0;i<cols;i++){drops[i]=Math.random()*H/fs|0;clr[i]=pal[Math.random()*pal.length|0];}
var ch='0123456789ABCDEFGHIJKLabcdefghijk@#$%&*<>{}|'
  +'&#xFF66;&#xFF67;&#xFF68;&#xFF69;&#xFF6A;&#xFF6B;&#xFF6C;&#xFF6D;&#xFF6E;&#xFF6F;&#xFF71;&#xFF72;&#xFF73;&#xFF74;&#xFF75;'
  +'&#xFF76;&#xFF77;&#xFF78;&#xFF79;&#xFF7A;&#xFF7B;&#xFF7C;&#xFF7D;&#xFF7E;&#xFF7F;&#xFF80;&#xFF81;&#xFF82;&#xFF83;&#xFF84;'
  +'&#xFF85;&#xFF86;&#xFF87;&#xFF88;&#xFF89;&#xFF8A;&#xFF8B;&#xFF8C;&#xFF8D;&#xFF8E;&#xFF8F;&#xFF90;&#xFF91;&#xFF92;&#xFF93;';
function draw(){
  ctx.fillStyle='rgba(0,0,0,0.05)';
  ctx.fillRect(0,0,W,H);
  ctx.font=fs+'px monospace';
  for(var i=0;i<cols;i++){
    ctx.shadowBlur=9;ctx.shadowColor=clr[i];
    ctx.fillStyle=clr[i];
    ctx.fillText(ch[Math.random()*ch.length|0],i*fs,drops[i]*fs);
    ctx.shadowBlur=0;
    if(drops[i]*fs>H&&Math.random()>.975)drops[i]=0;
    drops[i]++;
  }
}
setInterval(draw,50);
})();
</script></body></html>
)EOF";

// ─────────────────────────────────────────────────────────────────────────────
//  BINARY  —  Binary Rain (0/1, electric blue)
// ─────────────────────────────────────────────────────────────────────────────
static const char BINARY_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>BINARY &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000008}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,15,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(0,136,255,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#44aaff;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#aaddff}
.nav span{color:rgba(0,136,255,.3);font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;font-family:'Courier New',monospace;
font-size:.5rem;letter-spacing:6px;color:rgba(0,136,255,.2);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>BINARY</span></div>
<canvas id="c"></canvas>
<div class="lbl">MACHINE CODE &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var fs=16,cols=W/fs|0,drops=[];
for(var i=0;i<cols;i++)drops[i]=Math.random()*H/fs|0;
function draw(){
  ctx.fillStyle='rgba(0,0,12,0.05)';
  ctx.fillRect(0,0,W,H);
  ctx.font='bold '+fs+'px monospace';
  for(var i=0;i<cols;i++){
    ctx.fillStyle='#55bbff';
    ctx.fillText(Math.random()>.5?'1':'0',i*fs,drops[i]*fs);
    if(drops[i]*fs>H&&Math.random()>.975)drops[i]=0;
    drops[i]++;
  }
}
setInterval(draw,60);
})();
</script></body></html>
)EOF";

// ─────────────────────────────────────────────────────────────────────────────
//  STARFIELD  —  3D Warp Speed Hyperspace
// ─────────────────────────────────────────────────────────────────────────────
static const char STARFIELD_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>STARFIELD &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000005}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,10,.85);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(180,180,255,.15);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#b4b4ff;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#e0e0ff}
.nav span{color:rgba(180,180,255,.25);font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;font-family:'Courier New',monospace;
font-size:.5rem;letter-spacing:6px;color:rgba(180,180,255,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>STARFIELD</span></div>
<canvas id="c"></canvas>
<div class="lbl">HYPERSPACE JUMP &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var N=200,spd=5,stars=[];
function mk(){return{x:(Math.random()-.5)*W*2,y:(Math.random()-.5)*H*2,z:Math.random()*W};}
for(var i=0;i<N;i++)stars[i]=mk();
var cx=W/2,cy=H/2;
function draw(){
  ctx.fillStyle='rgba(0,0,5,0.22)';
  ctx.fillRect(0,0,W,H);
  for(var i=0;i<N;i++){
    var s=stars[i],pz=s.z;
    s.z-=spd;
    if(s.z<=1){stars[i]=mk();stars[i].z=W;continue;}
    var sx=s.x/s.z*W+cx,sy=s.y/s.z*H+cy;
    var px=s.x/pz*W+cx,py=s.y/pz*H+cy;
    var b=1-s.z/W;
    var v=180+b*75|0;
    ctx.strokeStyle='rgba('+v+','+v+',255,'+b+')';
    ctx.lineWidth=Math.max(0.5,b*2.5);
    ctx.beginPath();ctx.moveTo(px,py);ctx.lineTo(sx,sy);ctx.stroke();
  }
}
setInterval(draw,30);
})();
</script></body></html>
)EOF";

// ─────────────────────────────────────────────────────────────────────────────
//  PARTICLES  —  Constellation Mesh
// ─────────────────────────────────────────────────────────────────────────────
static const char PARTICLES_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>PARTICLES &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#03001a}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(3,0,26,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(255,215,0,.15);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#ffd700;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#ffe96b}
.nav span{color:rgba(255,215,0,.25);font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;font-family:'Courier New',monospace;
font-size:.5rem;letter-spacing:6px;color:rgba(255,215,0,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>PARTICLES</span></div>
<canvas id="c"></canvas>
<div class="lbl">CONSTELLATION MESH &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var N=70,DIST2=130*130,pts=[];
for(var i=0;i<N;i++)pts.push({x:Math.random()*W,y:Math.random()*H,
  vx:(Math.random()-.5)*.65,vy:(Math.random()-.5)*.65,r:Math.random()*2+1});
function draw(){
  ctx.fillStyle='rgba(3,0,26,0.18)';
  ctx.fillRect(0,0,W,H);
  for(var i=0;i<N;i++){
    var p=pts[i];
    p.x+=p.vx;p.y+=p.vy;
    if(p.x<0||p.x>W)p.vx*=-1;
    if(p.y<0||p.y>H)p.vy*=-1;
    for(var j=i+1;j<N;j++){
      var dx=pts[j].x-p.x,dy=pts[j].y-p.y,d2=dx*dx+dy*dy;
      if(d2<DIST2){
        var a=(1-Math.sqrt(d2/DIST2))*.7;
        ctx.strokeStyle='rgba(255,210,60,'+a+')';
        ctx.lineWidth=.7;
        ctx.beginPath();ctx.moveTo(p.x,p.y);ctx.lineTo(pts[j].x,pts[j].y);ctx.stroke();
      }
    }
    ctx.fillStyle='rgba(255,230,100,.9)';
    ctx.beginPath();ctx.arc(p.x,p.y,p.r,0,Math.PI*2);ctx.fill();
  }
}
setInterval(draw,33);
})();
</script></body></html>
)EOF";

// ─────────────────────────────────────────────────────────────────────────────
//  TUNNEL  —  Psychedelic Vortex
// ─────────────────────────────────────────────────────────────────────────────
static const char TUNNEL_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>TUNNEL &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,0,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(255,80,0,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#ff6622;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#ffaa77}
.nav span{color:rgba(255,80,0,.3);font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;font-family:'Courier New',monospace;
font-size:.5rem;letter-spacing:6px;color:rgba(255,80,0,.2);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>TUNNEL</span></div>
<canvas id="c"></canvas>
<div class="lbl">INFINITE VORTEX &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var t=0,cx=W/2,cy=H/2,R=20;
function draw(){
  ctx.fillStyle='rgba(0,0,0,0.12)';
  ctx.fillRect(0,0,W,H);
  for(var i=R;i>=0;i--){
    var f=i/R;
    var r=Math.pow(f,1.3)*Math.min(W,H)*.55;
    var hue=(t*80+i*(360/R))%360;
    ctx.strokeStyle='hsla('+hue+',100%,58%,'+(0.4+f*.5)+')';
    ctx.lineWidth=1.5;
    ctx.save();ctx.translate(cx,cy);ctx.rotate(t*.9+i*.13);
    ctx.beginPath();ctx.rect(-r,-r,r*2,r*2);ctx.stroke();
    ctx.restore();
  }
  t+=0.026;
}
setInterval(draw,30);
})();
</script></body></html>
)EOF";

// ─────────────────────────────────────────────────────────────────────────────
//  WAVE 1 — 25 New Modes
// ─────────────────────────────────────────────────────────────────────────────
static const char MFIRE_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>MATRIX FIRE &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#050000}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(8,0,0,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(255,80,0,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#ff6600;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#ffaa44}
.nav span{color:#446600;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(255,80,0,.2);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>MATRIX FIRE</span></div>
<canvas id="c"></canvas>
<div class="lbl">FIRE RAIN &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var fs=14,cols=W/fs|0,drops=[];
for(var i=0;i<cols;i++)drops[i]=Math.random()*H/fs|0;
var ch='0123456789ABCDEF@#$%&*<>{}|ｦｧｨｩｪｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝ';

function draw(){
  ctx.fillStyle='rgba(8,0,0,0.05)';
  ctx.fillRect(0,0,W,H);
  ctx.font=fs+'px monospace';
  for(var i=0;i<cols;i++){
    ctx.fillStyle='#ffee30';ctx.fillText(ch[Math.random()*ch.length|0],i*fs,drops[i]*fs);
    if(drops[i]*fs>H&&Math.random()>.975)drops[i]=0;
    drops[i]++;
  }
}
setInterval(draw,50);
})();
</script></body></html>

)EOF";

static const char MICE_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>MATRIX ICE &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000308}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,12,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(0,180,255,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#44ccff;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#aaeeff}
.nav span{color:#44cc44;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(0,180,255,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>MATRIX ICE</span></div>
<canvas id="c"></canvas>
<div class="lbl">ICE RAIN &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var fs=14,cols=W/fs|0,drops=[];
for(var i=0;i<cols;i++)drops[i]=Math.random()*H/fs|0;
var ch='0123456789ABCDEF@#$%&*<>{}|ｦｧｨｩｪｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝ';

function draw(){
  ctx.fillStyle='rgba(0,0,10,0.05)';
  ctx.fillRect(0,0,W,H);
  ctx.font=fs+'px monospace';
  for(var i=0;i<cols;i++){
    ctx.fillStyle='#c0ffff';ctx.fillText(ch[Math.random()*ch.length|0],i*fs,drops[i]*fs);
    if(drops[i]*fs>H&&Math.random()>.975)drops[i]=0;
    drops[i]++;
  }
}
setInterval(draw,50);
})();
</script></body></html>

)EOF";

static const char MSTORM_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>MATRIX STORM &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000408}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,12,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(120,100,255,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#9988ff;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#ccbbff}
.nav span{color:#998844;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(120,100,255,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>MATRIX STORM</span></div>
<canvas id="c"></canvas>
<div class="lbl">LIGHTNING RAIN &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var fs=14,cols=W/fs|0,drops=[];
for(var i=0;i<cols;i++)drops[i]=Math.random()*H/fs|0;
var ch='0123456789ABCDEF@#$%&*<>{}|ｦｧｨｩｪｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝ';

function draw(){
  ctx.fillStyle='rgba(0,0,6,0.05)';
  ctx.fillRect(0,0,W,H);
  ctx.font=fs+'px monospace';
  for(var i=0;i<cols;i++){
    ctx.fillStyle=Math.random()<0.04?'#ffffff':'#a0b0ff';ctx.fillText(ch[Math.random()*ch.length|0],i*fs,drops[i]*fs);
    if(drops[i]*fs>H&&Math.random()>.975)drops[i]=0;
    drops[i]++;
  }
}
setInterval(draw,50);
})();
</script></body></html>

)EOF";

static const char MBLOOD_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>MATRIX BLOOD &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#080000}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(10,0,0,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(200,0,0,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#cc2020;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#ff6060}
.nav span{color:#cc2020;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(200,0,0,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>MATRIX BLOOD</span></div>
<canvas id="c"></canvas>
<div class="lbl">BLOOD RAIN &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var fs=14,cols=W/fs|0,drops=[];
for(var i=0;i<cols;i++)drops[i]=Math.random()*H/fs|0;
var ch='0123456789ABCDEF@#$%&*<>{}|ｦｧｨｩｪｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝ';

function draw(){
  ctx.fillStyle='rgba(8,0,0,0.05)';
  ctx.fillRect(0,0,W,H);
  ctx.font=fs+'px monospace';
  for(var i=0;i<cols;i++){
    ctx.fillStyle='#ff2020';ctx.fillText(ch[Math.random()*ch.length|0],i*fs,drops[i]*fs);
    if(drops[i]*fs>H&&Math.random()>.975)drops[i]=0;
    drops[i]++;
  }
}
setInterval(draw,50);
})();
</script></body></html>

)EOF";

static const char MGOLD_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>MATRIX GOLD &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#050300}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(10,6,0,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(220,170,0,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#ddaa00;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#ffdd44}
.nav span{color:#ddaa00;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(220,170,0,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>MATRIX GOLD</span></div>
<canvas id="c"></canvas>
<div class="lbl">GOLDEN RAIN &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var fs=14,cols=W/fs|0,drops=[];
for(var i=0;i<cols;i++)drops[i]=Math.random()*H/fs|0;
var ch='0123456789ABCDEF@#$%&*<>{}|ｦｧｨｩｪｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝ';

function draw(){
  ctx.fillStyle='rgba(8,4,0,0.05)';
  ctx.fillRect(0,0,W,H);
  ctx.font=fs+'px monospace';
  for(var i=0;i<cols;i++){
    ctx.fillStyle='#ffd700';ctx.fillText(ch[Math.random()*ch.length|0],i*fs,drops[i]*fs);
    if(drops[i]*fs>H&&Math.random()>.975)drops[i]=0;
    drops[i]++;
  }
}
setInterval(draw,50);
})();
</script></body></html>

)EOF";

static const char MVOID_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>MATRIX VOID &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#e8e8ff}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(220,220,255,.92);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(50,0,150,.15);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#5500cc;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#8844ff}
.nav span{color:#5500cc;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(100,80,200,.3);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>MATRIX VOID</span></div>
<canvas id="c"></canvas>
<div class="lbl">VOID RAIN &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var fs=14,cols=W/fs|0,drops=[];
for(var i=0;i<cols;i++)drops[i]=Math.random()*H/fs|0;
var ch='0123456789ABCDEF@#$%&*<>{}|ｦｧｨｩｪｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝ';
ctx.fillStyle='rgb(232,232,255)';ctx.fillRect(0,0,W,H);
function draw(){
  ctx.fillStyle='rgba(232,232,255,0.12)';
  ctx.fillRect(0,0,W,H);
  ctx.font=fs+'px monospace';
  for(var i=0;i<cols;i++){
    ctx.fillStyle='#0a0028';
    ctx.fillText(ch[Math.random()*ch.length|0],i*fs,drops[i]*fs);
    if(drops[i]*fs>H&&Math.random()>.975)drops[i]=0;
    drops[i]++;
  }
}
setInterval(draw,50);
})();
</script></body></html>

)EOF";

static const char MPHANTOM_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>MATRIX PHANTOM &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,0,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(180,255,220,.15);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#80ffcc;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#bbffee}
.nav span{color:#8044cc;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(180,255,220,.15);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>MATRIX PHANTOM</span></div>
<canvas id="c"></canvas>
<div class="lbl">GHOST RAIN &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var fs=14,cols=W/fs|0,drops=[];
for(var i=0;i<cols;i++)drops[i]=Math.random()*H/fs|0;
var ch='0123456789ABCDEF@#$%&*<>{}|ｦｧｨｩｪｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝ';

function draw(){
  ctx.fillStyle='rgba(0,0,0,0.04)';
  ctx.fillRect(0,0,W,H);
  ctx.font=fs+'px monospace';
  for(var i=0;i<cols;i++){
    if(Math.random()>0.28){
    ctx.globalAlpha=0.35+Math.random()*0.65;
    ctx.fillStyle='#c0ffe8';
    ctx.fillText(ch[Math.random()*ch.length|0],i*fs,drops[i]*fs);
    ctx.globalAlpha=1;
  }
    if(drops[i]*fs>H&&Math.random()>.975)drops[i]=0;
    drops[i]++;
  }
}
setInterval(draw,50);
})();
</script></body></html>

)EOF";

static const char MRIPPLE_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>MATRIX RIPPLE &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000508}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,5,10,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(0,220,255,.18);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#00ddff;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#88eeff}
.nav span{color:#00dd44;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(0,220,255,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>MATRIX RIPPLE</span></div>
<canvas id="c"></canvas>
<div class="lbl">RIPPLE RAIN &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var fs=14,cols=W/fs|0,drops=[];
for(var i=0;i<cols;i++)drops[i]=Math.random()*H/fs|0;
var ch='0123456789ABCDEF@#$%&*<>{}|ｦｧｨｩｪｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝ';
var t=0;
function draw(){
  ctx.fillStyle='rgba(0,3,6,0.05)';
  ctx.fillRect(0,0,W,H);
  ctx.font=fs+'px monospace';
  for(var i=0;i<cols;i++){
    var xoff=Math.sin(t*2.2+i*0.28)*6;
    ctx.fillStyle='#50e8ff';
    ctx.fillText(ch[Math.random()*ch.length|0],i*fs+xoff,drops[i]*fs);
    if(drops[i]*fs>H&&Math.random()>.975)drops[i]=0;
    drops[i]++;
  }
  t+=0.016;
}
setInterval(draw,50);
})();
</script></body></html>

)EOF";

static const char MGLITCH_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>MATRIX GLITCH &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,0,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(0,255,65,.18);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#00ff41;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#afffaf}
.nav span{color:#004441;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(0,255,65,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>MATRIX GLITCH</span></div>
<canvas id="c"></canvas>
<div class="lbl">GLITCH RAIN &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var fs=14,cols=W/fs|0,drops=[];
for(var i=0;i<cols;i++)drops[i]=Math.random()*H/fs|0;
var ch='0123456789ABCDEF@#$%&*<>{}|ｦｧｨｩｪｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝ';
var pal=['#ff0066','#00ffaa','#ff9900','#aa00ff','#00ccff','#ff00ff'];
function draw(){
  ctx.fillStyle='rgba(0,0,0,0.05)';
  ctx.fillRect(0,0,W,H);
  ctx.font=fs+'px monospace';
  for(var i=0;i<cols;i++){
    var glitch=Math.random()<0.04;
    var xoff=glitch?(Math.random()-.5)*24:0;
    ctx.fillStyle=glitch?pal[Math.random()*pal.length|0]:'#00ff41';
    ctx.fillText(ch[Math.random()*ch.length|0],i*fs+xoff,drops[i]*fs);
    if(drops[i]*fs>H&&Math.random()>.975)drops[i]=0;
    drops[i]++;
  }
}
setInterval(draw,50);
})();
</script></body></html>

)EOF";

static const char HOPALONG_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>HOPALONG &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,0,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(255,100,200,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#ff66cc;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#ffaaee}
.nav span{color:#4466cc;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(255,100,200,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>HOPALONG</span></div>
<canvas id="c"></canvas>
<div class="lbl">HOPALONG ATTRACTOR &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var hx=0,hy=0,ha=7.7,hb=0.4,hc=1,ht=0;
ctx.fillStyle='#000';ctx.fillRect(0,0,W,H);
var cx=W/2,cy=H/2,sc=Math.min(W,H)/4.5;
function draw(){
  ha=7.7+Math.sin(ht)*2.5; hb=0.4+Math.cos(ht*1.3)*.25;
  ctx.fillStyle='rgba(0,0,0,0.012)';ctx.fillRect(0,0,W,H);
  for(var i=0;i<900;i++){
    var nx=hy-(hx<0?-1:1)*Math.sqrt(Math.abs(hb*hx-hc));
    var ny=ha-hx; hx=nx; hy=ny;
    var px=cx+hx*sc,py=cy+hy*sc;
    if(px>=0&&px<W&&py>=0&&py<H){
      var hue=(Math.atan2(hy,hx)/Math.PI*180+180)%360;
      ctx.fillStyle='hsl('+hue+',100%,65%)';
      ctx.fillRect(px|0,py|0,1,1);
    }
  }
  ht+=0.003;
}
setInterval(draw,30);
})();
</script></body></html>

)EOF";

static const char INTERFERENCE_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>INTERFERENCE &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,0,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(180,100,255,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#cc88ff;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#eeccff}
.nav span{color:#cc8844;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(180,100,255,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>INTERFERENCE</span></div>
<canvas id="c"></canvas>
<div class="lbl">WAVE INTERFERENCE &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var S=3,IW=W/S|0,IH=H/S|0;
var ofc=document.createElement('canvas');ofc.width=IW;ofc.height=IH;
var octx=ofc.getContext('2d');
var imgd=octx.createImageData(IW,IH);var d=imgd.data;
var t=0;
function draw(){
  var s1x=IW*.35|0,s1y=IH*.5|0,s2x=IW*.65|0,s2y=IH*.5|0;
  for(var py=0;py<IH;py++){
    for(var px=0;px<IW;px++){
      var d1=Math.sqrt((px-s1x)*(px-s1x)+(py-s1y)*(py-s1y));
      var d2=Math.sqrt((px-s2x)*(px-s2x)+(py-s2y)*(py-s2y));
      var w=(Math.sin(d1*.22-t*3)+Math.sin(d2*.22-t*3))*.5;
      var v=(w*.5+.5)*255|0;
      var idx=(py*IW+px)*4;
      d[idx]=v;d[idx+1]=(v*.65)|0;d[idx+2]=(v*.95)|0;d[idx+3]=255;
    }
  }
  octx.putImageData(imgd,0,0);
  ctx.drawImage(ofc,0,0,W,H);
  t+=0.05;
}
setInterval(draw,40);
})();
</script></body></html>

)EOF";

static const char VORONOI_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>VORONOI &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,0,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(255,200,50,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#ffcc33;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#ffee88}
.nav span{color:#44cc33;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(255,200,50,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>VORONOI</span></div>
<canvas id="c"></canvas>
<div class="lbl">VORONOI CELLS &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var N=10,t=0;
var seeds=[];
var pal=['#ff006e','#ff6b00','#ffd700','#06ffd0','#3a86ff','#8338ec','#ff4500','#00ffaa','#ff1493','#1e90ff'];
for(var i=0;i<N;i++) seeds.push({ax:.3+Math.random()*.5,ay:.25+Math.random()*.4,px:Math.random()*6.28,py:Math.random()*6.28});
var step=5,S=2,IW=W/S|0,IH=H/S|0;
var ofc=document.createElement('canvas');ofc.width=IW;ofc.height=IH;
var octx=ofc.getContext('2d');
function draw(){
  var sx=[],sy=[];
  for(var i=0;i<N;i++){
    sx[i]=IW*.5+Math.sin(t*seeds[i].ax+seeds[i].px)*IW*.4;
    sy[i]=IH*.5+Math.cos(t*seeds[i].ay+seeds[i].py)*IH*.38;
  }
  for(var py=0;py<IH;py+=step){
    for(var px=0;px<IW;px+=step){
      var mn=1e9,mi=0;
      for(var s=0;s<N;s++){
        var dx=px-sx[s],dy=py-sy[s],d=dx*dx+dy*dy;
        if(d<mn){mn=d;mi=s;}
      }
      octx.fillStyle=pal[mi];
      octx.fillRect(px,py,step,step);
    }
  }
  for(var i=0;i<N;i++){
    octx.fillStyle='#fff';
    octx.beginPath();octx.arc(sx[i],sy[i],4,0,Math.PI*2);octx.fill();
  }
  ctx.drawImage(ofc,0,0,W,H);
  t+=0.012;
}
setInterval(draw,40);
})();
</script></body></html>

)EOF";

static const char STRANGE_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>STRANGE ATTRACTOR &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,0,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(100,255,180,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#44ffaa;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#aaffe0}
.nav span{color:#4444aa;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(100,255,180,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>STRANGE ATTRACTOR</span></div>
<canvas id="c"></canvas>
<div class="lbl">CLIFFORD ATTRACTOR &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var sx=0.1,sy=0,st=0;
var a=-1.4,b=1.6,c=1.0,d=0.7;
ctx.fillStyle='#000';ctx.fillRect(0,0,W,H);
var cx=W/2,cy=H/2,sc=Math.min(W,H)/4.2;
function draw(){
  a=-1.4+Math.sin(st*.7)*.5; b=1.6+Math.cos(st*.5)*.4;
  c=1.0+Math.sin(st*.3)*.3;  d=0.7+Math.cos(st*.9)*.3;
  ctx.fillStyle='rgba(0,0,0,0.008)';ctx.fillRect(0,0,W,H);
  for(var i=0;i<3000;i++){
    var nx=Math.sin(a*sy)+c*Math.cos(a*sx);
    var ny=Math.sin(b*sx)+d*Math.cos(b*sy);
    sx=nx; sy=ny;
    var px=cx+sx*sc,py=cy+sy*sc;
    if(px>=0&&px<W&&py>=0&&py<H){
      var hue=(Math.atan2(sy,sx)/Math.PI*180+180)%360;
      ctx.fillStyle='hsla('+hue+',100%,62%,.5)';
      ctx.fillRect(px|0,py|0,1,1);
    }
  }
  st+=0.002;
}
setInterval(draw,30);
})();
</script></body></html>

)EOF";

static const char LISSAJOUS_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>LISSAJOUS &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,0,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(255,150,50,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#ff9933;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#ffcc88}
.nav span{color:#449933;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(255,150,50,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>LISSAJOUS</span></div>
<canvas id="c"></canvas>
<div class="lbl">LISSAJOUS FIGURES &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var lt=0;
var cvs=[
  {a:3,b:2,d:1.5708,col:'#ff006e'},
  {a:5,b:4,d:1.047, col:'#ffd700'},
  {a:4,b:3,d:0.785, col:'#06ffd0'},
  {a:7,b:6,d:0.628, col:'#8338ec'},
  {a:5,b:3,d:0.524, col:'#ff6b00'}
];
var cx=W/2,cy=H/2,rx=W*.42,ry=H*.38;
function draw(){
  ctx.fillStyle='rgba(0,0,0,0.06)';ctx.fillRect(0,0,W,H);
  for(var ci=0;ci<cvs.length;ci++){
    var cv=cvs[ci],ph=lt*.18+ci*.5;
    ctx.strokeStyle=cv.col;ctx.lineWidth=1.5;
    ctx.shadowBlur=7;ctx.shadowColor=cv.col;
    ctx.beginPath();
    for(var i=0;i<=360;i++){
      var th=i*Math.PI/180;
      var x=cx+Math.sin(cv.a*th+cv.d+ph)*rx;
      var y=cy+Math.sin(cv.b*th+ph*.7)*ry;
      i===0?ctx.moveTo(x,y):ctx.lineTo(x,y);
    }
    ctx.stroke();ctx.shadowBlur=0;
  }
  lt+=0.015;
}
setInterval(draw,33);
})();
</script></body></html>

)EOF";

static const char SIERPINSKI_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>SIERPINSKI &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,0,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(255,220,60,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#ffcc33;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#ffee88}
.nav span{color:#44cc33;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(255,220,60,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>SIERPINSKI</span></div>
<canvas id="c"></canvas>
<div class="lbl">CHAOS GAME &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var vx=[W/2,W*.06,W*.94],vy=[H*.04,H*.95,H*.95];
var px=W/2,py=H/2;
var cols=['#ff006e','#06ffd0','#ffd700'];
ctx.fillStyle='#000';ctx.fillRect(0,0,W,H);
function draw(){
  ctx.fillStyle='rgba(0,0,0,0.01)';ctx.fillRect(0,0,W,H);
  for(var i=0;i<5000;i++){
    var v=Math.random()*3|0;
    px=(px+vx[v])/2; py=(py+vy[v])/2;
    ctx.fillStyle=cols[v];
    ctx.fillRect(px|0,py|0,1,1);
  }
}
setInterval(draw,30);
})();
</script></body></html>

)EOF";

static const char SPIROGRAPH_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>SPIROGRAPH &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,0,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(200,50,255,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#cc33ff;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#ee99ff}
.nav span{color:#cc3344;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(200,50,255,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>SPIROGRAPH</span></div>
<canvas id="c"></canvas>
<div class="lbl">HYPOTROCHOID &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var spt=0;
var cfgs=[
  {R:110,r:67,d:80,col:'#ff006e'},
  {R:120,r:43,d:90,col:'#06ffd0'},
  {R:90, r:71,d:60,col:'#ffd700'},
  {R:105,r:37,d:95,col:'#8338ec'}
];
var cx=W/2,cy=H/2,sc=Math.min(W,H)/320;
function draw(){
  ctx.fillStyle='rgba(0,0,0,0.04)';ctx.fillRect(0,0,W,H);
  for(var ci=0;ci<cfgs.length;ci++){
    var cf=cfgs[ci];
    ctx.strokeStyle=cf.col;ctx.lineWidth=1.2;
    ctx.shadowBlur=5;ctx.shadowColor=cf.col;
    ctx.beginPath();
    for(var i=0;i<=720;i++){
      var th=i*Math.PI/180+spt;
      var x=cx+((cf.R-cf.r)*Math.cos(th)+cf.d*Math.cos((cf.R-cf.r)/cf.r*th))*sc;
      var y=cy+((cf.R-cf.r)*Math.sin(th)-cf.d*Math.sin((cf.R-cf.r)/cf.r*th))*sc;
      i===0?ctx.moveTo(x,y):ctx.lineTo(x,y);
    }
    ctx.stroke();ctx.shadowBlur=0;
  }
  spt+=0.006;
}
setInterval(draw,33);
})();
</script></body></html>

)EOF";

static const char BARNSLEY_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>BARNSLEY FERN &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,0,.88);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(50,220,80,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#33dd55;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#99ffaa}
.nav span{color:#33dd55;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(50,220,80,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>BARNSLEY FERN</span></div>
<canvas id="c"></canvas>
<div class="lbl">IFS FRACTAL &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var bx=0,by=0,bt=0;
var cx=W/2,base=H*.96,sc=H*.088;
ctx.fillStyle='#000';ctx.fillRect(0,0,W,H);
function draw(){
  ctx.fillStyle='rgba(0,0,0,0.006)';ctx.fillRect(0,0,W,H);
  for(var i=0;i<4000;i++){
    var r=Math.random(),nx,ny;
    if(r<0.01){nx=0;ny=0.16*by;}
    else if(r<0.86){nx=0.85*bx+0.04*by;ny=-0.04*bx+0.85*by+1.6;}
    else if(r<0.93){nx=0.2*bx-0.26*by;ny=0.23*bx+0.22*by+1.6;}
    else{nx=-0.15*bx+0.28*by;ny=0.26*bx+0.24*by+0.44;}
    bx=nx;by=ny;
    var sx=cx+bx*sc,sy=base-by*sc;
    if(sx>=0&&sx<W&&sy>=0&&sy<H){
      ctx.fillStyle='hsl('+(120+Math.sin(bt+by*.5)*40)+',90%,52%)';
      ctx.fillRect(sx|0,sy|0,1,1);
    }
  }
  bt+=0.005;
}
setInterval(draw,30);
})();
</script></body></html>

)EOF";

static const char CAMPFIRE_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>CAMPFIRE &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#020008}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(2,0,8,.92);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(255,120,0,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#ff8800;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#ffcc66}
.nav span{color:#448800;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(255,120,0,.2);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>CAMPFIRE</span></div>
<canvas id="c"></canvas>
<div class="lbl">FIRE SIMULATION &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var NUM=Math.floor(W/7)+1,t=0;
var fl=[];
for(var i=0;i<NUM;i++) fl.push({cx:i*7+(Math.random()*4-2),bh:H*.22+Math.random()*H*.2,ph:Math.random()*6.28,sp:.05+Math.random()*.1});
function draw(){
  ctx.fillStyle='rgba(2,0,8,0.14)';ctx.fillRect(0,0,W,H);
  for(var i=0;i<fl.length;i++){
    var f=fl[i];
    f.ph+=f.sp;
    var h=f.bh*(.65+Math.sin(f.ph)*.35);
    var w=10+Math.sin(f.ph*1.7)*4;
    var x=f.cx+Math.sin(f.ph*.8)*5;
    var g=ctx.createLinearGradient(x,H,x,H-h);
    g.addColorStop(0,'rgba(255,230,60,1)');
    g.addColorStop(0.2,'rgba(255,120,0,.9)');
    g.addColorStop(0.55,'rgba(200,10,0,.6)');
    g.addColorStop(1,'rgba(60,0,0,0)');
    ctx.fillStyle=g;
    ctx.beginPath();ctx.ellipse(x,H-h*.5,w/2,h/2,0,0,Math.PI*2);ctx.fill();
  }
  t++;
}
setInterval(draw,40);
})();
</script></body></html>

)EOF";

static const char RAINDROPS_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>RAINDROPS &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000810}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,8,18,.9);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(0,160,255,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#0099ff;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#66ccff}
.nav span{color:#009944;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(0,160,255,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>RAINDROPS</span></div>
<canvas id="c"></canvas>
<div class="lbl">WATER RIPPLES &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var drops=[];
function add(){
  if(drops.length<40) drops.push({x:Math.random()*W,y:Math.random()*H,r:0,mr:40+Math.random()*90,sp:1+Math.random()*2,hue:185+Math.random()*50});
}
function draw(){
  ctx.fillStyle='rgba(0,8,16,0.1)';ctx.fillRect(0,0,W,H);
  if(Math.random()<0.07) add();
  drops=drops.filter(function(d){
    d.r+=d.sp;
    if(d.r>d.mr) return false;
    var a=1-d.r/d.mr;
    ctx.strokeStyle='hsla('+d.hue+',80%,65%,'+a+')';
    ctx.lineWidth=1.2;
    ctx.beginPath();ctx.arc(d.x,d.y,d.r,0,Math.PI*2);ctx.stroke();
    if(d.r>15){
      ctx.strokeStyle='hsla('+d.hue+',60%,45%,'+(a*.5)+')';
      ctx.beginPath();ctx.arc(d.x,d.y,d.r*.65,0,Math.PI*2);ctx.stroke();
    }
    return true;
  });
}
setInterval(draw,33);
})();
</script></body></html>

)EOF";

static const char GAMEOFLIFE_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>GAME OF LIFE &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#020010}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(2,0,16,.9);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(0,255,200,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#00ffcc;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#88ffee}
.nav span{color:#0044cc;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(0,255,200,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>GAME OF LIFE</span></div>
<canvas id="c"></canvas>
<div class="lbl">CONWAY CELLULAR AUTOMATON &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var CW=75,CH=130;
var SX=W/CW,SY=H/CH;
var g=new Uint8Array(CW*CH),n=new Uint8Array(CW*CH);
for(var i=0;i<CW*CH;i++) g[i]=Math.random()<0.3?1:0;
var tick=0;
function step(){
  for(var y=0;y<CH;y++) for(var x=0;x<CW;x++){
    var nb=0;
    for(var dy=-1;dy<=1;dy++) for(var dx=-1;dx<=1;dx++){
      if(dx===0&&dy===0) continue;
      nb+=g[((y+dy+CH)%CH)*CW+(x+dx+CW)%CW];
    }
    var c=g[y*CW+x];
    n[y*CW+x]=(c?(nb===2||nb===3):nb===3)?1:0;
  }
  var t=g;g=n;n=t;
  tick++;
  if(tick%250===0) for(var i=0;i<CW*CH;i++) g[i]=Math.random()<0.3?1:0;
}
function draw(){
  ctx.fillStyle='#020010';ctx.fillRect(0,0,W,H);
  for(var y=0;y<CH;y++) for(var x=0;x<CW;x++)
    if(g[y*CW+x]){ctx.fillStyle='#00ffcc';ctx.fillRect(x*SX,y*SY,SX-.4,SY-.4);}
  step();
}
setInterval(draw,80);
})();
</script></body></html>

)EOF";

static const char AURORA_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>AURORA &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000508}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,5,10,.9);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(0,255,150,.18);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#00ee88;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#88ffcc}
.nav span{color:#00ee88;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(0,255,150,.16);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>AURORA</span></div>
<canvas id="c"></canvas>
<div class="lbl">AURORA BOREALIS &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var at=0;
function draw(){
  ctx.fillStyle='rgba(0,4,10,0.1)';ctx.fillRect(0,0,W,H);
  for(var b=0;b<5;b++){
    var baseY=H*.15+b*H*.13;
    var hue=140+b*30+Math.sin(at*.4)*25;
    ctx.beginPath();ctx.moveTo(0,baseY);
    for(var x=0;x<=W;x+=4){
      var y=baseY+Math.sin(x*.014+at*.9+b*.8)*H*.07+Math.sin(x*.028+at*.6+b*1.3)*H*.035;
      ctx.lineTo(x,y);
    }
    ctx.lineTo(W,H);ctx.lineTo(0,H);ctx.closePath();
    var g=ctx.createLinearGradient(0,baseY-H*.07,0,baseY+H*.09);
    g.addColorStop(0,'hsla('+hue+',100%,68%,0)');
    g.addColorStop(.4,'hsla('+hue+',100%,58%,.28)');
    g.addColorStop(1,'hsla('+hue+',100%,35%,0)');
    ctx.fillStyle=g;ctx.fill();
  }
  ctx.fillStyle='rgba(255,255,255,.55)';
  for(var s=0;s<25;s++){
    ctx.fillRect((s*137.5)%W|0,((s*79.3)%H*.18)|0,1,1);
  }
  at+=0.022;
}
setInterval(draw,33);
})();
</script></body></html>

)EOF";

static const char KALEIDOSCOPE_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>KALEIDOSCOPE &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,0,.9);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(255,100,255,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#ff66ff;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#ffaaff}
.nav span{color:#446644;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(255,100,255,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>KALEIDOSCOPE</span></div>
<canvas id="c"></canvas>
<div class="lbl">MIRROR SYMMETRY &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var kt=0,SEGS=8;
var cx=W/2,cy=H/2,R=Math.min(W,H)*.46;
function draw(){
  ctx.fillStyle='rgba(0,0,0,0.04)';ctx.fillRect(0,0,W,H);
  for(var s=0;s<SEGS*2;s++){
    ctx.save();
    ctx.translate(cx,cy);
    ctx.rotate(s*Math.PI/SEGS+(s%2?Math.PI/SEGS:0));
    if(s%2===1) ctx.scale(1,-1);
    for(var i=0;i<9;i++){
      var ang=kt*.35+i*.75;
      var r=R*(.08+i*.1);
      var x=Math.cos(ang)*r,y=Math.sin(ang*1.3)*r*.7;
      var hue=(kt*50+i*40+s*22)%360;
      ctx.fillStyle='hsla('+hue+',100%,62%,.65)';
      ctx.beginPath();ctx.arc(x,y,R*.058,0,Math.PI*2);ctx.fill();
    }
    ctx.restore();
  }
  kt+=0.022;
}
setInterval(draw,33);
})();
</script></body></html>

)EOF";

static const char DRAGON_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>DRAGON CURVE &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,0,.9);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(255,160,0,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#ff9900;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#ffcc66}
.nav span{color:#449900;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(255,160,0,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>DRAGON CURVE</span></div>
<canvas id="c"></canvas>
<div class="lbl">L-SYSTEM FRACTAL &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var pts=[];
(function(){
  var order=13,turns=[1];
  for(var i=1;i<order;i++){
    var n=turns.length;turns.push(1);
    for(var j=n-1;j>=0;j--) turns.push(-turns[j]);
  }
  var dir=0,cx=0,cy=0;
  pts.push([cx,cy]);
  var DX=[1,0,-1,0],DY=[0,1,0,-1];
  for(var i=0;i<turns.length;i++){
    cx+=DX[dir];cy+=DY[dir];pts.push([cx,cy]);
    dir=(dir+turns[i]+4)%4;
  }
  cx+=DX[dir];cy+=DY[dir];pts.push([cx,cy]);
  var mnX=Infinity,mxX=-Infinity,mnY=Infinity,mxY=-Infinity;
  for(var i=0;i<pts.length;i++){mnX=Math.min(mnX,pts[i][0]);mxX=Math.max(mxX,pts[i][0]);mnY=Math.min(mnY,pts[i][1]);mxY=Math.max(mxY,pts[i][1]);}
  var sc=Math.min(W/(mxX-mnX)*.85,H/(mxY-mnY)*.85);
  var ox=W/2-(mxX+mnX)/2*sc,oy=H/2-(mxY+mnY)/2*sc;
  for(var i=0;i<pts.length;i++) pts[i]=[pts[i][0]*sc+ox,pts[i][1]*sc+oy];
})();
var prog=0,N=pts.length;
function draw(){
  ctx.fillStyle='rgba(0,0,0,0.04)';ctx.fillRect(0,0,W,H);
  prog=(prog+N/90)%N;
  var end=prog|0;
  ctx.lineWidth=1;ctx.shadowBlur=4;ctx.shadowColor='#ff9900';
  ctx.beginPath();ctx.moveTo(pts[0][0],pts[0][1]);
  for(var i=1;i<=end;i++){
    ctx.strokeStyle='hsl('+(i/N*240|0)+',100%,55%)';
    ctx.lineTo(pts[i][0],pts[i][1]);
  }
  ctx.stroke();ctx.shadowBlur=0;
}
setInterval(draw,33);
})();
</script></body></html>

)EOF";

static const char LAVA2_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>LAVA LAMP &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#0a0000}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(8,0,0,.9);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(255,80,20,.2);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#ff5500;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#ff9966}
.nav span{color:#445500;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(255,80,20,.18);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>LAVA LAMP</span></div>
<canvas id="c"></canvas>
<div class="lbl">METABALLS &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var S=3,IW=W/S|0,IH=H/S|0;
var ofc=document.createElement('canvas');ofc.width=IW;ofc.height=IH;
var octx=ofc.getContext('2d');
var imgd=octx.createImageData(IW,IH);var d=imgd.data;
var blobs=[];
for(var i=0;i<7;i++) blobs.push({x:Math.random()*IW,y:Math.random()*IH,vx:(Math.random()-.5)*.6,vy:(Math.random()-.5)*.6,r:50+Math.random()*30});
function draw(){
  for(var i=0;i<blobs.length;i++){
    blobs[i].x+=blobs[i].vx;blobs[i].y+=blobs[i].vy;
    if(blobs[i].x<0||blobs[i].x>IW) blobs[i].vx*=-1;
    if(blobs[i].y<0||blobs[i].y>IH) blobs[i].vy*=-1;
  }
  for(var py=0;py<IH;py++){
    for(var px=0;px<IW;px++){
      var v=0;
      for(var i=0;i<blobs.length;i++){
        var dx=px-blobs[i].x,dy=py-blobs[i].y;
        v+=blobs[i].r*blobs[i].r/(dx*dx+dy*dy+1);
      }
      var idx=(py*IW+px)*4;
      if(v>1.8){
        var h=Math.min(1,(v-1.8)*.6);
        d[idx]=255;d[idx+1]=(h*140)|0;d[idx+2]=0;d[idx+3]=255;
      } else if(v>0.9){
        var e=(v-.9)/.9;
        d[idx]=(e*160)|0;d[idx+1]=0;d[idx+2]=0;d[idx+3]=255;
      } else {
        d[idx]=10;d[idx+1]=0;d[idx+2]=0;d[idx+3]=255;
      }
    }
  }
  octx.putImageData(imgd,0,0);ctx.drawImage(ofc,0,0,W,H);
}
setInterval(draw,40);
})();
</script></body></html>

)EOF";

static const char NOISE_HTML[] = R"EOF(
<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>NOISE FIELD &middot; COSMIC-C3</title><style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#000}
canvas{display:block;position:fixed;inset:0}
.nav{position:fixed;top:0;left:0;right:0;padding:9px 16px;
background:rgba(0,0,0,.9);backdrop-filter:blur(6px);
border-bottom:1px solid rgba(200,200,255,.18);z-index:99;
display:flex;align-items:center;justify-content:space-between;
font-family:'Courier New',monospace}
.nav a{color:#aaaaff;text-decoration:none;font-size:.62rem;letter-spacing:3px}
.nav a:hover{color:#ddddff}
.nav span{color:#aaaa44;font-size:.5rem;letter-spacing:3px}
.lbl{position:fixed;bottom:14px;width:100%;text-align:center;
font-family:'Courier New',monospace;font-size:.5rem;letter-spacing:6px;
color:rgba(200,200,255,.15);z-index:10;pointer-events:none}
</style></head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>NOISE FIELD</span></div>
<canvas id="c"></canvas>
<div class="lbl">ANIMATED SINE NOISE &middot; COSMIC-C3</div>
<script>
(function(){
var c=document.getElementById('c');
var W=c.width=window.innerWidth,H=c.height=window.innerHeight;
var ctx=c.getContext('2d');
var S=3,IW=W/S|0,IH=H/S|0;
var ofc=document.createElement('canvas');ofc.width=IW;ofc.height=IH;
var octx=ofc.getContext('2d');
var imgd=octx.createImageData(IW,IH);var d=imgd.data;
var nt=0;
function draw(){
  for(var py=0;py<IH;py++){
    for(var px=0;px<IW;px++){
      var v=Math.sin(px*.14+nt)+Math.sin(py*.1+nt*.7)
           +Math.sin((px+py)*.07+nt*.5)+Math.sin((px-py)*.055+nt*.35)
           +Math.sin(Math.sqrt(px*px+py*py)*.09+nt);
      v=v/5;
      var n=v*.5+.5;
      var idx=(py*IW+px)*4;
      d[idx]  =(Math.sin(n*6.2+nt)*.5+.5)*255|0;
      d[idx+1]=(Math.cos(n*5.1+nt*1.3)*.5+.5)*255|0;
      d[idx+2]=(Math.sin(n*7.3-nt*.9)*.5+.5)*255|0;
      d[idx+3]=255;
    }
  }
  octx.putImageData(imgd,0,0);ctx.drawImage(ofc,0,0,W,H);
  nt+=0.04;
}
setInterval(draw,33);
})();
</script></body></html>

)EOF";

// ── Request handlers ──────────────────────────────────────────────────────────

void handleIndex()   { server.send(200, "text/html", INDEX_HTML);   }
void handleMandala() { server.send(200, "text/html", MANDALA_HTML); }
void handlePlasma()  { server.send(200, "text/html", PLASMA_HTML);  }
void handleFractal() { server.send(200, "text/html", FRACTAL_HTML); }
void handleMatrix()    { server.send(200, "text/html", MATRIX_HTML);    }
void handleCyber()     { server.send(200, "text/html", CYBER_HTML);     }
void handleBinary()    { server.send(200, "text/html", BINARY_HTML);    }
void handleStarfield() { server.send(200, "text/html", STARFIELD_HTML); }
void handleParticles() { server.send(200, "text/html", PARTICLES_HTML); }
void handleTunnel()    { server.send(200, "text/html", TUNNEL_HTML);    }
void handleMfire()      { server.send(200, "text/html", MFIRE_HTML);      }
void handleMice()       { server.send(200, "text/html", MICE_HTML);       }
void handleMstorm()     { server.send(200, "text/html", MSTORM_HTML);     }
void handleMblood()     { server.send(200, "text/html", MBLOOD_HTML);     }
void handleMgold()      { server.send(200, "text/html", MGOLD_HTML);      }
void handleMvoid()      { server.send(200, "text/html", MVOID_HTML);      }
void handleMphantom()   { server.send(200, "text/html", MPHANTOM_HTML);   }
void handleMripple()    { server.send(200, "text/html", MRIPPLE_HTML);    }
void handleMglitch()    { server.send(200, "text/html", MGLITCH_HTML);    }
void handleHopalong()   { server.send(200, "text/html", HOPALONG_HTML);   }
void handleInterference(){ server.send(200, "text/html", INTERFERENCE_HTML); }
void handleVoronoi()    { server.send(200, "text/html", VORONOI_HTML);    }
void handleStrange()    { server.send(200, "text/html", STRANGE_HTML);    }
void handleLissajous()  { server.send(200, "text/html", LISSAJOUS_HTML);  }
void handleSierpinski() { server.send(200, "text/html", SIERPINSKI_HTML); }
void handleSpirograph() { server.send(200, "text/html", SPIROGRAPH_HTML); }
void handleBarnsley()   { server.send(200, "text/html", BARNSLEY_HTML);   }
void handleCampfire()   { server.send(200, "text/html", CAMPFIRE_HTML);   }
void handleRaindrops()  { server.send(200, "text/html", RAINDROPS_HTML);  }
void handleGameoflife() { server.send(200, "text/html", GAMEOFLIFE_HTML); }
void handleAurora()     { server.send(200, "text/html", AURORA_HTML);     }
void handleKaleidoscope(){ server.send(200, "text/html", KALEIDOSCOPE_HTML); }
void handleDragon()     { server.send(200, "text/html", DRAGON_HTML);     }
void handleLava2()      { server.send(200, "text/html", LAVA2_HTML);      }
void handleNoise()      { server.send(200, "text/html", NOISE_HTML);      }

// ── GAMES ──────────────────────────────────────────────────────────────────
static const char SNAKE_HTML[] = R"EOF(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"><title>SNAKE · COSMIC-C3</title>
<style>*{margin:0;padding:0;box-sizing:border-box}body{background:#000;color:#0f0;font-family:monospace;display:flex;flex-direction:column;align-items:center;height:100vh;overflow:hidden}
.nav{background:#001100;border-bottom:1px solid #0f0;padding:6px 12px;width:100%;display:flex;justify-content:space-between;align-items:center;font-size:12px}.nav a{color:#0f0;text-decoration:none}
canvas{display:block;image-rendering:pixelated;border:1px solid #0f0;box-shadow:0 0 14px #0f0;margin-top:4px}
#score{font-size:13px;color:#0f0;letter-spacing:2px}
#over{display:none;position:fixed;top:50%;left:50%;transform:translate(-50%,-50%);background:rgba(0,16,0,0.97);border:1px solid #0f0;padding:22px 28px;text-align:center;z-index:10;box-shadow:0 0 20px #0f0}
#over h2{color:#0f0;letter-spacing:4px;margin-bottom:10px;font-size:18px}#over p{color:#0a0;margin-bottom:18px;font-size:13px;letter-spacing:2px}
#over button{background:#000;border:1px solid #0f0;color:#0f0;padding:10px 24px;font-family:monospace;font-size:14px;cursor:pointer;letter-spacing:3px}#over button:hover{background:#0f0;color:#000}
#dpad{display:grid;grid-template-columns:repeat(3,50px);grid-template-rows:repeat(3,50px);gap:4px;margin:8px auto}
.btn{background:#001100;border:1px solid #0f0;color:#0f0;font-size:20px;display:flex;align-items:center;justify-content:center;border-radius:6px;user-select:none;touch-action:manipulation;cursor:pointer}.btn:active{background:#0f0;color:#000}</style>
</head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span id="score">SCORE: 0 &nbsp; HI: 0</span><span>SNAKE</span></div>
<canvas id="c"></canvas>
<div id="over"><h2>GAME OVER</h2><p id="fs">SCORE: 0</p><button onclick="init()">&#x25BA; RESTART</button></div>
<div id="dpad"><div></div><div class="btn" id="bu">&#x25B2;</div><div></div><div class="btn" id="bl">&#x25C4;</div><div></div><div class="btn" id="br">&#x25BA;</div><div></div><div class="btn" id="bd">&#x25BC;</div><div></div></div>
<script>
const C=document.getElementById('c'),ctx=C.getContext('2d');
const CELL=16,COLS=22,ROWS=20;
C.width=COLS*CELL;C.height=ROWS*CELL;
let snake,dir,nd,food,score,hi=0,tid,alive,animId;
function rand(n){return Math.floor(Math.random()*n)}
function spawn(){let x,y;do{x=rand(COLS);y=rand(ROWS)}while(snake.some(s=>s[0]==x&&s[1]==y));return[x,y]}
function init(){
  snake=[[11,10],[10,10],[9,10],[8,10]];dir=[1,0];nd=[1,0];
  food=spawn();score=0;alive=true;
  document.getElementById('over').style.display='none';
  clearInterval(tid);tid=setInterval(tick,135);
  cancelAnimationFrame(animId);loop();
}
function tick(){
  if(!alive)return;
  dir=[...nd];
  const h=[snake[0][0]+dir[0],snake[0][1]+dir[1]];
  if(h[0]<0||h[0]>=COLS||h[1]<0||h[1]>=ROWS||snake.some(s=>s[0]==h[0]&&s[1]==h[1])){
    alive=false;clearInterval(tid);
    if(score>hi)hi=score;
    document.getElementById('over').style.display='block';
    document.getElementById('fs').textContent='SCORE: '+score+(score>0&&score==hi?' \u2605 NEW HI':'');
    return;
  }
  snake.unshift(h);
  if(h[0]==food[0]&&h[1]==food[1]){
    score++;hi=Math.max(hi,score);
    document.getElementById('score').textContent='SCORE: '+score+' \u00A0 HI: '+hi;
    food=spawn();
    if(score%5==0){clearInterval(tid);tid=setInterval(tick,Math.max(55,135-score*5));}
  }else snake.pop();
}
function loop(){
  draw();animId=requestAnimationFrame(loop);
}
function draw(){
  ctx.fillStyle='#000';ctx.fillRect(0,0,C.width,C.height);
  ctx.fillStyle='#001600';
  for(let x=0;x<COLS;x++)for(let y=0;y<ROWS;y++)ctx.fillRect(x*CELL+7,y*CELL+7,2,2);
  const t=Date.now()/300;
  const r=5+Math.sin(t)*1.5;
  const fx=food[0]*CELL+CELL/2,fy=food[1]*CELL+CELL/2;
  const g=ctx.createRadialGradient(fx,fy,0,fx,fy,r+4);
  g.addColorStop(0,'#ff44aa');g.addColorStop(1,'transparent');
  ctx.fillStyle=g;ctx.beginPath();ctx.arc(fx,fy,r+4,0,Math.PI*2);ctx.fill();
  ctx.fillStyle='#ff2288';ctx.beginPath();ctx.arc(fx,fy,r,0,Math.PI*2);ctx.fill();
  snake.forEach((s,i)=>{
    const bright=Math.max(0,1-i*0.035);
    const gb=Math.floor(bright*255),gg=Math.floor(0x22+bright*0xdd);
    ctx.fillStyle='rgb(0,'+gg+','+gb+')';
    ctx.fillRect(s[0]*CELL+1,s[1]*CELL+1,CELL-2,CELL-2);
    if(i==0){
      ctx.fillStyle='#000';
      const ey=s[1]*CELL+4,ex1=s[0]*CELL+4,ex2=s[0]*CELL+CELL-7;
      ctx.fillRect(ex1,ey,3,3);ctx.fillRect(ex2,ey,3,3);
    }
  });
}
document.addEventListener('keydown',e=>{
  const m={ArrowUp:[0,-1],ArrowDown:[0,1],ArrowLeft:[-1,0],ArrowRight:[1,0],w:[0,-1],s:[0,1],a:[-1,0],d:[1,0]};
  const k=m[e.key];if(k&&!(k[0]==-dir[0]&&k[1]==-dir[1])){nd=k;e.preventDefault();}
});
function setDir(d){if(!(d[0]==-dir[0]&&d[1]==-dir[1]))nd=d;}
['bu','bd','bl','br'].forEach((id,i)=>{
  const dirs=[[0,-1],[0,1],[-1,0],[1,0]];
  const el=document.getElementById(id);
  el.addEventListener('touchstart',e=>{setDir(dirs[i]);e.preventDefault();},{passive:false});
  el.addEventListener('click',()=>setDir(dirs[i]));
});
let tx=0,ty=0;
C.addEventListener('touchstart',e=>{tx=e.touches[0].clientX;ty=e.touches[0].clientY;},{passive:true});
C.addEventListener('touchend',e=>{
  const dx=e.changedTouches[0].clientX-tx,dy=e.changedTouches[0].clientY-ty;
  if(Math.abs(dx)>Math.abs(dy)){setDir(dx>20?[1,0]:[-1,0]);}else{setDir(dy>20?[0,1]:[0,-1]);}
},{passive:true});
init();
</script>
</body></html>
)EOF";

static const char FIREWORKS_HTML[] = R"EOF(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>FIREWORKS · COSMIC-C3</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#000;overflow:hidden;font-family:monospace}
.nav{position:fixed;top:0;left:0;right:0;background:rgba(0,0,0,.7);
  border-bottom:1px solid #ffaa00;padding:5px 10px;display:flex;
  justify-content:space-between;font-size:11px;z-index:9}
.nav a{{color:#ffaa00;text-decoration:none}}
canvas{display:block}
</style>
</head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>FIREWORKS</span></div>
<canvas id="c"></canvas>
<script>
const C=document.getElementById('c');
const ctx=C.getContext('2d');
let W,H;
function resize(){W=C.width=innerWidth;H=C.height=innerHeight;}
window.addEventListener('resize',()=>{resize();init&&init();});
resize();

const rockets=[];
const sparks=[];
function launch(){
  const x=W*0.15+Math.random()*W*0.7;
  rockets.push({x,y:H,tx:W*0.1+Math.random()*W*0.8,ty:H*0.1+Math.random()*H*0.4,trail:[]});
}
function burst(x,y){
  const hue=Math.random()*360;
  const n=60+Math.floor(Math.random()*40);
  for(let i=0;i<n;i++){
    const a=Math.random()*Math.PI*2,s=2+Math.random()*5;
    sparks.push({x,y,vx:Math.cos(a)*s,vy:Math.sin(a)*s,life:1,
      r:Math.round(Math.sin((hue/360)*Math.PI*2)*127+128),
      g:Math.round(Math.sin((hue/360)*Math.PI*2+2.09)*127+128),
      b:Math.round(Math.sin((hue/360)*Math.PI*2+4.19)*127+128)});
  }
}
setInterval(launch,700+Math.random()*500);
function draw(){
  ctx.fillStyle='rgba(0,0,0,0.18)';ctx.fillRect(0,0,W,H);
  for(let i=rockets.length-1;i>=0;i--){
    const r=rockets[i];
    const dx=r.tx-r.x,dy=r.ty-r.y,d=Math.sqrt(dx*dx+dy*dy);
    r.trail.push([r.x,r.y]);if(r.trail.length>12)r.trail.shift();
    if(d<6){burst(r.x,r.y);rockets.splice(i,1);continue;}
    const sp=4+Math.random()*2;r.x+=dx/d*sp;r.y+=dy/d*sp;
    r.trail.forEach(([tx,ty],j)=>{
      ctx.fillStyle=`rgba(255,200,80,${j/r.trail.length*0.8})`;
      ctx.beginPath();ctx.arc(tx,ty,1.5,0,Math.PI*2);ctx.fill();
    });
    ctx.fillStyle='#ffe080';ctx.beginPath();ctx.arc(r.x,r.y,2,0,Math.PI*2);ctx.fill();
  }
  for(let i=sparks.length-1;i>=0;i--){
    const s=sparks[i];
    s.x+=s.vx;s.y+=s.vy;s.vy+=0.08;s.vx*=0.98;s.life-=0.016;
    if(s.life<=0){sparks.splice(i,1);continue;}
    ctx.fillStyle=`rgba(${s.r},${s.g},${s.b},${s.life})`;
    ctx.beginPath();ctx.arc(s.x,s.y,1.5*s.life,0,Math.PI*2);ctx.fill();
  }
  requestAnimationFrame(draw);
}
draw();

</script>
</body></html>
)EOF";

static const char CORAL_HTML[] = R"EOF(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>CORAL REEF · COSMIC-C3</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#000;overflow:hidden;font-family:monospace}
.nav{position:fixed;top:0;left:0;right:0;background:rgba(0,0,0,.7);
  border-bottom:1px solid #ff6688;padding:5px 10px;display:flex;
  justify-content:space-between;font-size:11px;z-index:9}
.nav a{{color:#ff6688;text-decoration:none}}
canvas{display:block}
</style>
</head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>CORAL REEF</span></div>
<canvas id="c"></canvas>
<script>
const C=document.getElementById('c');
const ctx=C.getContext('2d');
let W,H;
function resize(){W=C.width=innerWidth;H=C.height=innerHeight;}
window.addEventListener('resize',()=>{resize();init&&init();});
resize();

const COLS=60,ROWS=45;
const grid=Array.from({length:COLS},()=>new Uint8Array(ROWS));
const age=Array.from({length:COLS},()=>new Float32Array(ROWS));
let gen=0;
// seed bottom row
for(let x=0;x<COLS;x++)if(Math.random()<0.35){grid[x][ROWS-1]=1;age[x][ROWS-1]=Math.random();}
function step(){
  const ng=Array.from({length:COLS},(_,x)=>new Uint8Array(grid[x]));
  for(let x=1;x<COLS-1;x++)for(let y=1;y<ROWS-1;y++){
    if(!grid[x][y]){
      let n=0;
      for(let dx=-1;dx<=1;dx++)for(let dy=-1;dy<=1;dy++){if(dx==0&&dy==0)continue;if(grid[x+dx][y+dy])n++;}
      if(n>=1&&n<=3&&grid[x][y+1]&&Math.random()<0.18)ng[x][y]=1;
    }
  }
  for(let x=0;x<COLS;x++)for(let y=0;y<ROWS;y++){grid[x][y]=ng[x][y];if(grid[x][y])age[x][y]+=0.002;}
  gen++;if(gen>300){gen=0;for(let x=0;x<COLS;x++)for(let y=0;y<ROWS;y++){grid[x][y]=0;age[x][y]=0;}
    for(let x=0;x<COLS;x++)if(Math.random()<0.35){grid[x][ROWS-1]=1;age[x][ROWS-1]=Math.random();}}
}
let t=0;
function draw(){
  t+=0.02;
  ctx.fillStyle='#000c1a';ctx.fillRect(0,0,W,H);
  const cw=W/COLS,ch=H/ROWS;
  for(let x=0;x<COLS;x++)for(let y=0;y<ROWS;y++){
    if(grid[x][y]){
      const d=(ROWS-y)/ROWS,a=age[x][y];
      const r=Math.round(220-d*100+Math.sin(t+x*0.3)*20);
      const g=Math.round(80+d*120+Math.sin(t*0.7+y*0.2)*15);
      const b=Math.round(120+d*80);
      ctx.fillStyle=`rgb(${r},${g},${b})`;
      ctx.fillRect(x*cw,y*ch,cw+1,ch+1);
    }
  }
  // water shimmer
  for(let bx=0;bx<W;bx+=40){
    const sy=H*0.05+Math.sin(t+bx*0.04)*8;
    ctx.strokeStyle='rgba(100,180,255,0.12)';ctx.lineWidth=2;
    ctx.beginPath();ctx.moveTo(bx,sy);ctx.lineTo(bx+40,sy+Math.sin(t+bx*0.05)*6);ctx.stroke();
  }
  if(t%0.5<0.02)step();
  requestAnimationFrame(draw);
}
draw();setInterval(step,80);

</script>
</body></html>
)EOF";

static const char CWAVES_HTML[] = R"EOF(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>C-WAVES · COSMIC-C3</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#000;overflow:hidden;font-family:monospace}
.nav{position:fixed;top:0;left:0;right:0;background:rgba(0,0,0,.7);
  border-bottom:1px solid #44aaff;padding:5px 10px;display:flex;
  justify-content:space-between;font-size:11px;z-index:9}
.nav a{{color:#44aaff;text-decoration:none}}
canvas{display:block}
</style>
</head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>C-WAVES</span></div>
<canvas id="c"></canvas>
<script>
const C=document.getElementById('c');
const ctx=C.getContext('2d');
let W,H;
function resize(){W=C.width=innerWidth;H=C.height=innerHeight;}
window.addEventListener('resize',()=>{resize();init&&init();});
resize();

let t=0;
function draw(){
  t+=0.025;
  ctx.fillStyle='rgba(0,0,18,0.25)';ctx.fillRect(0,0,W,H);
  const cx=W/2,cy=H/2;
  for(let w=0;w<6;w++){
    const off=w*Math.PI/3+t*0.25;
    const amp=15+w*10,rad=30+w*20;
    const hue=(w*55+t*20)%360;
    ctx.strokeStyle=`hsla(${hue},100%,65%,0.8)`;ctx.lineWidth=2;
    ctx.beginPath();let first=true;
    for(let a=30;a<=330;a+=3){
      const ra=a*Math.PI/180+off;
      const wh=Math.sin(ra*3+t*4)*amp*0.3+Math.sin(ra*2+t*2)*amp*0.2;
      const cr=rad+wh;
      const x=cx+Math.cos(ra)*cr,y=cy+Math.sin(ra)*cr;
      first?(ctx.moveTo(x,y),first=false):ctx.lineTo(x,y);
    }
    ctx.stroke();
    // glow endpoints
    const s=30*Math.PI/180+off,e=330*Math.PI/180+off;
    ctx.fillStyle=`hsla(${hue},100%,90%,0.9)`;
    ctx.beginPath();ctx.arc(cx+Math.cos(s)*rad,cy+Math.sin(s)*rad,4,0,Math.PI*2);ctx.fill();
    ctx.beginPath();ctx.arc(cx+Math.cos(e)*rad,cy+Math.sin(e)*rad,4,0,Math.PI*2);ctx.fill();
  }
  requestAnimationFrame(draw);
}
draw();

</script>
</body></html>
)EOF";

static const char DEEPSTARS_HTML[] = R"EOF(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>DEEP STARS · COSMIC-C3</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#000;overflow:hidden;font-family:monospace}
.nav{position:fixed;top:0;left:0;right:0;background:rgba(0,0,0,.7);
  border-bottom:1px solid #aaaaff;padding:5px 10px;display:flex;
  justify-content:space-between;font-size:11px;z-index:9}
.nav a{{color:#aaaaff;text-decoration:none}}
canvas{display:block}
</style>
</head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>DEEP STARS</span></div>
<canvas id="c"></canvas>
<script>
const C=document.getElementById('c');
const ctx=C.getContext('2d');
let W,H;
function resize(){W=C.width=innerWidth;H=C.height=innerHeight;}
window.addEventListener('resize',()=>{resize();init&&init();});
resize();

const N=150;
const sx=new Float32Array(N),sy=new Float32Array(N),sz=new Float32Array(N),sbr=new Float32Array(N);
for(let i=0;i<N;i++){sx[i]=(Math.random()-0.5)*400;sy[i]=(Math.random()-0.5)*300;sz[i]=Math.random()*100+1;sbr[i]=80+Math.random()*175;}
let t=0;
function draw(){
  t+=0.016;
  ctx.fillStyle='#000007';ctx.fillRect(0,0,W,H);
  // nebula wisps
  for(let nx=0;nx<W;nx+=20){for(let ny=0;ny<H;ny+=20){
    const nd=Math.sin((nx+t*8)*0.018)*Math.cos((ny+t*6)*0.022);
    if(nd>0.68){const ni=(nd-0.68)*350|0;ctx.fillStyle=`rgba(${ni>>2},0,${ni},0.4)`;ctx.fillRect(nx,ny,20,20);}
  }}
  const cx=W/2,cy=H/2;
  for(let i=0;i<N;i++){
    sz[i]-=0.4+Math.sin(t*0.08)*0.25;
    if(sz[i]<1){sx[i]=(Math.random()-0.5)*400;sy[i]=(Math.random()-0.5)*300;sz[i]=90+Math.random()*10;sbr[i]=80+Math.random()*175;}
    const px=cx+sx[i]/sz[i]*120,py=cy+sy[i]/sz[i]*120;
    if(px<0||px>W||py<0||py>H)continue;
    const size=Math.max(1,(100/sz[i])*2.5)|0;
    const br=Math.min(255,(sbr[i]/sz[i]*22))|0;
    let color;
    if(br>200)color=`rgb(${br},${br},255)`;
    else if(br>140)color=`rgb(255,${br},${br})`;
    else if(br>90)color=`rgb(255,${br},${br>>1})`;
    else color=`rgb(${br},${br>>1},${br>>2})`;
    ctx.fillStyle=color;ctx.beginPath();ctx.arc(px,py,size,0,Math.PI*2);ctx.fill();
    if(sz[i]<18&&(t*10+i)%3<1){ctx.strokeStyle=`rgba(${br},${br},255,0.4)`;ctx.lineWidth=1;ctx.beginPath();ctx.arc(px,py,size+2,0,Math.PI*2);ctx.stroke();}
  }
  requestAnimationFrame(draw);
}
draw();

</script>
</body></html>
)EOF";

static const char FLOWFIELD_HTML[] = R"EOF(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>FLOW FIELD · COSMIC-C3</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#000;overflow:hidden;font-family:monospace}
.nav{position:fixed;top:0;left:0;right:0;background:rgba(0,0,0,.7);
  border-bottom:1px solid #ff8844;padding:5px 10px;display:flex;
  justify-content:space-between;font-size:11px;z-index:9}
.nav a{{color:#ff8844;text-decoration:none}}
canvas{display:block}
</style>
</head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>FLOW FIELD</span></div>
<canvas id="c"></canvas>
<script>
const C=document.getElementById('c');
const ctx=C.getContext('2d');
let W,H;
function resize(){W=C.width=innerWidth;H=C.height=innerHeight;}
window.addEventListener('resize',()=>{resize();init&&init();});
resize();

const NP=120;
const px=new Float32Array(NP),py=new Float32Array(NP),pa=new Float32Array(NP);
for(let i=0;i<NP;i++){px[i]=Math.random()*800;py[i]=Math.random()*600;pa[i]=Math.random()*2;}
let t=0;
function init(){for(let i=0;i<NP;i++){px[i]=Math.random()*W;py[i]=Math.random()*H;pa[i]=Math.random()*2;}}
function draw(){
  t+=0.03;
  ctx.fillStyle='rgba(0,0,0,0.04)';ctx.fillRect(0,0,W,H);
  const sc=0.003;
  for(let i=0;i<NP;i++){
    const fx=px[i]*sc,fy=py[i]*sc;
    const angle=Math.sin(fx+t)*Math.cos(fy+t)*Math.PI;
    const strength=(Math.sin(fx*2+t*0.5)+1)*0.5;
    const vx=Math.cos(angle)*strength*2.2,vy=Math.sin(angle)*strength*2.2;
    const ox=px[i],oy=py[i];
    px[i]+=vx;py[i]+=vy;pa[i]+=0.007;
    if(px[i]<0||px[i]>W||py[i]<0||py[i]>H||pa[i]>2){px[i]=Math.random()*W;py[i]=Math.random()*H;pa[i]=0;continue;}
    const al=1-pa[i]/2;
    const hue=(angle/Math.PI*180+t*30)%360;
    ctx.strokeStyle=`hsla(${hue|0},100%,65%,${al*0.7})`;ctx.lineWidth=1.5;
    ctx.beginPath();ctx.moveTo(ox,oy);ctx.lineTo(px[i],py[i]);ctx.stroke();
  }
  requestAnimationFrame(draw);
}
draw();

</script>
</body></html>
)EOF";

static const char METABALLS_HTML[] = R"EOF(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>METABALLS · COSMIC-C3</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#000;overflow:hidden;font-family:monospace}
.nav{position:fixed;top:0;left:0;right:0;background:rgba(0,0,0,.7);
  border-bottom:1px solid #cc44ff;padding:5px 10px;display:flex;
  justify-content:space-between;font-size:11px;z-index:9}
.nav a{{color:#cc44ff;text-decoration:none}}
canvas{display:block}
</style>
</head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>METABALLS</span></div>
<canvas id="c"></canvas>
<script>
const C=document.getElementById('c');
const ctx=C.getContext('2d');
let W,H;
function resize(){W=C.width=innerWidth;H=C.height=innerHeight;}
window.addEventListener('resize',()=>{resize();init&&init();});
resize();

const S=3;let IW,IH,ofc,octx;
function init(){IW=W/S|0;IH=H/S|0;ofc=new OffscreenCanvas(IW,IH);octx=ofc.getContext('2d');}
let t=0;
function draw(){
  t+=0.02;
  const id=octx.createImageData(IW,IH);const d=id.data;
  for(let y=0;y<IH;y++)for(let x=0;x<IW;x++){
    let f=0;
    for(let b=0;b<4;b++){
      const bx=IW/2+Math.cos(t+b*Math.PI/2)*IW*0.28;
      const by=IH/2+Math.sin(t*0.7+b*Math.PI/2)*IH*0.28;
      const dx=x-bx,dy=y-by,dist=Math.sqrt(dx*dx+dy*dy)||0.01;
      f+=900/(dist*dist);
    }
    if(f>1){
      const ff=Math.min(f,8);
      const i=(y*IW+x)*4;
      d[i]=Math.min(255,ff*40)|0;
      d[i+1]=Math.min(255,ff*20)|0;
      d[i+2]=Math.min(255,ff*70)|0;
      d[i+3]=255;
    }
  }
  octx.putImageData(id,0,0);
  ctx.fillStyle='#000';ctx.fillRect(0,0,W,H);
  ctx.drawImage(ofc,0,0,W,H);
  requestAnimationFrame(draw);
}
init();draw();

</script>
</body></html>
)EOF";

static const char GOOP_HTML[] = R"EOF(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>GOOP · COSMIC-C3</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#000;overflow:hidden;font-family:monospace}
.nav{position:fixed;top:0;left:0;right:0;background:rgba(0,0,0,.7);
  border-bottom:1px solid #88ff44;padding:5px 10px;display:flex;
  justify-content:space-between;font-size:11px;z-index:9}
.nav a{{color:#88ff44;text-decoration:none}}
canvas{display:block}
</style>
</head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>GOOP</span></div>
<canvas id="c"></canvas>
<script>
const C=document.getElementById('c');
const ctx=C.getContext('2d');
let W,H;
function resize(){W=C.width=innerWidth;H=C.height=innerHeight;}
window.addEventListener('resize',()=>{resize();init&&init();});
resize();

let t=0;
function draw(){
  t+=0.018;
  ctx.fillStyle='rgba(0,0,0,0.15)';ctx.fillRect(0,0,W,H);
  const cx=W/2,cy=H/2;
  const blobs=[];
  for(let b=0;b<8;b++){
    const a=b*Math.PI/4+t*0.5;
    const r=Math.min(W,H)*0.22+Math.sin(t*2+b)*Math.min(W,H)*0.06;
    blobs.push({x:cx+Math.cos(a)*r,y:cy+Math.sin(a)*r,
      sz:Math.min(W,H)*0.05+Math.sin(t*3+b*0.7)*Math.min(W,H)*0.025,
      hue:(b*45+t*25)%360});
  }
  blobs.forEach((bl,i)=>{
    const g=ctx.createRadialGradient(bl.x,bl.y,0,bl.x,bl.y,bl.sz*1.6);
    g.addColorStop(0,`hsla(${bl.hue},100%,65%,0.9)`);
    g.addColorStop(1,'transparent');
    ctx.fillStyle=g;ctx.beginPath();ctx.arc(bl.x,bl.y,bl.sz*1.6,0,Math.PI*2);ctx.fill();
    if(i<7){
      const n=blobs[i+1];
      ctx.strokeStyle=`hsla(${bl.hue},100%,60%,0.4)`;ctx.lineWidth=3;
      ctx.beginPath();ctx.moveTo(bl.x,bl.y);
      ctx.quadraticCurveTo(cx+Math.sin(t)*30,cy+Math.cos(t)*30,n.x,n.y);ctx.stroke();
    }
  });
  requestAnimationFrame(draw);
}
draw();

</script>
</body></html>
)EOF";

static const char WORMHOLE_HTML[] = R"EOF(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>WORMHOLE · COSMIC-C3</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#000;overflow:hidden;font-family:monospace}
.nav{position:fixed;top:0;left:0;right:0;background:rgba(0,0,0,.7);
  border-bottom:1px solid #00ffcc;padding:5px 10px;display:flex;
  justify-content:space-between;font-size:11px;z-index:9}
.nav a{{color:#00ffcc;text-decoration:none}}
canvas{display:block}
</style>
</head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>WORMHOLE</span></div>
<canvas id="c"></canvas>
<script>
const C=document.getElementById('c');
const ctx=C.getContext('2d');
let W,H;
function resize(){W=C.width=innerWidth;H=C.height=innerHeight;}
window.addEventListener('resize',()=>{resize();init&&init();});
resize();

let t=0;
function draw(){
  t+=0.025;
  ctx.fillStyle='rgba(0,0,0,0.2)';ctx.fillRect(0,0,W,H);
  const cx=W/2,cy=H/2;
  const N=28;
  for(let i=0;i<N;i++){
    const phase=t+i*(Math.PI*2/N)*2;
    const rad=Math.max(4,(1-i/N)*Math.min(W,H)*0.46);
    const hue=(i*13+t*40)%360;
    const alpha=0.55+Math.sin(phase)*0.25;
    ctx.strokeStyle=`hsla(${hue|0},100%,65%,${alpha})`;
    ctx.lineWidth=1.5+(i/N)*2;
    // slightly squished rotating ellipse
    ctx.save();ctx.translate(cx,cy);ctx.rotate(t*0.4+i*0.18);
    ctx.beginPath();ctx.ellipse(0,0,rad,rad*0.62,0,0,Math.PI*2);ctx.stroke();
    ctx.restore();
  }
  // center vortex glow
  const g=ctx.createRadialGradient(cx,cy,0,cx,cy,Math.min(W,H)*0.08);
  g.addColorStop(0,'rgba(0,255,200,0.7)');g.addColorStop(1,'transparent');
  ctx.fillStyle=g;ctx.beginPath();ctx.arc(cx,cy,Math.min(W,H)*0.08,0,Math.PI*2);ctx.fill();
  requestAnimationFrame(draw);
}
draw();

</script>
</body></html>
)EOF";

static const char CRYSTAL_HTML[] = R"EOF(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>CRYSTAL · COSMIC-C3</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#000;overflow:hidden;font-family:monospace}
.nav{position:fixed;top:0;left:0;right:0;background:rgba(0,0,0,.7);
  border-bottom:1px solid #aaddff;padding:5px 10px;display:flex;
  justify-content:space-between;font-size:11px;z-index:9}
.nav a{{color:#aaddff;text-decoration:none}}
canvas{display:block}
</style>
</head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>CRYSTAL</span></div>
<canvas id="c"></canvas>
<script>
const C=document.getElementById('c');
const ctx=C.getContext('2d');
let W,H;
function resize(){W=C.width=innerWidth;H=C.height=innerHeight;}
window.addEventListener('resize',()=>{resize();init&&init();});
resize();

let t=0;
function draw(){
  t+=0.022;
  ctx.fillStyle='rgba(0,0,20,0.22)';ctx.fillRect(0,0,W,H);
  const cx=W/2,cy=H/2;
  for(let layer=0;layer<6;layer++){
    const la=t*0.3+layer*Math.PI/3;
    const lr=Math.min(W,H)*(0.06+layer*0.07);
    for(let k=0;k<6;k++){
      const a=la+k*Math.PI/3;
      const kx=cx+Math.cos(a)*lr,ky=cy+Math.sin(a)*lr;
      const sz=Math.min(W,H)*(0.025+Math.sin(t*2+layer+k)*0.012);
      const hue=(layer*60+k*20+t*30)%360;
      ctx.strokeStyle=`hsla(${hue|0},100%,75%,0.85)`;ctx.lineWidth=1.5;
      for(let s=0;s<6;s++){
        const sa=a+s*Math.PI/3;
        ctx.beginPath();ctx.moveTo(kx,ky);
        ctx.lineTo(kx+Math.cos(sa)*sz,ky+Math.sin(sa)*sz);ctx.stroke();
        ctx.beginPath();
        ctx.moveTo(kx+Math.cos(sa)*sz,ky+Math.sin(sa)*sz);
        ctx.lineTo(kx+Math.cos(sa+Math.PI/3)*sz,ky+Math.sin(sa+Math.PI/3)*sz);ctx.stroke();
      }
      ctx.fillStyle=`hsla(${hue|0},100%,95%,0.9)`;
      ctx.beginPath();ctx.arc(kx,ky,2,0,Math.PI*2);ctx.fill();
    }
  }
  // drifting sparkles
  for(let i=0;i<18;i++){
    const pt=t*1.4+i*0.4;
    const px=cx+Math.sin(pt)*Math.min(W,H)*0.32+Math.cos(pt*0.7)*Math.min(W,H)*0.15;
    const py=cy+Math.cos(pt)*Math.min(W,H)*0.25+Math.sin(pt*1.3)*Math.min(W,H)*0.12;
    const sp=3+Math.abs(Math.sin(pt*8))*4|0;
    ctx.fillStyle=`rgba(180,220,255,${0.6+Math.sin(pt*8)*0.4})`;
    ctx.beginPath();ctx.arc(px,py,sp,0,Math.PI*2);ctx.fill();
  }
  requestAnimationFrame(draw);
}
draw();

</script>
</body></html>
)EOF";

static const char LIGHTNING_HTML[] = R"EOF(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>LIGHTNING · COSMIC-C3</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#000;overflow:hidden;font-family:monospace}
.nav{position:fixed;top:0;left:0;right:0;background:rgba(0,0,0,.7);
  border-bottom:1px solid #eeeeff;padding:5px 10px;display:flex;
  justify-content:space-between;font-size:11px;z-index:9}
.nav a{{color:#eeeeff;text-decoration:none}}
canvas{display:block}
</style>
</head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>LIGHTNING</span></div>
<canvas id="c"></canvas>
<script>
const C=document.getElementById('c');
const ctx=C.getContext('2d');
let W,H;
function resize(){W=C.width=innerWidth;H=C.height=innerHeight;}
window.addEventListener('resize',()=>{resize();init&&init();});
resize();

let bolts=[];
function bolt(x1,y1,x2,y2,depth,hue){
  if(depth<=0){return[[x1,y1,x2,y2,hue]];}
  const mx=(x1+x2)/2+(Math.random()-0.5)*(Math.abs(x2-x1)+Math.abs(y2-y1))*0.4;
  const my=(y1+y2)/2+(Math.random()-0.5)*(Math.abs(x2-x1)+Math.abs(y2-y1))*0.4;
  const forks=bolt(x1,y1,mx,my,depth-1,hue).concat(bolt(mx,my,x2,y2,depth-1,hue));
  if(depth>=2&&Math.random()<0.4){
    const fa=Math.random()*Math.PI*2;
    const fl=Math.min(W,H)*0.12;
    forks.push(...bolt(mx,my,mx+Math.cos(fa)*fl,my+Math.sin(fa)*fl,depth-2,(hue+40)%360));
  }
  return forks;
}
function newStrike(){
  const sx=W*0.1+Math.random()*W*0.8,hue=180+Math.random()*80;
  bolts.push({segs:bolt(sx,0,sx+(Math.random()-0.5)*W*0.3,H*0.5+Math.random()*H*0.4,5,hue),life:1});
}
setInterval(newStrike,300+Math.random()*400);
let t=0;
function draw(){
  t+=0.05;
  ctx.fillStyle='rgba(0,0,10,0.3)';ctx.fillRect(0,0,W,H);
  bolts=bolts.filter(b=>b.life>0);
  bolts.forEach(b=>{
    b.segs.forEach(([x1,y1,x2,y2,hue])=>{
      ctx.strokeStyle=`rgba(200,200,255,${b.life*0.7})`;ctx.lineWidth=b.life*2;
      ctx.beginPath();ctx.moveTo(x1,y1);ctx.lineTo(x2,y2);ctx.stroke();
      ctx.strokeStyle=`rgba(255,255,255,${b.life*0.9})`;ctx.lineWidth=b.life*0.5;
      ctx.beginPath();ctx.moveTo(x1,y1);ctx.lineTo(x2,y2);ctx.stroke();
    });
    b.life-=0.045;
  });
  requestAnimationFrame(draw);
}
draw();

</script>
</body></html>
)EOF";

static const char BOUNCEBALLS_HTML[] = R"EOF(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>BOUNCE BALLS · COSMIC-C3</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#000;overflow:hidden;font-family:monospace}
.nav{position:fixed;top:0;left:0;right:0;background:rgba(0,0,0,.7);
  border-bottom:1px solid #ff44cc;padding:5px 10px;display:flex;
  justify-content:space-between;font-size:11px;z-index:9}
.nav a{{color:#ff44cc;text-decoration:none}}
canvas{display:block}
</style>
</head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>BOUNCE BALLS</span></div>
<canvas id="c"></canvas>
<script>
const C=document.getElementById('c');
const ctx=C.getContext('2d');
let W,H;
function resize(){W=C.width=innerWidth;H=C.height=innerHeight;}
window.addEventListener('resize',()=>{resize();init&&init();});
resize();

const N=18;
const bx=new Float32Array(N),by=new Float32Array(N),bvx=new Float32Array(N),bvy=new Float32Array(N);
const br=new Float32Array(N),bhue=new Float32Array(N);
function init(){for(let i=0;i<N;i++){bx[i]=Math.random()*W;by[i]=Math.random()*H;
  bvx[i]=(Math.random()-0.5)*5;bvy[i]=(Math.random()-0.5)*5;
  br[i]=8+Math.random()*16;bhue[i]=Math.random()*360;}}
let t=0;
function draw(){
  t+=0.02;
  ctx.fillStyle='rgba(0,0,0,0.12)';ctx.fillRect(0,0,W,H);
  for(let i=0;i<N;i++){
    bx[i]+=bvx[i];by[i]+=bvy[i];
    if(bx[i]<br[i]){bx[i]=br[i];bvx[i]=Math.abs(bvx[i]);}
    if(bx[i]>W-br[i]){bx[i]=W-br[i];bvx[i]=-Math.abs(bvx[i]);}
    if(by[i]<br[i]){by[i]=br[i];bvy[i]=Math.abs(bvy[i]);}
    if(by[i]>H-br[i]){by[i]=H-br[i];bvy[i]=-Math.abs(bvy[i]);}
    // ball-ball collisions
    for(let j=i+1;j<N;j++){
      const dx=bx[j]-bx[i],dy=by[j]-by[i],d=Math.sqrt(dx*dx+dy*dy);
      if(d<br[i]+br[j]&&d>0.01){
        const nx=dx/d,ny=dy/d;
        const rv=(bvx[i]-bvx[j])*nx+(bvy[i]-bvy[j])*ny;
        if(rv>0){bvx[i]-=rv*nx;bvy[i]-=rv*ny;bvx[j]+=rv*nx;bvy[j]+=rv*ny;}
      }
    }
    bhue[i]=(bhue[i]+0.3)%360;
    const g=ctx.createRadialGradient(bx[i]-br[i]*0.3,by[i]-br[i]*0.3,1,bx[i],by[i],br[i]*1.4);
    g.addColorStop(0,`hsla(${bhue[i]},100%,85%,0.9)`);
    g.addColorStop(0.5,`hsla(${bhue[i]},100%,55%,0.8)`);
    g.addColorStop(1,'transparent');
    ctx.fillStyle=g;ctx.beginPath();ctx.arc(bx[i],by[i],br[i]*1.4,0,Math.PI*2);ctx.fill();
  }
  requestAnimationFrame(draw);
}
init();draw();

</script>
</body></html>
)EOF";

static const char NEONRAIN_HTML[] = R"EOF(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>NEON RAIN · COSMIC-C3</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#000;overflow:hidden;font-family:monospace}
.nav{position:fixed;top:0;left:0;right:0;background:rgba(0,0,0,.7);
  border-bottom:1px solid #00ff88;padding:5px 10px;display:flex;
  justify-content:space-between;font-size:11px;z-index:9}
.nav a{{color:#00ff88;text-decoration:none}}
canvas{display:block}
</style>
</head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>NEON RAIN</span></div>
<canvas id="c"></canvas>
<script>
const C=document.getElementById('c');
const ctx=C.getContext('2d');
let W,H;
function resize(){W=C.width=innerWidth;H=C.height=innerHeight;}
window.addEventListener('resize',()=>{resize();init&&init();});
resize();

const COLS2=Math.ceil(innerWidth/10)||40;
const streams2=[];
const SYM='@#%&*!?+=-~<>^[]{}|/\\';
for(let i=0;i<COLS2;i++)streams2.push({x:i*10,y:-Math.random()*400,speed:0.6+Math.random()*1.8,
  color:Math.random()<0.7?[0,255,80+Math.floor(Math.random()*60)]:[0,100+Math.floor(Math.random()*80),255],
  sym:SYM[Math.floor(Math.random()*SYM.length)]});
let t2=0;
function draw(){
  t2+=1;
  ctx.fillStyle='rgba(0,0,0,0.15)';ctx.fillRect(0,0,W,H);
  streams2.forEach(s=>{
    const [r,g,b]=s.color;
    ctx.fillStyle=`rgb(${r},${g},${b})`;
    ctx.font='bold 9px monospace';ctx.fillText(s.sym,s.x,s.y);
    s.y+=s.speed;
    if(t2%7===0)s.sym=SYM[Math.floor(Math.random()*SYM.length)];
    if(s.y>H+10){s.y=-Math.random()*200;s.speed=0.6+Math.random()*1.8;}
  });
  requestAnimationFrame(draw);
}
draw();

</script>
</body></html>
)EOF";

static const char DNA_HTML[] = R"EOF(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>DNA HELIX · COSMIC-C3</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#000;overflow:hidden;font-family:monospace}
.nav{position:fixed;top:0;left:0;right:0;background:rgba(0,0,0,.7);
  border-bottom:1px solid #ff66aa;padding:5px 10px;display:flex;
  justify-content:space-between;font-size:11px;z-index:9}
.nav a{{color:#ff66aa;text-decoration:none}}
canvas{display:block}
</style>
</head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>DNA HELIX</span></div>
<canvas id="c"></canvas>
<script>
const C=document.getElementById('c');
const ctx=C.getContext('2d');
let W,H;
function resize(){W=C.width=innerWidth;H=C.height=innerHeight;}
window.addEventListener('resize',()=>{resize();init&&init();});
resize();

let t=0;
function draw(){
  t+=0.03;
  ctx.fillStyle='rgba(0,0,0,0.2)';ctx.fillRect(0,0,W,H);
  const cx=W/2,amp=Math.min(W,H)*0.3;
  const N=60,step=H/N;
  const pts1=[],pts2=[];
  for(let i=0;i<N;i++){
    const y=i*step,phase=i*0.22+t;
    pts1.push([cx+Math.cos(phase)*amp, y]);
    pts2.push([cx+Math.cos(phase+Math.PI)*amp, y]);
  }
  // rungs
  for(let i=0;i<N;i+=3){
    const [x1,y1]=pts1[i],[x2,y2]=pts2[i];
    const hue=(i*6+t*30)%360;
    ctx.strokeStyle=`hsla(${hue|0},100%,65%,0.55)`;ctx.lineWidth=2;
    ctx.beginPath();ctx.moveTo(x1,y1);ctx.lineTo(x2,y2);ctx.stroke();
    ctx.fillStyle=`hsla(${hue|0},100%,80%,0.9)`;
    ctx.beginPath();ctx.arc(x1,y1,4,0,Math.PI*2);ctx.fill();
    ctx.beginPath();ctx.arc(x2,y2,4,0,Math.PI*2);ctx.fill();
  }
  // strands
  [[pts1,'#ff44aa'],[pts2,'#44aaff']].forEach(([pts,col])=>{
    ctx.strokeStyle=col;ctx.lineWidth=3;ctx.shadowColor=col;ctx.shadowBlur=8;
    ctx.beginPath();pts.forEach(([x,y],i)=>i?ctx.lineTo(x,y):ctx.moveTo(x,y));ctx.stroke();
    ctx.shadowBlur=0;
  });
  requestAnimationFrame(draw);
}
draw();

</script>
</body></html>
)EOF";

static const char SANDFALL_HTML[] = R"EOF(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>SAND FALL · COSMIC-C3</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#000;overflow:hidden;font-family:monospace}
.nav{position:fixed;top:0;left:0;right:0;background:rgba(0,0,0,.7);
  border-bottom:1px solid #ffcc44;padding:5px 10px;display:flex;
  justify-content:space-between;font-size:11px;z-index:9}
.nav a{{color:#ffcc44;text-decoration:none}}
canvas{display:block}
</style>
</head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>SAND FALL</span></div>
<canvas id="c"></canvas>
<script>
const C=document.getElementById('c');
const ctx=C.getContext('2d');
let W,H;
function resize(){W=C.width=innerWidth;H=C.height=innerHeight;}
window.addEventListener('resize',()=>{resize();init&&init();});
resize();

const SC=3;let GW,GH,grid,grid2;
function init(){GW=W/SC|0;GH=H/SC|0;grid=new Uint8Array(GW*GH);grid2=new Uint8Array(GW*GH);}
function idx(x,y){return y*GW+x;}
let t=0;
function update(){
  // spawn sand at top
  for(let i=0;i<4;i++){const x=GW*0.1+Math.random()*GW*0.8|0;if(grid[idx(x,0)]==0)grid[idx(x,0)]=1+Math.floor(Math.random()*4);}
  grid2.fill(0);
  for(let y=GH-2;y>=0;y--){for(let x=0;x<GW;x++){
    const c=grid[idx(x,y)];if(!c)continue;
    if(!grid[idx(x,y+1)]){grid2[idx(x,y+1)]=c;}
    else{const d=Math.random()<0.5?-1:1;
      if(x+d>=0&&x+d<GW&&!grid[idx(x+d,y+1)])grid2[idx(x+d,y+1)]=c;
      else grid2[idx(x,y)]=c;}
  }}
  grid.set(grid2);
  t++;if(t>400){grid.fill(0);t=0;}
}
const COLS3=['#ffdd44','#ffaa22','#ff8800','#cc6600','#ffee88'];
function draw(){
  ctx.fillStyle='#0a0500';ctx.fillRect(0,0,W,H);
  for(let y=0;y<GH;y++)for(let x=0;x<GW;x++){
    const c=grid[idx(x,y)];if(c){ctx.fillStyle=COLS3[c-1];ctx.fillRect(x*SC,y*SC,SC,SC);}
  }
  update();requestAnimationFrame(draw);
}
init();draw();

</script>
</body></html>
)EOF";

static const char ACIDSPIRAL_HTML[] = R"EOF(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>ACID SPIRAL · COSMIC-C3</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#000;overflow:hidden;font-family:monospace}
.nav{position:fixed;top:0;left:0;right:0;background:rgba(0,0,0,.7);
  border-bottom:1px solid #ff00ff;padding:5px 10px;display:flex;
  justify-content:space-between;font-size:11px;z-index:9}
.nav a{{color:#ff00ff;text-decoration:none}}
canvas{display:block}
</style>
</head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>ACID SPIRAL</span></div>
<canvas id="c"></canvas>
<script>
const C=document.getElementById('c');
const ctx=C.getContext('2d');
let W,H;
function resize(){W=C.width=innerWidth;H=C.height=innerHeight;}
window.addEventListener('resize',()=>{resize();init&&init();});
resize();

let t=0;
function draw(){
  t+=0.03;
  ctx.fillStyle='rgba(0,0,0,0.08)';ctx.fillRect(0,0,W,H);
  const cx=W/2,cy=H/2;
  for(let arm=0;arm<5;arm++){
    const armOff=arm*Math.PI*2/5;
    for(let i=0;i<200;i++){
      const theta=i*0.12+t+armOff;
      const r=i*Math.min(W,H)*0.0013;
      const x=cx+Math.cos(theta)*r*Math.min(W,H)*0.45;
      const y=cy+Math.sin(theta)*r*Math.min(W,H)*0.45;
      const hue=(i*2.5+t*60+arm*72)%360;
      const sz=1+Math.sin(i*0.18+t*3)*1.5;
      ctx.fillStyle=`hsla(${hue|0},100%,65%,0.7)`;
      ctx.beginPath();ctx.arc(x,y,sz,0,Math.PI*2);ctx.fill();
    }
  }
  requestAnimationFrame(draw);
}
draw();

</script>
</body></html>
)EOF";

static const char PLASMAGLOBE_HTML[] = R"EOF(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>PLASMA GLOBE · COSMIC-C3</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#000;overflow:hidden;font-family:monospace}
.nav{position:fixed;top:0;left:0;right:0;background:rgba(0,0,0,.7);
  border-bottom:1px solid #ff8800;padding:5px 10px;display:flex;
  justify-content:space-between;font-size:11px;z-index:9}
.nav a{{color:#ff8800;text-decoration:none}}
canvas{display:block}
</style>
</head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>PLASMA GLOBE</span></div>
<canvas id="c"></canvas>
<script>
const C=document.getElementById('c');
const ctx=C.getContext('2d');
let W,H;
function resize(){W=C.width=innerWidth;H=C.height=innerHeight;}
window.addEventListener('resize',()=>{resize();init&&init();});
resize();

const S2=4;let IW2,IH2,ofc2,octx2;
function init(){IW2=W/S2|0;IH2=H/S2|0;ofc2=new OffscreenCanvas(IW2,IH2);octx2=ofc2.getContext('2d');}
// tendrils
const TD=8;const tseeds=Array.from({length:TD},(_,i)=>({a:i*Math.PI*2/TD,spd:0.4+Math.random()*0.6}));
let t=0;
function draw(){
  t+=0.025;
  ctx.fillStyle='rgba(0,0,0,0.25)';ctx.fillRect(0,0,W,H);
  const cx=W/2,cy=H/2,glob=Math.min(W,H)*0.14;
  // tendrils
  tseeds.forEach((td,k)=>{
    td.a+=td.spd*0.015;
    const pts=[];
    for(let s=0;s<40;s++){
      const frac=s/39;
      const r=glob+frac*(Math.min(W,H)*0.38);
      const wob=Math.sin(t*3+k+s*0.4)*0.35;
      const a=td.a+wob*frac;
      pts.push([cx+Math.cos(a)*r,cy+Math.sin(a)*r]);
    }
    const hue=(k*42+t*25)%360;
    ctx.strokeStyle=`hsla(${hue|0},100%,70%,0.6)`;ctx.lineWidth=2;
    ctx.shadowColor=`hsla(${hue|0},100%,70%,1)`;ctx.shadowBlur=10;
    ctx.beginPath();pts.forEach(([x,y],i)=>i?ctx.lineTo(x,y):ctx.moveTo(x,y));ctx.stroke();
    ctx.shadowBlur=0;
  });
  // globe
  const gg=ctx.createRadialGradient(cx,cy,0,cx,cy,glob);
  gg.addColorStop(0,'rgba(255,255,255,0.9)');gg.addColorStop(0.3,`rgba(180,120,255,0.6)`);gg.addColorStop(1,'rgba(80,40,120,0.2)');
  ctx.fillStyle=gg;ctx.beginPath();ctx.arc(cx,cy,glob,0,Math.PI*2);ctx.fill();
  ctx.strokeStyle='rgba(200,180,255,0.5)';ctx.lineWidth=2;ctx.beginPath();ctx.arc(cx,cy,glob,0,Math.PI*2);ctx.stroke();
  requestAnimationFrame(draw);
}
init();draw();

</script>
</body></html>
)EOF";

static const char WARPGRID_HTML[] = R"EOF(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>WARP GRID · COSMIC-C3</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#000;overflow:hidden;font-family:monospace}
.nav{position:fixed;top:0;left:0;right:0;background:rgba(0,0,0,.7);
  border-bottom:1px solid #00ccff;padding:5px 10px;display:flex;
  justify-content:space-between;font-size:11px;z-index:9}
.nav a{{color:#00ccff;text-decoration:none}}
canvas{display:block}
</style>
</head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>WARP GRID</span></div>
<canvas id="c"></canvas>
<script>
const C=document.getElementById('c');
const ctx=C.getContext('2d');
let W,H;
function resize(){W=C.width=innerWidth;H=C.height=innerHeight;}
window.addEventListener('resize',()=>{resize();init&&init();});
resize();

let t=0;
const GX=24,GY=18;
function draw(){
  t+=0.025;
  ctx.fillStyle='rgba(0,0,10,0.3)';ctx.fillRect(0,0,W,H);
  function pt(gx,gy){
    const nx=gx/GX*2-1,ny=gy/GY*2-1;
    const d=Math.sqrt(nx*nx+ny*ny);
    const wave=Math.sin(d*4-t*3)*0.15;
    const twist=Math.sin(t*0.8+d)*0.2;
    const sx=nx+wave*Math.cos(t+gy)+twist*ny;
    const sy=ny+wave*Math.sin(t+gx)-twist*nx;
    return[(sx*0.45+0.5)*W,(sy*0.45+0.5)*H];
  }
  for(let gy=0;gy<=GY;gy++){
    const hue=(gy/GY*120+t*20)%360;
    ctx.strokeStyle=`hsla(${hue|0},100%,60%,0.65)`;ctx.lineWidth=1;
    ctx.beginPath();
    for(let gx=0;gx<=GX;gx++){const[x,y]=pt(gx,gy);gx?ctx.lineTo(x,y):ctx.moveTo(x,y);}ctx.stroke();
  }
  for(let gx=0;gx<=GX;gx++){
    const hue=(gx/GX*120+180+t*20)%360;
    ctx.strokeStyle=`hsla(${hue|0},100%,60%,0.65)`;ctx.lineWidth=1;
    ctx.beginPath();
    for(let gy=0;gy<=GY;gy++){const[x,y]=pt(gx,gy);gy?ctx.lineTo(x,y):ctx.moveTo(x,y);}ctx.stroke();
  }
  requestAnimationFrame(draw);
}
draw();

</script>
</body></html>
)EOF";

static const char NEBULA_HTML[] = R"EOF(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>NEBULA · COSMIC-C3</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{background:#000;overflow:hidden;font-family:monospace}
.nav{position:fixed;top:0;left:0;right:0;background:rgba(0,0,0,.7);
  border-bottom:1px solid #ff44ff;padding:5px 10px;display:flex;
  justify-content:space-between;font-size:11px;z-index:9}
.nav a{{color:#ff44ff;text-decoration:none}}
canvas{display:block}
</style>
</head><body>
<div class="nav"><a href="/">&#x2B21; MODES</a><span>NEBULA</span></div>
<canvas id="c"></canvas>
<script>
const C=document.getElementById('c');
const ctx=C.getContext('2d');
let W,H;
function resize(){W=C.width=innerWidth;H=C.height=innerHeight;}
window.addEventListener('resize',()=>{resize();init&&init();});
resize();

const S3=3;let IW3,IH3,ofc3,octx3;
function init(){IW3=W/S3|0;IH3=H/S3|0;ofc3=new OffscreenCanvas(IW3,IH3);octx3=ofc3.getContext('2d');}
const NS=80;const nx_=new Float32Array(NS),ny_=new Float32Array(NS),nbr=new Float32Array(NS);
for(let i=0;i<NS;i++){nx_[i]=Math.random();ny_[i]=Math.random();nbr[i]=60+Math.random()*195;}
let t=0;
function draw(){
  t+=0.018;
  const id=octx3.createImageData(IW3,IH3);const d=id.data;
  for(let y=0;y<IH3;y++)for(let x=0;x<IW3;x++){
    const fx=x/IW3,fy=y/IH3;
    let r=0,g=0,b=0;
    for(let s=0;s<NS;s++){
      const dx=(fx-nx_[s])*2.5,dy=(fy-ny_[s])*2;
      const dist=Math.sqrt(dx*dx+dy*dy)+0.001;
      const hue=(s*137.5+t*20)%360;
      const c=Math.max(0,nbr[s]/(dist*dist*400)-0.01);
      r+=c*Math.sin(hue/57.3)*127;g+=c*Math.sin((hue+120)/57.3)*127;b+=c*Math.sin((hue+240)/57.3)*127;
    }
    const idx2=(y*IW3+x)*4;
    d[idx2]=Math.min(255,r+30)|0;d[idx2+1]=Math.min(255,g+10)|0;d[idx2+2]=Math.min(255,b+40)|0;d[idx2+3]=255;
  }
  octx3.putImageData(id,0,0);
  ctx.drawImage(ofc3,0,0,W,H);
  // foreground stars
  for(let i=0;i<NS;i++){
    const sx=nx_[i]*W,sy=ny_[i]*H;
    const tw=0.5+Math.sin(t*2+i)*0.5;
    ctx.fillStyle=`rgba(255,255,255,${tw*0.8})`;
    ctx.beginPath();ctx.arc(sx,sy,1+tw,0,Math.PI*2);ctx.fill();
  }
  requestAnimationFrame(draw);
}
init();draw();

</script>
</body></html>
)EOF";

void handleSnake()      { server.send(200, "text/html", SNAKE_HTML);      }
void handleFireworks() { server.send(200, "text/html", FIREWORKS_HTML); }
void handleCoral() { server.send(200, "text/html", CORAL_HTML); }
void handleCwaves() { server.send(200, "text/html", CWAVES_HTML); }
void handleDeepstars() { server.send(200, "text/html", DEEPSTARS_HTML); }
void handleFlowfield() { server.send(200, "text/html", FLOWFIELD_HTML); }
void handleMetaballs() { server.send(200, "text/html", METABALLS_HTML); }
void handleGoop() { server.send(200, "text/html", GOOP_HTML); }
void handleWormhole() { server.send(200, "text/html", WORMHOLE_HTML); }
void handleCrystal() { server.send(200, "text/html", CRYSTAL_HTML); }
void handleLightning() { server.send(200, "text/html", LIGHTNING_HTML); }
void handleBounceballs() { server.send(200, "text/html", BOUNCEBALLS_HTML); }
void handleNeonrain() { server.send(200, "text/html", NEONRAIN_HTML); }
void handleDna() { server.send(200, "text/html", DNA_HTML); }
void handleSandfall() { server.send(200, "text/html", SANDFALL_HTML); }
void handleAcidspiral() { server.send(200, "text/html", ACIDSPIRAL_HTML); }
void handlePlasmaglobe() { server.send(200, "text/html", PLASMAGLOBE_HTML); }
void handleWarpgrid() { server.send(200, "text/html", WARPGRID_HTML); }
void handleNebula() { server.send(200, "text/html", NEBULA_HTML); }


// Redirect everything unknown → captive portal index
void handleRedirect() {
    server.sendHeader("Location", String("http://") + PORTAL_IP + "/", true);
    server.send(302, "text/plain", "");
}

// ── Setup ─────────────────────────────────────────────────────────────────────

void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);  // active LOW = on during init

    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID);
    delay(200);

    IPAddress apIP = WiFi.softAPIP();
    dnsServer.start(DNS_PORT, "*", apIP);

    server.on("/",            HTTP_GET, handleIndex);
    server.on("/mandala",     HTTP_GET, handleMandala);
    server.on("/plasma",      HTTP_GET, handlePlasma);
    server.on("/fractal",     HTTP_GET, handleFractal);
    server.on("/matrix",    HTTP_GET, handleMatrix);
    server.on("/cyber",     HTTP_GET, handleCyber);
    server.on("/binary",    HTTP_GET, handleBinary);
    server.on("/starfield", HTTP_GET, handleStarfield);
    server.on("/particles", HTTP_GET, handleParticles);
    server.on("/tunnel",    HTTP_GET, handleTunnel);
    server.on("/mfire",       HTTP_GET, handleMfire);
    server.on("/mice",        HTTP_GET, handleMice);
    server.on("/mstorm",      HTTP_GET, handleMstorm);
    server.on("/mblood",      HTTP_GET, handleMblood);
    server.on("/mgold",       HTTP_GET, handleMgold);
    server.on("/mvoid",       HTTP_GET, handleMvoid);
    server.on("/mphantom",    HTTP_GET, handleMphantom);
    server.on("/mripple",     HTTP_GET, handleMripple);
    server.on("/mglitch",     HTTP_GET, handleMglitch);
    server.on("/hopalong",    HTTP_GET, handleHopalong);
    server.on("/interference",HTTP_GET, handleInterference);
    server.on("/voronoi",     HTTP_GET, handleVoronoi);
    server.on("/strange",     HTTP_GET, handleStrange);
    server.on("/lissajous",   HTTP_GET, handleLissajous);
    server.on("/sierpinski",  HTTP_GET, handleSierpinski);
    server.on("/spirograph",  HTTP_GET, handleSpirograph);
    server.on("/barnsley",    HTTP_GET, handleBarnsley);
    server.on("/campfire",    HTTP_GET, handleCampfire);
    server.on("/raindrops",   HTTP_GET, handleRaindrops);
    server.on("/gameoflife",  HTTP_GET, handleGameoflife);
    server.on("/aurora",      HTTP_GET, handleAurora);
    server.on("/kaleidoscope",HTTP_GET, handleKaleidoscope);
    server.on("/dragon",      HTTP_GET, handleDragon);
    server.on("/lava2",       HTTP_GET, handleLava2);
    server.on("/noise",       HTTP_GET, handleNoise);
    server.on("/snake",       HTTP_GET, handleSnake);
    server.on("/fireworks", HTTP_GET, handleFireworks);
    server.on("/coral", HTTP_GET, handleCoral);
    server.on("/cwaves", HTTP_GET, handleCwaves);
    server.on("/deepstars", HTTP_GET, handleDeepstars);
    server.on("/flowfield", HTTP_GET, handleFlowfield);
    server.on("/metaballs", HTTP_GET, handleMetaballs);
    server.on("/goop", HTTP_GET, handleGoop);
    server.on("/wormhole", HTTP_GET, handleWormhole);
    server.on("/crystal", HTTP_GET, handleCrystal);
    server.on("/lightning", HTTP_GET, handleLightning);
    server.on("/bounceballs", HTTP_GET, handleBounceballs);
    server.on("/neonrain", HTTP_GET, handleNeonrain);
    server.on("/dna", HTTP_GET, handleDna);
    server.on("/sandfall", HTTP_GET, handleSandfall);
    server.on("/acidspiral", HTTP_GET, handleAcidspiral);
    server.on("/plasmaglobe", HTTP_GET, handlePlasmaglobe);
    server.on("/warpgrid", HTTP_GET, handleWarpgrid);
    server.on("/nebula", HTTP_GET, handleNebula);

    server.on("/favicon.ico", HTTP_GET, []() { server.send(404, "text/plain", ""); });
    server.onNotFound(handleRedirect);
    server.begin();
}

// ── Loop ──────────────────────────────────────────────────────────────────────

void loop() {
    dnsServer.processNextRequest();
    server.handleClient();

    // Rapid blink (80/80ms) when a client is on the portal, heartbeat (200/1800ms) when idle
    static unsigned long lastBlink = 0;
    static bool ledOn = false;
    unsigned long now = millis();
    bool active = WiFi.softAPgetStationNum() > 0;
    if (now - lastBlink > (ledOn ? (active ? 80UL : 200UL) : (active ? 80UL : 1800UL))) {
        ledOn = !ledOn;
        digitalWrite(LED_PIN, ledOn ? LOW : HIGH);
        lastBlink = now;
    }
}

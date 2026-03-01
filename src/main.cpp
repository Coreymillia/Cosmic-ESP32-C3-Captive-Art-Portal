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
justify-content:center;font-family:'Courier New',monospace;color:#fff;padding:24px 16px}
.glw{position:fixed;border-radius:50%;filter:blur(90px);z-index:-1;pointer-events:none}
.g1{width:350px;height:350px;top:-120px;left:-120px;background:rgba(131,56,236,.3)}
.g2{width:300px;height:300px;bottom:-100px;right:-100px;background:rgba(6,255,208,.18)}
h1{font-size:clamp(1.2rem,5vw,1.8rem);letter-spacing:7px;text-align:center;margin-bottom:4px;
background:linear-gradient(90deg,#ff006e,#ff6b00,#ffd700,#06ffd0,#3a86ff,#8338ec,#ff006e);
-webkit-background-clip:text;-webkit-text-fill-color:transparent;background-clip:text;
filter:drop-shadow(0 0 10px rgba(131,56,236,.7))}
.sub{font-size:.55rem;letter-spacing:8px;color:rgba(0,255,209,.5);margin-bottom:24px;text-align:center}
.grid{display:grid;grid-template-columns:repeat(auto-fill,minmax(200px,1fr));
gap:10px;width:min(450px,92vw)}
.card{display:block;padding:13px 16px;border-radius:11px;text-decoration:none;
background:rgba(10,0,40,.6);border:1px solid;transition:transform .15s,box-shadow .15s}
.card:hover,.card:active{transform:scale(1.03)}
.c1{border-color:rgba(131,56,236,.6);box-shadow:0 0 16px rgba(131,56,236,.12)}
.c1:hover{box-shadow:0 0 28px rgba(131,56,236,.4)}
.c2{border-color:rgba(255,107,0,.5);box-shadow:0 0 16px rgba(255,107,0,.08)}
.c2:hover{box-shadow:0 0 28px rgba(255,107,0,.35)}
.c3{border-color:rgba(6,255,208,.4);box-shadow:0 0 16px rgba(6,255,208,.08)}
.c3:hover{box-shadow:0 0 28px rgba(6,255,208,.3)}
.c4{border-color:rgba(0,255,65,.5);box-shadow:0 0 16px rgba(0,255,65,.08)}
.c4:hover{box-shadow:0 0 28px rgba(0,255,65,.4)}
.c5{border-color:rgba(255,0,170,.5);box-shadow:0 0 16px rgba(255,0,170,.08)}
.c5:hover{box-shadow:0 0 28px rgba(255,0,170,.4)}
.c6{border-color:rgba(0,136,255,.5);box-shadow:0 0 16px rgba(0,136,255,.08)}
.c6:hover{box-shadow:0 0 28px rgba(0,136,255,.4)}
.c7{border-color:rgba(180,180,255,.4);box-shadow:0 0 16px rgba(180,180,255,.08)}
.c7:hover{box-shadow:0 0 28px rgba(180,180,255,.35)}
.c8{border-color:rgba(255,215,0,.4);box-shadow:0 0 16px rgba(255,215,0,.08)}
.c8:hover{box-shadow:0 0 28px rgba(255,215,0,.35)}
.c9{border-color:rgba(255,80,0,.5);box-shadow:0 0 16px rgba(255,80,0,.08)}
.c9:hover{box-shadow:0 0 28px rgba(255,80,0,.4)}
.icon{font-size:1.5rem;margin-bottom:4px;display:block}
.name{font-size:.8rem;letter-spacing:4px;font-weight:bold;display:block;margin-bottom:3px}
.desc{font-size:.47rem;letter-spacing:1px;opacity:.5;display:block}
.n1{color:#c77dff}.n2{color:#ff9500}.n3{color:#06ffd0}
.n4{color:#00ff41}.n5{color:#ff00aa}.n6{color:#44aaff}
.n7{color:#b4b4ff}.n8{color:#ffd700}.n9{color:#ff6622}
footer{margin-top:22px;font-size:.45rem;letter-spacing:4px;color:rgba(255,255,255,.1);text-align:center}
</style></head><body>
<div class="glw g1"></div><div class="glw g2"></div>
<h1>COSMIC-C3</h1>
<p class="sub">SELECT TRANSMISSION MODE</p>
<div class="grid">
  <a class="card c1" href="/mandala">
    <span class="icon">&#x2B21;</span>
    <span class="name n1">MANDALA</span>
    <span class="desc">SACRED GEOMETRY &middot; CONIC GRADIENTS &middot; RAINBOW RINGS</span>
  </a>
  <a class="card c2" href="/plasma">
    <span class="icon">&#x25C9;</span>
    <span class="name n2">PLASMA</span>
    <span class="desc">LAVA BLOBS &middot; CSS ANIMATED &middot; SCREEN BLEND</span>
  </a>
  <a class="card c3" href="/fractal">
    <span class="icon">&#x2726;</span>
    <span class="name n3">FRACTAL</span>
    <span class="desc">JULIA SET &middot; INFINITE DETAIL &middot; BROWSER RENDERED</span>
  </a>
  <a class="card c4" href="/matrix">
    <span class="icon">&#x2328;</span>
    <span class="name n4">MATRIX</span>
    <span class="desc">DIGITAL RAIN &middot; FALLING KANA &middot; GREEN PHOSPHOR</span>
  </a>
  <a class="card c5" href="/cyber">
    <span class="icon">&#x26A1;</span>
    <span class="name n5">CYBER RAIN</span>
    <span class="desc">NEON MATRIX &middot; MULTICOLOR COLUMNS &middot; GLOW</span>
  </a>
  <a class="card c6" href="/binary">
    <span class="icon">&#x2B1B;</span>
    <span class="name n6">BINARY</span>
    <span class="desc">ZERO ONE &middot; ELECTRIC BLUE &middot; MACHINE CODE</span>
  </a>
  <a class="card c7" href="/starfield">
    <span class="icon">&#x2605;</span>
    <span class="name n7">STARFIELD</span>
    <span class="desc">WARP SPEED &middot; 3D PROJECTION &middot; HYPERSPACE JUMP</span>
  </a>
  <a class="card c8" href="/particles">
    <span class="icon">&#x22C6;</span>
    <span class="name n8">PARTICLES</span>
    <span class="desc">CONSTELLATION &middot; FLOATING NODES &middot; CONNECTED MESH</span>
  </a>
  <a class="card c9" href="/tunnel">
    <span class="icon">&#x25CE;</span>
    <span class="name n9">TUNNEL</span>
    <span class="desc">VORTEX &middot; ROTATING RINGS &middot; INFINITE DEPTH</span>
  </a>
</div>
<footer>esp32-c3 super mini &middot; wifi ap &middot; 192.168.4.1 &middot; 9 modes</footer>
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

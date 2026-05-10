@'
const mqtt = require('mqtt');
const CONFIG = { MQTT_BROKER: 'mqtt://localhost:1883', MS_PER_CELL: 1500 };
const ROWS = ['A','B','C','D'];
const COLS = ['1','2','3','4'];
function zoneToGrid(id) { return { col: COLS.indexOf(id[1]), row: ROWS.indexOf(id[0]) }; }
function gridToZone(col, row) { return ROWS[row] + COLS[col]; }
let roverState = { zone:'A1', col:0, row:0, heading:90, moving:false };
let stepQueue = [];
let stepTimer = null;
const client = mqtt.connect(CONFIG.MQTT_BROKER);
client.on('connect', () => { console.log('=== RoverNav Backend READY ==='); client.subscribe('rover/travel/target'); client.subscribe('rover/reset'); });
client.on('message', (topic, message) => { const raw = message.toString(); console.log(`[RECV] ${topic}: ${raw}`); if (topic === 'rover/travel/target') handleTarget(raw); if (topic === 'rover/reset') handleReset(); });
function handleTarget(raw) { const { zone: targetZone } = JSON.parse(raw); if (roverState.moving) { publishStatus('BUSY', 'busy'); return; } const { col: toCol, row: toRow } = zoneToGrid(targetZone); const waypoints = []; let c = roverState.col; let r = roverState.row; const sc = toCol > c ? 1 : toCol < c ? -1 : 0; const sr = toRow > r ? 1 : toRow < r ? -1 : 0; while (c !== toCol) { c += sc; waypoints.push({ col:c, row:roverState.row }); } while (r !== toRow) { r += sr; waypoints.push({ col:toCol, row:r }); } if (waypoints.length === 0) { publishStatus(`Already at ${targetZone}`, 'idle'); return; } console.log(`[PATH] ${roverState.zone} to ${targetZone}`); stepQueue = waypoints; roverState.moving = true; publishStatus(`Navigating to ${targetZone}`, 'moving'); executeNextStep(targetZone); }
function executeNextStep(finalZone) { if (stepQueue.length === 0) { roverState.moving = false; publishStatus(`Arrived at ${finalZone}`, 'idle'); return; } const next = stepQueue.shift(); const dx = next.col - roverState.col; const dy = next.row - roverState.row; const requiredHeading = Math.atan2(dy, dx) * 180 / Math.PI; let turn = requiredHeading - roverState.heading; while (turn > 180) turn -= 360; while (turn < -180) turn += 360; const dest = gridToZone(next.col, next.row); const cmd = { target_yaw: Math.round(turn), drive_ms: CONFIG.MS_PER_CELL, to_zone: dest }; console.log(`[STEP] to ${dest} turn=${cmd.target_yaw} drive=${cmd.drive_ms}ms`); client.publish('rover/cmd', JSON.stringify(cmd), { qos:1 }); roverState.heading = requiredHeading; roverState.col = next.col; roverState.row = next.row; roverState.zone = dest; publishStatus(`Moving to ${dest}`, 'moving', dest); stepTimer = setTimeout(() => executeNextStep(finalZone), CONFIG.MS_PER_CELL + 200); }
function handleReset() { if (stepTimer) clearTimeout(stepTimer); stepQueue = []; roverState = { zone:'A1', col:0, row:0, heading:90, moving:false }; publishStatus('Reset to A1', 'idle', 'A1'); }
function publishStatus(message, state='idle', zone=roverState.zone) { const payload = JSON.stringify({ rover_zone:zone, state, message, heading:Math.round(roverState.heading), timestamp:Date.now() }); console.log(`[STATUS] ${payload}`); client.publish('rover/status', payload); }
console.log('Connecting to Mosquitto broker...');
'@ | Out-File -FilePath server.js -Encoding utf8
/**
 * test_send.js
 * ─────────────────────────────────────────────────────────────
 * Quick test script — simulates what the frontend does when a
 * user clicks a zone in Travel Mode.
 *
 * Run WHILE server.js is already running:
 *   node test_send.js
 *
 * It sends zone "C3" as the travel target and then listens for
 * the backend to publish commands and status messages.
 * ─────────────────────────────────────────────────────────────
 */

const mqtt = require('mqtt');

const TARGET_ZONE = 'C3'; // change this to test different destinations

const client = mqtt.connect('mqtt://localhost:1883', {
  clientId: 'rovernav-tester',
});

client.on('connect', () => {
  console.log('[TEST] Connected to broker');

  // Listen for commands going to the ESP32
  client.subscribe('rover/cmd', () => {
    console.log('[TEST] Subscribed to rover/cmd  (watching what goes to ESP32)');
  });

  // Listen for status updates going to the frontend
  client.subscribe('rover/status', () => {
    console.log('[TEST] Subscribed to rover/status (watching frontend updates)');
  });

  // Give subscriptions a moment to register, then send the test target
  setTimeout(() => {
    const payload = JSON.stringify({ zone: TARGET_ZONE });
    console.log(`\n[TEST] Sending travel target: ${payload}`);
    client.publish('rover/travel/target', payload);
  }, 500);
});

client.on('message', (topic, message) => {
  const raw = message.toString();
  if (topic === 'rover/cmd') {
    const cmd = JSON.parse(raw);
    console.log(
      `  [ESP32 CMD] → zone=${cmd.to_zone}  turn=${cmd.target_yaw > 0 ? '+' : ''}${cmd.target_yaw}°  drive=${cmd.drive_ms}ms`
    );
  }
  if (topic === 'rover/status') {
    const s = JSON.parse(raw);
    console.log(`  [STATUS] ${s.state.toUpperCase().padEnd(6)} ${s.rover_zone}  "${s.message}"`);
    // Stop listening once rover arrives
    if (s.state === 'idle' && s.message && s.message.includes('Arrived')) {
      console.log('\n[TEST] Done — closing.');
      client.end();
      process.exit(0);
    }
  }
});

// Timeout safety
setTimeout(() => {
  console.log('[TEST] Timeout — closing.');
  client.end();
  process.exit(0);
}, 30000);

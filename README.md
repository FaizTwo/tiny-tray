# Tiny tray daemon

## Command

> Accept 2 kinds of command:

```
{"tip":"Lark","icon":"icon.ico","items":[{"text":"Open Lark  (Ctrl+Shift+L)","id":"1"}]}
```

> or

```
exit
```

> As for json, every properties is required(empty string acceptable), and only 1 line per json string.

## Simple usage

```
const { spawn } = require('child_process');
const ps = spawn('tray.exe');
ps.stdin.write(`{"tip":"Lark","icon":"icon.ico","items":[{"text":"Open Lark  (Ctrl+Shift+L)","id":"1"}]}\n`);
setTimeout(() => ps.stdin.write(`exit\n`), 5000);
```

rm -f /tmp/ttyReadPing /tmp/ttyWritePing /tmp/ttyReadGPS /tmp/ttyWriteGPS && \
socat -d -d PTY,raw,echo=0,link=/tmp/ttyReadPing,mode=666 PTY,raw,echo=0,link=/tmp/ttyWritePing,mode=666 &

socat -d -d PTY,raw,echo=0,link=/tmp/ttyReadGPS,mode=666 PTY,raw,echo=0,link=/tmp/ttyWriteGPS,mode=666 &

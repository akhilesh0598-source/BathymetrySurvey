#!/bin/bash

while true; do
  # Generate a fake GPGGA NMEA sentence with random lat/lon
  LAT=$(awk -v min=0 -v max=90 'BEGIN{srand(); print int(min+rand()*(max-min+1))}')
  LON=$(awk -v min=0 -v max=180 'BEGIN{srand(); print int(min+rand()*(max-min+1))}')
  LAT_DIR=$([ $((RANDOM % 2)) -eq 0 ] && echo "N" || echo "S")
  LON_DIR=$([ $((RANDOM % 2)) -eq 0 ] && echo "E" || echo "W")

  # Format latitude and longitude (degrees + decimal)
  LAT_STR=$(printf "%02d%05.2f" $LAT $(awk 'BEGIN{srand(); print rand()*60}'))
  LON_STR=$(printf "%03d%05.2f" $LON $(awk 'BEGIN{srand(); print rand()*60}'))

  # Build a simple GPGGA sentence (not fully accurate checksum)
  SENTENCE="\$GPGGA,123519,${LAT_STR},${LAT_DIR},${LON_STR},${LON_DIR},1,08,0.9,545.4,M,46.9,M,,*47"

  # Send it to your mock GPS writer device
  echo "$SENTENCE" > /tmp/GPS_Writer

  # Wait 1 second before sending next
  sleep 1
done

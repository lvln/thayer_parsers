#!/bin/bash

# Create tests for each message type.
./tv 0
echo "HEARTBEAT"
./tv 1
echo "SYS_STATUS"
./tv 4
echo "PING"
./tv 8
echo "LINK_NODE_STATUS"
./tv 22
echo "PARAM_VALUE"
./tv 24
echo "GPS_RAW_INT"
./tv 29
echo "SCALED_PRESSURE"
./tv 30
echo "ATTITUDE"
./tv 31
echo "ATTITUDE_QUATERNION"
./tv 32
echo "LOCAL_POSITION_NED"
./tv 33
echo "GLOBAL_POSITION_INT"
./tv 42
echo "MISSION_CURRENT"
./tv 46
echo "MISSION_ITEM_REACHED"
./tv 65
echo "RC_CHANNELS"
./tv 74
echo "VFR_HUD"
./tv 77
echo "COMMAND_ACK"
./tv 83
echo "ATTITUDE_TARGET"
./tv 85
echo "POSITION_TARGET_LOCAL_NED"
./tv 87
echo "POSITION_TARGET_GLOBAL_INT"
./tv 109
echo "RADIO_STATUS"
./tv 141
echo "ALTITUDE"
./tv 147
echo "BATTERY_STATUS"
./tv 230
echo "ESTIMATOR_STATUS"
./tv 241
echo "VIBRATION"
./tv 242
echo "HOME_POSITION"
./tv 245
echo "EXTENDED_SYS_STATE"
./tv 253
echo "STATUSTEXT"
./tv 340
echo "UTM_GLOBAL_POSITION"
./tv 380
echo "TIME_ESTIMATE_TO_TARGET"
./tv 410
echo "EVENT"
./tv 411
echo "CURRENT_EVENT_SEQUENCE"
./tv 12901
echo "OPEN_DRONE_ID_LOCATION"
./tv 12904
echo "OPEN_DRONE_ID_SYSTEM"

# Create tests for full drone flights.
if [ -e ../mavlink_source_files/run1.mav ]; then
		cp ../mavlink_source_files/run1.mav ./pass.1000 > /dev/null
fi

if [ -e ../mavlink_source_files/run2.mav ]; then
		cp ../mavlink_source_files/run2.mav ./pass.1001 > /dev/null
fi

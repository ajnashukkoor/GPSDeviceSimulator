# GPSDeviceSimulator
A Simple device simulator and Adaptor for Concox GPS Tracker
which does the following:

Device Simulator

    1. Sends login packet always (78 78 11 01 03 51 60 80 80 77 92 88 22 03 32 01 01 AA 53 36 0D 0A)
    2. Sends heartbeat packet on login packet response (78 78 0B 23 C0 01 22 04 00 01 00 08 18 72 0D 0A)
        1. Continues to send this packet on heartbeat packet response
    3. Sends GPS location packet on login packet response (78 78 22 22 0F 0C 1D 02 33 05 C9 02 7A C8 18 0C 46 58 60 00 14 00 01 CC 00 28 7D 00 1F 71 00 00 01 00 08 20 86 0D 0A)
        1. Continues to send this packet on login packet response
        
Device Adapter

    1. Receives the login packet
    2. Responds to the login packet
    3. Receives the heartbeat packet
    4. Responds to the heartbeat packet
    5. Receives the GPS location packet
    6. Responds to GPS location packet
    7. Saves GPS location packet in a file storage

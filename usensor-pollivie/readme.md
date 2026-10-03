# Programme

compile the project
```bash
# Compilation to have the exec
make 

# Clean to keep the exec
make clean

# Clean all
make fclean
```

```bash
stdin | ./prog
```

# Exercice décomposition

En embarquée et en architecture on ne sait pas forcement sur quel type d'architecture on es donc on vas devoir avoir un typage strict avec int(bits)_t

Header:

| Offset | Size | Meaning |
| --- | ---: | --- |
| 0 | 8 | ASCII `USENS001` |
| 8 | 2 | Record size, 32 |
| 10 | 2 | Version, 1 |
| 12 | 4 | FNV-1a 32-bit checksum of bytes 0–11 |

Total header = 16 octets

Record:

| Offset | Size | Meaning |
| --- | ---: | --- |
| 0 | 8 | Capture timestamp in nanoseconds since trace start |
| 8 | 4 | Per-sensor sequence number, starting at 0 |
| 12 | 1 | Sensor ID |
| 13 | 1 | Flags; bit 0 means valid |
| 14 | 2 | Reserved, zero |
| 16 | 12 | Three signed 32-bit payload values: x, y, z |
| 28 | 4 | FNV-1a checksum of bytes 0–27 |

Total Record = 32 octets

FNV-1a starts at **`2166136261`**
for each byte, **`hash = (hash XOR byte) * 16777619`**

```c
uint32_t	hash = 2166136261U;

while (/*For each bytes of data*/) {
	hash ^= data[i];
	hash *= 16777619U;
}
```

sensor type :

| ID | Sensor | Typical rate | Payload |
| ---: | --- | ---: | --- |
| 1 | Camera | 30 Hz | x = frame number; y, z unused |
| 2 | IMU | 200 Hz | yaw, pitch, roll in centidegrees |
| 3 | GPS | 5 Hz | latitude and longitude in degrees × 10⁷; altitude in millimeters |
| 4 | Temperature | 1 Hz | x = millidegrees Celsius |
| 5 | Button | Event driven | x = 1 means trigger |

data format :

```json
{"event_id":0,"timestamp_ns":2000000000,"camera":{"timestamp_ns":2000000000,"frame":60},"imu":{"timestamp_ns":2000000000,"yaw_cd":400,"pitch_cd":0,"roll_cd":0},"gps":{"timestamp_ns":2000000000,"lat_e7":488566000,"lon_e7":23522000,"alt_mm":35000},"imu_window":[{"timestamp_ns":1900000000,"yaw_cd":380,"pitch_cd":0,"roll_cd":0}]}
```

```
You may assume a maximum of 400 records per second and at most one button event per second.
```

## Data treatment

Before treatment we need to decode the data with our decode fnv-1a

After that we check if the checksum match with the checksum we received

```
sensor :
fnv-1a() == | 12 | 4 | FNV-1a 32-bit checksum of bytes 0–11 | 
or
record :
fnv-1a() == | 28 | 4 | FNV-1a checksum of bytes 0–27 |
```

In first place we need to reconstituate the data, with little endian for each integers like

## Capture data

when we read the data we transfer into a thread to check length and checksum to put in a share structure information and compare if it's the most recent or not ? 

or we check length of transmission checksum and we transfer in different thread to check the data if its the most recent or not?

## Architecture read & data treatment

Producteur-Consommateur (Producer-Consumer) basé sur une file d'attente thread-safe (Thread-safe Queue)

Thread Producer (read and push data in list)
Queue of data
Consumer (check data and treat data to produce something)
with actual et old data to switch if we met an camera ?
Thread write data ?

```
Main thread program
1 producer
1 consummer

or

Main thread programme
x thread for producer
y thread for consumer
```
?

Producer : read binary input 



## Source
Code Vault 	: Probleme Producer-Consumer thread
Searching	: Architecture read-treatment data multi-thread
https://bytebytego.com/guides/top-6-multithreading-design-patterns-you-must-know/
Man pthread	: http://manpagesfr.free.fr/man/man7/pthreads.7.html
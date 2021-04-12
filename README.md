# eVTOL_Simulation



## Introduce

A small simulation program. Given the parameters of planes produced by 5 companies, e.g., cruise speed, battery capacity, time to charge, energy use at cruise and probability of fault per hour. 20 planes from this 5 company take off at the same time. After the power is exhausted, these aircraft will successively go to the charging station to recharge. The charging station has 3 chargers, and these aircraft will take off after they are fully charged. Assume that the flight speed is always constant. Use three minutes in simulation program to simulate three hours in practice.



## How to run

- BUILD 

`make`

- RUN 

Single thread: `./sc`

Multi Thread:  `./mc`

Lock-less_MultiThread: `./pc`

## plane's state

Every frame: plane-class

- FLY: When the power is used up, if the charging pile is free, it will enter the charging state, if there is no free charging pile, it will enter the waiting queue
- CHARGING: When the battery is fully charged, it enters the flying state

Every frame: ChargingStation-class

- If there is an idle charging pile, the queued aircraft will be ejected from the queue to the charging pile for charging.

<img src="pic/plane_state.png" alt="plane_state" width="500">






## Solution
Solution1: Single Thread

Solution2: Multi Thread

- Use mutex lock the waiting queue
- Use semaphore tell plane the frame +1

Soltuion3: Lock-less_MultiThread

- Use  Non-Blocking Linked-Lists[1] instead of locked queues
- Use Hazard Pointer[2] to implement garbage collection (gc) in parallel computing



## Makefile

The .o file will be placed under the obj folder, and the executable file will be placed under the bin folder. If the corresponding folder does not exist, a new folder will be created.




## Unit test

Calculate whether the number of flying frames + the number of waiting frames + the number of charging frames is equal to the total number of frames.



## read data

- Support cvs (Use commas to separate data)
- Support txt (Use spaces to separate data)



## Result

#### Single Thread:

```
2021-03 09:41:13
2021-03 09:44:17
183605ms 
numPlane = 6 avgtimeInFly = 1.847hour avgtimeInCharging = 0.481hour avgtimeInWait = 0.672hour totalDistanceTraveled = 5319.333 maxNumOfFault = 12
numPlane = 6 avgtimeInFly = 1.444hour avgtimeInCharging = 0.233hour avgtimeInWait = 1.323hour totalDistanceTraveled = 4331.528 maxNumOfFault = 18
numPlane = 2 avgtimeInFly = 1.250hour avgtimeInCharging = 0.800hour avgtimeInWait = 0.950hour totalDistanceTraveled = 1200.000 maxNumOfFault = 6
numPlane = 2 avgtimeInFly = 1.667hour avgtimeInCharging = 0.000hour avgtimeInWait = 1.333hour totalDistanceTraveled = 600.000 maxNumOfFault = 4
numPlane = 4 avgtimeInFly = 1.724hour avgtimeInCharging = 0.300hour avgtimeInWait = 0.976hour totalDistanceTraveled = 413.733 maxNumOfFault = 4
company_frame_actual: 64800 == company_frame_expected: 64800 miss_frame = 0
company_frame_actual: 64800 == company_frame_expected: 64800 miss_frame = 0
company_frame_actual: 21600 == company_frame_expected: 21600 miss_frame = 0
company_frame_actual: 21600 == company_frame_expected: 21600 miss_frame = 0
company_frame_actual: 43200 == company_frame_expected: 43200 miss_frame = 0
total_miss_frame = 0
```

#### Multi Thread:

```
2021-03 09:36:27
2021-03 09:39:37
189785ms 
numPlane = 6 avgtimeInFly = 1.847hour avgtimeInCharging = 0.481hour avgtimeInWait = 0.672hour totalDistanceTraveled = 5319.333 maxNumOfFault = 12
numPlane = 6 avgtimeInFly = 1.444hour avgtimeInCharging = 0.233hour avgtimeInWait = 1.323hour totalDistanceTraveled = 4331.528 maxNumOfFault = 18
numPlane = 2 avgtimeInFly = 1.250hour avgtimeInCharging = 0.800hour avgtimeInWait = 0.950hour totalDistanceTraveled = 1200.000 maxNumOfFault = 6
numPlane = 2 avgtimeInFly = 1.667hour avgtimeInCharging = 0.000hour avgtimeInWait = 1.333hour totalDistanceTraveled = 600.000 maxNumOfFault = 4
numPlane = 4 avgtimeInFly = 1.724hour avgtimeInCharging = 0.300hour avgtimeInWait = 0.976hour totalDistanceTraveled = 413.733 maxNumOfFault = 4
company_frame_actual: 64800 == company_frame_expected: 64800 miss_frame = 0
company_frame_actual: 64800 == company_frame_expected: 64800 miss_frame = 0
company_frame_actual: 21600 == company_frame_expected: 21600 miss_frame = 0
company_frame_actual: 21600 == company_frame_expected: 21600 miss_frame = 0
company_frame_actual: 43200 == company_frame_expected: 43200 miss_frame = 0
total_miss_frame = 0
```

#### Lock-less_MultiThread:

```
2021-03 09:31:14
2021-03 09:34:18
183606ms 
numPlane = 6 avgtimeInFly = 1.847hour avgtimeInCharging = 0.480hour avgtimeInWait = 0.673hour totalDistanceTraveled = 5318.933 maxNumOfFault = 12
numPlane = 6 avgtimeInFly = 1.444hour avgtimeInCharging = 0.233hour avgtimeInWait = 1.323hour totalDistanceTraveled = 4331.528 maxNumOfFault = 18
numPlane = 2 avgtimeInFly = 1.250hour avgtimeInCharging = 0.800hour avgtimeInWait = 0.950hour totalDistanceTraveled = 1200.000 maxNumOfFault = 6
numPlane = 2 avgtimeInFly = 1.667hour avgtimeInCharging = 0.000hour avgtimeInWait = 1.333hour totalDistanceTraveled = 600.000 maxNumOfFault = 4
numPlane = 4 avgtimeInFly = 1.724hour avgtimeInCharging = 0.300hour avgtimeInWait = 0.976hour totalDistanceTraveled = 413.733 maxNumOfFault = 4
company_frame_actual: 64800 == company_frame_expected: 64800 miss_frame = 0
company_frame_actual: 64800 == company_frame_expected: 64800 miss_frame = 0
company_frame_actual: 21600 == company_frame_expected: 21600 miss_frame = 0
company_frame_actual: 21600 == company_frame_expected: 21600 miss_frame = 0
company_frame_actual: 43200 == company_frame_expected: 43200 miss_frame = 0
total_miss_frame = 0
```





## Reference

[1] Timothy L. Harris "**A Pragmatic Implementation of Non-Blocking Linked-Lists**". 2001. 

[2] Andrei Alexandrescu, Maged Michael  "**Lock-Free Data Structures with Hazard Pointers**". 2004.
Inter Process Communication
=======

Debug
-
- Build Debug *gtest.sln*
- Build Debug *protobuf.sln*
- Build Debug *ipc.sln*

Release
-
- Build *ipc.sln*


TODO
====
- Native <-> Managed
- Manged <-> Managed
- Tuning for small data (check DDE2 and tuning MMF/Pipes)
- Measure WM_COPYDATA to evaluate UI syncronization impact
- Pushing many small(1kb) real time data items to clients process
- Broadcasting data to 10 processes
- 400kb and 2000kb, 1kb and 1kb requests/responses
- Send 0.1kb, 1kb, 3kb one way

Conclusion
===
- When making few big data exchanges then IPC(syncronization and memory copying) is neglectable if to compare with object serialization to byte array. Better having simple objects (e.g. string or array of string) for IPC.
- Adhoc byte array serialization is viable solution because using objects translation to serializable(Protobuf) messages can happen to be unallowable overhead.
- Need find ways to optimize chaty conversations. E.g. tuning syncronization, IPC buffers, batching.

Measurments
===========
[Machine specification]
---

Client sending raw bytes via Shared Memory
---
Client process requests ~100kb and gets ~1000kb response of server process
  *  Avg. time `0.000800085 sec`
  *  Min. time 0.00100017 sec
  *  Max. time 0.00199986 sec
  *  Tot. time 0.00800085 sec
  *  Stops 10
  
Client process requests ~50kb and gets ~500kb response of server process
  *  Avg. time 0.000300038 sec
  *  Min. time 0 sec
  *  Max. time 0.0010004 sec
  *  Tot. time 0.00600076 sec
  *  Stops 20
  
Client process requests ~10kb and gets ~100kb response  of server process
  *  Avg. time 0.000110011 sec
  *  Min. time 0 sec
  *  Max. time 0.00100017 sec
  *  Tot. time 0.0110011 sec
  *  Stops 100

Client process requests ~1kb and gets ~10kb response  of server process
  *  Avg. time 6.40063e-005 sec
  *  Min. time 0 sec
  *  Max. time 0.0010004 sec
  *  Tot. time `0.0640063 sec`
  *  Stops 1000
  
  
Client sending raw bytes via Named Pipes
---

Client process requests ~100kb and gets ~1000kb response of server process
  *  Avg. time 0.00150018 sec
  *  Min. time 0.000999928 sec
  *  Max. time 0.00200057 sec
  *  Tot. time 0.0150018 sec
  *  Stops 10

Client process requests ~50kb and gets ~500kb response of server process
  *  Avg. time 0.000750077 sec
  *  Min. time 0.000999928 sec
  *  Max. time 0.00100017 sec
  *  Tot. time 0.0150015 sec
  *  Stops 20
Client process requests ~10kb and gets ~100kb response  of server process
  *  Avg. time 0.000130014 sec
  *  Min. time 0 sec
  *  Max. time 0.0010004 sec
  *  Tot. time 0.0130014 sec
  *  Stops 100

Client process requests ~1kb and gets ~10kb response  of server process
  *  Avg. time 5.70059e-005 sec
  *  Min. time 0 sec
  *  Max. time 0.0010004 sec
  *  Tot. time `0.0570059 sec`
  *  Stops 1000


 [Machine specification]: http://valid.canardpc.com/2639433
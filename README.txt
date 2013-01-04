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

Measurments
===========
[Machine specification]




[Machine specification]: http://valid.canardpc.com/2639433


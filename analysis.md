Inter Process Communication
===========================

Release
-------
- Build *ipc.sln*

Debug (do not use for measurements)
-----------------------------------
- Build Debug *gtest.sln*
- Build Debug *protobuf.sln*
- Build Debug *ipc.sln*

Considerations
==============
- Windows provides various IPC[1] mechanisms all based on shared memory[2].  It means that any custom mechanism cannot beat shared memory, but can provide different abstractions(e.g. remote procude call or stream reads/writes) and use different synchronization mechanisms(e.g. spin wait instead of lock) leading to different performances. COM/OLE/RPC additionaly use internal (A)LPC[8] mechanims which provides some improvments which can influnce speed of synronization and sending 256 byte messages.e
- Target machines can have only one hardware thread. Event having several processes can be similar to having one if second hardware thread is occupied with some task. Multithreading have is own impact - it can make thinks faster because several thinks done simultaneously,  but need to pay for synchronization. Number of channels of RAM can influence multithreaded situation. So all basic tests done synchronous with one sending process and one single threaded waiting process. Real multithread scenario can be estimated and tested after.
- Having request/response scenarios of different kind when client pull data from server many times by requests and when client makes single request then server pushes(streams) data many times. First test to have all synchronization and error checking, second minimum part of checks. 
- Requests and responses can have various sizes - several big request or many small requests.

Neglected measurements
======================
The maing goal is performance and possibility to tune it. Layers of abstractions (e.g. represeting shared memory reads/writes as generic object interface) decrease perfromance. Layring managed code above unmanged ads some penalty (marhaling). Need to measure reuquest/response scenarions and streaming even with minimum error checks. Second consideration is API should be usable from managed/unmanged code with small configuration hassle.
- Mailslots shows to be slow as shown by [3] and of customer teams reports. Provides one way communication which should be doubled to provide communication in other direction. Does to many things which are never be used by in considered scenarios.
- Sockets shows to be slow as shown by [3] and of customer teams reports. Provides hussle of choosing port[6] and potentialy can be blocket of security reasons (e.g. need administrator intervention to unblock).
- MSRPC uses Named Pipes, ALPC and local process memory(for in-proc calls). Documented usage is COM/C++ specific. There is wrappers for C#[9]. Providing transport independent function call abstraction and other stuff never good for workflows but not good for measuring raw performance at first hand.
- DDE is "not as efficient as newer technologies"[7], used Windows Messaging and Shared Memorory inside[4], considered legacy and not suggested for use for new projects[5].
- COM provides object layer above Shared Memory, ALPC and MSRPC, adds some marshaling to using it from managed code and configuration hussle. Good object API. 
- Windows Messages does not provide blocking calls abstractions and need heavy weight STA threads with hidden windows for multhireading. [3] shows that it is slower then named pipes or shared memory. Doing only streaming tests.
- When needed to support more workflow oriented scenarios need to consider COM and MSRPC(evaluate what C# abstractions provided, C#-C++ interop, streaming and synchronization), compare with raw API in perfromance. Potentilly doing streaming low level and other stuf throy aforemention APIs basing on perfromance measurements.

Conclusion
==========
- When making few big data exchanges then IPC(synchronization and memory copying) is neglectable if to compare with object serialization to byte array. Better having simple objects (e.g. string or array of string or with sequential memory layout) for IPC.
- Adhoc byte array serialization is viable solution because using objects translation to serializable (Protobuf) messages can happen to be unallowable overhead.
- Tests shows that batching of many requests( request ~100 bytes, response 1kb) into 2000 items bucket can improve IPC by 20-30%.
- Need some king of different API,  one for high perfromance low level for data pumping and another for workflows.

Measurments
===========
[Machine specification 1]
-----------------------


Client sending raw bytes via Shared Memory
---
Client process requests ~100kb and gets ~1000kb response of server process
  *  Avg. time **0.000800085 sec**
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
  *  Tot. time **0.0640063 sec**
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

  
Client sending adhoc serialized requests via Shared Memory and expecting adhoc objects response
---

Client process requests ~100kb and gets ~1000kb response of server process
  *  Avg. time 0.0214021 sec
  *  Min. time 0.00199986 sec
  *  Max. time 0.0370038 sec
  *  Tot. time 0.214021 sec
  *  Stops 10

Client process requests ~50kb and gets ~500kb response of server process
  *  Avg. time `0.0107511 sec`
  *  Min. time 0.00900078 sec
  *  Max. time 0.0180018 sec
  *  Tot. time 0.215022 sec
  *  Stops 20

Client process requests ~10kb and gets ~100kb response  of server process
  *  Avg. time 0.0019702 sec
  *  Min. time 0.000999928 sec
  *  Max. time 0.00800085 sec
  *  Tot. time 0.19702 sec
  *  Stops 100

Client process requests ~1kb and gets ~10kb response  of server process
  *  Avg. time 0.000246025 sec
  *  Min. time 0 sec
  *  Max. time 0.0010004 sec
  *  Tot. time 0.246025 sec
  *  Stops 1000

Client sending request via Shared Memory using objects translated into Protobuf messages, expecting messages and translating them back
---

Client process requests ~100kb and gets ~1000kb response of server process
  *  Avg. time 0.0342034 sec
  *  Min. time 0.00400043 sec
  *  Max. time 0.0560055 sec
  *  Tot. time 0.342034 sec
  *  Stops 10

Client process requests ~50kb and gets ~500kb response of server process
  *  Avg. time `0.0180518 sec`
  *  Min. time 0.0160017 sec
  *  Max. time 0.0330033 sec
  *  Tot. time 0.361036 sec
  *  Stops 20

Client process requests ~10kb and gets ~100kb response  of server process
  *  Avg. time 0.00326032 sec
  *  Min. time 0.00300002 sec
  *  Max. time 0.0160017 sec
  *  Tot. time 0.326032 sec
  *  Stops 100

Client process requests ~1kb and gets ~10kb response  of server process
  *  Avg. time 0.000384039 sec
  *  Min. time 0 sec
  *  Max. time 0.00200009 sec
  *  Tot. time 0.384039 sec
  *  Stops 1000

TODO
====
- Shuffle tests and shuffle data, investigate CPU cache influence
- Native <-> Managed
- Managed <-> Managed
- Evaluate UI syncronization impact
- Pushing many small(1kb) data items to clients process
- Send 0.1kb, 1kb, 3kb one way
- Broadcasting data to 10 processes
- 400kb and 2000kb, 1kb and 1kb requests/responses
- Make multithreaded tests (consider single chanell memory issues) with addint Windows Messaging
- Tuning syncronization to optimize chaty(many small requests) conversations
 
 [Machine specification 1]: http://valid.canardpc.com/2639433
 [1]: http://msdn.microsoft.com/en-us/library/windows/desktop/aa365574.aspx
 [2]: http://blogs.msdn.com/b/salvapatuel/archive/2009/06/08/working-with-memory-mapped-files-in-net-4.aspx
 [3]: https://onegazhang.wordpress.com/2008/05/28/fastest-ipc-method-on-windows-xpvista/
 [4]: http://msdn.microsoft.com/en-us/library/windows/desktop/ms648774.aspx
 [5]: http://ndde.codeplex.com/
 [6]: http://stackoverflow.com/questions/8748396/ipc-port-ranges
 [7]: http://msdn.microsoft.com/en-us/library/windows/desktop/aa365574.aspx#base.using_dde_for_ipc
 [8]: http://en.wikipedia.org/wiki/Local_Procedure_Call
 [9]: http://csharptest.net/projects/rpclibrary/
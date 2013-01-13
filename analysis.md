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
- Need some king of different API, one for high perfromance low level one way data pumping and another for workflows.

Measurments
===========
[Machine specification 1]
-----------------------

TODO
====
- Shuffle tests and shuffle data, investigate CPU cache influence
- Native <-> Managed
- Managed <-> Managed
- Evaluate UI syncronization impact
- Send 0.1kb 3kb one way
- Broadcasting data to 10 processes
- 400kb and 2000kb, 1kb and 1kb requests/responses
- Make multithreaded tests (consider single chanell memory issues)
- Tuning syncronization to optimize chaty(many small requests) conversations, spin vs lock
 
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
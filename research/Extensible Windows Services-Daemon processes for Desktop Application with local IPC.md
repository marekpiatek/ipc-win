Problem: We need  common workflows  for desktop applications suite consisting of many executable processes. Common things can be authentication, cross-process event exchange and web data refresh workflows. Custom workflows could be plugged and removed on demand. Services loaded into one process provided for usage for other processes.


Solution:


Daemon[1] - computer program that runs in the background, rather than under the direct control of a user.  Daemon is solution for such task. On windows default implementation of it is called Windows Service.

Possible constrains/requirements:

Next items could be considered and question during chossing righ techoloy.

    C++ host integration. If it is C++ tech, will it integrate with C# later? Eventually to innovate fast you need such integration.
    C# host integration. If it is .NET tech, will it integrate with C++ later? Eventually to write good Desktop apps for unmanages OS you should write unmanaged code.
    Reg Free/XCOPY. Should our software be installed and run with restricted user rights? Is desktop wide registration in some windows service used? Do we have registry access rights? How ready we are to solve dirty registry issues? Can we copy archives and things work?
    IPC Threading Models. Current laptops have 4 hardware threads. My laptop has 8 hardware threads. Will chosen technology employ this fact to improve perfromance of application?
    Multi Process. Does technology allows isolation of code using several slave subprocesses? If it supports, then how much resources consumed by each new process?
    C++ UI.
    C# UI.
    Lightweight. How much memory will add up each new component?
    Fast IPC.
    C++ IPC.
    C# IPC.
    Fast start. How often user open and closes your apps? Is possible to have several applications running?
    Isolation. How rights to access system wide resources are managed for loaded code? What are possibilities?
    Versatility for many scenarios.
    Malleability. Can we change or hack host for our needs?
    Coexistence with others. Is running several similar Daemons possible? Can do Daemons interop? Can update Daemon version easily?

Possible techs:

CLR Inside Out: .NET Application Extensibility , Add-ins and Extensibility[2] - System.AddIn library used for managed add-ins loaded into separate memory isolation(libraries and staticfileds- AppDomains). Employed by Visual Studio Tools for Office (VSTO). Not provides means to work with native(C++ based) add-ins. Provides means for many possible isolation ways, need to investigate for isolation free way for trusted add-ins. Has good technique for versioning which allows not to break backward compatibility for a very long time.

AddInProcess Class (System.AddIn.Hosting)[3]  Provides an external process for running add-ins out-of-process from the host application. Daemon is AddInProcess in terms of  System.AddIn. Remoting IPC is used.

WPF Add-Ins Overview[4] - allows Windows Presentation Foundation (WPF) UI componets to be integrated with System.AddIn add-ins. Similar to Object Linking and Embedding (OLE) but for managed code. Enforces usage of System.AddIn and no integration with native UI.

Topshelf/Topshelf · GitHub - open source Daemon, good choice to start look at instead of ad-hoc daemon creation. Not sure what IPC provided, may by Remoting.

System.ServiceProcess Namespace () - Windows Service is like Daemon, requires power user rights to be installed and integrated with Windows Management for Services.

Out-of-process COM EXE server, Dll Surrogate, ServicedComponent -  by default needs registration and single thread with UI message pump. Can be tuned for Reg-Free COM, custom Activation/Loading, runtime registration, usage of Running Object Table, needs investigation to use early binding and employ many user desktop cores. Has C++/C# compatbile IPC build in.


Internet Explorer and Chromium - modern browsers are generic reusable hosts for isolated heterogeneous code.



Evaluation grid see in `daemon-ipc-addins-win.csv` follows repertoires grid.




I can write my own daemon!

Yes it is possible, but you will meet next issues:

- if you will met another daemon written by other party need to work with then will end up with multiplied memory overhead to initialize several daemon processes

- less possibly to improve start up time because less daemon clients leading to less possibly of daemon already running

- will end up with need of many daemons synchronization with each other

- if several processes can run daemon then need to do start up synchronization

- if daemon crashed or killed by user then need restore daemon with all previous connection for clients

- need to control background daemon behavioral, e.g. it should not consume much memory when not used, e.g. should not do web requests and show message boxes


Other solutions:

There can be no daemon in some cases.

Peer to Peer(p2p)  and Volatile Master Process

Volatile Master processs that executes common workflow and interacts for coordination tasks with other waiting processes. Master is volatile, can be killed, and other process start to be Master.

P2P - having several process executing some code and sync using named events and shared memory.


Future 

Ideally some COM/OLE vNEXT from MS should include:

    Many processes with good isolation,or if managed language (JS or C#) then memory isolation in one process).
    Many cores and much outbound networking IO, means many patters of synchronization available.
    Graphics card based user interfaces composition (HTML5 and XAML)
    Native, managed and scripted languages (C++, C#, JS).
    New local IPC mechanism without networking garbage in internal implementation and external declaration with semantics similar to WCF contracts. (C++ 11 has attributes in VS2010 and can generate IL in Win8 to support compile time WCF contracts).


 

[1]: http://en.wikipedia.org/wiki/Daemon_(computing)
[2]: https://docs.microsoft.com/en-us/dotnet/framework/add-ins/index
[3]: https://docs.microsoft.com/en-us/dotnet/api/system.addin.hosting.addinprocess
[4]: https://docs.microsoft.com/en-us/dotnet/framework/wpf/app-development/wpf-add-ins-overview
[5]: https://github.com/Topshelf/Topshelf/
[6]: https://docs.microsoft.com/en-us/dotnet/api/system.serviceprocess
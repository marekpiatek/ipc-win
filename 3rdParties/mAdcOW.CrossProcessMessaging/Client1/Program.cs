using System;
using System.ServiceModel;
using System.ServiceModel.Channels;
using System.ServiceModel.Description;
using System.Threading;

namespace mAdcOW.Server
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            Console.WriteLine("Server");

            NamedPipeServer ns = new NamedPipeServer();
            ns.Run();
            ns.Run();
            ns.Run();
            //return;


            Thread wcfThread = new Thread(WCFServer);
            wcfThread.Start();

            MessageServer msg = new MessageServer("yooba");
            msg.BufferSize = 256*1024;

            Thread t = new Thread(msg.Monitor);
            t.Start();

            Console.ReadLine();
            Console.WriteLine("done");
            msg.Close();
            wcfThread.Abort();
        }

        static void WCFServer()
        {
            ServiceHost host = new ServiceHost(typeof(RemoteObject), new Uri("http://localhost:8001/Demo/"));
            ServiceMetadataBehavior smb = new ServiceMetadataBehavior();
            smb.HttpGetEnabled = true;
            smb.HttpGetUrl = new Uri("http://localhost:8001/Demo/Meta");
            host.Description.Behaviors.Add(smb);
            WSHttpBinding ws = new WSHttpBinding(SecurityMode.None);
            Binding mex = MetadataExchangeBindings.CreateMexHttpBinding();
            host.AddServiceEndpoint("WCFLib.IRemoteObject", ws, "http://localhost:8001/Demo/DoStuff"); //one to actually do things
            host.AddServiceEndpoint(typeof(IMetadataExchange), mex, "http://localhost:8001/Demo/Meta"); //and one to provide metadata

            NetNamedPipeBinding pipe = new NetNamedPipeBinding(NetNamedPipeSecurityMode.None);
            pipe.MaxReceivedMessageSize = 2147483647;
            pipe.ReaderQuotas.MaxArrayLength = 2147483647;
            pipe.ReaderQuotas.MaxBytesPerRead = 2147483647;
            host.AddServiceEndpoint("WCFLib.IRemoteObject", pipe, "net.pipe://localhost/Demo/DoStuff");


            host.Open();
            Console.WriteLine("Server Started");
            Console.ReadLine();
            host.Close();

            //using (ServiceHost host = new ServiceHost(typeof(RemoteObject)))
            //{
            //    host.Open();
            //    Console.WriteLine("Server Started");
            //    Console.ReadLine();
            //    host.Close();
            //}
        }
    }
}
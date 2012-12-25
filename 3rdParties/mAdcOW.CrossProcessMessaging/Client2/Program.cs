using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using Client2.WCFDemo;

namespace Client2
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Client");

            //Console.WriteLine("Press a key to start WCF sending");
            //Console.ReadKey();
            //WCFSending(128);
            //WCFSending(4 * 1024);
            //WCFSending(256 * 1024);
            //WCFSending(128);
            //WCFSending(4 * 1024);
            //WCFSending(256 * 1024);
            //return;

            NamedPipeClient nc = new NamedPipeClient();
            nc.BufferSize = 256*1024;
            nc.Run();
            nc.Run();
            nc.Run();
            //return;

            MessageClient msg = new MessageClient("yooba");

            ConsoleKeyInfo consoleKeyInfo;
            do
            {
                //msg.BufferSize = 4 * 1024;
                msg.BufferSize = 256*1024;
                Thread t = new Thread(msg.Send);
                t.Start();
                Console.WriteLine("Press a key to start shared mem sending");
                consoleKeyInfo = Console.ReadKey();
                msg.Signal();
                t.Join();

            } while (consoleKeyInfo.Key != ConsoleKey.Q);

        }

        private static void WCFSending(int bufferLength)
        {
            WCFDemo.RemoteObjectClient client = new RemoteObjectClient("NetNamedPipeBinding_IRemoteObject");
            byte[] buffer = new byte[bufferLength];
            client.ReceiveRBytes(buffer);

            var stopWatch = Stopwatch.StartNew();
            for (int i = 0; i < 50000; i++)
            {
                client.ReceiveRBytes(buffer);
            }
            stopWatch.Stop();
            Console.WriteLine("Time used: {0}\tPer sec: {1} - {2}", stopWatch.Elapsed, (50000 * 1000) / stopWatch.ElapsedMilliseconds, bufferLength);
        }
    }
}

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Text;
using System.Threading;

namespace Client2
{
    class NamedPipeClient
    {
        public int BufferSize { get; set; }      

        public void Run()
        {
            using (NamedPipeServerStream pipeServer = new NamedPipeServerStream("testpipe", PipeDirection.Out, 4, PipeTransmissionMode.Byte, PipeOptions.None))
            {
                Console.WriteLine("NamedPipeServerStream object created.");

                // Wait for a client to connect
                Console.Write("Waiting for client connection...");
                pipeServer.WaitForConnection();

                Console.WriteLine("Client connected.");
                Console.ReadLine();
                try
                {
                    byte[] message = new byte[BufferSize];
                    using (BinaryWriter sw = new BinaryWriter(pipeServer))
                    {
                        const int num = 50000;
                        var stopWatch = Stopwatch.StartNew();
                        for (int i = 0; i < num; i++)
                        {
                            sw.Write(message);
                        }
                        stopWatch.Stop();
                        Console.WriteLine("Time used: {0}\tPer sec: {1}", stopWatch.Elapsed, (num * 1000) / stopWatch.ElapsedMilliseconds);
                    }
                }
                // Catch the IOException that is raised if the pipe is 
                // broken or disconnected.
                catch (IOException e)
                {
                    Console.WriteLine("ERROR: {0}", e.Message);
                }
            }
        }
    }
}


using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Text;

namespace mAdcOW.Server
{
    class NamedPipeServer
    {
        public void Run()
        {
            using (NamedPipeClientStream pipeClient = new NamedPipeClientStream(".", "testpipe", PipeDirection.In, PipeOptions.None))
            {
                // Connect to the pipe or wait until the pipe is available.
                Console.Write("Attempting to connect to pipe...");
                pipeClient.Connect();

                Console.WriteLine("Connected to pipe.");
                Console.WriteLine("There are currently {0} pipe server instances open.", pipeClient.NumberOfServerInstances);
                using (BinaryReader sr = new BinaryReader(pipeClient))
                {
                    // Display the read text to the console
                    byte[] temp = new byte[1024*256];
                    int count = 0;
                    while ((sr.Read(temp,0,temp.Length)) > 0 )
                    {
                        //Console.Write(".");
                        count++;
                    }
                    Console.WriteLine("Read {0} messages", count);
                    //while ((temp = sr.ReadLine()) != null)
                    //{
                    //    Console.WriteLine("Received from server: {0}", temp);
                    //}
                }
            }
            Console.Write("Press Enter to continue...");
            Console.ReadLine();
        }
    }
}

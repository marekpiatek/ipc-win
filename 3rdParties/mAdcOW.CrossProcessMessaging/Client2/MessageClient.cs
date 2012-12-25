using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.MemoryMappedFiles;
using System.Linq;
using System.Text;
using System.Threading;

/*
http://msdn.microsoft.com/en-us/library/bb310550.aspx
http://www.theserverside.net/blogs/thread.tss?thread_id=45078

 */

namespace Client2
{
    class MessageClient
    {
        private MemoryMappedFile _mmf;
        private EventWaitHandle _messageWait;
        private EventWaitHandle _messageHandled;
        public int BufferSize { get; set; }

        public MessageClient(string commName)
        {
            _mmf = MemoryMappedFile.CreateOrOpen("mmf" + commName, 256*1024);
            _messageWait = new EventWaitHandle(false, EventResetMode.AutoReset, "wait" + commName);
            _messageHandled = new EventWaitHandle(false, EventResetMode.AutoReset, "handled" + commName);
        }

        public void Send()
        {
            _messageHandled.WaitOne();
            byte[] message = new byte[BufferSize];
            //using (var viewAccessor = _mmf.CreateViewAccessor())
            using(var viewStream = _mmf.CreateViewStream())
            {
                const int num = 50000;
                var stopWatch = Stopwatch.StartNew();
                for (int i = 0; i < num; i++)
                {
                    //stopWatch.Stop();
                    //FillArray(i, message);
                    //stopWatch.Start();
                    viewStream.Position = 0;
                    viewStream.Write(message, 0, BufferSize);
                    WaitHandle.SignalAndWait(_messageWait, _messageHandled);
                }
                stopWatch.Stop();
                Console.WriteLine("Time used: {0}\tPer sec: {1}", stopWatch.Elapsed, (num*1000)/stopWatch.ElapsedMilliseconds);
            }
        }

        private void FillArray(int val, byte[] message)
        {
            for (int i = 0; i < message.Length; i++)
            {
                message[i] = (byte)val;
            }
        }

        public void Close()
        {
            _mmf.Dispose();
            _messageHandled.Set();
            _messageHandled.Dispose();
            _messageWait.Dispose();
        }

        public void Signal()
        {
            _messageHandled.Set();
        }
    }
}

using System;
using System.IO.MemoryMappedFiles;
using System.Threading;

namespace mAdcOW.Server
{
    class MessageServer
    {
        private MemoryMappedFile _mmf;
        private EventWaitHandle _messageWait;
        private EventWaitHandle _messageHandled;
        public int BufferSize { get; set; }

        private bool _quit;

        public MessageServer(string commName)
        {
            _mmf = MemoryMappedFile.CreateOrOpen("mmf" + commName, 256 * 1024);
            _messageWait = new EventWaitHandle(false, EventResetMode.AutoReset, "wait" + commName);
            _messageHandled = new EventWaitHandle(false, EventResetMode.AutoReset, "handled" + commName);
        }

        public void Monitor()
        {
            byte[] message = new byte[BufferSize];
            //var viewAccessor = _mmf.CreateViewAccessor();
            var viewStream = _mmf.CreateViewStream(0, BufferSize);
            //_messageWait.WaitOne();
            Console.WriteLine("Monitor started");
            while (!_quit)
            {
                _messageWait.WaitOne();
                if (_quit) break;                
                viewStream.Position = 0;
                viewStream.Read(message, 0, BufferSize);
                _messageHandled.Set();
            }
        }

        public void Close()
        {
            _quit = true;
            _mmf.Dispose();
            _messageWait.Set();
            _messageHandled.Dispose();
            _messageWait.Dispose();
        }
    }
}



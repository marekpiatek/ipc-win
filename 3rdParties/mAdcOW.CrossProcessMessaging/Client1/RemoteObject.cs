using System;
using WCFLib;

namespace mAdcOW.Server
{
    class RemoteObject : IRemoteObject
    {
        public byte[] GetRBytes(int numBytes)
        {
            return new byte[numBytes];
        }

        public bool ReceiveRBytes(byte[] bytes)
        {
            return true;
        }
    }
}



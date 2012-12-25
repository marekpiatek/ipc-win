using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;

namespace WCFLib
{
    [ServiceContract]
    public interface IRemoteObject
    {
        [OperationContract(IsOneWay = false)]
        byte[] GetRBytes(int numBytes);

        [OperationContract(IsOneWay = false)]
        bool ReceiveRBytes(byte[] bytes);
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace ManagedServer
{
    class Program
    {
        static void Main(string[] args)
        {
        }
    }

    [ComVisible(true)]
    public class Test {
        public Test2 Get()
        {
            return new Test2();
        }
    }
    [ComVisible(true)] [Serializable] public class Test2
    {
        public int P1 { get; set; }
        public string P2 { get; set; }
    }
}

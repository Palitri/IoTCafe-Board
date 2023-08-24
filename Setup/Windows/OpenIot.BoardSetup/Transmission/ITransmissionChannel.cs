using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenIoT.BoardSetup.Transmission
{
    public interface ITransmissionChannel
    {
        void Write(byte[] buffer, int offset, int length);
        int Read(byte[] buffer, int offset, int length);

        bool Open();
        bool Close();

        bool IsOpened();
    }
}

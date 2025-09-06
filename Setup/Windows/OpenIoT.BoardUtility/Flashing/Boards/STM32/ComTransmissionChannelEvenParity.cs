using Palitri.OpenIoT.Board.Transmission;
using System.IO.Ports;

namespace Palitri.OpenIoT.BoardUtility.Flashing.Boards.STM32
{
    public class ComTransmissionChannelEvenParity : ITransmissionChannel
    {
        public bool IsOpen()
        {
            return serialPort.IsOpen;
        }

        public string Name { get; private set; }

        private SerialPort serialPort;

        public ComTransmissionChannelEvenParity(string comPortName, int baudRate = 9600)
        {
            Name = comPortName;

            serialPort = new SerialPort();

            serialPort.PortName = Name;
            serialPort.BaudRate = baudRate;
            serialPort.Parity = Parity.Even; // None
            serialPort.DataBits = 8;
            serialPort.StopBits = StopBits.One;
            serialPort.Handshake = Handshake.None;
            serialPort.ReadTimeout = 500;
            serialPort.WriteTimeout = 500;
        }

        public void Write(byte[] buffer, int offset, int length)
        {
            serialPort.Write(buffer, offset, length);
        }

        public int Read(byte[] buffer, int offset, int length)
        {
            if (serialPort.BytesToRead > 0)
                return serialPort.Read(buffer, offset, length);

            return 0;
        }

        public bool Open()
        {
            serialPort.Open();
            return true;
        }

        public bool Close()
        {
            serialPort.Close();
            return true;
        }

        public bool IsOpened()
        {
            return serialPort.IsOpen;
        }

        public static IEnumerable<string> GetAvailablePorts()
        {
            return SerialPort.GetPortNames();
        }
    }
}

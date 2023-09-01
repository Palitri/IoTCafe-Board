using OpenIoT.BoardSetup.Transmission;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace OpenIot.BoardSetup.Flashing.Boards.STM32
{
    internal class STM32F103CFlashProgrammer : IFlashProgrammer
    {
        private static byte[] commands = new byte[]
        {
            0x7f,           // GetDataFrame
            0x01, 0xfe,     // GetVersionAndProtectionStatus
            0x00, 0xff,     // GetVersionAndSupportedCommands
            0x02, 0xfd,     // GetChipId
            0x43, 0xbc,     // EraseFlash
            0xff, 0x00,     // GlobalEraseFlash
            0x31, 0xce,     // WriteFlash
            0x21, 0xde      // G0
        };

        private static byte Response_ACK = 0x79;

        private byte[] programData;
        private string comPort;
        private ComTransmissionChannel serial;
        private IFlashLog log;

        private Stopwatch idleTimer;
        private byte[] buffer;

        private byte bootloaderVersion;
        private byte[] supportedCommands;
        private byte[] chipId;

        public static string BoardName = "STM32F103C";

        public event BeganFlashingDelegate BeganFlashing;

        // https://forum.arduino.cc/t/uploading-a-sketch-from-a-binary-file/637085/3
        // https://www.st.com/content/ccc/resource/technical/document/application_note/51/5f/03/1e/bd/9b/45/be/CD00264342.pdf/files/CD00264342.pdf/jcr:content/translations/en.CD00264342.pdf
        public STM32F103CFlashProgrammer(string comPort, IFlashLog log)
        {
            programData = File.ReadAllBytes(@"Boards\\STM32F103C\\OpenIoT.ino.generic_stm32f103c.bin");
            this.comPort = comPort;
            this.log = log;

            idleTimer = new Stopwatch();
            buffer = new byte[258];
            supportedCommands = Array.Empty<byte>();
            chipId = Array.Empty<byte>();
        }

        public async Task<bool> FlashBoardAsync()
        {
            return await Task.Run(() => FlashBoard());
        }

        public bool FlashBoard()
        {
            try
            {
                log.Log("Initiating programming sequence on STM32F103C, port " + comPort);

                serial = new ComTransmissionChannel(comPort, 115200);
                serial.Open();

                if (!SendDataFrame())
                    return false;

                if (!GetVersionAndSupportedCommands())
                    return false;

                if (!GetVersionAndProtectionStatus())
                    return false;

                if (!GetChipId())
                    return false;

                if (!EraseFlash())
                    return false;

                BeganFlashing?.Invoke();

                if (!WriteFlash(programData, 0, programData.Length, 0x08000000))
                    return false;

                if (!Go(0x08000000))
                    return false;

                log.Log("Programming sequence completed");

                return true;
            }
            finally
            {
                serial.Close();
            }
        }

        private void SendData(byte[] source, int offset, int length)
        {
            serial.Write(source, offset, length);
        }

        private int ReadData(byte[] destination, int offset, int length, int timeoutMillis = 100)
        {
            idleTimer.Restart();
            int totalRead = 0;
            while (length > 0)
            {
                int read = serial.Read(destination, offset, length);

                totalRead += read;
                offset += read;
                length -= read;

                if (idleTimer.ElapsedMilliseconds > timeoutMillis)
                    break;

            }

            return totalRead;
        }

        private int ReadData(int length, int timeoutMillis = 100)
        {
            return ReadData(buffer, 0, length);
        }

        private byte GetChecksum(byte[] data, int offset, int length)
        {
            byte result = 0;
            int end = offset + length;
            for (int i = offset; i < end; i++)
                result ^= data[i];

            return result;
        }

        private bool ReadACK()
        {
            bool result = false;

            if (ReadData(1) == 1)
                result = buffer[0] == Response_ACK;

            if (!result)
                log.Log("Error receiving acknowledgement from device");

            return result;
        }

        private bool SendDataFrame()
        {
            log.Log("Performing data frame handshake");

            SendData(commands, 0, 1);
            Thread.Sleep(10);
            return ReadACK();
        }

        //Byte 1: ACK
        //Byte 2: Bootloader version (0 < version  255), example: 0x10 = version 1.0
        //Byte 3: Option byte 1: 0x00 to keep the compatibility with generic bootloader protocol
        //Byte 4: Option byte 2: 0x00 to keep the compatibility with generic bootloader protocol
        //Byte 5: ACK
        private bool GetVersionAndProtectionStatus()
        {
            log.Log("Retrieving device version: ", false);

            SendData(commands, 1, 2);

            if (!ReadACK())
                return false;

            ReadData(3);
            bootloaderVersion = buffer[0];

            log.Log(bootloaderVersion.ToString("X2"), true);

            if (!ReadACK())
                return false;

            return true;
        }

        //Byte 1: ACK
        //Byte 2: N = 11 = the number of bytes to follow – 1 except current and ACKs.
        //Byte 3: Bootloader version (0 < version< 255), example: 0x10 = version 1.0
        //Byte 4: 0x00 Get command
        //Byte 5: 0x01 Get Version and Read Protection Status
        //Byte 6: 0x02 Get ID
        //Byte 7: 0x11 Read Memory command
        //Byte 8: 0x21 Go command
        //Byte 9: 0x31 Write Memory command
        //Byte 10: 0x43 or 0x44 Erase command or Extended Erase command (exclusive commands)
        //Byte 11: 0x63 Write Protect command
        //Byte 12: 0x73 Write Unprotect command
        //Byte 13: 0x82 Readout Protect command
        //Byte 14: 0x92 Readout Unprotect command
        //Byte 15: 0xA1 Get Checksum command(only for version V3.3)
        private bool GetVersionAndSupportedCommands()
        {
            log.Log("Retrieving device commands: ", false);

            SendData(commands, 3, 2);

            if (!ReadACK())
                return false;

            if (ReadData(2) != 2)
                return false;
            int responseLength = buffer[0] + 1;
            bootloaderVersion = buffer[1];


            supportedCommands = new byte[responseLength - 1];
            if (ReadData(supportedCommands, 0, responseLength - 1) != responseLength - 1)
                return false;

            log.Log(string.Join(' ', supportedCommands.Select(id => id.ToString("X2"))), true);

            if (!ReadACK())
                return false;

            return true;
        }

        //Byte 1: ACK
        //Byte 2: N = the number of bytes – 1 (N = 1 for STM32), except for current byte and ACKs.
        //Byte 3-4: PID(1) byte 3 = 0x04, byte 4 = 0xXX
        //Byte 5: ACK
        private bool GetChipId()
        {
            log.Log("Retrieving chip ID: ", false);

            SendData(commands, 5, 2);

            if (!ReadACK())
                return false;

            if (ReadData(1) != 1)
                return false;
            int responseLength = buffer[0] + 1;

            chipId = new byte[responseLength];
            if (ReadData(chipId, 0, responseLength) != responseLength)
                return false;

            log.Log(string.Join(null, chipId.Select(id => id.ToString("X2"))), true);

            if (!ReadACK())
                return false;

            return true;
        }

        private bool EraseFlash()
        {
            log.Log("Erasing flash memory");

            SendData(commands, 7, 2);

            if (!ReadACK())
                return false;

            SendData(commands, 9, 2);

            if (!ReadACK())
                return false;

            return true;
        }

        private bool WriteFlashBlock(byte[] data, int offset, int length, int address)
        {
            log.Log(string.Format("Flashing block {0} - {1}", address.ToString("X8"), (address + length - 1).ToString("X8")));

            SendData(commands, 11, 2);

            if (!ReadACK())
                return false;

            for (int i = 3; i >= 0; i--)
            {
                buffer[i] = (byte)address;
                address >>= 8;
            }
            buffer[4] = GetChecksum(buffer, 0, 4);
            SendData(buffer, 0, 5);

            if (!ReadACK())
                return false;

            buffer[0] = (byte)(length - 1);
            Array.Copy(data, offset, buffer, 1, length);
            if (length % 4 != 0)
                length = (length / 4 + 1) * 4;
            buffer[length + 1] = GetChecksum(buffer, 0, length + 1);
            SendData(buffer, 0, length + 2);

            if (!ReadACK())
                return false;

            return true;
        }

        private bool WriteFlash(byte[] data, int offset, int length, int address)
        {
            log.Log("Writing program data");

            while (length > 0)
            {
                int blockSize = Math.Min(length, 256);
                if (!WriteFlashBlock(data, offset, blockSize, address))
                {
                    return false;
                }

                offset += blockSize;
                address += blockSize;
                length -= blockSize;
            }

            log.Log("Program data written");

            return true;
        }

        private bool Go(int address)
        {
            log.Log("Starting program");

            SendData(commands, 13, 2);

            if (!ReadACK())
                return false;

            for (int i = 3; i >= 0; i--)
            {
                buffer[i] = (byte)address;
                address >>= 8;
            }
            buffer[4] = GetChecksum(buffer, 0, 4);
            SendData(buffer, 0, 5);

            if (!ReadACK())
                return false;

            return true;
        }
    }
}



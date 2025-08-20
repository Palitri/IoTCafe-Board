using Palitri.OpenIoT.BoardFlash;
using Palitri.OpenIoT.BoardFlash.Flashing.Boards.STM32;
using Palitri.OpenIoT.Setup.Shared.Flashing;

namespace Palitri.OpenIoT.BoardSetup;

class Program
{
    static void Main(string[] args)
    {
        const StringComparison stringComparison = StringComparison.OrdinalIgnoreCase;

        //args = new string[] { "--flash", "STM32", "COM4", "Firmware.bin" };

        if (args.Length == 0)
        {
            PrintGeneralInformation();
            return;
        }

        if (args.Length > 3)
        {
            if (args[0].Equals("--flash", stringComparison))
            {
                if (args[1].Equals("STM32"))
                {
                    string port = args[2];
                    string firmwareFile = args[3];

                    IFlashProgrammer programmer = new STM32F103CFlashProgrammer(port, firmwareFile, new ConsoleLog());

                    Task.Run(async () =>
                    {
                        await programmer.FlashBoardAsync();
                    }).Wait();
                }
            }
        }
    }

    static void PrintGeneralInformation()
    {
        Console.WriteLine("OpenIoT Board Flashing utility v0.89");
        Console.WriteLine();
        Console.WriteLine("Usage:");
        Console.WriteLine("--flash <BoardName> <Port> <FirmwareFileName>");
        Console.WriteLine();
        Console.WriteLine("Example:");
        Console.WriteLine("--flash STM32 COM3 firmware.bin");
    }
}
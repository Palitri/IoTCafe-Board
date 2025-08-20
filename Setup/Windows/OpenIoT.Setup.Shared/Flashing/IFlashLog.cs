namespace Palitri.OpenIoT.Setup.Shared.Flashing
{
    public interface IFlashLog
    {
        void Log(string message, bool newLine = true, FlashMessageType messageType = FlashMessageType.Info, int messageId = 0);
    }
}

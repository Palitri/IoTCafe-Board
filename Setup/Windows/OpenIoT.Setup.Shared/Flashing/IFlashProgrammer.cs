namespace Palitri.OpenIoT.Setup.Shared.Flashing
{
    public delegate void BeganFlashingDelegate();
    public delegate void MessageReceivedDelegate(string message, FlashMessageType messageType);

    public interface IFlashProgrammer
    {
        Task<bool> FlashBoardAsync();

        event BeganFlashingDelegate BeganFlashing;
        event MessageReceivedDelegate MessageReceived;
    }
}

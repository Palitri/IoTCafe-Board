namespace Palitri.OpenIoT.Setup.Shared.Flashing
{
    public class FlashProgrammersList : List<IFlashProgrammer>, IFlashProgrammer
    {
        public bool BreakOnFailure { get; private set; } = false;

        public event BeganFlashingDelegate BeganFlashing;
        public event MessageReceivedDelegate MessageReceived;

        private bool hasBeganFlashing = false;

        public async Task<bool> FlashBoardAsync()
        {
            bool result = true;

            foreach (IFlashProgrammer programmer in this)
            {
                programmer.BeganFlashing += this.OnBeganItemFlashing;
                programmer.MessageReceived += this.OnMessageReceived;

                bool success = await programmer.FlashBoardAsync();
                result &= success;

                programmer.BeganFlashing -= this.OnBeganItemFlashing;
                programmer.MessageReceived -= this.OnMessageReceived;

                if (!success && this.BreakOnFailure)
                    return false;
            }

            return result;
        }

        public virtual void OnMessageReceived(string message, FlashMessageType messageType)
        {
            this.MessageReceived?.Invoke(message, messageType);
        }

        protected void OnBeganItemFlashing()
        {
            if (this.hasBeganFlashing)
                return;

            this.hasBeganFlashing = true;

            this.BeganFlashing?.Invoke();
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.Flashing
{
    internal class FlashProgrammersList : List<IFlashProgrammer>, IFlashProgrammer
    {
        public bool BreakOnFailure { get; private set; } = false;

        public event BeganFlashingDelegate BeganFlashing;

        private bool hasBeganFlashing = false;

        public async Task<bool> FlashBoardAsync()
        {
            foreach (IFlashProgrammer programmer in this)
            {
                programmer.BeganFlashing += this.OnBeganItemFlashing;

                bool success = await programmer.FlashBoardAsync();

                programmer.BeganFlashing -= this.OnBeganItemFlashing;

                if (!success && this.BreakOnFailure)
                    return false;
            }

            return true;
        }

        public void MessageReceived(string message, FlashMessageType messageType)
        {

        }

        private void OnBeganItemFlashing()
        {
            if (this.hasBeganFlashing)
                return;

            this.hasBeganFlashing = true;

            this.BeganFlashing?.Invoke();
        }
    }
}

namespace Palitri.OpenIoT.Setup.Shared.Flashing
{
    public class CommandLineProgrammer : FlashProgrammersList
    {
        public CommandLineProgrammer(IFlashLog log, IEnumerable<string> commands, string firmwaresDownloadDirectory)
        {
            FlashProgrammersList programmer = new FlashProgrammersList();

            foreach (string command in commands)
            {
                List<string> commandArgs = SplitCommand(command, true);
                var exeName = commandArgs[0];
                var args = commandArgs.Take(new Range(1, commandArgs.Count)).ToList();

                this.Add(new ExternalFlashProgrammer(log, firmwaresDownloadDirectory, exeName, string.Join(" ", args.Select(a => "\"" + a + "\""))));
            }
        }

        public override void OnMessageReceived(string message, FlashMessageType messageType)
        {
            base.OnMessageReceived(message, messageType);

            string[] beganFlashingKeywords = { "Erasing", "Writing" };

            foreach (var keyword in beganFlashingKeywords)
            {
                if (message.Contains(keyword, StringComparison.OrdinalIgnoreCase))
                {
                    this.OnBeganItemFlashing();
                    break;
                }
            }
        }

        public static List<string> SplitCommand(string command, bool removeQuotations = false)
        {
            List<string> result = new List<string>();
            int start = 0;
            bool quoted = false;

            for (int i = 0; i < command.Length; i++)
            {
                char c = command[i];
                if (c == '\"')
                    quoted = !quoted;

                if (c == ' ' && !quoted)
                {
                    result.Add(command.Substring(start, i - start));
                    start = i + 1;
                }
            }

            result.Add(command.Substring(start));

            result.RemoveAll(c => string.IsNullOrWhiteSpace(c));

            if (removeQuotations)
                result = result.Select(c => c.Trim('\"')).ToList();

            return result;
        }
    }
}
